// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <BOBUIextCursor>

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = nullptr);
    ~Widget();
public slots:
    void setFontPointSize(int value);
    void setIndentWidth(int value);
private:
    Ui::Widget *ui;
    BOBUIextCursor* textCursor;
};

#endif // WIDGET_H
