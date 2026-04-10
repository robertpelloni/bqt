/*

* Copyright (C) 2016 The BobUI Company Ltd.
* SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

*/

/********************************************************************************
** Form generated from reading UI file 'tablewidgeteditor.ui'
**
** Created by: BobUI User Interface Compiler version 6.0.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef TABLEWIDGETEDITOR_H
#define TABLEWIDGETEDITOR_H

#include <BobUICore/QVariant>
#include <BobUIWidgets/QAbstractButton>
#include <BobUIWidgets/QApplication>
#include <BobUIWidgets/QDialog>
#include <BobUIWidgets/QDialogButtonBox>
#include <BobUIWidgets/QGridLayout>
#include <BobUIWidgets/QGroupBox>
#include <BobUIWidgets/QHBoxLayout>
#include <BobUIWidgets/QHeaderView>
#include <BobUIWidgets/QLabel>
#include <BobUIWidgets/QListWidget>
#include <BobUIWidgets/QSpacerItem>
#include <BobUIWidgets/BOBUIableWidget>
#include <BobUIWidgets/BOBUIoolButton>
#include <BobUIWidgets/QVBoxLayout>
#include <BobUIWidgets/QWidget>
#include "iconselector_p.h"

BOBUI_BEGIN_NAMESPACE

namespace qdesigner_internal {

class Ui_TableWidgetEditor
{
public:
    QGridLayout *gridLayout_4;
    QGroupBox *itemsBox;
    QGridLayout *gridLayout;
    BOBUIableWidget *tableWidget;
    QHBoxLayout *horizontalLayout_5;
    QLabel *label_3;
    qdesigner_internal::IconSelector *itemIconSelector;
    QSpacerItem *horizontalSpacer;
    QDialogButtonBox *buttonBox;
    QWidget *widget;
    QVBoxLayout *verticalLayout;
    QGroupBox *columnsBox;
    QGridLayout *gridLayout_2;
    QListWidget *columnsListWidget;
    QHBoxLayout *horizontalLayout_3;
    BOBUIoolButton *newColumnButton;
    BOBUIoolButton *deleteColumnButton;
    QSpacerItem *spacerItem;
    BOBUIoolButton *moveColumnUpButton;
    BOBUIoolButton *moveColumnDownButton;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label;
    qdesigner_internal::IconSelector *columnIconSelector;
    QSpacerItem *spacerItem1;
    QGroupBox *rowsBox;
    QGridLayout *gridLayout_3;
    QListWidget *rowsListWidget;
    QHBoxLayout *horizontalLayout_4;
    BOBUIoolButton *newRowButton;
    BOBUIoolButton *deleteRowButton;
    QSpacerItem *spacerItem2;
    BOBUIoolButton *moveRowUpButton;
    BOBUIoolButton *moveRowDownButton;
    QHBoxLayout *horizontalLayout;
    QLabel *label_2;
    qdesigner_internal::IconSelector *rowIconSelector;
    QSpacerItem *spacerItem3;

    void setupUi(QDialog *qdesigner_internal__TableWidgetEditor)
    {
        if (qdesigner_internal__TableWidgetEditor->objectName().isEmpty())
            qdesigner_internal__TableWidgetEditor->setObjectName("qdesigner_internal__TableWidgetEditor");
        qdesigner_internal__TableWidgetEditor->resize(591, 455);
        gridLayout_4 = new QGridLayout(qdesigner_internal__TableWidgetEditor);
        gridLayout_4->setObjectName("gridLayout_4");
        itemsBox = new QGroupBox(qdesigner_internal__TableWidgetEditor);
        itemsBox->setObjectName("itemsBox");
        gridLayout = new QGridLayout(itemsBox);
        gridLayout->setObjectName("gridLayout");
        tableWidget = new BOBUIableWidget(itemsBox);
        tableWidget->setObjectName("tableWidget");

        gridLayout->addWidget(tableWidget, 0, 0, 1, 1);

        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setObjectName("horizontalLayout_5");
        label_3 = new QLabel(itemsBox);
        label_3->setObjectName("label_3");

        horizontalLayout_5->addWidget(label_3);

        itemIconSelector = new qdesigner_internal::IconSelector(itemsBox);
        itemIconSelector->setObjectName("itemIconSelector");

        horizontalLayout_5->addWidget(itemIconSelector);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout_5->addItem(horizontalSpacer);


        gridLayout->addLayout(horizontalLayout_5, 1, 0, 1, 1);


        gridLayout_4->addWidget(itemsBox, 0, 0, 1, 1);

        buttonBox = new QDialogButtonBox(qdesigner_internal__TableWidgetEditor);
        buttonBox->setObjectName("buttonBox");
        buttonBox->setOrientation(BobUI::Orientation::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::StandardButton::Cancel|QDialogButtonBox::StandardButton::Ok);

        gridLayout_4->addWidget(buttonBox, 1, 0, 1, 2);

        widget = new QWidget(qdesigner_internal__TableWidgetEditor);
        widget->setObjectName("widget");
        QSizePolicy sizePolicy(QSizePolicy::Policy::Fixed, QSizePolicy::Policy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(widget->sizePolicy().hasHeightForWidth());
        widget->setSizePolicy(sizePolicy);
        verticalLayout = new QVBoxLayout(widget);
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        verticalLayout->setObjectName("verticalLayout");
        columnsBox = new QGroupBox(widget);
        columnsBox->setObjectName("columnsBox");
        gridLayout_2 = new QGridLayout(columnsBox);
        gridLayout_2->setObjectName("gridLayout_2");
        columnsListWidget = new QListWidget(columnsBox);
        columnsListWidget->setObjectName("columnsListWidget");
        QSizePolicy sizePolicy1(QSizePolicy::Policy::Ignored, QSizePolicy::Policy::Expanding);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(columnsListWidget->sizePolicy().hasHeightForWidth());
        columnsListWidget->setSizePolicy(sizePolicy1);
        columnsListWidget->setFocusPolicy(BobUI::FocusPolicy::TabFocus);

        gridLayout_2->addWidget(columnsListWidget, 0, 0, 1, 1);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName("horizontalLayout_3");
        newColumnButton = new BOBUIoolButton(columnsBox);
        newColumnButton->setObjectName("newColumnButton");

        horizontalLayout_3->addWidget(newColumnButton);

        deleteColumnButton = new BOBUIoolButton(columnsBox);
        deleteColumnButton->setObjectName("deleteColumnButton");

        horizontalLayout_3->addWidget(deleteColumnButton);

        spacerItem = new QSpacerItem(0, 23, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout_3->addItem(spacerItem);

        moveColumnUpButton = new BOBUIoolButton(columnsBox);
        moveColumnUpButton->setObjectName("moveColumnUpButton");

        horizontalLayout_3->addWidget(moveColumnUpButton);

        moveColumnDownButton = new BOBUIoolButton(columnsBox);
        moveColumnDownButton->setObjectName("moveColumnDownButton");

        horizontalLayout_3->addWidget(moveColumnDownButton);


        gridLayout_2->addLayout(horizontalLayout_3, 1, 0, 1, 1);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName("horizontalLayout_2");
        label = new QLabel(columnsBox);
        label->setObjectName("label");

        horizontalLayout_2->addWidget(label);

        columnIconSelector = new qdesigner_internal::IconSelector(columnsBox);
        columnIconSelector->setObjectName("columnIconSelector");

        horizontalLayout_2->addWidget(columnIconSelector);

        spacerItem1 = new QSpacerItem(0, 21, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout_2->addItem(spacerItem1);


        gridLayout_2->addLayout(horizontalLayout_2, 2, 0, 1, 1);


        verticalLayout->addWidget(columnsBox);

        rowsBox = new QGroupBox(widget);
        rowsBox->setObjectName("rowsBox");
        gridLayout_3 = new QGridLayout(rowsBox);
        gridLayout_3->setObjectName("gridLayout_3");
        rowsListWidget = new QListWidget(rowsBox);
        rowsListWidget->setObjectName("rowsListWidget");
        sizePolicy1.setHeightForWidth(rowsListWidget->sizePolicy().hasHeightForWidth());
        rowsListWidget->setSizePolicy(sizePolicy1);
        rowsListWidget->setFocusPolicy(BobUI::FocusPolicy::TabFocus);

        gridLayout_3->addWidget(rowsListWidget, 0, 0, 1, 1);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName("horizontalLayout_4");
        newRowButton = new BOBUIoolButton(rowsBox);
        newRowButton->setObjectName("newRowButton");

        horizontalLayout_4->addWidget(newRowButton);

        deleteRowButton = new BOBUIoolButton(rowsBox);
        deleteRowButton->setObjectName("deleteRowButton");

        horizontalLayout_4->addWidget(deleteRowButton);

        spacerItem2 = new QSpacerItem(0, 23, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout_4->addItem(spacerItem2);

        moveRowUpButton = new BOBUIoolButton(rowsBox);
        moveRowUpButton->setObjectName("moveRowUpButton");

        horizontalLayout_4->addWidget(moveRowUpButton);

        moveRowDownButton = new BOBUIoolButton(rowsBox);
        moveRowDownButton->setObjectName("moveRowDownButton");

        horizontalLayout_4->addWidget(moveRowDownButton);


        gridLayout_3->addLayout(horizontalLayout_4, 1, 0, 1, 1);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName("horizontalLayout");
        label_2 = new QLabel(rowsBox);
        label_2->setObjectName("label_2");

        horizontalLayout->addWidget(label_2);

        rowIconSelector = new qdesigner_internal::IconSelector(rowsBox);
        rowIconSelector->setObjectName("rowIconSelector");

        horizontalLayout->addWidget(rowIconSelector);

        spacerItem3 = new QSpacerItem(0, 21, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout->addItem(spacerItem3);


        gridLayout_3->addLayout(horizontalLayout, 2, 0, 1, 1);


        verticalLayout->addWidget(rowsBox);


        gridLayout_4->addWidget(widget, 0, 1, 1, 1);

        QWidget::setTabOrder(tableWidget, columnsListWidget);
        QWidget::setTabOrder(columnsListWidget, newColumnButton);
        QWidget::setTabOrder(newColumnButton, deleteColumnButton);
        QWidget::setTabOrder(deleteColumnButton, moveColumnUpButton);
        QWidget::setTabOrder(moveColumnUpButton, moveColumnDownButton);
        QWidget::setTabOrder(moveColumnDownButton, rowsListWidget);
        QWidget::setTabOrder(rowsListWidget, newRowButton);
        QWidget::setTabOrder(newRowButton, deleteRowButton);
        QWidget::setTabOrder(deleteRowButton, moveRowUpButton);
        QWidget::setTabOrder(moveRowUpButton, moveRowDownButton);

        retranslateUi(qdesigner_internal__TableWidgetEditor);
        QObject::connect(buttonBox, &QDialogButtonBox::accepted, qdesigner_internal__TableWidgetEditor, qOverload<>(&QDialog::accept));
        QObject::connect(buttonBox, &QDialogButtonBox::rejected, qdesigner_internal__TableWidgetEditor, qOverload<>(&QDialog::reject));

        QMetaObject::connectSlotsByName(qdesigner_internal__TableWidgetEditor);
    } // setupUi

    void retranslateUi(QDialog *qdesigner_internal__TableWidgetEditor)
    {
        qdesigner_internal__TableWidgetEditor->setWindowTitle(QCoreApplication::translate("qdesigner_internal::TableWidgetEditor", "Edit Table Widget", nullptr));
        itemsBox->setTitle(QCoreApplication::translate("qdesigner_internal::TableWidgetEditor", "Table Items", nullptr));
#if BOBUI_CONFIG(tooltip)
        tableWidget->setToolTip(QCoreApplication::translate("qdesigner_internal::TableWidgetEditor", "Table Items", nullptr));
#endif // BOBUI_CONFIG(tooltip)
        label_3->setText(QCoreApplication::translate("qdesigner_internal::TableWidgetEditor", "Icon", nullptr));
        columnsBox->setTitle(QCoreApplication::translate("qdesigner_internal::TableWidgetEditor", "Columns", nullptr));
#if BOBUI_CONFIG(tooltip)
        columnsListWidget->setToolTip(QCoreApplication::translate("qdesigner_internal::TableWidgetEditor", "Table Columns", nullptr));
#endif // BOBUI_CONFIG(tooltip)
#if BOBUI_CONFIG(tooltip)
        newColumnButton->setToolTip(QCoreApplication::translate("qdesigner_internal::TableWidgetEditor", "New Column", nullptr));
#endif // BOBUI_CONFIG(tooltip)
        newColumnButton->setText(QCoreApplication::translate("qdesigner_internal::TableWidgetEditor", "New", nullptr));
#if BOBUI_CONFIG(tooltip)
        deleteColumnButton->setToolTip(QCoreApplication::translate("qdesigner_internal::TableWidgetEditor", "Delete Column", nullptr));
#endif // BOBUI_CONFIG(tooltip)
        deleteColumnButton->setText(QCoreApplication::translate("qdesigner_internal::TableWidgetEditor", "Delete", nullptr));
#if BOBUI_CONFIG(tooltip)
        moveColumnUpButton->setToolTip(QCoreApplication::translate("qdesigner_internal::TableWidgetEditor", "Move Column Up", nullptr));
#endif // BOBUI_CONFIG(tooltip)
        moveColumnUpButton->setText(QCoreApplication::translate("qdesigner_internal::TableWidgetEditor", "U", nullptr));
#if BOBUI_CONFIG(tooltip)
        moveColumnDownButton->setToolTip(QCoreApplication::translate("qdesigner_internal::TableWidgetEditor", "Move Column Down", nullptr));
#endif // BOBUI_CONFIG(tooltip)
        moveColumnDownButton->setText(QCoreApplication::translate("qdesigner_internal::TableWidgetEditor", "D", nullptr));
        label->setText(QCoreApplication::translate("qdesigner_internal::TableWidgetEditor", "Icon", nullptr));
        rowsBox->setTitle(QCoreApplication::translate("qdesigner_internal::TableWidgetEditor", "Rows", nullptr));
#if BOBUI_CONFIG(tooltip)
        rowsListWidget->setToolTip(QCoreApplication::translate("qdesigner_internal::TableWidgetEditor", "Table Rows", nullptr));
#endif // BOBUI_CONFIG(tooltip)
#if BOBUI_CONFIG(tooltip)
        newRowButton->setToolTip(QCoreApplication::translate("qdesigner_internal::TableWidgetEditor", "New Row", nullptr));
#endif // BOBUI_CONFIG(tooltip)
        newRowButton->setText(QCoreApplication::translate("qdesigner_internal::TableWidgetEditor", "New", nullptr));
#if BOBUI_CONFIG(tooltip)
        deleteRowButton->setToolTip(QCoreApplication::translate("qdesigner_internal::TableWidgetEditor", "Delete Row", nullptr));
#endif // BOBUI_CONFIG(tooltip)
        deleteRowButton->setText(QCoreApplication::translate("qdesigner_internal::TableWidgetEditor", "Delete", nullptr));
#if BOBUI_CONFIG(tooltip)
        moveRowUpButton->setToolTip(QCoreApplication::translate("qdesigner_internal::TableWidgetEditor", "Move Row Up", nullptr));
#endif // BOBUI_CONFIG(tooltip)
        moveRowUpButton->setText(QCoreApplication::translate("qdesigner_internal::TableWidgetEditor", "U", nullptr));
#if BOBUI_CONFIG(tooltip)
        moveRowDownButton->setToolTip(QCoreApplication::translate("qdesigner_internal::TableWidgetEditor", "Move Row Down", nullptr));
#endif // BOBUI_CONFIG(tooltip)
        moveRowDownButton->setText(QCoreApplication::translate("qdesigner_internal::TableWidgetEditor", "D", nullptr));
        label_2->setText(QCoreApplication::translate("qdesigner_internal::TableWidgetEditor", "Icon", nullptr));
    } // retranslateUi

};

} // namespace qdesigner_internal

namespace qdesigner_internal {
namespace Ui {
    class TableWidgetEditor: public Ui_TableWidgetEditor {};
} // namespace Ui
} // namespace qdesigner_internal

BOBUI_END_NAMESPACE

#endif // TABLEWIDGETEDITOR_H
