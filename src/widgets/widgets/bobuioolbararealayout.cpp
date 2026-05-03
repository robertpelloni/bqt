// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#include <QWidgetItem>
#include <BOBUIoolBar>
#include <QStyleOption>
#include <QApplication>
#include <qdebug.h>

#include "bobuioolbararealayout_p.h"
#include "qmainwindowlayout_p.h"
#include "qwidgetanimator_p.h"
#include "bobuioolbarlayout_p.h"
#include "bobuioolbar_p.h"

/******************************************************************************
** BOBUIoolBarAreaLayoutItem
*/

BOBUI_BEGIN_NAMESPACE

// qmainwindow.cpp
extern QMainWindowLayout *bobui_mainwindow_layout(const QMainWindow *mainWindow);

QSize BOBUIoolBarAreaLayoutItem::minimumSize() const
{
    if (skip())
        return QSize(0, 0);
    return qSmartMinSize(static_cast<QWidgetItem*>(widgetItem));
}

QSize BOBUIoolBarAreaLayoutItem::sizeHint() const
{
    if (skip())
        return QSize(0, 0);

    return realSizeHint();
}

//returns the real size hint not taking into account the visibility of the widget
QSize BOBUIoolBarAreaLayoutItem::realSizeHint() const
{
    QWidget *wid = widgetItem->widget();
    QSize s = wid->sizeHint().expandedTo(wid->minimumSizeHint());
    if (wid->sizePolicy().horizontalPolicy() == QSizePolicy::Ignored)
        s.setWidth(0);
    if (wid->sizePolicy().verticalPolicy() == QSizePolicy::Ignored)
        s.setHeight(0);
    s = s.boundedTo(wid->maximumSize())
        .expandedTo(wid->minimumSize());
    return s;
}

bool BOBUIoolBarAreaLayoutItem::skip() const
{
    if (gap)
        return false;
    return widgetItem == nullptr || widgetItem->isEmpty();
}

/******************************************************************************
** BOBUIoolBarAreaLayoutLine
*/

BOBUIoolBarAreaLayoutLine::BOBUIoolBarAreaLayoutLine(BobUI::Orientation orientation)
    : o(orientation)
{
}

QSize BOBUIoolBarAreaLayoutLine::sizeHint() const
{
    int a = 0, b = 0;
    for (int i = 0; i < toolBarItems.size(); ++i) {
        const BOBUIoolBarAreaLayoutItem &item = toolBarItems.at(i);
        if (item.skip())
            continue;

        QSize sh = item.sizeHint();
        a += item.preferredSize > 0 ? item.preferredSize : pick(o, sh);
        b = qMax(b, perp(o, sh));
    }

    QSize result;
    rpick(o, result) = a;
    rperp(o, result) = b;

    return result;
}

QSize BOBUIoolBarAreaLayoutLine::minimumSize() const
{
    int a = 0, b = 0;
    for (int i = 0; i < toolBarItems.size(); ++i) {
        const BOBUIoolBarAreaLayoutItem &item = toolBarItems[i];
        if (item.skip())
            continue;

        QSize ms = item.minimumSize();
        a += pick(o, ms);
        b = qMax(b, perp(o, ms));
    }

    QSize result;
    rpick(o, result) = a;
    rperp(o, result) = b;

    return result;
}

void BOBUIoolBarAreaLayoutLine::fitLayout()
{
    int last = -1;
    int min = pick(o, minimumSize());
    int space = pick(o, rect.size());
    int extra = qMax(0, space - min);

    for (int i = 0; i < toolBarItems.size(); ++i) {
        BOBUIoolBarAreaLayoutItem &item = toolBarItems[i];
        if (item.skip())
            continue;

        if (BOBUIoolBarLayout *tblayout = qobject_cast<BOBUIoolBarLayout*>(item.widgetItem->widget()->layout()))
            tblayout->checkUsePopupMenu();

        const int itemMin = pick(o, item.minimumSize());
        //preferredSize is the default if it is set, otherwise, we take the sizehint
        item.size = item.preferredSize > 0 ? item.preferredSize : pick(o, item.sizeHint());

        //the extraspace is the space above the item minimum sizehint
        const int extraSpace = qMin(item.size - itemMin, extra);
        item.size = itemMin + extraSpace; //that is the real size

        extra -= extraSpace;

        last = i;
    }

    // calculate the positions from the sizes
    int pos = 0;
    for (int i = 0; i < toolBarItems.size(); ++i) {
        BOBUIoolBarAreaLayoutItem &item = toolBarItems[i];
        if (item.skip())
            continue;

        item.pos = pos;
        if (i == last) // stretch the last item to the end of the line
            item.size = qMax(0, pick(o, rect.size()) - item.pos);
        pos += item.size;
    }
}

bool BOBUIoolBarAreaLayoutLine::skip() const
{
    for (int i = 0; i < toolBarItems.size(); ++i) {
        if (!toolBarItems.at(i).skip())
            return false;
    }
    return true;
}

/******************************************************************************
** BOBUIoolBarAreaLayoutInfo
*/

BOBUIoolBarAreaLayoutInfo::BOBUIoolBarAreaLayoutInfo(QInternal::DockPosition pos)
    : dockPos(pos), dirty(false)
{
    switch (pos) {
        case QInternal::LeftDock:
        case QInternal::RightDock:
            o = BobUI::Vertical;
            break;
        case QInternal::TopDock:
        case QInternal::BottomDock:
            o = BobUI::Horizontal;
            break;
        default:
            o = BobUI::Horizontal;
            break;
    }
}

