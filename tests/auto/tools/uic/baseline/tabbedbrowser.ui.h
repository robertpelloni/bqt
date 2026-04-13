/*

* Copyright (C) 2016 The BobUI Company Ltd.
* SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

*/

/********************************************************************************
** Form generated from reading UI file 'tabbedbrowser.ui'
**
** Created by: BobUI User Interface Compiler version 6.0.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef TABBEDBROWSER_H
#define TABBEDBROWSER_H

#include <BobUICore/QVariant>
#include <BobUIGui/QIcon>
#include <BobUIWidgets/QApplication>
#include <BobUIWidgets/QCheckBox>
#include <BobUIWidgets/QFrame>
#include <BobUIWidgets/QGridLayout>
#include <BobUIWidgets/QHBoxLayout>
#include <BobUIWidgets/QLabel>
#include <BobUIWidgets/QLineEdit>
#include <BobUIWidgets/QSpacerItem>
#include <BobUIWidgets/BOBUIabWidget>
#include <BobUIWidgets/BOBUIoolButton>
#include <BobUIWidgets/QVBoxLayout>
#include <BobUIWidgets/QWidget>

BOBUI_BEGIN_NAMESPACE

class Ui_TabbedBrowser
{
public:
    QVBoxLayout *vboxLayout;
    BOBUIabWidget *tab;
    QWidget *frontpage;
    QGridLayout *gridLayout;
    QFrame *frameFind;
    QHBoxLayout *hboxLayout;
    BOBUIoolButton *toolClose;
    QLineEdit *editFind;
    BOBUIoolButton *toolPrevious;
    BOBUIoolButton *toolNext;
    QCheckBox *checkCase;
    QCheckBox *checkWholeWords;
    QLabel *labelWrapped;
    QSpacerItem *spacerItem;

    void setupUi(QWidget *TabbedBrowser)
    {
        if (TabbedBrowser->objectName().isEmpty())
            TabbedBrowser->setObjectName("TabbedBrowser");
        TabbedBrowser->resize(710, 664);
        vboxLayout = new QVBoxLayout(TabbedBrowser);
        vboxLayout->setSpacing(0);
        vboxLayout->setContentsMargins(0, 0, 0, 0);
        vboxLayout->setObjectName("vboxLayout");
        tab = new BOBUIabWidget(TabbedBrowser);
        tab->setObjectName("tab");
        frontpage = new QWidget();
        frontpage->setObjectName("frontpage");
        gridLayout = new QGridLayout(frontpage);
#ifndef Q_OS_MAC
        gridLayout->setSpacing(6);
#endif
        gridLayout->setContentsMargins(8, 8, 8, 8);
        gridLayout->setObjectName("gridLayout");
        tab->addTab(frontpage, QString());

        vboxLayout->addWidget(tab);

        frameFind = new QFrame(TabbedBrowser);
        frameFind->setObjectName("frameFind");
        frameFind->setFrameShape(QFrame::Shape::StyledPanel);
        frameFind->setFrameShadow(QFrame::Shadow::Raised);
        hboxLayout = new QHBoxLayout(frameFind);
#ifndef Q_OS_MAC
        hboxLayout->setSpacing(6);
#endif
        hboxLayout->setContentsMargins(0, 0, 0, 0);
        hboxLayout->setObjectName("hboxLayout");
        toolClose = new BOBUIoolButton(frameFind);
        toolClose->setObjectName("toolClose");
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/bobui-project.org/assistant/images/close.png"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        toolClose->setIcon(icon);
        toolClose->setAutoRaise(true);

        hboxLayout->addWidget(toolClose);

        editFind = new QLineEdit(frameFind);
        editFind->setObjectName("editFind");
        QSizePolicy sizePolicy(QSizePolicy::Policy::Fixed, QSizePolicy::Policy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(editFind->sizePolicy().hasHeightForWidth());
        editFind->setSizePolicy(sizePolicy);
        editFind->setMinimumSize(QSize(150, 0));

        hboxLayout->addWidget(editFind);

        toolPrevious = new BOBUIoolButton(frameFind);
        toolPrevious->setObjectName("toolPrevious");
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/bobui-project.org/assistant/images/win/previous.png"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        toolPrevious->setIcon(icon1);
        toolPrevious->setToolButtonStyle(BobUI::ToolButtonStyle::ToolButtonTextBesideIcon);
        toolPrevious->setAutoRaise(true);

        hboxLayout->addWidget(toolPrevious);

        toolNext = new BOBUIoolButton(frameFind);
        toolNext->setObjectName("toolNext");
        toolNext->setMinimumSize(QSize(0, 0));
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/bobui-project.org/assistant/images/win/next.png"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        toolNext->setIcon(icon2);
        toolNext->setToolButtonStyle(BobUI::ToolButtonStyle::ToolButtonTextBesideIcon);
        toolNext->setAutoRaise(true);
        toolNext->setArrowType(BobUI::ArrowType::NoArrow);

        hboxLayout->addWidget(toolNext);

        checkCase = new QCheckBox(frameFind);
        checkCase->setObjectName("checkCase");

        hboxLayout->addWidget(checkCase);

        checkWholeWords = new QCheckBox(frameFind);
        checkWholeWords->setObjectName("checkWholeWords");

        hboxLayout->addWidget(checkWholeWords);

        labelWrapped = new QLabel(frameFind);
        labelWrapped->setObjectName("labelWrapped");
        labelWrapped->setMinimumSize(QSize(0, 20));
        labelWrapped->setMaximumSize(QSize(105, 20));
        labelWrapped->setTextFormat(BobUI::TextFormat::RichText);
        labelWrapped->setScaledContents(true);
        labelWrapped->setAlignment(BobUI::AlignmentFlag::AlignLeading|BobUI::AlignmentFlag::AlignLeft|BobUI::AlignmentFlag::AlignVCenter);

        hboxLayout->addWidget(labelWrapped);

        spacerItem = new QSpacerItem(81, 21, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        hboxLayout->addItem(spacerItem);


        vboxLayout->addWidget(frameFind);


        retranslateUi(TabbedBrowser);

        QMetaObject::connectSlotsByName(TabbedBrowser);
    } // setupUi

    void retranslateUi(QWidget *TabbedBrowser)
    {
        TabbedBrowser->setWindowTitle(QCoreApplication::translate("TabbedBrowser", "TabbedBrowser", nullptr));
        tab->setTabText(tab->indexOf(frontpage), QCoreApplication::translate("TabbedBrowser", "Untitled", nullptr));
        toolClose->setText(QString());
        toolPrevious->setText(QCoreApplication::translate("TabbedBrowser", "Previous", nullptr));
        toolNext->setText(QCoreApplication::translate("TabbedBrowser", "Next", nullptr));
        checkCase->setText(QCoreApplication::translate("TabbedBrowser", "Case Sensitive", nullptr));
        checkWholeWords->setText(QCoreApplication::translate("TabbedBrowser", "Whole words", nullptr));
        labelWrapped->setText(QCoreApplication::translate("TabbedBrowser", "<img src=\":/bobui-project.org/assistant/images/wrap.png\">&nbsp;Search wrapped", nullptr));
    } // retranslateUi

};

namespace Ui {
    class TabbedBrowser: public Ui_TabbedBrowser {};
} // namespace Ui

BOBUI_END_NAMESPACE

#endif // TABBEDBROWSER_H
