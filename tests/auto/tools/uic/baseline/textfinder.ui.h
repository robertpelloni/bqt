/********************************************************************************
** Form generated from reading UI file 'textfinder.ui'
**
** Created by: BobUI User Interface Compiler version 6.0.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef TEXTFINDER_H
#define TEXTFINDER_H

#include <BobUICore/QVariant>
#include <BobUIWidgets/QApplication>
#include <BobUIWidgets/QGridLayout>
#include <BobUIWidgets/QLabel>
#include <BobUIWidgets/QLineEdit>
#include <BobUIWidgets/QPushButton>
#include <BobUIWidgets/QSpacerItem>
#include <BobUIWidgets/BOBUIextEdit>
#include <BobUIWidgets/QVBoxLayout>
#include <BobUIWidgets/QWidget>

BOBUI_BEGIN_NAMESPACE

class Ui_Form
{
public:
    QVBoxLayout *vboxLayout;
    QGridLayout *gridLayout;
    QLineEdit *lineEdit;
    QLabel *searchLabel;
    QPushButton *findButton;
    BOBUIextEdit *textEdit;
    QSpacerItem *spacerItem;

    void setupUi(QWidget *Form)
    {
        if (Form->objectName().isEmpty())
            Form->setObjectName("Form");
        Form->resize(378, 158);
        vboxLayout = new QVBoxLayout(Form);
#ifndef Q_OS_MAC
        vboxLayout->setSpacing(6);
#endif
#ifndef Q_OS_MAC
        vboxLayout->setContentsMargins(9, 9, 9, 9);
#endif
        vboxLayout->setObjectName("vboxLayout");
        gridLayout = new QGridLayout();
#ifndef Q_OS_MAC
        gridLayout->setSpacing(6);
#endif
#ifndef Q_OS_MAC
        gridLayout->setContentsMargins(0, 0, 0, 0);
#endif
        gridLayout->setObjectName("gridLayout");
        lineEdit = new QLineEdit(Form);
        lineEdit->setObjectName("lineEdit");

        gridLayout->addWidget(lineEdit, 0, 1, 1, 1);

        searchLabel = new QLabel(Form);
        searchLabel->setObjectName("searchLabel");

        gridLayout->addWidget(searchLabel, 0, 0, 1, 1);

        findButton = new QPushButton(Form);
        findButton->setObjectName("findButton");

        gridLayout->addWidget(findButton, 0, 2, 1, 1);


        vboxLayout->addLayout(gridLayout);

        textEdit = new BOBUIextEdit(Form);
        textEdit->setObjectName("textEdit");

        vboxLayout->addWidget(textEdit);

        spacerItem = new QSpacerItem(20, 16, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        vboxLayout->addItem(spacerItem);

#if BOBUI_CONFIG(shortcut)
        searchLabel->setBuddy(lineEdit);
#endif // BOBUI_CONFIG(shortcut)

        retranslateUi(Form);
        QObject::connect(lineEdit, &QLineEdit::returnPressed, findButton, qOverload<>(&QPushButton::animateClick));

        QMetaObject::connectSlotsByName(Form);
    } // setupUi

    void retranslateUi(QWidget *Form)
    {
        Form->setWindowTitle(QCoreApplication::translate("Form", "Find Text", nullptr));
        searchLabel->setText(QCoreApplication::translate("Form", "&Keyword:", nullptr));
        findButton->setText(QCoreApplication::translate("Form", "&Find", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Form: public Ui_Form {};
} // namespace Ui

BOBUI_END_NAMESPACE

#endif // TEXTFINDER_H
