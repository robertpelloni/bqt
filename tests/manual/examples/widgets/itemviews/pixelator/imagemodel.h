// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

#ifndef IMAGEMODEL_H
#define IMAGEMODEL_H

#include <QAbstractTableModel>
#include <QImage>

//! [0]
class ImageModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    ImageModel(QObject *parent = nullptr);

    void setImage(const QImage &image);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = BobUI::DisplayRole) const override;
    QVariant headerData(int section, BobUI::Orientation orientation, int role = BobUI::DisplayRole) const override;

private:
    QImage modelImage;
};
//! [0]

#endif // IMAGEMODEL_H
