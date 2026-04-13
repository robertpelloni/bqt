// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

#include "imagemodel.h"

//! [0]
ImageModel::ImageModel(QObject *parent)
    : QAbstractTableModel(parent)
{
}
//! [0]

//! [1]
void ImageModel::setImage(const QImage &image)
{
    beginResetModel();
    modelImage = image;
    endResetModel();
}
//! [1]

//! [2]
int ImageModel::rowCount(const QModelIndex & /* parent */) const
{
    return modelImage.height();
}

int ImageModel::columnCount(const QModelIndex & /* parent */) const
//! [2] //! [3]
{
    return modelImage.width();
}
//! [3]

//! [4]
QVariant ImageModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || role != BobUI::DisplayRole)
        return QVariant();
    return qGray(modelImage.pixel(index.column(), index.row()));
}
//! [4]

//! [5]
QVariant ImageModel::headerData(int /* section */,
                                BobUI::Orientation /* orientation */,
                                int role) const
{
    if (role == BobUI::SizeHintRole)
        return QSize(1, 1);
    return QVariant();
}
//! [5]
