/*

* Copyright (C) 2016 The BobUI Company Ltd.
* SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

*/

/********************************************************************************
** Form generated from reading UI file 'bobuigradientdialog.ui'
**
** Created by: BobUI User Interface Compiler version 6.0.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef BOBUIGRADIENTDIALOG_H
#define BOBUIGRADIENTDIALOG_H

#include <BobUICore/QVariant>
#include <BobUIWidgets/QAbstractButton>
#include <BobUIWidgets/QApplication>
#include <BobUIWidgets/QDialog>
#include <BobUIWidgets/QDialogButtonBox>
#include <BobUIWidgets/QVBoxLayout>
#include "bobuigradienteditor.h"

BOBUI_BEGIN_NAMESPACE

class Ui_BobUIGradientDialog
{
public:
    QVBoxLayout *vboxLayout;
    BobUIGradientEditor *gradientEditor;
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *BobUIGradientDialog)
    {
        if (BobUIGradientDialog->objectName().isEmpty())
            BobUIGradientDialog->setObjectName("BobUIGradientDialog");
        BobUIGradientDialog->resize(188, 81);
        vboxLayout = new QVBoxLayout(BobUIGradientDialog);
        vboxLayout->setObjectName("vboxLayout");
        gradientEditor = new BobUIGradientEditor(BobUIGradientDialog);
        gradientEditor->setObjectName("gradientEditor");
        QSizePolicy sizePolicy(QSizePolicy::Policy::MinimumExpanding, QSizePolicy::Policy::MinimumExpanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(gradientEditor->sizePolicy().hasHeightForWidth());
        gradientEditor->setSizePolicy(sizePolicy);

        vboxLayout->addWidget(gradientEditor);

        buttonBox = new QDialogButtonBox(BobUIGradientDialog);
        buttonBox->setObjectName("buttonBox");
        buttonBox->setOrientation(BobUI::Orientation::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::StandardButton::Cancel|QDialogButtonBox::StandardButton::Ok);

        vboxLayout->addWidget(buttonBox);


        retranslateUi(BobUIGradientDialog);
        QObject::connect(buttonBox, &QDialogButtonBox::accepted, BobUIGradientDialog, qOverload<>(&QDialog::accept));
        QObject::connect(buttonBox, &QDialogButtonBox::rejected, BobUIGradientDialog, qOverload<>(&QDialog::reject));

        QMetaObject::connectSlotsByName(BobUIGradientDialog);
    } // setupUi

    void retranslateUi(QDialog *BobUIGradientDialog)
    {
        BobUIGradientDialog->setWindowTitle(QCoreApplication::translate("BobUIGradientDialog", "Edit Gradient", nullptr));
    } // retranslateUi

};

namespace Ui {
    class BobUIGradientDialog: public Ui_BobUIGradientDialog {};
} // namespace Ui

BOBUI_END_NAMESPACE

#endif // BOBUIGRADIENTDIALOG_H
