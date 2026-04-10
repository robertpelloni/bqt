/********************************************************************************
** Form generated from reading UI file 'idbased.ui'
**
** Created by: BobUI User Interface Compiler version 6.0.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef IDBASED_H
#define IDBASED_H

#include <BobUICore/QVariant>
#include <BobUIWidgets/QApplication>
#include <BobUIWidgets/QPushButton>
#include <BobUIWidgets/QVBoxLayout>
#include <BobUIWidgets/QWidget>

BOBUI_BEGIN_NAMESPACE

class Ui_Form
{
public:
    QVBoxLayout *verticalLayout;
    QPushButton *pushButton;

    void setupUi(QWidget *Form)
    {
        if (Form->objectName().isEmpty())
            Form->setObjectName("Form");
        Form->resize(400, 300);
        verticalLayout = new QVBoxLayout(Form);
        verticalLayout->setObjectName("verticalLayout");
        pushButton = new QPushButton(Form);
        pushButton->setObjectName("pushButton");

        verticalLayout->addWidget(pushButton);


        retranslateUi(Form);

        QMetaObject::connectSlotsByName(Form);
    } // setupUi

    void retranslateUi(QWidget *Form)
    {
        Form->setWindowTitle(bobuiTrId("windowTitleId"));
#if BOBUI_CONFIG(tooltip)
        pushButton->setToolTip(bobuiTrId("buttonToolTipId"));
#endif // BOBUI_CONFIG(tooltip)
#if BOBUI_CONFIG(statustip)
        pushButton->setStatusTip(bobuiTrId("buttonStatusTipId"));
#endif // BOBUI_CONFIG(statustip)
#if BOBUI_CONFIG(whatsthis)
        pushButton->setWhatsThis(bobuiTrId("buttonWhatsThisId"));
#endif // BOBUI_CONFIG(whatsthis)
        pushButton->setText(bobuiTrId("buttonTextId"));
    } // retranslateUi

};

namespace Ui {
    class Form: public Ui_Form {};
} // namespace Ui

BOBUI_END_NAMESPACE

#endif // IDBASED_H
