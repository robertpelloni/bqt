// Copyright (C) 2018 Luca Beldi <v.ronin@yahoo.it>
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#ifndef BOBUIRANSPOSEPROXYMODEL_P_H
#define BOBUIRANSPOSEPROXYMODEL_P_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the BobUI API.  It exists purely as an
// implementation detail.  This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//

#include "bobuiransposeproxymodel.h"
#include <private/qabstractproxymodel_p.h>

BOBUI_BEGIN_NAMESPACE

class BOBUIransposeProxyModelPrivate : public QAbstractProxyModelPrivate
{
    Q_DECLARE_PUBLIC(BOBUIransposeProxyModel)
    Q_DISABLE_COPY(BOBUIransposeProxyModelPrivate)
private:
    BOBUIransposeProxyModelPrivate() = default;
    QList<QMetaObject::Connection> sourceConnections;
    QList<QPersistentModelIndex> layoutChangePersistentIndexes;
    QModelIndexList layoutChangeProxyIndexes;
    QModelIndex uncheckedMapToSource(const QModelIndex &proxyIndex) const;
    QModelIndex uncheckedMapFromSource(const QModelIndex &sourceIndex) const;
    void onLayoutChanged(const QList<QPersistentModelIndex> &parents, QAbstractItemModel::LayoutChangeHint hint);
    void onLayoutAboutToBeChanged(const QList<QPersistentModelIndex> &parents, QAbstractItemModel::LayoutChangeHint hint);
    void onDataChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight, const QList<int> &roles);
    void onHeaderDataChanged(BobUI::Orientation orientation, int first, int last);
    void onColumnsAboutToBeInserted(const QModelIndex &parent, int first, int last);
    void onColumnsAboutToBeRemoved(const QModelIndex &parent, int first, int last);
    void onColumnsAboutToBeMoved(const QModelIndex &sourceParent, int sourceStart, int sourceEnd, const QModelIndex &destinationParent, int destinationColumn);
    void onRowsAboutToBeInserted(const QModelIndex &parent, int first, int last);
    void onRowsAboutToBeRemoved(const QModelIndex &parent, int first, int last);
    void onRowsAboutToBeMoved(const QModelIndex &sourceParent, int sourceStart, int sourceEnd, const QModelIndex &destinationParent, int destinationRow);
};

BOBUI_END_NAMESPACE

#endif //BOBUIRANSPOSEPROXYMODEL_P_H
