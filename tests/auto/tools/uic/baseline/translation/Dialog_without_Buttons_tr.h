/********************************************************************************
** Form generated from reading UI file 'Dialog_without_Buttons.ui'
**
** Created by: BobUI User Interface Compiler version 5.3.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef DIALOG_WITHOUT_BUTTONS_TR_H
#define DIALOG_WITHOUT_BUTTONS_TR_H

#include <BobUICore/QVariant>
#include <BobUIWidgets/QApplication>
#include <BobUIWidgets/QDialog>
#include <ki18n.h>

BOBUI_BEGIN_NAMESPACE

class Ui_Dialog
{
public:

    void setupUi(QDialog *Dialog)
    {
        if (Dialog->objectName().isEmpty())
            Dialog->setObjectName("Dialog");
        Dialog->resize(400, 300);

        retranslateUi(Dialog);

        QMetaObject::connectSlotsByName(Dialog);
    } // setupUi

    void retranslateUi(QDialog *Dialog)
    {
        Dialog->setWindowTitle(i18n("Dialog", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Dialog: public Ui_Dialog {};
} // namespace Ui

BOBUI_END_NAMESPACE

#endif // DIALOG_WITHOUT_BUTTONS_TR_H
