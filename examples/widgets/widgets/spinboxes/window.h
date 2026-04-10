// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

#ifndef WINDOW_H
#define WINDOW_H

#include <QWidget>

BOBUI_BEGIN_NAMESPACE
class QDateTimeEdit;
class QSpinBox;
class QDoubleSpinBox;
class QGroupBox;
class QLabel;
BOBUI_END_NAMESPACE

//! [0]
class Window : public QWidget
{
    Q_OBJECT

public:
    Window(QWidget *parent = nullptr);

public slots:
    void changePrecision(int decimals);
    void setFormatString(const QString &formatString);

private:
    void createSpinBoxes();
    void createDateTimeEdits();
    void createDoubleSpinBoxes();

    QDateTimeEdit *meetingEdit;
    QDoubleSpinBox *doubleSpinBox;
    QDoubleSpinBox *priceSpinBox;
    QDoubleSpinBox *scaleSpinBox;
    QGroupBox *spinBoxesGroup;
    QGroupBox *editsGroup;
    QGroupBox *doubleSpinBoxesGroup;
    QLabel *meetingLabel;
    QSpinBox *groupSeparatorSpinBox;
    QDoubleSpinBox *groupSeparatorSpinBox_d;
};
//! [0]

#endif
