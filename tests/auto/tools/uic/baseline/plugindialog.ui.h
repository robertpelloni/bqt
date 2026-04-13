/*

* Copyright (C) 2016 The BobUI Company Ltd.
* SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

*/

/********************************************************************************
** Form generated from reading UI file 'plugindialog.ui'
**
** Created by: BobUI User Interface Compiler version 6.0.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef PLUGINDIALOG_H
#define PLUGINDIALOG_H

#include <BobUICore/QVariant>
#include <BobUIWidgets/QAbstractButton>
#include <BobUIWidgets/QApplication>
#include <BobUIWidgets/QDialog>
#include <BobUIWidgets/QDialogButtonBox>
#include <BobUIWidgets/QHBoxLayout>
#include <BobUIWidgets/QHeaderView>
#include <BobUIWidgets/QLabel>
#include <BobUIWidgets/BOBUIreeWidget>
#include <BobUIWidgets/QVBoxLayout>

BOBUI_BEGIN_NAMESPACE

class Ui_PluginDialog
{
public:
    QVBoxLayout *vboxLayout;
    QLabel *label;
    BOBUIreeWidget *treeWidget;
    QLabel *message;
    QHBoxLayout *hboxLayout;
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *PluginDialog)
    {
        if (PluginDialog->objectName().isEmpty())
            PluginDialog->setObjectName("PluginDialog");
        PluginDialog->resize(401, 331);
        vboxLayout = new QVBoxLayout(PluginDialog);
        vboxLayout->setSpacing(6);
        vboxLayout->setObjectName("vboxLayout");
        vboxLayout->setContentsMargins(8, 8, 8, 8);
        label = new QLabel(PluginDialog);
        label->setObjectName("label");
        label->setWordWrap(true);

        vboxLayout->addWidget(label);

        treeWidget = new BOBUIreeWidget(PluginDialog);
        treeWidget->setObjectName("treeWidget");
        treeWidget->setTextElideMode(BobUI::TextElideMode::ElideNone);

        vboxLayout->addWidget(treeWidget);

        message = new QLabel(PluginDialog);
        message->setObjectName("message");
        message->setWordWrap(true);

        vboxLayout->addWidget(message);

        hboxLayout = new QHBoxLayout();
        hboxLayout->setSpacing(6);
        hboxLayout->setObjectName("hboxLayout");
        hboxLayout->setContentsMargins(0, 0, 0, 0);

        vboxLayout->addLayout(hboxLayout);

        buttonBox = new QDialogButtonBox(PluginDialog);
        buttonBox->setObjectName("buttonBox");
        buttonBox->setOrientation(BobUI::Orientation::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::StandardButton::Close);

        vboxLayout->addWidget(buttonBox);


        retranslateUi(PluginDialog);
        QObject::connect(buttonBox, &QDialogButtonBox::rejected, PluginDialog, qOverload<>(&QDialog::reject));

        QMetaObject::connectSlotsByName(PluginDialog);
    } // setupUi

    void retranslateUi(QDialog *PluginDialog)
    {
        PluginDialog->setWindowTitle(QCoreApplication::translate("PluginDialog", "Plugin Information", nullptr));
        label->setText(QCoreApplication::translate("PluginDialog", "TextLabel", nullptr));
        BOBUIreeWidgetItem *___bobuireewidgetitem = treeWidget->headerItem();
        ___bobuireewidgetitem->setText(0, QCoreApplication::translate("PluginDialog", "1", nullptr));
        message->setText(QCoreApplication::translate("PluginDialog", "TextLabel", nullptr));
    } // retranslateUi

};

namespace Ui {
    class PluginDialog: public Ui_PluginDialog {};
} // namespace Ui

BOBUI_END_NAMESPACE

#endif // PLUGINDIALOG_H
