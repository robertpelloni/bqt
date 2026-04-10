// Copyright (C) 2024 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only
#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>

BOBUI_BEGIN_NAMESPACE
namespace Ui { class Widget; }
BOBUI_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

private:
    Ui::Widget *ui;
};
#endif // WIDGET_H
