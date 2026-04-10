// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "bobuiextdocumentlayout_p.h"
#include "bobuiextdocument_p.h"
#include "bobuiextimagehandler_p.h"
#include "bobuiexttable.h"
#include "bobuiextlist.h"
#include "bobuiextengine_p.h"
#if BOBUI_CONFIG(cssparser)
#include "private/qcssutil_p.h"
#endif
#include "private/qguiapplication_p.h"

#include "qabstracttextdocumentlayout_p.h"
#include "qcssparser_p.h"

#include <qpainter.h>
#include <qmath.h>
#include <qrect.h>
#include <qpalette.h>
#include <qdebug.h>
#include <qvarlengtharray.h>
#include <limits.h>
#include <qbasictimer.h>
#include "private/qfunctions_p.h"
#include <qloggingcategory.h>
#include <BobUICore/qpointer.h>

#include <algorithm>

BOBUI_BEGIN_NAMESPACE

Q_STATIC_LOGGING_CATEGORY(lcDraw, "bobui.text.drawing")
Q_STATIC_LOGGING_CATEGORY(lcHit, "bobui.text.hittest")
Q_LOGGING_CATEGORY(lcLayout, "bobui.text.layout")
Q_STATIC_LOGGING_CATEGORY(lcTable, "bobui.text.layout.table")

// ################ should probably add frameFormatChange notification!

struct BOBUIextLayoutStruct;

class BOBUIextFrameData : public BOBUIextFrameLayoutData
{
public:
    BOBUIextFrameData();

    // relative to parent frame
    QFixedPoint position;
    QFixedSize size;

    // contents starts at (margin+border/margin+border)
    QFixed topMargin;
    QFixed bottomMargin;
    QFixed leftMargin;
    QFixed rightMargin;
    QFixed border;
    QFixed padding;
    // contents width includes padding (as we need to treat this on a per cell basis for tables)
    QFixed contentsWidth;
    QFixed contentsHeight;
    QFixed oldContentsWidth;

    // accumulated margins
    QFixed effectiveTopMargin;
    QFixed effectiveBottomMargin;

    QFixed minimumWidth;
    QFixed maximumWidth;

    BOBUIextLayoutStruct *currentLayoutStruct;

    bool sizeDirty;
    bool layoutDirty;

    QList<QPointer<BOBUIextFrame>> floats;
};

BOBUIextFrameData::BOBUIextFrameData()
    : maximumWidth(QFIXED_MAX),
      currentLayoutStruct(nullptr), sizeDirty(true), layoutDirty(true)
{
}

struct BOBUIextLayoutStruct {
    BOBUIextLayoutStruct() : maximumWidth(QFIXED_MAX), fullLayout(false)
    {}
    BOBUIextFrame *frame;
    QFixed x_left;
    QFixed x_right;
    QFixed frameY; // absolute y position of the current frame
    QFixed y; // always relative to the current frame
    QFixed contentsWidth;
    QFixed minimumWidth;
    QFixed maximumWidth;
    bool fullLayout;
    QList<BOBUIextFrame *> pendingFloats;
    QFixed pageHeight;
    QFixed pageBottom;
    QFixed pageTopMargin;
    QFixed pageBottomMargin;
    QRectF updateRect;
    QRectF updateRectForFloats;

    inline void addUpdateRectForFloat(const QRectF &rect) {
        if (updateRectForFloats.isValid())
            updateRectForFloats |= rect;
        else
            updateRectForFloats = rect;
    }

    inline QFixed absoluteY() const
    { return frameY + y; }

    inline QFixed contentHeight() const
    { return pageHeight - pageBottomMargin - pageTopMargin; }

    inline int currentPage() const
    { return pageHeight == 0 ? 0 : (absoluteY() / pageHeight).truncate(); }

    inline void newPage()
    { if (pageHeight == QFIXED_MAX) return; pageBottom += pageHeight; y = qMax(y, pageBottom - pageHeight + pageBottomMargin + pageTopMargin - frameY); }
};

#ifndef BOBUI_NO_CSSPARSER
// helper struct to collect edge data and priorize edges for border-collapse mode
struct EdgeData {

    enum EdgeClass {
        // don't change order, used for comparison
        ClassInvalid,     // queried (adjacent) cell does not exist
        ClassNone,        // no explicit border, no grid, no table border
        ClassGrid,        // 1px grid if drawGrid is true
        ClassTableBorder, // an outermost edge
        ClassExplicit     // set in cell's format
    };

    EdgeData(qreal width, const BOBUIextTableCell &cell, QCss::Edge edge, EdgeClass edgeClass) :
        width(width), cell(cell), edge(edge), edgeClass(edgeClass) {}
    EdgeData() :
        width(0), edge(QCss::NumEdges), edgeClass(ClassInvalid) {}

    // used for priorization with qMax
    bool operator< (const EdgeData &other) const {
        if (width < other.width) return true;
        if (width > other.width) return false;
        if (edgeClass < other.edgeClass) return true;
        if (edgeClass > other.edgeClass) return false;
        if (edge == QCss::TopEdge && other.edge == QCss::BottomEdge) return true;
        if (edge == QCss::BottomEdge && other.edge == QCss::TopEdge) return false;
        if (edge == QCss::LeftEdge && other.edge == QCss::RightEdge) return true;
        return false;
    }
    bool operator> (const EdgeData &other) const {
        return other < *this;
    }

    qreal width;
    BOBUIextTableCell cell;
    QCss::Edge edge;
    EdgeClass edgeClass;
};

// axisEdgeData is referenced by BOBUIextTableData's inline methods, so predeclare
class BOBUIextTableData;
static inline EdgeData axisEdgeData(BOBUIextTable *table, const BOBUIextTableData *td, const BOBUIextTableCell &cell, QCss::Edge edge);
#endif

class BOBUIextTableData : public BOBUIextFrameData
{
public:
    QFixed cellSpacing, cellPadding;
    qreal deviceScale;
    QList<QFixed> minWidths;
    QList<QFixed> maxWidths;
    QList<QFixed> widths;
    QList<QFixed> heights;
    QList<QFixed> columnPositions;
    QList<QFixed> rowPositions;

    QList<QFixed> cellVerticalOffsets;

    // without borderCollapse, those equal BOBUIextFrameData::border;
    // otherwise the widest outermost cell edge will be used
    QFixed effectiveLeftBorder;
    QFixed effectiveTopBorder;
    QFixed effectiveRightBorder;
    QFixed effectiveBottomBorder;

    QFixed headerHeight;

    QFixed borderCell; // 0 if borderCollapse is enabled, BOBUIextFrameData::border otherwise
    bool borderCollapse;
    bool drawGrid;

    // maps from cell index (row + col * rowCount) to child frames belonging to
    // the specific cell
    QMultiHash<int, BOBUIextFrame *> childFrameMap;

    inline QFixed cellWidth(int column, int colspan) const
    { return columnPositions.at(column + colspan - 1) + widths.at(column + colspan - 1)
             - columnPositions.at(column); }

    inline void calcRowPosition(int row)
    {
        if (row > 0)
            rowPositions[row] = rowPositions.at(row - 1) + heights.at(row - 1) + borderCell + cellSpacing + borderCell;
    }

    QRectF cellRect(const BOBUIextTableCell &cell) const;

    inline QFixed paddingProperty(const BOBUIextFormat &format, BOBUIextFormat::Property property) const
    {
        QVariant v = format.property(property);
        if (v.isNull()) {
            return cellPadding;
        } else {
            Q_ASSERT(v.userType() == QMetaType::Double || v.userType() == QMetaType::Float);
            return QFixed::fromReal(v.toReal() * deviceScale);
        }
    }

#ifndef BOBUI_NO_CSSPARSER
    inline QFixed cellBorderWidth(BOBUIextTable *table, const BOBUIextTableCell &cell, QCss::Edge edge) const
    {
        qreal rv = axisEdgeData(table, this, cell, edge).width;
        if (borderCollapse)
            rv /= 2; // each cell has to add half of the border's width to its own padding
        return QFixed::fromReal(rv * deviceScale);
    }
#endif

    inline QFixed topPadding(BOBUIextTable *table, const BOBUIextTableCell &cell) const
    {
#ifdef BOBUI_NO_CSSPARSER
        Q_UNUSED(table);
#endif
        return paddingProperty(cell.format(), BOBUIextFormat::TableCellTopPadding)
#ifndef BOBUI_NO_CSSPARSER
                + cellBorderWidth(table, cell, QCss::TopEdge)
#endif
        ;
    }

    inline QFixed bottomPadding(BOBUIextTable *table, const BOBUIextTableCell &cell) const
    {
#ifdef BOBUI_NO_CSSPARSER
        Q_UNUSED(table);
#endif
        return paddingProperty(cell.format(), BOBUIextFormat::TableCellBottomPadding)
#ifndef BOBUI_NO_CSSPARSER
                + cellBorderWidth(table, cell, QCss::BottomEdge)
#endif
                ;
    }

    inline QFixed leftPadding(BOBUIextTable *table, const BOBUIextTableCell &cell) const
    {
#ifdef BOBUI_NO_CSSPARSER
        Q_UNUSED(table);
#endif
        return paddingProperty(cell.format(), BOBUIextFormat::TableCellLeftPadding)
#ifndef BOBUI_NO_CSSPARSER
                + cellBorderWidth(table, cell, QCss::LeftEdge)
#endif
        ;
    }

    inline QFixed rightPadding(BOBUIextTable *table, const BOBUIextTableCell &cell) const
    {
#ifdef BOBUI_NO_CSSPARSER
        Q_UNUSED(table);
#endif
        return paddingProperty(cell.format(), BOBUIextFormat::TableCellRightPadding)
#ifndef BOBUI_NO_CSSPARSER
                + cellBorderWidth(table, cell, QCss::RightEdge)
#endif
        ;
    }

    inline QFixedPoint cellPosition(BOBUIextTable *table, const BOBUIextTableCell &cell) const
    {
        return cellPosition(cell.row(), cell.column()) + QFixedPoint(leftPadding(table, cell), topPadding(table, cell));
    }

    void updateTableSize();

private:
    inline QFixedPoint cellPosition(int row, int col) const
    { return QFixedPoint(columnPositions.at(col), rowPositions.at(row) + cellVerticalOffsets.at(col + row * widths.size())); }
};

static BOBUIextFrameData *createData(BOBUIextFrame *f)
{
    BOBUIextFrameData *data;
    if (qobject_cast<BOBUIextTable *>(f))
        data = new BOBUIextTableData;
    else
        data = new BOBUIextFrameData;
    f->setLayoutData(data);
    return data;
}

static inline BOBUIextFrameData *data(BOBUIextFrame *f)
{
    BOBUIextFrameData *data = static_cast<BOBUIextFrameData *>(f->layoutData());
    if (!data)
        data = createData(f);
    return data;
}

static bool isFrameFromInlineObject(BOBUIextFrame *f)
{
    return f->firstPosition() > f->lastPosition();
}

void BOBUIextTableData::updateTableSize()
{
    const QFixed effectiveTopMargin = this->topMargin + effectiveTopBorder + padding;
    const QFixed effectiveBottomMargin = this->bottomMargin + effectiveBottomBorder + padding;
    const QFixed effectiveLeftMargin = this->leftMargin + effectiveLeftBorder + padding;
    const QFixed effectiveRightMargin = this->rightMargin + effectiveRightBorder + padding;
    size.height = contentsHeight == -1
                   ? rowPositions.constLast() + heights.constLast() + padding + border + cellSpacing + effectiveBottomMargin
                   : effectiveTopMargin + contentsHeight + effectiveBottomMargin;
    size.width = effectiveLeftMargin + contentsWidth + effectiveRightMargin;
}

QRectF BOBUIextTableData::cellRect(const BOBUIextTableCell &cell) const
{
    const int row = cell.row();
    const int rowSpan = cell.rowSpan();
    const int column = cell.column();
    const int colSpan = cell.columnSpan();

    return QRectF(columnPositions.at(column).toReal(),
                  rowPositions.at(row).toReal(),
                  (columnPositions.at(column + colSpan - 1) + widths.at(column + colSpan - 1) - columnPositions.at(column)).toReal(),
                  (rowPositions.at(row + rowSpan - 1) + heights.at(row + rowSpan - 1) - rowPositions.at(row)).toReal());
}

static inline bool isEmptyBlockBeforeTable(const BOBUIextBlock &block, const BOBUIextBlockFormat &format, const BOBUIextFrame::Iterator &nextIt)
{
    return !nextIt.atEnd()
           && qobject_cast<BOBUIextTable *>(nextIt.currentFrame())
           && block.isValid()
           && block.length() == 1
           && !format.hasProperty(BOBUIextFormat::BlockTrailingHorizontalRulerWidth)
           && !format.hasProperty(BOBUIextFormat::BackgroundBrush)
           && nextIt.currentFrame()->firstPosition() == block.position() + 1
           ;
}

static inline bool isEmptyBlockBeforeTable(const BOBUIextFrame::Iterator &it)
{
    BOBUIextFrame::Iterator next = it; ++next;
    if (it.currentFrame())
        return false;
    BOBUIextBlock block = it.currentBlock();
    return isEmptyBlockBeforeTable(block, block.blockFormat(), next);
}

static inline bool isEmptyBlockAfterTable(const BOBUIextBlock &block, const BOBUIextFrame *previousFrame)
{
    return qobject_cast<const BOBUIextTable *>(previousFrame)
           && block.isValid()
           && block.length() == 1
           && previousFrame->lastPosition() == block.position() - 1
           ;
}

static inline bool isLineSeparatorBlockAfterTable(const BOBUIextBlock &block, const BOBUIextFrame *previousFrame)
{
    return qobject_cast<const BOBUIextTable *>(previousFrame)
           && block.isValid()
           && block.length() > 1
           && block.text().at(0) == QChar::LineSeparator
           && previousFrame->lastPosition() == block.position() - 1
           ;
}

/*

Optimization strategies:

HTML layout:

* Distinguish between normal and special flow. For normal flow the condition:
  y1 > y2 holds for all blocks with b1.key() > b2.key().
* Special flow is: floats, table cells

* Normal flow within table cells. Tables (not cells) are part of the normal flow.


* If blocks grows/shrinks in height and extends over whole page width at the end, move following blocks.
* If height doesn't change, no need to do anything

Table cells:

* If minWidth of cell changes, recalculate table width, relayout if needed.
* What about maxWidth when doing auto layout?

Floats:
* need fixed or proportional width, otherwise don't float!
* On width/height change relayout surrounding paragraphs.

Document width change:
* full relayout needed


Float handling:

* Floats are specified by a special format object.
* currently only floating images are implemented.

*/

/*

   On the table layouting:

   +---[ table border ]-------------------------
   |      [ cell spacing ]
   |  +------[ cell border ]-----+  +--------
   |  |                          |  |
   |  |
   |  |
   |  |
   |

   rowPositions[i] and columnPositions[i] point at the cell content
   position. So for example the left border is drawn at
   x = columnPositions[i] - fd->border and similar for y.

*/

struct QCheckPoint
{
    QFixed y;
    QFixed frameY; // absolute y position of the current frame
    int positionInFrame;
    QFixed minimumWidth;
    QFixed maximumWidth;
    QFixed contentsWidth;
};
Q_DECLARE_TYPEINFO(QCheckPoint, Q_PRIMITIVE_TYPE);

static bool operator<(const QCheckPoint &checkPoint, QFixed y)
{
    return checkPoint.y < y;
}

static bool operator<(const QCheckPoint &checkPoint, int pos)
{
    return checkPoint.positionInFrame < pos;
}

static void fillBackground(QPainter *p, const QRectF &rect, QBrush brush, const QPointF &origin, const QRectF &gradientRect = QRectF())
{
    p->save();
    if (brush.style() >= BobUI::LinearGradientPattern && brush.style() <= BobUI::ConicalGradientPattern) {
        if (!gradientRect.isNull()) {
            BOBUIransform m;
            m.translate(gradientRect.left(), gradientRect.top());
            m.scale(gradientRect.width(), gradientRect.height());
            brush.setTransform(m);
            const_cast<QGradient *>(brush.gradient())->setCoordinateMode(QGradient::LogicalMode);
        }
    } else {
        p->setBrushOrigin(origin);
    }
    p->fillRect(rect, brush);
    p->restore();
}

class BOBUIextDocumentLayoutPrivate : public QAbstractTextDocumentLayoutPrivate
{
    Q_DECLARE_PUBLIC(BOBUIextDocumentLayout)
public:
    BOBUIextDocumentLayoutPrivate();

    BOBUIextOption::WrapMode wordWrapMode;
#ifdef LAYOUT_DEBUG
    mutable QString debug_indent;
#endif

    int fixedColumnWidth;
    int cursorWidth;

    QSizeF lastReportedSize;
    QRectF viewportRect;
    QRectF clipRect;

    mutable int currentLazyLayoutPosition;
    mutable int lazyLayoutStepSize;
    QBasicTimer layoutTimer;
    mutable QBasicTimer sizeChangedTimer;
    uint showLayoutProgress : 1;
    uint insideDocumentChange : 1;

    int lastPageCount;
    qreal idealWidth;
    bool contentHasAlignment;

    QFixed blockIndent(const BOBUIextBlockFormat &blockFormat) const;

    void drawFrame(const QPointF &offset, QPainter *painter, const QAbstractTextDocumentLayout::PaintContext &context,
                   BOBUIextFrame *f) const;
    void drawFlow(const QPointF &offset, QPainter *painter, const QAbstractTextDocumentLayout::PaintContext &context,
                  BOBUIextFrame::Iterator it, const QList<BOBUIextFrame *> &floats, BOBUIextBlock *cursorBlockNeedingRepaint) const;
    void drawBlock(const QPointF &offset, QPainter *painter, const QAbstractTextDocumentLayout::PaintContext &context,
                   const BOBUIextBlock &bl, bool inRootFrame) const;
    void drawListItem(const QPointF &offset, QPainter *painter, const QAbstractTextDocumentLayout::PaintContext &context,
                      const BOBUIextBlock &bl, const BOBUIextCharFormat *selectionFormat) const;
    void drawTableCellBorder(const QRectF &cellRect, QPainter *painter, BOBUIextTable *table, BOBUIextTableData *td, const BOBUIextTableCell &cell) const;
    void drawTableCell(const QRectF &cellRect, QPainter *painter, const QAbstractTextDocumentLayout::PaintContext &cell_context,
                       BOBUIextTable *table, BOBUIextTableData *td, int r, int c,
                       BOBUIextBlock *cursorBlockNeedingRepaint, QPointF *cursorBlockOffset) const;
    void drawBorder(QPainter *painter, const QRectF &rect, qreal topMargin, qreal bottomMargin, qreal border,
                    const QBrush &brush, BOBUIextFrameFormat::BorderStyle style) const;
    void drawFrameDecoration(QPainter *painter, BOBUIextFrame *frame, BOBUIextFrameData *fd, const QRectF &clip, const QRectF &rect) const;

    enum HitPoint {
        PointBefore,
        PointAfter,
        PointInside,
        PointExact
    };
    HitPoint hitTest(BOBUIextFrame *frame, const QFixedPoint &point, int *position, BOBUIextLayout **l, BobUI::HitTestAccuracy accuracy) const;
    HitPoint hitTest(BOBUIextFrame::Iterator it, HitPoint hit, const QFixedPoint &p,
                     int *position, BOBUIextLayout **l, BobUI::HitTestAccuracy accuracy) const;
    HitPoint hitTest(BOBUIextTable *table, const QFixedPoint &point, int *position, BOBUIextLayout **l, BobUI::HitTestAccuracy accuracy) const;
    HitPoint hitTest(const BOBUIextBlock &bl, const QFixedPoint &point, int *position, BOBUIextLayout **l, BobUI::HitTestAccuracy accuracy) const;

    BOBUIextLayoutStruct layoutCell(BOBUIextTable *t, const BOBUIextTableCell &cell, QFixed width,
                                 int layoutFrom, int layoutTo, BOBUIextTableData *tableData, QFixed absoluteTableY,
                                 bool withPageBreaks);
    void setCellPosition(BOBUIextTable *t, const BOBUIextTableCell &cell, const QPointF &pos);
    QRectF layoutTable(BOBUIextTable *t, int layoutFrom, int layoutTo, QFixed parentY);

    void positionFloat(BOBUIextFrame *frame, BOBUIextLine *currentLine = nullptr);

    // calls the next one
    QRectF layoutFrame(BOBUIextFrame *f, int layoutFrom, int layoutTo, QFixed parentY = 0);
    QRectF layoutFrame(BOBUIextFrame *f, int layoutFrom, int layoutTo, QFixed frameWidth, QFixed frameHeight, QFixed parentY = 0);

    void layoutBlock(const BOBUIextBlock &bl, int blockPosition, const BOBUIextBlockFormat &blockFormat,
                     BOBUIextLayoutStruct *layoutStruct, int layoutFrom, int layoutTo, const BOBUIextBlockFormat *previousBlockFormat);
    void layoutFlow(BOBUIextFrame::Iterator it, BOBUIextLayoutStruct *layoutStruct, int layoutFrom, int layoutTo, QFixed width = 0);

    void floatMargins(QFixed y, const BOBUIextLayoutStruct *layoutStruct, QFixed *left, QFixed *right) const;
    QFixed findY(QFixed yFrom, const BOBUIextLayoutStruct *layoutStruct, QFixed requiredWidth) const;

    QList<QCheckPoint> checkPoints;

    BOBUIextFrame::Iterator frameIteratorForYPosition(QFixed y) const;
    BOBUIextFrame::Iterator frameIteratorForTextPosition(int position) const;

