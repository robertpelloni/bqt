/********************************************************************************
** Form generated from reading UI file 'bobuitoolbardialog.ui'
**
** Created by: BobUI User Interface Compiler version 6.0.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef BOBUITOOLBARDIALOG_H
#define BOBUITOOLBARDIALOG_H

#include <BobUICore/QVariant>
#include <BobUIWidgets/QAbstractButton>
#include <BobUIWidgets/QApplication>
#include <BobUIWidgets/QDialog>
#include <BobUIWidgets/QDialogButtonBox>
#include <BobUIWidgets/QGridLayout>
#include <BobUIWidgets/QHBoxLayout>
#include <BobUIWidgets/QHeaderView>
#include <BobUIWidgets/QLabel>
#include <BobUIWidgets/QListWidget>
#include <BobUIWidgets/QSpacerItem>
#include <BobUIWidgets/BOBUIoolButton>
#include <BobUIWidgets/BOBUIreeWidget>
#include <BobUIWidgets/QVBoxLayout>

BOBUI_BEGIN_NAMESPACE

class Ui_BobUIToolBarDialog
{
public:
    QGridLayout *gridLayout;
    BOBUIreeWidget *actionTree;
    QLabel *label;
    QHBoxLayout *hboxLayout;
    QLabel *label_2;
    BOBUIoolButton *newButton;
    BOBUIoolButton *removeButton;
    BOBUIoolButton *renameButton;
    QVBoxLayout *vboxLayout;
    BOBUIoolButton *upButton;
    BOBUIoolButton *leftButton;
    BOBUIoolButton *rightButton;
    BOBUIoolButton *downButton;
    QSpacerItem *spacerItem;
    QListWidget *currentToolBarList;
    QLabel *label_3;
    QListWidget *toolBarList;
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *BobUIToolBarDialog)
    {
        if (BobUIToolBarDialog->objectName().isEmpty())
            BobUIToolBarDialog->setObjectName("BobUIToolBarDialog");
        BobUIToolBarDialog->resize(583, 508);
        gridLayout = new QGridLayout(BobUIToolBarDialog);
#ifndef Q_OS_MAC
        gridLayout->setSpacing(6);
#endif
        gridLayout->setContentsMargins(8, 8, 8, 8);
        gridLayout->setObjectName("gridLayout");
        actionTree = new BOBUIreeWidget(BobUIToolBarDialog);
        actionTree->setObjectName("actionTree");

        gridLayout->addWidget(actionTree, 1, 0, 3, 1);

        label = new QLabel(BobUIToolBarDialog);
        label->setObjectName("label");

        gridLayout->addWidget(label, 0, 0, 1, 1);

        hboxLayout = new QHBoxLayout();
#ifndef Q_OS_MAC
        hboxLayout->setSpacing(6);
#endif
        hboxLayout->setContentsMargins(0, 0, 0, 0);
        hboxLayout->setObjectName("hboxLayout");
        label_2 = new QLabel(BobUIToolBarDialog);
        label_2->setObjectName("label_2");

        hboxLayout->addWidget(label_2);

        newButton = new BOBUIoolButton(BobUIToolBarDialog);
        newButton->setObjectName("newButton");

        hboxLayout->addWidget(newButton);

        removeButton = new BOBUIoolButton(BobUIToolBarDialog);
        removeButton->setObjectName("removeButton");

        hboxLayout->addWidget(removeButton);

        renameButton = new BOBUIoolButton(BobUIToolBarDialog);
        renameButton->setObjectName("renameButton");

        hboxLayout->addWidget(renameButton);


        gridLayout->addLayout(hboxLayout, 0, 1, 1, 2);

        vboxLayout = new QVBoxLayout();
#ifndef Q_OS_MAC
        vboxLayout->setSpacing(6);
#endif
        vboxLayout->setContentsMargins(0, 0, 0, 0);
        vboxLayout->setObjectName("vboxLayout");
        upButton = new BOBUIoolButton(BobUIToolBarDialog);
        upButton->setObjectName("upButton");
        QSizePolicy sizePolicy(QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(upButton->sizePolicy().hasHeightForWidth());
        upButton->setSizePolicy(sizePolicy);

        vboxLayout->addWidget(upButton);

        leftButton = new BOBUIoolButton(BobUIToolBarDialog);
        leftButton->setObjectName("leftButton");
        sizePolicy.setHeightForWidth(leftButton->sizePolicy().hasHeightForWidth());
        leftButton->setSizePolicy(sizePolicy);

        vboxLayout->addWidget(leftButton);

        rightButton = new BOBUIoolButton(BobUIToolBarDialog);
        rightButton->setObjectName("rightButton");
        sizePolicy.setHeightForWidth(rightButton->sizePolicy().hasHeightForWidth());
        rightButton->setSizePolicy(sizePolicy);

        vboxLayout->addWidget(rightButton);

        downButton = new BOBUIoolButton(BobUIToolBarDialog);
        downButton->setObjectName("downButton");
        sizePolicy.setHeightForWidth(downButton->sizePolicy().hasHeightForWidth());
        downButton->setSizePolicy(sizePolicy);

        vboxLayout->addWidget(downButton);

        spacerItem = new QSpacerItem(29, 16, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        vboxLayout->addItem(spacerItem);


        gridLayout->addLayout(vboxLayout, 3, 1, 1, 1);

        currentToolBarList = new QListWidget(BobUIToolBarDialog);
        currentToolBarList->setObjectName("currentToolBarList");

        gridLayout->addWidget(currentToolBarList, 3, 2, 1, 1);

        label_3 = new QLabel(BobUIToolBarDialog);
        label_3->setObjectName("label_3");

        gridLayout->addWidget(label_3, 2, 1, 1, 2);

        toolBarList = new QListWidget(BobUIToolBarDialog);
        toolBarList->setObjectName("toolBarList");

        gridLayout->addWidget(toolBarList, 1, 1, 1, 2);

        buttonBox = new QDialogButtonBox(BobUIToolBarDialog);
        buttonBox->setObjectName("buttonBox");
        buttonBox->setStandardButtons(QDialogButtonBox::StandardButton::Apply|QDialogButtonBox::StandardButton::Cancel|QDialogButtonBox::StandardButton::Ok|QDialogButtonBox::StandardButton::RestoreDefaults);

        gridLayout->addWidget(buttonBox, 5, 0, 1, 3);

        QWidget::setTabOrder(newButton, removeButton);
        QWidget::setTabOrder(removeButton, renameButton);
        QWidget::setTabOrder(renameButton, toolBarList);
        QWidget::setTabOrder(toolBarList, upButton);
        QWidget::setTabOrder(upButton, leftButton);
        QWidget::setTabOrder(leftButton, rightButton);
        QWidget::setTabOrder(rightButton, downButton);
        QWidget::setTabOrder(downButton, currentToolBarList);

        retranslateUi(BobUIToolBarDialog);

        QMetaObject::connectSlotsByName(BobUIToolBarDialog);
    } // setupUi

    void retranslateUi(QDialog *BobUIToolBarDialog)
    {
        BobUIToolBarDialog->setWindowTitle(QCoreApplication::translate("BobUIToolBarDialog", "Customize Toolbars", nullptr));
        BOBUIreeWidgetItem *___bobuireewidgetitem = actionTree->headerItem();
        ___bobuireewidgetitem->setText(0, QCoreApplication::translate("BobUIToolBarDialog", "1", nullptr));
        label->setText(QCoreApplication::translate("BobUIToolBarDialog", "Actions", nullptr));
        label_2->setText(QCoreApplication::translate("BobUIToolBarDialog", "Toolbars", nullptr));
#if BOBUI_CONFIG(tooltip)
        newButton->setToolTip(QCoreApplication::translate("BobUIToolBarDialog", "Add new toolbar", nullptr));
#endif // BOBUI_CONFIG(tooltip)
        newButton->setText(QCoreApplication::translate("BobUIToolBarDialog", "New", nullptr));
#if BOBUI_CONFIG(tooltip)
        removeButton->setToolTip(QCoreApplication::translate("BobUIToolBarDialog", "Remove selected toolbar", nullptr));
#endif // BOBUI_CONFIG(tooltip)
        removeButton->setText(QCoreApplication::translate("BobUIToolBarDialog", "Remove", nullptr));
#if BOBUI_CONFIG(tooltip)
        renameButton->setToolTip(QCoreApplication::translate("BobUIToolBarDialog", "Rename toolbar", nullptr));
#endif // BOBUI_CONFIG(tooltip)
        renameButton->setText(QCoreApplication::translate("BobUIToolBarDialog", "Rename", nullptr));
#if BOBUI_CONFIG(tooltip)
        upButton->setToolTip(QCoreApplication::translate("BobUIToolBarDialog", "Move action up", nullptr));
#endif // BOBUI_CONFIG(tooltip)
        upButton->setText(QCoreApplication::translate("BobUIToolBarDialog", "Up", nullptr));
#if BOBUI_CONFIG(tooltip)
        leftButton->setToolTip(QCoreApplication::translate("BobUIToolBarDialog", "Remove action from toolbar", nullptr));
#endif // BOBUI_CONFIG(tooltip)
        leftButton->setText(QCoreApplication::translate("BobUIToolBarDialog", "<-", nullptr));
#if BOBUI_CONFIG(tooltip)
        rightButton->setToolTip(QCoreApplication::translate("BobUIToolBarDialog", "Add action to toolbar", nullptr));
#endif // BOBUI_CONFIG(tooltip)
        rightButton->setText(QCoreApplication::translate("BobUIToolBarDialog", "->", nullptr));
#if BOBUI_CONFIG(tooltip)
        downButton->setToolTip(QCoreApplication::translate("BobUIToolBarDialog", "Move action down", nullptr));
#endif // BOBUI_CONFIG(tooltip)
        downButton->setText(QCoreApplication::translate("BobUIToolBarDialog", "Down", nullptr));
        label_3->setText(QCoreApplication::translate("BobUIToolBarDialog", "Current Toolbar Actions", nullptr));
    } // retranslateUi

};

namespace Ui {
    class BobUIToolBarDialog: public Ui_BobUIToolBarDialog {};
} // namespace Ui

BOBUI_END_NAMESPACE

#endif // BOBUITOOLBARDIALOG_H
