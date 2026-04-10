/********************************************************************************
** Form generated from reading UI file 'previewconfigurationwidget.ui'
**
** Created by: BobUI User Interface Compiler version 6.0.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef PREVIEWCONFIGURATIONWIDGET_H
#define PREVIEWCONFIGURATIONWIDGET_H

#include <BobUICore/QVariant>
#include <BobUIWidgets/QApplication>
#include <BobUIWidgets/QComboBox>
#include <BobUIWidgets/QFormLayout>
#include <BobUIWidgets/QGroupBox>
#include <BobUIWidgets/QHBoxLayout>
#include <BobUIWidgets/QLabel>
#include <BobUIWidgets/BOBUIoolButton>
#include <textpropertyeditor_p.h>

BOBUI_BEGIN_NAMESPACE

class Ui_PreviewConfigurationWidget
{
public:
    QFormLayout *formLayout;
    QLabel *m_styleLabel;
    QComboBox *m_styleCombo;
    QLabel *m_appStyleSheetLabel;
    QHBoxLayout *hboxLayout;
    qdesigner_internal::TextPropertyEditor *m_appStyleSheetLineEdit;
    BOBUIoolButton *m_appStyleSheetChangeButton;
    BOBUIoolButton *m_appStyleSheetClearButton;
    QLabel *m_skinLabel;
    QHBoxLayout *hboxLayout1;
    QComboBox *m_skinCombo;
    BOBUIoolButton *m_skinRemoveButton;

    void setupUi(QGroupBox *PreviewConfigurationWidget)
    {
        if (PreviewConfigurationWidget->objectName().isEmpty())
            PreviewConfigurationWidget->setObjectName("PreviewConfigurationWidget");
        PreviewConfigurationWidget->resize(335, 155);
        PreviewConfigurationWidget->setCheckable(true);
        formLayout = new QFormLayout(PreviewConfigurationWidget);
        formLayout->setObjectName("formLayout");
        m_styleLabel = new QLabel(PreviewConfigurationWidget);
        m_styleLabel->setObjectName("m_styleLabel");

        formLayout->setWidget(0, QFormLayout::ItemRole::LabelRole, m_styleLabel);

        m_styleCombo = new QComboBox(PreviewConfigurationWidget);
        m_styleCombo->setObjectName("m_styleCombo");

        formLayout->setWidget(0, QFormLayout::ItemRole::FieldRole, m_styleCombo);

        m_appStyleSheetLabel = new QLabel(PreviewConfigurationWidget);
        m_appStyleSheetLabel->setObjectName("m_appStyleSheetLabel");

        formLayout->setWidget(1, QFormLayout::ItemRole::LabelRole, m_appStyleSheetLabel);

        hboxLayout = new QHBoxLayout();
        hboxLayout->setObjectName("hboxLayout");
        m_appStyleSheetLineEdit = new qdesigner_internal::TextPropertyEditor(PreviewConfigurationWidget);
        m_appStyleSheetLineEdit->setObjectName("m_appStyleSheetLineEdit");
        m_appStyleSheetLineEdit->setMinimumSize(QSize(149, 0));

        hboxLayout->addWidget(m_appStyleSheetLineEdit);

        m_appStyleSheetChangeButton = new BOBUIoolButton(PreviewConfigurationWidget);
        m_appStyleSheetChangeButton->setObjectName("m_appStyleSheetChangeButton");

        hboxLayout->addWidget(m_appStyleSheetChangeButton);

        m_appStyleSheetClearButton = new BOBUIoolButton(PreviewConfigurationWidget);
        m_appStyleSheetClearButton->setObjectName("m_appStyleSheetClearButton");

        hboxLayout->addWidget(m_appStyleSheetClearButton);


        formLayout->setLayout(1, QFormLayout::ItemRole::FieldRole, hboxLayout);

        m_skinLabel = new QLabel(PreviewConfigurationWidget);
        m_skinLabel->setObjectName("m_skinLabel");

        formLayout->setWidget(2, QFormLayout::ItemRole::LabelRole, m_skinLabel);

        hboxLayout1 = new QHBoxLayout();
        hboxLayout1->setObjectName("hboxLayout1");
        m_skinCombo = new QComboBox(PreviewConfigurationWidget);
        m_skinCombo->setObjectName("m_skinCombo");

        hboxLayout1->addWidget(m_skinCombo);

        m_skinRemoveButton = new BOBUIoolButton(PreviewConfigurationWidget);
        m_skinRemoveButton->setObjectName("m_skinRemoveButton");

        hboxLayout1->addWidget(m_skinRemoveButton);


        formLayout->setLayout(2, QFormLayout::ItemRole::FieldRole, hboxLayout1);


        retranslateUi(PreviewConfigurationWidget);

        QMetaObject::connectSlotsByName(PreviewConfigurationWidget);
    } // setupUi

    void retranslateUi(QGroupBox *PreviewConfigurationWidget)
    {
        PreviewConfigurationWidget->setWindowTitle(QCoreApplication::translate("PreviewConfigurationWidget", "Form", nullptr));
        PreviewConfigurationWidget->setTitle(QCoreApplication::translate("PreviewConfigurationWidget", "Print/Preview Configuration", nullptr));
        m_styleLabel->setText(QCoreApplication::translate("PreviewConfigurationWidget", "Style", nullptr));
        m_appStyleSheetLabel->setText(QCoreApplication::translate("PreviewConfigurationWidget", "Style sheet", nullptr));
        m_appStyleSheetChangeButton->setText(QCoreApplication::translate("PreviewConfigurationWidget", "...", nullptr));
        m_appStyleSheetClearButton->setText(QCoreApplication::translate("PreviewConfigurationWidget", "...", nullptr));
        m_skinLabel->setText(QCoreApplication::translate("PreviewConfigurationWidget", "Device skin", nullptr));
        m_skinRemoveButton->setText(QCoreApplication::translate("PreviewConfigurationWidget", "...", nullptr));
    } // retranslateUi

};

namespace Ui {
    class PreviewConfigurationWidget: public Ui_PreviewConfigurationWidget {};
} // namespace Ui

BOBUI_END_NAMESPACE

#endif // PREVIEWCONFIGURATIONWIDGET_H