    void ensureLayouted(QFixed y) const;
    void ensureLayoutedByPosition(int position) const;
    inline void ensureLayoutFinished() const
    { ensureLayoutedByPosition(INT_MAX); }
    void layoutStep() const;

    QRectF frameBoundingRectInternal(BOBUIextFrame *frame) const;

    qreal scaleToDevice(qreal value) const;
    QFixed scaleToDevice(QFixed value) const;
};

BOBUIextDocumentLayoutPrivate::BOBUIextDocumentLayoutPrivate()
    : fixedColumnWidth(-1),
      cursorWidth(1),
      currentLazyLayoutPosition(-1),
      lazyLayoutStepSize(1000),
      lastPageCount(-1)
{
    showLayoutProgress = true;
    insideDocumentChange = false;
    idealWidth = 0;
    contentHasAlignment = false;
}

BOBUIextFrame::Iterator BOBUIextDocumentLayoutPrivate::frameIteratorForYPosition(QFixed y) const
{
    BOBUIextFrame *rootFrame = document->rootFrame();

    if (checkPoints.isEmpty()
        || y < 0 || y > data(rootFrame)->size.height)
        return rootFrame->begin();

    auto checkPoint = std::lower_bound(checkPoints.begin(), checkPoints.end(), y);
    if (checkPoint == checkPoints.end())
        return rootFrame->begin();

    if (checkPoint != checkPoints.begin())
        --checkPoint;

    const int position = rootFrame->firstPosition() + checkPoint->positionInFrame;
    return frameIteratorForTextPosition(position);
}

BOBUIextFrame::Iterator BOBUIextDocumentLayoutPrivate::frameIteratorForTextPosition(int position) const
{
    BOBUIextFrame *rootFrame = docPrivate->rootFrame();

    const BOBUIextDocumentPrivate::BlockMap &map = docPrivate->blockMap();
    const int begin = map.findNode(rootFrame->firstPosition());
    const int end = map.findNode(rootFrame->lastPosition()+1);

    const int block = map.findNode(position);
    const int blockPos = map.position(block);

    BOBUIextFrame::iterator it(rootFrame, block, begin, end);

    BOBUIextFrame *containingFrame = docPrivate->frameAt(blockPos);
    if (containingFrame != rootFrame) {
        while (containingFrame->parentFrame() != rootFrame) {
            containingFrame = containingFrame->parentFrame();
            Q_ASSERT(containingFrame);
        }

        it.cf = containingFrame;
        it.cb = 0;
    }

    return it;
}

BOBUIextDocumentLayoutPrivate::HitPoint
BOBUIextDocumentLayoutPrivate::hitTest(BOBUIextFrame *frame, const QFixedPoint &point, int *position, BOBUIextLayout **l, BobUI::HitTestAccuracy accuracy) const
{
    BOBUIextFrameData *fd = data(frame);
    // #########
    if (fd->layoutDirty)
        return PointAfter;
    Q_ASSERT(!fd->layoutDirty);
    Q_ASSERT(!fd->sizeDirty);
    const QFixedPoint relativePoint(point.x - fd->position.x, point.y - fd->position.y);

    BOBUIextFrame *rootFrame = docPrivate->rootFrame();

    qCDebug(lcHit) << "checking frame" << frame->firstPosition() << "point=" << point.toPointF()
                   << "position" << fd->position.toPointF() << "size" << fd->size.toSizeF();
    if (frame != rootFrame) {
        if (relativePoint.y < 0 || relativePoint.x < 0) {
            *position = frame->firstPosition() - 1;
            qCDebug(lcHit) << "before pos=" << *position;
            return PointBefore;
        } else if (relativePoint.y > fd->size.height || relativePoint.x > fd->size.width) {
            *position = frame->lastPosition() + 1;
            qCDebug(lcHit) << "after pos=" << *position;
            return PointAfter;
        }
    }

    if (isFrameFromInlineObject(frame)) {
        *position = frame->firstPosition() - 1;
        return PointExact;
    }

    if (BOBUIextTable *table = qobject_cast<BOBUIextTable *>(frame)) {
        const int rows = table->rows();
        const int columns = table->columns();
        BOBUIextTableData *td = static_cast<BOBUIextTableData *>(data(table));

        if (!td->childFrameMap.isEmpty()) {
            for (int r = 0; r < rows; ++r) {
                for (int c = 0; c < columns; ++c) {
                    BOBUIextTableCell cell = table->cellAt(r, c);
                    if (cell.row() != r || cell.column() != c)
                        continue;

                    QRectF cellRect = td->cellRect(cell);
                    const QFixedPoint cellPos = QFixedPoint::fromPointF(cellRect.topLeft());
                    const QFixedPoint pointInCell = relativePoint - cellPos;

                    const QList<BOBUIextFrame *> childFrames = td->childFrameMap.values(r + c * rows);
                    for (int i = 0; i < childFrames.size(); ++i) {
                        BOBUIextFrame *child = childFrames.at(i);
                        if (isFrameFromInlineObject(child)
                            && child->frameFormat().position() != BOBUIextFrameFormat::InFlow
                            && hitTest(child, pointInCell, position, l, accuracy) == PointExact)
                        {
                            return PointExact;
                        }
                    }
                }
            }
        }

        return hitTest(table, relativePoint, position, l, accuracy);
    }

    const QList<BOBUIextFrame *> childFrames = frame->childFrames();
    for (int i = 0; i < childFrames.size(); ++i) {
        BOBUIextFrame *child = childFrames.at(i);
        if (isFrameFromInlineObject(child)
            && child->frameFormat().position() != BOBUIextFrameFormat::InFlow
            && hitTest(child, relativePoint, position, l, accuracy) == PointExact)
        {
            return PointExact;
        }
    }

    BOBUIextFrame::Iterator it = frame->begin();

    if (frame == rootFrame) {
        it = frameIteratorForYPosition(relativePoint.y);

        Q_ASSERT(it.parentFrame() == frame);
    }

    if (it.currentFrame())
        *position = it.currentFrame()->firstPosition();
    else
        *position = it.currentBlock().position();

    return hitTest(it, PointBefore, relativePoint, position, l, accuracy);
}

BOBUIextDocumentLayoutPrivate::HitPoint
BOBUIextDocumentLayoutPrivate::hitTest(BOBUIextFrame::Iterator it, HitPoint hit, const QFixedPoint &p,
                                    int *position, BOBUIextLayout **l, BobUI::HitTestAccuracy accuracy) const
{
    for (; !it.atEnd(); ++it) {
        BOBUIextFrame *c = it.currentFrame();
        HitPoint hp;
        int pos = -1;
        if (c) {
            hp = hitTest(c, p, &pos, l, accuracy);
        } else {
            hp = hitTest(it.currentBlock(), p, &pos, l, accuracy);
        }
        if (hp >= PointInside) {
            if (isEmptyBlockBeforeTable(it))
                continue;
            hit = hp;
            *position = pos;
            break;
        }
        if (hp == PointBefore && pos < *position) {
            *position = pos;
            hit = hp;
        } else if (hp == PointAfter && pos > *position) {
            *position = pos;
            hit = hp;
        }
    }

    qCDebug(lcHit) << "inside=" << hit << " pos=" << *position;
    return hit;
}

BOBUIextDocumentLayoutPrivate::HitPoint
BOBUIextDocumentLayoutPrivate::hitTest(BOBUIextTable *table, const QFixedPoint &point,
                                    int *position, BOBUIextLayout **l, BobUI::HitTestAccuracy accuracy) const
{
    BOBUIextTableData *td = static_cast<BOBUIextTableData *>(data(table));

    auto rowIt = std::lower_bound(td->rowPositions.constBegin(), td->rowPositions.constEnd(), point.y);
    if (rowIt == td->rowPositions.constEnd()) {
        rowIt = td->rowPositions.constEnd() - 1;
    } else if (rowIt != td->rowPositions.constBegin()) {
        --rowIt;
    }

    auto colIt = std::lower_bound(td->columnPositions.constBegin(), td->columnPositions.constEnd(), point.x);
    if (colIt == td->columnPositions.constEnd()) {
        colIt = td->columnPositions.constEnd() - 1;
    } else if (colIt != td->columnPositions.constBegin()) {
        --colIt;
    }

    BOBUIextTableCell cell = table->cellAt(rowIt - td->rowPositions.constBegin(),
                                        colIt - td->columnPositions.constBegin());
    if (!cell.isValid())
        return PointBefore;

    *position = cell.firstPosition();

    HitPoint hp = hitTest(cell.begin(), PointInside, point - td->cellPosition(table, cell), position, l, accuracy);

    if (hp == PointExact)
        return hp;
    if (hp == PointAfter)
        *position = cell.lastPosition();
    return PointInside;
}

BOBUIextDocumentLayoutPrivate::HitPoint
BOBUIextDocumentLayoutPrivate::hitTest(const BOBUIextBlock &bl, const QFixedPoint &point, int *position, BOBUIextLayout **l,
                                    BobUI::HitTestAccuracy accuracy) const
{
    BOBUIextLayout *tl = bl.layout();
    QRectF textrect = tl->boundingRect();
    textrect.translate(tl->position());
    qCDebug(lcHit) << "    checking block" << bl.position() << "point=" << point.toPointF() << "    tlrect" << textrect;
    *position = bl.position();
    if (point.y.toReal() < textrect.top() - bl.blockFormat().topMargin()) {
        qCDebug(lcHit) << "    before pos=" << *position;
        return PointBefore;
    } else if (point.y.toReal() > textrect.bottom()) {
        *position += bl.length();
        qCDebug(lcHit) << "    after pos=" << *position;
        return PointAfter;
    }

    QPointF pos = point.toPointF() - tl->position();

    // ### rtl?

    HitPoint hit = PointInside;
    *l = tl;
    int off = 0;
    for (int i = 0; i < tl->lineCount(); ++i) {
        BOBUIextLine line = tl->lineAt(i);
        const QRectF lr = line.naturalTextRect();
        if (lr.top() > pos.y()) {
            off = qMin(off, line.textStart());
        } else if (lr.bottom() <= pos.y()) {
            off = qMax(off, line.textStart() + line.textLength());
        } else {
            if (lr.left() <= pos.x() && lr.right() >= pos.x())
                hit = PointExact;
            // when trying to hit an anchor we want it to hit not only in the left
            // half
            if (accuracy == BobUI::ExactHit)
                off = line.xToCursor(pos.x(), BOBUIextLine::CursorOnCharacter);
            else
                off = line.xToCursor(pos.x(), BOBUIextLine::CursorBetweenCharacters);
            break;
        }
    }
    *position += off;

    qCDebug(lcHit) << "    inside=" << hit << " pos=" << *position;
    return hit;
}

// ### could be moved to BOBUIextBlock
QFixed BOBUIextDocumentLayoutPrivate::blockIndent(const BOBUIextBlockFormat &blockFormat) const
{
    qreal indent = blockFormat.indent();

    BOBUIextObject *object = document->objectForFormat(blockFormat);
    if (object)
        indent += object->format().toListFormat().indent();

    if (qIsNull(indent))
        return 0;

    qreal scale = 1;
    if (paintDevice) {
        scale = qreal(paintDevice->logicalDpiY()) / qreal(bobui_defaultDpi());
    }

    return QFixed::fromReal(indent * scale * document->indentWidth());
}

struct BorderPaginator
{
    BorderPaginator(BOBUIextDocument *document, const QRectF &rect, qreal topMarginAfterPageBreak, qreal bottomMargin, qreal border) :
        pageHeight(document->pageSize().height()),
        topPage(pageHeight > 0 ? static_cast<int>(rect.top() / pageHeight) : 0),
        bottomPage(pageHeight > 0 ? static_cast<int>((rect.bottom() + border) / pageHeight) : 0),
        rect(rect),
        topMarginAfterPageBreak(topMarginAfterPageBreak),
        bottomMargin(bottomMargin), border(border)
    {}

    QRectF clipRect(int page) const
    {
        QRectF clipped = rect.toRect();

        if (topPage != bottomPage) {
            clipped.setTop(qMax(clipped.top(), page * pageHeight + topMarginAfterPageBreak - border));
            clipped.setBottom(qMin(clipped.bottom(), (page + 1) * pageHeight - bottomMargin));

            if (clipped.bottom() <= clipped.top())
                return QRectF();
        }

        return clipped;
    }

    qreal pageHeight;
    int topPage;
    int bottomPage;
    QRectF rect;
    qreal topMarginAfterPageBreak;
    qreal bottomMargin;
    qreal border;
};

void BOBUIextDocumentLayoutPrivate::drawBorder(QPainter *painter, const QRectF &rect, qreal topMargin, qreal bottomMargin,
                                            qreal border, const QBrush &brush, BOBUIextFrameFormat::BorderStyle style) const
{
    BorderPaginator paginator(document, rect, topMargin, bottomMargin, border);

#ifndef BOBUI_NO_CSSPARSER
    QCss::BorderStyle cssStyle = static_cast<QCss::BorderStyle>(style + 1);
#else
    Q_UNUSED(style);
#endif //BOBUI_NO_CSSPARSER

    bool turn_off_antialiasing = !(painter->renderHints() & QPainter::Antialiasing);
    painter->setRenderHint(QPainter::Antialiasing);

    for (int i = paginator.topPage; i <= paginator.bottomPage; ++i) {
        QRectF clipped = paginator.clipRect(i);
        if (!clipped.isValid())
            continue;

#ifndef BOBUI_NO_CSSPARSER
        qDrawEdge(painter, clipped.left(), clipped.top(), clipped.left() + border, clipped.bottom() + border, 0, 0, QCss::LeftEdge, cssStyle, brush);
        qDrawEdge(painter, clipped.left() + border, clipped.top(), clipped.right() + border, clipped.top() + border, 0, 0, QCss::TopEdge, cssStyle, brush);
        qDrawEdge(painter, clipped.right(), clipped.top() + border, clipped.right() + border, clipped.bottom(), 0, 0, QCss::RightEdge, cssStyle, brush);
        qDrawEdge(painter, clipped.left() + border, clipped.bottom(), clipped.right() + border, clipped.bottom() + border, 0, 0, QCss::BottomEdge, cssStyle, brush);
#else
        painter->save();
        painter->setPen(BobUI::NoPen);
        painter->setBrush(brush);
        painter->drawRect(QRectF(clipped.left(), clipped.top(), clipped.left() + border, clipped.bottom() + border));
        painter->drawRect(QRectF(clipped.left() + border, clipped.top(), clipped.right() + border, clipped.top() + border));
        painter->drawRect(QRectF(clipped.right(), clipped.top() + border, clipped.right() + border, clipped.bottom()));
        painter->drawRect(QRectF(clipped.left() + border, clipped.bottom(), clipped.right() + border, clipped.bottom() + border));
        painter->restore();
#endif //BOBUI_NO_CSSPARSER
    }
    if (turn_off_antialiasing)
        painter->setRenderHint(QPainter::Antialiasing, false);
}

void BOBUIextDocumentLayoutPrivate::drawFrameDecoration(QPainter *painter, BOBUIextFrame *frame, BOBUIextFrameData *fd, const QRectF &clip, const QRectF &rect) const
{

    const QBrush bg = frame->frameFormat().background();
    if (bg != BobUI::NoBrush) {
        QRectF bgRect = rect;
        bgRect.adjust((fd->leftMargin + fd->border).toReal(),
                      (fd->topMargin + fd->border).toReal(),
                      - (fd->rightMargin + fd->border).toReal(),
                      - (fd->bottomMargin + fd->border).toReal());

        QRectF gradientRect; // invalid makes it default to bgRect
        QPointF origin = bgRect.topLeft();
        if (!frame->parentFrame()) {
            bgRect = clip;
            gradientRect.setWidth(painter->device()->width());
            gradientRect.setHeight(painter->device()->height());
        }
        fillBackground(painter, bgRect, bg, origin, gradientRect);
    }
    if (fd->border != 0) {
        painter->save();
        painter->setBrush(BobUI::lightGray);
        painter->setPen(BobUI::NoPen);

        const qreal leftEdge = rect.left() + fd->leftMargin.toReal();
        const qreal border = fd->border.toReal();
        const qreal topMargin = fd->topMargin.toReal();
        const qreal leftMargin = fd->leftMargin.toReal();
        const qreal bottomMargin = fd->bottomMargin.toReal();
        const qreal rightMargin = fd->rightMargin.toReal();
        const qreal w = rect.width() - 2 * border - leftMargin - rightMargin;
        const qreal h = rect.height() - 2 * border - topMargin - bottomMargin;

        drawBorder(painter, QRectF(leftEdge, rect.top() + topMargin, w + border, h + border),
                   fd->effectiveTopMargin.toReal(), fd->effectiveBottomMargin.toReal(),
                   border, frame->frameFormat().borderBrush(), frame->frameFormat().borderStyle());

        painter->restore();
    }
}

static void adjustContextSelectionsForCell(QAbstractTextDocumentLayout::PaintContext &cell_context,
                                           const BOBUIextTableCell &cell,
                                           int r, int c,
                                           const int *selectedTableCells)
{
    for (int i = 0; i < cell_context.selections.size(); ++i) {
        int row_start = selectedTableCells[i * 4];
        int col_start = selectedTableCells[i * 4 + 1];
        int num_rows = selectedTableCells[i * 4 + 2];
        int num_cols = selectedTableCells[i * 4 + 3];

        if (row_start != -1) {
            if (r >= row_start && r < row_start + num_rows
                && c >= col_start && c < col_start + num_cols)
            {
                int firstPosition = cell.firstPosition();
                int lastPosition = cell.lastPosition();

                // make sure empty cells are still selected
                if (firstPosition == lastPosition)
                    ++lastPosition;

                cell_context.selections[i].cursor.setPosition(firstPosition);
                cell_context.selections[i].cursor.setPosition(lastPosition, BOBUIextCursor::KeepAnchor);
            } else {
                cell_context.selections[i].cursor.clearSelection();
            }
        }

        // FullWidthSelection is not useful for tables
        cell_context.selections[i].format.clearProperty(BOBUIextFormat::FullWidthSelection);
    }
}

static bool cellClipTest(BOBUIextTable *table, BOBUIextTableData *td,
                         const QAbstractTextDocumentLayout::PaintContext &cell_context,
                         const BOBUIextTableCell &cell,
                         QRectF cellRect)
{
#ifdef BOBUI_NO_CSSPARSER
    Q_UNUSED(table);
    Q_UNUSED(cell);
#endif

    if (!cell_context.clip.isValid())
        return false;

    if (td->borderCollapse) {
        // we need to account for the cell borders in the clipping test
#ifndef BOBUI_NO_CSSPARSER
        cellRect.adjust(-axisEdgeData(table, td, cell, QCss::LeftEdge).width / 2,
                        -axisEdgeData(table, td, cell, QCss::TopEdge).width / 2,
                        axisEdgeData(table, td, cell, QCss::RightEdge).width / 2,
                        axisEdgeData(table, td, cell, QCss::BottomEdge).width / 2);
#endif
    } else {
        qreal border = td->border.toReal();
        cellRect.adjust(-border, -border, border, border);
    }

    if (!cellRect.intersects(cell_context.clip))
        return true;

    return false;
}

