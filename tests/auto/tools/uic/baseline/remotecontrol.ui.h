/********************************************************************************
** Form generated from reading UI file 'remotecontrol.ui'
**
** Created by: BobUI User Interface Compiler version 6.0.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef REMOTECONTROL_H
#define REMOTECONTROL_H

#include <BobUICore/QVariant>
#include <BobUIGui/QAction>
#include <BobUIGui/QIcon>
#include <BobUIWidgets/QApplication>
#include <BobUIWidgets/QCheckBox>
#include <BobUIWidgets/QGridLayout>
#include <BobUIWidgets/QGroupBox>
#include <BobUIWidgets/QHBoxLayout>
#include <BobUIWidgets/QLabel>
#include <BobUIWidgets/QLineEdit>
#include <BobUIWidgets/QMainWindow>
#include <BobUIWidgets/QMenu>
#include <BobUIWidgets/QMenuBar>
#include <BobUIWidgets/QPushButton>
#include <BobUIWidgets/QSpacerItem>
#include <BobUIWidgets/QStatusBar>
#include <BobUIWidgets/BOBUIoolButton>
#include <BobUIWidgets/QWidget>

BOBUI_BEGIN_NAMESPACE

class Ui_RemoteControlClass
{
public:
    QAction *actionQuit;
    QWidget *centralWidget;
    QGridLayout *gridLayout;
    QLabel *label;
    QLineEdit *startUrlLineEdit;
    QPushButton *launchButton;
    QSpacerItem *spacerItem;
    QSpacerItem *spacerItem1;
    QGroupBox *actionGroupBox;
    QGridLayout *gridLayout1;
    QLabel *label_2;
    QHBoxLayout *hboxLayout;
    QLineEdit *indexLineEdit;
    BOBUIoolButton *indexButton;
    QLabel *label_4;
    QHBoxLayout *hboxLayout1;
    QLineEdit *identifierLineEdit;
    BOBUIoolButton *identifierButton;
    QLabel *label_3;
    QHBoxLayout *hboxLayout2;
    QLineEdit *urlLineEdit;
    BOBUIoolButton *urlButton;
    QPushButton *syncContentsButton;
    QSpacerItem *spacerItem2;
    QCheckBox *contentsCheckBox;
    QCheckBox *indexCheckBox;
    QCheckBox *bookmarksCheckBox;
    QMenuBar *menuBar;
    QMenu *menuFile;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *RemoteControlClass)
    {
        if (RemoteControlClass->objectName().isEmpty())
            RemoteControlClass->setObjectName("RemoteControlClass");
        RemoteControlClass->resize(344, 457);
        actionQuit = new QAction(RemoteControlClass);
        actionQuit->setObjectName("actionQuit");
        centralWidget = new QWidget(RemoteControlClass);
        centralWidget->setObjectName("centralWidget");
        gridLayout = new QGridLayout(centralWidget);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName("gridLayout");
        label = new QLabel(centralWidget);
        label->setObjectName("label");

        gridLayout->addWidget(label, 0, 0, 1, 1);

        startUrlLineEdit = new QLineEdit(centralWidget);
        startUrlLineEdit->setObjectName("startUrlLineEdit");

        gridLayout->addWidget(startUrlLineEdit, 0, 1, 1, 2);

        launchButton = new QPushButton(centralWidget);
        launchButton->setObjectName("launchButton");

        gridLayout->addWidget(launchButton, 1, 1, 1, 1);

        spacerItem = new QSpacerItem(101, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        gridLayout->addItem(spacerItem, 1, 2, 1, 1);

        spacerItem1 = new QSpacerItem(113, 16, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Fixed);

        gridLayout->addItem(spacerItem1, 2, 1, 1, 1);

        actionGroupBox = new QGroupBox(centralWidget);
        actionGroupBox->setObjectName("actionGroupBox");
        actionGroupBox->setEnabled(false);
        gridLayout1 = new QGridLayout(actionGroupBox);
        gridLayout1->setSpacing(6);
        gridLayout1->setContentsMargins(11, 11, 11, 11);
        gridLayout1->setObjectName("gridLayout1");
        label_2 = new QLabel(actionGroupBox);
        label_2->setObjectName("label_2");

        gridLayout1->addWidget(label_2, 0, 0, 1, 1);

        hboxLayout = new QHBoxLayout();
        hboxLayout->setSpacing(0);
        hboxLayout->setObjectName("hboxLayout");
        indexLineEdit = new QLineEdit(actionGroupBox);
        indexLineEdit->setObjectName("indexLineEdit");

        hboxLayout->addWidget(indexLineEdit);

        indexButton = new BOBUIoolButton(actionGroupBox);
        indexButton->setObjectName("indexButton");
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/remotecontrol/enter.png"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        indexButton->setIcon(icon);

        hboxLayout->addWidget(indexButton);


        gridLayout1->addLayout(hboxLayout, 0, 1, 1, 2);

        label_4 = new QLabel(actionGroupBox);
        label_4->setObjectName("label_4");

        gridLayout1->addWidget(label_4, 1, 0, 1, 1);

        hboxLayout1 = new QHBoxLayout();
        hboxLayout1->setSpacing(0);
        hboxLayout1->setObjectName("hboxLayout1");
        identifierLineEdit = new QLineEdit(actionGroupBox);
        identifierLineEdit->setObjectName("identifierLineEdit");

        hboxLayout1->addWidget(identifierLineEdit);

        identifierButton = new BOBUIoolButton(actionGroupBox);
        identifierButton->setObjectName("identifierButton");
        identifierButton->setIcon(icon);

        hboxLayout1->addWidget(identifierButton);


        gridLayout1->addLayout(hboxLayout1, 1, 1, 1, 2);

        label_3 = new QLabel(actionGroupBox);
        label_3->setObjectName("label_3");

        gridLayout1->addWidget(label_3, 2, 0, 1, 1);

        hboxLayout2 = new QHBoxLayout();
        hboxLayout2->setSpacing(0);
        hboxLayout2->setObjectName("hboxLayout2");
        urlLineEdit = new QLineEdit(actionGroupBox);
        urlLineEdit->setObjectName("urlLineEdit");

        hboxLayout2->addWidget(urlLineEdit);

        urlButton = new BOBUIoolButton(actionGroupBox);
        urlButton->setObjectName("urlButton");
        urlButton->setIcon(icon);

        hboxLayout2->addWidget(urlButton);


        gridLayout1->addLayout(hboxLayout2, 2, 1, 1, 2);

        syncContentsButton = new QPushButton(actionGroupBox);
        syncContentsButton->setObjectName("syncContentsButton");

        gridLayout1->addWidget(syncContentsButton, 3, 1, 1, 1);

        spacerItem2 = new QSpacerItem(81, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        gridLayout1->addItem(spacerItem2, 3, 2, 1, 1);

        contentsCheckBox = new QCheckBox(actionGroupBox);
        contentsCheckBox->setObjectName("contentsCheckBox");

        gridLayout1->addWidget(contentsCheckBox, 4, 0, 1, 3);

        indexCheckBox = new QCheckBox(actionGroupBox);
        indexCheckBox->setObjectName("indexCheckBox");

        gridLayout1->addWidget(indexCheckBox, 5, 0, 1, 1);

        bookmarksCheckBox = new QCheckBox(actionGroupBox);
        bookmarksCheckBox->setObjectName("bookmarksCheckBox");

        gridLayout1->addWidget(bookmarksCheckBox, 6, 0, 1, 3);


        gridLayout->addWidget(actionGroupBox, 3, 0, 1, 3);

        RemoteControlClass->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(RemoteControlClass);
        menuBar->setObjectName("menuBar");
        menuBar->setGeometry(QRect(0, 0, 344, 26));
        menuFile = new QMenu(menuBar);
        menuFile->setObjectName("menuFile");
        RemoteControlClass->setMenuBar(menuBar);
        statusBar = new QStatusBar(RemoteControlClass);
        statusBar->setObjectName("statusBar");
        RemoteControlClass->setStatusBar(statusBar);

        menuBar->addAction(menuFile->menuAction());
        menuFile->addAction(actionQuit);

        retranslateUi(RemoteControlClass);

        QMetaObject::connectSlotsByName(RemoteControlClass);
    } // setupUi

    void retranslateUi(QMainWindow *RemoteControlClass)
    {
        RemoteControlClass->setWindowTitle(QCoreApplication::translate("RemoteControlClass", "RemoteControl", nullptr));
        actionQuit->setText(QCoreApplication::translate("RemoteControlClass", "Quit", nullptr));
        label->setText(QCoreApplication::translate("RemoteControlClass", "Start URL:", nullptr));
        launchButton->setText(QCoreApplication::translate("RemoteControlClass", "Launch BobUI HelpViewer", nullptr));
        actionGroupBox->setTitle(QCoreApplication::translate("RemoteControlClass", "Actions", nullptr));
        label_2->setText(QCoreApplication::translate("RemoteControlClass", "Search in Index:", nullptr));
        indexButton->setText(QString());
        label_4->setText(QCoreApplication::translate("RemoteControlClass", "Identifier:", nullptr));
        identifierButton->setText(QString());
        label_3->setText(QCoreApplication::translate("RemoteControlClass", "Show URL:", nullptr));
        urlButton->setText(QString());
        syncContentsButton->setText(QCoreApplication::translate("RemoteControlClass", "Sync Contents", nullptr));
        contentsCheckBox->setText(QCoreApplication::translate("RemoteControlClass", "Show Contents", nullptr));
        indexCheckBox->setText(QCoreApplication::translate("RemoteControlClass", "Show Index", nullptr));
        bookmarksCheckBox->setText(QCoreApplication::translate("RemoteControlClass", "Show Bookmarks", nullptr));
        menuFile->setTitle(QCoreApplication::translate("RemoteControlClass", "File", nullptr));
    } // retranslateUi

};

namespace Ui {
    class RemoteControlClass: public Ui_RemoteControlClass {};
} // namespace Ui

BOBUI_END_NAMESPACE

#endif // REMOTECONTROL_H
