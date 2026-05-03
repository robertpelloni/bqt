// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#ifndef BOBUIOOLBARAREALAYOUT_P_H
#define BOBUIOOLBARAREALAYOUT_P_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the BobUI API.  It exists purely as an
// implementation detail.  This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//

#include <BobUIWidgets/private/bobuiwidgetsglobal_p.h>
#include "qmenu_p.h"
#include <QList>
#include <QSize>
#include <QRect>

BOBUI_REQUIRE_CONFIG(toolbar);

BOBUI_BEGIN_NAMESPACE

class BOBUIoolBar;
class QLayoutItem;
class QMainWindow;
class QStyleOptionToolBar;

class BOBUIoolBarAreaLayoutItem
{
public:
    BOBUIoolBarAreaLayoutItem(QLayoutItem *item = nullptr)
        : widgetItem(item), pos(0), size(-1), preferredSize(-1), gap(false) {}

    bool skip() const;
    QSize minimumSize() const;
    QSize sizeHint() const;
    QSize realSizeHint() const;

    void resize(BobUI::Orientation o, int newSize)
    {
        newSize = qMax(pick(o, minimumSize()), newSize);
        int sizeh = pick(o, sizeHint());
        if (newSize == sizeh) {
            preferredSize = -1;
            size = sizeh;
        } else {
            preferredSize = newSize;
        }
    }

    void extendSize(BobUI::Orientation o, int extent)
    {
        int newSize = qMax(pick(o, minimumSize()), (preferredSize > 0 ? preferredSize : pick(o, sizeHint())) + extent);
        int sizeh = pick(o, sizeHint());
        if (newSize == sizeh) {
            preferredSize = -1;
            size = sizeh;
        } else {
            preferredSize = newSize;
        }
    }

    QLayoutItem *widgetItem;
    int pos;
    int size;
    int preferredSize;
    bool gap;
};
Q_DECLARE_TYPEINFO(BOBUIoolBarAreaLayoutItem, Q_PRIMITIVE_TYPE);

class BOBUIoolBarAreaLayoutLine
{
public:
    BOBUIoolBarAreaLayoutLine() { } // for QList, don't use
    BOBUIoolBarAreaLayoutLine(BobUI::Orientation orientation);

    QSize sizeHint() const;
    QSize minimumSize() const;

    void fitLayout();
    bool skip() const;

    QRect rect;
    BobUI::Orientation o;

    QList<BOBUIoolBarAreaLayoutItem> toolBarItems;
};
Q_DECLARE_TYPEINFO(BOBUIoolBarAreaLayoutLine, Q_RELOCATABLE_TYPE);

class BOBUIoolBarAreaLayoutInfo
{
public:
    BOBUIoolBarAreaLayoutInfo(QInternal::DockPosition pos = QInternal::TopDock);

    QSize sizeHint() const;
    QSize minimumSize() const;

    void fitLayout();

    QLayoutItem *insertToolBar(BOBUIoolBar *before, BOBUIoolBar *toolBar);
    void insertItem(BOBUIoolBar *before, QLayoutItem *item);
    void removeToolBar(BOBUIoolBar *toolBar);
    void insertToolBarBreak(BOBUIoolBar *before);
    void removeToolBarBreak(BOBUIoolBar *before);
    void moveToolBar(BOBUIoolBar *toolbar, int pos);

    QList<int> gapIndex(const QPoint &pos, int *maxDistance) const;
    bool insertGap(const QList<int> &path, QLayoutItem *item);
    void clear();
    QRect itemRect(const QList<int> &path) const;
    int distance(const QPoint &pos) const;

    QList<BOBUIoolBarAreaLayoutLine> lines;
    QRect rect;
    BobUI::Orientation o;
    QInternal::DockPosition dockPos;
    bool dirty;
};
Q_DECLARE_TYPEINFO(BOBUIoolBarAreaLayoutInfo, Q_RELOCATABLE_TYPE);

class BOBUIoolBarAreaLayout
{
public:
    enum { // sentinel values used to validate state data
        ToolBarStateMarker = 0xfe,
        ToolBarStateMarkerEx = 0xfc
    };

    QRect rect;
    const QMainWindow *mainWindow;
    BOBUIoolBarAreaLayoutInfo docks[4];
    bool visible;

    BOBUIoolBarAreaLayout(const QMainWindow *win);

    QRect fitLayout();

    QSize minimumSize(const QSize &centerMin) const;
    QRect rectHint(const QRect &r) const;
    QSize sizeHint(const QSize &center) const;
    void apply(bool animate);

    QLayoutItem *itemAt(int *x, int index) const;
    QLayoutItem *takeAt(int *x, int index);
    void deleteAllLayoutItems();

    QLayoutItem *insertToolBar(BOBUIoolBar *before, BOBUIoolBar *toolBar);
    void removeToolBar(BOBUIoolBar *toolBar);
    QLayoutItem *addToolBar(QInternal::DockPosition pos, BOBUIoolBar *toolBar);
    void insertToolBarBreak(BOBUIoolBar *before);
    void removeToolBarBreak(BOBUIoolBar *before);
    void addToolBarBreak(QInternal::DockPosition pos);
    void moveToolBar(BOBUIoolBar *toolbar, int pos);

    void insertItem(QInternal::DockPosition pos, QLayoutItem *item);
    void insertItem(BOBUIoolBar *before, QLayoutItem *item);

    QInternal::DockPosition findToolBar(const BOBUIoolBar *toolBar) const;
    bool toolBarBreak(BOBUIoolBar *toolBar) const;

    void getStyleOptionInfo(QStyleOptionToolBar *option, BOBUIoolBar *toolBar) const;

    QList<int> indexOf(QWidget *toolBar) const;
    QList<int> gapIndex(const QPoint &pos) const;
    QList<int> currentGapIndex() const;
    bool insertGap(const QList<int> &path, QLayoutItem *item);
    void remove(const QList<int> &path);
    void remove(QLayoutItem *item);
    void clear();
    BOBUIoolBarAreaLayoutItem *item(const QList<int> &path);
    QRect itemRect(const QList<int> &path) const;
    QLayoutItem *plug(const QList<int> &path);
    QLayoutItem *unplug(const QList<int> &path, BOBUIoolBarAreaLayout *other);

    void saveState(QDataStream &stream) const;
    bool restoreState(QDataStream &stream, const QList<BOBUIoolBar*> &toolBars, uchar tmarker, bool testing = false);
    bool isEmpty() const;
};

BOBUI_END_NAMESPACE

#endif // BOBUIOOLBARAREALAYOUT_P_H