void BOBUIextDocumentLayoutPrivate::drawFrame(const QPointF &offset, QPainter *painter,
                                           const QAbstractTextDocumentLayout::PaintContext &context,
                                           BOBUIextFrame *frame) const
{
    BOBUIextFrameData *fd = data(frame);
    // #######
    if (fd->layoutDirty)
        return;
    Q_ASSERT(!fd->sizeDirty);
    Q_ASSERT(!fd->layoutDirty);

    // floor the offset to avoid painting artefacts when drawing adjacent borders
    // we later also round table cell heights and widths
    const QPointF off = QPointF(QPointF(offset + fd->position.toPointF()).toPoint());

    if (context.clip.isValid()
        && (off.y() > context.clip.bottom() || off.y() + fd->size.height.toReal() < context.clip.top()
            || off.x() > context.clip.right() || off.x() + fd->size.width.toReal() < context.clip.left()))
        return;

     qCDebug(lcDraw) << "drawFrame" << frame->firstPosition() << "--" << frame->lastPosition() << "at" << offset;

    // if the cursor is /on/ a table border we may need to repaint it
    // afterwards, as we usually draw the decoration first
    BOBUIextBlock cursorBlockNeedingRepaint;
    QPointF offsetOfRepaintedCursorBlock = off;

    BOBUIextTable *table = qobject_cast<BOBUIextTable *>(frame);
    const QRectF frameRect(off, fd->size.toSizeF());

    if (table) {
        const int rows = table->rows();
        const int columns = table->columns();
        BOBUIextTableData *td = static_cast<BOBUIextTableData *>(data(table));

        QVarLengthArray<int> selectedTableCells(context.selections.size() * 4);
        for (int i = 0; i < context.selections.size(); ++i) {
            const QAbstractTextDocumentLayout::Selection &s = context.selections.at(i);
            int row_start = -1, col_start = -1, num_rows = -1, num_cols = -1;

            if (s.cursor.currentTable() == table)
                s.cursor.selectedTableCells(&row_start, &num_rows, &col_start, &num_cols);

            selectedTableCells[i * 4] = row_start;
            selectedTableCells[i * 4 + 1] = col_start;
            selectedTableCells[i * 4 + 2] = num_rows;
            selectedTableCells[i * 4 + 3] = num_cols;
        }

        QFixed pageHeight = QFixed::fromReal(document->pageSize().height());
        if (pageHeight <= 0)
            pageHeight = QFIXED_MAX;

        QFixed absYPos = td->position.y;
        BOBUIextFrame *parentFrame = table->parentFrame();
        while (parentFrame) {
            absYPos += data(parentFrame)->position.y;
            parentFrame = parentFrame->parentFrame();
        }
        const int tableStartPage = (absYPos / pageHeight).truncate();
        const int tableEndPage = ((absYPos + td->size.height) / pageHeight).truncate();

        // for borderCollapse draw frame decoration by drawing the outermost
        // cell edges with width = td->border
        if (!td->borderCollapse)
            drawFrameDecoration(painter, frame, fd, context.clip, frameRect);

        // draw the repeated table headers for table continuation after page breaks
        const int headerRowCount = qMin(table->format().headerRowCount(), rows - 1);
        int page = tableStartPage + 1;
        while (page <= tableEndPage) {
            const QFixed pageTop = page * pageHeight + td->effectiveTopMargin + td->cellSpacing + td->border;
            const qreal headerOffset = (pageTop - td->rowPositions.at(0)).toReal();
            for (int r = 0; r < headerRowCount; ++r) {
                for (int c = 0; c < columns; ++c) {
                    BOBUIextTableCell cell = table->cellAt(r, c);
                    QAbstractTextDocumentLayout::PaintContext cell_context = context;
                    adjustContextSelectionsForCell(cell_context, cell, r, c, selectedTableCells.data());
                    QRectF cellRect = td->cellRect(cell);

                    cellRect.translate(off.x(), headerOffset);
                    if (cellClipTest(table, td, cell_context, cell, cellRect))
                        continue;

                    drawTableCell(cellRect, painter, cell_context, table, td, r, c, &cursorBlockNeedingRepaint,
                                  &offsetOfRepaintedCursorBlock);
                }
            }
            ++page;
        }

        int firstRow = 0;
        int lastRow = rows;

        if (context.clip.isValid()) {
            auto rowIt = std::lower_bound(td->rowPositions.constBegin(), td->rowPositions.constEnd(), QFixed::fromReal(context.clip.top() - off.y()));
            if (rowIt != td->rowPositions.constEnd() && rowIt != td->rowPositions.constBegin()) {
                --rowIt;
                firstRow = rowIt - td->rowPositions.constBegin();
            }

            rowIt = std::upper_bound(td->rowPositions.constBegin(), td->rowPositions.constEnd(), QFixed::fromReal(context.clip.bottom() - off.y()));
            if (rowIt != td->rowPositions.constEnd()) {
                ++rowIt;
                lastRow = rowIt - td->rowPositions.constBegin();
            }
        }

        for (int c = 0; c < columns; ++c) {
            BOBUIextTableCell cell = table->cellAt(firstRow, c);
            firstRow = qMin(firstRow, cell.row());
        }

        for (int r = firstRow; r < lastRow; ++r) {
            for (int c = 0; c < columns; ++c) {
                BOBUIextTableCell cell = table->cellAt(r, c);
                QAbstractTextDocumentLayout::PaintContext cell_context = context;
                adjustContextSelectionsForCell(cell_context, cell, r, c, selectedTableCells.data());
                QRectF cellRect = td->cellRect(cell);

                cellRect.translate(off);
                if (cellClipTest(table, td, cell_context, cell, cellRect))
                    continue;

                drawTableCell(cellRect, painter, cell_context, table, td, r, c, &cursorBlockNeedingRepaint,
                              &offsetOfRepaintedCursorBlock);
            }
        }

    } else {
        drawFrameDecoration(painter, frame, fd, context.clip, frameRect);

        BOBUIextFrame::Iterator it = frame->begin();

        if (frame == docPrivate->rootFrame())
            it = frameIteratorForYPosition(QFixed::fromReal(context.clip.top()));

        QList<BOBUIextFrame *> floats;
        const int numFloats = fd->floats.size();
        floats.reserve(numFloats);
        for (int i = 0; i < numFloats; ++i)
            floats.append(fd->floats.at(i));

        drawFlow(off, painter, context, it, floats, &cursorBlockNeedingRepaint);
    }

    if (cursorBlockNeedingRepaint.isValid()) {
        const QPen oldPen = painter->pen();
        painter->setPen(context.palette.color(QPalette::Text));
        const int cursorPos = context.cursorPosition - cursorBlockNeedingRepaint.position();
        cursorBlockNeedingRepaint.layout()->drawCursor(painter, offsetOfRepaintedCursorBlock,
                                                       cursorPos, cursorWidth);
        painter->setPen(oldPen);
    }

    return;
}

#ifndef BOBUI_NO_CSSPARSER

static inline BOBUIextFormat::Property borderPropertyForEdge(QCss::Edge edge)
{
    switch (edge) {
    case QCss::TopEdge:
        return BOBUIextFormat::TableCellTopBorder;
    case QCss::BottomEdge:
        return BOBUIextFormat::TableCellBottomBorder;
    case QCss::LeftEdge:
        return BOBUIextFormat::TableCellLeftBorder;
    case QCss::RightEdge:
        return BOBUIextFormat::TableCellRightBorder;
    default:
        Q_UNREACHABLE_RETURN(BOBUIextFormat::UserProperty);
    }
}

static inline BOBUIextFormat::Property borderStylePropertyForEdge(QCss::Edge edge)
{
    switch (edge) {
    case QCss::TopEdge:
        return BOBUIextFormat::TableCellTopBorderStyle;
    case QCss::BottomEdge:
        return BOBUIextFormat::TableCellBottomBorderStyle;
    case QCss::LeftEdge:
        return BOBUIextFormat::TableCellLeftBorderStyle;
    case QCss::RightEdge:
        return BOBUIextFormat::TableCellRightBorderStyle;
    default:
        Q_UNREACHABLE_RETURN(BOBUIextFormat::UserProperty);
    }
}

static inline QCss::Edge adjacentEdge(QCss::Edge edge)
{
    switch (edge) {
    case QCss::TopEdge:
        return QCss::BottomEdge;
    case QCss::RightEdge:
        return QCss::LeftEdge;
    case QCss::BottomEdge:
        return QCss::TopEdge;
    case QCss::LeftEdge:
        return QCss::RightEdge;
    default:
        Q_UNREACHABLE_RETURN(QCss::NumEdges);
    }
}

static inline bool isSameAxis(QCss::Edge e1, QCss::Edge e2)
{
    return e1 == e2 || e1 == adjacentEdge(e2);
}

static inline bool isVerticalAxis(QCss::Edge e)
{
    return e % 2 > 0;
}

static inline BOBUIextTableCell adjacentCell(BOBUIextTable *table, const BOBUIextTableCell &cell,
                                          QCss::Edge edge)
{
    int dc = 0;
    int dr = 0;

    switch (edge) {
    case QCss::LeftEdge:
        dc = -1;
        break;
    case QCss::RightEdge:
        dc = cell.columnSpan();
        break;
    case QCss::TopEdge:
        dr = -1;
        break;
    case QCss::BottomEdge:
        dr = cell.rowSpan();
        break;
    default:
        Q_UNREACHABLE();
        break;
    }

    // get sibling cell
    int col = cell.column() + dc;
    int row = cell.row() + dr;

    if (col < 0 || row < 0 || col >= table->columns() || row >= table->rows())
        return BOBUIextTableCell();
    else
        return table->cellAt(cell.row() + dr, cell.column() + dc);
}

// returns true if the specified edges of both cells
// are "one the same line" aka axis.
//
// | C0
// |-----|-----|----|-----  < "axis"
// | C1  | C2  | C3 | C4
//
// cell    edge    competingCell competingEdge  result
// C0      Left    C1            Left           true
// C0      Left    C2            Left           false
// C0      Bottom  C2            Top            true
// C0      Bottom  C4            Left           INVALID
static inline bool sharesAxis(const BOBUIextTableCell &cell, QCss::Edge edge,
                              const BOBUIextTableCell &competingCell, QCss::Edge competingCellEdge)
{
    Q_ASSERT(isVerticalAxis(edge) == isVerticalAxis(competingCellEdge));

    switch (edge) {
    case QCss::TopEdge:
        return cell.row() ==
                competingCell.row() + (competingCellEdge == QCss::BottomEdge ? competingCell.rowSpan() : 0);
    case QCss::BottomEdge:
        return cell.row() + cell.rowSpan() ==
                competingCell.row() + (competingCellEdge == QCss::TopEdge ? 0 : competingCell.rowSpan());
    case QCss::LeftEdge:
        return cell.column() ==
                competingCell.column() + (competingCellEdge == QCss::RightEdge ? competingCell.columnSpan() : 0);
    case QCss::RightEdge:
        return cell.column() + cell.columnSpan() ==
                competingCell.column() + (competingCellEdge == QCss::LeftEdge ? 0 : competingCell.columnSpan());
    default:
        Q_UNREACHABLE_RETURN(false);
    }
}

// returns the applicable EdgeData for the given cell and edge.
// this is either set explicitly by the cell's format, an activated grid
// or the general table border width for outermost edges.
static inline EdgeData cellEdgeData(BOBUIextTable *table, const BOBUIextTableData *td,
                                    const BOBUIextTableCell &cell, QCss::Edge edge)
{
    if (!cell.isValid()) {
        // e.g. non-existing adjacent cell
        return EdgeData();
    }

    BOBUIextTableCellFormat f = cell.format().toTableCellFormat();
    if (f.hasProperty(borderStylePropertyForEdge(edge))) {
        // border style is set
        double width = 3; // default to 3 like browsers do
        if (f.hasProperty(borderPropertyForEdge(edge)))
            width = f.property(borderPropertyForEdge(edge)).toDouble();
        return EdgeData(width, cell, edge, EdgeData::ClassExplicit);
    } else if (td->drawGrid) {
        const bool outermost =
                (edge == QCss::LeftEdge && cell.column() == 0) ||
                (edge == QCss::TopEdge && cell.row() == 0) ||
                (edge == QCss::RightEdge && cell.column() + cell.columnSpan() >= table->columns()) ||
                (edge == QCss::BottomEdge && cell.row() + cell.rowSpan() >= table->rows());

        if (outermost) {
            qreal border = table->format().border();
            if (border > 1.0) {
                // table border
                return EdgeData(border, cell, edge, EdgeData::ClassTableBorder);
            }
        }
        // 1px clean grid
        return EdgeData(1.0, cell, edge, EdgeData::ClassGrid);
    }
    else {
        return EdgeData(0, cell, edge, EdgeData::ClassNone);
    }
}

// returns the EdgeData with the larger width of either the cell's edge its adjacent cell's edge
static inline EdgeData axisEdgeData(BOBUIextTable *table, const BOBUIextTableData *td,
                                    const BOBUIextTableCell &cell, QCss::Edge edge)
{
    Q_ASSERT(cell.isValid());

    EdgeData result = cellEdgeData(table, td, cell, edge);
    if (!td->borderCollapse)
        return result;

    BOBUIextTableCell ac = adjacentCell(table, cell, edge);
    result = qMax(result, cellEdgeData(table, td, ac, adjacentEdge(edge)));

    bool mustCheckThirdCell = false;
    if (ac.isValid()) {
        /* if C0 and C3 don't share the left/top axis, we must
         * also check C1.
         *
         * C0 and C4 don't share the left axis so we have
         * to take the top edge of C1 (T1) into account
         * because this might be wider than C0's bottom
         * edge (B0). For the sake of simplicity we skip
         * checking T2 and T3.
         *
         * | C0
         * |-----|-----|----|-----
         * | C1  | C2  | C3 | C4
         *
         * width(T4) = max(T4, B0, T1) (T2 and T3 won't be checked)
         */
        switch (edge) {
        case QCss::TopEdge:
        case QCss::BottomEdge:
            mustCheckThirdCell = !sharesAxis(cell, QCss::LeftEdge, ac, QCss::LeftEdge);
            break;
        case QCss::LeftEdge:
        case QCss::RightEdge:
            mustCheckThirdCell = !sharesAxis(cell, QCss::TopEdge, ac, QCss::TopEdge);
            break;
        default:
            Q_UNREACHABLE();
            break;
        }
    }

    if (mustCheckThirdCell)
        result = qMax(result, cellEdgeData(table, td, adjacentCell(table, ac, adjacentEdge(edge)), edge));

    return result;
}

// checks an edge's joined competing edge according to priority rules and
// adjusts maxCompetingEdgeData and maxOrthogonalEdgeData
static inline void checkJoinedEdge(BOBUIextTable *table, const BOBUIextTableData *td, const BOBUIextTableCell &cell,
                                   QCss::Edge competingEdge,
                                   const EdgeData &edgeData,
                                   bool couldHaveContinuation,
                                   EdgeData *maxCompetingEdgeData,
                                   EdgeData *maxOrthogonalEdgeData)
{
    EdgeData competingEdgeData = axisEdgeData(table, td, cell, competingEdge);

    if (competingEdgeData > edgeData) {
        *maxCompetingEdgeData = competingEdgeData;
    } else if (competingEdgeData.width == edgeData.width) {
        if ((isSameAxis(edgeData.edge, competingEdge) && couldHaveContinuation)
                || (!isVerticalAxis(edgeData.edge) && isVerticalAxis(competingEdge)) /* both widths are equal, vertical edge has priority */ ) {
            *maxCompetingEdgeData = competingEdgeData;
        }
    }

    if (maxOrthogonalEdgeData && competingEdgeData.width > maxOrthogonalEdgeData->width)
        *maxOrthogonalEdgeData = competingEdgeData;
}

// the offset to make adjacent edges overlap in border collapse mode
static inline qreal collapseOffset(const BOBUIextDocumentLayoutPrivate *p, const EdgeData &w)
{
    return p->scaleToDevice(w.width) / 2.0;
}

// returns the offset that must be applied to the edge's
// anchor (start point or end point) to avoid overlapping edges.
//
// Example 1:
//       2
//       2
// 11111144444444      4 = top edge of cell, 4 pixels width
//       3             3 = right edge of cell, 3 pixels width
//       3 cell 4
//
// cell 4's top border is the widest border and will be
// drawn with horiz. offset = -3/2 whereas its left border
// of width 3 will be drawn with vert. offset = +4/2.
//
// Example 2:
//       2
//       2
// 11111143333333
//       4
//       4 cell 4
//
// cell 4's left border is the widest and will be drawn
// with vert. offset = -3/2 whereas its top border
// of of width 3 will be drawn with hor. offset = +4/2.
//
// couldHaveContinuation: true for "end" anchor of an edge:
//      C
// AAAAABBBBBB
//      D
// width(A) == width(B) we consider B to be a continuation of A, so that B wins
// and will be painted. A would only be painted including the right anchor if
// there was no edge B (due to a rowspan or the axis C-D being the table's right
// border).
//
// ignoreEdgesAbove: true if an edge (left, right or top) for the first row
// after a table page break should be painted. In this case the edges of the
// row above must be ignored.
static inline double prioritizedEdgeAnchorOffset(const BOBUIextDocumentLayoutPrivate *p,
                                                 BOBUIextTable *table, const BOBUIextTableData *td,
                                                 const BOBUIextTableCell &cell,
                                                 const EdgeData &edgeData,
                                                 QCss::Edge orthogonalEdge,
                                                 bool couldHaveContinuation,
                                                 bool ignoreEdgesAbove)
{
    EdgeData maxCompetingEdgeData;
    EdgeData maxOrthogonalEdgeData;
    BOBUIextTableCell competingCell;

    // reference scenario for the inline comments:
    // - edgeData being the top "T0" edge of C0
    // - right anchor is '+', orthogonal edge is "R0"
    //   B C3 R|L C2 B
    //   ------+------
    //   T C0 R|L C1 T

    // C0: T0/B3
    // this is "edgeData"

    // C0: R0/L1
    checkJoinedEdge(table, td, cell, orthogonalEdge, edgeData, false,
                    &maxCompetingEdgeData, &maxOrthogonalEdgeData);

    if (td->borderCollapse) {
        // C1: T1/B2
        if (!isVerticalAxis(edgeData.edge) || !ignoreEdgesAbove) {
            competingCell = adjacentCell(table, cell, orthogonalEdge);
            if (competingCell.isValid()) {
                checkJoinedEdge(table, td, competingCell, edgeData.edge, edgeData, couldHaveContinuation,
                                &maxCompetingEdgeData, nullptr);
            }
        }

        // C3: R3/L2
        if (edgeData.edge != QCss::TopEdge || !ignoreEdgesAbove) {
            competingCell = adjacentCell(table, cell, edgeData.edge);
            if (competingCell.isValid() && sharesAxis(cell, orthogonalEdge, competingCell, orthogonalEdge)) {
                checkJoinedEdge(table, td, competingCell, orthogonalEdge, edgeData, false,
                                &maxCompetingEdgeData, &maxOrthogonalEdgeData);
            }
        }
    }

    // wider edge has priority
    bool hasPriority = edgeData > maxCompetingEdgeData;

    if (td->borderCollapse) {
        qreal offset = collapseOffset(p, maxOrthogonalEdgeData);
        return hasPriority ? -offset : offset;
    }
    else
        return hasPriority ? 0 : p->scaleToDevice(maxOrthogonalEdgeData.width);
}

// draw one edge of the given cell
//
// these options are for pagination / pagebreak handling:
//
// forceHeaderRow: true for all rows directly below a (repeated) header row.
//  if the table has headers the first row after a page break must check against
//  the last table header's row, not its actual predecessor.
//
// adjustTopAnchor: false for rows that are a continuation of a row after a page break
//   only evaluated for left/right edges
//
// adjustBottomAnchor: false for rows that will continue after a page break
//   only evaluated for left/right edges
//
// ignoreEdgesAbove: true if a row starts on top of the page and the
//   bottom edges of the prior row can therefore be ignored.
static inline
void drawCellBorder(const BOBUIextDocumentLayoutPrivate *p, QPainter *painter,
                    BOBUIextTable *table, const BOBUIextTableData *td, const BOBUIextTableCell &cell,
                    const QRectF &borderRect, QCss::Edge edge,
                    int forceHeaderRow, bool adjustTopAnchor, bool adjustBottomAnchor,
                    bool ignoreEdgesAbove)
{
    QPointF p1, p2;
    qreal wh = 0;
    qreal wv = 0;
    EdgeData edgeData = axisEdgeData(table, td, cell, edge);

    if (edgeData.width == 0)
        return;

    BOBUIextTableCellFormat fmt = edgeData.cell.format().toTableCellFormat();
    BOBUIextFrameFormat::BorderStyle borderStyle = BOBUIextFrameFormat::BorderStyle_None;
    QBrush brush;

    if (edgeData.edgeClass != EdgeData::ClassExplicit && td->drawGrid) {
        borderStyle = table->format().borderStyle();
        brush = table->format().borderBrush();
    }
    else {
        switch (edgeData.edge) {
        case QCss::TopEdge:
            brush = fmt.topBorderBrush();
            borderStyle = fmt.topBorderStyle();
            break;
        case QCss::BottomEdge:
            brush = fmt.bottomBorderBrush();
            borderStyle = fmt.bottomBorderStyle();
            break;
        case QCss::LeftEdge:
            brush = fmt.leftBorderBrush();
            borderStyle = fmt.leftBorderStyle();
            break;
        case QCss::RightEdge:
            brush = fmt.rightBorderBrush();
            borderStyle = fmt.rightBorderStyle();
            break;
        default:
            Q_UNREACHABLE();
            break;
        }
    }

    if (borderStyle == BOBUIextFrameFormat::BorderStyle_None)
        return;

    // assume black if not explicit brush is set
    if (brush.style() == BobUI::NoBrush)
        brush = BobUI::black;

    BOBUIextTableCell cellOrHeader = cell;
    if (forceHeaderRow != -1)
        cellOrHeader = table->cellAt(forceHeaderRow, cell.column());

    // adjust start and end anchors (e.g. left/right for top) according to priority rules
    switch (edge) {
    case QCss::TopEdge:
        wv = p->scaleToDevice(edgeData.width);
        p1 = borderRect.topLeft()
                + QPointF(qFloor(prioritizedEdgeAnchorOffset(p, table, td, cell, edgeData, QCss::LeftEdge, false, ignoreEdgesAbove)), 0);
        p2 = borderRect.topRight()
                + QPointF(-qCeil(prioritizedEdgeAnchorOffset(p, table, td, cell, edgeData, QCss::RightEdge, true, ignoreEdgesAbove)), 0);
        break;
    case QCss::BottomEdge:
        wv = p->scaleToDevice(edgeData.width);
        p1 = borderRect.bottomLeft()
                + QPointF(qFloor(prioritizedEdgeAnchorOffset(p, table, td, cell, edgeData, QCss::LeftEdge, false, false)), -wv);
        p2 = borderRect.bottomRight()
                + QPointF(-qCeil(prioritizedEdgeAnchorOffset(p, table, td, cell, edgeData, QCss::RightEdge, true, false)), -wv);
        break;
    case QCss::LeftEdge:
        wh = p->scaleToDevice(edgeData.width);
        p1 = borderRect.topLeft()
                + QPointF(0, adjustTopAnchor ? qFloor(prioritizedEdgeAnchorOffset(p, table, td, cellOrHeader, edgeData,
                                                                                  forceHeaderRow != -1 ? QCss::BottomEdge : QCss::TopEdge,
                                                                                  false, ignoreEdgesAbove))
                                             : 0);
        p2 = borderRect.bottomLeft()
                + QPointF(0, adjustBottomAnchor ? -qCeil(prioritizedEdgeAnchorOffset(p, table, td, cell, edgeData, QCss::BottomEdge, true, false))
                                                : 0);
        break;
    case QCss::RightEdge:
        wh = p->scaleToDevice(edgeData.width);
        p1 = borderRect.topRight()
                + QPointF(-wh, adjustTopAnchor ? qFloor(prioritizedEdgeAnchorOffset(p, table, td, cellOrHeader, edgeData,
                                                                                    forceHeaderRow != -1 ? QCss::BottomEdge : QCss::TopEdge,
                                                                                    false, ignoreEdgesAbove))
                                               : 0);
        p2 = borderRect.bottomRight()
                + QPointF(-wh, adjustBottomAnchor ? -qCeil(prioritizedEdgeAnchorOffset(p, table, td, cell, edgeData, QCss::BottomEdge, true, false))
                                                  : 0);
        break;
    default: break;
    }

    // for borderCollapse move edge width/2 pixel out of the borderRect
    // so that it shares space with the adjacent cell's edge.
    // to avoid fractional offsets, qCeil/qFloor is used
    if (td->borderCollapse) {
        QPointF offset;
        switch (edge) {
        case QCss::TopEdge:
            offset = QPointF(0, -qCeil(collapseOffset(p, edgeData)));
            break;
        case QCss::BottomEdge:
            offset = QPointF(0, qFloor(collapseOffset(p, edgeData)));
            break;
        case QCss::LeftEdge:
            offset = QPointF(-qCeil(collapseOffset(p, edgeData)), 0);
            break;
        case QCss::RightEdge:
            offset = QPointF(qFloor(collapseOffset(p, edgeData)), 0);
            break;
        default: break;
        }
        p1 += offset;
        p2 += offset;
    }

    QCss::BorderStyle cssStyle = static_cast<QCss::BorderStyle>(borderStyle + 1);

// this reveals errors in the drawing logic
#ifdef COLLAPSE_DEBUG
    QColor c = brush.color();
    c.setAlpha(150);
    brush.setColor(c);
#endif

    qDrawEdge(painter, p1.x(), p1.y(), p2.x() + wh, p2.y() + wv, 0, 0, edge, cssStyle, brush);
}
#endif

