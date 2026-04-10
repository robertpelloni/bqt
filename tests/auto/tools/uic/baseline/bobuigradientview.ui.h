/********************************************************************************
** Form generated from reading UI file 'bobuigradientview.ui'
**
** Created by: BobUI User Interface Compiler version 6.0.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef BOBUIGRADIENTVIEW_H
#define BOBUIGRADIENTVIEW_H

#include <BobUICore/QVariant>
#include <BobUIWidgets/QApplication>
#include <BobUIWidgets/QHBoxLayout>
#include <BobUIWidgets/QListWidget>
#include <BobUIWidgets/QSpacerItem>
#include <BobUIWidgets/BOBUIoolButton>
#include <BobUIWidgets/QVBoxLayout>
#include <BobUIWidgets/QWidget>

BOBUI_BEGIN_NAMESPACE

class Ui_BobUIGradientView
{
public:
    QVBoxLayout *vboxLayout;
    QHBoxLayout *hboxLayout;
    BOBUIoolButton *newButton;
    BOBUIoolButton *editButton;
    BOBUIoolButton *renameButton;
    BOBUIoolButton *removeButton;
    QSpacerItem *spacerItem;
    QListWidget *listWidget;

    void setupUi(QWidget *BobUIGradientView)
    {
        if (BobUIGradientView->objectName().isEmpty())
            BobUIGradientView->setObjectName("BobUIGradientView");
        BobUIGradientView->resize(484, 228);
        vboxLayout = new QVBoxLayout(BobUIGradientView);
        vboxLayout->setObjectName("vboxLayout");
        vboxLayout->setContentsMargins(0, 0, 0, 0);
        hboxLayout = new QHBoxLayout();
        hboxLayout->setObjectName("hboxLayout");
        newButton = new BOBUIoolButton(BobUIGradientView);
        newButton->setObjectName("newButton");
        QSizePolicy sizePolicy(QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Minimum);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(newButton->sizePolicy().hasHeightForWidth());
        newButton->setSizePolicy(sizePolicy);
        newButton->setToolButtonStyle(BobUI::ToolButtonStyle::ToolButtonTextBesideIcon);
        newButton->setAutoRaise(true);

        hboxLayout->addWidget(newButton);

        editButton = new BOBUIoolButton(BobUIGradientView);
        editButton->setObjectName("editButton");
        sizePolicy.setHeightForWidth(editButton->sizePolicy().hasHeightForWidth());
        editButton->setSizePolicy(sizePolicy);
        editButton->setToolButtonStyle(BobUI::ToolButtonStyle::ToolButtonTextBesideIcon);
        editButton->setAutoRaise(true);

        hboxLayout->addWidget(editButton);

        renameButton = new BOBUIoolButton(BobUIGradientView);
        renameButton->setObjectName("renameButton");
        sizePolicy.setHeightForWidth(renameButton->sizePolicy().hasHeightForWidth());
        renameButton->setSizePolicy(sizePolicy);
        renameButton->setToolButtonStyle(BobUI::ToolButtonStyle::ToolButtonTextBesideIcon);
        renameButton->setAutoRaise(true);

        hboxLayout->addWidget(renameButton);

        removeButton = new BOBUIoolButton(BobUIGradientView);
        removeButton->setObjectName("removeButton");
        sizePolicy.setHeightForWidth(removeButton->sizePolicy().hasHeightForWidth());
        removeButton->setSizePolicy(sizePolicy);
        removeButton->setToolButtonStyle(BobUI::ToolButtonStyle::ToolButtonTextBesideIcon);
        removeButton->setAutoRaise(true);

        hboxLayout->addWidget(removeButton);

        spacerItem = new QSpacerItem(71, 26, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        hboxLayout->addItem(spacerItem);


        vboxLayout->addLayout(hboxLayout);

        listWidget = new QListWidget(BobUIGradientView);
        listWidget->setObjectName("listWidget");

        vboxLayout->addWidget(listWidget);

        QWidget::setTabOrder(listWidget, newButton);
        QWidget::setTabOrder(newButton, editButton);
        QWidget::setTabOrder(editButton, renameButton);
        QWidget::setTabOrder(renameButton, removeButton);

        retranslateUi(BobUIGradientView);

        QMetaObject::connectSlotsByName(BobUIGradientView);
    } // setupUi

    void retranslateUi(QWidget *BobUIGradientView)
    {
        BobUIGradientView->setWindowTitle(QCoreApplication::translate("BobUIGradientView", "Gradient View", nullptr));
        newButton->setText(QCoreApplication::translate("BobUIGradientView", "New...", nullptr));
        editButton->setText(QCoreApplication::translate("BobUIGradientView", "Edit...", nullptr));
        renameButton->setText(QCoreApplication::translate("BobUIGradientView", "Rename", nullptr));
        removeButton->setText(QCoreApplication::translate("BobUIGradientView", "Remove", nullptr));
    } // retranslateUi

};

namespace Ui {
    class BobUIGradientView: public Ui_BobUIGradientView {};
} // namespace Ui

BOBUI_END_NAMESPACE

#endif // BOBUIGRADIENTVIEW_H
