// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

#ifndef DOMITEM_H
#define DOMITEM_H

#include <QDomNode>
#include <QHash>

//! [0]
class DomItem
{
public:
    DomItem(const QDomNode &node, int row, DomItem *parent = nullptr);
    ~DomItem();
    DomItem *child(int i);
    DomItem *parent();
    QDomNode node() const;
    int row() const;

private:
    QDomNode domNode;
    QHash<int, DomItem *> childItems;
    DomItem *parentItem;
    int rowNumber;
};
//! [0]

#endif // DOMITEM_H