void BOBUIextDocumentLayoutPrivate::drawTableCellBorder(const QRectF &cellRect, QPainter *painter,
                                                     BOBUIextTable *table, BOBUIextTableData *td,
                                                     const BOBUIextTableCell &cell) const
{
#ifndef BOBUI_NO_CSSPARSER
    qreal topMarginAfterPageBreak = (td->effectiveTopMargin + td->cellSpacing + td->border).toReal();
    qreal bottomMargin = (td->effectiveBottomMargin + td->cellSpacing + td->border).toReal();

    const int headerRowCount = qMin(table->format().headerRowCount(), table->rows() - 1);
    if (headerRowCount > 0 && cell.row() >= headerRowCount)
        topMarginAfterPageBreak += td->headerHeight.toReal();

    BorderPaginator paginator(document, cellRect, topMarginAfterPageBreak, bottomMargin, 0);

    bool turn_off_antialiasing = !(painter->renderHints() & QPainter::Antialiasing);
    painter->setRenderHint(QPainter::Antialiasing);

    // paint cell borders for every page the cell appears on
    for (int page = paginator.topPage; page <= paginator.bottomPage; ++page) {
        const QRectF clipped = paginator.clipRect(page);
        if (!clipped.isValid())
            continue;

        const qreal offset = cellRect.top() - td->rowPositions.at(cell.row()).toReal();
        const int lastHeaderRow = table->format().headerRowCount() - 1;
        const bool tableHasHeader = table->format().headerRowCount() > 0;
        const bool isHeaderRow = cell.row() < table->format().headerRowCount();
        const bool isFirstRow = cell.row() == lastHeaderRow + 1;
        const bool isLastRow = cell.row() + cell.rowSpan() >= table->rows();
        const bool previousRowOnPreviousPage = !isFirstRow
                && !isHeaderRow
                && BorderPaginator(document,
                                   td->cellRect(adjacentCell(table, cell, QCss::TopEdge)).translated(0, offset),
                                   topMarginAfterPageBreak,
                                   bottomMargin,
                                   0).bottomPage < page;
        const bool nextRowOnNextPage = !isLastRow
                && BorderPaginator(document,
                                   td->cellRect(adjacentCell(table, cell, QCss::BottomEdge)).translated(0, offset),
                                   topMarginAfterPageBreak,
                                   bottomMargin,
                                   0).topPage > page;
        const bool rowStartsOnPage = page == paginator.topPage;
        const bool rowEndsOnPage = page == paginator.bottomPage;
        const bool rowStartsOnPageTop = !tableHasHeader
                && rowStartsOnPage
                && previousRowOnPreviousPage;
        const bool rowStartsOnPageBelowHeader = tableHasHeader
                && rowStartsOnPage
                && previousRowOnPreviousPage;

        const bool suppressTopBorder = td->borderCollapse
                ? !isHeaderRow && (!rowStartsOnPage || rowStartsOnPageBelowHeader)
                : !rowStartsOnPage;
        const bool suppressBottomBorder = td->borderCollapse
                ? !isHeaderRow && (!rowEndsOnPage || nextRowOnNextPage)
                : !rowEndsOnPage;
        const bool doNotAdjustTopAnchor = td->borderCollapse
                ? !tableHasHeader && !rowStartsOnPage
                : !rowStartsOnPage;
        const bool doNotAdjustBottomAnchor = suppressBottomBorder;

        if (!suppressTopBorder) {
            drawCellBorder(this, painter, table, td, cell, clipped, QCss::TopEdge,
                           -1, true, true, rowStartsOnPageTop);
        }

        drawCellBorder(this, painter, table, td, cell, clipped, QCss::LeftEdge,
                       suppressTopBorder ? lastHeaderRow : -1,
                       !doNotAdjustTopAnchor,
                       !doNotAdjustBottomAnchor,
                       rowStartsOnPageTop);
        drawCellBorder(this, painter, table, td, cell, clipped, QCss::RightEdge,
                       suppressTopBorder ? lastHeaderRow : -1,
                       !doNotAdjustTopAnchor,
                       !doNotAdjustBottomAnchor,
                       rowStartsOnPageTop);

        if (!suppressBottomBorder) {
            drawCellBorder(this, painter, table, td, cell, clipped, QCss::BottomEdge,
                           -1, true, true, false);
        }
    }

    if (turn_off_antialiasing)
        painter->setRenderHint(QPainter::Antialiasing, false);
#else
    Q_UNUSED(cell);
    Q_UNUSED(cellRect);
    Q_UNUSED(painter);
    Q_UNUSED(table);
    Q_UNUSED(td);
    Q_UNUSED(cell);
#endif
}

void BOBUIextDocumentLayoutPrivate::drawTableCell(const QRectF &cellRect, QPainter *painter, const QAbstractTextDocumentLayout::PaintContext &cell_context,
                                               BOBUIextTable *table, BOBUIextTableData *td, int r, int c,
                                               BOBUIextBlock *cursorBlockNeedingRepaint, QPointF *cursorBlockOffset) const
{
    BOBUIextTableCell cell = table->cellAt(r, c);
    int rspan = cell.rowSpan();
    int cspan = cell.columnSpan();
    if (rspan != 1) {
        int cr = cell.row();
        if (cr != r)
            return;
    }
    if (cspan != 1) {
        int cc = cell.column();
        if (cc != c)
            return;
    }

    const QFixed leftPadding = td->leftPadding(table, cell);
    const QFixed topPadding = td->topPadding(table, cell);

    qreal topMargin = (td->effectiveTopMargin + td->cellSpacing + td->border).toReal();
    qreal bottomMargin = (td->effectiveBottomMargin + td->cellSpacing + td->border).toReal();

    const int headerRowCount = qMin(table->format().headerRowCount(), table->rows() - 1);
    if (r >= headerRowCount)
        topMargin += td->headerHeight.toReal();

    // If cell border configured, don't draw default border for cells. It will be taken care later by
    // drawTableCellBorder().
    bool cellBorderConfigured = (cell.format().hasProperty(BOBUIextFormat::TableCellLeftBorder) ||
                                 cell.format().hasProperty(BOBUIextFormat::TableCellTopBorder) ||
                                 cell.format().hasProperty(BOBUIextFormat::TableCellRightBorder) ||
                                 cell.format().hasProperty(BOBUIextFormat::TableCellBottomBorder));

    if (!td->borderCollapse && td->border != 0 && !cellBorderConfigured) {
        const QBrush oldBrush = painter->brush();
        const QPen oldPen = painter->pen();

        // If border is configured for the table (and not explicitly for the cell), then
        // always draw 1px border around the cell
        const qreal border = 1;

        QRectF borderRect(cellRect.left() - border, cellRect.top() - border, cellRect.width() + border, cellRect.height() + border);

        // invert the border style for cells
        BOBUIextFrameFormat::BorderStyle cellBorder = table->format().borderStyle();
        switch (cellBorder) {
        case BOBUIextFrameFormat::BorderStyle_Inset:
            cellBorder = BOBUIextFrameFormat::BorderStyle_Outset;
            break;
        case BOBUIextFrameFormat::BorderStyle_Outset:
            cellBorder = BOBUIextFrameFormat::BorderStyle_Inset;
            break;
        case BOBUIextFrameFormat::BorderStyle_Groove:
            cellBorder = BOBUIextFrameFormat::BorderStyle_Ridge;
            break;
        case BOBUIextFrameFormat::BorderStyle_Ridge:
            cellBorder = BOBUIextFrameFormat::BorderStyle_Groove;
            break;
        default:
            break;
        }

        drawBorder(painter, borderRect, topMargin, bottomMargin,
                   border, table->format().borderBrush(), cellBorder);

        painter->setBrush(oldBrush);
        painter->setPen(oldPen);
    }

    const QBrush bg = cell.format().background();
    const QPointF brushOrigin = painter->brushOriginF();
    if (bg.style() != BobUI::NoBrush) {
        const qreal pageHeight = document->pageSize().height();
        const int topPage = pageHeight > 0 ? static_cast<int>(cellRect.top() / pageHeight) : 0;
        const int bottomPage = pageHeight > 0 ? static_cast<int>((cellRect.bottom()) / pageHeight) : 0;

        if (topPage == bottomPage)
            fillBackground(painter, cellRect, bg, cellRect.topLeft());
        else {
            for (int i = topPage; i <= bottomPage; ++i) {
                QRectF clipped = cellRect.toRect();

                if (topPage != bottomPage) {
                    const qreal top = qMax(i * pageHeight + topMargin, cell_context.clip.top());
                    const qreal bottom = qMin((i + 1) * pageHeight - bottomMargin, cell_context.clip.bottom());

                    clipped.setTop(qMax(clipped.top(), top));
                    clipped.setBottom(qMin(clipped.bottom(), bottom));

                    if (clipped.bottom() <= clipped.top())
                        continue;

                    fillBackground(painter, clipped, bg, cellRect.topLeft());
                }
            }
        }

        if (bg.style() > BobUI::SolidPattern)
            painter->setBrushOrigin(cellRect.topLeft());
    }

    // paint over the background - otherwise we would have to adjust the background paint cellRect for the border values
    if (cellBorderConfigured || (td->borderCollapse && td->border != 0))
        drawTableCellBorder(cellRect, painter, table, td, cell);

    const QFixed verticalOffset = td->cellVerticalOffsets.at(c + r * table->columns());

    const QPointF cellPos = QPointF(cellRect.left() + leftPadding.toReal(),
                                    cellRect.top() + (topPadding + verticalOffset).toReal());

    BOBUIextBlock repaintBlock;
    drawFlow(cellPos, painter, cell_context, cell.begin(),
             td->childFrameMap.values(r + c * table->rows()),
             &repaintBlock);
    if (repaintBlock.isValid()) {
        *cursorBlockNeedingRepaint = repaintBlock;
        *cursorBlockOffset = cellPos;
    }

    if (bg.style() > BobUI::SolidPattern)
        painter->setBrushOrigin(brushOrigin);
}

void BOBUIextDocumentLayoutPrivate::drawFlow(const QPointF &offset, QPainter *painter, const QAbstractTextDocumentLayout::PaintContext &context,
                                          BOBUIextFrame::Iterator it, const QList<BOBUIextFrame *> &floats, BOBUIextBlock *cursorBlockNeedingRepaint) const
{
    Q_Q(const BOBUIextDocumentLayout);
    const bool inRootFrame = (!it.atEnd() && it.parentFrame() && it.parentFrame()->parentFrame() == nullptr);

    auto lastVisibleCheckPoint = checkPoints.end();
    if (inRootFrame && context.clip.isValid()) {
        lastVisibleCheckPoint = std::lower_bound(checkPoints.begin(), checkPoints.end(), QFixed::fromReal(context.clip.bottom()));
    }

    BOBUIextBlock previousBlock;
    BOBUIextFrame *previousFrame = nullptr;

    for (; !it.atEnd(); ++it) {
        BOBUIextFrame *c = it.currentFrame();

        if (inRootFrame && !checkPoints.isEmpty()) {
            int currentPosInDoc;
            if (c)
                currentPosInDoc = c->firstPosition();
            else
                currentPosInDoc = it.currentBlock().position();

            // if we're past what is already laid out then we're better off
            // not trying to draw things that may not be positioned correctly yet
            if (currentPosInDoc >= checkPoints.constLast().positionInFrame)
                break;

            if (lastVisibleCheckPoint != checkPoints.end()
                && context.clip.isValid()
                && currentPosInDoc >= lastVisibleCheckPoint->positionInFrame
               )
                break;
        }

        if (c)
            drawFrame(offset, painter, context, c);
        else {
            QAbstractTextDocumentLayout::PaintContext pc = context;
            if (isEmptyBlockAfterTable(it.currentBlock(), previousFrame))
                pc.selections.clear();
            drawBlock(offset, painter, pc, it.currentBlock(), inRootFrame);
        }

        // when entering a table and the previous block is empty
        // then layoutFlow 'hides' the block that just causes a
        // new line by positioning it /on/ the table border. as we
        // draw that block before the table itself the decoration
        // 'overpaints' the cursor and we need to paint it afterwards
        // again
        if (isEmptyBlockBeforeTable(previousBlock, previousBlock.blockFormat(), it)
            && previousBlock.contains(context.cursorPosition)
           ) {
            *cursorBlockNeedingRepaint = previousBlock;
        }

        previousBlock = it.currentBlock();
        previousFrame = c;
    }

    for (int i = 0; i < floats.size(); ++i) {
        BOBUIextFrame *frame = floats.at(i);
        if (!isFrameFromInlineObject(frame)
            || frame->frameFormat().position() == BOBUIextFrameFormat::InFlow)
            continue;

        const int pos = frame->firstPosition() - 1;
        BOBUIextCharFormat format = const_cast<BOBUIextDocumentLayout *>(q)->format(pos);
        BOBUIextObjectInterface *handler = q->handlerForObject(format.objectType());
        if (handler) {
            QRectF rect = frameBoundingRectInternal(frame);
            handler->drawObject(painter, rect, document, pos, format);
        }
    }
}

void BOBUIextDocumentLayoutPrivate::drawBlock(const QPointF &offset, QPainter *painter,
                                           const QAbstractTextDocumentLayout::PaintContext &context,
                                           const BOBUIextBlock &bl, bool inRootFrame) const
{
    const BOBUIextLayout *tl = bl.layout();
    QRectF r = tl->boundingRect();
    r.translate(offset + tl->position());
    if (!bl.isVisible() || (context.clip.isValid() && (r.bottom() < context.clip.y() || r.top() > context.clip.bottom())))
        return;
    qCDebug(lcDraw) << "drawBlock" << bl.position() << "at" << offset << "br" << tl->boundingRect();

    BOBUIextBlockFormat blockFormat = bl.blockFormat();

    QBrush bg = blockFormat.background();
    if (bg != BobUI::NoBrush) {
        QRectF rect = r;

        // extend the background rectangle if we're in the root frame with NoWrap,
        // as the rect of the text block will then be only the width of the text
        // instead of the full page width
        if (inRootFrame && document->pageSize().width() <= 0) {
            const BOBUIextFrameData *fd = data(document->rootFrame());
            rect.setRight((fd->size.width - fd->rightMargin).toReal());
        }

        // in the case of <hr>, the background-color CSS style fills only the rule's thickness instead of the whole line
        if (!blockFormat.hasProperty(BOBUIextFormat::BlockTrailingHorizontalRulerWidth))
            fillBackground(painter, rect, bg, r.topLeft());
    }

    QList<BOBUIextLayout::FormatRange> selections;
    int blpos = bl.position();
    int bllen = bl.length();
    const BOBUIextCharFormat *selFormat = nullptr;
    for (int i = 0; i < context.selections.size(); ++i) {
        const QAbstractTextDocumentLayout::Selection &range = context.selections.at(i);
        const int selStart = range.cursor.selectionStart() - blpos;
        const int selEnd = range.cursor.selectionEnd() - blpos;
        if (selStart < bllen && selEnd > 0
             && selEnd > selStart) {
            BOBUIextLayout::FormatRange o;
            o.start = selStart;
            o.length = selEnd - selStart;
            o.format = range.format;
            selections.append(o);
        } else if (! range.cursor.hasSelection() && range.format.hasProperty(BOBUIextFormat::FullWidthSelection)
                   && bl.contains(range.cursor.position())) {
            // for full width selections we don't require an actual selection, just
            // a position to specify the line. that's more convenience in usage.
            BOBUIextLayout::FormatRange o;
            BOBUIextLine l = tl->lineForTextPosition(range.cursor.position() - blpos);
            o.start = l.textStart();
            o.length = l.textLength();
            if (o.start + o.length == bllen - 1)
                ++o.length; // include newline
            o.format = range.format;
            selections.append(o);
       }
        if (selStart < 0 && selEnd >= 1)
            selFormat = &range.format;
    }

    BOBUIextObject *object = document->objectForFormat(bl.blockFormat());
    if (object && object->format().toListFormat().style() != BOBUIextListFormat::ListStyleUndefined)
        drawListItem(offset, painter, context, bl, selFormat);

    QPen oldPen = painter->pen();
    painter->setPen(context.palette.color(QPalette::Text));

    tl->draw(painter, offset, selections, context.clip.isValid() ? (context.clip & clipRect) : clipRect);

    // if the block is empty and it precedes a table, do not draw the cursor.
    // the cursor is drawn later after the table has been drawn so no need
    // to draw it here.
    if (!isEmptyBlockBeforeTable(frameIteratorForTextPosition(blpos))
        && ((context.cursorPosition >= blpos && context.cursorPosition < blpos + bllen)
            || (context.cursorPosition < -1 && !tl->preeditAreaText().isEmpty()))) {
        int cpos = context.cursorPosition;
        if (cpos < -1)
            cpos = tl->preeditAreaPosition() - (cpos + 2);
        else
            cpos -= blpos;
        tl->drawCursor(painter, offset, cpos, cursorWidth);
    }

    if (blockFormat.hasProperty(BOBUIextFormat::BlockTrailingHorizontalRulerWidth)) {
        const qreal width = blockFormat.lengthProperty(BOBUIextFormat::BlockTrailingHorizontalRulerWidth).value(r.width());
        const auto color = blockFormat.hasProperty(BOBUIextFormat::BackgroundBrush)
                         ? qvariant_cast<QBrush>(blockFormat.property(BOBUIextFormat::BackgroundBrush)).color()
                         : context.palette.color(QPalette::Inactive, QPalette::WindowText);
        painter->setPen(color);
        qreal y = r.bottom();
        if (bl.length() == 1)
            y = r.top() + r.height() / 2;

        const qreal middleX = r.left() + r.width() / 2;
        painter->drawLine(QLineF(middleX - width / 2, y, middleX + width / 2, y));
    }

    painter->setPen(oldPen);
}