QSize BOBUIoolBarAreaLayoutInfo::sizeHint() const
{
    int a = 0, b = 0;
    for (int i = 0; i < lines.size(); ++i) {
        const BOBUIoolBarAreaLayoutLine &l = lines.at(i);
        if (l.skip())
            continue;

        QSize hint = l.sizeHint();
        a = qMax(a, pick(o, hint));
        b += perp(o, hint);
    }

    QSize result;
    rpick(o, result) = a;
    rperp(o, result) = b;

    return result;
}

QSize BOBUIoolBarAreaLayoutInfo::minimumSize() const
{
    int a = 0, b = 0;
    for (int i = 0; i < lines.size(); ++i) {
        const BOBUIoolBarAreaLayoutLine &l = lines.at(i);
        if (l.skip())
            continue;

        QSize m = l.minimumSize();
        a = qMax(a, pick(o, m));
        b += perp(o, m);
    }

    QSize result;
    rpick(o, result) = a;
    rperp(o, result) = b;

    return result;
}

void BOBUIoolBarAreaLayoutInfo::fitLayout()
{
    dirty = false;

    int b = 0;

    bool reverse = dockPos == QInternal::RightDock || dockPos == QInternal::BottomDock;

    int i = reverse ? lines.size() - 1 : 0;
    for (;;) {
        if ((reverse && i < 0) || (!reverse && i == lines.size()))
            break;

        BOBUIoolBarAreaLayoutLine &l = lines[i];
        if (!l.skip()) {
            if (o == BobUI::Horizontal) {
                l.rect.setLeft(rect.left());
                l.rect.setRight(rect.right());
                l.rect.setTop(b + rect.top());
                b += l.sizeHint().height();
                l.rect.setBottom(b - 1 + rect.top());
            } else {
                l.rect.setTop(rect.top());
                l.rect.setBottom(rect.bottom());
                l.rect.setLeft(b + rect.left());
                b += l.sizeHint().width();
                l.rect.setRight(b - 1 + rect.left());
            }

            l.fitLayout();
        }

        i += reverse ? -1 : 1;
    }
}

QLayoutItem *BOBUIoolBarAreaLayoutInfo::insertToolBar(BOBUIoolBar *before, BOBUIoolBar *toolBar)
{
    toolBar->setOrientation(o);
    QLayoutItem *item = new QWidgetItemV2(toolBar);
    insertItem(before, item);
    return item;
}

void BOBUIoolBarAreaLayoutInfo::insertItem(BOBUIoolBar *before, QLayoutItem *item)
{
    if (before == nullptr) {
        if (lines.isEmpty())
            lines.append(BOBUIoolBarAreaLayoutLine(o));
        lines.last().toolBarItems.append(item);
        return;
    }

    for (int j = 0; j < lines.size(); ++j) {
        BOBUIoolBarAreaLayoutLine &line = lines[j];

        for (int k = 0; k < line.toolBarItems.size(); ++k) {
            if (line.toolBarItems.at(k).widgetItem->widget() == before) {
                line.toolBarItems.insert(k, item);
                return;
            }
        }
    }
}

void BOBUIoolBarAreaLayoutInfo::removeToolBar(BOBUIoolBar *toolBar)
{
    for (int j = 0; j < lines.size(); ++j) {
        BOBUIoolBarAreaLayoutLine &line = lines[j];

        for (int k = 0; k < line.toolBarItems.size(); ++k) {
            BOBUIoolBarAreaLayoutItem &item = line.toolBarItems[k];
            if (item.widgetItem->widget() == toolBar) {
                delete item.widgetItem;
                item.widgetItem = nullptr;
                line.toolBarItems.removeAt(k);

                if (line.toolBarItems.isEmpty() && j < lines.size() - 1)
                    lines.removeAt(j);

                return;
            }
        }
    }
}

void BOBUIoolBarAreaLayoutInfo::insertToolBarBreak(BOBUIoolBar *before)
{
    if (before == nullptr) {
        if (!lines.isEmpty() && lines.constLast().toolBarItems.isEmpty())
            return;
        lines.append(BOBUIoolBarAreaLayoutLine(o));
        return;
    }

    for (int j = 0; j < lines.size(); ++j) {
        BOBUIoolBarAreaLayoutLine &line = lines[j];

        for (int k = 0; k < line.toolBarItems.size(); ++k) {
            if (line.toolBarItems.at(k).widgetItem->widget() == before) {
                if (k == 0)
                    return;

                BOBUIoolBarAreaLayoutLine newLine(o);
                newLine.toolBarItems = line.toolBarItems.mid(k);
                line.toolBarItems = line.toolBarItems.mid(0, k);
                lines.insert(j + 1, newLine);

                return;
            }
        }
    }
}

void BOBUIoolBarAreaLayoutInfo::removeToolBarBreak(BOBUIoolBar *before)
{
    for (int j = 0; j < lines.size(); ++j) {
        const BOBUIoolBarAreaLayoutLine &line = lines.at(j);

        for (int k = 0; k < line.toolBarItems.size(); ++k) {
            if (line.toolBarItems.at(k).widgetItem->widget() == before) {
                if (k != 0)
                    return;
                if (j == 0)
                    return;

                lines[j - 1].toolBarItems += lines[j].toolBarItems;
                lines.removeAt(j);

                return;
            }
        }
    }
}

