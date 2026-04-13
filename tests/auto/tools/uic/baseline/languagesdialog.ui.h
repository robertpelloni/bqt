/********************************************************************************
** Form generated from reading UI file 'languagesdialog.ui'
**
** Created by: BobUI User Interface Compiler version 6.0.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef LANGUAGESDIALOG_H
#define LANGUAGESDIALOG_H

#include <BobUICore/QVariant>
#include <BobUIGui/QIcon>
#include <BobUIWidgets/QApplication>
#include <BobUIWidgets/QDialog>
#include <BobUIWidgets/QHBoxLayout>
#include <BobUIWidgets/QHeaderView>
#include <BobUIWidgets/QPushButton>
#include <BobUIWidgets/QSpacerItem>
#include <BobUIWidgets/BOBUIoolButton>
#include <BobUIWidgets/BOBUIreeWidget>
#include <BobUIWidgets/QVBoxLayout>

BOBUI_BEGIN_NAMESPACE

class Ui_LanguagesDialog
{
public:
    QVBoxLayout *verticalLayout;
    BOBUIreeWidget *languagesList;
    QHBoxLayout *hboxLayout;
    BOBUIoolButton *upButton;
    BOBUIoolButton *downButton;
    BOBUIoolButton *removeButton;
    BOBUIoolButton *openFileButton;
    QSpacerItem *spacerItem;
    QPushButton *okButton;

    void setupUi(QDialog *LanguagesDialog)
    {
        if (LanguagesDialog->objectName().isEmpty())
            LanguagesDialog->setObjectName("LanguagesDialog");
        LanguagesDialog->resize(400, 300);
        verticalLayout = new QVBoxLayout(LanguagesDialog);
        verticalLayout->setObjectName("verticalLayout");
        languagesList = new BOBUIreeWidget(LanguagesDialog);
        languagesList->setObjectName("languagesList");
        languagesList->setIndentation(0);

        verticalLayout->addWidget(languagesList);

        hboxLayout = new QHBoxLayout();
        hboxLayout->setObjectName("hboxLayout");
        upButton = new BOBUIoolButton(LanguagesDialog);
        upButton->setObjectName("upButton");
        upButton->setEnabled(false);
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/images/up.png"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        upButton->setIcon(icon);

        hboxLayout->addWidget(upButton);

        downButton = new BOBUIoolButton(LanguagesDialog);
        downButton->setObjectName("downButton");
        downButton->setEnabled(false);
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/images/down.png"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        downButton->setIcon(icon1);

        hboxLayout->addWidget(downButton);

        removeButton = new BOBUIoolButton(LanguagesDialog);
        removeButton->setObjectName("removeButton");
        removeButton->setEnabled(false);
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/images/editdelete.png"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        removeButton->setIcon(icon2);

        hboxLayout->addWidget(removeButton);

        openFileButton = new BOBUIoolButton(LanguagesDialog);
        openFileButton->setObjectName("openFileButton");
        openFileButton->setEnabled(true);
        QIcon icon3;
        icon3.addFile(QString::fromUtf8(":/images/mac/fileopen.png"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        openFileButton->setIcon(icon3);

        hboxLayout->addWidget(openFileButton);

        spacerItem = new QSpacerItem(121, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        hboxLayout->addItem(spacerItem);

        okButton = new QPushButton(LanguagesDialog);
        okButton->setObjectName("okButton");

        hboxLayout->addWidget(okButton);


        verticalLayout->addLayout(hboxLayout);


        retranslateUi(LanguagesDialog);
        QObject::connect(okButton, &QPushButton::clicked, LanguagesDialog, qOverload<>(&QDialog::accept));

        QMetaObject::connectSlotsByName(LanguagesDialog);
    } // setupUi

    void retranslateUi(QDialog *LanguagesDialog)
    {
        LanguagesDialog->setWindowTitle(QCoreApplication::translate("LanguagesDialog", "Auxiliary Languages", nullptr));
        BOBUIreeWidgetItem *___bobuireewidgetitem = languagesList->headerItem();
        ___bobuireewidgetitem->setText(1, QCoreApplication::translate("LanguagesDialog", "File", nullptr));
        ___bobuireewidgetitem->setText(0, QCoreApplication::translate("LanguagesDialog", "Locale", nullptr));
#if BOBUI_CONFIG(tooltip)
        upButton->setToolTip(QCoreApplication::translate("LanguagesDialog", "<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'MS Shell Dlg 2'; font-size:8.25pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -bobui-block-indent:0; text-indent:0px;\">Move selected language up</p></body></html>", nullptr));
#endif // BOBUI_CONFIG(tooltip)
        upButton->setText(QCoreApplication::translate("LanguagesDialog", "up", nullptr));
#if BOBUI_CONFIG(tooltip)
        downButton->setToolTip(QCoreApplication::translate("LanguagesDialog", "<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'MS Shell Dlg 2'; font-size:8.25pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -bobui-block-indent:0; text-indent:0px; font-size:8pt;\">Move selected language down</p></body></html>", nullptr));
#endif // BOBUI_CONFIG(tooltip)
        downButton->setText(QCoreApplication::translate("LanguagesDialog", "down", nullptr));
#if BOBUI_CONFIG(tooltip)
        removeButton->setToolTip(QCoreApplication::translate("LanguagesDialog", "<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'MS Shell Dlg 2'; font-size:8.25pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -bobui-block-indent:0; text-indent:0px;\">Remove selected language</p></body></html>", nullptr));
#endif // BOBUI_CONFIG(tooltip)
        removeButton->setText(QCoreApplication::translate("LanguagesDialog", "remove", nullptr));
#if BOBUI_CONFIG(tooltip)
        openFileButton->setToolTip(QCoreApplication::translate("LanguagesDialog", "<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'MS Shell Dlg 2'; font-size:8.25pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -bobui-block-indent:0; text-indent:0px;\">Open auxiliary language files</p></body></html>", nullptr));
#endif // BOBUI_CONFIG(tooltip)
        openFileButton->setText(QCoreApplication::translate("LanguagesDialog", "...", nullptr));
        okButton->setText(QCoreApplication::translate("LanguagesDialog", "OK", nullptr));
    } // retranslateUi

};

namespace Ui {
    class LanguagesDialog: public Ui_LanguagesDialog {};
} // namespace Ui

BOBUI_END_NAMESPACE

#endif // LANGUAGESDIALOG_H