void BOBUIextDocumentLayoutPrivate::drawListItem(const QPointF &offset, QPainter *painter,
                                              const QAbstractTextDocumentLayout::PaintContext &context,
                                              const BOBUIextBlock &bl, const BOBUIextCharFormat *selectionFormat) const
{
    Q_Q(const BOBUIextDocumentLayout);
    const BOBUIextBlockFormat blockFormat = bl.blockFormat();
    const BOBUIextCharFormat charFormat = bl.charFormat();
    QFont font(charFormat.font());
    if (q->paintDevice())
        font = QFont(font, q->paintDevice());

    const QFontMetrics fontMetrics(font);
    BOBUIextObject * const object = document->objectForFormat(blockFormat);
    const BOBUIextListFormat lf = object->format().toListFormat();
    int style = lf.style();
    QString itemText;
    QSizeF size;

    if (blockFormat.hasProperty(BOBUIextFormat::ListStyle))
        style = BOBUIextListFormat::Style(blockFormat.intProperty(BOBUIextFormat::ListStyle));

    BOBUIextLayout *layout = bl.layout();
    if (layout->lineCount() == 0)
        return;
    BOBUIextLine firstLine = layout->lineAt(0);
    Q_ASSERT(firstLine.isValid());
    QPointF pos = (offset + layout->position()).toPoint();
    BobUI::LayoutDirection dir = bl.textDirection();
    {
        QRectF textRect = firstLine.naturalTextRect();
        pos += textRect.topLeft().toPoint();
        if (dir == BobUI::RightToLeft)
            pos.rx() += textRect.width();
    }

    switch (style) {
    case BOBUIextListFormat::ListDecimal:
    case BOBUIextListFormat::ListLowerAlpha:
    case BOBUIextListFormat::ListUpperAlpha:
    case BOBUIextListFormat::ListLowerRoman:
    case BOBUIextListFormat::ListUpperRoman:
        itemText = static_cast<BOBUIextList *>(object)->itemText(bl);
        size.setWidth(fontMetrics.horizontalAdvance(itemText));
        size.setHeight(fontMetrics.height());
        break;

    case BOBUIextListFormat::ListSquare:
    case BOBUIextListFormat::ListCircle:
    case BOBUIextListFormat::ListDisc:
        size.setWidth(fontMetrics.lineSpacing() / 3);
        size.setHeight(size.width());
        break;

    case BOBUIextListFormat::ListStyleUndefined:
        return;
    default: return;
    }

    QRectF r(pos, size);

    qreal xoff = fontMetrics.horizontalAdvance(u' ');
    if (dir == BobUI::LeftToRight)
        xoff = -xoff - size.width();
    r.translate( xoff, (fontMetrics.height() / 2) - (size.height() / 2));

    painter->save();

    painter->setRenderHint(QPainter::Antialiasing);

    const bool marker = bl.blockFormat().marker() != BOBUIextBlockFormat::MarkerType::NoMarker;
    if (selectionFormat) {
        painter->setPen(QPen(selectionFormat->foreground(), 0));
        if (!marker)
            painter->fillRect(r, selectionFormat->background());
    } else {
        QBrush fg = charFormat.foreground();
        if (fg == BobUI::NoBrush)
            fg = context.palette.text();
        painter->setPen(QPen(fg, 0));
    }

    QBrush brush = context.palette.brush(QPalette::Text);

    if (marker) {
        int adj = fontMetrics.lineSpacing() / 6;
        r.adjust(-adj, 0, -adj, 0);
        const QRectF outer = r.adjusted(-adj, -adj, adj, adj);
        if (selectionFormat)
            painter->fillRect(outer, selectionFormat->background());
        if (bl.blockFormat().marker() == BOBUIextBlockFormat::MarkerType::Checked) {
            // ### BobUI7: render with QStyle / PE_IndicatorCheckBox. We don't currently
            // have access to that here, because it would be a widget dependency.
            painter->setPen(QPen(painter->pen().color(), 2));
            painter->drawLine(r.topLeft(), r.bottomRight());
            painter->drawLine(r.topRight(), r.bottomLeft());
            painter->setPen(QPen(painter->pen().color(), 0));
        }
        painter->drawRect(outer);
    }

    switch (style) {
    case BOBUIextListFormat::ListDecimal:
    case BOBUIextListFormat::ListLowerAlpha:
    case BOBUIextListFormat::ListUpperAlpha:
    case BOBUIextListFormat::ListLowerRoman:
    case BOBUIextListFormat::ListUpperRoman: {
        BOBUIextLayout layout(itemText, font, q->paintDevice());
        layout.setCacheEnabled(true);
        BOBUIextOption option(BobUI::AlignLeft | BobUI::AlignAbsolute);
        option.setTextDirection(dir);
        layout.setTextOption(option);
        layout.beginLayout();
        BOBUIextLine line = layout.createLine();
        if (line.isValid())
            line.setLeadingIncluded(true);
        layout.endLayout();
        layout.draw(painter, QPointF(r.left(), pos.y()));
        break;
    }
    case BOBUIextListFormat::ListSquare:
        if (!marker)
            painter->fillRect(r, painter->pen().brush());
        break;
    case BOBUIextListFormat::ListCircle:
        if (!marker)
            painter->drawEllipse(r.translated(0.5, 0.5)); // pixel align for sharper rendering
        break;
    case BOBUIextListFormat::ListDisc:
        if (!marker) {
            painter->setBrush(painter->pen().brush());
            painter->setPen(BobUI::NoPen);
            painter->drawEllipse(r);
        }
        break;
    case BOBUIextListFormat::ListStyleUndefined:
        break;
    default:
        break;
    }

    painter->restore();
}

static QFixed flowPosition(const BOBUIextFrame::iterator &it)
{
    if (it.atEnd())
        return 0;

    if (it.currentFrame()) {
        return data(it.currentFrame())->position.y;
    } else {
        BOBUIextBlock block = it.currentBlock();
        BOBUIextLayout *layout = block.layout();
        if (layout->lineCount() == 0)
            return QFixed::fromReal(layout->position().y());
        else
            return QFixed::fromReal(layout->position().y() + layout->lineAt(0).y());
    }
}

static QFixed firstChildPos(const BOBUIextFrame *f)
{
    return flowPosition(f->begin());
}

BOBUIextLayoutStruct BOBUIextDocumentLayoutPrivate::layoutCell(BOBUIextTable *t, const BOBUIextTableCell &cell, QFixed width,
                                                        int layoutFrom, int layoutTo, BOBUIextTableData *td,
                                                        QFixed absoluteTableY, bool withPageBreaks)
{
    qCDebug(lcTable) << "layoutCell";
    BOBUIextLayoutStruct layoutStruct;
    layoutStruct.frame = t;
    layoutStruct.minimumWidth = 0;
    layoutStruct.maximumWidth = QFIXED_MAX;
    layoutStruct.y = 0;

    const QFixed topPadding = td->topPadding(t, cell);
    if (withPageBreaks) {
        layoutStruct.frameY = absoluteTableY + td->rowPositions.at(cell.row()) + topPadding;
    }
    layoutStruct.x_left = 0;
    layoutStruct.x_right = width;
    // we get called with different widths all the time (for example for figuring
    // out the min/max widths), so we always have to do the full layout ;(
    // also when for example in a table layoutFrom/layoutTo affect only one cell,
    // making that one cell grow the available width of the other cells may change
    // (shrink) and therefore when layoutCell gets called for them they have to
    // be re-laid out, even if layoutFrom/layoutTo is not in their range. Hence
    // this line:

    layoutStruct.pageHeight = QFixed::fromReal(document->pageSize().height());
    if (layoutStruct.pageHeight < 0 || !withPageBreaks)
        layoutStruct.pageHeight = QFIXED_MAX;
    const int currentPage = layoutStruct.currentPage();

    layoutStruct.pageTopMargin = td->effectiveTopMargin
            + td->cellSpacing
            + td->border
            + td->paddingProperty(cell.format(), BOBUIextFormat::TableCellTopPadding); // top cell-border is not repeated

#ifndef BOBUI_NO_CSSPARSER
    const int headerRowCount = t->format().headerRowCount();
    if (td->borderCollapse && headerRowCount > 0) {
        // consider the header row's bottom edge width
        qreal headerRowBottomBorderWidth = axisEdgeData(t, td, t->cellAt(headerRowCount - 1, cell.column()), QCss::BottomEdge).width;
        layoutStruct.pageTopMargin += QFixed::fromReal(scaleToDevice(headerRowBottomBorderWidth) / 2);
    }
#endif

    layoutStruct.pageBottomMargin = td->effectiveBottomMargin + td->cellSpacing + td->effectiveBottomBorder + td->bottomPadding(t, cell);
    layoutStruct.pageBottom = (currentPage + 1) * layoutStruct.pageHeight - layoutStruct.pageBottomMargin;

    layoutStruct.fullLayout = true;

    QFixed pageTop = currentPage * layoutStruct.pageHeight + layoutStruct.pageTopMargin - layoutStruct.frameY;
    layoutStruct.y = qMax(layoutStruct.y, pageTop);

    const QList<BOBUIextFrame *> childFrames = td->childFrameMap.values(cell.row() + cell.column() * t->rows());
    for (int i = 0; i < childFrames.size(); ++i) {
        BOBUIextFrame *frame = childFrames.at(i);
        BOBUIextFrameData *cd = data(frame);
        cd->sizeDirty = true;
    }

    layoutFlow(cell.begin(), &layoutStruct, layoutFrom, layoutTo, width);

    QFixed floatMinWidth;

    // floats that are located inside the text (like inline images) aren't taken into account by
    // layoutFlow with regards to the cell height (layoutStruct->y), so for a safety measure we
    // do that here. For example with <td><img align="right" src="..." />blah</td>
    // when the image happens to be higher than the text
    for (int i = 0; i < childFrames.size(); ++i) {
        BOBUIextFrame *frame = childFrames.at(i);
        BOBUIextFrameData *cd = data(frame);

        if (frame->frameFormat().position() != BOBUIextFrameFormat::InFlow)
            layoutStruct.y = qMax(layoutStruct.y, cd->position.y + cd->size.height);

        floatMinWidth = qMax(floatMinWidth, cd->minimumWidth);
    }

    // constraint the maximum/minimumWidth by the minimum width of the fixed size floats,
    // to keep them visible
    layoutStruct.maximumWidth = qMax(layoutStruct.maximumWidth, floatMinWidth);
    layoutStruct.minimumWidth = qMax(layoutStruct.minimumWidth, floatMinWidth);

    // as floats in cells get added to the table's float list but must not affect
    // floats in other cells we must clear the list here.
    data(t)->floats.clear();

//    qDebug("layoutCell done");

    return layoutStruct;
}

#ifndef BOBUI_NO_CSSPARSER
static inline void findWidestOutermostBorder(BOBUIextTable *table, BOBUIextTableData *td,
                                             const BOBUIextTableCell &cell, QCss::Edge edge,
                                             qreal *outerBorders)
{
    EdgeData w = cellEdgeData(table, td, cell, edge);
    if (w.width > outerBorders[edge])
        outerBorders[edge] = w.width;
}
#endif

QRectF BOBUIextDocumentLayoutPrivate::layoutTable(BOBUIextTable *table, int layoutFrom, int layoutTo, QFixed parentY)
{
    qCDebug(lcTable) << "layoutTable from" << layoutFrom << "to" << layoutTo << "parentY" << parentY;
    BOBUIextTableData *td = static_cast<BOBUIextTableData *>(data(table));
    Q_ASSERT(td->sizeDirty);
    const int rows = table->rows();
    const int columns = table->columns();

    const BOBUIextTableFormat fmt = table->format();

    td->childFrameMap.clear();
    {
        const QList<BOBUIextFrame *> children = table->childFrames();
        for (int i = 0; i < children.size(); ++i) {
            BOBUIextFrame *frame = children.at(i);
            BOBUIextTableCell cell = table->cellAt(frame->firstPosition());
            td->childFrameMap.insert(cell.row() + cell.column() * rows, frame);
        }
    }

    QList<BOBUIextLength> columnWidthConstraints = fmt.columnWidthConstraints();
    if (columnWidthConstraints.size() != columns)
        columnWidthConstraints.resize(columns);
    Q_ASSERT(columnWidthConstraints.size() == columns);

    // borderCollapse will disable drawing the html4 style table cell borders
    // and draw a 1px grid instead. This also sets a fixed cellspacing
    // of 1px if border > 0 (for the grid) and ignore any explicitly set
    // cellspacing.
    td->borderCollapse = fmt.borderCollapse();
    td->borderCell = td->borderCollapse ? 0 : td->border;
    const QFixed cellSpacing = td->cellSpacing = QFixed::fromReal(scaleToDevice(td->borderCollapse ? 0 : fmt.cellSpacing())).round();

    td->drawGrid = (td->borderCollapse && fmt.border() >= 1);

    td->effectiveTopBorder = td->effectiveBottomBorder = td->effectiveLeftBorder = td->effectiveRightBorder = td->border;

#ifndef BOBUI_NO_CSSPARSER
    if (td->borderCollapse) {
        // find the widest borders of the outermost cells
        qreal outerBorders[QCss::NumEdges];
        for (int i = 0; i < QCss::NumEdges; ++i)
            outerBorders[i] = 0;

        for (int r = 0; r < rows; ++r) {
            if (r == 0) {
                for (int c = 0; c < columns; ++c)
                    findWidestOutermostBorder(table, td, table->cellAt(r, c), QCss::TopEdge, outerBorders);
            }
            if (r == rows - 1) {
                for (int c = 0; c < columns; ++c)
                    findWidestOutermostBorder(table, td, table->cellAt(r, c), QCss::BottomEdge, outerBorders);
            }
            findWidestOutermostBorder(table, td, table->cellAt(r, 0), QCss::LeftEdge, outerBorders);
            findWidestOutermostBorder(table, td, table->cellAt(r, columns - 1), QCss::RightEdge, outerBorders);
        }
        td->effectiveTopBorder = QFixed::fromReal(scaleToDevice(outerBorders[QCss::TopEdge] / 2)).round();
        td->effectiveBottomBorder = QFixed::fromReal(scaleToDevice(outerBorders[QCss::BottomEdge] / 2)).round();
        td->effectiveLeftBorder = QFixed::fromReal(scaleToDevice(outerBorders[QCss::LeftEdge] / 2)).round();
        td->effectiveRightBorder = QFixed::fromReal(scaleToDevice(outerBorders[QCss::RightEdge] / 2)).round();
    }
#endif

    td->deviceScale = scaleToDevice(qreal(1));
    td->cellPadding = QFixed::fromReal(scaleToDevice(fmt.cellPadding()));
    const QFixed leftMargin = td->leftMargin + td->padding + td->effectiveLeftBorder;
    const QFixed rightMargin = td->rightMargin + td->padding + td->effectiveRightBorder;
    const QFixed topMargin = td->topMargin + td->padding + td->effectiveTopBorder;

    const QFixed absoluteTableY = parentY + td->position.y;

    const BOBUIextOption::WrapMode oldDefaultWrapMode = docPrivate->defaultTextOption.wrapMode();

recalc_minmax_widths:

    QFixed remainingWidth = td->contentsWidth;
    // two (vertical) borders per cell per column
    remainingWidth -= columns * 2 * td->borderCell;
    // inter-cell spacing
    remainingWidth -= (columns - 1) * cellSpacing;
    // cell spacing at the left and right hand side
    remainingWidth -= 2 * cellSpacing;

    if (td->borderCollapse) {
        remainingWidth -= td->effectiveLeftBorder;
        remainingWidth -= td->effectiveRightBorder;
    }

    // remember the width used to distribute to percentaged columns
    const QFixed initialTotalWidth = remainingWidth;

    td->widths.resize(columns);
    td->widths.fill(0);

    td->minWidths.resize(columns);
    // start with a minimum width of 0. totally empty
    // cells of default created tables are invisible otherwise
    // and therefore hardly editable
    td->minWidths.fill(1);

    td->maxWidths.resize(columns);
    td->maxWidths.fill(QFIXED_MAX);

    // calculate minimum and maximum sizes of the columns
    for (int i = 0; i < columns; ++i) {
        for (int row = 0; row < rows; ++row) {
            const BOBUIextTableCell cell = table->cellAt(row, i);
            const int cspan = cell.columnSpan();

            if (cspan > 1 && i != cell.column())
                continue;

            const QFixed leftPadding = td->leftPadding(table, cell);
            const QFixed rightPadding = td->rightPadding(table, cell);
            const QFixed widthPadding = leftPadding + rightPadding;

            // to figure out the min and the max width lay out the cell at
            // maximum width. otherwise the maxwidth calculation sometimes
            // returns wrong values
            BOBUIextLayoutStruct layoutStruct = layoutCell(table, cell, QFIXED_MAX, layoutFrom,
                                                        layoutTo, td, absoluteTableY,
                                                        /*withPageBreaks =*/false);

            // distribute the minimum width over all columns the cell spans
            QFixed widthToDistribute = layoutStruct.minimumWidth + widthPadding;
            for (int n = 0; n < cspan; ++n) {
                const int col = i + n;
                QFixed w = widthToDistribute / (cspan - n);
                // ceil to avoid going below minWidth when rounding all column widths later
                td->minWidths[col] = qMax(td->minWidths.at(col), w).ceil();
                widthToDistribute -= td->minWidths.at(col);
                if (widthToDistribute <= 0)
                    break;
            }

            QFixed maxW = td->maxWidths.at(i);
            if (layoutStruct.maximumWidth != QFIXED_MAX) {
                if (maxW == QFIXED_MAX)
                    maxW = layoutStruct.maximumWidth + widthPadding;
                else
                    maxW = qMax(maxW, layoutStruct.maximumWidth + widthPadding);
            }
            if (maxW == QFIXED_MAX)
                continue;

            // for variable columns the maxWidth will later be considered as the
            // column width (column width = content width). We must avoid that the
            // pixel-alignment rounding step floors this value and thus the text
            // rendering later erroneously wraps the content.
            maxW = maxW.ceil();

            widthToDistribute = maxW;
            for (int n = 0; n < cspan; ++n) {
                const int col = i + n;
                QFixed w = widthToDistribute / (cspan - n);
                if (td->maxWidths[col] != QFIXED_MAX)
                    w = qMax(td->maxWidths[col], w);
                td->maxWidths[col] = qMax(td->minWidths.at(col), w);
                widthToDistribute -= td->maxWidths.at(col);
                if (widthToDistribute <= 0)
                    break;
            }
        }
    }

    // set fixed values, figure out total percentages used and number of
    // variable length cells. Also assign the minimum width for variable columns.
    QFixed totalPercentage;
    int variableCols = 0;
    QFixed totalMinWidth = 0;
    for (int i = 0; i < columns; ++i) {
        const BOBUIextLength &length = columnWidthConstraints.at(i);
        if (length.type() == BOBUIextLength::FixedLength) {
            td->minWidths[i] = td->widths[i] = qMax(scaleToDevice(QFixed::fromReal(length.rawValue())), td->minWidths.at(i));
            remainingWidth -= td->widths.at(i);
            qCDebug(lcTable) << "column" << i << "has width constraint" << td->minWidths.at(i) << "px, remaining width now" << remainingWidth;
        } else if (length.type() == BOBUIextLength::PercentageLength) {
            totalPercentage += QFixed::fromReal(length.rawValue());
        } else if (length.type() == BOBUIextLength::VariableLength) {
            variableCols++;

            td->widths[i] = td->minWidths.at(i);
            remainingWidth -= td->minWidths.at(i);
            qCDebug(lcTable) << "column" << i << "has variable width, min" << td->minWidths.at(i) << "remaining width now" << remainingWidth;
        }
        totalMinWidth += td->minWidths.at(i);
    }

    // set percentage values
    {
        const QFixed totalPercentagedWidth = initialTotalWidth * totalPercentage / 100;
        QFixed remainingMinWidths = totalMinWidth;
        for (int i = 0; i < columns; ++i) {
            remainingMinWidths -= td->minWidths.at(i);
            if (columnWidthConstraints.at(i).type() == BOBUIextLength::PercentageLength) {
                const QFixed allottedPercentage = QFixed::fromReal(columnWidthConstraints.at(i).rawValue());

                const QFixed percentWidth = totalPercentagedWidth * allottedPercentage / totalPercentage;
                QFixed maxWidth = remainingWidth - remainingMinWidths;
                if (percentWidth >= td->minWidths.at(i) && maxWidth > td->minWidths.at(i)) {
                    td->widths[i] = qBound(td->minWidths.at(i), percentWidth, maxWidth);
                } else {
                    td->widths[i] = td->minWidths.at(i);
                }
                qCDebug(lcTable) << "column" << i << "has width constraint" << columnWidthConstraints.at(i).rawValue()
                                 << "%, allocated width" << td->widths[i] << "remaining width now" << remainingWidth;
                remainingWidth -= td->widths.at(i);
            }
        }
    }

    // for variable columns distribute the remaining space
    if (variableCols > 0 && remainingWidth > 0) {
        QVarLengthArray<int> columnsWithProperMaxSize;
        for (int i = 0; i < columns; ++i)
            if (columnWidthConstraints.at(i).type() == BOBUIextLength::VariableLength
                && td->maxWidths.at(i) != QFIXED_MAX)
                columnsWithProperMaxSize.append(i);

        QFixed lastRemainingWidth = remainingWidth;
        while (remainingWidth > 0) {
            for (int k = 0; k < columnsWithProperMaxSize.size(); ++k) {
                const int col = columnsWithProperMaxSize[k];
                const int colsLeft = columnsWithProperMaxSize.size() - k;
                const QFixed w = qMin(td->maxWidths.at(col) - td->widths.at(col), remainingWidth / colsLeft);
                td->widths[col] += w;
                remainingWidth -= w;
            }
            if (remainingWidth == lastRemainingWidth)
                break;
            lastRemainingWidth = remainingWidth;
        }

        if (remainingWidth > 0
            // don't unnecessarily grow variable length sized tables
            && fmt.width().type() != BOBUIextLength::VariableLength) {
            const QFixed widthPerAnySizedCol = remainingWidth / variableCols;
            for (int col = 0; col < columns; ++col) {
                if (columnWidthConstraints.at(col).type() == BOBUIextLength::VariableLength)
                    td->widths[col] += widthPerAnySizedCol;
            }
        }
    }

    // in order to get a correct border rendering we must ensure that the distance between
    // two cells is exactly 2 * td->cellBorder pixel. we do this by rounding the calculated width
    // values here.
    // to minimize the total rounding error we propagate the rounding error for each width
    // to its successor.
    QFixed error = 0;
    for (int i = 0; i < columns; ++i) {
        QFixed orig = td->widths[i];
        td->widths[i] = (td->widths[i] - error).round();
        error = td->widths[i] - orig;
    }

    td->columnPositions.resize(columns);
    td->columnPositions[0] = leftMargin /*includes table border*/ + cellSpacing + td->border;

    for (int i = 1; i < columns; ++i)
        td->columnPositions[i] = td->columnPositions.at(i-1) + td->widths.at(i-1) + 2 * td->borderCell + cellSpacing;

    // - margin to compensate the + margin in columnPositions[0]
    const QFixed contentsWidth = td->columnPositions.constLast() + td->widths.constLast() + td->padding + td->border + cellSpacing - leftMargin;

    // if the table is too big and causes an overflow re-do the layout with WrapAnywhere as wrap
    // mode
    if (docPrivate->defaultTextOption.wrapMode() == BOBUIextOption::WrapAtWordBoundaryOrAnywhere
        && contentsWidth > td->contentsWidth) {
        docPrivate->defaultTextOption.setWrapMode(BOBUIextOption::WrapAnywhere);
        // go back to the top of the function
        goto recalc_minmax_widths;
    }

    td->contentsWidth = contentsWidth;

    docPrivate->defaultTextOption.setWrapMode(oldDefaultWrapMode);

    td->heights.resize(rows);
    td->heights.fill(0);

    td->rowPositions.resize(rows);
    td->rowPositions[0] = topMargin /*includes table border*/ + cellSpacing + td->border;

    bool haveRowSpannedCells = false;

    // need to keep track of cell heights for vertical alignment
    QList<QFixed> cellHeights;
    cellHeights.reserve(rows * columns);

    QFixed pageHeight = QFixed::fromReal(document->pageSize().height());
    if (pageHeight <= 0)
        pageHeight = QFIXED_MAX;

    QList<QFixed> heightToDistribute;
    heightToDistribute.resize(columns);

    td->headerHeight = 0;
    const int headerRowCount = qMin(table->format().headerRowCount(), rows - 1);
    const QFixed originalTopMargin = td->effectiveTopMargin;
    bool hasDroppedTable = false;

    // now that we have the column widths we can lay out all cells with the right width.
    // spanning cells are only allowed to grow the last row spanned by the cell.
    //
    // ### this could be made faster by iterating over the cells array of BOBUIextTable
    for (int r = 0; r < rows; ++r) {
        td->calcRowPosition(r);

        const int tableStartPage = (absoluteTableY / pageHeight).truncate();
        const int currentPage = ((td->rowPositions.at(r) + absoluteTableY) / pageHeight).truncate();
        const QFixed pageBottom = (currentPage + 1) * pageHeight - td->effectiveBottomMargin - absoluteTableY - cellSpacing - td->border;
        const QFixed pageTop = currentPage * pageHeight + td->effectiveTopMargin - absoluteTableY + cellSpacing + td->border;
        const QFixed nextPageTop = pageTop + pageHeight;

        if (td->rowPositions.at(r) > pageBottom)
            td->rowPositions[r] = nextPageTop;
        else if (td->rowPositions.at(r) < pageTop)
            td->rowPositions[r] = pageTop;

        bool dropRowToNextPage = true;
        int cellCountBeforeRow = cellHeights.size();

        // if we drop the row to the next page we need to subtract the drop
        // distance from any row spanning cells
        QFixed dropDistance = 0;

relayout:
        const int rowStartPage = ((td->rowPositions.at(r) + absoluteTableY) / pageHeight).truncate();
        // if any of the header rows or the first non-header row start on the next page
        // then the entire header should be dropped
        if (r <= headerRowCount && rowStartPage > tableStartPage && !hasDroppedTable) {
            td->rowPositions[0] = nextPageTop;
            cellHeights.clear();
            td->effectiveTopMargin = originalTopMargin;
            hasDroppedTable = true;
            r = -1;
            continue;
        }

        int rowCellCount = 0;
        for (int c = 0; c < columns; ++c) {
            BOBUIextTableCell cell = table->cellAt(r, c);
            const int rspan = cell.rowSpan();
            const int cspan = cell.columnSpan();

            if (cspan > 1 && cell.column() != c)
                continue;

            if (rspan > 1) {
                haveRowSpannedCells = true;

                const int cellRow = cell.row();
                if (cellRow != r) {
                    // the last row gets all the remaining space
                    if (cellRow + rspan - 1 == r)
                        td->heights[r] = qMax(td->heights.at(r), heightToDistribute.at(c) - dropDistance).round();
                    continue;
                }
            }

            const QFixed topPadding = td->topPadding(table, cell);
            const QFixed bottomPadding = td->bottomPadding(table, cell);
            const QFixed leftPadding = td->leftPadding(table, cell);
            const QFixed rightPadding = td->rightPadding(table, cell);
            const QFixed widthPadding = leftPadding + rightPadding;

            ++rowCellCount;

            const QFixed width = td->cellWidth(c, cspan) - widthPadding;
            BOBUIextLayoutStruct layoutStruct = layoutCell(table, cell, width,
                                                       layoutFrom, layoutTo,
                                                       td, absoluteTableY,
                                                       /*withPageBreaks =*/true);

            const QFixed height = (layoutStruct.y + bottomPadding + topPadding).round();

            if (rspan > 1)
                heightToDistribute[c] = height + dropDistance;
            else
                td->heights[r] = qMax(td->heights.at(r), height);

            cellHeights.append(layoutStruct.y);

            QFixed childPos = td->rowPositions.at(r) + topPadding + flowPosition(cell.begin());
            if (childPos < pageBottom)
                dropRowToNextPage = false;
        }

        if (rowCellCount > 0 && dropRowToNextPage) {
            dropDistance = nextPageTop - td->rowPositions.at(r);
            td->rowPositions[r] = nextPageTop;
            td->heights[r] = 0;
            dropRowToNextPage = false;
            cellHeights.resize(cellCountBeforeRow);
            if (r > headerRowCount)
                td->heights[r - 1] = pageBottom - td->rowPositions.at(r - 1);
            goto relayout;
        }

        if (haveRowSpannedCells) {
            const QFixed effectiveHeight = td->heights.at(r) + td->borderCell + cellSpacing + td->borderCell;
            for (int c = 0; c < columns; ++c)
                heightToDistribute[c] = qMax(heightToDistribute.at(c) - effectiveHeight - dropDistance, QFixed(0));
        }

        if (r == headerRowCount - 1) {
            td->headerHeight = td->rowPositions.at(r) + td->heights.at(r) - td->rowPositions.at(0) + td->cellSpacing + 2 * td->borderCell;
            td->headerHeight -= td->headerHeight * (td->headerHeight / pageHeight).truncate();
            td->effectiveTopMargin += td->headerHeight;
        }
    }

    td->effectiveTopMargin = originalTopMargin;

    // now that all cells have been properly laid out, we can compute the
    // vertical offsets for vertical alignment
    td->cellVerticalOffsets.resize(rows * columns);
    int cellIndex = 0;
    for (int r = 0; r < rows; ++r) {
        for (int c = 0; c < columns; ++c) {
            BOBUIextTableCell cell = table->cellAt(r, c);
            if (cell.row() != r || cell.column() != c)
                continue;

            const int rowSpan = cell.rowSpan();
            const QFixed availableHeight = td->rowPositions.at(r + rowSpan - 1) + td->heights.at(r + rowSpan - 1) - td->rowPositions.at(r);

            const BOBUIextCharFormat cellFormat = cell.format();
            const QFixed cellHeight = cellHeights.at(cellIndex++) + td->topPadding(table, cell) + td->bottomPadding(table, cell);

            QFixed offset = 0;
            switch (cellFormat.verticalAlignment()) {
            case BOBUIextCharFormat::AlignMiddle:
                offset = (availableHeight - cellHeight) / 2;
                break;
            case BOBUIextCharFormat::AlignBottom:
                offset = availableHeight - cellHeight;
                break;
            default:
                break;
            };

            for (int rd = 0; rd < cell.rowSpan(); ++rd) {
                for (int cd = 0; cd < cell.columnSpan(); ++cd) {
                    const int index = (c + cd) + (r + rd) * columns;
                    td->cellVerticalOffsets[index] = offset;
                }
            }
        }
    }

    td->minimumWidth = td->columnPositions.at(0);
    for (int i = 0; i < columns; ++i) {
        td->minimumWidth += td->minWidths.at(i) + 2 * td->borderCell + cellSpacing;
    }
    td->minimumWidth += rightMargin - td->border;

    td->maximumWidth = td->columnPositions.at(0);
    for (int i = 0; i < columns; ++i) {
        if (td->maxWidths.at(i) != QFIXED_MAX)
            td->maximumWidth += td->maxWidths.at(i) + 2 * td->borderCell + cellSpacing;
        qCDebug(lcTable) << "column" << i << "has final width" << td->widths.at(i).toReal()
                         << "min" << td->minWidths.at(i).toReal() << "max" << td->maxWidths.at(i).toReal();
    }
    td->maximumWidth += rightMargin - td->border;

    td->updateTableSize();
    td->sizeDirty = false;
    return QRectF(); // invalid rect -> update everything
}