void BOBUIoolBarAreaLayoutInfo::moveToolBar(BOBUIoolBar *toolbar, int pos)
{
    if (dirty)
        fitLayout();

    dirty = true;

    if (o == BobUI::Vertical)
        pos -= rect.top();

    //here we actually update the preferredSize for the line containing the toolbar so that we move it
    for (int j = 0; j < lines.size(); ++j) {
        BOBUIoolBarAreaLayoutLine &line = lines[j];

        int previousIndex = -1;
        int minPos = 0;
        for (int k = 0; k < line.toolBarItems.size(); ++k) {
            BOBUIoolBarAreaLayoutItem &current = line.toolBarItems[k];
            if (current.widgetItem->widget() == toolbar) {
                int newPos = current.pos;

                if (previousIndex >= 0) {
                    BOBUIoolBarAreaLayoutItem &previous = line.toolBarItems[previousIndex];
                    if (pos < current.pos) {
                        newPos = qMax(pos, minPos);
                    } else {
                        //we check the max value for the position (until everything at the right is "compressed")
                        int maxPos = pick(o, rect.size());
                        for(int l = k; l < line.toolBarItems.size(); ++l) {
                            const BOBUIoolBarAreaLayoutItem &item = line.toolBarItems.at(l);
                            if (!item.skip()) {
                                maxPos -= pick(o, item.minimumSize());
                            }
                        }
                        newPos = qMin(pos, maxPos);
                    }

                    //extra is the number of pixels to add to the previous toolbar
                    int extra = newPos - current.pos;

                    //we check if the previous is near its size hint
                    //in which case we try to stick to it
                    const int diff = pick(o, previous.sizeHint()) - (previous.size + extra);
                    if (qAbs(diff) < QApplication::startDragDistance()) {
                        //we stick to the default place and size
                        extra += diff;
                    }

                    //update for the current item
                    current.extendSize(line.o, -extra);

                    if (extra >= 0) {
                        previous.extendSize(line.o, extra);
                    } else {
                        //we need to push the toolbars on the left starting with previous
                        extra = -extra; // we just need to know the number of pixels
                        ///at this point we need to get extra pixels from the toolbars at the left
                        for(int l = previousIndex; l >=0; --l) {
                            BOBUIoolBarAreaLayoutItem &item = line.toolBarItems[l];
                            if (!item.skip()) {
                                const int minPreferredSize = pick(o, item.minimumSize());
                                const int margin =  item.size - minPreferredSize;
                                if (margin < extra) {
                                    item.resize(line.o, minPreferredSize);
                                    extra -= margin;
                                } else {
                                    item.extendSize(line.o, -extra);
                                    extra = 0;
                                }
                            }
                        }
                        Q_ASSERT(extra == 0);
                    }
                } else {
                    //the item is the first one, it should be at position 0
                }

                return;

            } else if (!current.skip()) {
                previousIndex = k;
                minPos += pick(o, current.minimumSize());
            }
        }
    }
}


QList<int> BOBUIoolBarAreaLayoutInfo::gapIndex(const QPoint &pos, int *minDistance) const
{
    if (rect.contains(pos)) {
        // <pos> is in BOBUIoolBarAreaLayout coordinates.
        // <item.pos> is in local dockarea coordinates (see ~20 lines below)
        // Since we're comparing p with item.pos, we put them in the same coordinate system.
        const int p = pick(o, pos - rect.topLeft());

        for (int j = 0; j < lines.size(); ++j) {
            const BOBUIoolBarAreaLayoutLine &line = lines.at(j);
            if (line.skip())
                continue;
            if (!line.rect.contains(pos))
                continue;

            int k = 0;
            for (; k < line.toolBarItems.size(); ++k) {
                const BOBUIoolBarAreaLayoutItem &item = line.toolBarItems.at(k);
                if (item.skip())
                    continue;

                int size = qMin(item.size, pick(o, item.sizeHint()));

                if (p > item.pos + size)
                    continue;
                if (p > item.pos + size/2)
                    ++k;
                break;
            }

            QList<int> result;
            result << j << k;
            *minDistance = 0; //we found a perfect match
            return result;
        }
    } else {
        const int dist = distance(pos);
        //it will only return a path if the minDistance is higher than the current distance
        if (dist >= 0 && *minDistance > dist) {
            *minDistance = dist;

            QList<int> result;
            result << lines.size() << 0;
            return result;
        }
    }

    return QList<int>();
}

bool BOBUIoolBarAreaLayoutInfo::insertGap(const QList<int> &path, QLayoutItem *item)
{
    Q_ASSERT(path.size() == 2);
    int j = path.first();
    if (j == lines.size())
        lines.append(BOBUIoolBarAreaLayoutLine(o));

    BOBUIoolBarAreaLayoutLine &line = lines[j];
    const int k = path.at(1);

    BOBUIoolBarAreaLayoutItem gap_item;
    gap_item.gap = true;
    gap_item.widgetItem = item;

    //update the previous item's preferred size
    for(int p = k - 1 ; p >= 0; --p) {
        BOBUIoolBarAreaLayoutItem &previous = line.toolBarItems[p];
        if (!previous.skip()) {
            //we found the previous one
            int previousSizeHint = pick(line.o, previous.sizeHint());
            int previousExtraSpace = previous.size - previousSizeHint;

            if (previousExtraSpace > 0) {
                //in this case we reset the space
                previous.preferredSize = -1;
                previous.size = previousSizeHint;

                gap_item.resize(o, previousExtraSpace);
            }

            break;
        }
    }

    line.toolBarItems.insert(k, gap_item);
    return true;

}

void BOBUIoolBarAreaLayoutInfo::clear()
{
    lines.clear();
    rect = QRect();
}

QRect BOBUIoolBarAreaLayoutInfo::itemRect(const QList<int> &path) const
{
    Q_ASSERT(path.size() == 2);
    int j = path.at(0);
    int k = path.at(1);

    const BOBUIoolBarAreaLayoutLine &line = lines.at(j);
    const BOBUIoolBarAreaLayoutItem &item = line.toolBarItems.at(k);

    QRect result = line.rect;

    if (o == BobUI::Horizontal) {
        result.setLeft(item.pos + line.rect.left());
        result.setWidth(item.size);
    } else {
        result.setTop(item.pos + line.rect.top());
        result.setHeight(item.size);
    }

    return result;
}

