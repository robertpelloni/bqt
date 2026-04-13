/********************************************************************************
** Form generated from reading UI file 'addlinkdialog.ui'
**
** Created by: BobUI User Interface Compiler version 6.0.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef ADDLINKDIALOG_H
#define ADDLINKDIALOG_H

#include <BobUICore/QVariant>
#include <BobUIWidgets/QAbstractButton>
#include <BobUIWidgets/QApplication>
#include <BobUIWidgets/QDialog>
#include <BobUIWidgets/QDialogButtonBox>
#include <BobUIWidgets/QFormLayout>
#include <BobUIWidgets/QFrame>
#include <BobUIWidgets/QLabel>
#include <BobUIWidgets/QLineEdit>
#include <BobUIWidgets/QSpacerItem>
#include <BobUIWidgets/QVBoxLayout>

BOBUI_BEGIN_NAMESPACE

class Ui_AddLinkDialog
{
public:
    QVBoxLayout *verticalLayout;
    QFormLayout *formLayout;
    QLabel *label;
    QLineEdit *titleInput;
    QLabel *label_2;
    QLineEdit *urlInput;
    QSpacerItem *verticalSpacer;
    QFrame *line;
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *AddLinkDialog)
    {
        if (AddLinkDialog->objectName().isEmpty())
            AddLinkDialog->setObjectName("AddLinkDialog");
        AddLinkDialog->resize(400, 137);
        AddLinkDialog->setSizeGripEnabled(false);
        AddLinkDialog->setModal(true);
        verticalLayout = new QVBoxLayout(AddLinkDialog);
        verticalLayout->setObjectName("verticalLayout");
        formLayout = new QFormLayout();
        formLayout->setObjectName("formLayout");
        label = new QLabel(AddLinkDialog);
        label->setObjectName("label");

        formLayout->setWidget(0, QFormLayout::ItemRole::LabelRole, label);

        titleInput = new QLineEdit(AddLinkDialog);
        titleInput->setObjectName("titleInput");
        titleInput->setMinimumSize(QSize(337, 0));

        formLayout->setWidget(0, QFormLayout::ItemRole::FieldRole, titleInput);

        label_2 = new QLabel(AddLinkDialog);
        label_2->setObjectName("label_2");

        formLayout->setWidget(1, QFormLayout::ItemRole::LabelRole, label_2);

        urlInput = new QLineEdit(AddLinkDialog);
        urlInput->setObjectName("urlInput");

        formLayout->setWidget(1, QFormLayout::ItemRole::FieldRole, urlInput);


        verticalLayout->addLayout(formLayout);

        verticalSpacer = new QSpacerItem(0, 0, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        verticalLayout->addItem(verticalSpacer);

        line = new QFrame(AddLinkDialog);
        line->setObjectName("line");
        line->setFrameShape(QFrame::Shape::HLine);
        line->setFrameShadow(QFrame::Shadow::Sunken);

        verticalLayout->addWidget(line);

        buttonBox = new QDialogButtonBox(AddLinkDialog);
        buttonBox->setObjectName("buttonBox");
        buttonBox->setOrientation(BobUI::Orientation::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::StandardButton::Cancel|QDialogButtonBox::StandardButton::Ok);

        verticalLayout->addWidget(buttonBox);


        retranslateUi(AddLinkDialog);
        QObject::connect(buttonBox, &QDialogButtonBox::accepted, AddLinkDialog, qOverload<>(&QDialog::accept));
        QObject::connect(buttonBox, &QDialogButtonBox::rejected, AddLinkDialog, qOverload<>(&QDialog::reject));

        QMetaObject::connectSlotsByName(AddLinkDialog);
    } // setupUi

    void retranslateUi(QDialog *AddLinkDialog)
    {
        AddLinkDialog->setWindowTitle(QCoreApplication::translate("AddLinkDialog", "Insert Link", nullptr));
        label->setText(QCoreApplication::translate("AddLinkDialog", "Title:", nullptr));
        label_2->setText(QCoreApplication::translate("AddLinkDialog", "URL:", nullptr));
    } // retranslateUi

};

namespace Ui {
    class AddLinkDialog: public Ui_AddLinkDialog {};
} // namespace Ui

BOBUI_END_NAMESPACE

#endif // ADDLINKDIALOG_H
