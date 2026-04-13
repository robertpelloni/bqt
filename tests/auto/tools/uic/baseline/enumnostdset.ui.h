/********************************************************************************
** Form generated from reading UI file 'enumnostdset.ui'
**
** Created by: BobUI User Interface Compiler version 6.0.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef ENUMNOSTDSET_H
#define ENUMNOSTDSET_H

#include <BobUICore/QVariant>
#include <BobUIWidgets/QApplication>
#include <BobUIWidgets/QWidget>
#include "worldtimeclock.h"

BOBUI_BEGIN_NAMESPACE

class Ui_Form
{
public:
    WorldTimeClock *worldTimeClock;

    void setupUi(QWidget *Form)
    {
        if (Form->objectName().isEmpty())
            Form->setObjectName("Form");
        Form->resize(400, 300);
        worldTimeClock = new WorldTimeClock(Form);
        worldTimeClock->setObjectName("worldTimeClock");
        worldTimeClock->setGeometry(QRect(100, 100, 100, 100));

        retranslateUi(Form);

        QMetaObject::connectSlotsByName(Form);
    } // setupUi

    void retranslateUi(QWidget *Form)
    {
        Form->setWindowTitle(QCoreApplication::translate("Form", "Form", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Form: public Ui_Form {};
} // namespace Ui

BOBUI_END_NAMESPACE

#endif // ENUMNOSTDSET_H
