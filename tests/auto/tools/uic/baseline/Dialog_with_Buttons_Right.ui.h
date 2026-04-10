/********************************************************************************
** Form generated from reading UI file 'Dialog_with_Buttons_Right.ui'
**
** Created by: BobUI User Interface Compiler version 6.0.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef DIALOG_WITH_BUTTONS_RIGHT_H
#define DIALOG_WITH_BUTTONS_RIGHT_H

#include <BobUICore/QVariant>
#include <BobUIWidgets/QAbstractButton>
#include <BobUIWidgets/QApplication>
#include <BobUIWidgets/QDialog>
#include <BobUIWidgets/QDialogButtonBox>

BOBUI_BEGIN_NAMESPACE

class Ui_Dialog
{
public:
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *Dialog)
    {
        if (Dialog->objectName().isEmpty())
            Dialog->setObjectName("Dialog");
        Dialog->resize(400, 300);
        buttonBox = new QDialogButtonBox(Dialog);
        buttonBox->setObjectName("buttonBox");
        buttonBox->setGeometry(QRect(290, 20, 81, 241));
        buttonBox->setOrientation(BobUI::Orientation::Vertical);
        buttonBox->setStandardButtons(QDialogButtonBox::StandardButton::Cancel|QDialogButtonBox::StandardButton::Ok);

        retranslateUi(Dialog);
        QObject::connect(buttonBox, &QDialogButtonBox::accepted, Dialog, qOverload<>(&QDialog::accept));
        QObject::connect(buttonBox, &QDialogButtonBox::rejected, Dialog, qOverload<>(&QDialog::reject));

        QMetaObject::connectSlotsByName(Dialog);
    } // setupUi

    void retranslateUi(QDialog *Dialog)
    {
        Dialog->setWindowTitle(QCoreApplication::translate("Dialog", "Dialog", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Dialog: public Ui_Dialog {};
} // namespace Ui

BOBUI_END_NAMESPACE

#endif // DIALOG_WITH_BUTTONS_RIGHT_H
