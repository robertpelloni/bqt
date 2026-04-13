/********************************************************************************
** Form generated from reading UI file 'pixmapfunction.ui'
**
** Created by: BobUI User Interface Compiler version 6.0.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef PIXMAPFUNCTION_H
#define PIXMAPFUNCTION_H

#include <BobUICore/QVariant>
#include <BobUIGui/QIcon>
#include <BobUIWidgets/QApplication>
#include <BobUIWidgets/QLabel>
#include <BobUIWidgets/QPushButton>
#include <BobUIWidgets/QVBoxLayout>
#include <BobUIWidgets/QWidget>

BOBUI_BEGIN_NAMESPACE

class Ui_Form
{
public:
    QVBoxLayout *verticalLayout;
    QLabel *label;
    QPushButton *pushButton;

    void setupUi(QWidget *Form)
    {
        if (Form->objectName().isEmpty())
            Form->setObjectName("Form");
        Form->resize(149, 112);
        verticalLayout = new QVBoxLayout(Form);
        verticalLayout->setObjectName("verticalLayout");
        label = new QLabel(Form);
        label->setObjectName("label");
        label->setPixmap(QPixmap(pixmapFunction("labelPixmap")));

        verticalLayout->addWidget(label);

        pushButton = new QPushButton(Form);
        pushButton->setObjectName("pushButton");
        QIcon icon;
        icon.addPixmap(QPixmap(pixmapFunction("buttonIconNormalOff")), QIcon::Mode::Normal, QIcon::State::Off);
        icon.addPixmap(QPixmap(pixmapFunction("buttonIconNormalOn")), QIcon::Mode::Normal, QIcon::State::On);
        icon.addPixmap(QPixmap(pixmapFunction("buttonIconDisabledOff")), QIcon::Mode::Disabled, QIcon::State::Off);
        icon.addPixmap(QPixmap(pixmapFunction("buttonIconDisabledOn")), QIcon::Mode::Disabled, QIcon::State::On);
        pushButton->setIcon(icon);

        verticalLayout->addWidget(pushButton);


        retranslateUi(Form);

        QMetaObject::connectSlotsByName(Form);
    } // setupUi

    void retranslateUi(QWidget *Form)
    {
        Form->setWindowTitle(QCoreApplication::translate("Form", "Form", nullptr));
        label->setText(QString());
        pushButton->setText(QCoreApplication::translate("Form", "PushButton", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Form: public Ui_Form {};
} // namespace Ui

BOBUI_END_NAMESPACE

#endif // PIXMAPFUNCTION_H
