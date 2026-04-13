/*
Copyright (C) 2016 The BobUI Company Ltd.
SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only
*/

/********************************************************************************
** Form generated from reading UI file 'Widget.ui'
**
** Created by: BobUI User Interface Compiler version 6.0.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef WIDGET_H
#define WIDGET_H

#include <BobUICore/QVariant>
#include <BobUIWidgets/QApplication>
#include <BobUIWidgets/QGroupBox>
#include <BobUIWidgets/QLabel>
#include <BobUIWidgets/QPushButton>
#include <BobUIWidgets/QVBoxLayout>
#include <BobUIWidgets/QWidget>

BOBUI_BEGIN_NAMESPACE

class Ui_Form
{
public:
    QVBoxLayout *vboxLayout;
    QLabel *Alabel;
    QGroupBox *groupBox;
    QPushButton *pushButton;

    void setupUi(QWidget *Form)
    {
        if (Form->objectName().isEmpty())
            Form->setObjectName("Form");
        Form->resize(400, 300);
        vboxLayout = new QVBoxLayout(Form);
        vboxLayout->setObjectName("vboxLayout");
        Alabel = new QLabel(Form);
        Alabel->setObjectName("Alabel");

        vboxLayout->addWidget(Alabel);

        groupBox = new QGroupBox(Form);
        groupBox->setObjectName("groupBox");

        vboxLayout->addWidget(groupBox);

        pushButton = new QPushButton(Form);
        pushButton->setObjectName("pushButton");

        vboxLayout->addWidget(pushButton);


        retranslateUi(Form);

        QMetaObject::connectSlotsByName(Form);
    } // setupUi

    void retranslateUi(QWidget *Form)
    {
        Form->setWindowTitle(QCoreApplication::translate("Form", "Form", nullptr));
        Alabel->setText(QCoreApplication::translate("Form", "A label.\n"
"One new line.\n"
"Another new line.\n"
"Last line.", nullptr));
        groupBox->setTitle(QCoreApplication::translate("Form", "A Group Box", nullptr));
        pushButton->setText(QCoreApplication::translate("Form", "PushButton", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Form: public Ui_Form {};
} // namespace Ui

BOBUI_END_NAMESPACE

#endif // WIDGET_H
