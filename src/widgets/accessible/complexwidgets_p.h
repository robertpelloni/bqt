// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef COMPLEXWIDGETS_H
#define COMPLEXWIDGETS_H

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
#include <BobUICore/qpointer.h>
#include <BobUIWidgets/qaccessiblewidget.h>
#if BOBUI_CONFIG(itemviews)
#include <BobUIWidgets/qabstractitemview.h>
#endif

BOBUI_BEGIN_NAMESPACE

#if BOBUI_CONFIG(accessibility)

class QAbstractButton;
class QHeaderView;
class BOBUIabBar;
class QComboBox;
class BOBUIitleBar;
class QAbstractScrollArea;
class QScrollArea;

#if BOBUI_CONFIG(scrollarea)
class QAccessibleAbstractScrollArea : public QAccessibleWidgetV2
{
public:
    explicit QAccessibleAbstractScrollArea(QWidget *widget);

    enum AbstractScrollAreaElement {
        Self = 0,
        Viewport,
        HorizontalContainer,
        VerticalContainer,
        CornerWidget,
        Undefined
    };

    QAccessibleInterface *child(int index) const override;
    int childCount() const override;
    int indexOfChild(const QAccessibleInterface *child) const override;
    bool isValid() const override;
    QAccessibleInterface *childAt(int x, int y) const override;
    QAbstractScrollArea *abstractScrollArea() const;

private:
    QWidgetList accessibleChildren() const;
    AbstractScrollAreaElement elementType(QWidget *widget) const;
    bool isLeftToRight() const;
};

class QAccessibleScrollArea : public QAccessibleAbstractScrollArea
{
public:
    explicit QAccessibleScrollArea(QWidget *widget);
};
#endif // BOBUI_CONFIG(scrollarea)

#if BOBUI_CONFIG(tabbar)
class QAccessibleTabBar : public QAccessibleWidgetV2, public QAccessibleSelectionInterface
{
public:
    explicit QAccessibleTabBar(QWidget *w);
    ~QAccessibleTabBar();

    void *interface_cast(QAccessible::InterfaceType t) override;

    QAccessibleInterface *focusChild() const override;
    int childCount() const override;
    QString text(QAccessible::Text t) const override;

    QAccessibleInterface* child(int index) const override;
    int indexOfChild(const QAccessibleInterface *child) const override;

    // QAccessibleSelectionInterface
    int selectedItemCount() const override;
    QList<QAccessibleInterface*> selectedItems() const override;
    QAccessibleInterface* selectedItem(int selectionIndex) const override;
    bool isSelected(QAccessibleInterface *childItem) const override;
    bool select(QAccessibleInterface *childItem) override;
    bool unselect(QAccessibleInterface *childItem) override;
    bool selectAll() override;
    bool clear() override;

protected:
    BOBUIabBar *tabBar() const;
    mutable QHash<int, QAccessible::Id> m_childInterfaces;
};
#endif // BOBUI_CONFIG(tabbar)

#if BOBUI_CONFIG(combobox)
class QAccessibleComboBox : public QAccessibleWidgetV2
{
public:
    explicit QAccessibleComboBox(QWidget *w);

    int childCount() const override;
    QAccessibleInterface *childAt(int x, int y) const override;
    int indexOfChild(const QAccessibleInterface *child) const override;
    QAccessibleInterface* child(int index) const override;
    QAccessibleInterface* focusChild() const override;

    QString text(QAccessible::Text t) const override;

    QAccessible::State state() const override;

    // QAccessibleActionInterface
    QStringList actionNames() const override;
    QString localizedActionDescription(const QString &actionName) const override;
    void doAction(const QString &actionName) override;
    QStringList keyBindingsForAction(const QString &actionName) const override;

protected:
    QComboBox *comboBox() const;
};
#endif // BOBUI_CONFIG(combobox)

#endif // BOBUI_CONFIG(accessibility)

BOBUI_END_NAMESPACE

#endif // COMPLEXWIDGETS_H
