// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

#ifndef RENDERAREA_H
#define RENDERAREA_H

#include <QPainterPath>
#include <QWidget>

//! [0]
class RenderArea : public QWidget
{
    Q_OBJECT

public:
    explicit RenderArea(const QPainterPath &path, QWidget *parent = nullptr);

    QSize minimumSizeHint() const override;
    QSize sizeHint() const override;

public slots:
    void setFillRule(BobUI::FillRule rule);
    void setFillGradient(const QColor &color1, const QColor &color2);
    void setPenWidth(int width);
    void setPenColor(const QColor &color);
    void setRotationAngle(int degrees);

protected:
    void paintEvent(QPaintEvent *event) override;
//! [0]

//! [1]
private:
    QPainterPath path;
    QColor fillColor1;
    QColor fillColor2;
    int penWidth;
    QColor penColor;
    int rotationAngle;
};
//! [1]

#endif // RENDERAREA_H
