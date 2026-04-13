// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

#include "piecesmodel.h"

#include <QIcon>
#include <QMimeData>
#include <QRandomGenerator>

PiecesModel::PiecesModel(int pieceSize, QObject *parent)
    : QAbstractListModel(parent), m_PieceSize(pieceSize)
{
}

QVariant PiecesModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (role == BobUI::DecorationRole)
        return QIcon(pixmaps.value(index.row()).scaled(m_PieceSize, m_PieceSize,
                         BobUI::KeepAspectRatio, BobUI::SmoothTransformation));
    else if (role == BobUI::UserRole)
        return pixmaps.value(index.row());
    else if (role == BobUI::UserRole + 1)
        return locations.value(index.row());

    return QVariant();
}

void PiecesModel::addPiece(const QPixmap &pixmap, const QPoint &location)
{
    int row;
    if (QRandomGenerator::global()->bounded(2) == 1)
        row = 0;
    else
        row = pixmaps.size();

    beginInsertRows(QModelIndex(), row, row);
    pixmaps.insert(row, pixmap);
    locations.insert(row, location);
    endInsertRows();
}

BobUI::ItemFlags PiecesModel::flags(const QModelIndex &index) const
{
    if (index.isValid())
        return (QAbstractListModel::flags(index)|BobUI::ItemIsDragEnabled);

    return BobUI::ItemIsDropEnabled;
}

bool PiecesModel::removeRows(int row, int count, const QModelIndex &parent)
{
    if (parent.isValid())
        return false;

    if (row >= pixmaps.size() || row + count <= 0)
        return false;

    int beginRow = qMax(0, row);
    int endRow = qMin(row + count - 1, pixmaps.size() - 1);

    beginRemoveRows(parent, beginRow, endRow);

    while (beginRow <= endRow) {
        pixmaps.removeAt(beginRow);
        locations.removeAt(beginRow);
        ++beginRow;
    }

    endRemoveRows();
    return true;
}

QStringList PiecesModel::mimeTypes() const
{
    QStringList types;
    types << "image/x-puzzle-piece";
    return types;
}

QMimeData *PiecesModel::mimeData(const QModelIndexList &indexes) const
{
    QMimeData *mimeData = new QMimeData();
    QByteArray encodedData;

    QDataStream stream(&encodedData, QDataStream::WriteOnly);

    for (const QModelIndex &index : indexes) {
        if (index.isValid()) {
            QPixmap pixmap = qvariant_cast<QPixmap>(data(index, BobUI::UserRole));
            QPoint location = data(index, BobUI::UserRole+1).toPoint();
            stream << pixmap << location;
        }
    }

    mimeData->setData("image/x-puzzle-piece", encodedData);
    return mimeData;
}

bool PiecesModel::dropMimeData(const QMimeData *data, BobUI::DropAction action,
                               int row, int column, const QModelIndex &parent)
{
    if (!data->hasFormat("image/x-puzzle-piece"))
        return false;

    if (action == BobUI::IgnoreAction)
        return true;

    if (column > 0)
        return false;

    int endRow;

    if (!parent.isValid()) {
        if (row < 0)
            endRow = pixmaps.size();
        else
            endRow = qMin(row, pixmaps.size());
    } else {
        endRow = parent.row();
    }

    QByteArray encodedData = data->data("image/x-puzzle-piece");
    QDataStream stream(&encodedData, QDataStream::ReadOnly);

    while (!stream.atEnd()) {
        QPixmap pixmap;
        QPoint location;
        stream >> pixmap >> location;

        beginInsertRows(QModelIndex(), endRow, endRow);
        pixmaps.insert(endRow, pixmap);
        locations.insert(endRow, location);
        endInsertRows();

        ++endRow;
    }

    return true;
}

int PiecesModel::rowCount(const QModelIndex &parent) const
{
    return parent.isValid() ? 0 : pixmaps.size();
}

BobUI::DropActions PiecesModel::supportedDropActions() const
{
    return BobUI::CopyAction | BobUI::MoveAction;
}

void PiecesModel::addPieces(const QPixmap &pixmap)
{
    if (!pixmaps.isEmpty()) {
        beginRemoveRows(QModelIndex(), 0, pixmaps.size() - 1);
        pixmaps.clear();
        locations.clear();
        endRemoveRows();
    }
    for (int y = 0; y < 5; ++y) {
        for (int x = 0; x < 5; ++x) {
            QPixmap pieceImage = pixmap.copy(x*m_PieceSize, y*m_PieceSize, m_PieceSize, m_PieceSize);
            addPiece(pieceImage, QPoint(x, y));
        }
    }
}
