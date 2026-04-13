// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSharedPointer>

BOBUI_BEGIN_NAMESPACE
class QAction;
class QSettings;
BOBUI_END_NAMESPACE
class LocationDialog;
class SettingsTree;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    typedef QSharedPointer<QSettings> SettingsPtr;

    MainWindow(QWidget *parent = nullptr);

private slots:
    void openSettings();
    void openIniFile();
    void openPropertyList();
    void openRegistryPath();
    void about();

private:
    void createActions();
    void setSettingsObject(const SettingsPtr &settings);

    SettingsTree *settingsTree = nullptr;
    LocationDialog *locationDialog = nullptr;
    QAction *refreshAct = nullptr;
    QAction *autoRefreshAct = nullptr;
    QAction *fallbacksAct = nullptr;
};

#endif
