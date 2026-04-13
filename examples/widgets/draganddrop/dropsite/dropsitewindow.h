// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

#ifndef DROPSITEWINDOW_H
#define DROPSITEWINDOW_H

#include <QWidget>

BOBUI_BEGIN_NAMESPACE
class QDialogButtonBox;
class QLabel;
class QMimeData;
class QPushButton;
class BOBUIableWidget;
BOBUI_END_NAMESPACE
class DropArea;

//! [DropSiteWindow header]
class DropSiteWindow : public QWidget
{
    Q_OBJECT

public:
    DropSiteWindow();

public slots:
    void updateFormatsTable(const QMimeData *mimeData);
    void copy();

private:
    DropArea *dropArea;
    QLabel *abstractLabel;
    BOBUIableWidget *formatsTable;

    QPushButton *clearButton;
    QPushButton *copyButton;
    QPushButton *quitButton;
    QDialogButtonBox *buttonBox;
};
//! [DropSiteWindow header]

#endif // DROPSITEWINDOW_H
