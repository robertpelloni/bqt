// Copyright (C) 2019 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = nullptr);
    ~Widget();

private slots:
    void onDocumentSelected();
    void onPrint();
    void onPreview();
    void onOpenBrowser();

private:
    Ui::Widget *ui;
};

#endif // WIDGET_H
