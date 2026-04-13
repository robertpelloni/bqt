// Copyright (C) 2025 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

package org.bobuiproject.bobui.android.benchmark;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;

import org.bobuiproject.bobui.android.BobUIAbstractItemModel;
import org.bobuiproject.bobui.android.BobUIModelIndex;

public class BenchBobUIAbstractItemModel
        extends BobUIAbstractItemModel
{
    int m_rows = 1;
    int m_cols = 1;

    @Override
    public int columnCount(BobUIModelIndex parent)
    {
        return m_cols;
    }

    @Override
    public Object data(BobUIModelIndex index, int role)
    {
        return null;
    }

    @Override
    public BobUIModelIndex index(int row, int column, BobUIModelIndex parent)
    {
        return createIndex(row, column, 0);
    }

    @Override
    public BobUIModelIndex parent(BobUIModelIndex bobuiModelIndex)
    {
        return new BobUIModelIndex();
    }

    @Override
    public int rowCount(BobUIModelIndex parent)
    {
         return m_rows;
    }

    @Override
    public HashMap<Integer, String> roleNames()
    {
        final HashMap<Integer, String> roles = new HashMap<Integer, String>();
        roles.put(0, "integerRole");
        return roles;
    }
}