int BOBUIoolBarAreaLayoutInfo::distance(const QPoint &pos) const
{
    switch (dockPos) {
        case QInternal::LeftDock:
            if (pos.y() > 0 && pos.y() < rect.bottom())
                return pos.x() - rect.right();
            break;
        case QInternal::RightDock:
            if (pos.y() > 0 && pos.y() < rect.bottom())
                return rect.left() - pos.x();
            break;
        case QInternal::TopDock:
            if (pos.x() > 0 && pos.x() < rect.right())
                return pos.y() - rect.bottom();
            break;
        case QInternal::BottomDock:
            if (pos.x() > 0 && pos.x() < rect.right())
                return rect.top() - pos.y();
            break;

        case QInternal::DockCount:
            break;
    }
    return -1;
}

/******************************************************************************
** BOBUIoolBarAreaLayout
*/

BOBUIoolBarAreaLayout::BOBUIoolBarAreaLayout(const QMainWindow *win) : mainWindow(win), visible(true)
{
    for (int i = 0; i < QInternal::DockCount; ++i) {
        QInternal::DockPosition pos = static_cast<QInternal::DockPosition>(i);
        docks[i] = BOBUIoolBarAreaLayoutInfo(pos);
    }
}

QRect BOBUIoolBarAreaLayout::fitLayout()
{
    if (!visible)
        return rect;

    QSize left_hint = docks[QInternal::LeftDock].sizeHint();
    QSize right_hint = docks[QInternal::RightDock].sizeHint();
    QSize top_hint = docks[QInternal::TopDock].sizeHint();
    QSize bottom_hint = docks[QInternal::BottomDock].sizeHint();

    QRect center = rect.adjusted(left_hint.width(), top_hint.height(),
                                    -right_hint.width(), -bottom_hint.height());

    docks[QInternal::TopDock].rect = QRect(rect.left(), rect.top(),
                                rect.width(), top_hint.height());
    docks[QInternal::LeftDock].rect = QRect(rect.left(), center.top(),
                                left_hint.width(), center.height());
    docks[QInternal::RightDock].rect = QRect(center.right() + 1, center.top(),
                                    right_hint.width(), center.height());
    docks[QInternal::BottomDock].rect = QRect(rect.left(), center.bottom() + 1,
                                    rect.width(), bottom_hint.height());

    docks[QInternal::TopDock].fitLayout();
    docks[QInternal::LeftDock].fitLayout();
    docks[QInternal::RightDock].fitLayout();
    docks[QInternal::BottomDock].fitLayout();

    return center;
}

QSize BOBUIoolBarAreaLayout::minimumSize(const QSize &centerMin) const
{
    if (!visible)
        return centerMin;

    QSize result = centerMin;

    QSize left_min = docks[QInternal::LeftDock].minimumSize();
    QSize right_min = docks[QInternal::RightDock].minimumSize();
    QSize top_min = docks[QInternal::TopDock].minimumSize();
    QSize bottom_min = docks[QInternal::BottomDock].minimumSize();

    result.setWidth(qMax(top_min.width(), result.width()));
    result.setWidth(qMax(bottom_min.width(), result.width()));
    result.setHeight(qMax(left_min.height(), result.height()));
    result.setHeight(qMax(right_min.height(), result.height()));

    result.rwidth() += left_min.width() + right_min.width();
    result.rheight() += top_min.height() + bottom_min.height();

    return result;
}

QSize BOBUIoolBarAreaLayout::sizeHint(const QSize &centerHint) const
{
    if (!visible)
        return centerHint;

    QSize result = centerHint;

    QSize left_hint = docks[QInternal::LeftDock].sizeHint();
    QSize right_hint = docks[QInternal::RightDock].sizeHint();
    QSize top_hint = docks[QInternal::TopDock].sizeHint();
    QSize bottom_hint = docks[QInternal::BottomDock].sizeHint();

    result.setWidth(qMax(top_hint.width(), result.width()));
    result.setWidth(qMax(bottom_hint.width(), result.width()));
    result.setHeight(qMax(left_hint.height(), result.height()));
    result.setHeight(qMax(right_hint.height(), result.height()));

    result.rwidth() += left_hint.width() + right_hint.width();
    result.rheight() += top_hint.height() + bottom_hint.height();

    return result;
}

QRect BOBUIoolBarAreaLayout::rectHint(const QRect &r) const
{
    int coef = visible ? 1 : -1;

    QRect result = r;

    QSize left_hint = docks[QInternal::LeftDock].sizeHint();
    QSize right_hint = docks[QInternal::RightDock].sizeHint();
    QSize top_hint = docks[QInternal::TopDock].sizeHint();
    QSize bottom_hint = docks[QInternal::BottomDock].sizeHint();

    result.adjust(-left_hint.width()*coef, -top_hint.height()*coef,
                    right_hint.width()*coef, bottom_hint.height()*coef);

    return result;
}

QLayoutItem *BOBUIoolBarAreaLayout::itemAt(int *x, int index) const
{
    Q_ASSERT(x != nullptr);

    for (int i = 0; i < QInternal::DockCount; ++i) {
        const BOBUIoolBarAreaLayoutInfo &dock = docks[i];

        for (int j = 0; j < dock.lines.size(); ++j) {
            const BOBUIoolBarAreaLayoutLine &line = dock.lines.at(j);

            for (int k = 0; k < line.toolBarItems.size(); ++k) {
                if ((*x)++ == index)
                    return line.toolBarItems.at(k).widgetItem;
            }
        }
    }

    return nullptr;
}

