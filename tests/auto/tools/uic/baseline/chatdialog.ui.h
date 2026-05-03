/********************************************************************************
** Form generated from reading UI file 'chatdialog.ui'
**
** Created by: BobUI User Interface Compiler version 6.0.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef CHATDIALOG_H
#define CHATDIALOG_H

#include <BobUICore/QVariant>
#include <BobUIWidgets/QApplication>
#include <BobUIWidgets/QDialog>
#include <BobUIWidgets/QHBoxLayout>
#include <BobUIWidgets/QLabel>
#include <BobUIWidgets/QLineEdit>
#include <BobUIWidgets/QListWidget>
#include <BobUIWidgets/BOBUIextEdit>
#include <BobUIWidgets/QVBoxLayout>

BOBUI_BEGIN_NAMESPACE

class Ui_ChatDialog
{
public:
    QVBoxLayout *vboxLayout;
    QHBoxLayout *hboxLayout;
    BOBUIextEdit *textEdit;
    QListWidget *listWidget;
    QHBoxLayout *hboxLayout1;
    QLabel *label;
    QLineEdit *lineEdit;

    void setupUi(QDialog *ChatDialog)
    {
        if (ChatDialog->objectName().isEmpty())
            ChatDialog->setObjectName("ChatDialog");
        ChatDialog->resize(513, 349);
        vboxLayout = new QVBoxLayout(ChatDialog);
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
#ifndef Q_OS_MAC
        hboxLayout->setContentsMargins(0, 0, 0, 0);
#endif
        hboxLayout->setObjectName("hboxLayout");
        textEdit = new BOBUIextEdit(ChatDialog);
        textEdit->setObjectName("textEdit");
        textEdit->setFocusPolicy(BobUI::FocusPolicy::NoFocus);
        textEdit->setReadOnly(true);

        hboxLayout->addWidget(textEdit);

        listWidget = new QListWidget(ChatDialog);
        listWidget->setObjectName("listWidget");
        listWidget->setMaximumSize(QSize(180, 16777215));
        listWidget->setFocusPolicy(BobUI::FocusPolicy::NoFocus);

        hboxLayout->addWidget(listWidget);


        vboxLayout->addLayout(hboxLayout);

        hboxLayout1 = new QHBoxLayout();
#ifndef Q_OS_MAC
        hboxLayout1->setSpacing(6);
#endif
        hboxLayout1->setContentsMargins(0, 0, 0, 0);
        hboxLayout1->setObjectName("hboxLayout1");
        label = new QLabel(ChatDialog);
        label->setObjectName("label");

        hboxLayout1->addWidget(label);

        lineEdit = new QLineEdit(ChatDialog);
        lineEdit->setObjectName("lineEdit");

        hboxLayout1->addWidget(lineEdit);


        vboxLayout->addLayout(hboxLayout1);


        retranslateUi(ChatDialog);

        QMetaObject::connectSlotsByName(ChatDialog);
    } // setupUi

    void retranslateUi(QDialog *ChatDialog)
    {
        ChatDialog->setWindowTitle(QCoreApplication::translate("ChatDialog", "Chat", nullptr));
        label->setText(QCoreApplication::translate("ChatDialog", "Message:", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ChatDialog: public Ui_ChatDialog {};
} // namespace Ui

BOBUI_END_NAMESPACE

#endif // CHATDIALOG_H
