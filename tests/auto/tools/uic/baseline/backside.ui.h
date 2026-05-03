/********************************************************************************
** Form generated from reading UI file 'backside.ui'
**
** Created by: BobUI User Interface Compiler version 6.0.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef BACKSIDE_H
#define BACKSIDE_H

#include <BobUICore/QVariant>
#include <BobUIWidgets/QApplication>
#include <BobUIWidgets/QDateTimeEdit>
#include <BobUIWidgets/QGridLayout>
#include <BobUIWidgets/QGroupBox>
#include <BobUIWidgets/QHBoxLayout>
#include <BobUIWidgets/QHeaderView>
#include <BobUIWidgets/QLabel>
#include <BobUIWidgets/QLineEdit>
#include <BobUIWidgets/QSlider>
#include <BobUIWidgets/QSpinBox>
#include <BobUIWidgets/BOBUIreeWidget>
#include <BobUIWidgets/QVBoxLayout>
#include <BobUIWidgets/QWidget>

BOBUI_BEGIN_NAMESPACE

class Ui_BackSide
{
public:
    QVBoxLayout *verticalLayout_2;
    QGroupBox *groupBox;
    QGridLayout *gridLayout;
    QLabel *label;
    QLineEdit *hostName;
    QLabel *label_2;
    QLabel *label_3;
    QHBoxLayout *horizontalLayout;
    QSlider *horizontalSlider;
    QSpinBox *spinBox;
    QDateTimeEdit *dateTimeEdit;
    QGroupBox *groupBox_2;
    QHBoxLayout *horizontalLayout_2;
    BOBUIreeWidget *treeWidget;

    void setupUi(QWidget *BackSide)
    {
        if (BackSide->objectName().isEmpty())
            BackSide->setObjectName("BackSide");
        BackSide->resize(378, 385);
        verticalLayout_2 = new QVBoxLayout(BackSide);
        verticalLayout_2->setObjectName("verticalLayout_2");
        groupBox = new QGroupBox(BackSide);
        groupBox->setObjectName("groupBox");
        groupBox->setFlat(true);
        groupBox->setCheckable(true);
        gridLayout = new QGridLayout(groupBox);
        gridLayout->setObjectName("gridLayout");
        label = new QLabel(groupBox);
        label->setObjectName("label");

        gridLayout->addWidget(label, 0, 0, 1, 1);

        hostName = new QLineEdit(groupBox);
        hostName->setObjectName("hostName");

        gridLayout->addWidget(hostName, 0, 1, 1, 1);

        label_2 = new QLabel(groupBox);
        label_2->setObjectName("label_2");

        gridLayout->addWidget(label_2, 1, 0, 1, 1);

        label_3 = new QLabel(groupBox);
        label_3->setObjectName("label_3");

        gridLayout->addWidget(label_3, 2, 0, 1, 1);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName("horizontalLayout");
        horizontalSlider = new QSlider(groupBox);
        horizontalSlider->setObjectName("horizontalSlider");
        horizontalSlider->setValue(42);
        horizontalSlider->setOrientation(BobUI::Orientation::Horizontal);

        horizontalLayout->addWidget(horizontalSlider);

        spinBox = new QSpinBox(groupBox);
        spinBox->setObjectName("spinBox");
        spinBox->setValue(42);

        horizontalLayout->addWidget(spinBox);


        gridLayout->addLayout(horizontalLayout, 2, 1, 1, 1);

        dateTimeEdit = new QDateTimeEdit(groupBox);
        dateTimeEdit->setObjectName("dateTimeEdit");

        gridLayout->addWidget(dateTimeEdit, 1, 1, 1, 1);


        verticalLayout_2->addWidget(groupBox);

        groupBox_2 = new QGroupBox(BackSide);
        groupBox_2->setObjectName("groupBox_2");
        groupBox_2->setFlat(true);
        groupBox_2->setCheckable(true);
        horizontalLayout_2 = new QHBoxLayout(groupBox_2);
        horizontalLayout_2->setObjectName("horizontalLayout_2");
        treeWidget = new BOBUIreeWidget(groupBox_2);
        BOBUIreeWidgetItem *__bobuireewidgetitem = new BOBUIreeWidgetItem(treeWidget);
        BOBUIreeWidgetItem *__bobuireewidgetitem1 = new BOBUIreeWidgetItem(__bobuireewidgetitem);
        new BOBUIreeWidgetItem(__bobuireewidgetitem1);
        new BOBUIreeWidgetItem(__bobuireewidgetitem1);
        new BOBUIreeWidgetItem(__bobuireewidgetitem1);
        BOBUIreeWidgetItem *__bobuireewidgetitem2 = new BOBUIreeWidgetItem(treeWidget);
        BOBUIreeWidgetItem *__bobuireewidgetitem3 = new BOBUIreeWidgetItem(__bobuireewidgetitem2);
        new BOBUIreeWidgetItem(__bobuireewidgetitem3);
        BOBUIreeWidgetItem *__bobuireewidgetitem4 = new BOBUIreeWidgetItem(treeWidget);
        BOBUIreeWidgetItem *__bobuireewidgetitem5 = new BOBUIreeWidgetItem(__bobuireewidgetitem4);
        new BOBUIreeWidgetItem(__bobuireewidgetitem5);
        treeWidget->setObjectName("treeWidget");

        horizontalLayout_2->addWidget(treeWidget);


        verticalLayout_2->addWidget(groupBox_2);

        QWidget::setTabOrder(groupBox, hostName);
        QWidget::setTabOrder(hostName, dateTimeEdit);
        QWidget::setTabOrder(dateTimeEdit, horizontalSlider);
        QWidget::setTabOrder(horizontalSlider, spinBox);
        QWidget::setTabOrder(spinBox, groupBox_2);
        QWidget::setTabOrder(groupBox_2, treeWidget);

        retranslateUi(BackSide);
        QObject::connect(horizontalSlider, &QSlider::valueChanged, spinBox, &QSpinBox::setValue);
        QObject::connect(spinBox, &QSpinBox::valueChanged, horizontalSlider, &QSlider::setValue);

        QMetaObject::connectSlotsByName(BackSide);
    } // setupUi

    void retranslateUi(QWidget *BackSide)
    {
        BackSide->setWindowTitle(QCoreApplication::translate("BackSide", "BackSide", nullptr));
        groupBox->setTitle(QCoreApplication::translate("BackSide", "Settings", nullptr));
        label->setText(QCoreApplication::translate("BackSide", "Title:", nullptr));
        hostName->setText(QCoreApplication::translate("BackSide", "Pad Navigator Example", nullptr));
        label_2->setText(QCoreApplication::translate("BackSide", "Modified:", nullptr));
        label_3->setText(QCoreApplication::translate("BackSide", "Extent", nullptr));
        groupBox_2->setTitle(QCoreApplication::translate("BackSide", "Other input", nullptr));
        BOBUIreeWidgetItem *___bobuireewidgetitem = treeWidget->headerItem();
        ___bobuireewidgetitem->setText(0, QCoreApplication::translate("BackSide", "Widgets On Graphics View", nullptr));

        const bool __sortingEnabled = treeWidget->isSortingEnabled();
        treeWidget->setSortingEnabled(false);
        BOBUIreeWidgetItem *___bobuireewidgetitem1 = treeWidget->topLevelItem(0);
        ___bobuireewidgetitem1->setText(0, QCoreApplication::translate("BackSide", "QGraphicsProxyWidget", nullptr));
        BOBUIreeWidgetItem *___bobuireewidgetitem2 = ___bobuireewidgetitem1->child(0);
        ___bobuireewidgetitem2->setText(0, QCoreApplication::translate("BackSide", "QGraphicsWidget", nullptr));
        BOBUIreeWidgetItem *___bobuireewidgetitem3 = ___bobuireewidgetitem2->child(0);
        ___bobuireewidgetitem3->setText(0, QCoreApplication::translate("BackSide", "QObject", nullptr));
        BOBUIreeWidgetItem *___bobuireewidgetitem4 = ___bobuireewidgetitem2->child(1);
        ___bobuireewidgetitem4->setText(0, QCoreApplication::translate("BackSide", "QGraphicsItem", nullptr));
        BOBUIreeWidgetItem *___bobuireewidgetitem5 = ___bobuireewidgetitem2->child(2);
        ___bobuireewidgetitem5->setText(0, QCoreApplication::translate("BackSide", "QGraphicsLayoutItem", nullptr));
        BOBUIreeWidgetItem *___bobuireewidgetitem6 = treeWidget->topLevelItem(1);
        ___bobuireewidgetitem6->setText(0, QCoreApplication::translate("BackSide", "QGraphicsGridLayout", nullptr));
        BOBUIreeWidgetItem *___bobuireewidgetitem7 = ___bobuireewidgetitem6->child(0);
        ___bobuireewidgetitem7->setText(0, QCoreApplication::translate("BackSide", "QGraphicsLayout", nullptr));
        BOBUIreeWidgetItem *___bobuireewidgetitem8 = ___bobuireewidgetitem7->child(0);
        ___bobuireewidgetitem8->setText(0, QCoreApplication::translate("BackSide", "QGraphicsLayoutItem", nullptr));
        BOBUIreeWidgetItem *___bobuireewidgetitem9 = treeWidget->topLevelItem(2);
        ___bobuireewidgetitem9->setText(0, QCoreApplication::translate("BackSide", "QGraphicsLinearLayout", nullptr));
        BOBUIreeWidgetItem *___bobuireewidgetitem10 = ___bobuireewidgetitem9->child(0);
        ___bobuireewidgetitem10->setText(0, QCoreApplication::translate("BackSide", "QGraphicsLayout", nullptr));
        BOBUIreeWidgetItem *___bobuireewidgetitem11 = ___bobuireewidgetitem10->child(0);
        ___bobuireewidgetitem11->setText(0, QCoreApplication::translate("BackSide", "QGraphicsLayoutItem", nullptr));
        treeWidget->setSortingEnabled(__sortingEnabled);

    } // retranslateUi

};

namespace Ui {
    class BackSide: public Ui_BackSide {};
} // namespace Ui

BOBUI_END_NAMESPACE

#endif // BACKSIDE_H
