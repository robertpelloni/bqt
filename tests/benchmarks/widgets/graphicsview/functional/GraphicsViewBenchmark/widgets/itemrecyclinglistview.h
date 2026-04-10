// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

#ifndef ITEMRECYCLINGLISTVIEW_H
#define ITEMRECYCLINGLISTVIEW_H

#include "abstractitemview.h"

class ItemRecyclingListView : public AbstractItemView
{
public:
    ItemRecyclingListView(QGraphicsWidget * parent = nullptr);
    virtual ~ItemRecyclingListView();
    void setCurrentRow(const int row);
    int rows() const;
    virtual bool listItemCaching() const = 0;
    virtual void setListItemCaching(bool enabled) = 0;

protected:
    void rowsInserted(const QModelIndex &parent, int start, int end);
    void rowsRemoved(const QModelIndex &parent,int start,int end);

private:
    QModelIndex m_rootIndex;
};

#endif // ITEMRECYCLINGLISTVIEW_H
