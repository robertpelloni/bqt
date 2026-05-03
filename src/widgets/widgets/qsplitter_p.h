// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#ifndef QSPLITTER_P_H
#define QSPLITTER_P_H

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
#include "private/qframe_p.h"

#include <BobUICore/qpointer.h>

BOBUI_REQUIRE_CONFIG(splitter);

BOBUI_BEGIN_NAMESPACE

static const uint Default = 2;

class QSplitterLayoutStruct
{
public:
    QRect rect;
    int sizer;
    uint collapsed : 1;
    uint collapsible : 2;
    QWidget *widget;
    QSplitterHandle *handle;

    QSplitterLayoutStruct() : sizer(-1), collapsed(false), collapsible(Default), widget(nullptr), handle(nullptr) {}
    ~QSplitterLayoutStruct() { delete handle; }
    int getWidgetSize(BobUI::Orientation orient);
    int getHandleSize(BobUI::Orientation orient);
    int pick(const QSize &size, BobUI::Orientation orient)
    { return (orient == BobUI::Horizontal) ? size.width() : size.height(); }
};

class QSplitterPrivate : public QFramePrivate
{
    Q_DECLARE_PUBLIC(QSplitter)
public:
    QSplitterPrivate() :
#if BOBUI_CONFIG(rubberband)
                         rubberBand(nullptr),
#endif
                         opaque(true), firstShow(true),
                         childrenCollapsible(true), compatMode(false), handleWidth(-1), blockChildAdd(false), opaqueResizeSet(false) {}
    ~QSplitterPrivate();

#if BOBUI_CONFIG(rubberband)
    QPointer<QRubberBand> rubberBand;
#endif
    mutable QList<QSplitterLayoutStruct *> list;
    BobUI::Orientation orient;
    bool opaque : 8;
    bool firstShow : 8;
    bool childrenCollapsible : 8;
    bool compatMode : 8;
    int handleWidth;
    bool blockChildAdd;
    bool opaqueResizeSet;

    inline int pick(const QPoint &pos) const
    { return orient == BobUI::Horizontal ? pos.x() : pos.y(); }
    inline int pick(const QSize &s) const
    { return orient == BobUI::Horizontal ? s.width() : s.height(); }

    inline int trans(const QPoint &pos) const
    { return orient == BobUI::Vertical ? pos.x() : pos.y(); }
    inline int trans(const QSize &s) const
    { return orient == BobUI::Vertical ? s.width() : s.height(); }

    void init();
    void recalc(bool update = false);
    void doResize();
    void storeSizes();
    void getRange(int index, int *, int *, int *, int *) const;
    void addContribution(int, int *, int *, bool) const;
    int adjustPos(int, int, int *, int *, int *, int *) const;
    bool collapsible(QSplitterLayoutStruct *) const;
    bool collapsible(int index) const
    { return (index < 0 || index >= list.size()) ? true : collapsible(list.at(index)); }
    QSplitterLayoutStruct *findWidget(QWidget *) const;
    void insertWidget_helper(int index, QWidget *widget, bool show);
    QSplitterLayoutStruct *insertWidget(int index, QWidget *);
    void doMove(bool backwards, int pos, int index, int delta,
                bool mayCollapse, int *positions, int *widths);
    void setGeo(QSplitterLayoutStruct *s, int pos, int size, bool allowCollapse);
    int findWidgetJustBeforeOrJustAfter(int index, int delta, int &collapsibleSize) const;
    void updateHandles();
    void setSizes_helper(const QList<int> &sizes, bool clampNegativeSize = false);
    bool shouldShowWidget(const QWidget *w) const;

};

class QSplitterHandlePrivate : public QWidgetPrivate
{
    Q_DECLARE_PUBLIC(QSplitterHandle)
public:
    QSplitterHandlePrivate() : s(nullptr), orient(BobUI::Horizontal), mouseOffset(0), opaq(false), hover(false), pressed(false) {}

    inline int pick(const QPoint &pos) const
    { return orient == BobUI::Horizontal ? pos.x() : pos.y(); }

    QSplitter *s;
    BobUI::Orientation orient;
    int mouseOffset;
    bool opaq    : 1;
    bool hover   : 1;
    bool pressed : 1;
};

BOBUI_END_NAMESPACE

#endif
