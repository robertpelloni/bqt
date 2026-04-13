// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#ifndef QGRIDLAYOUTENGINE_P_H
#define QGRIDLAYOUTENGINE_P_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the BobUI API.  It exists for the convenience
// of the graphics view layout classes.  This header
// file may change from version to version without notice, or even be removed.
//
// We mean it.
//

#include <BobUIGui/private/bobuiguiglobal_p.h>

#include <BobUICore/qalgorithms.h>
#include <BobUICore/qbitarray.h>
#include <BobUICore/qlist.h>
#include <BobUICore/qmap.h>
#include <BobUICore/qsize.h>
#include <BobUICore/qrect.h>
#include <BobUICore/qdebug.h>

#include <float.h>
#include "qlayoutpolicy_p.h"
#include "qabstractlayoutstyleinfo_p.h"

// #define QGRIDLAYOUTENGINE_DEBUG

BOBUI_BEGIN_NAMESPACE

class QStyle;
class QWidget;

// ### deal with Descent in a similar way
enum {
    MinimumSize = BobUI::MinimumSize,
    PreferredSize = BobUI::PreferredSize,
    MaximumSize = BobUI::MaximumSize,
    NSizes
};

// do not reorder
enum LayoutSide {
    Left,
    Top,
    Right,
    Bottom
};

enum {
    NoConstraint,
    HorizontalConstraint,   // Width depends on the height
    VerticalConstraint,     // Height depends on the width
    UnknownConstraint,      // need to update cache
    UnfeasibleConstraint    // not feasible, it be has some items with Vertical and others with Horizontal constraints
};

/*
    Minimal container to store BobUI::Orientation-discriminated values.

    The salient feature is the indexing operator, which takes
    BobUI::Orientation (and assumes it's passed only BobUI::Horizontal or BobUI::Vertical).
*/
template <typename T>
class QHVContainer {
    T m_data[2];

    static_assert(BobUI::Horizontal == 0x1);
    static_assert(BobUI::Vertical == 0x2);
    static constexpr int map(BobUI::Orientation o)
    {
        Q_ASSERT(o == BobUI::Horizontal || o == BobUI::Vertical); // Q_PRE
        return int(o) - 1;
    }
    static constexpr int mapOther(BobUI::Orientation o)
    {
        Q_ASSERT(o == BobUI::Horizontal || o == BobUI::Vertical); // Q_PRE
        return 2 - int(o);
    }
public:
    constexpr QHVContainer(const T &h, const T &v)
            noexcept(std::is_nothrow_copy_constructible_v<T>)
        : m_data{h, v} {}
    QHVContainer() = default;

    constexpr T &operator[](BobUI::Orientation o) { return m_data[map(o)]; }
    constexpr const T &operator[](BobUI::Orientation o) const { return m_data[map(o)]; }

    constexpr T &other(BobUI::Orientation o) { return m_data[mapOther(o)]; }
    constexpr const T &other(BobUI::Orientation o) const { return m_data[mapOther(o)]; }

    constexpr void transpose() noexcept { qSwap(m_data[0], m_data[1]); }
    constexpr QHVContainer transposed() const
        noexcept(std::is_nothrow_copy_constructible_v<T>)
    { return {m_data[1], m_data[0]}; }
};

template <typename T>
class QLayoutParameter
{
public:
    enum State { Default, User, Cached };

    inline QLayoutParameter() : q_value(T()), q_state(Default) {}
    inline QLayoutParameter(T value, State state = Default) : q_value(value), q_state(state) {}

    inline void setUserValue(T value) {
        q_value = value;
        q_state = User;
    }
    inline void setCachedValue(T value) const {
        if (q_state != User) {
            q_value = value;
            q_state = Cached;
        }
    }
    inline T value() const { return q_value; }
    inline T value(T defaultValue) const { return isUser() ? q_value : defaultValue; }
    inline bool isDefault() const { return q_state == Default; }
    inline bool isUser() const { return q_state == User; }
    inline bool isCached() const { return q_state == Cached; }

private:
    mutable T q_value;
    mutable State q_state;
};

class QStretchParameter : public QLayoutParameter<int>
{
public:
    QStretchParameter() : QLayoutParameter<int>(-1) {}

};

class Q_GUI_EXPORT QGridLayoutBox
{
public:
    inline QGridLayoutBox()
        : q_minimumSize(0), q_preferredSize(0), q_maximumSize(FLT_MAX),
          q_minimumDescent(-1), q_minimumAscent(-1) {}

