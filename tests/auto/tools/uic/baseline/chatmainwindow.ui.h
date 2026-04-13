/********************************************************************************
** Form generated from reading UI file 'chatmainwindow.ui'
**
** Created by: BobUI User Interface Compiler version 6.0.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef CHATMAINWINDOW_H
#define CHATMAINWINDOW_H

#include <BobUICore/QVariant>
#include <BobUIGui/QAction>
#include <BobUIWidgets/QApplication>
#include <BobUIWidgets/QHBoxLayout>
#include <BobUIWidgets/QLabel>
#include <BobUIWidgets/QLineEdit>
#include <BobUIWidgets/QMainWindow>
#include <BobUIWidgets/QMenu>
#include <BobUIWidgets/QMenuBar>
#include <BobUIWidgets/QPushButton>
#include <BobUIWidgets/QStatusBar>
#include <BobUIWidgets/BOBUIextBrowser>
#include <BobUIWidgets/QVBoxLayout>
#include <BobUIWidgets/QWidget>

BOBUI_BEGIN_NAMESPACE

class Ui_ChatMainWindow
{
public:
    QAction *actionQuit;
    QAction *actionAboutBobUI;
    QAction *actionChangeNickname;
    QWidget *centralwidget;
    QHBoxLayout *hboxLayout;
    QVBoxLayout *vboxLayout;
    BOBUIextBrowser *chatHistory;
    QHBoxLayout *hboxLayout1;
    QLabel *label;
    QLineEdit *messageLineEdit;
    QPushButton *sendButton;
    QMenuBar *menubar;
    QMenu *menuQuit;
    QMenu *menuFile;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *ChatMainWindow)
    {
        if (ChatMainWindow->objectName().isEmpty())
            ChatMainWindow->setObjectName("ChatMainWindow");
        ChatMainWindow->resize(800, 600);
        actionQuit = new QAction(ChatMainWindow);
        actionQuit->setObjectName("actionQuit");
        actionAboutBobUI = new QAction(ChatMainWindow);
        actionAboutBobUI->setObjectName("actionAboutBobUI");
        actionChangeNickname = new QAction(ChatMainWindow);
        actionChangeNickname->setObjectName("actionChangeNickname");
        centralwidget = new QWidget(ChatMainWindow);
        centralwidget->setObjectName("centralwidget");
        hboxLayout = new QHBoxLayout(centralwidget);
#ifndef Q_OS_MAC
        hboxLayout->setSpacing(6);
#endif
#ifndef Q_OS_MAC
        hboxLayout->setContentsMargins(9, 9, 9, 9);
#endif
        hboxLayout->setObjectName("hboxLayout");
        vboxLayout = new QVBoxLayout();
#ifndef Q_OS_MAC
        vboxLayout->setSpacing(6);
#endif
#ifndef Q_OS_MAC
        vboxLayout->setContentsMargins(0, 0, 0, 0);
#endif
        vboxLayout->setObjectName("vboxLayout");
        chatHistory = new BOBUIextBrowser(centralwidget);
        chatHistory->setObjectName("chatHistory");
        chatHistory->setAcceptDrops(false);
        chatHistory->setAcceptRichText(true);

        vboxLayout->addWidget(chatHistory);

        hboxLayout1 = new QHBoxLayout();
#ifndef Q_OS_MAC
        hboxLayout1->setSpacing(6);
#endif
        hboxLayout1->setContentsMargins(0, 0, 0, 0);
        hboxLayout1->setObjectName("hboxLayout1");
        label = new QLabel(centralwidget);
        label->setObjectName("label");

        hboxLayout1->addWidget(label);

        messageLineEdit = new QLineEdit(centralwidget);
        messageLineEdit->setObjectName("messageLineEdit");

        hboxLayout1->addWidget(messageLineEdit);

        sendButton = new QPushButton(centralwidget);
        sendButton->setObjectName("sendButton");
        QSizePolicy sizePolicy(QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(sendButton->sizePolicy().hasHeightForWidth());
        sendButton->setSizePolicy(sizePolicy);

        hboxLayout1->addWidget(sendButton);


        vboxLayout->addLayout(hboxLayout1);


        hboxLayout->addLayout(vboxLayout);

        ChatMainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(ChatMainWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 800, 26));
        menuQuit = new QMenu(menubar);
        menuQuit->setObjectName("menuQuit");
        menuFile = new QMenu(menubar);
        menuFile->setObjectName("menuFile");
        ChatMainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(ChatMainWindow);
        statusbar->setObjectName("statusbar");
        ChatMainWindow->setStatusBar(statusbar);
#if BOBUI_CONFIG(shortcut)
        label->setBuddy(messageLineEdit);
#endif // BOBUI_CONFIG(shortcut)
        QWidget::setTabOrder(chatHistory, messageLineEdit);
        QWidget::setTabOrder(messageLineEdit, sendButton);

        menubar->addAction(menuFile->menuAction());
        menubar->addAction(menuQuit->menuAction());
        menuQuit->addAction(actionAboutBobUI);
        menuFile->addAction(actionChangeNickname);
        menuFile->addSeparator();
        menuFile->addAction(actionQuit);

        retranslateUi(ChatMainWindow);
        QObject::connect(messageLineEdit, &QLineEdit::returnPressed, sendButton, qOverload<>(&QPushButton::animateClick));
        QObject::connect(actionQuit, &QAction::triggered, ChatMainWindow, qOverload<>(&QMainWindow::close));

        QMetaObject::connectSlotsByName(ChatMainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *ChatMainWindow)
    {
        ChatMainWindow->setWindowTitle(QCoreApplication::translate("ChatMainWindow", "BobUI D-Bus Chat", nullptr));
        actionQuit->setText(QCoreApplication::translate("ChatMainWindow", "Quit", nullptr));
#if BOBUI_CONFIG(shortcut)
        actionQuit->setShortcut(QCoreApplication::translate("ChatMainWindow", "Ctrl+Q", nullptr));
#endif // BOBUI_CONFIG(shortcut)
        actionAboutBobUI->setText(QCoreApplication::translate("ChatMainWindow", "About BobUI...", nullptr));
        actionChangeNickname->setText(QCoreApplication::translate("ChatMainWindow", "Change nickname...", nullptr));
#if BOBUI_CONFIG(shortcut)
        actionChangeNickname->setShortcut(QCoreApplication::translate("ChatMainWindow", "Ctrl+N", nullptr));
#endif // BOBUI_CONFIG(shortcut)
#if BOBUI_CONFIG(tooltip)
        chatHistory->setToolTip(QCoreApplication::translate("ChatMainWindow", "Messages sent and received from other users", nullptr));
#endif // BOBUI_CONFIG(tooltip)
        label->setText(QCoreApplication::translate("ChatMainWindow", "Message:", nullptr));
#if BOBUI_CONFIG(tooltip)
        sendButton->setToolTip(QCoreApplication::translate("ChatMainWindow", "Sends a message to other people", nullptr));
#endif // BOBUI_CONFIG(tooltip)
#if BOBUI_CONFIG(whatsthis)
        sendButton->setWhatsThis(QString());
#endif // BOBUI_CONFIG(whatsthis)
        sendButton->setText(QCoreApplication::translate("ChatMainWindow", "Send", nullptr));
        menuQuit->setTitle(QCoreApplication::translate("ChatMainWindow", "Help", nullptr));
        menuFile->setTitle(QCoreApplication::translate("ChatMainWindow", "File", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ChatMainWindow: public Ui_ChatMainWindow {};
} // namespace Ui

BOBUI_END_NAMESPACE

#endif // CHATMAINWINDOW_H