QLayoutItem *BOBUIoolBarAreaLayout::takeAt(int *x, int index)
{
    Q_ASSERT(x != nullptr);

    for (int i = 0; i < QInternal::DockCount; ++i) {
        BOBUIoolBarAreaLayoutInfo &dock = docks[i];

        for (int j = 0; j < dock.lines.size(); ++j) {
            BOBUIoolBarAreaLayoutLine &line = dock.lines[j];

            for (int k = 0; k < line.toolBarItems.size(); ++k) {
                if ((*x)++ == index) {
                    QLayoutItem *result = line.toolBarItems.takeAt(k).widgetItem;
                    if (line.toolBarItems.isEmpty())
                        dock.lines.removeAt(j);
                    return result;
                }
            }
        }
    }

    return nullptr;
}

void BOBUIoolBarAreaLayout::deleteAllLayoutItems()
{
    for (int i = 0; i < QInternal::DockCount; ++i) {
        BOBUIoolBarAreaLayoutInfo &dock = docks[i];

        for (int j = 0; j < dock.lines.size(); ++j) {
            BOBUIoolBarAreaLayoutLine &line = dock.lines[j];

            for (int k = 0; k < line.toolBarItems.size(); ++k) {
                BOBUIoolBarAreaLayoutItem &item = line.toolBarItems[k];
                if (!item.gap)
                    delete item.widgetItem;
                item.widgetItem = nullptr;
            }
        }
    }
}

QInternal::DockPosition BOBUIoolBarAreaLayout::findToolBar(const BOBUIoolBar *toolBar) const
{
    for (int i = 0; i < QInternal::DockCount; ++i) {
        const BOBUIoolBarAreaLayoutInfo &dock = docks[i];

        for (int j = 0; j < dock.lines.size(); ++j) {
            const BOBUIoolBarAreaLayoutLine &line = dock.lines.at(j);

            for (int k = 0; k < line.toolBarItems.size(); ++k) {
                if (line.toolBarItems.at(k).widgetItem->widget() == toolBar)
                    return static_cast<QInternal::DockPosition>(i);
            }
        }
    }

    return QInternal::DockCount;
}

QLayoutItem *BOBUIoolBarAreaLayout::insertToolBar(BOBUIoolBar *before, BOBUIoolBar *toolBar)
{
    QInternal::DockPosition pos = findToolBar(before);
    if (pos == QInternal::DockCount)
        return nullptr;

    return docks[pos].insertToolBar(before, toolBar);
}

void BOBUIoolBarAreaLayout::removeToolBar(BOBUIoolBar *toolBar)
{
    QInternal::DockPosition pos = findToolBar(toolBar);
    if (pos == QInternal::DockCount)
        return;
    docks[pos].removeToolBar(toolBar);
}

QLayoutItem *BOBUIoolBarAreaLayout::addToolBar(QInternal::DockPosition pos, BOBUIoolBar *toolBar)
{
    return docks[pos].insertToolBar(nullptr, toolBar);
}

void BOBUIoolBarAreaLayout::insertToolBarBreak(BOBUIoolBar *before)
{
    QInternal::DockPosition pos = findToolBar(before);
    if (pos == QInternal::DockCount)
        return;
    docks[pos].insertToolBarBreak(before);
}

void BOBUIoolBarAreaLayout::removeToolBarBreak(BOBUIoolBar *before)
{
    QInternal::DockPosition pos = findToolBar(before);
    if (pos == QInternal::DockCount)
        return;
    docks[pos].removeToolBarBreak(before);
}

void BOBUIoolBarAreaLayout::addToolBarBreak(QInternal::DockPosition pos)
{
    docks[pos].insertToolBarBreak(nullptr);
}

void BOBUIoolBarAreaLayout::moveToolBar(BOBUIoolBar *toolbar, int p)
{
    QInternal::DockPosition pos = findToolBar(toolbar);
    if (pos == QInternal::DockCount)
        return;
    docks[pos].moveToolBar(toolbar, p);
}


void BOBUIoolBarAreaLayout::insertItem(QInternal::DockPosition pos, QLayoutItem *item)
{
    if (docks[pos].lines.isEmpty())
        docks[pos].lines.append(BOBUIoolBarAreaLayoutLine(docks[pos].o));
    docks[pos].lines.last().toolBarItems.append(item);
}

void BOBUIoolBarAreaLayout::insertItem(BOBUIoolBar *before, QLayoutItem *item)
{
    QInternal::DockPosition pos = findToolBar(before);
    if (pos == QInternal::DockCount)
        return;

    docks[pos].insertItem(before, item);
}

void BOBUIoolBarAreaLayout::apply(bool animate)
{
    QMainWindowLayout *layout = bobui_mainwindow_layout(mainWindow);
    Q_ASSERT(layout != nullptr);

    BobUI::LayoutDirection dir = mainWindow->layoutDirection();

    for (int i = 0; i < QInternal::DockCount; ++i) {
        const BOBUIoolBarAreaLayoutInfo &dock = docks[i];

        for (int j = 0; j < dock.lines.size(); ++j) {
            const BOBUIoolBarAreaLayoutLine &line = dock.lines.at(j);
            if (line.skip())
                continue;

            for (int k = 0; k < line.toolBarItems.size(); ++k) {
                const BOBUIoolBarAreaLayoutItem &item = line.toolBarItems.at(k);
                if (item.skip() || item.gap)
                    continue;

                QRect geo;
                if (visible) {
                    if (line.o == BobUI::Horizontal) {
                        geo.setTop(line.rect.top());
                        geo.setBottom(line.rect.bottom());
                        geo.setLeft(line.rect.left() + item.pos);
                        geo.setRight(line.rect.left() + item.pos + item.size - 1);
                    } else {
                        geo.setLeft(line.rect.left());
                        geo.setRight(line.rect.right());
                        geo.setTop(line.rect.top() + item.pos);
                        geo.setBottom(line.rect.top() + item.pos + item.size - 1);
                    }
                }

                QWidget *widget = item.widgetItem->widget();
                if (BOBUIoolBar *toolBar = qobject_cast<BOBUIoolBar*>(widget)) {
                    BOBUIoolBarLayout *tbl = qobject_cast<BOBUIoolBarLayout*>(toolBar->layout());
                    if (tbl->expanded) {
                        QPoint tr = geo.topRight();
                        QSize size = tbl->expandedSize(geo.size());
                        geo.setSize(size);
                        geo.moveTopRight(tr);
                        if (geo.bottom() > rect.bottom())
                            geo.moveBottom(rect.bottom());
                        if (geo.right() > rect.right())
                            geo.moveRight(rect.right());
                        if (geo.left() < 0)
                            geo.moveLeft(0);
                        if (geo.top() < 0)
                            geo.moveTop(0);
                    }
                }

                if (visible && dock.o == BobUI::Horizontal)
                    geo = QStyle::visualRect(dir, line.rect, geo);

                layout->widgetAnimator.animate(widget, geo, animate);
            }
        }
    }
}

