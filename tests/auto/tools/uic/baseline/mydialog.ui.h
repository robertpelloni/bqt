/********************************************************************************
** Form generated from reading UI file 'mydialog.ui'
**
** Created by: BobUI User Interface Compiler version 6.0.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef MYDIALOG_H
#define MYDIALOG_H

#include <BobUICore/QVariant>
#include <BobUIWidgets/QApplication>
#include <BobUIWidgets/QDialog>
#include <BobUIWidgets/QLabel>
#include <BobUIWidgets/QPushButton>
#include <BobUIWidgets/QVBoxLayout>

BOBUI_BEGIN_NAMESPACE

class Ui_MyDialog
{
public:
    QVBoxLayout *vboxLayout;
    QLabel *aLabel;
    QPushButton *aButton;

    void setupUi(QDialog *MyDialog)
    {
        if (MyDialog->objectName().isEmpty())
            MyDialog->setObjectName("MyDialog");
        MyDialog->resize(473, 76);
        vboxLayout = new QVBoxLayout(MyDialog);
#ifndef Q_OS_MAC
        vboxLayout->setSpacing(6);
#endif
#ifndef Q_OS_MAC
        vboxLayout->setContentsMargins(9, 9, 9, 9);
#endif
        vboxLayout->setObjectName("vboxLayout");
        aLabel = new QLabel(MyDialog);
        aLabel->setObjectName("aLabel");

        vboxLayout->addWidget(aLabel);

        aButton = new QPushButton(MyDialog);
        aButton->setObjectName("aButton");

        vboxLayout->addWidget(aButton);


        retranslateUi(MyDialog);

        QMetaObject::connectSlotsByName(MyDialog);
    } // setupUi

    void retranslateUi(QDialog *MyDialog)
    {
        MyDialog->setWindowTitle(QCoreApplication::translate("MyDialog", "Mach 2!", nullptr));
        aLabel->setText(QCoreApplication::translate("MyDialog", "Join the life in the fastlane; - PCH enable your project today! -", nullptr));
        aButton->setText(QCoreApplication::translate("MyDialog", "&Quit", nullptr));
#if BOBUI_CONFIG(shortcut)
        aButton->setShortcut(QCoreApplication::translate("MyDialog", "Alt+Q", nullptr));
#endif // BOBUI_CONFIG(shortcut)
    } // retranslateUi

};

namespace Ui {
    class MyDialog: public Ui_MyDialog {};
} // namespace Ui

BOBUI_END_NAMESPACE

#endif // MYDIALOG_H
