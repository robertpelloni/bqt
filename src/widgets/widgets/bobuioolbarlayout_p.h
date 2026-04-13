// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#ifndef BOBUIOOLBARLAYOUT_P_H
#define BOBUIOOLBARLAYOUT_P_H

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
#include <BobUIWidgets/qlayout.h>
#include <private/qlayoutengine_p.h>
#include <QList>

BOBUI_REQUIRE_CONFIG(toolbar);

BOBUI_BEGIN_NAMESPACE

class QAction;
class BOBUIoolBarExtension;
class QMenu;

class BOBUIoolBarItem : public QWidgetItem
{
public:
    BOBUIoolBarItem(QWidget *widget);
    bool isEmpty() const override;

    QAction *action;
    bool customWidget;
};

class Q_AUTOTEST_EXPORT BOBUIoolBarLayout : public QLayout
{
    Q_OBJECT

public:
    BOBUIoolBarLayout(QWidget *parent = nullptr);
    ~BOBUIoolBarLayout();

    void addItem(QLayoutItem *item) override;
    QLayoutItem *itemAt(int index) const override;
    QLayoutItem *takeAt(int index) override;
    int count() const override;

    bool isEmpty() const override;
    void invalidate() override;
    BobUI::Orientations expandingDirections() const override;

    void setGeometry(const QRect &r) override;
    QSize minimumSize() const override;
    QSize sizeHint() const override;

    void insertAction(int index, QAction *action);
    int indexOf(const QAction *action) const;
    using QLayout::indexOf; // bring back the hidden members

    bool layoutActions(const QSize &size);
    QSize expandedSize(const QSize &size) const;
    bool expanded, animating;

    void setUsePopupMenu(bool set); // Yeah, there's no getter, but it's internal.
    void checkUsePopupMenu();

    bool movable() const;
    void updateMarginAndSpacing();
    bool hasExpandFlag() const;

    void updateMacBorderMetrics();
public Q_SLOTS:
    void setExpanded(bool b);

private:
    QList<BOBUIoolBarItem*> items;
    QSize hint, minSize;
    bool dirty, expanding, empty, expandFlag;
    QList<QLayoutStruct> geomArray;
    QRect handRect;
    BOBUIoolBarExtension *extension;

    void updateGeomArray() const;
    BOBUIoolBarItem *createItem(QAction *action);
    QMenu *popupMenu;
};

BOBUI_END_NAMESPACE

#endif // BOBUIOOLBARLAYOUT_P_H
