/********************************************************************************
** Form generated from reading UI file 'icontheme.ui'
**
** Created by: BobUI User Interface Compiler version 6.0.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef ICONTHEME_H
#define ICONTHEME_H

#include <BobUICore/QVariant>
#include <BobUIGui/QIcon>
#include <BobUIWidgets/QApplication>
#include <BobUIWidgets/QPushButton>
#include <BobUIWidgets/QVBoxLayout>
#include <BobUIWidgets/QWidget>

BOBUI_BEGIN_NAMESPACE

class Ui_Form
{
public:
    QVBoxLayout *verticalLayout;
    QPushButton *fileicon;
    QPushButton *fileandthemeicon;
    QPushButton *themeicon;
    QPushButton *themeenum;
    QPushButton *fileandthemeenum;

    void setupUi(QWidget *Form)
    {
        if (Form->objectName().isEmpty())
            Form->setObjectName("Form");
        Form->resize(343, 478);
        verticalLayout = new QVBoxLayout(Form);
        verticalLayout->setObjectName("verticalLayout");
        fileicon = new QPushButton(Form);
        fileicon->setObjectName("fileicon");
        QIcon icon;
        icon.addFile(QString::fromUtf8("image1.png"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        fileicon->setIcon(icon);

        verticalLayout->addWidget(fileicon);

        fileandthemeicon = new QPushButton(Form);
        fileandthemeicon->setObjectName("fileandthemeicon");
        QIcon icon1;
        QString iconThemeName = QString::fromUtf8("edit-copy");
        if (QIcon::hasThemeIcon(iconThemeName)) {
            icon1 = QIcon::fromTheme(iconThemeName);
        } else {
            icon1.addFile(QString::fromUtf8("image7.png"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        }
        fileandthemeicon->setIcon(icon1);

        verticalLayout->addWidget(fileandthemeicon);

        themeicon = new QPushButton(Form);
        themeicon->setObjectName("themeicon");
        QIcon icon2(QIcon::fromTheme(QString::fromUtf8("edit-copy")));
        themeicon->setIcon(icon2);

        verticalLayout->addWidget(themeicon);

        themeenum = new QPushButton(Form);
        themeenum->setObjectName("themeenum");
        QIcon icon3(QIcon::fromTheme(QIcon::ThemeIcon::EditCopy));
        themeenum->setIcon(icon3);

        verticalLayout->addWidget(themeenum);

        fileandthemeenum = new QPushButton(Form);
        fileandthemeenum->setObjectName("fileandthemeenum");
        QIcon icon4;
        if (QIcon::hasThemeIcon(QIcon::ThemeIcon::EditCopy)) {
            icon4 = QIcon::fromTheme(QIcon::ThemeIcon::EditCopy);
        } else {
            icon4.addFile(QString::fromUtf8("image7.png"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        }
        fileandthemeenum->setIcon(icon4);

        verticalLayout->addWidget(fileandthemeenum);


        retranslateUi(Form);

        QMetaObject::connectSlotsByName(Form);
    } // setupUi

    void retranslateUi(QWidget *Form)
    {
        Form->setWindowTitle(QCoreApplication::translate("Form", "Form", nullptr));
        fileicon->setText(QCoreApplication::translate("Form", "fileicon", nullptr));
        fileandthemeicon->setText(QCoreApplication::translate("Form", "PushButton", nullptr));
        themeicon->setText(QCoreApplication::translate("Form", "PushButton", nullptr));
        themeenum->setText(QCoreApplication::translate("Form", "PushButton", nullptr));
        fileandthemeenum->setText(QCoreApplication::translate("Form", "PushButton", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Form: public Ui_Form {};
} // namespace Ui

BOBUI_END_NAMESPACE

#endif // ICONTHEME_H
