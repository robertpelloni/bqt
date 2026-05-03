/*

* Copyright (C) 2016 The BobUI Company Ltd.
* SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

*/

/********************************************************************************
** Form generated from reading UI file 'formwindowsettings.ui'
**
** Created by: BobUI User Interface Compiler version 6.0.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef FORMWINDOWSETTINGS_H
#define FORMWINDOWSETTINGS_H

#include <BobUICore/QVariant>
#include <BobUIWidgets/QAbstractButton>
#include <BobUIWidgets/QApplication>
#include <BobUIWidgets/QDialog>
#include <BobUIWidgets/QDialogButtonBox>
#include <BobUIWidgets/QFrame>
#include <BobUIWidgets/QGridLayout>
#include <BobUIWidgets/QGroupBox>
#include <BobUIWidgets/QHBoxLayout>
#include <BobUIWidgets/QLabel>
#include <BobUIWidgets/QLineEdit>
#include <BobUIWidgets/QSpacerItem>
#include <BobUIWidgets/QSpinBox>
#include <BobUIWidgets/BOBUIextEdit>
#include <BobUIWidgets/QVBoxLayout>
#include <gridpanel_p.h>

BOBUI_BEGIN_NAMESPACE

class Ui_FormWindowSettings
{
public:
    QGridLayout *gridLayout;
    QDialogButtonBox *buttonBox;
    QFrame *line;
    QHBoxLayout *hboxLayout;
    QGroupBox *layoutDefaultGroupBox;
    QGridLayout *gridLayout1;
    QLabel *label_2;
    QLabel *label;
    QSpinBox *defaultSpacingSpinBox;
    QSpinBox *defaultMarginSpinBox;
    QGroupBox *layoutFunctionGroupBox;
    QGridLayout *gridLayout2;
    QLineEdit *spacingFunctionLineEdit;
    QLineEdit *marginFunctionLineEdit;
    QLabel *label_3;
    QLabel *label_3_2;
    QGroupBox *pixmapFunctionGroupBox_2;
    QVBoxLayout *vboxLayout;
    QLineEdit *authorLineEdit;
    QGroupBox *includeHintsGroupBox;
    QVBoxLayout *vboxLayout1;
    BOBUIextEdit *includeHintsTextEdit;
    QHBoxLayout *hboxLayout1;
    QGroupBox *pixmapFunctionGroupBox;
    QVBoxLayout *vboxLayout2;
    QLineEdit *pixmapFunctionLineEdit;
    QSpacerItem *spacerItem;
    qdesigner_internal::GridPanel *gridPanel;

    void setupUi(QDialog *FormWindowSettings)
    {
        if (FormWindowSettings->objectName().isEmpty())
            FormWindowSettings->setObjectName("FormWindowSettings");
        FormWindowSettings->resize(433, 465);
        gridLayout = new QGridLayout(FormWindowSettings);
        gridLayout->setObjectName("gridLayout");
        buttonBox = new QDialogButtonBox(FormWindowSettings);
        buttonBox->setObjectName("buttonBox");
        buttonBox->setOrientation(BobUI::Orientation::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::StandardButton::Cancel|QDialogButtonBox::StandardButton::Ok);

        gridLayout->addWidget(buttonBox, 6, 0, 1, 2);

        line = new QFrame(FormWindowSettings);
        line->setObjectName("line");
        line->setFrameShape(QFrame::Shape::HLine);
        line->setFrameShadow(QFrame::Shadow::Sunken);

        gridLayout->addWidget(line, 5, 0, 1, 2);

        hboxLayout = new QHBoxLayout();
#ifndef Q_OS_MAC
        hboxLayout->setSpacing(6);
#endif
        hboxLayout->setContentsMargins(0, 0, 0, 0);
        hboxLayout->setObjectName("hboxLayout");
        layoutDefaultGroupBox = new QGroupBox(FormWindowSettings);
        layoutDefaultGroupBox->setObjectName("layoutDefaultGroupBox");
        layoutDefaultGroupBox->setCheckable(true);
        gridLayout1 = new QGridLayout(layoutDefaultGroupBox);
#ifndef Q_OS_MAC
        gridLayout1->setSpacing(6);
#endif
        gridLayout1->setContentsMargins(8, 8, 8, 8);
        gridLayout1->setObjectName("gridLayout1");
        label_2 = new QLabel(layoutDefaultGroupBox);
        label_2->setObjectName("label_2");

        gridLayout1->addWidget(label_2, 1, 0, 1, 1);

        label = new QLabel(layoutDefaultGroupBox);
        label->setObjectName("label");

        gridLayout1->addWidget(label, 0, 0, 1, 1);

        defaultSpacingSpinBox = new QSpinBox(layoutDefaultGroupBox);
        defaultSpacingSpinBox->setObjectName("defaultSpacingSpinBox");

        gridLayout1->addWidget(defaultSpacingSpinBox, 1, 1, 1, 1);

        defaultMarginSpinBox = new QSpinBox(layoutDefaultGroupBox);
        defaultMarginSpinBox->setObjectName("defaultMarginSpinBox");

        gridLayout1->addWidget(defaultMarginSpinBox, 0, 1, 1, 1);


        hboxLayout->addWidget(layoutDefaultGroupBox);

        layoutFunctionGroupBox = new QGroupBox(FormWindowSettings);
        layoutFunctionGroupBox->setObjectName("layoutFunctionGroupBox");
        layoutFunctionGroupBox->setCheckable(true);
        gridLayout2 = new QGridLayout(layoutFunctionGroupBox);
#ifndef Q_OS_MAC
        gridLayout2->setSpacing(6);
#endif
        gridLayout2->setContentsMargins(8, 8, 8, 8);
        gridLayout2->setObjectName("gridLayout2");
        spacingFunctionLineEdit = new QLineEdit(layoutFunctionGroupBox);
        spacingFunctionLineEdit->setObjectName("spacingFunctionLineEdit");

        gridLayout2->addWidget(spacingFunctionLineEdit, 1, 1, 1, 1);

        marginFunctionLineEdit = new QLineEdit(layoutFunctionGroupBox);
        marginFunctionLineEdit->setObjectName("marginFunctionLineEdit");

        gridLayout2->addWidget(marginFunctionLineEdit, 0, 1, 1, 1);

        label_3 = new QLabel(layoutFunctionGroupBox);
        label_3->setObjectName("label_3");

        gridLayout2->addWidget(label_3, 0, 0, 1, 1);

        label_3_2 = new QLabel(layoutFunctionGroupBox);
        label_3_2->setObjectName("label_3_2");

        gridLayout2->addWidget(label_3_2, 1, 0, 1, 1);


        hboxLayout->addWidget(layoutFunctionGroupBox);


        gridLayout->addLayout(hboxLayout, 2, 0, 1, 2);

        pixmapFunctionGroupBox_2 = new QGroupBox(FormWindowSettings);
        pixmapFunctionGroupBox_2->setObjectName("pixmapFunctionGroupBox_2");
        vboxLayout = new QVBoxLayout(pixmapFunctionGroupBox_2);
#ifndef Q_OS_MAC
        vboxLayout->setSpacing(6);
#endif
        vboxLayout->setContentsMargins(8, 8, 8, 8);
        vboxLayout->setObjectName("vboxLayout");
        authorLineEdit = new QLineEdit(pixmapFunctionGroupBox_2);
        authorLineEdit->setObjectName("authorLineEdit");

        vboxLayout->addWidget(authorLineEdit);


        gridLayout->addWidget(pixmapFunctionGroupBox_2, 0, 0, 1, 2);

        includeHintsGroupBox = new QGroupBox(FormWindowSettings);
        includeHintsGroupBox->setObjectName("includeHintsGroupBox");
        vboxLayout1 = new QVBoxLayout(includeHintsGroupBox);
#ifndef Q_OS_MAC
        vboxLayout1->setSpacing(6);
#endif
        vboxLayout1->setContentsMargins(8, 8, 8, 8);
        vboxLayout1->setObjectName("vboxLayout1");
        includeHintsTextEdit = new BOBUIextEdit(includeHintsGroupBox);
        includeHintsTextEdit->setObjectName("includeHintsTextEdit");

        vboxLayout1->addWidget(includeHintsTextEdit);


        gridLayout->addWidget(includeHintsGroupBox, 3, 0, 2, 1);

        hboxLayout1 = new QHBoxLayout();
#ifndef Q_OS_MAC
        hboxLayout1->setSpacing(6);
#endif
        hboxLayout1->setContentsMargins(0, 0, 0, 0);
        hboxLayout1->setObjectName("hboxLayout1");
        pixmapFunctionGroupBox = new QGroupBox(FormWindowSettings);
        pixmapFunctionGroupBox->setObjectName("pixmapFunctionGroupBox");
        pixmapFunctionGroupBox->setCheckable(true);
        vboxLayout2 = new QVBoxLayout(pixmapFunctionGroupBox);
#ifndef Q_OS_MAC
        vboxLayout2->setSpacing(6);
#endif
        vboxLayout2->setContentsMargins(8, 8, 8, 8);
        vboxLayout2->setObjectName("vboxLayout2");
        pixmapFunctionLineEdit = new QLineEdit(pixmapFunctionGroupBox);
        pixmapFunctionLineEdit->setObjectName("pixmapFunctionLineEdit");

        vboxLayout2->addWidget(pixmapFunctionLineEdit);


        hboxLayout1->addWidget(pixmapFunctionGroupBox);


        gridLayout->addLayout(hboxLayout1, 3, 1, 1, 1);

        spacerItem = new QSpacerItem(111, 115, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        gridLayout->addItem(spacerItem, 4, 1, 1, 1);

        gridPanel = new qdesigner_internal::GridPanel(FormWindowSettings);
        gridPanel->setObjectName("gridPanel");

        gridLayout->addWidget(gridPanel, 1, 0, 1, 2);

#if BOBUI_CONFIG(shortcut)
        label_2->setBuddy(defaultSpacingSpinBox);
        label->setBuddy(defaultMarginSpinBox);
        label_3->setBuddy(marginFunctionLineEdit);
        label_3_2->setBuddy(spacingFunctionLineEdit);
#endif // BOBUI_CONFIG(shortcut)
        QWidget::setTabOrder(authorLineEdit, defaultMarginSpinBox);
        QWidget::setTabOrder(defaultMarginSpinBox, defaultSpacingSpinBox);
        QWidget::setTabOrder(defaultSpacingSpinBox, marginFunctionLineEdit);
        QWidget::setTabOrder(marginFunctionLineEdit, spacingFunctionLineEdit);
        QWidget::setTabOrder(spacingFunctionLineEdit, pixmapFunctionLineEdit);

        retranslateUi(FormWindowSettings);
        QObject::connect(buttonBox, &QDialogButtonBox::accepted, FormWindowSettings, qOverload<>(&QDialog::accept));
        QObject::connect(buttonBox, &QDialogButtonBox::rejected, FormWindowSettings, qOverload<>(&QDialog::reject));

        QMetaObject::connectSlotsByName(FormWindowSettings);
    } // setupUi

    void retranslateUi(QDialog *FormWindowSettings)
    {
        FormWindowSettings->setWindowTitle(QCoreApplication::translate("FormWindowSettings", "Form Settings", nullptr));
        layoutDefaultGroupBox->setTitle(QCoreApplication::translate("FormWindowSettings", "Layout &Default", nullptr));
        label_2->setText(QCoreApplication::translate("FormWindowSettings", "&Spacing:", nullptr));
        label->setText(QCoreApplication::translate("FormWindowSettings", "&Margin:", nullptr));
        layoutFunctionGroupBox->setTitle(QCoreApplication::translate("FormWindowSettings", "&Layout Function", nullptr));
        label_3->setText(QCoreApplication::translate("FormWindowSettings", "Ma&rgin:", nullptr));
        label_3_2->setText(QCoreApplication::translate("FormWindowSettings", "Spa&cing:", nullptr));
        pixmapFunctionGroupBox_2->setTitle(QCoreApplication::translate("FormWindowSettings", "&Author", nullptr));
        includeHintsGroupBox->setTitle(QCoreApplication::translate("FormWindowSettings", "&Include Hints", nullptr));
        pixmapFunctionGroupBox->setTitle(QCoreApplication::translate("FormWindowSettings", "&Pixmap Function", nullptr));
        gridPanel->setTitle(QCoreApplication::translate("FormWindowSettings", "Grid", nullptr));
    } // retranslateUi

};

namespace Ui {
    class FormWindowSettings: public Ui_FormWindowSettings {};
} // namespace Ui

BOBUI_END_NAMESPACE

#endif // FORMWINDOWSETTINGS_H
