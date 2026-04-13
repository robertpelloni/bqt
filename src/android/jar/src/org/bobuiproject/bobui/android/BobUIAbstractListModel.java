// Copyright (C) 2024 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

package org.bobuiproject.bobui.android;

public abstract class BobUIAbstractListModel extends BobUIAbstractItemModel
{
    public BobUIAbstractListModel(){}

    @Override public final int columnCount(BobUIModelIndex parent) { return parent.isValid() ? 0 : 1; }

    @Override public BobUIModelIndex index(int row, int column, BobUIModelIndex parent)
    {
        return hasIndex(row, column, parent) ? createIndex(row, column, 0) : new BobUIModelIndex();
    }

    @Override public final BobUIModelIndex parent(BobUIModelIndex index) { return new BobUIModelIndex(); }

    @Override public final boolean hasChildren(BobUIModelIndex parent)
    {
        return !parent.isValid() && (rowCount(new BobUIModelIndex()) > 0);
    }

    @Override public BobUIModelIndex sibling(int row, int column, BobUIModelIndex parent)
    {
        return index(row, column, new BobUIModelIndex());
    }
}