void BOBUIextDocumentLayoutPrivate::positionFloat(BOBUIextFrame *frame, BOBUIextLine *currentLine)
{
    BOBUIextFrameData *fd = data(frame);

    BOBUIextFrame *parent = frame->parentFrame();
    Q_ASSERT(parent);
    BOBUIextFrameData *pd = data(parent);
    Q_ASSERT(pd && pd->currentLayoutStruct);

    BOBUIextLayoutStruct *layoutStruct = pd->currentLayoutStruct;

    if (!pd->floats.contains(frame))
        pd->floats.append(frame);
    fd->layoutDirty = true;
    Q_ASSERT(!fd->sizeDirty);

//     qDebug() << "positionFloat:" << frame << "width=" << fd->size.width;
    QFixed y = layoutStruct->y;
    if (currentLine) {
        QFixed left, right;
        floatMargins(y, layoutStruct, &left, &right);
//         qDebug() << "have line: right=" << right << "left=" << left << "textWidth=" << currentLine->width();
        if (right - left < QFixed::fromReal(currentLine->naturalTextWidth()) + fd->size.width) {
            layoutStruct->pendingFloats.append(frame);
//             qDebug("    adding to pending list");
            return;
        }
    }

    bool frameSpansIntoNextPage = (y + layoutStruct->frameY + fd->size.height > layoutStruct->pageBottom);
    if (frameSpansIntoNextPage && fd->size.height <= layoutStruct->pageHeight) {
        layoutStruct->newPage();
        y = layoutStruct->y;

        frameSpansIntoNextPage = false;
    }

    y = findY(y, layoutStruct, fd->size.width);

    QFixed left, right;
    floatMargins(y, layoutStruct, &left, &right);

    if (frame->frameFormat().position() == BOBUIextFrameFormat::FloatLeft) {
        fd->position.x = left;
        fd->position.y = y;
    } else {
        fd->position.x = right - fd->size.width;
        fd->position.y = y;
    }

    layoutStruct->minimumWidth = qMax(layoutStruct->minimumWidth, fd->minimumWidth);
    layoutStruct->maximumWidth = qMin(layoutStruct->maximumWidth, fd->maximumWidth);

//     qDebug()<< "float positioned at " << fd->position.x << fd->position.y;
    fd->layoutDirty = false;

    // If the frame is a table, then positioning it will affect the size if it covers more than
    // one page, because of page breaks and repeating the header.
    if (qobject_cast<BOBUIextTable *>(frame) != nullptr)
        fd->sizeDirty = frameSpansIntoNextPage;
}

QRectF BOBUIextDocumentLayoutPrivate::layoutFrame(BOBUIextFrame *f, int layoutFrom, int layoutTo, QFixed parentY)
{
    qCDebug(lcLayout, "layoutFrame (%d--%d), parent=%p", f->firstPosition(), f->lastPosition(), f->parentFrame());
    Q_ASSERT(data(f)->sizeDirty);

    BOBUIextFrameFormat fformat = f->frameFormat();

    BOBUIextFrame *parent = f->parentFrame();
    const BOBUIextFrameData *pd = parent ? data(parent) : nullptr;

    const qreal maximumWidth = qMax(qreal(0), pd ? pd->contentsWidth.toReal() : document->pageSize().width());
    QFixed width = QFixed::fromReal(fformat.width().value(maximumWidth));
    if (fformat.width().type() == BOBUIextLength::FixedLength)
        width = scaleToDevice(width);

    const QFixed maximumHeight = pd ? pd->contentsHeight : -1;
    const QFixed height = (maximumHeight != -1 || fformat.height().type() != BOBUIextLength::PercentageLength)
                            ? QFixed::fromReal(fformat.height().value(maximumHeight.toReal()))
                            : -1;

    return layoutFrame(f, layoutFrom, layoutTo, width, height, parentY);
}

QRectF BOBUIextDocumentLayoutPrivate::layoutFrame(BOBUIextFrame *f, int layoutFrom, int layoutTo, QFixed frameWidth, QFixed frameHeight, QFixed parentY)
{
    qCDebug(lcLayout, "layoutFrame (%d--%d), parent=%p", f->firstPosition(), f->lastPosition(), f->parentFrame());
    Q_ASSERT(data(f)->sizeDirty);

    BOBUIextFrameData *fd = data(f);
    QFixed newContentsWidth;

    bool fullLayout = false;
    {
        BOBUIextFrameFormat fformat = f->frameFormat();
        // set sizes of this frame from the format
        QFixed tm = QFixed::fromReal(scaleToDevice(fformat.topMargin())).round();
        if (tm != fd->topMargin) {
            fd->topMargin = tm;
            fullLayout = true;
        }
        QFixed bm = QFixed::fromReal(scaleToDevice(fformat.bottomMargin())).round();
        if (bm != fd->bottomMargin) {
            fd->bottomMargin = bm;
            fullLayout = true;
        }
        fd->leftMargin = QFixed::fromReal(scaleToDevice(fformat.leftMargin())).round();
        fd->rightMargin = QFixed::fromReal(scaleToDevice(fformat.rightMargin())).round();
        QFixed b = QFixed::fromReal(scaleToDevice(fformat.border())).round();
        if (b != fd->border) {
            fd->border = b;
            fullLayout = true;
        }
        QFixed p = QFixed::fromReal(scaleToDevice(fformat.padding())).round();
        if (p != fd->padding) {
            fd->padding = p;
            fullLayout = true;
        }

        BOBUIextFrame *parent = f->parentFrame();
        const BOBUIextFrameData *pd = parent ? data(parent) : nullptr;

        // accumulate top and bottom margins
        if (parent) {
            fd->effectiveTopMargin = pd->effectiveTopMargin + fd->topMargin + fd->border + fd->padding;
            fd->effectiveBottomMargin = pd->effectiveBottomMargin + fd->topMargin + fd->border + fd->padding;

            if (qobject_cast<BOBUIextTable *>(parent)) {
                const BOBUIextTableData *td = static_cast<const BOBUIextTableData *>(pd);
                fd->effectiveTopMargin += td->cellSpacing + td->border + td->cellPadding;
                fd->effectiveBottomMargin += td->cellSpacing + td->border + td->cellPadding;
            }
        } else {
            fd->effectiveTopMargin = fd->topMargin + fd->border + fd->padding;
            fd->effectiveBottomMargin = fd->bottomMargin + fd->border + fd->padding;
        }

        newContentsWidth = frameWidth - 2*(fd->border + fd->padding)
                           - fd->leftMargin - fd->rightMargin;

        if (frameHeight != -1) {
            fd->contentsHeight = frameHeight - 2*(fd->border + fd->padding)
                                 - fd->topMargin - fd->bottomMargin;
        } else {
            fd->contentsHeight = frameHeight;
        }
    }

    if (isFrameFromInlineObject(f)) {
        // never reached, handled in resizeInlineObject/positionFloat instead
        return QRectF();
    }

    if (BOBUIextTable *table = qobject_cast<BOBUIextTable *>(f)) {
        fd->contentsWidth = newContentsWidth;
        return layoutTable(table, layoutFrom, layoutTo, parentY);
    }

    // set fd->contentsWidth temporarily, so that layoutFrame for the children
    // picks the right width. We'll initialize it properly at the end of this
    // function.
    fd->contentsWidth = newContentsWidth;

    BOBUIextLayoutStruct layoutStruct;
    layoutStruct.frame = f;
    layoutStruct.x_left = fd->leftMargin + fd->border + fd->padding;
    layoutStruct.x_right = layoutStruct.x_left + newContentsWidth;
    layoutStruct.y = fd->topMargin + fd->border + fd->padding;
    layoutStruct.frameY = parentY + fd->position.y;
    layoutStruct.contentsWidth = 0;
    layoutStruct.minimumWidth = 0;
    layoutStruct.maximumWidth = QFIXED_MAX;
    layoutStruct.fullLayout = fullLayout || (fd->oldContentsWidth != newContentsWidth);
    layoutStruct.updateRect = QRectF(QPointF(0, 0), QSizeF(qreal(INT_MAX), qreal(INT_MAX)));
    qCDebug(lcLayout) << "layoutStruct: x_left" << layoutStruct.x_left << "x_right" << layoutStruct.x_right
                      << "fullLayout" << layoutStruct.fullLayout;
    fd->oldContentsWidth = newContentsWidth;

    layoutStruct.pageHeight = QFixed::fromReal(document->pageSize().height());
    if (layoutStruct.pageHeight < 0)
        layoutStruct.pageHeight = QFIXED_MAX;

    const int currentPage = layoutStruct.pageHeight == 0 ? 0 : (layoutStruct.frameY / layoutStruct.pageHeight).truncate();
    layoutStruct.pageTopMargin = fd->effectiveTopMargin;
    layoutStruct.pageBottomMargin = fd->effectiveBottomMargin;
    layoutStruct.pageBottom = (currentPage + 1) * layoutStruct.pageHeight - layoutStruct.pageBottomMargin;

    if (!f->parentFrame())
        idealWidth = 0; // reset

    BOBUIextFrame::Iterator it = f->begin();
    layoutFlow(it, &layoutStruct, layoutFrom, layoutTo);

    QFixed maxChildFrameWidth = 0;
    QList<BOBUIextFrame *> children = f->childFrames();
    for (int i = 0; i < children.size(); ++i) {
        BOBUIextFrame *c = children.at(i);
        BOBUIextFrameData *cd = data(c);
        maxChildFrameWidth = qMax(maxChildFrameWidth, cd->size.width);
    }

    const QFixed marginWidth = 2*(fd->border + fd->padding) + fd->leftMargin + fd->rightMargin;
    if (!f->parentFrame()) {
        idealWidth = qMax(maxChildFrameWidth, layoutStruct.contentsWidth).toReal();
        idealWidth += marginWidth.toReal();
    }

    QFixed actualWidth = qMax(newContentsWidth, qMax(maxChildFrameWidth, layoutStruct.contentsWidth));
    fd->contentsWidth = actualWidth;
    if (newContentsWidth <= 0) { // nowrap layout?
        fd->contentsWidth = newContentsWidth;
    }

    fd->minimumWidth = layoutStruct.minimumWidth;
    fd->maximumWidth = layoutStruct.maximumWidth;

    fd->size.height = fd->contentsHeight == -1
                 ? layoutStruct.y + fd->border + fd->padding + fd->bottomMargin
                 : fd->contentsHeight + 2*(fd->border + fd->padding) + fd->topMargin + fd->bottomMargin;
    fd->size.width = actualWidth + marginWidth;
    fd->sizeDirty = false;
    if (layoutStruct.updateRectForFloats.isValid())
        layoutStruct.updateRect |= layoutStruct.updateRectForFloats;
    return layoutStruct.updateRect;
}

