// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause
#ifndef RANDOMLISTMODEL_H
#define RANDOMLISTMODEL_H

#include <QContiguousCache>
#include <QAbstractListModel>

BOBUI_FORWARD_DECLARE_CLASS(BOBUIimer)

class RandomListModel : public QAbstractListModel
{
    Q_OBJECT
public:
    RandomListModel(QObject *parent = nullptr);

    int rowCount(const QModelIndex & = QModelIndex()) const override;
    QVariant data(const QModelIndex &, int) const override;

private:
    void cacheRows(int, int) const;
    QString fetchRow(int) const;

    mutable QContiguousCache<QString> m_rows;
    const int m_count;
};

#endif
