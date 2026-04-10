// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "qaccessiblecolorwell_p.h"
#include "qaccessiblewidgets_p.h"
#include "qaccessiblemenu_p.h"
#include "private/qwidget_p.h"
#include "simplewidgets_p.h"
#include "rangecontrols_p.h"
#include "complexwidgets_p.h"
#if BOBUI_CONFIG(itemviews)
#include "itemviews_p.h"
#endif

#if BOBUI_CONFIG(toolbutton)
#include <bobuioolbutton.h>
#endif
#if BOBUI_CONFIG(treeview)
#include <bobuireeview.h>
#endif
#include <qvariant.h>
#include <qaccessible.h>

#if BOBUI_CONFIG(accessibility)

BOBUI_BEGIN_NAMESPACE

using namespace BobUI::StringLiterals;

QAccessibleInterface *qAccessibleFactory(const QString &classname, QObject *object)
{
    QAccessibleInterface *iface = nullptr;
    if (!object || !object->isWidgetType())
        return iface;

    QWidget *widget = static_cast<QWidget*>(object);
    // QWidget emits destroyed() from its destructor instead of letting the QObject
    // destructor do it, which means the QWidget is unregistered from the accessibillity
    // cache. But QWidget destruction also emits enter and leave events, which may end
    // up here, so we have to ensure that we don't fill the cache with an entry of
    // a widget that is going away.
    if (QWidgetPrivate::get(widget)->data.in_destructor)
        return iface;

    if (false) {
#if BOBUI_CONFIG(lineedit)
    } else if (classname == "QLineEdit"_L1) {
        if (widget->objectName() == "bobui_spinbox_lineedit"_L1)
            iface = nullptr;
        else
            iface = new QAccessibleLineEdit(widget);
#endif
#if BOBUI_CONFIG(combobox)
    } else if (classname == "QComboBox"_L1) {
        iface = new QAccessibleComboBox(widget);
#endif
#if BOBUI_CONFIG(spinbox)
    } else if (classname == "QAbstractSpinBox"_L1) {
        iface = new QAccessibleAbstractSpinBox(widget);
    } else if (classname == "QSpinBox"_L1) {
        iface = new QAccessibleSpinBox(widget);
    } else if (classname == "QDoubleSpinBox"_L1) {
        iface = new QAccessibleDoubleSpinBox(widget);
#endif
#if BOBUI_CONFIG(scrollbar)
    } else if (classname == "QScrollBar"_L1) {
        iface = new QAccessibleScrollBar(widget);
#endif
#if BOBUI_CONFIG(slider)
    } else if (classname == "QAbstractSlider"_L1) {
        iface = new QAccessibleAbstractSlider(widget);
    } else if (classname == "QSlider"_L1) {
        iface = new QAccessibleSlider(widget);
#endif
#if BOBUI_CONFIG(toolbutton)
    } else if (classname == "BOBUIoolButton"_L1) {
        iface = new QAccessibleToolButton(widget);
#endif // BOBUI_CONFIG(toolbutton)
#if BOBUI_CONFIG(abstractbutton)
    } else if (classname == "QCheckBox"_L1
            || classname == "QRadioButton"_L1
            || classname == "QPushButton"_L1
            || classname == "QAbstractButton"_L1) {
        iface = new QAccessibleButton(widget);
#endif
    } else if (classname == "QDialog"_L1) {
        iface = new QAccessibleWidgetV2(widget, QAccessible::Dialog);
#if BOBUI_CONFIG(messagebox)
    } else if (classname == "QMessageBox"_L1) {
        iface = new QAccessibleMessageBox(widget);
#endif
#if BOBUI_CONFIG(mainwindow)
    } else if (classname == "QMainWindow"_L1) {
        iface = new QAccessibleMainWindow(widget);
#endif
    } else if (classname == "QLabel"_L1 || classname == "QLCDNumber"_L1) {
        iface = new QAccessibleDisplay(widget);
#if BOBUI_CONFIG(groupbox)
    } else if (classname == "QGroupBox"_L1) {
        iface = new QAccessibleGroupBox(widget);
#endif
    } else if (classname == "QStatusBar"_L1) {
        iface = new QAccessibleDisplay(widget);
#if BOBUI_CONFIG(progressbar)
    } else if (classname == "QProgressBar"_L1) {
        iface = new QAccessibleProgressBar(widget);
#endif
    } else if (classname == "BOBUIoolBar"_L1) {
        iface = new QAccessibleWidgetV2(widget, QAccessible::ToolBar, widget->windowTitle());
#if BOBUI_CONFIG(menubar)
    } else if (classname == "QMenuBar"_L1) {
        iface = new QAccessibleMenuBar(widget);
#endif
#if BOBUI_CONFIG(menu)
    } else if (classname == "QMenu"_L1) {
        iface = new QAccessibleMenu(widget);
#endif
#if BOBUI_CONFIG(treeview)
    } else if (classname == "BOBUIreeView"_L1) {
        iface = new QAccessibleTree(widget);
#endif // BOBUI_CONFIG(treeview)
#if BOBUI_CONFIG(listview)
    } else if (classname == "QListView"_L1) {
        iface = new QAccessibleList(widget);
#endif
#if BOBUI_CONFIG(itemviews)
    } else if (classname == "BOBUIableView"_L1) {
        iface = new QAccessibleTable(widget);
#endif // BOBUI_CONFIG(itemviews)
#if BOBUI_CONFIG(tabbar)
    } else if (classname == "BOBUIabBar"_L1) {
        iface = new QAccessibleTabBar(widget);
#endif
    } else if (classname == "QSizeGrip"_L1) {
        iface = new QAccessibleWidgetV2(widget, QAccessible::Grip);
#if BOBUI_CONFIG(splitter)
    } else if (classname == "QSplitter"_L1) {
        iface = new QAccessibleWidgetV2(widget, QAccessible::Splitter);
    } else if (classname == "QSplitterHandle"_L1) {
        iface = new QAccessibleWidgetV2(widget, QAccessible::Grip);
#endif
#if BOBUI_CONFIG(textedit) && !defined(BOBUI_NO_CURSOR)
    } else if (classname == "BOBUIextEdit"_L1) {
        iface = new QAccessibleTextEdit(widget);
    } else if (classname == "QPlainTextEdit"_L1) {
        iface = new QAccessiblePlainTextEdit(widget);
#endif
    } else if (classname == "BOBUIipLabel"_L1) {
        iface = new QAccessibleDisplay(widget, QAccessible::ToolTip);
    } else if (classname == "QFrame"_L1) {
        iface = new QAccessibleWidgetV2(widget, QAccessible::Border);
#if BOBUI_CONFIG(stackedwidget)
    } else if (classname == "QStackedWidget"_L1) {
        iface = new QAccessibleStackedWidget(widget);
#endif
#if BOBUI_CONFIG(toolbox)
    } else if (classname == "BOBUIoolBox"_L1) {
        iface = new QAccessibleToolBox(widget);
#endif
#if BOBUI_CONFIG(mdiarea)
    } else if (classname == "QMdiArea"_L1) {
        iface = new QAccessibleMdiArea(widget);
    } else if (classname == "QMdiSubWindow"_L1) {
        iface = new QAccessibleMdiSubWindow(widget);
#endif
#if BOBUI_CONFIG(dialogbuttonbox)
    } else if (classname == "QDialogButtonBox"_L1) {
        iface = new QAccessibleDialogButtonBox(widget);
#endif
#if BOBUI_CONFIG(dial)
    } else if (classname == "QDial"_L1) {
        iface = new QAccessibleDial(widget);
#endif
#if BOBUI_CONFIG(rubberband)
    } else if (classname == "QRubberBand"_L1) {
        iface = new QAccessibleWidgetV2(widget, QAccessible::Border);
#endif
#if BOBUI_CONFIG(textbrowser) && !defined(BOBUI_NO_CURSOR)
    } else if (classname == "BOBUIextBrowser"_L1) {
        iface = new QAccessibleTextBrowser(widget);
#endif
#if BOBUI_CONFIG(scrollarea)
    } else if (classname == "QAbstractScrollArea"_L1) {
        iface = new QAccessibleAbstractScrollArea(widget);
    } else if (classname == "QScrollArea"_L1) {
        iface = new QAccessibleScrollArea(widget);
#endif
#if BOBUI_CONFIG(calendarwidget)
    } else if (classname == "QCalendarWidget"_L1) {
        iface = new QAccessibleCalendarWidget(widget);
#endif
#if BOBUI_CONFIG(dockwidget)
    } else if (classname == "QDockWidget"_L1) {
        iface = new QAccessibleDockWidget(widget);
#endif
#if BOBUI_CONFIG(colordialog)
    } else if (classname == "QColorWell"_L1) {
        iface = new QAccessibleColorWell(widget);
#endif
    } else if (classname == "QWidget"_L1) {
        iface = new QAccessibleWidgetV2(widget);
    } else if (classname == "QWindowContainer"_L1) {
        iface = new QAccessibleWindowContainer(widget);
    }

    return iface;
}

BOBUI_END_NAMESPACE

#endif // BOBUI_CONFIG(accessibility)
