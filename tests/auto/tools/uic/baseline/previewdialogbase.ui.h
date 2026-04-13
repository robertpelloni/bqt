/********************************************************************************
** Form generated from reading UI file 'previewdialogbase.ui'
**
** Created by: BobUI User Interface Compiler version 6.0.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef PREVIEWDIALOGBASE_H
#define PREVIEWDIALOGBASE_H

#include <BobUICore/QVariant>
#include <BobUIWidgets/QAbstractButton>
#include <BobUIWidgets/QApplication>
#include <BobUIWidgets/QComboBox>
#include <BobUIWidgets/QDialog>
#include <BobUIWidgets/QDialogButtonBox>
#include <BobUIWidgets/QHBoxLayout>
#include <BobUIWidgets/QHeaderView>
#include <BobUIWidgets/QLabel>
#include <BobUIWidgets/QProgressBar>
#include <BobUIWidgets/QScrollArea>
#include <BobUIWidgets/QSpacerItem>
#include <BobUIWidgets/BOBUIreeWidget>
#include <BobUIWidgets/QVBoxLayout>

BOBUI_BEGIN_NAMESPACE

class Ui_PreviewDialogBase
{
public:
    QVBoxLayout *vboxLayout;
    QHBoxLayout *hboxLayout;
    QLabel *label;
    QComboBox *paperSizeCombo;
    QLabel *label_2;
    QComboBox *paperOrientationCombo;
    QSpacerItem *spacerItem;
    QHBoxLayout *hboxLayout1;
    BOBUIreeWidget *pageList;
    QScrollArea *previewArea;
    QHBoxLayout *hboxLayout2;
    QProgressBar *progressBar;
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *PreviewDialogBase)
    {
        if (PreviewDialogBase->objectName().isEmpty())
            PreviewDialogBase->setObjectName("PreviewDialogBase");
        PreviewDialogBase->resize(733, 479);
        vboxLayout = new QVBoxLayout(PreviewDialogBase);
#ifndef Q_OS_MAC
        vboxLayout->setSpacing(6);
#endif
#ifndef Q_OS_MAC
        vboxLayout->setContentsMargins(9, 9, 9, 9);
#endif
        vboxLayout->setObjectName("vboxLayout");
        hboxLayout = new QHBoxLayout();
#ifndef Q_OS_MAC
        hboxLayout->setSpacing(6);
#endif
#ifndef Q_OS_MAC
        hboxLayout->setContentsMargins(0, 0, 0, 0);
#endif
        hboxLayout->setObjectName("hboxLayout");
        label = new QLabel(PreviewDialogBase);
        label->setObjectName("label");

        hboxLayout->addWidget(label);

        paperSizeCombo = new QComboBox(PreviewDialogBase);
        paperSizeCombo->setObjectName("paperSizeCombo");
        QSizePolicy sizePolicy(QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(paperSizeCombo->sizePolicy().hasHeightForWidth());
        paperSizeCombo->setSizePolicy(sizePolicy);

        hboxLayout->addWidget(paperSizeCombo);

        label_2 = new QLabel(PreviewDialogBase);
        label_2->setObjectName("label_2");

        hboxLayout->addWidget(label_2);

        paperOrientationCombo = new QComboBox(PreviewDialogBase);
        paperOrientationCombo->setObjectName("paperOrientationCombo");
        sizePolicy.setHeightForWidth(paperOrientationCombo->sizePolicy().hasHeightForWidth());
        paperOrientationCombo->setSizePolicy(sizePolicy);

        hboxLayout->addWidget(paperOrientationCombo);

        spacerItem = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        hboxLayout->addItem(spacerItem);


        vboxLayout->addLayout(hboxLayout);

        hboxLayout1 = new QHBoxLayout();
#ifndef Q_OS_MAC
        hboxLayout1->setSpacing(6);
#endif
        hboxLayout1->setContentsMargins(0, 0, 0, 0);
        hboxLayout1->setObjectName("hboxLayout1");
        pageList = new BOBUIreeWidget(PreviewDialogBase);
        pageList->setObjectName("pageList");
        pageList->setIndentation(0);
        pageList->setRootIsDecorated(false);
        pageList->setUniformRowHeights(true);
        pageList->setItemsExpandable(false);
        pageList->setColumnCount(1);

        hboxLayout1->addWidget(pageList);

        previewArea = new QScrollArea(PreviewDialogBase);
        previewArea->setObjectName("previewArea");
        QSizePolicy sizePolicy1(QSizePolicy::Policy::Preferred, QSizePolicy::Policy::Preferred);
        sizePolicy1.setHorizontalStretch(1);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(previewArea->sizePolicy().hasHeightForWidth());
        previewArea->setSizePolicy(sizePolicy1);

        hboxLayout1->addWidget(previewArea);


        vboxLayout->addLayout(hboxLayout1);

        hboxLayout2 = new QHBoxLayout();
#ifndef Q_OS_MAC
        hboxLayout2->setSpacing(6);
#endif
        hboxLayout2->setContentsMargins(0, 0, 0, 0);
        hboxLayout2->setObjectName("hboxLayout2");
        progressBar = new QProgressBar(PreviewDialogBase);
        progressBar->setObjectName("progressBar");
        progressBar->setEnabled(false);
        QSizePolicy sizePolicy2(QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Fixed);
        sizePolicy2.setHorizontalStretch(1);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(progressBar->sizePolicy().hasHeightForWidth());
        progressBar->setSizePolicy(sizePolicy2);
        progressBar->setValue(0);
        progressBar->setTextVisible(false);
        progressBar->setOrientation(BobUI::Orientation::Horizontal);

        hboxLayout2->addWidget(progressBar);

        buttonBox = new QDialogButtonBox(PreviewDialogBase);
        buttonBox->setObjectName("buttonBox");
        buttonBox->setOrientation(BobUI::Orientation::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::StandardButton::Cancel|QDialogButtonBox::StandardButton::Ok);

        hboxLayout2->addWidget(buttonBox);


        vboxLayout->addLayout(hboxLayout2);

#if BOBUI_CONFIG(shortcut)
        label->setBuddy(paperSizeCombo);
        label_2->setBuddy(paperOrientationCombo);
#endif // BOBUI_CONFIG(shortcut)

        retranslateUi(PreviewDialogBase);
        QObject::connect(buttonBox, &QDialogButtonBox::accepted, PreviewDialogBase, qOverload<>(&QDialog::accept));
        QObject::connect(buttonBox, &QDialogButtonBox::rejected, PreviewDialogBase, qOverload<>(&QDialog::reject));

        QMetaObject::connectSlotsByName(PreviewDialogBase);
    } // setupUi

    void retranslateUi(QDialog *PreviewDialogBase)
    {
        PreviewDialogBase->setWindowTitle(QCoreApplication::translate("PreviewDialogBase", "Print Preview", nullptr));
        label->setText(QCoreApplication::translate("PreviewDialogBase", "&Paper Size:", nullptr));
        label_2->setText(QCoreApplication::translate("PreviewDialogBase", "&Orientation:", nullptr));
        BOBUIreeWidgetItem *___bobuireewidgetitem = pageList->headerItem();
        ___bobuireewidgetitem->setText(0, QCoreApplication::translate("PreviewDialogBase", "1", nullptr));
    } // retranslateUi

};

namespace Ui {
    class PreviewDialogBase: public Ui_PreviewDialogBase {};
} // namespace Ui

BOBUI_END_NAMESPACE

#endif // PREVIEWDIALOGBASE_H
