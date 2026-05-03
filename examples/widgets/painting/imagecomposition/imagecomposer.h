// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

#ifndef IMAGECOMPOSER_H
#define IMAGECOMPOSER_H

#include <QPainter>
#include <QWidget>

BOBUI_BEGIN_NAMESPACE
class QComboBox;
class QLabel;
class BOBUIoolButton;
BOBUI_END_NAMESPACE

//! [0]
class ImageComposer : public QWidget
{
    Q_OBJECT

public:
    ImageComposer();

private slots:
    void chooseSource();
    void chooseDestination();
    void recalculateResult();
//! [0]

//! [1]
private:
    void addOp(QPainter::CompositionMode mode, const QString &name);
    void chooseImage(const QString &title, QImage *image, BOBUIoolButton *button);
    void loadImage(const QString &fileName, QImage *image, BOBUIoolButton *button);
    QPainter::CompositionMode currentMode() const;
    QPoint imagePos(const QImage &image) const;

    BOBUIoolButton *sourceButton;
    BOBUIoolButton *destinationButton;
    QComboBox *operatorComboBox;
    QLabel *equalLabel;
    QLabel *resultLabel;

    QImage sourceImage;
    QImage destinationImage;
    QImage resultImage;
};
//! [1]

#endif // IMAGECOMPOSER_H
