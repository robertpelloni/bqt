// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
#ifndef BOBUIEXTLAYOUT_H
#define BOBUIEXTLAYOUT_H

#include <BobUIGui/qcolor.h>
#include <BobUIGui/qevent.h>
#include <BobUIGui/qglyphrun.h>
#include <BobUIGui/bobuiextcursor.h>
#include <BobUIGui/bobuiextformat.h>
#include <BobUIGui/bobuiguiglobal.h>

#include <BobUICore/qlist.h>
#include <BobUICore/qnamespace.h>
#include <BobUICore/qobject.h>
#include <BobUICore/qrect.h>
#include <BobUICore/qstring.h>

BOBUI_BEGIN_NAMESPACE


class BOBUIextEngine;
class QFont;
#ifndef BOBUI_NO_RAWFONT
class QRawFont;
#endif
class QRect;
class QRegion;
class BOBUIextFormat;
class QPalette;
class QPainter;

class Q_GUI_EXPORT BOBUIextInlineObject
{
public:
    BOBUIextInlineObject(int i, BOBUIextEngine *e) : itm(i), eng(e) {}
    inline BOBUIextInlineObject() : itm(0), eng(nullptr) {}
    inline bool isValid() const { return eng; }

    QRectF rect() const;
    qreal width() const;
    qreal ascent() const;
    qreal descent() const;
    qreal height() const;

    BobUI::LayoutDirection textDirection() const;

    void setWidth(qreal w);
    void setAscent(qreal a);
    void setDescent(qreal d);

    int textPosition() const;

    int formatIndex() const;
    BOBUIextFormat format() const;

private:
    friend class BOBUIextLayout;
    int itm;
    BOBUIextEngine *eng;
};

class QPaintDevice;
class BOBUIextFormat;
class BOBUIextLine;
class BOBUIextBlock;
class BOBUIextOption;

class Q_GUI_EXPORT BOBUIextLayout
{
public:
    enum GlyphRunRetrievalFlag : quint16 {
        RetrieveGlyphIndexes    = 0x1,
        RetrieveGlyphPositions  = 0x2,
        RetrieveStringIndexes   = 0x4,
        RetrieveString          = 0x8,

        DefaultRetrievalFlags   = RetrieveGlyphIndexes | RetrieveGlyphPositions,
        RetrieveAll             = 0xffff
    };
    Q_DECLARE_FLAGS(GlyphRunRetrievalFlags, GlyphRunRetrievalFlag)

    // does itemization
    BOBUIextLayout();
    BOBUIextLayout(const QString& text);
    BOBUIextLayout(const QString &text, const QFont &font, const QPaintDevice *paintdevice = nullptr);
    BOBUIextLayout(const BOBUIextBlock &b);
    ~BOBUIextLayout();

    void setFont(const QFont &f);
    QFont font() const;

#ifndef BOBUI_NO_RAWFONT
    void setRawFont(const QRawFont &rawFont);
#endif

    void setText(const QString& string);
    QString text() const;

    void setTextOption(const BOBUIextOption &option);
    const BOBUIextOption &textOption() const;

    void setPreeditArea(int position, const QString &text);
    int preeditAreaPosition() const;
    QString preeditAreaText() const;

    struct FormatRange {
        int start;
        int length;
        BOBUIextCharFormat format;

        friend bool operator==(const FormatRange &lhs, const FormatRange &rhs)
        { return lhs.start == rhs.start && lhs.length == rhs.length && lhs.format == rhs.format; }
        friend bool operator!=(const FormatRange &lhs, const FormatRange &rhs)
        { return !operator==(lhs, rhs); }
    };
    void setFormats(const QList<FormatRange> &overrides);
    QList<FormatRange> formats() const;
    void clearFormats();

    void setCacheEnabled(bool enable);
    bool cacheEnabled() const;

    void setCursorMoveStyle(BobUI::CursorMoveStyle style);
    BobUI::CursorMoveStyle cursorMoveStyle() const;

    void beginLayout();
    void endLayout();
    void clearLayout();

    BOBUIextLine createLine();

    int lineCount() const;
    BOBUIextLine lineAt(int i) const;
    BOBUIextLine lineForTextPosition(int pos) const;

    enum CursorMode {
        SkipCharacters,
        SkipWords
    };
    bool isValidCursorPosition(int pos) const;
    int nextCursorPosition(int oldPos, CursorMode mode = SkipCharacters) const;
    int previousCursorPosition(int oldPos, CursorMode mode = SkipCharacters) const;
    int leftCursorPosition(int oldPos) const;
    int rightCursorPosition(int oldPos) const;

