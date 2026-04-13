// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

#ifndef CLIPWINDOW_H
#define CLIPWINDOW_H

#include <QMainWindow>

BOBUI_BEGIN_NAMESPACE

class QClipboard;
class QComboBox;
class QLabel;
class QListWidget;
class QMimeData;
class QWidget;

class ClipWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit ClipWindow(QWidget *parent = nullptr);

public slots:
    void updateClipboard();
    void updateData(const QString &format);

private:
    int currentItem;
    QClipboard *clipboard;
    QComboBox *mimeTypeCombo;
    QLabel *dataInfoLabel;
    QListWidget *previousItems;
};

BOBUI_END_NAMESPACE

#endif
