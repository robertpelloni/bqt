// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

#ifndef TABDIALOG_H
#define TABDIALOG_H

#include <QDialog>

BOBUI_BEGIN_NAMESPACE
class QDialogButtonBox;
class QFileInfo;
class BOBUIabWidget;
BOBUI_END_NAMESPACE

//! [0]
class GeneralTab : public QWidget
{
    Q_OBJECT

public:
    explicit GeneralTab(const QFileInfo &fileInfo, QWidget *parent = nullptr);
};
//! [0]


//! [1]
class PermissionsTab : public QWidget
{
    Q_OBJECT

public:
    explicit PermissionsTab(const QFileInfo &fileInfo, QWidget *parent = nullptr);
};
//! [1]


//! [2]
class ApplicationsTab : public QWidget
{
    Q_OBJECT

public:
    explicit ApplicationsTab(const QFileInfo &fileInfo, QWidget *parent = nullptr);
};
//! [2]


//! [3]
class TabDialog : public QDialog
{
    Q_OBJECT

public:
    explicit TabDialog(const QString &fileName, QWidget *parent = nullptr);

private:
    BOBUIabWidget *tabWidget;
    QDialogButtonBox *buttonBox;
};
//! [3]

#endif
