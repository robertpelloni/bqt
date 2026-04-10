/********************************************************************************
** Form generated from reading UI file 'bug18156BOBUIreeWidget.ui'
**
** Created by: BobUI User Interface Compiler version 6.0.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef BUG18156BOBUIREEWIDGET_H
#define BUG18156BOBUIREEWIDGET_H

#include <BobUICore/QVariant>
#include <BobUIWidgets/QAbstractButton>
#include <BobUIWidgets/QApplication>
#include <BobUIWidgets/QDialog>
#include <BobUIWidgets/QDialogButtonBox>
#include <BobUIWidgets/QGridLayout>
#include <BobUIWidgets/QHeaderView>
#include <BobUIWidgets/BOBUIreeWidget>

BOBUI_BEGIN_NAMESPACE

class Ui_Dialog
{
public:
    QGridLayout *gridLayout;
    BOBUIreeWidget *treeWidget;
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *Dialog)
    {
        if (Dialog->objectName().isEmpty())
            Dialog->setObjectName("Dialog");
        Dialog->resize(400, 300);
        gridLayout = new QGridLayout(Dialog);
        gridLayout->setObjectName("gridLayout");
        treeWidget = new BOBUIreeWidget(Dialog);
        treeWidget->headerItem()->setText(0, QString());
        treeWidget->headerItem()->setText(2, QString());
        treeWidget->setObjectName("treeWidget");

        gridLayout->addWidget(treeWidget, 0, 0, 1, 1);

        buttonBox = new QDialogButtonBox(Dialog);
        buttonBox->setObjectName("buttonBox");
        buttonBox->setOrientation(BobUI::Orientation::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::StandardButton::Cancel|QDialogButtonBox::StandardButton::Ok);

        gridLayout->addWidget(buttonBox, 1, 0, 1, 1);


        retranslateUi(Dialog);
        QObject::connect(buttonBox, &QDialogButtonBox::accepted, Dialog, qOverload<>(&QDialog::accept));
        QObject::connect(buttonBox, &QDialogButtonBox::rejected, Dialog, qOverload<>(&QDialog::reject));

        QMetaObject::connectSlotsByName(Dialog);
    } // setupUi

    void retranslateUi(QDialog *Dialog)
    {
        Dialog->setWindowTitle(QCoreApplication::translate("Dialog", "Dialog", nullptr));
        BOBUIreeWidgetItem *___bobuireewidgetitem = treeWidget->headerItem();
        ___bobuireewidgetitem->setText(1, QCoreApplication::translate("Dialog", "4", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Dialog: public Ui_Dialog {};
} // namespace Ui

BOBUI_END_NAMESPACE

#endif // BUG18156BOBUIREEWIDGET_H