    void draw(QPainter *p, const QPointF &pos,
              const QList<FormatRange> &selections = QList<FormatRange>(),
              const QRectF &clip = QRectF()) const;
    void drawCursor(QPainter *p, const QPointF &pos, int cursorPosition) const;
    void drawCursor(QPainter *p, const QPointF &pos, int cursorPosition, int width) const;

    QPointF position() const;
    void setPosition(const QPointF &p);

    QRectF boundingRect() const;

    qreal minimumWidth() const;
    qreal maximumWidth() const;

#if !defined(BOBUI_NO_RAWFONT)

#  if BOBUI_VERSION < BOBUI_VERSION_CHECK(7, 0, 0)
    QList<QGlyphRun> glyphRuns(int from, int length, GlyphRunRetrievalFlags flags) const;
    QList<QGlyphRun> glyphRuns(int from = -1, int length = -1) const;
#  else
    QList<QGlyphRun> glyphRuns(int from = -1,
                               int length = -1,
                               GlyphRunRetrievalFlags flags = DefaultRetrievalFlags) const;
#  endif
#endif

    BOBUIextEngine *engine() const { return d; }
    void setFlags(int flags);
private:
    BOBUIextLayout(BOBUIextEngine *e) : d(e) {}
    Q_DISABLE_COPY(BOBUIextLayout)

    friend class QPainter;
    friend class QGraphicsSimpleTextItemPrivate;
    friend class QGraphicsSimpleTextItem;
    friend void bobui_format_text(const QFont &font, const QRectF &_r, int tf, int alignment, const BOBUIextOption *, const QString& str,
                               QRectF *brect, int tabstops, int* tabarray, int tabarraylen,
                               QPainter *painter);
    BOBUIextEngine *d;
};
Q_DECLARE_TYPEINFO(BOBUIextLayout::FormatRange, Q_RELOCATABLE_TYPE);
Q_DECLARE_OPERATORS_FOR_FLAGS(BOBUIextLayout::GlyphRunRetrievalFlags)

class Q_GUI_EXPORT BOBUIextLine
{
public:
    inline BOBUIextLine() : index(0), eng(nullptr) {}
    inline bool isValid() const { return eng; }

    QRectF rect() const;
    qreal x() const;
    qreal y() const;
    qreal width() const;
    qreal ascent() const;
    qreal descent() const;
    qreal height() const;
    qreal leading() const;

    void setLeadingIncluded(bool included);
    bool leadingIncluded() const;

    qreal naturalTextWidth() const;
    qreal horizontalAdvance() const;
    QRectF naturalTextRect() const;

    enum Edge {
        Leading,
        Trailing
    };
    enum CursorPosition {
        CursorBetweenCharacters,
        CursorOnCharacter
    };

    /* cursorPos gets set to the valid position */
    qreal cursorToX(int *cursorPos, Edge edge = Leading) const;
    inline qreal cursorToX(int cursorPos, Edge edge = Leading) const { return cursorToX(&cursorPos, edge); }
    int xToCursor(qreal x, CursorPosition = CursorBetweenCharacters) const;

    void setLineWidth(qreal width);
    void setNumColumns(int columns);
    void setNumColumns(int columns, qreal alignmentWidth);

    void setPosition(const QPointF &pos);
    QPointF position() const;

    int textStart() const;
    int textLength() const;

    int lineNumber() const { return index; }

    void draw(QPainter *painter, const QPointF &position) const;

#if !defined(BOBUI_NO_RAWFONT)
#  if BOBUI_VERSION < BOBUI_VERSION_CHECK(7, 0, 0)
    QList<QGlyphRun> glyphRuns(int from, int length, BOBUIextLayout::GlyphRunRetrievalFlags flags) const;
    QList<QGlyphRun> glyphRuns(int from = -1, int length = -1) const;
#  else
    QList<QGlyphRun> glyphRuns(int from = -1,
                               int length = -1,
                               BOBUIextLayout::GlyphRunRetrievalFlags flags = BOBUIextLayout::DefaultRetrievalFlags) const;
#  endif
#endif

private:
    BOBUIextLine(int line, BOBUIextEngine *e) : index(line), eng(e) {}
    void layout_helper(int numGlyphs);
    void draw_internal(QPainter *p, const QPointF &pos,
                       const BOBUIextLayout::FormatRange *selection) const;

    friend class BOBUIextLayout;
    friend class BOBUIextFragment;
    int index;
    BOBUIextEngine *eng;
};

BOBUI_END_NAMESPACE

#endif // BOBUIEXTLAYOUT_H