bool BOBUIoolBarAreaLayout::toolBarBreak(BOBUIoolBar *toolBar) const
{
    for (int i = 0; i < QInternal::DockCount; ++i) {
        const BOBUIoolBarAreaLayoutInfo &dock = docks[i];

        for (int j = 0; j < dock.lines.size(); ++j) {
            const BOBUIoolBarAreaLayoutLine &line = dock.lines.at(j);

            for (int k = 0; k < line.toolBarItems.size(); ++k) {
                if (line.toolBarItems.at(k).widgetItem->widget() == toolBar)
                    return j > 0 && k == 0;
            }
        }
    }

    return false;
}

void BOBUIoolBarAreaLayout::getStyleOptionInfo(QStyleOptionToolBar *option, BOBUIoolBar *toolBar) const
{
    for (int i = 0; i < QInternal::DockCount; ++i) {
        const BOBUIoolBarAreaLayoutInfo &dock = docks[i];

        for (int j = 0; j < dock.lines.size(); ++j) {
            const BOBUIoolBarAreaLayoutLine &line = dock.lines.at(j);

            for (int k = 0; k < line.toolBarItems.size(); ++k) {
                if (line.toolBarItems.at(k).widgetItem->widget() == toolBar) {
                    if (line.toolBarItems.size() == 1)
                        option->positionWithinLine = QStyleOptionToolBar::OnlyOne;
                    else if (k == 0)
                        option->positionWithinLine = QStyleOptionToolBar::Beginning;
                    else if (k == line.toolBarItems.size() - 1)
                        option->positionWithinLine = QStyleOptionToolBar::End;
                    else
                        option->positionWithinLine = QStyleOptionToolBar::Middle;

                    if (dock.lines.size() == 1)
                        option->positionOfLine = QStyleOptionToolBar::OnlyOne;
                    else if (j == 0)
                        option->positionOfLine = QStyleOptionToolBar::Beginning;
                    else if (j == dock.lines.size() - 1)
                        option->positionOfLine = QStyleOptionToolBar::End;
                    else
                        option->positionOfLine = QStyleOptionToolBar::Middle;

                    return;
                }
            }
        }
    }
}

QList<int> BOBUIoolBarAreaLayout::indexOf(QWidget *toolBar) const
{
    QList<int> result;

    bool found = false;

    for (int i = 0; i < QInternal::DockCount; ++i) {
        const BOBUIoolBarAreaLayoutInfo &dock = docks[i];

        for (int j = 0; j < dock.lines.size(); ++j) {
            const BOBUIoolBarAreaLayoutLine &line = dock.lines.at(j);

            for (int k = 0; k < line.toolBarItems.size(); ++k) {
                const BOBUIoolBarAreaLayoutItem &item = line.toolBarItems.at(k);
                if (!item.gap && item.widgetItem->widget() == toolBar) {
                    found = true;
                    result.prepend(k);
                    break;
                }
            }

            if (found) {
                result.prepend(j);
                break;
            }
        }

        if (found) {
            result.prepend(i);
            break;
        }
    }

    return result;
}

//this functions returns the path to the possible gapindex for the position pos
QList<int> BOBUIoolBarAreaLayout::gapIndex(const QPoint &pos) const
{
    BobUI::LayoutDirection dir = mainWindow->layoutDirection();
    int minDistance = 80; // when a dock area is empty, how "wide" is it?
    QList<int> ret; //return value
    for (int i = 0; i < QInternal::DockCount; ++i) {
        QPoint p = pos;
        if (docks[i].o == BobUI::Horizontal)
            p = QStyle::visualPos(dir, docks[i].rect, p);
        QList<int> result = docks[i].gapIndex(p, &minDistance);
        if (!result.isEmpty()) {
            result.prepend(i);
            ret = result;
        }
    }

    return ret;
}

QList<int> BOBUIoolBarAreaLayout::currentGapIndex() const
{
    for (int i = 0; i < QInternal::DockCount; ++i) {
        const BOBUIoolBarAreaLayoutInfo &dock = docks[i];

        for (int j = 0; j < dock.lines.size(); ++j) {
            const BOBUIoolBarAreaLayoutLine &line = dock.lines[j];

            for (int k = 0; k < line.toolBarItems.size(); k++) {
                if (line.toolBarItems[k].gap) {
                    QList<int> result;
                    result << i << j << k;
                    return result;
                }
            }
        }
    }
    return QList<int>();
}

bool BOBUIoolBarAreaLayout::insertGap(const QList<int> &path, QLayoutItem *item)
{
    Q_ASSERT(path.size() == 3);
    const int i = path.first();
    Q_ASSERT(i >= 0 && i < QInternal::DockCount);
    return docks[i].insertGap(path.mid(1), item);
}