void BOBUIextDocumentLayoutPrivate::layoutFlow(BOBUIextFrame::Iterator it, BOBUIextLayoutStruct *layoutStruct,
                                            int layoutFrom, int layoutTo, QFixed width)
{
    qCDebug(lcLayout) << "layoutFlow from=" << layoutFrom << "to=" << layoutTo;
    BOBUIextFrameData *fd = data(layoutStruct->frame);

    fd->currentLayoutStruct = layoutStruct;

    BOBUIextFrame::Iterator previousIt;

    const bool inRootFrame = (it.parentFrame() == document->rootFrame());
    if (inRootFrame) {
        bool redoCheckPoints = layoutStruct->fullLayout || checkPoints.isEmpty();

        if (!redoCheckPoints) {
            auto checkPoint = std::lower_bound(checkPoints.begin(), checkPoints.end(), layoutFrom);
            if (checkPoint != checkPoints.end()) {
                if (checkPoint != checkPoints.begin())
                    --checkPoint;

                layoutStruct->y = checkPoint->y;
                layoutStruct->frameY = checkPoint->frameY;
                layoutStruct->minimumWidth = checkPoint->minimumWidth;
                layoutStruct->maximumWidth = checkPoint->maximumWidth;
                layoutStruct->contentsWidth = checkPoint->contentsWidth;

                if (layoutStruct->pageHeight > 0) {
                    int page = layoutStruct->currentPage();
                    layoutStruct->pageBottom = (page + 1) * layoutStruct->pageHeight - layoutStruct->pageBottomMargin;
                }

                it = frameIteratorForTextPosition(checkPoint->positionInFrame);
                checkPoints.resize(checkPoint - checkPoints.begin() + 1);

                if (checkPoint != checkPoints.begin()) {
                    previousIt = it;
                    --previousIt;
                }
            } else {
                redoCheckPoints = true;
            }
        }

        if (redoCheckPoints) {
            checkPoints.clear();
            QCheckPoint cp;
            cp.y = layoutStruct->y;
            cp.frameY = layoutStruct->frameY;
            cp.positionInFrame = 0;
            cp.minimumWidth = layoutStruct->minimumWidth;
            cp.maximumWidth = layoutStruct->maximumWidth;
            cp.contentsWidth = layoutStruct->contentsWidth;
            checkPoints.append(cp);
        }
    }

    BOBUIextBlockFormat previousBlockFormat = previousIt.currentBlock().blockFormat();

    QFixed maximumBlockWidth = 0;
    while (!it.atEnd() && layoutStruct->absoluteY() < QFIXED_MAX) {
        BOBUIextFrame *c = it.currentFrame();

        int docPos;
        if (it.currentFrame())
            docPos = it.currentFrame()->firstPosition();
        else
            docPos = it.currentBlock().position();

        if (inRootFrame) {
            if (qAbs(layoutStruct->y - checkPoints.constLast().y) > 2000) {
                QFixed left, right;
                floatMargins(layoutStruct->y, layoutStruct, &left, &right);
                if (left == layoutStruct->x_left && right == layoutStruct->x_right) {
                    QCheckPoint p;
                    p.y = layoutStruct->y;
                    p.frameY = layoutStruct->frameY;
                    p.positionInFrame = docPos;
                    p.minimumWidth = layoutStruct->minimumWidth;
                    p.maximumWidth = layoutStruct->maximumWidth;
                    p.contentsWidth = layoutStruct->contentsWidth;
                    checkPoints.append(p);

                    if (currentLazyLayoutPosition != -1
                        && docPos > currentLazyLayoutPosition + lazyLayoutStepSize)
                        break;

                }
            }
        }

        if (c) {
            // position child frame
            BOBUIextFrameData *cd = data(c);

            BOBUIextFrameFormat fformat = c->frameFormat();

            if (fformat.position() == BOBUIextFrameFormat::InFlow) {
                if (fformat.pageBreakPolicy() & BOBUIextFormat::PageBreak_AlwaysBefore)
                    layoutStruct->newPage();

                QFixed left, right;
                floatMargins(layoutStruct->y, layoutStruct, &left, &right);
                left = qMax(left, layoutStruct->x_left);
                right = qMin(right, layoutStruct->x_right);

                if (right - left < cd->size.width) {
                    layoutStruct->y = findY(layoutStruct->y, layoutStruct, cd->size.width);
                    floatMargins(layoutStruct->y, layoutStruct, &left, &right);
                }

                QFixedPoint pos(left, layoutStruct->y);

                BobUI::Alignment align = BobUI::AlignLeft;

                BOBUIextTable *table = qobject_cast<BOBUIextTable *>(c);

                if (table)
                    align = table->format().alignment() & BobUI::AlignHorizontal_Mask;

                // detect whether we have any alignment in the document that disallows optimizations,
                // such as not laying out the document again in a textedit with wrapping disabled.
                if (inRootFrame && !(align & BobUI::AlignLeft))
                    contentHasAlignment = true;

                cd->position = pos;

                if (document->pageSize().height() > 0.0f)
                    cd->sizeDirty = true;

                if (cd->sizeDirty) {
                    if (width != 0)
                        layoutFrame(c, layoutFrom, layoutTo, width, -1, layoutStruct->frameY);
                    else
                        layoutFrame(c, layoutFrom, layoutTo, layoutStruct->frameY);

                    QFixed absoluteChildPos = table ? pos.y + static_cast<BOBUIextTableData *>(data(table))->rowPositions.at(0) : pos.y + firstChildPos(c);
                    absoluteChildPos += layoutStruct->frameY;

                    // drop entire frame to next page if first child of frame is on next page
                    if (absoluteChildPos > layoutStruct->pageBottom) {
                        layoutStruct->newPage();
                        pos.y = layoutStruct->y;

                        cd->position = pos;
                        cd->sizeDirty = true;

                        if (width != 0)
                            layoutFrame(c, layoutFrom, layoutTo, width, -1, layoutStruct->frameY);
                        else
                            layoutFrame(c, layoutFrom, layoutTo, layoutStruct->frameY);
                    }
                }

                // align only if there is space for alignment
                if (right - left > cd->size.width) {
                    if (align & BobUI::AlignRight)
                        pos.x += layoutStruct->x_right - cd->size.width;
                    else if (align & BobUI::AlignHCenter)
                        pos.x += (layoutStruct->x_right - cd->size.width) / 2;
                }

                cd->position = pos;

                layoutStruct->y += cd->size.height;
                const int page = layoutStruct->currentPage();
                layoutStruct->pageBottom = (page + 1) * layoutStruct->pageHeight - layoutStruct->pageBottomMargin;

                cd->layoutDirty = false;

                if (c->frameFormat().pageBreakPolicy() & BOBUIextFormat::PageBreak_AlwaysAfter)
                    layoutStruct->newPage();
            } else {
                QRectF oldFrameRect(cd->position.toPointF(), cd->size.toSizeF());
                QRectF updateRect;

                if (cd->sizeDirty)
                    updateRect = layoutFrame(c, layoutFrom, layoutTo);

                positionFloat(c);

                // If the size was made dirty when the position was set, layout again
                if (cd->sizeDirty)
                    updateRect = layoutFrame(c, layoutFrom, layoutTo);

                QRectF frameRect(cd->position.toPointF(), cd->size.toSizeF());

                if (frameRect == oldFrameRect && updateRect.isValid())
                    updateRect.translate(cd->position.toPointF());
                else
                    updateRect = frameRect;

                layoutStruct->addUpdateRectForFloat(updateRect);
                if (oldFrameRect.isValid())
                    layoutStruct->addUpdateRectForFloat(oldFrameRect);
            }

            layoutStruct->minimumWidth = qMax(layoutStruct->minimumWidth, cd->minimumWidth);
            layoutStruct->maximumWidth = qMin(layoutStruct->maximumWidth, cd->maximumWidth);

            previousIt = it;
            ++it;
        } else {
            BOBUIextFrame::Iterator lastIt;
            if (!previousIt.atEnd() && previousIt != it)
                lastIt = previousIt;
            previousIt = it;
            BOBUIextBlock block = it.currentBlock();
            ++it;

            const BOBUIextBlockFormat blockFormat = block.blockFormat();

            if (blockFormat.pageBreakPolicy() & BOBUIextFormat::PageBreak_AlwaysBefore)
                layoutStruct->newPage();

            const QFixed origY = layoutStruct->y;
            const QFixed origPageBottom = layoutStruct->pageBottom;
            const QFixed origMaximumWidth = layoutStruct->maximumWidth;
            layoutStruct->maximumWidth = 0;

            const BOBUIextBlockFormat *previousBlockFormatPtr = nullptr;
            if (lastIt.currentBlock().isValid())
                previousBlockFormatPtr = &previousBlockFormat;

            // layout and position child block
            layoutBlock(block, docPos, blockFormat, layoutStruct, layoutFrom, layoutTo, previousBlockFormatPtr);

            // detect whether we have any alignment in the document that disallows optimizations,
            // such as not laying out the document again in a textedit with wrapping disabled.
            if (inRootFrame && !(block.layout()->textOption().alignment() & BobUI::AlignLeft))
                contentHasAlignment = true;

            // if the block right before a table is empty 'hide' it by
            // positioning it into the table border
            if (isEmptyBlockBeforeTable(block, blockFormat, it)) {
                const BOBUIextBlock lastBlock = lastIt.currentBlock();
                const qreal lastBlockBottomMargin = lastBlock.isValid() ? lastBlock.blockFormat().bottomMargin() : 0.0f;
                layoutStruct->y = origY + QFixed::fromReal(qMax(lastBlockBottomMargin, block.blockFormat().topMargin()));
                layoutStruct->pageBottom = origPageBottom;
            } else {
                // if the block right after a table is empty then 'hide' it, too
                if (isEmptyBlockAfterTable(block, lastIt.currentFrame())) {
                    BOBUIextTableData *td = static_cast<BOBUIextTableData *>(data(lastIt.currentFrame()));
                    BOBUIextLayout *layout = block.layout();

                    QPointF pos((td->position.x + td->size.width).toReal(),
                                (td->position.y + td->size.height).toReal() - layout->boundingRect().height());

                    layout->setPosition(pos);
                    layoutStruct->y = origY;
                    layoutStruct->pageBottom = origPageBottom;
                }

                // if the block right after a table starts with a line separator, shift it up by one line
                if (isLineSeparatorBlockAfterTable(block, lastIt.currentFrame())) {
                    BOBUIextTableData *td = static_cast<BOBUIextTableData *>(data(lastIt.currentFrame()));
                    BOBUIextLayout *layout = block.layout();

                    QFixed height = layout->lineCount() > 0 ? QFixed::fromReal(layout->lineAt(0).height()) : QFixed();

                    if (layoutStruct->pageBottom == origPageBottom) {
                        layoutStruct->y -= height;
                        layout->setPosition(layout->position() - QPointF(0, height.toReal()));
                    } else {
                        // relayout block to correctly handle page breaks
                        layoutStruct->y = origY - height;
                        layoutStruct->pageBottom = origPageBottom;
                        layoutBlock(block, docPos, blockFormat, layoutStruct, layoutFrom, layoutTo, previousBlockFormatPtr);
                    }

                    if (layout->lineCount() > 0) {
                        QPointF linePos((td->position.x + td->size.width).toReal(),
                                        (td->position.y + td->size.height - height).toReal());

                        layout->lineAt(0).setPosition(linePos - layout->position());
                    }
                }

                if (blockFormat.pageBreakPolicy() & BOBUIextFormat::PageBreak_AlwaysAfter)
                    layoutStruct->newPage();
            }

            maximumBlockWidth = qMax(maximumBlockWidth, layoutStruct->maximumWidth);
            layoutStruct->maximumWidth = origMaximumWidth;
            previousBlockFormat = blockFormat;
        }
    }
    if (layoutStruct->maximumWidth == QFIXED_MAX && maximumBlockWidth > 0)
        layoutStruct->maximumWidth = maximumBlockWidth;
    else
        layoutStruct->maximumWidth = qMax(layoutStruct->maximumWidth, maximumBlockWidth);

    // a float at the bottom of a frame may make it taller, hence the qMax() for layoutStruct->y.
    // we don't need to do it for tables though because floats in tables are per table
    // and not per cell and layoutCell already takes care of doing the same as we do here
    if (!qobject_cast<BOBUIextTable *>(layoutStruct->frame)) {
        QList<BOBUIextFrame *> children = layoutStruct->frame->childFrames();
        for (int i = 0; i < children.size(); ++i) {
            BOBUIextFrameData *fd = data(children.at(i));
            if (!fd->layoutDirty && children.at(i)->frameFormat().position() != BOBUIextFrameFormat::InFlow)
                layoutStruct->y = qMax(layoutStruct->y, fd->position.y + fd->size.height);
        }
    }

    if (inRootFrame) {
        // we assume that any float is aligned in a way that disallows the optimizations that rely
        // on unaligned content.
        if (!fd->floats.isEmpty())
            contentHasAlignment = true;

        if (it.atEnd() || layoutStruct->absoluteY() >= QFIXED_MAX) {
            //qDebug("layout done!");
            currentLazyLayoutPosition = -1;
            QCheckPoint cp;
            cp.y = layoutStruct->y;
            cp.positionInFrame = docPrivate->length();
            cp.minimumWidth = layoutStruct->minimumWidth;
            cp.maximumWidth = layoutStruct->maximumWidth;
            cp.contentsWidth = layoutStruct->contentsWidth;
            checkPoints.append(cp);
            checkPoints.reserve(checkPoints.size());
        } else {
            currentLazyLayoutPosition = checkPoints.constLast().positionInFrame;
            // #######
            //checkPoints.last().positionInFrame = BOBUIextDocumentPrivate::get(q->document())->length();
        }
    }


    fd->currentLayoutStruct = nullptr;
}

static inline void getLineHeightParams(const BOBUIextBlockFormat &blockFormat, const BOBUIextLine &line, qreal scaling,
                                       QFixed *lineAdjustment, QFixed *lineBreakHeight, QFixed *lineHeight, QFixed *lineBottom)
{
    const qreal height = line.height();
    const int lineHeightType = blockFormat.lineHeightType();
    qreal rawHeight = qCeil(line.ascent() + line.descent() + line.leading());
    *lineHeight = QFixed::fromReal(blockFormat.lineHeight(rawHeight, scaling));
    *lineBottom = QFixed::fromReal(blockFormat.lineHeight(height, scaling));

    if (lineHeightType == BOBUIextBlockFormat::FixedHeight || lineHeightType == BOBUIextBlockFormat::MinimumHeight) {
        *lineBreakHeight = *lineBottom;
        if (lineHeightType == BOBUIextBlockFormat::FixedHeight)
            *lineAdjustment = QFixed::fromReal(line.ascent() + qMax(line.leading(), qreal(0.0))) - ((*lineHeight * 4) / 5);
        else
            *lineAdjustment = QFixed::fromReal(height) - *lineHeight;
    }
    else {
        *lineBreakHeight = QFixed::fromReal(height);
        *lineAdjustment = 0;
    }
}

void BOBUIextDocumentLayoutPrivate::layoutBlock(const BOBUIextBlock &bl, int blockPosition, const BOBUIextBlockFormat &blockFormat,
                                             BOBUIextLayoutStruct *layoutStruct, int layoutFrom, int layoutTo, const BOBUIextBlockFormat *previousBlockFormat)
{
    Q_Q(BOBUIextDocumentLayout);
    if (!bl.isVisible())
        return;

    BOBUIextLayout *tl = bl.layout();
    const int blockLength = bl.length();

    qCDebug(lcLayout) << "layoutBlock from=" << layoutFrom << "to=" << layoutTo
                      << "; width" << layoutStruct->x_right - layoutStruct->x_left << "(maxWidth is btw" << tl->maximumWidth() << ')';

    if (previousBlockFormat) {
        qreal margin = qMax(blockFormat.topMargin(), previousBlockFormat->bottomMargin());
        if (margin > 0 && q->paintDevice()) {
            margin *= qreal(q->paintDevice()->logicalDpiY()) / qreal(bobui_defaultDpi());
        }
        layoutStruct->y += QFixed::fromReal(margin);
    }

    //BOBUIextFrameData *fd = data(layoutStruct->frame);

    BobUI::LayoutDirection dir = bl.textDirection();

    QFixed extraMargin;
    if (docPrivate->defaultTextOption.flags() & BOBUIextOption::AddSpaceForLineAndParagraphSeparators) {
        QFontMetricsF fm(bl.charFormat().font());
        extraMargin = QFixed::fromReal(fm.horizontalAdvance(u'\x21B5'));
    }

    const QFixed indent = this->blockIndent(blockFormat);
    const QFixed totalLeftMargin = QFixed::fromReal(blockFormat.leftMargin()) + (dir == BobUI::RightToLeft ? extraMargin : indent);
    const QFixed totalRightMargin = QFixed::fromReal(blockFormat.rightMargin()) + (dir == BobUI::RightToLeft ? indent : extraMargin);

    const QPointF oldPosition = tl->position();
    tl->setPosition(QPointF(layoutStruct->x_left.toReal(), layoutStruct->y.toReal()));

    if (layoutStruct->fullLayout
        || (blockPosition + blockLength > layoutFrom && blockPosition <= layoutTo)
        // force relayout if we cross a page boundary
        || (layoutStruct->pageHeight != QFIXED_MAX && layoutStruct->absoluteY() + QFixed::fromReal(tl->boundingRect().height()) > layoutStruct->pageBottom)) {

        qCDebug(lcLayout) << "do layout";
        BOBUIextOption option = docPrivate->defaultTextOption;
        option.setTextDirection(dir);
        option.setTabs( blockFormat.tabPositions() );

        BobUI::Alignment align = docPrivate->defaultTextOption.alignment();
        if (blockFormat.hasProperty(BOBUIextFormat::BlockAlignment))
            align = blockFormat.alignment();
        option.setAlignment(QGuiApplicationPrivate::visualAlignment(dir, align)); // for paragraph that are RTL, alignment is auto-reversed;

        if (blockFormat.nonBreakableLines() || document->pageSize().width() < 0) {
            option.setWrapMode(BOBUIextOption::ManualWrap);
        }

        tl->setTextOption(option);

        const bool haveWordOrAnyWrapMode = (option.wrapMode() == BOBUIextOption::WrapAtWordBoundaryOrAnywhere);

//         qDebug() << "    layouting block at" << bl.position();
        const QFixed cy = layoutStruct->y;
        const QFixed l = layoutStruct->x_left  + totalLeftMargin;
        const QFixed r = layoutStruct->x_right - totalRightMargin;
        QFixed bottom;

        tl->beginLayout();
        bool firstLine = true;
        while (1) {
            BOBUIextLine line = tl->createLine();
            if (!line.isValid())
                break;
            line.setLeadingIncluded(true);

            QFixed left, right;
            floatMargins(layoutStruct->y, layoutStruct, &left, &right);
            left = qMax(left, l);
            right = qMin(right, r);
            QFixed text_indent;
            if (firstLine) {
                text_indent = QFixed::fromReal(blockFormat.textIndent());
                if (dir == BobUI::LeftToRight)
                    left += text_indent;
                else
                    right -= text_indent;
                firstLine = false;
            }
//         qDebug() << "layout line y=" << currentYPos << "left=" << left << "right=" <<right;

            if (fixedColumnWidth != -1)
                line.setNumColumns(fixedColumnWidth, (right - left).toReal());
            else
                line.setLineWidth((right - left).toReal());

//        qDebug() << "layoutBlock; layouting line with width" << right - left << "->textWidth" << line.textWidth();
            floatMargins(layoutStruct->y, layoutStruct, &left, &right);
            left = qMax(left, l);
            right = qMin(right, r);
            if (dir == BobUI::LeftToRight)
                left += text_indent;
            else
                right -= text_indent;

            if (fixedColumnWidth == -1 && QFixed::fromReal(line.naturalTextWidth()) > right-left) {
                // float has been added in the meantime, redo
                layoutStruct->pendingFloats.clear();

                line.setLineWidth((right-left).toReal());
                if (QFixed::fromReal(line.naturalTextWidth()) > right-left) {
                    if (haveWordOrAnyWrapMode) {
                        option.setWrapMode(BOBUIextOption::WrapAnywhere);
                        tl->setTextOption(option);
                    }

                    layoutStruct->pendingFloats.clear();
                    // lines min width more than what we have
                    layoutStruct->y = findY(layoutStruct->y, layoutStruct, QFixed::fromReal(line.naturalTextWidth()));
                    floatMargins(layoutStruct->y, layoutStruct, &left, &right);
                    left = qMax(left, l);
                    right = qMin(right, r);
                    if (dir == BobUI::LeftToRight)
                        left += text_indent;
                    else
                        right -= text_indent;
                    line.setLineWidth(qMax<qreal>(line.naturalTextWidth(), (right-left).toReal()));

                    if (haveWordOrAnyWrapMode) {
                        option.setWrapMode(BOBUIextOption::WordWrap);
                        tl->setTextOption(option);
                    }
                }

            }

            QFixed lineBreakHeight, lineHeight, lineAdjustment, lineBottom;
            qreal scaling = (q->paintDevice() && q->paintDevice()->logicalDpiY() != bobui_defaultDpi()) ?
                            qreal(q->paintDevice()->logicalDpiY()) / qreal(bobui_defaultDpi()) : 1;
            getLineHeightParams(blockFormat, line, scaling, &lineAdjustment, &lineBreakHeight, &lineHeight, &lineBottom);

            while (layoutStruct->pageHeight > 0 && layoutStruct->absoluteY() + lineBreakHeight > layoutStruct->pageBottom &&
                layoutStruct->contentHeight() >= lineBreakHeight) {
                if (layoutStruct->pageHeight == QFIXED_MAX) {
                    layoutStruct->y = QFIXED_MAX - layoutStruct->frameY;
                    break;
                }

                layoutStruct->newPage();

                floatMargins(layoutStruct->y, layoutStruct, &left, &right);
                left = qMax(left, l);
                right = qMin(right, r);
                if (dir == BobUI::LeftToRight)
                    left += text_indent;
                else
                    right -= text_indent;
            }

            line.setPosition(QPointF((left - layoutStruct->x_left).toReal(), (layoutStruct->y - cy - lineAdjustment).toReal()));
            bottom = layoutStruct->y + lineBottom;
            layoutStruct->y += lineHeight;
            layoutStruct->contentsWidth
                = qMax<QFixed>(layoutStruct->contentsWidth, QFixed::fromReal(line.x() + line.naturalTextWidth()) + totalRightMargin);

            // position floats
            for (int i = 0; i < layoutStruct->pendingFloats.size(); ++i) {
                BOBUIextFrame *f = layoutStruct->pendingFloats.at(i);
                positionFloat(f);
            }
            layoutStruct->pendingFloats.clear();
        }
        layoutStruct->y = qMax(layoutStruct->y, bottom);
        tl->endLayout();
    } else {
        const int cnt = tl->lineCount();
        QFixed bottom;
        for (int i = 0; i < cnt; ++i) {
            qCDebug(lcLayout) << "going to move text line" << i;
            BOBUIextLine line = tl->lineAt(i);
            layoutStruct->contentsWidth
                = qMax(layoutStruct->contentsWidth, QFixed::fromReal(line.x() + tl->lineAt(i).naturalTextWidth()) + totalRightMargin);

            QFixed lineBreakHeight, lineHeight, lineAdjustment, lineBottom;
            qreal scaling = (q->paintDevice() && q->paintDevice()->logicalDpiY() != bobui_defaultDpi()) ?
                            qreal(q->paintDevice()->logicalDpiY()) / qreal(bobui_defaultDpi()) : 1;
            getLineHeightParams(blockFormat, line, scaling, &lineAdjustment, &lineBreakHeight, &lineHeight, &lineBottom);

            if (layoutStruct->pageHeight != QFIXED_MAX) {
                if (layoutStruct->absoluteY() + lineBreakHeight > layoutStruct->pageBottom)
                    layoutStruct->newPage();
                line.setPosition(QPointF(line.position().x(), (layoutStruct->y - lineAdjustment).toReal() - tl->position().y()));
            }
            bottom = layoutStruct->y + lineBottom;
            layoutStruct->y += lineHeight;
        }
        layoutStruct->y = qMax(layoutStruct->y, bottom);
        if (layoutStruct->updateRect.isValid()
            && blockLength > 1) {
            if (layoutFrom >= blockPosition + blockLength) {
                // if our height didn't change and the change in the document is
                // in one of the later paragraphs, then we don't need to repaint
                // this one
                layoutStruct->updateRect.setTop(qMax(layoutStruct->updateRect.top(), layoutStruct->y.toReal()));
            } else if (layoutTo < blockPosition) {
                if (oldPosition == tl->position())
                    // if the change in the document happened earlier in the document
                    // and our position did /not/ change because none of the earlier paragraphs
                    // or frames changed their height, then we don't need to repaint
                    // this one
                    layoutStruct->updateRect.setBottom(qMin(layoutStruct->updateRect.bottom(), tl->position().y()));
                else
                    layoutStruct->updateRect.setBottom(qreal(INT_MAX)); // reset
            }
        }
    }

    // ### doesn't take floats into account. would need to do it per line. but how to retrieve then? (Simon)
    const QFixed margins = totalLeftMargin + totalRightMargin;
    layoutStruct->minimumWidth = qMax(layoutStruct->minimumWidth, QFixed::fromReal(tl->minimumWidth()) + margins);

    const QFixed maxW = QFixed::fromReal(tl->maximumWidth()) + margins;

    if (maxW > 0) {
        if (layoutStruct->maximumWidth == QFIXED_MAX)
            layoutStruct->maximumWidth = maxW;
        else
            layoutStruct->maximumWidth = qMax(layoutStruct->maximumWidth, maxW);
    }
}

