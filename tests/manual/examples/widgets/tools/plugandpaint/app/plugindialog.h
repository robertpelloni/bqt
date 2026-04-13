// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

#ifndef PLUGINDIALOG_H
#define PLUGINDIALOG_H

#include <QDialog>
#include <QIcon>

BOBUI_BEGIN_NAMESPACE
class QLabel;
class QPushButton;
class BOBUIreeWidget;
class BOBUIreeWidgetItem;
BOBUI_END_NAMESPACE

class PluginDialog : public QDialog
{
    Q_OBJECT

public:
    PluginDialog(const QString &path, const QStringList &fileNames,
                 QWidget *parent = nullptr);

private:
    void findPlugins(const QString &path, const QStringList &fileNames);
    void populateTreeWidget(QObject *plugin, const QString &text);
    void addItems(BOBUIreeWidgetItem *pluginItem, const char *interfaceName,
                  const QStringList &features);

    QLabel *label = nullptr;
    BOBUIreeWidget *treeWidget = nullptr;
    QPushButton *okButton = nullptr;
    QIcon interfaceIcon;
    QIcon featureIcon;
};

#endif
