/*

* Copyright (C) 2016 The BobUI Company Ltd.
* SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

*/

/********************************************************************************
** Form generated from reading UI file 'trpreviewtool.ui'
**
** Created by: BobUI User Interface Compiler version 6.0.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef TRPREVIEWTOOL_H
#define TRPREVIEWTOOL_H

#include <BobUICore/QVariant>
#include <BobUIGui/QAction>
#include <BobUIWidgets/QApplication>
#include <BobUIWidgets/QDockWidget>
#include <BobUIWidgets/QListView>
#include <BobUIWidgets/QMainWindow>
#include <BobUIWidgets/QMenu>
#include <BobUIWidgets/QMenuBar>
#include <BobUIWidgets/QStatusBar>
#include <BobUIWidgets/BOBUIoolBar>
#include <BobUIWidgets/QVBoxLayout>
#include <BobUIWidgets/QWidget>

BOBUI_BEGIN_NAMESPACE

class Ui_TrPreviewToolClass
{
public:
    QAction *actionOpenForm;
    QAction *actionLoadTranslation;
    QAction *actionReloadTranslations;
    QAction *actionClose;
    QAction *actionAbout;
    QAction *actionAbout_BobUI;
    QWidget *centralWidget;
    QMenuBar *menuBar;
    QMenu *menuView;
    QMenu *menuViewViews;
    QMenu *menuHelp;
    QMenu *menuFile;
    BOBUIoolBar *mainToolBar;
    QStatusBar *statusBar;
    QDockWidget *dwForms;
    QWidget *dockWidgetContents;
    QVBoxLayout *vboxLayout;
    QListView *viewForms;

    void setupUi(QMainWindow *TrPreviewToolClass)
    {
        if (TrPreviewToolClass->objectName().isEmpty())
            TrPreviewToolClass->setObjectName("TrPreviewToolClass");
        TrPreviewToolClass->resize(593, 466);
        actionOpenForm = new QAction(TrPreviewToolClass);
        actionOpenForm->setObjectName("actionOpenForm");
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/images/open_form.png"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        actionOpenForm->setIcon(icon);
        actionLoadTranslation = new QAction(TrPreviewToolClass);
        actionLoadTranslation->setObjectName("actionLoadTranslation");
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/images/load_translation.png"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        actionLoadTranslation->setIcon(icon1);
        actionReloadTranslations = new QAction(TrPreviewToolClass);
        actionReloadTranslations->setObjectName("actionReloadTranslations");
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/images/reload_translations.png"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        actionReloadTranslations->setIcon(icon2);
        actionClose = new QAction(TrPreviewToolClass);
        actionClose->setObjectName("actionClose");
        actionAbout = new QAction(TrPreviewToolClass);
        actionAbout->setObjectName("actionAbout");
        actionAbout_BobUI = new QAction(TrPreviewToolClass);
        actionAbout_BobUI->setObjectName("actionAbout_BobUI");
        centralWidget = new QWidget(TrPreviewToolClass);
        centralWidget->setObjectName("centralWidget");
        TrPreviewToolClass->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(TrPreviewToolClass);
        menuBar->setObjectName("menuBar");
        menuBar->setGeometry(QRect(0, 0, 593, 26));
        menuView = new QMenu(menuBar);
        menuView->setObjectName("menuView");
        menuViewViews = new QMenu(menuView);
        menuViewViews->setObjectName("menuViewViews");
        menuHelp = new QMenu(menuBar);
        menuHelp->setObjectName("menuHelp");
        menuFile = new QMenu(menuBar);
        menuFile->setObjectName("menuFile");
        TrPreviewToolClass->setMenuBar(menuBar);
        mainToolBar = new BOBUIoolBar(TrPreviewToolClass);
        mainToolBar->setObjectName("mainToolBar");
        mainToolBar->setOrientation(BobUI::Orientation::Horizontal);
        TrPreviewToolClass->addToolBar(BobUI::ToolBarArea::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(TrPreviewToolClass);
        statusBar->setObjectName("statusBar");
        TrPreviewToolClass->setStatusBar(statusBar);
        dwForms = new QDockWidget(TrPreviewToolClass);
        dwForms->setObjectName("dwForms");
        dockWidgetContents = new QWidget();
        dockWidgetContents->setObjectName("dockWidgetContents");
        vboxLayout = new QVBoxLayout(dockWidgetContents);
        vboxLayout->setSpacing(0);
        vboxLayout->setContentsMargins(0, 0, 0, 0);
        vboxLayout->setObjectName("vboxLayout");
        viewForms = new QListView(dockWidgetContents);
        viewForms->setObjectName("viewForms");
        viewForms->setEditTriggers(QAbstractItemView::EditTrigger::NoEditTriggers);
        viewForms->setAlternatingRowColors(true);
        viewForms->setUniformItemSizes(true);

        vboxLayout->addWidget(viewForms);

        dwForms->setWidget(dockWidgetContents);
        TrPreviewToolClass->addDockWidget(BobUI::DockWidgetArea::LeftDockWidgetArea, dwForms);

        menuBar->addAction(menuFile->menuAction());
        menuBar->addAction(menuView->menuAction());
        menuBar->addAction(menuHelp->menuAction());
        menuView->addAction(menuViewViews->menuAction());
        menuHelp->addAction(actionAbout);
        menuHelp->addAction(actionAbout_BobUI);
        menuFile->addAction(actionOpenForm);
        menuFile->addAction(actionLoadTranslation);
        menuFile->addAction(actionReloadTranslations);
        menuFile->addSeparator();
        menuFile->addAction(actionClose);
        mainToolBar->addAction(actionOpenForm);
        mainToolBar->addAction(actionLoadTranslation);
        mainToolBar->addAction(actionReloadTranslations);

        retranslateUi(TrPreviewToolClass);

        QMetaObject::connectSlotsByName(TrPreviewToolClass);
    } // setupUi

    void retranslateUi(QMainWindow *TrPreviewToolClass)
    {
        TrPreviewToolClass->setWindowTitle(QCoreApplication::translate("TrPreviewToolClass", "BobUI Translation Preview Tool", nullptr));
        actionOpenForm->setText(QCoreApplication::translate("TrPreviewToolClass", "&Open Form...", nullptr));
        actionLoadTranslation->setText(QCoreApplication::translate("TrPreviewToolClass", "&Load Translation...", nullptr));
        actionReloadTranslations->setText(QCoreApplication::translate("TrPreviewToolClass", "&Reload Translations", nullptr));
#if BOBUI_CONFIG(shortcut)
        actionReloadTranslations->setShortcut(QCoreApplication::translate("TrPreviewToolClass", "F5", nullptr));
#endif // BOBUI_CONFIG(shortcut)
        actionClose->setText(QCoreApplication::translate("TrPreviewToolClass", "&Close", nullptr));
        actionAbout->setText(QCoreApplication::translate("TrPreviewToolClass", "About", nullptr));
        actionAbout_BobUI->setText(QCoreApplication::translate("TrPreviewToolClass", "About BobUI", nullptr));
        menuView->setTitle(QCoreApplication::translate("TrPreviewToolClass", "&View", nullptr));
        menuViewViews->setTitle(QCoreApplication::translate("TrPreviewToolClass", "&Views", nullptr));
        menuHelp->setTitle(QCoreApplication::translate("TrPreviewToolClass", "&Help", nullptr));
        menuFile->setTitle(QCoreApplication::translate("TrPreviewToolClass", "&File", nullptr));
        dwForms->setWindowTitle(QCoreApplication::translate("TrPreviewToolClass", "Forms", nullptr));
    } // retranslateUi

};

namespace Ui {
    class TrPreviewToolClass: public Ui_TrPreviewToolClass {};
} // namespace Ui

BOBUI_END_NAMESPACE

#endif // TRPREVIEWTOOL_H