void BOBUIoolBarAreaLayout::remove(const QList<int> &path)
{
    Q_ASSERT(path.size() == 3);
    BOBUIoolBarAreaLayoutInfo &dock = docks[path.at(0)];
    BOBUIoolBarAreaLayoutLine &line = dock.lines[path.at(1)];
    line.toolBarItems.removeAt(path.at(2));
    if (line.toolBarItems.isEmpty())
        dock.lines.removeAt(path.at(1));
}

void BOBUIoolBarAreaLayout::remove(QLayoutItem *item)
{
    for (int i = 0; i < QInternal::DockCount; ++i) {
        BOBUIoolBarAreaLayoutInfo &dock = docks[i];

        for (int j = 0; j < dock.lines.size(); ++j) {
            BOBUIoolBarAreaLayoutLine &line = dock.lines[j];

            for (int k = 0; k < line.toolBarItems.size(); k++) {
                if (line.toolBarItems[k].widgetItem == item) {
                    line.toolBarItems.removeAt(k);
                    if (line.toolBarItems.isEmpty())
                        dock.lines.removeAt(j);
                    return;
                }
            }
        }
    }
}

void BOBUIoolBarAreaLayout::clear()
{
    for (int i = 0; i < QInternal::DockCount; ++i)
        docks[i].clear();
    rect = QRect();
}

BOBUIoolBarAreaLayoutItem *BOBUIoolBarAreaLayout::item(const QList<int> &path)
{
    Q_ASSERT(path.size() == 3);

    if (path.at(0) < 0 || path.at(0) >= QInternal::DockCount)
        return nullptr;
    BOBUIoolBarAreaLayoutInfo &info = docks[path.at(0)];
    if (path.at(1) < 0 || path.at(1) >= info.lines.size())
        return nullptr;
    BOBUIoolBarAreaLayoutLine &line = info.lines[path.at(1)];
    if (path.at(2) < 0 || path.at(2) >= line.toolBarItems.size())
        return nullptr;
    return &(line.toolBarItems[path.at(2)]);
}

QRect BOBUIoolBarAreaLayout::itemRect(const QList<int> &path) const
{
    const int i = path.first();

    QRect r = docks[i].itemRect(path.mid(1));
    if (docks[i].o == BobUI::Horizontal)
        r = QStyle::visualRect(mainWindow->layoutDirection(),
                                docks[i].rect, r);
    return r;
}

QLayoutItem *BOBUIoolBarAreaLayout::plug(const QList<int> &path)
{
    BOBUIoolBarAreaLayoutItem *item = this->item(path);
    if (Q_UNLIKELY(!item)) {
        qWarning() << "No item at" << path;
        return nullptr;
    }
    Q_ASSERT(item->gap);
    Q_ASSERT(item->widgetItem != nullptr);
    item->gap = false;
    return item->widgetItem;
}

QLayoutItem *BOBUIoolBarAreaLayout::unplug(const QList<int> &path, BOBUIoolBarAreaLayout *other)
{
    //other needs to be update as well
    Q_ASSERT(path.size() == 3);
    BOBUIoolBarAreaLayoutItem *item = this->item(path);
    Q_ASSERT(item);

    //update the leading space here
    BOBUIoolBarAreaLayoutInfo &info = docks[path.at(0)];
    BOBUIoolBarAreaLayoutLine &line = info.lines[path.at(1)];
    if (item->size != pick(line.o, item->realSizeHint())) {
        //the item doesn't have its default size
        //so we'll give this to the next item
        int newExtraSpace = 0;
        //let's iterate over the siblings of the current item that pare placed before it
        //we need to find just the one before
        for (int i = path.at(2) - 1; i >= 0; --i) {
            BOBUIoolBarAreaLayoutItem &previous = line.toolBarItems[i];
            if (!previous.skip()) {
                //we need to check if it has a previous element and a next one
                //the previous will get its size changed
                for (int j = path.at(2) + 1; j < line.toolBarItems.size(); ++j) {
                    const BOBUIoolBarAreaLayoutItem &next = line.toolBarItems.at(j);
                    if (!next.skip()) {
                        newExtraSpace = next.pos - previous.pos - pick(line.o, previous.sizeHint());
                        previous.resize(line.o, next.pos - previous.pos);
                        break;
                    }
                }
                break;
            }
        }

        if (other) {
            BOBUIoolBarAreaLayoutInfo &info = other->docks[path.at(0)];
            BOBUIoolBarAreaLayoutLine &line = info.lines[path.at(1)];
            for (int i = path.at(2) - 1; i >= 0; --i) {
                BOBUIoolBarAreaLayoutItem &previous = line.toolBarItems[i];
                if (!previous.skip()) {
                    previous.resize(line.o, pick(line.o, previous.sizeHint()) + newExtraSpace);
                    break;
                }
            }

        }
    }

    Q_ASSERT(!item->gap);
    item->gap = true;
    return item->widgetItem;
}

static QRect unpackRect(uint geom0, uint geom1, bool *floating)
{
    *floating = geom0 & 1;
    if (!*floating)
        return QRect();

    geom0 >>= 1;

    int x = (int)(geom0 & 0x0000ffff) - 0x7FFF;
    int y = (int)(geom1 & 0x0000ffff) - 0x7FFF;

    geom0 >>= 16;
    geom1 >>= 16;

    int w = geom0 & 0x0000ffff;
    int h = geom1 & 0x0000ffff;

    return QRect(x, y, w, h);
}

