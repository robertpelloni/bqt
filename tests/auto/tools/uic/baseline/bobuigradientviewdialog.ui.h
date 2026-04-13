/*

* Copyright (C) 2016 The BobUI Company Ltd.
* SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

*/

/********************************************************************************
** Form generated from reading UI file 'bobuigradientviewdialog.ui'
**
** Created by: BobUI User Interface Compiler version 6.0.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef BOBUIGRADIENTVIEWDIALOG_H
#define BOBUIGRADIENTVIEWDIALOG_H

#include <BobUICore/QVariant>
#include <BobUIWidgets/QAbstractButton>
#include <BobUIWidgets/QApplication>
#include <BobUIWidgets/QDialog>
#include <BobUIWidgets/QDialogButtonBox>
#include <BobUIWidgets/QVBoxLayout>
#include "bobuigradientview.h"

BOBUI_BEGIN_NAMESPACE

class Ui_BobUIGradientViewDialog
{
public:
    QVBoxLayout *vboxLayout;
    BobUIGradientView *gradientView;
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *BobUIGradientViewDialog)
    {
        if (BobUIGradientViewDialog->objectName().isEmpty())
            BobUIGradientViewDialog->setObjectName("BobUIGradientViewDialog");
        BobUIGradientViewDialog->resize(188, 72);
        vboxLayout = new QVBoxLayout(BobUIGradientViewDialog);
        vboxLayout->setObjectName("vboxLayout");
        gradientView = new BobUIGradientView(BobUIGradientViewDialog);
        gradientView->setObjectName("gradientView");
        QSizePolicy sizePolicy(QSizePolicy::Policy::MinimumExpanding, QSizePolicy::Policy::MinimumExpanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(gradientView->sizePolicy().hasHeightForWidth());
        gradientView->setSizePolicy(sizePolicy);

        vboxLayout->addWidget(gradientView);

        buttonBox = new QDialogButtonBox(BobUIGradientViewDialog);
        buttonBox->setObjectName("buttonBox");
        buttonBox->setOrientation(BobUI::Orientation::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::StandardButton::Cancel|QDialogButtonBox::StandardButton::Ok);

        vboxLayout->addWidget(buttonBox);


        retranslateUi(BobUIGradientViewDialog);
        QObject::connect(buttonBox, &QDialogButtonBox::accepted, BobUIGradientViewDialog, qOverload<>(&QDialog::accept));
        QObject::connect(buttonBox, &QDialogButtonBox::rejected, BobUIGradientViewDialog, qOverload<>(&QDialog::reject));

        QMetaObject::connectSlotsByName(BobUIGradientViewDialog);
    } // setupUi

    void retranslateUi(QDialog *BobUIGradientViewDialog)
    {
        BobUIGradientViewDialog->setWindowTitle(QCoreApplication::translate("BobUIGradientViewDialog", "Select Gradient", nullptr));
    } // retranslateUi

};

namespace Ui {
    class BobUIGradientViewDialog: public Ui_BobUIGradientViewDialog {};
} // namespace Ui

BOBUI_END_NAMESPACE

#endif // BOBUIGRADIENTVIEWDIALOG_H
