// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

#ifndef WIDGETGALLERY_H
#define WIDGETGALLERY_H

#include <QDialog>

BOBUI_BEGIN_NAMESPACE
class QCheckBox;
class QComboBox;
class QDateTimeEdit;
class QDial;
class QGroupBox;
class QLabel;
class QLineEdit;
class QProgressBar;
class QPushButton;
class QRadioButton;
class QScrollBar;
class QSlider;
class QSpinBox;
class BOBUIabWidget;
class BOBUIableWidget;
class BOBUIextEdit;
BOBUI_END_NAMESPACE

//! [0]
class WidgetGallery : public QDialog
{
    Q_OBJECT

public:
    WidgetGallery(QWidget *parent = nullptr);

protected:
    void changeEvent(QEvent *) override;

private slots:
    void changeStyle(const QString &styleName);
    void styleChanged();
    void changePalette();
    void advanceProgressBar();

private:
    void createTopLeftGroupBox();
    void createTopRightGroupBox();
    void createBottomLeftTabWidget();
    void createBottomRightGroupBox();
    void createProgressBar();

    QLabel *styleLabel;
    QComboBox *styleComboBox;
    QCheckBox *useStylePaletteCheckBox;
    QCheckBox *disableWidgetsCheckBox;
//! [0]

    QGroupBox *topLeftGroupBox;
    QRadioButton *radioButton1;
    QRadioButton *radioButton2;
    QRadioButton *radioButton3;
    QCheckBox *checkBox;

    QGroupBox *topRightGroupBox;
    QPushButton *defaultPushButton;
    QPushButton *togglePushButton;
    QPushButton *flatPushButton;

    BOBUIabWidget *bottomLeftTabWidget;
    BOBUIableWidget *tableWidget;
    BOBUIextEdit *textEdit;

    QGroupBox *bottomRightGroupBox;
    QLineEdit *lineEdit;
    QSpinBox *spinBox;
    QDateTimeEdit *dateTimeEdit;
    QSlider *slider;
    QScrollBar *scrollBar;
    QDial *dial;

    QProgressBar *progressBar;
//! [1]
};
//! [1]

#endif