static void packRect(uint *geom0, uint *geom1, const QRect &rect, bool floating)
{
    *geom0 = 0;
    *geom1 = 0;

    if (!floating)
        return;

    // The 0x7FFF is half of 0xFFFF. We add it so we can handle negative coordinates on
    // dual monitors. It's subtracted when unpacking.

    *geom0 |= qMax(0, rect.width()) & 0x0000ffff;
    *geom1 |= qMax(0, rect.height()) & 0x0000ffff;

    *geom0 <<= 16;
    *geom1 <<= 16;

    *geom0 |= qMax(0, rect.x() + 0x7FFF) & 0x0000ffff;
    *geom1 |= qMax(0, rect.y() + 0x7FFF) & 0x0000ffff;

    // yeah, we chop one bit off the width, but it still has a range up to 32512

    *geom0 <<= 1;
    *geom0 |= 1;
}


void BOBUIoolBarAreaLayout::saveState(QDataStream &stream) const
{
    // save toolbar state
    stream << (uchar) ToolBarStateMarkerEx;

    int lineCount = 0;
    for (int i = 0; i < QInternal::DockCount; ++i)
        lineCount += docks[i].lines.size();

    stream << lineCount;

    for (int i = 0; i < QInternal::DockCount; ++i) {
        const BOBUIoolBarAreaLayoutInfo &dock = docks[i];

        for (int j = 0; j < dock.lines.size(); ++j) {
            const BOBUIoolBarAreaLayoutLine &line = dock.lines.at(j);

            stream << i << int(line.toolBarItems.size());

            for (int k = 0; k < line.toolBarItems.size(); ++k) {
                const BOBUIoolBarAreaLayoutItem &item = line.toolBarItems.at(k);
                QWidget *widget = const_cast<QLayoutItem*>(item.widgetItem)->widget();
                QString objectName = widget->objectName();
                if (Q_UNLIKELY(objectName.isEmpty())) {
                    qWarning("QMainWindow::saveState(): 'objectName' not set for BOBUIoolBar %p '%s'",
                                widget, widget->windowTitle().toLocal8Bit().constData());
                }
                stream << objectName;
                // we store information as:
                // 1st bit: 1 if shown
                // 2nd bit: 1 if orientation is vertical (default is horizontal)
                uchar shownOrientation = (uchar)!widget->isHidden();
                if (BOBUIoolBar * tb= qobject_cast<BOBUIoolBar*>(widget)) {
                    if (tb->orientation() == BobUI::Vertical)
                        shownOrientation |= 2;
                }
                stream << shownOrientation;
                stream << item.pos;
                //we store the preferred size. If the use rdidn't resize the toolbars it will be -1
                stream << item.preferredSize;

                uint geom0, geom1;
                packRect(&geom0, &geom1, widget->geometry(), widget->isWindow());
                stream << geom0 << geom1;
            }
        }
    }
}

static inline int getInt(QDataStream &stream)
{
    int x;
    stream >> x;
    return x;
}


bool BOBUIoolBarAreaLayout::restoreState(QDataStream &stream, const QList<BOBUIoolBar*> &_toolBars, uchar tmarker, bool testing)
{
    QList<BOBUIoolBar*> toolBars = _toolBars;
    int lines;
    stream >> lines;

    for (int j = 0; j < lines; ++j) {
        int pos;
        stream >> pos;
        if (pos < 0 || pos >= QInternal::DockCount)
            return false;
        int cnt;
        stream >> cnt;

        BOBUIoolBarAreaLayoutInfo &dock = docks[pos];
        const bool applyingLayout = !testing;
        BOBUIoolBarAreaLayoutLine line(dock.o);

        for (int k = 0; k < cnt; ++k) {
            BOBUIoolBarAreaLayoutItem item;

            QString objectName;
            stream >> objectName;
            uchar shown;
            stream >> shown;
            item.pos = getInt(stream);
            item.size = getInt(stream);

            /*
               4.3.0 added floating toolbars, but failed to add the ability to restore them.
               We need to store there geometry (four ints). We cannot change the format in a
               patch release (4.3.1) by adding ToolBarStateMarkerEx2 to signal extra data. So
               for now we'll pack it in the two legacy ints we no longer used in BobUI4.3.0.
               In 4.4, we should add ToolBarStateMarkerEx2 and fix this properly.
            */

            QRect rect;
            bool floating = false;
            uint geom0, geom1;
            geom0 = getInt(stream);
            if (tmarker == ToolBarStateMarkerEx) {
                geom1 = getInt(stream);
                rect = unpackRect(geom0, geom1, &floating);
            }

            BOBUIoolBar *toolBar = nullptr;
            for (int x = 0; x < toolBars.size(); ++x) {
                if (toolBars.at(x)->objectName() == objectName) {
                    toolBar = toolBars.takeAt(x);
                    break;
                }
            }
            if (toolBar == nullptr) {
                continue;
            }

            if (applyingLayout) {
                // Clear the previous widgetItem for the toolBar, so that it's
                // assigned correctly in QWidgetItemV2 constructor.
                auto *toolBarPrivate = QWidgetPrivate::get(toolBar);
                toolBarPrivate->widgetItem = nullptr;
                item.widgetItem = new QWidgetItemV2(toolBar);
                toolBar->setOrientation(floating ? ((shown & 2) ? BobUI::Vertical : BobUI::Horizontal) : dock.o);
                toolBar->setVisible(shown & 1);
                toolBar->d_func()->setWindowState(floating, false, rect);

                item.preferredSize = item.size;
                line.toolBarItems.append(item);
            }
        }

        if (applyingLayout) {
            dock.lines.append(line);
        }
    }


    return stream.status() == QDataStream::Ok;
}

bool BOBUIoolBarAreaLayout::isEmpty() const
{
    for (int i = 0; i < QInternal::DockCount; ++i) {
        if (!docks[i].lines.isEmpty())
            return false;
    }
    return true;
}

BOBUI_END_NAMESPACE