    void add(const QGridLayoutBox &other, int stretch, qreal spacing);
    void combine(const QGridLayoutBox &other);
    void normalize();

#ifdef QGRIDLAYOUTENGINE_DEBUG
    void dump(int indent = 0) const;
#endif
    // This code could use the union-struct-array trick, but a compiler
    // bug prevents this from working.
    qreal q_minimumSize;
    qreal q_preferredSize;
    qreal q_maximumSize;
    qreal q_minimumDescent;
    qreal q_minimumAscent;
    inline qreal &q_sizes(int which)
    {
        return const_cast<qreal&>(static_cast<const QGridLayoutBox*>(this)->q_sizes(which));
    }
    inline const qreal &q_sizes(int which) const
    {
        switch (which) {
        case BobUI::MinimumSize:
            return q_minimumSize;
        case BobUI::PreferredSize:
            return q_preferredSize;
        case BobUI::MaximumSize:
            return q_maximumSize;
        case BobUI::MinimumDescent:
            return q_minimumDescent;
        case (BobUI::MinimumDescent + 1):
            return q_minimumAscent;
        default:
            Q_UNREACHABLE();
        }
    }
};
Q_DECLARE_TYPEINFO(QGridLayoutBox, Q_RELOCATABLE_TYPE); // cannot be Q_PRIMITIVE_TYPE, as q_maximumSize, say, is != 0

bool operator==(const QGridLayoutBox &box1, const QGridLayoutBox &box2);
inline bool operator!=(const QGridLayoutBox &box1, const QGridLayoutBox &box2)
    { return !operator==(box1, box2); }

class QGridLayoutMultiCellData
{
public:
    inline QGridLayoutMultiCellData() : q_stretch(-1) {}

    QGridLayoutBox q_box;
    int q_stretch;
};

typedef QMap<std::pair<int, int>, QGridLayoutMultiCellData> MultiCellMap;

class QGridLayoutRowInfo;

class QGridLayoutRowData
{
public:
    void reset(int count);
    void distributeMultiCells(const QGridLayoutRowInfo &rowInfo, bool snapToPixelGrid);
    void calculateGeometries(int start, int end, qreal targetSize, qreal *positions, qreal *sizes,
                             qreal *descents, const QGridLayoutBox &totalBox,
                             const QGridLayoutRowInfo &rowInfo, bool snapToPixelGrid);
    QGridLayoutBox totalBox(int start, int end) const;
    void stealBox(int start, int end, int which, qreal *positions, qreal *sizes);

#ifdef QGRIDLAYOUTENGINE_DEBUG
    void dump(int indent = 0) const;
#endif

    QBitArray ignore;   // ### rename q_
    QList<QGridLayoutBox> boxes;
    MultiCellMap multiCellMap;
    QList<int> stretches;
    QList<qreal> spacings;
    bool hasIgnoreFlag;
};

class QGridLayoutRowInfo
{
public:
    inline QGridLayoutRowInfo() : count(0) {}

    void insertOrRemoveRows(int row, int delta);

#ifdef QGRIDLAYOUTENGINE_DEBUG
    void dump(int indent = 0) const;
#endif

    int count;
    QList<QStretchParameter> stretches;
    QList<QLayoutParameter<qreal>> spacings;
    QList<BobUI::Alignment> alignments;
    QList<QGridLayoutBox> boxes;
};


class Q_GUI_EXPORT QGridLayoutItem
{
public:
    QGridLayoutItem(int row, int column, int rowSpan = 1, int columnSpan = 1,
                    BobUI::Alignment alignment = { });
    virtual ~QGridLayoutItem() {}

    inline int firstRow() const { return q_firstRows[BobUI::Vertical]; }
    inline int firstColumn() const { return q_firstRows[BobUI::Horizontal]; }
    inline int rowSpan() const { return q_rowSpans[BobUI::Vertical]; }
    inline int columnSpan() const { return q_rowSpans[BobUI::Horizontal]; }
    inline int lastRow() const { return firstRow() + rowSpan() - 1; }
    inline int lastColumn() const { return firstColumn() + columnSpan() - 1; }

    int firstRow(BobUI::Orientation orientation) const;
    int firstColumn(BobUI::Orientation orientation) const;
    int lastRow(BobUI::Orientation orientation) const;
    int lastColumn(BobUI::Orientation orientation) const;
    int rowSpan(BobUI::Orientation orientation) const;
    int columnSpan(BobUI::Orientation orientation) const;
    void setFirstRow(int row, BobUI::Orientation orientation = BobUI::Vertical);
    void setRowSpan(int rowSpan, BobUI::Orientation orientation = BobUI::Vertical);

    int stretchFactor(BobUI::Orientation orientation) const;
    void setStretchFactor(int stretch, BobUI::Orientation orientation);

