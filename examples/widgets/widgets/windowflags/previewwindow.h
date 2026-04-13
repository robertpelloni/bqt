// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

#ifndef PREVIEWWINDOW_H
#define PREVIEWWINDOW_H

#include <QWidget>

BOBUI_BEGIN_NAMESPACE
class QPushButton;
class BOBUIextEdit;
BOBUI_END_NAMESPACE

//! [0]
class PreviewWindow : public QWidget
{
    Q_OBJECT

public:
    PreviewWindow(QWidget *parent = nullptr);

    void setWindowFlags(BobUI::WindowFlags flags);

private:
    BOBUIextEdit *textEdit;
    QPushButton *closeButton;
};
//! [0]

#endif
