/*

* Copyright (C) 2016 The BobUI Company Ltd.
* SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

*/

/********************************************************************************
** Form generated from reading UI file 'listwidgeteditor.ui'
**
** Created by: BobUI User Interface Compiler version 6.0.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef LISTWIDGETEDITOR_H
#define LISTWIDGETEDITOR_H

#include <BobUICore/QVariant>
#include <BobUIWidgets/QAbstractButton>
#include <BobUIWidgets/QApplication>
#include <BobUIWidgets/QDialog>
#include <BobUIWidgets/QDialogButtonBox>
#include <BobUIWidgets/QGridLayout>
#include <BobUIWidgets/QGroupBox>
#include <BobUIWidgets/QHBoxLayout>
#include <BobUIWidgets/QLabel>
#include <BobUIWidgets/QListWidget>
#include <BobUIWidgets/QSpacerItem>
#include <BobUIWidgets/BOBUIoolButton>
#include <BobUIWidgets/QVBoxLayout>
#include "iconselector_p.h"

BOBUI_BEGIN_NAMESPACE

namespace qdesigner_internal {

class Ui_ListWidgetEditor
{
public:
    QVBoxLayout *vboxLayout;
    QGroupBox *groupBox;
    QGridLayout *gridLayout;
    QListWidget *listWidget;
    QHBoxLayout *horizontalLayout_2;
    BOBUIoolButton *newItemButton;
    BOBUIoolButton *deleteItemButton;
    QSpacerItem *spacerItem;
    BOBUIoolButton *moveItemUpButton;
    BOBUIoolButton *moveItemDownButton;
    QHBoxLayout *horizontalLayout;
    QLabel *label;
    qdesigner_internal::IconSelector *itemIconSelector;
    QSpacerItem *horizontalSpacer;
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *qdesigner_internal__ListWidgetEditor)
    {
        if (qdesigner_internal__ListWidgetEditor->objectName().isEmpty())
            qdesigner_internal__ListWidgetEditor->setObjectName("qdesigner_internal__ListWidgetEditor");
        qdesigner_internal__ListWidgetEditor->resize(232, 245);
        vboxLayout = new QVBoxLayout(qdesigner_internal__ListWidgetEditor);
#ifndef Q_OS_MAC
        vboxLayout->setSpacing(6);
#endif
#ifndef Q_OS_MAC
        vboxLayout->setContentsMargins(9, 9, 9, 9);
#endif
        vboxLayout->setObjectName("vboxLayout");
        groupBox = new QGroupBox(qdesigner_internal__ListWidgetEditor);
        groupBox->setObjectName("groupBox");
        gridLayout = new QGridLayout(groupBox);
        gridLayout->setObjectName("gridLayout");
        listWidget = new QListWidget(groupBox);
        listWidget->setObjectName("listWidget");

        gridLayout->addWidget(listWidget, 0, 0, 1, 1);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName("horizontalLayout_2");
        newItemButton = new BOBUIoolButton(groupBox);
        newItemButton->setObjectName("newItemButton");

        horizontalLayout_2->addWidget(newItemButton);

        deleteItemButton = new BOBUIoolButton(groupBox);
        deleteItemButton->setObjectName("deleteItemButton");

        horizontalLayout_2->addWidget(deleteItemButton);

        spacerItem = new QSpacerItem(16, 10, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout_2->addItem(spacerItem);

        moveItemUpButton = new BOBUIoolButton(groupBox);
        moveItemUpButton->setObjectName("moveItemUpButton");

        horizontalLayout_2->addWidget(moveItemUpButton);

        moveItemDownButton = new BOBUIoolButton(groupBox);
        moveItemDownButton->setObjectName("moveItemDownButton");

        horizontalLayout_2->addWidget(moveItemDownButton);


        gridLayout->addLayout(horizontalLayout_2, 1, 0, 1, 1);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName("horizontalLayout");
        label = new QLabel(groupBox);
        label->setObjectName("label");

        horizontalLayout->addWidget(label);

        itemIconSelector = new qdesigner_internal::IconSelector(groupBox);
        itemIconSelector->setObjectName("itemIconSelector");

        horizontalLayout->addWidget(itemIconSelector);

        horizontalSpacer = new QSpacerItem(108, 21, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);


        gridLayout->addLayout(horizontalLayout, 2, 0, 1, 1);


        vboxLayout->addWidget(groupBox);

        buttonBox = new QDialogButtonBox(qdesigner_internal__ListWidgetEditor);
        buttonBox->setObjectName("buttonBox");
        buttonBox->setOrientation(BobUI::Orientation::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::StandardButton::Cancel|QDialogButtonBox::StandardButton::Ok);

        vboxLayout->addWidget(buttonBox);

        buttonBox->raise();
        groupBox->raise();
        QWidget::setTabOrder(listWidget, newItemButton);
        QWidget::setTabOrder(newItemButton, deleteItemButton);
        QWidget::setTabOrder(deleteItemButton, moveItemUpButton);
        QWidget::setTabOrder(moveItemUpButton, moveItemDownButton);

        retranslateUi(qdesigner_internal__ListWidgetEditor);
        QObject::connect(buttonBox, &QDialogButtonBox::accepted, qdesigner_internal__ListWidgetEditor, qOverload<>(&QDialog::accept));
        QObject::connect(buttonBox, &QDialogButtonBox::rejected, qdesigner_internal__ListWidgetEditor, qOverload<>(&QDialog::reject));

        QMetaObject::connectSlotsByName(qdesigner_internal__ListWidgetEditor);
    } // setupUi

    void retranslateUi(QDialog *qdesigner_internal__ListWidgetEditor)
    {
        qdesigner_internal__ListWidgetEditor->setWindowTitle(QCoreApplication::translate("qdesigner_internal::ListWidgetEditor", "Dialog", nullptr));
        groupBox->setTitle(QCoreApplication::translate("qdesigner_internal::ListWidgetEditor", "Items List", nullptr));
#if BOBUI_CONFIG(tooltip)
        listWidget->setToolTip(QCoreApplication::translate("qdesigner_internal::ListWidgetEditor", "Items List", nullptr));
#endif // BOBUI_CONFIG(tooltip)
#if BOBUI_CONFIG(tooltip)
        newItemButton->setToolTip(QCoreApplication::translate("qdesigner_internal::ListWidgetEditor", "New Item", nullptr));
#endif // BOBUI_CONFIG(tooltip)
        newItemButton->setText(QCoreApplication::translate("qdesigner_internal::ListWidgetEditor", "&New", nullptr));
#if BOBUI_CONFIG(tooltip)
        deleteItemButton->setToolTip(QCoreApplication::translate("qdesigner_internal::ListWidgetEditor", "Delete Item", nullptr));
#endif // BOBUI_CONFIG(tooltip)
        deleteItemButton->setText(QCoreApplication::translate("qdesigner_internal::ListWidgetEditor", "&Delete", nullptr));
#if BOBUI_CONFIG(tooltip)
        moveItemUpButton->setToolTip(QCoreApplication::translate("qdesigner_internal::ListWidgetEditor", "Move Item Up", nullptr));
#endif // BOBUI_CONFIG(tooltip)
        moveItemUpButton->setText(QCoreApplication::translate("qdesigner_internal::ListWidgetEditor", "U", nullptr));
#if BOBUI_CONFIG(tooltip)
        moveItemDownButton->setToolTip(QCoreApplication::translate("qdesigner_internal::ListWidgetEditor", "Move Item Down", nullptr));
#endif // BOBUI_CONFIG(tooltip)
        moveItemDownButton->setText(QCoreApplication::translate("qdesigner_internal::ListWidgetEditor", "D", nullptr));
        label->setText(QCoreApplication::translate("qdesigner_internal::ListWidgetEditor", "Icon", nullptr));
    } // retranslateUi

};

} // namespace qdesigner_internal

namespace qdesigner_internal {
namespace Ui {
    class ListWidgetEditor: public Ui_ListWidgetEditor {};
} // namespace Ui
} // namespace qdesigner_internal

BOBUI_END_NAMESPACE

#endif // LISTWIDGETEDITOR_H
