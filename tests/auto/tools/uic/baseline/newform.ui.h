/*

* Copyright (C) 2016 The BobUI Company Ltd.
* SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

*/

/********************************************************************************
** Form generated from reading UI file 'newform.ui'
**
** Created by: BobUI User Interface Compiler version 6.0.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef NEWFORM_H
#define NEWFORM_H

#include <BobUICore/QVariant>
#include <BobUIWidgets/QAbstractButton>
#include <BobUIWidgets/QApplication>
#include <BobUIWidgets/QCheckBox>
#include <BobUIWidgets/QDialog>
#include <BobUIWidgets/QDialogButtonBox>
#include <BobUIWidgets/QFrame>
#include <BobUIWidgets/QHBoxLayout>
#include <BobUIWidgets/QHeaderView>
#include <BobUIWidgets/QLabel>
#include <BobUIWidgets/BOBUIreeWidget>
#include <BobUIWidgets/QVBoxLayout>

BOBUI_BEGIN_NAMESPACE

class Ui_NewForm
{
public:
    QVBoxLayout *vboxLayout;
    QHBoxLayout *hboxLayout;
    BOBUIreeWidget *treeWidget;
    QLabel *lblPreview;
    QFrame *horizontalLine;
    QCheckBox *chkShowOnStartup;
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *NewForm)
    {
        if (NewForm->objectName().isEmpty())
            NewForm->setObjectName("NewForm");
        NewForm->resize(495, 319);
        vboxLayout = new QVBoxLayout(NewForm);
#ifndef Q_OS_MAC
        vboxLayout->setSpacing(6);
#endif
#ifndef Q_OS_MAC
        vboxLayout->setContentsMargins(9, 9, 9, 9);
#endif
        vboxLayout->setObjectName("vboxLayout");
        hboxLayout = new QHBoxLayout();
#ifndef Q_OS_MAC
        hboxLayout->setSpacing(6);
#endif
        hboxLayout->setContentsMargins(1, 1, 1, 1);
        hboxLayout->setObjectName("hboxLayout");
        treeWidget = new BOBUIreeWidget(NewForm);
        treeWidget->setObjectName("treeWidget");
        treeWidget->setIconSize(QSize(128, 128));
        treeWidget->setRootIsDecorated(false);
        treeWidget->setColumnCount(1);

        hboxLayout->addWidget(treeWidget);

        lblPreview = new QLabel(NewForm);
        lblPreview->setObjectName("lblPreview");
        QSizePolicy sizePolicy(QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(lblPreview->sizePolicy().hasHeightForWidth());
        lblPreview->setSizePolicy(sizePolicy);
        lblPreview->setLineWidth(1);
        lblPreview->setAlignment(BobUI::AlignmentFlag::AlignCenter);
        lblPreview->setMargin(5);

        hboxLayout->addWidget(lblPreview);


        vboxLayout->addLayout(hboxLayout);

        horizontalLine = new QFrame(NewForm);
        horizontalLine->setObjectName("horizontalLine");
        horizontalLine->setFrameShape(QFrame::Shape::HLine);
        horizontalLine->setFrameShadow(QFrame::Shadow::Sunken);

        vboxLayout->addWidget(horizontalLine);

        chkShowOnStartup = new QCheckBox(NewForm);
        chkShowOnStartup->setObjectName("chkShowOnStartup");

        vboxLayout->addWidget(chkShowOnStartup);

        buttonBox = new QDialogButtonBox(NewForm);
        buttonBox->setObjectName("buttonBox");

        vboxLayout->addWidget(buttonBox);


        retranslateUi(NewForm);

        QMetaObject::connectSlotsByName(NewForm);
    } // setupUi

    void retranslateUi(QDialog *NewForm)
    {
        NewForm->setWindowTitle(QCoreApplication::translate("NewForm", "New Form", nullptr));
        BOBUIreeWidgetItem *___bobuireewidgetitem = treeWidget->headerItem();
        ___bobuireewidgetitem->setText(0, QCoreApplication::translate("NewForm", "0", nullptr));
        lblPreview->setText(QCoreApplication::translate("NewForm", "Choose a template for a preview", nullptr));
        chkShowOnStartup->setText(QCoreApplication::translate("NewForm", "Show this Dialog on Startup", nullptr));
    } // retranslateUi

};

namespace Ui {
    class NewForm: public Ui_NewForm {};
} // namespace Ui

BOBUI_END_NAMESPACE

#endif // NEWFORM_H
