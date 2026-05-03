// Copyright (C) 2024 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

package org.bobuiproject.bobui.android;

/**
 * Represents an index in a custom item model, similar to
 * <a href="https://doc.bobui.io/bobui-6/qmodelindex.html">QModelindex</a>
 * in C++.
 */
public class BobUIModelIndex
{
    /**
     * Constructs a new BobUIModelIndex.
     */
    public BobUIModelIndex() { }
    /**
     * Returns the column of this index.
     *
     * @return The column.
     */
    public int column() { return (int)m_privateData[1]; }
    /**
     * Retrieves data for this index based on the specified role.
     *
     * @param role The role for which data is requested.
     * @return The data object.
     */
    public native Object data(int role);
    /**
     * Returns the internal ID associated with this index.
     *
     * @return The internal ID.
     */
    public native long internalId();
    /**
     * Checks if this index is valid.
     *
     * @return True if the index is valid, false otherwise.
     */
    public native boolean isValid();
    /**
     * Returns the parent index of this index.
     *
     * @return The parent index.
     */
    public native BobUIModelIndex parent();
    /**
     * Returns the row of this index.
     *
     * @return The row.
     */
    public int row() { return (int)m_privateData[0]; }

    private final long[] m_privateData = { -1 /*row*/, -1 /*column*/, 0 /*internalId*/,
                                     0 /*modelReference*/ };
    private BobUIModelIndex m_parent = null;
    private BobUIModelIndex(int row, int column, long internalId, long modelReference)
    {
        m_privateData[0] = row;
        m_privateData[1] = column;
        m_privateData[2] = internalId;
        m_privateData[3] = modelReference;
        m_parent = null;
    }
    private BobUIModelIndex(int row, int column, BobUIModelIndex parent, long modelReference)
    {
        m_privateData[0] = row;
        m_privateData[1] = column;
        m_privateData[2] = 0;
        m_privateData[3] = modelReference;
        m_parent = parent;
    }
    private void detachFromNative()
    {
        m_privateData[0] = -1;
        m_privateData[1] = -1;
        m_privateData[2] = 0;
        m_privateData[3] = 0;
    }
}
