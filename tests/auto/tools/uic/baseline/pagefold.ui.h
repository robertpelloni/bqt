/********************************************************************************
** Form generated from reading UI file 'pagefold.ui'
**
** Created by: BobUI User Interface Compiler version 6.0.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef PAGEFOLD_H
#define PAGEFOLD_H

#include <BobUICore/QVariant>
#include <BobUIGui/QAction>
#include <BobUIWidgets/QAbstractButton>
#include <BobUIWidgets/QApplication>
#include <BobUIWidgets/QCheckBox>
#include <BobUIWidgets/QComboBox>
#include <BobUIWidgets/QDialogButtonBox>
#include <BobUIWidgets/QFrame>
#include <BobUIWidgets/QGridLayout>
#include <BobUIWidgets/QLabel>
#include <BobUIWidgets/QLineEdit>
#include <BobUIWidgets/QListWidget>
#include <BobUIWidgets/QMainWindow>
#include <BobUIWidgets/QMenu>
#include <BobUIWidgets/QMenuBar>
#include <BobUIWidgets/QRadioButton>
#include <BobUIWidgets/QSpacerItem>
#include <BobUIWidgets/QSpinBox>
#include <BobUIWidgets/QStatusBar>
#include <BobUIWidgets/QVBoxLayout>
#include <BobUIWidgets/QWidget>

BOBUI_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *exitAction;
    QAction *aboutBobUIAction;
    QAction *editStyleAction;
    QAction *aboutAction;
    QWidget *centralwidget;
    QVBoxLayout *vboxLayout;
    QFrame *mainFrame;
    QGridLayout *gridLayout;
    QComboBox *nameCombo;
    QSpacerItem *spacerItem;
    QRadioButton *femaleRadioButton;
    QLabel *genderLabel;
    QLabel *ageLabel;
    QRadioButton *maleRadioButton;
    QLabel *nameLabel;
    QLabel *passwordLabel;
    QSpinBox *ageSpinBox;
    QDialogButtonBox *buttonBox;
    QCheckBox *agreeCheckBox;
    QLineEdit *passwordEdit;
    QListWidget *professionList;
    QLabel *label;
    QComboBox *countryCombo;
    QLabel *countryLabel;
    QMenuBar *menubar;
    QMenu *menu_File;
    QMenu *menu_Help;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(392, 412);
        exitAction = new QAction(MainWindow);
        exitAction->setObjectName("exitAction");
        aboutBobUIAction = new QAction(MainWindow);
        aboutBobUIAction->setObjectName("aboutBobUIAction");
        editStyleAction = new QAction(MainWindow);
        editStyleAction->setObjectName("editStyleAction");
        aboutAction = new QAction(MainWindow);
        aboutAction->setObjectName("aboutAction");
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        vboxLayout = new QVBoxLayout(centralwidget);
#ifndef Q_OS_MAC
        vboxLayout->setSpacing(6);
#endif
#ifndef Q_OS_MAC
        vboxLayout->setContentsMargins(9, 9, 9, 9);
#endif
        vboxLayout->setObjectName("vboxLayout");
        mainFrame = new QFrame(centralwidget);
        mainFrame->setObjectName("mainFrame");
        mainFrame->setFrameShape(QFrame::Shape::StyledPanel);
        mainFrame->setFrameShadow(QFrame::Shadow::Raised);
        gridLayout = new QGridLayout(mainFrame);
#ifndef Q_OS_MAC
        gridLayout->setSpacing(6);
#endif
#ifndef Q_OS_MAC
        gridLayout->setContentsMargins(9, 9, 9, 9);
#endif
        gridLayout->setObjectName("gridLayout");
        nameCombo = new QComboBox(mainFrame);
        nameCombo->addItem(QString());
        nameCombo->addItem(QString());
        nameCombo->addItem(QString());
        nameCombo->addItem(QString());
        nameCombo->setObjectName("nameCombo");
        nameCombo->setEditable(true);

        gridLayout->addWidget(nameCombo, 0, 1, 1, 3);

        spacerItem = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        gridLayout->addItem(spacerItem, 1, 3, 1, 1);

        femaleRadioButton = new QRadioButton(mainFrame);
        femaleRadioButton->setObjectName("femaleRadioButton");

        gridLayout->addWidget(femaleRadioButton, 1, 2, 1, 1);

        genderLabel = new QLabel(mainFrame);
        genderLabel->setObjectName("genderLabel");

        gridLayout->addWidget(genderLabel, 1, 0, 1, 1);

        ageLabel = new QLabel(mainFrame);
        ageLabel->setObjectName("ageLabel");

        gridLayout->addWidget(ageLabel, 2, 0, 1, 1);

        maleRadioButton = new QRadioButton(mainFrame);
        maleRadioButton->setObjectName("maleRadioButton");

        gridLayout->addWidget(maleRadioButton, 1, 1, 1, 1);

        nameLabel = new QLabel(mainFrame);
        nameLabel->setObjectName("nameLabel");

        gridLayout->addWidget(nameLabel, 0, 0, 1, 1);

        passwordLabel = new QLabel(mainFrame);
        passwordLabel->setObjectName("passwordLabel");

        gridLayout->addWidget(passwordLabel, 3, 0, 1, 1);

        ageSpinBox = new QSpinBox(mainFrame);
        ageSpinBox->setObjectName("ageSpinBox");
        ageSpinBox->setMinimum(12);
        ageSpinBox->setValue(22);

        gridLayout->addWidget(ageSpinBox, 2, 1, 1, 3);

        buttonBox = new QDialogButtonBox(mainFrame);
        buttonBox->setObjectName("buttonBox");
        buttonBox->setOrientation(BobUI::Orientation::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::StandardButton::Cancel|QDialogButtonBox::StandardButton::Ok);

        gridLayout->addWidget(buttonBox, 7, 2, 1, 2);

        agreeCheckBox = new QCheckBox(mainFrame);
        agreeCheckBox->setObjectName("agreeCheckBox");

        gridLayout->addWidget(agreeCheckBox, 6, 0, 1, 4);

        passwordEdit = new QLineEdit(mainFrame);
        passwordEdit->setObjectName("passwordEdit");
        passwordEdit->setEchoMode(QLineEdit::EchoMode::Password);

        gridLayout->addWidget(passwordEdit, 3, 1, 1, 3);

        professionList = new QListWidget(mainFrame);
        new QListWidgetItem(professionList);
        new QListWidgetItem(professionList);
        new QListWidgetItem(professionList);
        professionList->setObjectName("professionList");

        gridLayout->addWidget(professionList, 5, 1, 1, 3);

        label = new QLabel(mainFrame);
        label->setObjectName("label");

        gridLayout->addWidget(label, 5, 0, 1, 1);

        countryCombo = new QComboBox(mainFrame);
        countryCombo->addItem(QString());
        countryCombo->addItem(QString());
        countryCombo->addItem(QString());
        countryCombo->addItem(QString());
        countryCombo->addItem(QString());
        countryCombo->addItem(QString());
        countryCombo->addItem(QString());
        countryCombo->setObjectName("countryCombo");

        gridLayout->addWidget(countryCombo, 4, 1, 1, 3);

        countryLabel = new QLabel(mainFrame);
        countryLabel->setObjectName("countryLabel");

        gridLayout->addWidget(countryLabel, 4, 0, 1, 1);


        vboxLayout->addWidget(mainFrame);

        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 392, 26));
        menu_File = new QMenu(menubar);
        menu_File->setObjectName("menu_File");
        menu_Help = new QMenu(menubar);
        menu_Help->setObjectName("menu_Help");
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);
#if BOBUI_CONFIG(shortcut)
        ageLabel->setBuddy(ageSpinBox);
        nameLabel->setBuddy(nameCombo);
        passwordLabel->setBuddy(passwordEdit);
        label->setBuddy(professionList);
        countryLabel->setBuddy(professionList);
#endif // BOBUI_CONFIG(shortcut)

        menubar->addAction(menu_File->menuAction());
        menubar->addAction(menu_Help->menuAction());
        menu_File->addAction(editStyleAction);
        menu_File->addSeparator();
        menu_File->addAction(exitAction);
        menu_Help->addAction(aboutAction);
        menu_Help->addSeparator();
        menu_Help->addAction(aboutBobUIAction);

        retranslateUi(MainWindow);

        nameCombo->setCurrentIndex(-1);
        professionList->setCurrentRow(0);
        countryCombo->setCurrentIndex(6);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        exitAction->setText(QCoreApplication::translate("MainWindow", "&Exit", nullptr));
        aboutBobUIAction->setText(QCoreApplication::translate("MainWindow", "About BobUI", nullptr));
        editStyleAction->setText(QCoreApplication::translate("MainWindow", "Edit &Style", nullptr));
        aboutAction->setText(QCoreApplication::translate("MainWindow", "About", nullptr));
        nameCombo->setItemText(0, QCoreApplication::translate("MainWindow", "Girish", nullptr));
        nameCombo->setItemText(1, QCoreApplication::translate("MainWindow", "Jasmin", nullptr));
        nameCombo->setItemText(2, QCoreApplication::translate("MainWindow", "Simon", nullptr));
        nameCombo->setItemText(3, QCoreApplication::translate("MainWindow", "Zack", nullptr));

#if BOBUI_CONFIG(tooltip)
        nameCombo->setToolTip(QCoreApplication::translate("MainWindow", "Specify your name", nullptr));
#endif // BOBUI_CONFIG(tooltip)
        femaleRadioButton->setStyleSheet(QCoreApplication::translate("MainWindow", "Check this if you are female", nullptr));
        femaleRadioButton->setText(QCoreApplication::translate("MainWindow", "&Female", nullptr));
        genderLabel->setText(QCoreApplication::translate("MainWindow", "Gender:", nullptr));
        ageLabel->setText(QCoreApplication::translate("MainWindow", "&Age:", nullptr));
#if BOBUI_CONFIG(tooltip)
        maleRadioButton->setToolTip(QCoreApplication::translate("MainWindow", "Check this if you are male", nullptr));
#endif // BOBUI_CONFIG(tooltip)
        maleRadioButton->setText(QCoreApplication::translate("MainWindow", "&Male", nullptr));
        nameLabel->setText(QCoreApplication::translate("MainWindow", "&Name:", nullptr));
        passwordLabel->setText(QCoreApplication::translate("MainWindow", "&Password:", nullptr));
#if BOBUI_CONFIG(tooltip)
        ageSpinBox->setToolTip(QCoreApplication::translate("MainWindow", "Specify your age", nullptr));
#endif // BOBUI_CONFIG(tooltip)
#if BOBUI_CONFIG(statustip)
        ageSpinBox->setStatusTip(QCoreApplication::translate("MainWindow", "Specify your age", nullptr));
#endif // BOBUI_CONFIG(statustip)
#if BOBUI_CONFIG(tooltip)
        agreeCheckBox->setToolTip(QCoreApplication::translate("MainWindow", "Please read the LICENSE file before checking", nullptr));
#endif // BOBUI_CONFIG(tooltip)
        agreeCheckBox->setText(QCoreApplication::translate("MainWindow", "I &accept the terms and &conditions", nullptr));
#if BOBUI_CONFIG(tooltip)
        passwordEdit->setToolTip(QCoreApplication::translate("MainWindow", "Specify your password", nullptr));
#endif // BOBUI_CONFIG(tooltip)
#if BOBUI_CONFIG(statustip)
        passwordEdit->setStatusTip(QCoreApplication::translate("MainWindow", "Specify your password", nullptr));
#endif // BOBUI_CONFIG(statustip)
        passwordEdit->setText(QCoreApplication::translate("MainWindow", "Password", nullptr));

        const bool __sortingEnabled = professionList->isSortingEnabled();
        professionList->setSortingEnabled(false);
        QListWidgetItem *___qlistwidgetitem = professionList->item(0);
        ___qlistwidgetitem->setText(QCoreApplication::translate("MainWindow", "Developer", nullptr));
        QListWidgetItem *___qlistwidgetitem1 = professionList->item(1);
        ___qlistwidgetitem1->setText(QCoreApplication::translate("MainWindow", "Student", nullptr));
        QListWidgetItem *___qlistwidgetitem2 = professionList->item(2);
        ___qlistwidgetitem2->setText(QCoreApplication::translate("MainWindow", "Fisherman", nullptr));
        professionList->setSortingEnabled(__sortingEnabled);

#if BOBUI_CONFIG(tooltip)
        professionList->setToolTip(QCoreApplication::translate("MainWindow", "Select your profession", nullptr));
#endif // BOBUI_CONFIG(tooltip)
#if BOBUI_CONFIG(statustip)
        professionList->setStatusTip(QCoreApplication::translate("MainWindow", "Specify your name here", nullptr));
#endif // BOBUI_CONFIG(statustip)
#if BOBUI_CONFIG(whatsthis)
        professionList->setWhatsThis(QCoreApplication::translate("MainWindow", "Specify your name here", nullptr));
#endif // BOBUI_CONFIG(whatsthis)
        label->setText(QCoreApplication::translate("MainWindow", "Profession:", nullptr));
        countryCombo->setItemText(0, QCoreApplication::translate("MainWindow", "Egypt", nullptr));
        countryCombo->setItemText(1, QCoreApplication::translate("MainWindow", "France", nullptr));
        countryCombo->setItemText(2, QCoreApplication::translate("MainWindow", "Germany", nullptr));
        countryCombo->setItemText(3, QCoreApplication::translate("MainWindow", "India", nullptr));
        countryCombo->setItemText(4, QCoreApplication::translate("MainWindow", "Italy", nullptr));
        countryCombo->setItemText(5, QCoreApplication::translate("MainWindow", "Korea", nullptr));
        countryCombo->setItemText(6, QCoreApplication::translate("MainWindow", "Norway", nullptr));

#if BOBUI_CONFIG(tooltip)
        countryCombo->setToolTip(QCoreApplication::translate("MainWindow", "Specify country of origin", nullptr));
#endif // BOBUI_CONFIG(tooltip)
#if BOBUI_CONFIG(statustip)
        countryCombo->setStatusTip(QCoreApplication::translate("MainWindow", "Specify country of origin", nullptr));
#endif // BOBUI_CONFIG(statustip)
        countryLabel->setText(QCoreApplication::translate("MainWindow", "Pro&fession", nullptr));
        menu_File->setTitle(QCoreApplication::translate("MainWindow", "&File", nullptr));
        menu_Help->setTitle(QCoreApplication::translate("MainWindow", "&Help", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

BOBUI_END_NAMESPACE

#endif // PAGEFOLD_H
