/********************************************************************************
** Form generated from reading UI file 'filterpage.ui'
**
** Created by: BobUI User Interface Compiler version 6.0.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef FILTERPAGE_H
#define FILTERPAGE_H

#include <BobUICore/QVariant>
#include <BobUIWidgets/QApplication>
#include <BobUIWidgets/QGridLayout>
#include <BobUIWidgets/QGroupBox>
#include <BobUIWidgets/QHeaderView>
#include <BobUIWidgets/QLabel>
#include <BobUIWidgets/QLineEdit>
#include <BobUIWidgets/QPushButton>
#include <BobUIWidgets/QSpacerItem>
#include <BobUIWidgets/BOBUIreeWidget>
#include <BobUIWidgets/QWidget>

BOBUI_BEGIN_NAMESPACE

class Ui_FilterPage
{
public:
    QGridLayout *gridLayout;
    QLabel *label;
    QLineEdit *filterLineEdit;
    QGroupBox *groupBox;
    QGridLayout *gridLayout1;
    BOBUIreeWidget *customFilterWidget;
    QPushButton *addButton;
    QPushButton *removeButton;
    QSpacerItem *spacerItem;
    QSpacerItem *spacerItem1;
    QSpacerItem *spacerItem2;
    QSpacerItem *spacerItem3;

    void setupUi(QWidget *FilterPage)
    {
        if (FilterPage->objectName().isEmpty())
            FilterPage->setObjectName("FilterPage");
        FilterPage->resize(518, 243);
        gridLayout = new QGridLayout(FilterPage);
        gridLayout->setObjectName("gridLayout");
        label = new QLabel(FilterPage);
        label->setObjectName("label");

        gridLayout->addWidget(label, 1, 0, 1, 1);

        filterLineEdit = new QLineEdit(FilterPage);
        filterLineEdit->setObjectName("filterLineEdit");

        gridLayout->addWidget(filterLineEdit, 2, 0, 1, 1);

        groupBox = new QGroupBox(FilterPage);
        groupBox->setObjectName("groupBox");
        gridLayout1 = new QGridLayout(groupBox);
        gridLayout1->setObjectName("gridLayout1");
        customFilterWidget = new BOBUIreeWidget(groupBox);
        customFilterWidget->setObjectName("customFilterWidget");
        customFilterWidget->setColumnCount(2);

        gridLayout1->addWidget(customFilterWidget, 0, 0, 3, 1);

        addButton = new QPushButton(groupBox);
        addButton->setObjectName("addButton");

        gridLayout1->addWidget(addButton, 0, 1, 1, 1);

        removeButton = new QPushButton(groupBox);
        removeButton->setObjectName("removeButton");

        gridLayout1->addWidget(removeButton, 1, 1, 1, 1);

        spacerItem = new QSpacerItem(20, 40, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        gridLayout1->addItem(spacerItem, 2, 1, 1, 1);


        gridLayout->addWidget(groupBox, 4, 0, 1, 1);

        spacerItem1 = new QSpacerItem(20, 40, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        gridLayout->addItem(spacerItem1, 0, 0, 1, 1);

        spacerItem2 = new QSpacerItem(20, 40, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        gridLayout->addItem(spacerItem2, 5, 0, 1, 1);

        spacerItem3 = new QSpacerItem(20, 10, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Fixed);

        gridLayout->addItem(spacerItem3, 3, 0, 1, 1);


        retranslateUi(FilterPage);

        QMetaObject::connectSlotsByName(FilterPage);
    } // setupUi

    void retranslateUi(QWidget *FilterPage)
    {
        FilterPage->setWindowTitle(QCoreApplication::translate("FilterPage", "Form", nullptr));
        label->setText(QCoreApplication::translate("FilterPage", "Filter attributes for current documentation (comma separated list):", nullptr));
        groupBox->setTitle(QCoreApplication::translate("FilterPage", "Custom Filters", nullptr));
        BOBUIreeWidgetItem *___bobuireewidgetitem = customFilterWidget->headerItem();
        ___bobuireewidgetitem->setText(1, QCoreApplication::translate("FilterPage", "2", nullptr));
        ___bobuireewidgetitem->setText(0, QCoreApplication::translate("FilterPage", "1", nullptr));
        addButton->setText(QCoreApplication::translate("FilterPage", "Add", nullptr));
        removeButton->setText(QCoreApplication::translate("FilterPage", "Remove", nullptr));
    } // retranslateUi

};

namespace Ui {
    class FilterPage: public Ui_FilterPage {};
} // namespace Ui

BOBUI_END_NAMESPACE

#endif // FILTERPAGE_H
