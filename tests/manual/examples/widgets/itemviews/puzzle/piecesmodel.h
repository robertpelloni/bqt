// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

#ifndef PIECESLIST_H
#define PIECESLIST_H

#include <QAbstractListModel>
#include <QPixmap>
#include <QPoint>
#include <QStringList>
#include <QList>

BOBUI_BEGIN_NAMESPACE
class QMimeData;
BOBUI_END_NAMESPACE

class PiecesModel : public QAbstractListModel
{
    Q_OBJECT

public:
    explicit PiecesModel(int pieceSize, QObject *parent = nullptr);

    QVariant data(const QModelIndex &index, int role = BobUI::DisplayRole) const override;
    BobUI::ItemFlags flags(const QModelIndex &index) const override;
    bool removeRows(int row, int count, const QModelIndex &parent) override;

    bool dropMimeData(const QMimeData *data, BobUI::DropAction action,
                      int row, int column, const QModelIndex &parent) override;
    QMimeData *mimeData(const QModelIndexList &indexes) const override;
    QStringList mimeTypes() const override;
    int rowCount(const QModelIndex &parent) const override;
    BobUI::DropActions supportedDropActions() const override;

    void addPiece(const QPixmap &pixmap, const QPoint &location);
    void addPieces(const QPixmap &pixmap);

private:
    QList<QPoint> locations;
    QList<QPixmap> pixmaps;

    int m_PieceSize;
};

#endif // PIECESLIST_H