    inline BobUI::Alignment alignment() const { return q_alignment; }
    inline void setAlignment(BobUI::Alignment alignment) { q_alignment = alignment; }

    virtual QLayoutPolicy::Policy sizePolicy(BobUI::Orientation orientation) const = 0;
    virtual QSizeF sizeHint(BobUI::SizeHint which, const QSizeF &constraint) const = 0;
    virtual bool isEmpty() const { return false; }

    virtual void setGeometry(const QRectF &rect) = 0;
    /*
      returns true if the size policy returns true for either hasHeightForWidth()
      or hasWidthForHeight()
     */
    virtual bool hasDynamicConstraint() const { return false; }
    virtual BobUI::Orientation dynamicConstraintOrientation() const { return BobUI::Horizontal; }


    virtual QLayoutPolicy::ControlTypes controlTypes(LayoutSide side) const;

    inline virtual QString toString() const { return QDebug::toString(this); }

    QRectF geometryWithin(qreal x, qreal y, qreal width, qreal height, qreal rowDescent, BobUI::Alignment align, bool snapToPixelGrid) const;
    QGridLayoutBox box(BobUI::Orientation orientation, bool snapToPixelGrid, qreal constraint = -1.0) const;


    void transpose();
    void insertOrRemoveRows(int row, int delta, BobUI::Orientation orientation = BobUI::Vertical);
    QSizeF effectiveMaxSize(const QSizeF &constraint) const;

#ifdef QGRIDLAYOUTENGINE_DEBUG
    void dump(int indent = 0) const;
#endif

private:
    QHVContainer<int> q_firstRows;
    QHVContainer<int> q_rowSpans;
    QHVContainer<int> q_stretches;
    BobUI::Alignment q_alignment;

};

class Q_GUI_EXPORT QGridLayoutEngine
{
public:
    QGridLayoutEngine(BobUI::Alignment defaultAlignment = { }, bool snapToPixelGrid = false);
    inline ~QGridLayoutEngine() { qDeleteAll(q_items); }

    int rowCount(BobUI::Orientation orientation) const;
    int columnCount(BobUI::Orientation orientation) const;
    inline int rowCount() const { return q_infos[BobUI::Vertical].count; }
    inline int columnCount() const { return q_infos[BobUI::Horizontal].count; }
    // returns the number of items inserted, which may be less than (rowCount * columnCount)
    int itemCount() const;
    QGridLayoutItem *itemAt(int index) const;

    int effectiveFirstRow(BobUI::Orientation orientation = BobUI::Vertical) const;
    int effectiveLastRow(BobUI::Orientation orientation = BobUI::Vertical) const;

    void setSpacing(qreal spacing, BobUI::Orientations orientations);
    qreal spacing(BobUI::Orientation orientation, const QAbstractLayoutStyleInfo *styleInfo) const;
    // ### setSpacingAfterRow(), spacingAfterRow()
    void setRowSpacing(int row, qreal spacing, BobUI::Orientation orientation = BobUI::Vertical);
    qreal rowSpacing(int row, BobUI::Orientation orientation = BobUI::Vertical) const;

    void setRowStretchFactor(int row, int stretch, BobUI::Orientation orientation = BobUI::Vertical);
    int rowStretchFactor(int row, BobUI::Orientation orientation = BobUI::Vertical) const;

    void setRowSizeHint(BobUI::SizeHint which, int row, qreal size,
                        BobUI::Orientation orientation = BobUI::Vertical);
    qreal rowSizeHint(BobUI::SizeHint which, int row,
                      BobUI::Orientation orientation = BobUI::Vertical) const;

    bool uniformCellWidths() const;
    void setUniformCellWidths(bool uniformCellWidths);

    bool uniformCellHeights() const;
    void setUniformCellHeights(bool uniformCellHeights);

    void setRowAlignment(int row, BobUI::Alignment alignment, BobUI::Orientation orientation);
    BobUI::Alignment rowAlignment(int row, BobUI::Orientation orientation) const;

    BobUI::Alignment effectiveAlignment(const QGridLayoutItem *layoutItem) const;


    void insertItem(QGridLayoutItem *item, int index);
    void addItem(QGridLayoutItem *item);
    void removeItem(QGridLayoutItem *item);
    void deleteItems()
    {
        const QList<QGridLayoutItem *> oldItems = q_items;
        q_items.clear();    // q_items are used as input when the grid is regenerated in removeRows
        // The following calls to removeRows are suboptimal
        int rows = rowCount(BobUI::Vertical);
        removeRows(0, rows, BobUI::Vertical);
        rows = rowCount(BobUI::Horizontal);
        removeRows(0, rows, BobUI::Horizontal);
        qDeleteAll(oldItems);
    }

