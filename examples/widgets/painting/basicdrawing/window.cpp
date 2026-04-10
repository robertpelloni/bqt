// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

#include "renderarea.h"
#include "window.h"

#include <BobUIWidgets>

//! [0]
const int IdRole = BobUI::UserRole;
//! [0]

//! [1]
Window::Window()
{
    renderArea = new RenderArea;

    shapeComboBox = new QComboBox;
    shapeComboBox->addItem(tr("Polygon"), RenderArea::Polygon);
    shapeComboBox->addItem(tr("Rectangle"), RenderArea::Rect);
    shapeComboBox->addItem(tr("Rounded Rectangle"), RenderArea::RoundedRect);
    shapeComboBox->addItem(tr("Ellipse"), RenderArea::Ellipse);
    shapeComboBox->addItem(tr("Pie"), RenderArea::Pie);
    shapeComboBox->addItem(tr("Chord"), RenderArea::Chord);
    shapeComboBox->addItem(tr("Path"), RenderArea::Path);
    shapeComboBox->addItem(tr("Line"), RenderArea::Line);
    shapeComboBox->addItem(tr("Polyline"), RenderArea::Polyline);
    shapeComboBox->addItem(tr("Arc"), RenderArea::Arc);
    shapeComboBox->addItem(tr("Points"), RenderArea::Points);
    shapeComboBox->addItem(tr("Text"), RenderArea::Text);
    shapeComboBox->addItem(tr("Pixmap"), RenderArea::Pixmap);

    shapeLabel = new QLabel(tr("&Shape:"));
    shapeLabel->setBuddy(shapeComboBox);
//! [1]

//! [2]
    penWidthSpinBox = new QSpinBox;
    penWidthSpinBox->setRange(0, 20);
    penWidthSpinBox->setSpecialValueText(tr("0 (cosmetic pen)"));

    penWidthLabel = new QLabel(tr("Pen &Width:"));
    penWidthLabel->setBuddy(penWidthSpinBox);
//! [2]

//! [3]
    penStyleComboBox = new QComboBox;
    penStyleComboBox->addItem(tr("Solid"), static_cast<int>(BobUI::SolidLine));
    penStyleComboBox->addItem(tr("Dash"), static_cast<int>(BobUI::DashLine));
    penStyleComboBox->addItem(tr("Dot"), static_cast<int>(BobUI::DotLine));
    penStyleComboBox->addItem(tr("Dash Dot"), static_cast<int>(BobUI::DashDotLine));
    penStyleComboBox->addItem(tr("Dash Dot Dot"), static_cast<int>(BobUI::DashDotDotLine));
    penStyleComboBox->addItem(tr("None"), static_cast<int>(BobUI::NoPen));

    penStyleLabel = new QLabel(tr("&Pen Style:"));
    penStyleLabel->setBuddy(penStyleComboBox);

    penCapComboBox = new QComboBox;
    penCapComboBox->addItem(tr("Flat"), BobUI::FlatCap);
    penCapComboBox->addItem(tr("Square"), BobUI::SquareCap);
    penCapComboBox->addItem(tr("Round"), BobUI::RoundCap);

    penCapLabel = new QLabel(tr("Pen &Cap:"));
    penCapLabel->setBuddy(penCapComboBox);

    penJoinComboBox = new QComboBox;
    penJoinComboBox->addItem(tr("Miter"), BobUI::MiterJoin);
    penJoinComboBox->addItem(tr("Bevel"), BobUI::BevelJoin);
    penJoinComboBox->addItem(tr("Round"), BobUI::RoundJoin);

    penJoinLabel = new QLabel(tr("Pen &Join:"));
    penJoinLabel->setBuddy(penJoinComboBox);
//! [3]

//! [4]
    brushStyleComboBox = new QComboBox;
    brushStyleComboBox->addItem(tr("Linear Gradient"),
            static_cast<int>(BobUI::LinearGradientPattern));
    brushStyleComboBox->addItem(tr("Radial Gradient"),
            static_cast<int>(BobUI::RadialGradientPattern));
    brushStyleComboBox->addItem(tr("Conical Gradient"),
            static_cast<int>(BobUI::ConicalGradientPattern));
    brushStyleComboBox->addItem(tr("Texture"), static_cast<int>(BobUI::TexturePattern));
    brushStyleComboBox->addItem(tr("Solid"), static_cast<int>(BobUI::SolidPattern));
    brushStyleComboBox->addItem(tr("Horizontal"), static_cast<int>(BobUI::HorPattern));
    brushStyleComboBox->addItem(tr("Vertical"), static_cast<int>(BobUI::VerPattern));
    brushStyleComboBox->addItem(tr("Cross"), static_cast<int>(BobUI::CrossPattern));
    brushStyleComboBox->addItem(tr("Backward Diagonal"), static_cast<int>(BobUI::BDiagPattern));
    brushStyleComboBox->addItem(tr("Forward Diagonal"), static_cast<int>(BobUI::FDiagPattern));
    brushStyleComboBox->addItem(tr("Diagonal Cross"), static_cast<int>(BobUI::DiagCrossPattern));
    brushStyleComboBox->addItem(tr("Dense 1"), static_cast<int>(BobUI::Dense1Pattern));
    brushStyleComboBox->addItem(tr("Dense 2"), static_cast<int>(BobUI::Dense2Pattern));
    brushStyleComboBox->addItem(tr("Dense 3"), static_cast<int>(BobUI::Dense3Pattern));
    brushStyleComboBox->addItem(tr("Dense 4"), static_cast<int>(BobUI::Dense4Pattern));
    brushStyleComboBox->addItem(tr("Dense 5"), static_cast<int>(BobUI::Dense5Pattern));
    brushStyleComboBox->addItem(tr("Dense 6"), static_cast<int>(BobUI::Dense6Pattern));
    brushStyleComboBox->addItem(tr("Dense 7"), static_cast<int>(BobUI::Dense7Pattern));
    brushStyleComboBox->addItem(tr("None"), static_cast<int>(BobUI::NoBrush));

    brushStyleLabel = new QLabel(tr("&Brush:"));
    brushStyleLabel->setBuddy(brushStyleComboBox);
//! [4]

//! [5]
    otherOptionsLabel = new QLabel(tr("Options:"));
//! [5] //! [6]
    antialiasingCheckBox = new QCheckBox(tr("&Antialiasing"));
//! [6] //! [7]
    transformationsCheckBox = new QCheckBox(tr("&Transformations"));
//! [7]

//! [8]
    connect(shapeComboBox, &QComboBox::activated,
            this, &Window::shapeChanged);
    connect(penWidthSpinBox, &QSpinBox::valueChanged,
            this, &Window::penChanged);
    connect(penStyleComboBox, &QComboBox::activated,
            this, &Window::penChanged);
    connect(penCapComboBox, &QComboBox::activated,
            this, &Window::penChanged);
    connect(penJoinComboBox, &QComboBox::activated,
            this, &Window::penChanged);
    connect(brushStyleComboBox, &QComboBox::activated,
            this, &Window::brushChanged);
    connect(antialiasingCheckBox, &QAbstractButton::toggled,
            renderArea, &RenderArea::setAntialiased);
    connect(transformationsCheckBox, &QAbstractButton::toggled,
            renderArea, &RenderArea::setTransformed);
//! [8]

//! [9]
    QGridLayout *mainLayout = new QGridLayout;
//! [9] //! [10]
    mainLayout->setColumnStretch(0, 1);
    mainLayout->setColumnStretch(3, 1);
    mainLayout->addWidget(renderArea, 0, 0, 1, 4);
    mainLayout->addWidget(shapeLabel, 2, 0, BobUI::AlignRight);
    mainLayout->addWidget(shapeComboBox, 2, 1);
    mainLayout->addWidget(penWidthLabel, 3, 0, BobUI::AlignRight);
    mainLayout->addWidget(penWidthSpinBox, 3, 1);
    mainLayout->addWidget(penStyleLabel, 4, 0, BobUI::AlignRight);
    mainLayout->addWidget(penStyleComboBox, 4, 1);
    mainLayout->addWidget(penCapLabel, 3, 2, BobUI::AlignRight);
    mainLayout->addWidget(penCapComboBox, 3, 3);
    mainLayout->addWidget(penJoinLabel, 2, 2, BobUI::AlignRight);
    mainLayout->addWidget(penJoinComboBox, 2, 3);
    mainLayout->addWidget(brushStyleLabel, 4, 2, BobUI::AlignRight);
    mainLayout->addWidget(brushStyleComboBox, 4, 3);
    mainLayout->addWidget(otherOptionsLabel, 5, 0, BobUI::AlignRight);
    mainLayout->addWidget(antialiasingCheckBox, 5, 1, 1, 1, BobUI::AlignRight);
    mainLayout->addWidget(transformationsCheckBox, 5, 2, 1, 2, BobUI::AlignRight);
    setLayout(mainLayout);

    shapeChanged();
    penChanged();
    brushChanged();
    antialiasingCheckBox->setChecked(true);

    setWindowTitle(tr("Basic Drawing"));
}
//! [10]

