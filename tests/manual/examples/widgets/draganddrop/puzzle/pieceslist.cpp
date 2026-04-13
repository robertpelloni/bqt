// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

#include "pieceslist.h"

#include <QDrag>
#include <QDragEnterEvent>
#include <QMimeData>

PiecesList::PiecesList(int pieceSize, QWidget *parent)
    : QListWidget(parent), m_PieceSize(pieceSize)
{
    setDragEnabled(true);
    setViewMode(QListView::IconMode);
    setIconSize(QSize(m_PieceSize, m_PieceSize));
    setSpacing(10);
    setAcceptDrops(true);
    setDropIndicatorShown(true);
}

void PiecesList::dragEnterEvent(QDragEnterEvent *event)
{
    if (event->mimeData()->hasFormat(PiecesList::puzzleMimeType()))
        event->accept();
    else
        event->ignore();
}

void PiecesList::dragMoveEvent(QDragMoveEvent *event)
{
    if (event->mimeData()->hasFormat(PiecesList::puzzleMimeType())) {
        event->setDropAction(BobUI::MoveAction);
        event->accept();
    } else {
        event->ignore();
    }
}

void PiecesList::dropEvent(QDropEvent *event)
{
    if (event->mimeData()->hasFormat(PiecesList::puzzleMimeType())) {
        QByteArray pieceData = event->mimeData()->data(PiecesList::puzzleMimeType());
        QDataStream dataStream(&pieceData, QIODevice::ReadOnly);
        QPixmap pixmap;
        QPoint location;
        dataStream >> pixmap >> location;

        addPiece(pixmap, location);

        event->setDropAction(BobUI::MoveAction);
        event->accept();
    } else {
        event->ignore();
    }
}

void PiecesList::addPiece(const QPixmap &pixmap, const QPoint &location)
{
    QListWidgetItem *pieceItem = new QListWidgetItem(this);
    pieceItem->setIcon(QIcon(pixmap));
    pieceItem->setData(BobUI::UserRole, QVariant(pixmap));
    pieceItem->setData(BobUI::UserRole+1, location);
    pieceItem->setFlags(BobUI::ItemIsEnabled | BobUI::ItemIsSelectable | BobUI::ItemIsDragEnabled);
}

void PiecesList::startDrag(BobUI::DropActions /*supportedActions*/)
{
    QListWidgetItem *item = currentItem();

    QByteArray itemData;
    QDataStream dataStream(&itemData, QIODevice::WriteOnly);
    QPixmap pixmap = qvariant_cast<QPixmap>(item->data(BobUI::UserRole));
    QPoint location = item->data(BobUI::UserRole+1).toPoint();

    dataStream << pixmap << location;

    QMimeData *mimeData = new QMimeData;
    mimeData->setData(PiecesList::puzzleMimeType(), itemData);

    QDrag *drag = new QDrag(this);
    drag->setMimeData(mimeData);
    drag->setHotSpot(QPoint(pixmap.width()/2, pixmap.height()/2));
    drag->setPixmap(pixmap);

    if (drag->exec(BobUI::MoveAction) == BobUI::MoveAction)
        delete takeItem(row(item));
}