    QGridLayoutItem *itemAt(int row, int column, BobUI::Orientation orientation = BobUI::Vertical) const;
    inline void insertRow(int row, BobUI::Orientation orientation = BobUI::Vertical)
        { insertOrRemoveRows(row, +1, orientation); }
    inline void removeRows(int row, int count, BobUI::Orientation orientation)
        { insertOrRemoveRows(row, -count, orientation); }

    void invalidate();
    void setGeometries(const QRectF &contentsGeometry, const QAbstractLayoutStyleInfo *styleInfo);
    QRectF cellRect(const QRectF &contentsGeometry, int row, int column, int rowSpan, int columnSpan,
                    const QAbstractLayoutStyleInfo *styleInfo) const;
    QSizeF sizeHint(BobUI::SizeHint which, const QSizeF &constraint,
                    const QAbstractLayoutStyleInfo *styleInfo) const;

    // heightForWidth / widthForHeight support
    QSizeF dynamicallyConstrainedSizeHint(BobUI::SizeHint which, const QSizeF &constraint) const;
    bool ensureDynamicConstraint() const;
    bool hasDynamicConstraint() const;
    BobUI::Orientation constraintOrientation() const;


    QLayoutPolicy::ControlTypes controlTypes(LayoutSide side) const;
    void transpose();
    void setVisualDirection(BobUI::LayoutDirection direction);
    BobUI::LayoutDirection visualDirection() const;
#ifdef QGRIDLAYOUTENGINE_DEBUG
    void dump(int indent = 0) const;
#endif

private:
    static int grossRoundUp(int n) { return ((n + 2) | 0x3) - 2; }

    void maybeExpandGrid(int row, int column, BobUI::Orientation orientation = BobUI::Vertical);
    void regenerateGrid();
    inline int internalGridRowCount() const { return grossRoundUp(rowCount()); }
    inline int internalGridColumnCount() const { return grossRoundUp(columnCount()); }
    void setItemAt(int row, int column, QGridLayoutItem *item);
    void insertOrRemoveRows(int row, int delta, BobUI::Orientation orientation = BobUI::Vertical);
    void fillRowData(QGridLayoutRowData *rowData,
                     const qreal *colPositions, const qreal *colSizes,
                     BobUI::Orientation orientation,
                     const QAbstractLayoutStyleInfo *styleInfo) const;
    void ensureEffectiveFirstAndLastRows() const;
    void ensureColumnAndRowData(QGridLayoutRowData *rowData, QGridLayoutBox *totalBox,
                                            const qreal *colPositions, const qreal *colSizes,
                                            BobUI::Orientation orientation,
                                            const QAbstractLayoutStyleInfo *styleInfo) const;

    void ensureGeometries(const QSizeF &size, const QAbstractLayoutStyleInfo *styleInfo) const;
protected:
    QList<QGridLayoutItem *> q_items;
private:
    // User input
    QList<QGridLayoutItem *> q_grid;
    QHVContainer<QLayoutParameter<qreal>> q_defaultSpacings;
    QHVContainer<QGridLayoutRowInfo> q_infos;
    BobUI::LayoutDirection m_visualDirection;

    // Configuration
    BobUI::Alignment m_defaultAlignment;
    unsigned m_snapToPixelGrid : 1;
    unsigned m_uniformCellWidths : 1;
    unsigned m_uniformCellHeights : 1;

    // Lazily computed from the above user input
    mutable QHVContainer<int> q_cachedEffectiveFirstRows;
    mutable QHVContainer<int> q_cachedEffectiveLastRows;
    mutable quint8 q_cachedConstraintOrientation : 3;

    // this is useful to cache
    mutable QHVContainer<QGridLayoutBox> q_totalBoxes;
    enum {
        NotCached = -2,             // Cache is empty. Happens when the engine is invalidated.
        CachedWithNoConstraint = -1 // cache has a totalBox without any HFW/WFH constraints.
        // >= 0                     // cache has a totalBox with this specific constraint.
    };
    mutable QHVContainer<qreal> q_totalBoxCachedConstraints;   // holds the constraint used for the cached totalBox

    // Layout item input
    mutable QGridLayoutRowData q_columnData;
    mutable QGridLayoutRowData q_rowData;

    // Output
    mutable QSizeF q_cachedSize;
    mutable QList<qreal> q_xx;
    mutable QList<qreal> q_yy;
    mutable QList<qreal> q_widths;
    mutable QList<qreal> q_heights;
    mutable QList<qreal> q_descents;

    friend class QGridLayoutItem;
};

BOBUI_END_NAMESPACE

#endif
