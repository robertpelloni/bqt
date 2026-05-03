// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

#ifndef ADDRESSWIDGET_H
#define ADDRESSWIDGET_H

#include "newaddresstab.h"
#include "tablemodel.h"

#include <QItemSelection>
#include <BOBUIabWidget>
#include <QStandardPaths>

BOBUI_BEGIN_NAMESPACE
class QSortFilterProxyModel;
class QItemSelectionModel;
BOBUI_END_NAMESPACE

//! [0]
class AddressWidget : public BOBUIabWidget
{
    Q_OBJECT

public:
    AddressWidget(QWidget *parent = nullptr);
    void readFromFile();
    void writeToFile();

public slots:
    void showAddEntryDialog();
    void addEntry(const QString &name, const QString &address);
    void editEntry();
    void removeEntry();

signals:
    void selectionChanged (const QItemSelection &selected);

private:
    void setupTabs();

    inline static QString fileName =
        QStandardPaths::standardLocations(QStandardPaths::TempLocation).value(0)
        + QStringLiteral("/addressbook.dat");
    TableModel *table;
    NewAddressTab *newAddressTab;
};
//! [0]

#endif // ADDRESSWIDGET_H