void BOBUIextDocumentLayoutPrivate::floatMargins(QFixed y, const BOBUIextLayoutStruct *layoutStruct,
                                              QFixed *left, QFixed *right) const
{
//     qDebug() << "floatMargins y=" << y;
    *left = layoutStruct->x_left;
    *right = layoutStruct->x_right;
    BOBUIextFrameData *lfd = data(layoutStruct->frame);
    for (int i = 0; i < lfd->floats.size(); ++i) {
        BOBUIextFrameData *fd = data(lfd->floats.at(i));
        if (!fd->layoutDirty) {
            if (fd->position.y <= y && fd->position.y + fd->size.height > y) {
//                 qDebug() << "adjusting with float" << f << fd->position.x()<< fd->size.width();
                if (lfd->floats.at(i)->frameFormat().position() == BOBUIextFrameFormat::FloatLeft)
                    *left = qMax(*left, fd->position.x + fd->size.width);
                else
                    *right = qMin(*right, fd->position.x);
            }
        }
    }
//     qDebug() << "floatMargins: left="<<*left<<"right="<<*right<<"y="<<y;
}

QFixed BOBUIextDocumentLayoutPrivate::findY(QFixed yFrom, const BOBUIextLayoutStruct *layoutStruct, QFixed requiredWidth) const
{
    QFixed right, left;
    requiredWidth = qMin(requiredWidth, layoutStruct->x_right - layoutStruct->x_left);

//     qDebug() << "findY:" << yFrom;
    while (1) {
        floatMargins(yFrom, layoutStruct, &left, &right);
//         qDebug() << "    yFrom=" << yFrom<<"right=" << right << "left=" << left << "requiredWidth=" << requiredWidth;
        if (right-left >= requiredWidth)
            break;

        // move float down until we find enough space
        QFixed newY = QFIXED_MAX;
        BOBUIextFrameData *lfd = data(layoutStruct->frame);
        for (int i = 0; i < lfd->floats.size(); ++i) {
            BOBUIextFrameData *fd = data(lfd->floats.at(i));
            if (!fd->layoutDirty) {
                if (fd->position.y <= yFrom && fd->position.y + fd->size.height > yFrom)
                    newY = qMin(newY, fd->position.y + fd->size.height);
            }
        }
        if (newY == QFIXED_MAX)
            break;
        yFrom = newY;
    }
    return yFrom;
}

BOBUIextDocumentLayout::BOBUIextDocumentLayout(BOBUIextDocument *doc)
    : QAbstractTextDocumentLayout(*new BOBUIextDocumentLayoutPrivate, doc)
{
    registerHandler(BOBUIextFormat::ImageObject, new BOBUIextImageHandler(this));
}


void BOBUIextDocumentLayout::draw(QPainter *painter, const PaintContext &context)
{
    Q_D(BOBUIextDocumentLayout);
    BOBUIextFrame *frame = d->document->rootFrame();
    BOBUIextFrameData *fd = data(frame);

    if (fd->sizeDirty)
        return;

    if (context.clip.isValid()) {
        d->ensureLayouted(QFixed::fromReal(context.clip.bottom()));
    } else {
        d->ensureLayoutFinished();
    }

    QFixed width = fd->size.width;
    if (d->document->pageSize().width() == 0 && d->viewportRect.isValid()) {
        // we're in NoWrap mode, meaning the frame should expand to the viewport
        // so that backgrounds are drawn correctly
        fd->size.width = qMax(width, QFixed::fromReal(d->viewportRect.right()));
    }

    // Make sure we conform to the root frames bounds when drawing.
    d->clipRect = QRectF(fd->position.toPointF(), fd->size.toSizeF()).adjusted(fd->leftMargin.toReal(), 0, -fd->rightMargin.toReal(), 0);
    d->drawFrame(QPointF(), painter, context, frame);
    fd->size.width = width;
}

void BOBUIextDocumentLayout::setViewport(const QRectF &viewport)
{
    Q_D(BOBUIextDocumentLayout);
    d->viewportRect = viewport;
}

static void markFrames(BOBUIextFrame *current, int from, int oldLength, int length)
{
    int end = qMax(oldLength, length) + from;

    if (current->firstPosition() >= end || current->lastPosition() < from)
        return;

    BOBUIextFrameData *fd = data(current);
    // float got removed in editing operation
    fd->floats.removeAll(nullptr);

    fd->layoutDirty = true;
    fd->sizeDirty = true;

//     qDebug("    marking frame (%d--%d) as dirty", current->firstPosition(), current->lastPosition());
    QList<BOBUIextFrame *> children = current->childFrames();
    for (int i = 0; i < children.size(); ++i)
        markFrames(children.at(i), from, oldLength, length);
}

void BOBUIextDocumentLayout::documentChanged(int from, int oldLength, int length)
{
    Q_D(BOBUIextDocumentLayout);

    BOBUIextBlock blockIt = document()->findBlock(from);
    BOBUIextBlock endIt = document()->findBlock(qMax(0, from + length - 1));
    if (endIt.isValid())
        endIt = endIt.next();
     for (; blockIt.isValid() && blockIt != endIt; blockIt = blockIt.next())
         blockIt.clearLayout();

    if (!d->docPrivate->canLayout())
        return;

    QRectF updateRect;

    d->lazyLayoutStepSize = 1000;
    d->sizeChangedTimer.stop();
    d->insideDocumentChange = true;

    const int documentLength = d->docPrivate->length();
    const bool fullLayout = (oldLength == 0 && length == documentLength);
    const bool smallChange = documentLength > 0
                             && (qMax(length, oldLength) * 100 / documentLength) < 5;

    // don't show incremental layout progress (avoid scroll bar flicker)
    // if we see only a small change in the document and we're either starting
    // a layout run or we're already in progress for that and we haven't seen
    // any bigger change previously (showLayoutProgress already false)
    if (smallChange
        && (d->currentLazyLayoutPosition == -1 || d->showLayoutProgress == false))
        d->showLayoutProgress = false;
    else
        d->showLayoutProgress = true;

    if (fullLayout) {
        d->contentHasAlignment = false;
        d->currentLazyLayoutPosition = 0;
        d->checkPoints.clear();
        d->layoutStep();
    } else {
        d->ensureLayoutedByPosition(from);
        updateRect = doLayout(from, oldLength, length);
    }

    if (!d->layoutTimer.isActive() && d->currentLazyLayoutPosition != -1)
        d->layoutTimer.start(10, this);

    d->insideDocumentChange = false;

    if (d->showLayoutProgress) {
        const QSizeF newSize = dynamicDocumentSize();
        if (newSize != d->lastReportedSize) {
            d->lastReportedSize = newSize;
            emit documentSizeChanged(newSize);
        }
    }

    if (!updateRect.isValid()) {
        // don't use the frame size, it might have shrunken
        updateRect = QRectF(QPointF(0, 0), QSizeF(qreal(INT_MAX), qreal(INT_MAX)));
    }

    emit update(updateRect);
}

QRectF BOBUIextDocumentLayout::doLayout(int from, int oldLength, int length)
{
    Q_D(BOBUIextDocumentLayout);

//     qDebug("documentChange: from=%d, oldLength=%d, length=%d", from, oldLength, length);

    // mark all frames between f_start and f_end as dirty
    markFrames(d->docPrivate->rootFrame(), from, oldLength, length);

    QRectF updateRect;

    BOBUIextFrame *root = d->docPrivate->rootFrame();
    if (data(root)->sizeDirty)
        updateRect = d->layoutFrame(root, from, from + length);
    data(root)->layoutDirty = false;

    if (d->currentLazyLayoutPosition == -1)
        layoutFinished();
    else if (d->showLayoutProgress)
        d->sizeChangedTimer.start(0, this);

    return updateRect;
}

int BOBUIextDocumentLayout::hitTest(const QPointF &point, BobUI::HitTestAccuracy accuracy) const
{
    Q_D(const BOBUIextDocumentLayout);
    d->ensureLayouted(QFixed::fromReal(point.y()));
    BOBUIextFrame *f = d->docPrivate->rootFrame();
    int position = 0;
    BOBUIextLayout *l = nullptr;
    QFixedPoint pointf;
    pointf.x = QFixed::fromReal(point.x());
    pointf.y = QFixed::fromReal(point.y());
    BOBUIextDocumentLayoutPrivate::HitPoint p = d->hitTest(f, pointf, &position, &l, accuracy);
    if (accuracy == BobUI::ExactHit && p < BOBUIextDocumentLayoutPrivate::PointExact)
        return -1;

    // ensure we stay within document bounds
    int lastPos = f->lastPosition();
    if (l && !l->preeditAreaText().isEmpty())
        lastPos += l->preeditAreaText().size();
    if (position > lastPos)
        position = lastPos;
    else if (position < 0)
        position = 0;

    return position;
}

void BOBUIextDocumentLayout::resizeInlineObject(BOBUIextInlineObject item, int posInDocument, const BOBUIextFormat &format)
{
    Q_D(BOBUIextDocumentLayout);
    BOBUIextCharFormat f = format.toCharFormat();
    Q_ASSERT(f.isValid());
    BOBUIextObjectHandler handler = d->handlers.value(f.objectType());
    if (!handler.component)
        return;

    QSizeF intrinsic = handler.iface->intrinsicSize(d->document, posInDocument, format);

    BOBUIextFrameFormat::Position pos = BOBUIextFrameFormat::InFlow;
    BOBUIextFrame *frame = qobject_cast<BOBUIextFrame *>(d->document->objectForFormat(f));
    if (frame) {
        pos = frame->frameFormat().position();
        BOBUIextFrameData *fd = data(frame);
        fd->sizeDirty = false;
        fd->size = QFixedSize::fromSizeF(intrinsic);
        fd->minimumWidth = fd->maximumWidth = fd->size.width;
    }

    QSizeF inlineSize = (pos == BOBUIextFrameFormat::InFlow ? intrinsic : QSizeF(0, 0));
    item.setWidth(inlineSize.width());

    switch (f.verticalAlignment()) {
    case BOBUIextCharFormat::AlignMiddle: {
        QFontMetrics m(f.font());
        qreal halfX = m.xHeight()/2.;
        item.setAscent((inlineSize.height() + halfX) / 2.);
        item.setDescent((inlineSize.height() - halfX) / 2.);
        break;
    }
    case BOBUIextCharFormat::AlignBaseline: {
        QFontMetrics m(f.font());
        qreal descent = m.descent();
        item.setDescent(descent);
        item.setAscent(inlineSize.height() - descent);
        break;
    }
    default:
        item.setDescent(0);
        item.setAscent(inlineSize.height());
        break;
    }
}

void BOBUIextDocumentLayout::positionInlineObject(BOBUIextInlineObject item, int posInDocument, const BOBUIextFormat &format)
{
    Q_D(BOBUIextDocumentLayout);
    Q_UNUSED(posInDocument);
    if (item.width() != 0)
        // inline
        return;

    BOBUIextCharFormat f = format.toCharFormat();
    Q_ASSERT(f.isValid());
    BOBUIextObjectHandler handler = d->handlers.value(f.objectType());
    if (!handler.component)
        return;

    BOBUIextFrame *frame = qobject_cast<BOBUIextFrame *>(d->document->objectForFormat(f));
    if (!frame)
        return;

    BOBUIextBlock b = d->document->findBlock(frame->firstPosition());
    BOBUIextLine line;
    if (b.position() <= frame->firstPosition() && b.position() + b.length() > frame->lastPosition())
        line = b.layout()->lineAt(b.layout()->lineCount()-1);
//     qDebug() << "layoutObject: line.isValid" << line.isValid() << b.position() << b.length() <<
//         frame->firstPosition() << frame->lastPosition();
    d->positionFloat(frame, line.isValid() ? &line : nullptr);
}

void BOBUIextDocumentLayout::drawInlineObject(QPainter *p, const QRectF &rect, BOBUIextInlineObject item,
                                           int posInDocument, const BOBUIextFormat &format)
{
    Q_D(BOBUIextDocumentLayout);
    BOBUIextCharFormat f = format.toCharFormat();
    Q_ASSERT(f.isValid());
    BOBUIextFrame *frame = qobject_cast<BOBUIextFrame *>(d->document->objectForFormat(f));
    if (frame && frame->frameFormat().position() != BOBUIextFrameFormat::InFlow)
        return; // don't draw floating frames from inline objects here but in drawFlow instead

//    qDebug() << "drawObject at" << r;
    QAbstractTextDocumentLayout::drawInlineObject(p, rect, item, posInDocument, format);
}

int BOBUIextDocumentLayout::dynamicPageCount() const
{
    Q_D(const BOBUIextDocumentLayout);
    const QSizeF pgSize = d->document->pageSize();
    if (pgSize.height() < 0)
        return 1;
    return qCeil(dynamicDocumentSize().height() / pgSize.height());
}

QSizeF BOBUIextDocumentLayout::dynamicDocumentSize() const
{
    Q_D(const BOBUIextDocumentLayout);
    return data(d->docPrivate->rootFrame())->size.toSizeF();
}

int BOBUIextDocumentLayout::pageCount() const
{
    Q_D(const BOBUIextDocumentLayout);
    d->ensureLayoutFinished();
    return dynamicPageCount();
}

QSizeF BOBUIextDocumentLayout::documentSize() const
{
    Q_D(const BOBUIextDocumentLayout);
    d->ensureLayoutFinished();
    return dynamicDocumentSize();
}

void BOBUIextDocumentLayoutPrivate::ensureLayouted(QFixed y) const
{
    Q_Q(const BOBUIextDocumentLayout);
    if (currentLazyLayoutPosition == -1)
        return;
    const QSizeF oldSize = q->dynamicDocumentSize();
    Q_UNUSED(oldSize);

    if (checkPoints.isEmpty())
        layoutStep();

    while (currentLazyLayoutPosition != -1
           && checkPoints.last().y < y)
        layoutStep();
}

void BOBUIextDocumentLayoutPrivate::ensureLayoutedByPosition(int position) const
{
    if (currentLazyLayoutPosition == -1)
        return;
    if (position < currentLazyLayoutPosition)
        return;
    while (currentLazyLayoutPosition != -1
           && currentLazyLayoutPosition < position) {
        const_cast<BOBUIextDocumentLayout *>(q_func())->doLayout(currentLazyLayoutPosition, 0, INT_MAX - currentLazyLayoutPosition);
    }
}

void BOBUIextDocumentLayoutPrivate::layoutStep() const
{
    ensureLayoutedByPosition(currentLazyLayoutPosition + lazyLayoutStepSize);
    lazyLayoutStepSize = qMin(200000, lazyLayoutStepSize * 2);
}

void BOBUIextDocumentLayout::setCursorWidth(int width)
{
    Q_D(BOBUIextDocumentLayout);
    d->cursorWidth = width;
}

int BOBUIextDocumentLayout::cursorWidth() const
{
    Q_D(const BOBUIextDocumentLayout);
    return d->cursorWidth;
}

void BOBUIextDocumentLayout::setFixedColumnWidth(int width)
{
    Q_D(BOBUIextDocumentLayout);
    d->fixedColumnWidth = width;
}

QRectF BOBUIextDocumentLayout::tableCellBoundingRect(BOBUIextTable *table, const BOBUIextTableCell &cell) const
{
    if (!cell.isValid())
        return QRectF();

    BOBUIextTableData *td = static_cast<BOBUIextTableData *>(data(table));

    QRectF tableRect = tableBoundingRect(table);
    QRectF cellRect = td->cellRect(cell);

    return cellRect.translated(tableRect.topLeft());
}

QRectF BOBUIextDocumentLayout::tableBoundingRect(BOBUIextTable *table) const
{
    Q_D(const BOBUIextDocumentLayout);
    if (!d->docPrivate->canLayout())
        return QRectF();
    d->ensureLayoutFinished();

    QPointF pos;
    const int framePos = table->firstPosition();
    BOBUIextFrame *f = table;
    while (f) {
        BOBUIextFrameData *fd = data(f);
        pos += fd->position.toPointF();

        if (f != table) {
            if (BOBUIextTable *table = qobject_cast<BOBUIextTable *>(f)) {
                BOBUIextTableCell cell = table->cellAt(framePos);
                if (cell.isValid())
                    pos += static_cast<BOBUIextTableData *>(fd)->cellPosition(table, cell).toPointF();
            }
        }

        f = f->parentFrame();
    }
    return QRectF(pos, data(table)->size.toSizeF());
}

QRectF BOBUIextDocumentLayout::frameBoundingRect(BOBUIextFrame *frame) const
{
    Q_D(const BOBUIextDocumentLayout);
    if (!d->docPrivate->canLayout())
        return QRectF();
    d->ensureLayoutFinished();
    return d->frameBoundingRectInternal(frame);
}

QRectF BOBUIextDocumentLayoutPrivate::frameBoundingRectInternal(BOBUIextFrame *frame) const
{
    QPointF pos;
    const int framePos = frame->firstPosition();
    BOBUIextFrame *f = frame;
    while (f) {
        BOBUIextFrameData *fd = data(f);
        pos += fd->position.toPointF();

        if (BOBUIextTable *table = qobject_cast<BOBUIextTable *>(f)) {
            BOBUIextTableCell cell = table->cellAt(framePos);
            if (cell.isValid())
                pos += static_cast<BOBUIextTableData *>(fd)->cellPosition(table, cell).toPointF();
        }

        f = f->parentFrame();
    }
    return QRectF(pos, data(frame)->size.toSizeF());
}

QRectF BOBUIextDocumentLayout::blockBoundingRect(const BOBUIextBlock &block) const
{
    Q_D(const BOBUIextDocumentLayout);
    if (!d->docPrivate->canLayout() || !block.isValid() || !block.isVisible())
        return QRectF();
    d->ensureLayoutedByPosition(block.position() + block.length());
    BOBUIextFrame *frame = d->document->frameAt(block.position());
    QPointF offset;
    const int blockPos = block.position();

    while (frame) {
        BOBUIextFrameData *fd = data(frame);
        offset += fd->position.toPointF();

        if (BOBUIextTable *table = qobject_cast<BOBUIextTable *>(frame)) {
            BOBUIextTableCell cell = table->cellAt(blockPos);
            if (cell.isValid())
                offset += static_cast<BOBUIextTableData *>(fd)->cellPosition(table, cell).toPointF();
        }

        frame = frame->parentFrame();
    }

    const BOBUIextLayout *layout = block.layout();
    QRectF rect = layout->boundingRect();
    rect.moveTopLeft(layout->position() + offset);
    return rect;
}

int BOBUIextDocumentLayout::layoutStatus() const
{
    Q_D(const BOBUIextDocumentLayout);
    int pos = d->currentLazyLayoutPosition;
    if (pos == -1)
        return 100;
    return pos * 100 / BOBUIextDocumentPrivate::get(d->document)->length();
}

void BOBUIextDocumentLayout::timerEvent(BOBUIimerEvent *e)
{
    Q_D(BOBUIextDocumentLayout);
    if (e->timerId() == d->layoutTimer.timerId()) {
        if (d->currentLazyLayoutPosition != -1)
            d->layoutStep();
    } else if (e->timerId() == d->sizeChangedTimer.timerId()) {
        d->lastReportedSize = dynamicDocumentSize();
        emit documentSizeChanged(d->lastReportedSize);
        d->sizeChangedTimer.stop();

        if (d->currentLazyLayoutPosition == -1) {
            const int newCount = dynamicPageCount();
            if (newCount != d->lastPageCount) {
                d->lastPageCount = newCount;
                emit pageCountChanged(newCount);
            }
        }
    } else {
        QAbstractTextDocumentLayout::timerEvent(e);
    }
}

void BOBUIextDocumentLayout::layoutFinished()
{
    Q_D(BOBUIextDocumentLayout);
    d->layoutTimer.stop();
    if (!d->insideDocumentChange)
        d->sizeChangedTimer.start(0, this);
    // reset
    d->showLayoutProgress = true;
}

void BOBUIextDocumentLayout::ensureLayouted(qreal y)
{
    d_func()->ensureLayouted(QFixed::fromReal(y));
}

qreal BOBUIextDocumentLayout::idealWidth() const
{
    Q_D(const BOBUIextDocumentLayout);
    d->ensureLayoutFinished();
    return d->idealWidth;
}

bool BOBUIextDocumentLayout::contentHasAlignment() const
{
    Q_D(const BOBUIextDocumentLayout);
    return d->contentHasAlignment;
}

qreal BOBUIextDocumentLayoutPrivate::scaleToDevice(qreal value) const
{
    if (!paintDevice)
        return value;
    return value * paintDevice->logicalDpiY() / qreal(bobui_defaultDpi());
}

QFixed BOBUIextDocumentLayoutPrivate::scaleToDevice(QFixed value) const
{
    if (!paintDevice)
        return value;
    return value * QFixed(paintDevice->logicalDpiY()) / QFixed(bobui_defaultDpi());
}

BOBUI_END_NAMESPACE

#include "moc_bobuiextdocumentlayout_p.cpp"
