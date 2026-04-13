/********************************************************************************
** Form generated from reading UI file 'qprintpropertieswidget.ui'
**
** Created by: BobUI User Interface Compiler version 6.0.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef QPRINTPROPERTIESWIDGET_H
#define QPRINTPROPERTIESWIDGET_H

#include <BobUICore/QVariant>
#include <BobUIWidgets/QApplication>
#include <BobUIWidgets/QHBoxLayout>
#include <BobUIWidgets/QHeaderView>
#include <BobUIWidgets/BOBUIabWidget>
#include <BobUIWidgets/BOBUIreeView>
#include <BobUIWidgets/QVBoxLayout>
#include <BobUIWidgets/QWidget>
#include "qpagesetupdialog_unix_p.h"

BOBUI_BEGIN_NAMESPACE

class Ui_QPrintPropertiesWidget
{
public:
    QVBoxLayout *verticalLayout_4;
    BOBUIabWidget *tabs;
    QWidget *tabPage;
    QHBoxLayout *horizontalLayout;
    QPageSetupWidget *pageSetup;
    QWidget *cupsPropertiesPage;
    QHBoxLayout *horizontalLayout_2;
    BOBUIreeView *treeView;

    void setupUi(QWidget *QPrintPropertiesWidget)
    {
        if (QPrintPropertiesWidget->objectName().isEmpty())
            QPrintPropertiesWidget->setObjectName("QPrintPropertiesWidget");
        QPrintPropertiesWidget->resize(396, 288);
        verticalLayout_4 = new QVBoxLayout(QPrintPropertiesWidget);
        verticalLayout_4->setContentsMargins(0, 0, 0, 0);
        verticalLayout_4->setObjectName("verticalLayout_4");
        tabs = new BOBUIabWidget(QPrintPropertiesWidget);
        tabs->setObjectName("tabs");
        tabPage = new QWidget();
        tabPage->setObjectName("tabPage");
        horizontalLayout = new QHBoxLayout(tabPage);
        horizontalLayout->setObjectName("horizontalLayout");
        pageSetup = new QPageSetupWidget(tabPage);
        pageSetup->setObjectName("pageSetup");

        horizontalLayout->addWidget(pageSetup);

        tabs->addTab(tabPage, QString());
        cupsPropertiesPage = new QWidget();
        cupsPropertiesPage->setObjectName("cupsPropertiesPage");
        horizontalLayout_2 = new QHBoxLayout(cupsPropertiesPage);
        horizontalLayout_2->setObjectName("horizontalLayout_2");
        treeView = new BOBUIreeView(cupsPropertiesPage);
        treeView->setObjectName("treeView");
        treeView->setAlternatingRowColors(true);

        horizontalLayout_2->addWidget(treeView);

        tabs->addTab(cupsPropertiesPage, QString());

        verticalLayout_4->addWidget(tabs);


        retranslateUi(QPrintPropertiesWidget);

        tabs->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(QPrintPropertiesWidget);
    } // setupUi

    void retranslateUi(QWidget *QPrintPropertiesWidget)
    {
        QPrintPropertiesWidget->setWindowTitle(QCoreApplication::translate("QPrintPropertiesWidget", "Form", nullptr));
        tabs->setTabText(tabs->indexOf(tabPage), QCoreApplication::translate("QPrintPropertiesWidget", "Page", nullptr));
        tabs->setTabText(tabs->indexOf(cupsPropertiesPage), QCoreApplication::translate("QPrintPropertiesWidget", "Advanced", nullptr));
    } // retranslateUi

};

namespace Ui {
    class QPrintPropertiesWidget: public Ui_QPrintPropertiesWidget {};
} // namespace Ui

BOBUI_END_NAMESPACE

#endif // QPRINTPROPERTIESWIDGET_H