//! [11]
void Window::shapeChanged()
{
    RenderArea::Shape shape = RenderArea::Shape(shapeComboBox->itemData(
            shapeComboBox->currentIndex(), IdRole).toInt());
    renderArea->setShape(shape);
}
//! [11]

//! [12]
void Window::penChanged()
{
    int width = penWidthSpinBox->value();
    BobUI::PenStyle style = BobUI::PenStyle(penStyleComboBox->itemData(
            penStyleComboBox->currentIndex(), IdRole).toInt());
    BobUI::PenCapStyle cap = BobUI::PenCapStyle(penCapComboBox->itemData(
            penCapComboBox->currentIndex(), IdRole).toInt());
    BobUI::PenJoinStyle join = BobUI::PenJoinStyle(penJoinComboBox->itemData(
            penJoinComboBox->currentIndex(), IdRole).toInt());

    renderArea->setPen(QPen(BobUI::blue, width, style, cap, join));
}
//! [12]

//! [13]
void Window::brushChanged()
{
    BobUI::BrushStyle style = BobUI::BrushStyle(brushStyleComboBox->itemData(
//! [13]
            brushStyleComboBox->currentIndex(), IdRole).toInt());

//! [14]
    if (style == BobUI::LinearGradientPattern) {
        QLinearGradient linearGradient(0, 0, 100, 100);
        linearGradient.setColorAt(0.0, BobUI::white);
        linearGradient.setColorAt(0.2, BobUI::green);
        linearGradient.setColorAt(1.0, BobUI::black);
        renderArea->setBrush(linearGradient);
//! [14] //! [15]
    } else if (style == BobUI::RadialGradientPattern) {
        QRadialGradient radialGradient(50, 50, 50, 70, 70);
        radialGradient.setColorAt(0.0, BobUI::white);
        radialGradient.setColorAt(0.2, BobUI::green);
        radialGradient.setColorAt(1.0, BobUI::black);
        renderArea->setBrush(radialGradient);
    } else if (style == BobUI::ConicalGradientPattern) {
        QConicalGradient conicalGradient(50, 50, 150);
        conicalGradient.setColorAt(0.0, BobUI::white);
        conicalGradient.setColorAt(0.2, BobUI::green);
        conicalGradient.setColorAt(1.0, BobUI::black);
        renderArea->setBrush(conicalGradient);
//! [15] //! [16]
    } else if (style == BobUI::TexturePattern) {
        renderArea->setBrush(QBrush(QPixmap(":/images/brick.png")));
//! [16] //! [17]
    } else {
        renderArea->setBrush(QBrush(BobUI::green, style));
    }
}
//! [17]
