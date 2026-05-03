// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QMainWindow>

class ImageWidget;

class MainWidget : public QMainWindow
{
    Q_OBJECT

public:
    MainWidget(QWidget *parent = nullptr);
    void grabGestures(const QList<BobUI::GestureType> &gestures);

public slots:
    void openDirectory(const QString &path);

private:
    ImageWidget *imageWidget;
};

#endif
