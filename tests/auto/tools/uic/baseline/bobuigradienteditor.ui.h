/*

* Copyright (C) 2016 The BobUI Company Ltd.
* SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

*/

/********************************************************************************
** Form generated from reading UI file 'bobuigradienteditor.ui'
**
** Created by: BobUI User Interface Compiler version 6.0.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef BOBUIGRADIENTEDITOR_H
#define BOBUIGRADIENTEDITOR_H

#include <BobUICore/QVariant>
#include <BobUIWidgets/QApplication>
#include <BobUIWidgets/QComboBox>
#include <BobUIWidgets/QDoubleSpinBox>
#include <BobUIWidgets/QFrame>
#include <BobUIWidgets/QHBoxLayout>
#include <BobUIWidgets/QLabel>
#include <BobUIWidgets/QRadioButton>
#include <BobUIWidgets/QSpacerItem>
#include <BobUIWidgets/QSpinBox>
#include <BobUIWidgets/BOBUIoolButton>
#include <BobUIWidgets/QVBoxLayout>
#include <BobUIWidgets/QWidget>
#include "bobuicolorbutton.h"
#include "bobuicolorline.h"
#include "bobuigradientstopswidget.h"
#include "bobuigradientwidget.h"

BOBUI_BEGIN_NAMESPACE

class Ui_BobUIGradientEditor
{
public:
    QFrame *frame;
    QVBoxLayout *vboxLayout;
    BobUIGradientWidget *gradientWidget;
    QLabel *label1;
    QDoubleSpinBox *spinBox1;
    QLabel *label2;
    QDoubleSpinBox *spinBox2;
    QLabel *label3;
    QDoubleSpinBox *spinBox3;
    QLabel *label4;
    QDoubleSpinBox *spinBox4;
    QLabel *label5;
    QDoubleSpinBox *spinBox5;
    BobUIGradientStopsWidget *gradientStopsWidget;
    QLabel *zoomLabel;
    BOBUIoolButton *zoomAllButton;
    QLabel *positionLabel;
    QLabel *hLabel;
    QFrame *frame_2;
    QHBoxLayout *hboxLayout;
    BobUIColorLine *hueColorLine;
    QLabel *hueLabel;
    QLabel *sLabel;
    QFrame *frame_5;
    QHBoxLayout *hboxLayout1;
    BobUIColorLine *saturationColorLine;
    QLabel *saturationLabel;
    QLabel *vLabel;
    QFrame *frame_3;
    QHBoxLayout *hboxLayout2;
    BobUIColorLine *valueColorLine;
    QLabel *valueLabel;
    QLabel *aLabel;
    QFrame *frame_4;
    QHBoxLayout *hboxLayout3;
    BobUIColorLine *alphaColorLine;
    QLabel *alphaLabel;
    QComboBox *typeComboBox;
    QComboBox *spreadComboBox;
    QLabel *colorLabel;
    BobUIColorButton *colorButton;
    QRadioButton *hsvRadioButton;
    QRadioButton *rgbRadioButton;
    QWidget *positionWidget;
    QVBoxLayout *vboxLayout1;
    QDoubleSpinBox *positionSpinBox;
    QWidget *hueWidget;
    QVBoxLayout *vboxLayout2;
    QSpinBox *hueSpinBox;
    QWidget *saturationWidget;
    QVBoxLayout *vboxLayout3;
    QSpinBox *saturationSpinBox;
    QWidget *valueWidget;
    QVBoxLayout *vboxLayout4;
    QSpinBox *valueSpinBox;
    QWidget *alphaWidget;
    QVBoxLayout *vboxLayout5;
    QSpinBox *alphaSpinBox;
    QWidget *zoomWidget;
    QVBoxLayout *vboxLayout6;
    QSpinBox *zoomSpinBox;
    QWidget *line1Widget;
    QVBoxLayout *vboxLayout7;
    QFrame *line1;
    QWidget *line2Widget;
    QVBoxLayout *vboxLayout8;
    QFrame *line2;
    QWidget *zoomButtonsWidget;
    QHBoxLayout *hboxLayout4;
    BOBUIoolButton *zoomInButton;
    BOBUIoolButton *zoomOutButton;
    QSpacerItem *spacerItem;
    BOBUIoolButton *detailsButton;
    BOBUIoolButton *linearButton;
    BOBUIoolButton *radialButton;
    BOBUIoolButton *conicalButton;
    BOBUIoolButton *padButton;
    BOBUIoolButton *repeatButton;
    BOBUIoolButton *reflectButton;

    void setupUi(QWidget *BobUIGradientEditor)
    {
        if (BobUIGradientEditor->objectName().isEmpty())
            BobUIGradientEditor->setObjectName("BobUIGradientEditor");
        BobUIGradientEditor->resize(364, 518);
        frame = new QFrame(BobUIGradientEditor);
        frame->setObjectName("frame");
        frame->setGeometry(QRect(10, 69, 193, 150));
        QSizePolicy sizePolicy(QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(frame->sizePolicy().hasHeightForWidth());
        frame->setSizePolicy(sizePolicy);
        frame->setFrameShape(QFrame::Shape::StyledPanel);
        frame->setFrameShadow(QFrame::Shadow::Raised);
        vboxLayout = new QVBoxLayout(frame);
        vboxLayout->setSpacing(6);
        vboxLayout->setObjectName("vboxLayout");
        vboxLayout->setContentsMargins(0, 0, 0, 0);
        gradientWidget = new BobUIGradientWidget(frame);
        gradientWidget->setObjectName("gradientWidget");
        sizePolicy.setHeightForWidth(gradientWidget->sizePolicy().hasHeightForWidth());
        gradientWidget->setSizePolicy(sizePolicy);

        vboxLayout->addWidget(gradientWidget);

        label1 = new QLabel(BobUIGradientEditor);
        label1->setObjectName("label1");
        label1->setGeometry(QRect(209, 69, 64, 23));
        spinBox1 = new QDoubleSpinBox(BobUIGradientEditor);
        spinBox1->setObjectName("spinBox1");
        spinBox1->setGeometry(QRect(279, 69, 73, 23));
        spinBox1->setKeyboardTracking(false);
        spinBox1->setDecimals(3);
        spinBox1->setMaximum(1.000000000000000);
        spinBox1->setSingleStep(0.010000000000000);
        label2 = new QLabel(BobUIGradientEditor);
        label2->setObjectName("label2");
        label2->setGeometry(QRect(209, 99, 64, 23));
        spinBox2 = new QDoubleSpinBox(BobUIGradientEditor);
        spinBox2->setObjectName("spinBox2");
        spinBox2->setGeometry(QRect(279, 99, 73, 23));
        spinBox2->setKeyboardTracking(false);
        spinBox2->setDecimals(3);
        spinBox2->setMaximum(1.000000000000000);
        spinBox2->setSingleStep(0.010000000000000);
        label3 = new QLabel(BobUIGradientEditor);
        label3->setObjectName("label3");
        label3->setGeometry(QRect(209, 129, 64, 23));
        spinBox3 = new QDoubleSpinBox(BobUIGradientEditor);
        spinBox3->setObjectName("spinBox3");
        spinBox3->setGeometry(QRect(279, 129, 73, 23));
        spinBox3->setKeyboardTracking(false);
        spinBox3->setDecimals(3);
        spinBox3->setMaximum(1.000000000000000);
        spinBox3->setSingleStep(0.010000000000000);
        label4 = new QLabel(BobUIGradientEditor);
        label4->setObjectName("label4");
        label4->setGeometry(QRect(209, 159, 64, 23));
        spinBox4 = new QDoubleSpinBox(BobUIGradientEditor);
        spinBox4->setObjectName("spinBox4");
        spinBox4->setGeometry(QRect(279, 159, 73, 23));
        spinBox4->setKeyboardTracking(false);
        spinBox4->setDecimals(3);
        spinBox4->setMaximum(1.000000000000000);
        spinBox4->setSingleStep(0.010000000000000);
        label5 = new QLabel(BobUIGradientEditor);
        label5->setObjectName("label5");
        label5->setGeometry(QRect(209, 189, 64, 23));
        spinBox5 = new QDoubleSpinBox(BobUIGradientEditor);
        spinBox5->setObjectName("spinBox5");
        spinBox5->setGeometry(QRect(279, 189, 73, 23));
        spinBox5->setKeyboardTracking(false);
        spinBox5->setDecimals(3);
        spinBox5->setMaximum(1.000000000000000);
        spinBox5->setSingleStep(0.010000000000000);
        gradientStopsWidget = new BobUIGradientStopsWidget(BobUIGradientEditor);
        gradientStopsWidget->setObjectName("gradientStopsWidget");
        gradientStopsWidget->setGeometry(QRect(10, 225, 193, 67));
        zoomLabel = new QLabel(BobUIGradientEditor);
        zoomLabel->setObjectName("zoomLabel");
        zoomLabel->setGeometry(QRect(209, 231, 64, 23));
        zoomAllButton = new BOBUIoolButton(BobUIGradientEditor);
        zoomAllButton->setObjectName("zoomAllButton");
        zoomAllButton->setGeometry(QRect(279, 260, 72, 26));
        QSizePolicy sizePolicy1(QSizePolicy::Policy::Preferred, QSizePolicy::Policy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(zoomAllButton->sizePolicy().hasHeightForWidth());
        zoomAllButton->setSizePolicy(sizePolicy1);
        positionLabel = new QLabel(BobUIGradientEditor);
        positionLabel->setObjectName("positionLabel");
        positionLabel->setGeometry(QRect(209, 304, 64, 23));
        hLabel = new QLabel(BobUIGradientEditor);
        hLabel->setObjectName("hLabel");
        hLabel->setGeometry(QRect(10, 335, 32, 18));
        sizePolicy1.setHeightForWidth(hLabel->sizePolicy().hasHeightForWidth());
        hLabel->setSizePolicy(sizePolicy1);
        frame_2 = new QFrame(BobUIGradientEditor);
        frame_2->setObjectName("frame_2");
        frame_2->setGeometry(QRect(48, 333, 155, 23));
        QSizePolicy sizePolicy2(QSizePolicy::Policy::Ignored, QSizePolicy::Policy::Preferred);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(frame_2->sizePolicy().hasHeightForWidth());
        frame_2->setSizePolicy(sizePolicy2);
        frame_2->setFrameShape(QFrame::Shape::StyledPanel);
        frame_2->setFrameShadow(QFrame::Shadow::Raised);
        hboxLayout = new QHBoxLayout(frame_2);
        hboxLayout->setObjectName("hboxLayout");
        hboxLayout->setContentsMargins(0, 0, 0, 0);
        hueColorLine = new BobUIColorLine(frame_2);
        hueColorLine->setObjectName("hueColorLine");
        QSizePolicy sizePolicy3(QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Preferred);
        sizePolicy3.setHorizontalStretch(0);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(hueColorLine->sizePolicy().hasHeightForWidth());
        hueColorLine->setSizePolicy(sizePolicy3);

        hboxLayout->addWidget(hueColorLine);

        hueLabel = new QLabel(BobUIGradientEditor);
        hueLabel->setObjectName("hueLabel");
        hueLabel->setGeometry(QRect(209, 335, 64, 18));
        sizePolicy1.setHeightForWidth(hueLabel->sizePolicy().hasHeightForWidth());
        hueLabel->setSizePolicy(sizePolicy1);
        sLabel = new QLabel(BobUIGradientEditor);
        sLabel->setObjectName("sLabel");
        sLabel->setGeometry(QRect(10, 364, 32, 18));
        sizePolicy1.setHeightForWidth(sLabel->sizePolicy().hasHeightForWidth());
        sLabel->setSizePolicy(sizePolicy1);
        frame_5 = new QFrame(BobUIGradientEditor);
        frame_5->setObjectName("frame_5");
        frame_5->setGeometry(QRect(48, 362, 155, 23));
        sizePolicy2.setHeightForWidth(frame_5->sizePolicy().hasHeightForWidth());
        frame_5->setSizePolicy(sizePolicy2);
        frame_5->setFrameShape(QFrame::Shape::StyledPanel);
        frame_5->setFrameShadow(QFrame::Shadow::Raised);
        hboxLayout1 = new QHBoxLayout(frame_5);
        hboxLayout1->setObjectName("hboxLayout1");
        hboxLayout1->setContentsMargins(0, 0, 0, 0);
        saturationColorLine = new BobUIColorLine(frame_5);
        saturationColorLine->setObjectName("saturationColorLine");
        sizePolicy3.setHeightForWidth(saturationColorLine->sizePolicy().hasHeightForWidth());
        saturationColorLine->setSizePolicy(sizePolicy3);

        hboxLayout1->addWidget(saturationColorLine);

        saturationLabel = new QLabel(BobUIGradientEditor);
        saturationLabel->setObjectName("saturationLabel");
        saturationLabel->setGeometry(QRect(209, 364, 64, 18));
        sizePolicy1.setHeightForWidth(saturationLabel->sizePolicy().hasHeightForWidth());
        saturationLabel->setSizePolicy(sizePolicy1);
        vLabel = new QLabel(BobUIGradientEditor);
        vLabel->setObjectName("vLabel");
        vLabel->setGeometry(QRect(10, 393, 32, 18));
        sizePolicy1.setHeightForWidth(vLabel->sizePolicy().hasHeightForWidth());
        vLabel->setSizePolicy(sizePolicy1);
        frame_3 = new QFrame(BobUIGradientEditor);
        frame_3->setObjectName("frame_3");
        frame_3->setGeometry(QRect(48, 391, 155, 23));
        sizePolicy2.setHeightForWidth(frame_3->sizePolicy().hasHeightForWidth());
        frame_3->setSizePolicy(sizePolicy2);
        frame_3->setFrameShape(QFrame::Shape::StyledPanel);
        frame_3->setFrameShadow(QFrame::Shadow::Raised);
        hboxLayout2 = new QHBoxLayout(frame_3);
        hboxLayout2->setObjectName("hboxLayout2");
        hboxLayout2->setContentsMargins(0, 0, 0, 0);
        valueColorLine = new BobUIColorLine(frame_3);
        valueColorLine->setObjectName("valueColorLine");
        sizePolicy3.setHeightForWidth(valueColorLine->sizePolicy().hasHeightForWidth());
        valueColorLine->setSizePolicy(sizePolicy3);

        hboxLayout2->addWidget(valueColorLine);

        valueLabel = new QLabel(BobUIGradientEditor);
        valueLabel->setObjectName("valueLabel");
        valueLabel->setGeometry(QRect(209, 393, 64, 18));
        sizePolicy1.setHeightForWidth(valueLabel->sizePolicy().hasHeightForWidth());
        valueLabel->setSizePolicy(sizePolicy1);
        aLabel = new QLabel(BobUIGradientEditor);
        aLabel->setObjectName("aLabel");
        aLabel->setGeometry(QRect(10, 422, 32, 18));
        sizePolicy1.setHeightForWidth(aLabel->sizePolicy().hasHeightForWidth());
        aLabel->setSizePolicy(sizePolicy1);
        frame_4 = new QFrame(BobUIGradientEditor);
        frame_4->setObjectName("frame_4");
        frame_4->setGeometry(QRect(48, 420, 155, 23));
        sizePolicy2.setHeightForWidth(frame_4->sizePolicy().hasHeightForWidth());
        frame_4->setSizePolicy(sizePolicy2);
        frame_4->setFrameShape(QFrame::Shape::StyledPanel);
        frame_4->setFrameShadow(QFrame::Shadow::Raised);
        hboxLayout3 = new QHBoxLayout(frame_4);
        hboxLayout3->setObjectName("hboxLayout3");
        hboxLayout3->setContentsMargins(0, 0, 0, 0);
        alphaColorLine = new BobUIColorLine(frame_4);
        alphaColorLine->setObjectName("alphaColorLine");
        sizePolicy3.setHeightForWidth(alphaColorLine->sizePolicy().hasHeightForWidth());
        alphaColorLine->setSizePolicy(sizePolicy3);

        hboxLayout3->addWidget(alphaColorLine);

        alphaLabel = new QLabel(BobUIGradientEditor);
        alphaLabel->setObjectName("alphaLabel");
        alphaLabel->setGeometry(QRect(209, 422, 64, 18));
        sizePolicy1.setHeightForWidth(alphaLabel->sizePolicy().hasHeightForWidth());
        alphaLabel->setSizePolicy(sizePolicy1);
        typeComboBox = new QComboBox(BobUIGradientEditor);
        typeComboBox->setObjectName("typeComboBox");
        typeComboBox->setGeometry(QRect(10, 40, 79, 22));
        spreadComboBox = new QComboBox(BobUIGradientEditor);
        spreadComboBox->setObjectName("spreadComboBox");
        spreadComboBox->setGeometry(QRect(96, 40, 72, 22));
        colorLabel = new QLabel(BobUIGradientEditor);
        colorLabel->setObjectName("colorLabel");
        colorLabel->setGeometry(QRect(10, 298, 32, 29));
        QSizePolicy sizePolicy4(QSizePolicy::Policy::Fixed, QSizePolicy::Policy::Preferred);
        sizePolicy4.setHorizontalStretch(0);
        sizePolicy4.setVerticalStretch(0);
        sizePolicy4.setHeightForWidth(colorLabel->sizePolicy().hasHeightForWidth());
        colorLabel->setSizePolicy(sizePolicy4);
        colorButton = new BobUIColorButton(BobUIGradientEditor);
        colorButton->setObjectName("colorButton");
        colorButton->setGeometry(QRect(48, 300, 26, 25));
        hsvRadioButton = new QRadioButton(BobUIGradientEditor);
        hsvRadioButton->setObjectName("hsvRadioButton");
        hsvRadioButton->setGeometry(QRect(80, 301, 49, 23));
        QSizePolicy sizePolicy5(QSizePolicy::Policy::Fixed, QSizePolicy::Policy::Fixed);
        sizePolicy5.setHorizontalStretch(0);
        sizePolicy5.setVerticalStretch(0);
        sizePolicy5.setHeightForWidth(hsvRadioButton->sizePolicy().hasHeightForWidth());
        hsvRadioButton->setSizePolicy(sizePolicy5);
        hsvRadioButton->setChecked(true);
        rgbRadioButton = new QRadioButton(BobUIGradientEditor);
        rgbRadioButton->setObjectName("rgbRadioButton");
        rgbRadioButton->setGeometry(QRect(135, 301, 49, 23));
        sizePolicy5.setHeightForWidth(rgbRadioButton->sizePolicy().hasHeightForWidth());
        rgbRadioButton->setSizePolicy(sizePolicy5);
        positionWidget = new QWidget(BobUIGradientEditor);
        positionWidget->setObjectName("positionWidget");
        positionWidget->setGeometry(QRect(279, 304, 73, 23));
        vboxLayout1 = new QVBoxLayout(positionWidget);
        vboxLayout1->setObjectName("vboxLayout1");
        vboxLayout1->setContentsMargins(0, 0, 0, 0);
        positionSpinBox = new QDoubleSpinBox(positionWidget);
        positionSpinBox->setObjectName("positionSpinBox");
        positionSpinBox->setKeyboardTracking(false);
        positionSpinBox->setDecimals(3);
        positionSpinBox->setMinimum(0.000000000000000);
        positionSpinBox->setMaximum(1.000000000000000);
        positionSpinBox->setSingleStep(0.010000000000000);
        positionSpinBox->setValue(0.000000000000000);

        vboxLayout1->addWidget(positionSpinBox);

        hueWidget = new QWidget(BobUIGradientEditor);
        hueWidget->setObjectName("hueWidget");
        hueWidget->setGeometry(QRect(279, 333, 73, 23));
        vboxLayout2 = new QVBoxLayout(hueWidget);
        vboxLayout2->setObjectName("vboxLayout2");
        vboxLayout2->setContentsMargins(0, 0, 0, 0);
        hueSpinBox = new QSpinBox(hueWidget);
        hueSpinBox->setObjectName("hueSpinBox");
        hueSpinBox->setKeyboardTracking(false);
        hueSpinBox->setMaximum(359);

        vboxLayout2->addWidget(hueSpinBox);

        saturationWidget = new QWidget(BobUIGradientEditor);
        saturationWidget->setObjectName("saturationWidget");
        saturationWidget->setGeometry(QRect(279, 362, 73, 23));
        vboxLayout3 = new QVBoxLayout(saturationWidget);
        vboxLayout3->setObjectName("vboxLayout3");
        vboxLayout3->setContentsMargins(0, 0, 0, 0);
        saturationSpinBox = new QSpinBox(saturationWidget);
        saturationSpinBox->setObjectName("saturationSpinBox");
        saturationSpinBox->setKeyboardTracking(false);
        saturationSpinBox->setMaximum(255);

        vboxLayout3->addWidget(saturationSpinBox);

        valueWidget = new QWidget(BobUIGradientEditor);
        valueWidget->setObjectName("valueWidget");
        valueWidget->setGeometry(QRect(279, 391, 73, 23));
        vboxLayout4 = new QVBoxLayout(valueWidget);
        vboxLayout4->setObjectName("vboxLayout4");
        vboxLayout4->setContentsMargins(0, 0, 0, 0);
        valueSpinBox = new QSpinBox(valueWidget);
        valueSpinBox->setObjectName("valueSpinBox");
        valueSpinBox->setKeyboardTracking(false);
        valueSpinBox->setMaximum(255);

        vboxLayout4->addWidget(valueSpinBox);

        alphaWidget = new QWidget(BobUIGradientEditor);
        alphaWidget->setObjectName("alphaWidget");
        alphaWidget->setGeometry(QRect(279, 420, 73, 23));
        vboxLayout5 = new QVBoxLayout(alphaWidget);
        vboxLayout5->setObjectName("vboxLayout5");
        vboxLayout5->setContentsMargins(0, 0, 0, 0);
        alphaSpinBox = new QSpinBox(alphaWidget);
        alphaSpinBox->setObjectName("alphaSpinBox");
        alphaSpinBox->setKeyboardTracking(false);
        alphaSpinBox->setMaximum(255);

        vboxLayout5->addWidget(alphaSpinBox);

        zoomWidget = new QWidget(BobUIGradientEditor);
        zoomWidget->setObjectName("zoomWidget");
        zoomWidget->setGeometry(QRect(279, 231, 73, 23));
        vboxLayout6 = new QVBoxLayout(zoomWidget);
        vboxLayout6->setObjectName("vboxLayout6");
        vboxLayout6->setContentsMargins(0, 0, 0, 0);
        zoomSpinBox = new QSpinBox(zoomWidget);
        zoomSpinBox->setObjectName("zoomSpinBox");
        zoomSpinBox->setKeyboardTracking(false);
        zoomSpinBox->setMinimum(100);
        zoomSpinBox->setMaximum(10000);
        zoomSpinBox->setSingleStep(100);
        zoomSpinBox->setValue(100);

        vboxLayout6->addWidget(zoomSpinBox);

        line1Widget = new QWidget(BobUIGradientEditor);
        line1Widget->setObjectName("line1Widget");
        line1Widget->setGeometry(QRect(209, 219, 143, 16));
        vboxLayout7 = new QVBoxLayout(line1Widget);
        vboxLayout7->setObjectName("vboxLayout7");
        vboxLayout7->setContentsMargins(0, 0, 0, 0);
        line1 = new QFrame(line1Widget);
        line1->setObjectName("line1");
        line1->setFrameShape(QFrame::Shape::HLine);
        line1->setFrameShadow(QFrame::Shadow::Sunken);

        vboxLayout7->addWidget(line1);

        line2Widget = new QWidget(BobUIGradientEditor);
        line2Widget->setObjectName("line2Widget");
        line2Widget->setGeometry(QRect(209, 292, 143, 16));
        vboxLayout8 = new QVBoxLayout(line2Widget);
        vboxLayout8->setObjectName("vboxLayout8");
        vboxLayout8->setContentsMargins(0, 0, 0, 0);
        line2 = new QFrame(line2Widget);
        line2->setObjectName("line2");
        line2->setFrameShape(QFrame::Shape::HLine);
        line2->setFrameShadow(QFrame::Shadow::Sunken);

        vboxLayout8->addWidget(line2);

        zoomButtonsWidget = new QWidget(BobUIGradientEditor);
        zoomButtonsWidget->setObjectName("zoomButtonsWidget");
        zoomButtonsWidget->setGeometry(QRect(209, 260, 64, 26));
        QSizePolicy sizePolicy6(QSizePolicy::Policy::Maximum, QSizePolicy::Policy::Preferred);
        sizePolicy6.setHorizontalStretch(0);
        sizePolicy6.setVerticalStretch(0);
        sizePolicy6.setHeightForWidth(zoomButtonsWidget->sizePolicy().hasHeightForWidth());
        zoomButtonsWidget->setSizePolicy(sizePolicy6);
        hboxLayout4 = new QHBoxLayout(zoomButtonsWidget);
        hboxLayout4->setObjectName("hboxLayout4");
        hboxLayout4->setContentsMargins(0, 0, 0, 0);
        zoomInButton = new BOBUIoolButton(zoomButtonsWidget);
        zoomInButton->setObjectName("zoomInButton");

        hboxLayout4->addWidget(zoomInButton);

        zoomOutButton = new BOBUIoolButton(zoomButtonsWidget);
        zoomOutButton->setObjectName("zoomOutButton");

        hboxLayout4->addWidget(zoomOutButton);

        spacerItem = new QSpacerItem(0, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        hboxLayout4->addItem(spacerItem);

        detailsButton = new BOBUIoolButton(BobUIGradientEditor);
        detailsButton->setObjectName("detailsButton");
        detailsButton->setGeometry(QRect(176, 40, 25, 22));
        QSizePolicy sizePolicy7(QSizePolicy::Policy::Fixed, QSizePolicy::Policy::Ignored);
        sizePolicy7.setHorizontalStretch(0);
        sizePolicy7.setVerticalStretch(0);
        sizePolicy7.setHeightForWidth(detailsButton->sizePolicy().hasHeightForWidth());
        detailsButton->setSizePolicy(sizePolicy7);
        detailsButton->setCheckable(true);
        detailsButton->setAutoRaise(true);
        linearButton = new BOBUIoolButton(BobUIGradientEditor);
        linearButton->setObjectName("linearButton");
        linearButton->setGeometry(QRect(10, 10, 30, 26));
        linearButton->setCheckable(true);
        linearButton->setAutoRaise(true);
        radialButton = new BOBUIoolButton(BobUIGradientEditor);
        radialButton->setObjectName("radialButton");
        radialButton->setGeometry(QRect(40, 10, 30, 26));
        radialButton->setCheckable(true);
        radialButton->setAutoRaise(true);
        conicalButton = new BOBUIoolButton(BobUIGradientEditor);
        conicalButton->setObjectName("conicalButton");
        conicalButton->setGeometry(QRect(70, 10, 30, 26));
        conicalButton->setCheckable(true);
        conicalButton->setAutoRaise(true);
        padButton = new BOBUIoolButton(BobUIGradientEditor);
        padButton->setObjectName("padButton");
        padButton->setGeometry(QRect(110, 10, 30, 26));
        padButton->setCheckable(true);
        padButton->setAutoRaise(true);
        repeatButton = new BOBUIoolButton(BobUIGradientEditor);
        repeatButton->setObjectName("repeatButton");
        repeatButton->setGeometry(QRect(140, 10, 30, 26));
        repeatButton->setCheckable(true);
        repeatButton->setAutoRaise(true);
        reflectButton = new BOBUIoolButton(BobUIGradientEditor);
        reflectButton->setObjectName("reflectButton");
        reflectButton->setGeometry(QRect(170, 10, 30, 26));
        reflectButton->setCheckable(true);
        reflectButton->setAutoRaise(true);
        QWidget::setTabOrder(typeComboBox, spreadComboBox);
        QWidget::setTabOrder(spreadComboBox, detailsButton);
        QWidget::setTabOrder(detailsButton, spinBox1);
        QWidget::setTabOrder(spinBox1, spinBox2);
        QWidget::setTabOrder(spinBox2, spinBox3);
        QWidget::setTabOrder(spinBox3, spinBox4);
        QWidget::setTabOrder(spinBox4, spinBox5);
        QWidget::setTabOrder(spinBox5, zoomSpinBox);
        QWidget::setTabOrder(zoomSpinBox, zoomInButton);
        QWidget::setTabOrder(zoomInButton, zoomOutButton);
        QWidget::setTabOrder(zoomOutButton, zoomAllButton);
        QWidget::setTabOrder(zoomAllButton, colorButton);
        QWidget::setTabOrder(colorButton, hsvRadioButton);
        QWidget::setTabOrder(hsvRadioButton, rgbRadioButton);
        QWidget::setTabOrder(rgbRadioButton, positionSpinBox);
        QWidget::setTabOrder(positionSpinBox, hueSpinBox);
        QWidget::setTabOrder(hueSpinBox, saturationSpinBox);
        QWidget::setTabOrder(saturationSpinBox, valueSpinBox);
        QWidget::setTabOrder(valueSpinBox, alphaSpinBox);

        retranslateUi(BobUIGradientEditor);

        QMetaObject::connectSlotsByName(BobUIGradientEditor);
    } // setupUi

    void retranslateUi(QWidget *BobUIGradientEditor)
    {
        BobUIGradientEditor->setWindowTitle(QCoreApplication::translate("BobUIGradientEditor", "Form", nullptr));
#if BOBUI_CONFIG(tooltip)
        gradientWidget->setToolTip(QCoreApplication::translate("BobUIGradientEditor", "Gradient Editor", nullptr));
#endif // BOBUI_CONFIG(tooltip)
#if BOBUI_CONFIG(whatsthis)
        gradientWidget->setWhatsThis(QCoreApplication::translate("BobUIGradientEditor", "This area shows a preview of the gradient being edited. It also allows you to edit parameters specific to the gradient's type such as start and final point, radius, etc. by drag & drop.", nullptr));
#endif // BOBUI_CONFIG(whatsthis)
        label1->setText(QCoreApplication::translate("BobUIGradientEditor", "1", nullptr));
        label2->setText(QCoreApplication::translate("BobUIGradientEditor", "2", nullptr));
        label3->setText(QCoreApplication::translate("BobUIGradientEditor", "3", nullptr));
        label4->setText(QCoreApplication::translate("BobUIGradientEditor", "4", nullptr));
        label5->setText(QCoreApplication::translate("BobUIGradientEditor", "5", nullptr));
#if BOBUI_CONFIG(tooltip)
        gradientStopsWidget->setToolTip(QCoreApplication::translate("BobUIGradientEditor", "Gradient Stops Editor", nullptr));
#endif // BOBUI_CONFIG(tooltip)
#if BOBUI_CONFIG(whatsthis)
        gradientStopsWidget->setWhatsThis(QCoreApplication::translate("BobUIGradientEditor", "This area allows you to edit gradient stops. Double click on the existing stop handle to duplicate it. Double click outside of the existing stop handles to create a new stop. Drag & drop the handle to reposition it. Use right mouse button to popup context menu with extra actions.", nullptr));
#endif // BOBUI_CONFIG(whatsthis)
        zoomLabel->setText(QCoreApplication::translate("BobUIGradientEditor", "Zoom", nullptr));
#if BOBUI_CONFIG(tooltip)
        zoomAllButton->setToolTip(QCoreApplication::translate("BobUIGradientEditor", "Reset Zoom", nullptr));
#endif // BOBUI_CONFIG(tooltip)
        zoomAllButton->setText(QCoreApplication::translate("BobUIGradientEditor", "Reset Zoom", nullptr));
        positionLabel->setText(QCoreApplication::translate("BobUIGradientEditor", "Position", nullptr));
#if BOBUI_CONFIG(tooltip)
        hLabel->setToolTip(QCoreApplication::translate("BobUIGradientEditor", "Hue", nullptr));
#endif // BOBUI_CONFIG(tooltip)
        hLabel->setText(QCoreApplication::translate("BobUIGradientEditor", "H", nullptr));
#if BOBUI_CONFIG(tooltip)
        hueColorLine->setToolTip(QCoreApplication::translate("BobUIGradientEditor", "Hue", nullptr));
#endif // BOBUI_CONFIG(tooltip)
        hueLabel->setText(QCoreApplication::translate("BobUIGradientEditor", "Hue", nullptr));
#if BOBUI_CONFIG(tooltip)
        sLabel->setToolTip(QCoreApplication::translate("BobUIGradientEditor", "Saturation", nullptr));
#endif // BOBUI_CONFIG(tooltip)
        sLabel->setText(QCoreApplication::translate("BobUIGradientEditor", "S", nullptr));
#if BOBUI_CONFIG(tooltip)
        saturationColorLine->setToolTip(QCoreApplication::translate("BobUIGradientEditor", "Saturation", nullptr));
#endif // BOBUI_CONFIG(tooltip)
        saturationLabel->setText(QCoreApplication::translate("BobUIGradientEditor", "Sat", nullptr));
#if BOBUI_CONFIG(tooltip)
        vLabel->setToolTip(QCoreApplication::translate("BobUIGradientEditor", "Value", nullptr));
#endif // BOBUI_CONFIG(tooltip)
        vLabel->setText(QCoreApplication::translate("BobUIGradientEditor", "V", nullptr));
#if BOBUI_CONFIG(tooltip)
        valueColorLine->setToolTip(QCoreApplication::translate("BobUIGradientEditor", "Value", nullptr));
#endif // BOBUI_CONFIG(tooltip)
        valueLabel->setText(QCoreApplication::translate("BobUIGradientEditor", "Val", nullptr));
#if BOBUI_CONFIG(tooltip)
        aLabel->setToolTip(QCoreApplication::translate("BobUIGradientEditor", "Alpha", nullptr));
#endif // BOBUI_CONFIG(tooltip)
        aLabel->setText(QCoreApplication::translate("BobUIGradientEditor", "A", nullptr));
#if BOBUI_CONFIG(tooltip)
        alphaColorLine->setToolTip(QCoreApplication::translate("BobUIGradientEditor", "Alpha", nullptr));
#endif // BOBUI_CONFIG(tooltip)
        alphaLabel->setText(QCoreApplication::translate("BobUIGradientEditor", "Alpha", nullptr));
#if BOBUI_CONFIG(tooltip)
        typeComboBox->setToolTip(QCoreApplication::translate("BobUIGradientEditor", "Type", nullptr));
#endif // BOBUI_CONFIG(tooltip)
#if BOBUI_CONFIG(tooltip)
        spreadComboBox->setToolTip(QCoreApplication::translate("BobUIGradientEditor", "Spread", nullptr));
#endif // BOBUI_CONFIG(tooltip)
        colorLabel->setText(QCoreApplication::translate("BobUIGradientEditor", "Color", nullptr));
#if BOBUI_CONFIG(tooltip)
        colorButton->setToolTip(QCoreApplication::translate("BobUIGradientEditor", "Current stop's color", nullptr));
#endif // BOBUI_CONFIG(tooltip)
        colorButton->setText(QString());
#if BOBUI_CONFIG(tooltip)
        hsvRadioButton->setToolTip(QCoreApplication::translate("BobUIGradientEditor", "Show HSV specification", nullptr));
#endif // BOBUI_CONFIG(tooltip)
        hsvRadioButton->setText(QCoreApplication::translate("BobUIGradientEditor", "HSV", nullptr));
#if BOBUI_CONFIG(tooltip)
        rgbRadioButton->setToolTip(QCoreApplication::translate("BobUIGradientEditor", "Show RGB specification", nullptr));
#endif // BOBUI_CONFIG(tooltip)
        rgbRadioButton->setText(QCoreApplication::translate("BobUIGradientEditor", "RGB", nullptr));
#if BOBUI_CONFIG(tooltip)
        positionSpinBox->setToolTip(QCoreApplication::translate("BobUIGradientEditor", "Current stop's position", nullptr));
#endif // BOBUI_CONFIG(tooltip)
        zoomSpinBox->setSuffix(QCoreApplication::translate("BobUIGradientEditor", "%", nullptr));
#if BOBUI_CONFIG(tooltip)
        zoomInButton->setToolTip(QCoreApplication::translate("BobUIGradientEditor", "Zoom In", nullptr));
#endif // BOBUI_CONFIG(tooltip)
#if BOBUI_CONFIG(tooltip)
        zoomOutButton->setToolTip(QCoreApplication::translate("BobUIGradientEditor", "Zoom Out", nullptr));
#endif // BOBUI_CONFIG(tooltip)
#if BOBUI_CONFIG(tooltip)
        detailsButton->setToolTip(QCoreApplication::translate("BobUIGradientEditor", "Toggle details extension", nullptr));
#endif // BOBUI_CONFIG(tooltip)
        detailsButton->setText(QCoreApplication::translate("BobUIGradientEditor", ">", nullptr));
#if BOBUI_CONFIG(tooltip)
        linearButton->setToolTip(QCoreApplication::translate("BobUIGradientEditor", "Linear Type", nullptr));
#endif // BOBUI_CONFIG(tooltip)
        linearButton->setText(QCoreApplication::translate("BobUIGradientEditor", "...", nullptr));
#if BOBUI_CONFIG(tooltip)
        radialButton->setToolTip(QCoreApplication::translate("BobUIGradientEditor", "Radial Type", nullptr));
#endif // BOBUI_CONFIG(tooltip)
        radialButton->setText(QCoreApplication::translate("BobUIGradientEditor", "...", nullptr));
#if BOBUI_CONFIG(tooltip)
        conicalButton->setToolTip(QCoreApplication::translate("BobUIGradientEditor", "Conical Type", nullptr));
#endif // BOBUI_CONFIG(tooltip)
        conicalButton->setText(QCoreApplication::translate("BobUIGradientEditor", "...", nullptr));
#if BOBUI_CONFIG(tooltip)
        padButton->setToolTip(QCoreApplication::translate("BobUIGradientEditor", "Pad Spread", nullptr));
#endif // BOBUI_CONFIG(tooltip)
        padButton->setText(QCoreApplication::translate("BobUIGradientEditor", "...", nullptr));
#if BOBUI_CONFIG(tooltip)
        repeatButton->setToolTip(QCoreApplication::translate("BobUIGradientEditor", "Repeat Spread", nullptr));
#endif // BOBUI_CONFIG(tooltip)
        repeatButton->setText(QCoreApplication::translate("BobUIGradientEditor", "...", nullptr));
#if BOBUI_CONFIG(tooltip)
        reflectButton->setToolTip(QCoreApplication::translate("BobUIGradientEditor", "Reflect Spread", nullptr));
#endif // BOBUI_CONFIG(tooltip)
        reflectButton->setText(QCoreApplication::translate("BobUIGradientEditor", "...", nullptr));
    } // retranslateUi

};

namespace Ui {
    class BobUIGradientEditor: public Ui_BobUIGradientEditor {};
} // namespace Ui

BOBUI_END_NAMESPACE

#endif // BOBUIGRADIENTEDITOR_H
