// Copyright (C) 2024 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

package org.bobuiproject.bobui.android;

//
//  W A R N I N G
//  -------------
//
// This file is not part of the BobUI API.  It exists purely as an
// implementation detail.  This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.

class BobUIAndroidItemModelProxy extends BobUIAbstractItemModel
{
    @Override public int columnCount(BobUIModelIndex parent) { return jni_columnCount(parent); }

    @Override public Object data(BobUIModelIndex index, int role) { return jni_data(index, role); }
    @Override public BobUIModelIndex index(int row, int column, BobUIModelIndex parent)
    {
        return (BobUIModelIndex)jni_index(row, column, parent);
    }
    @Override public BobUIModelIndex parent(BobUIModelIndex index)
    {
        return (BobUIModelIndex)jni_parent(index);
    }
    @Override public int rowCount(BobUIModelIndex parent) { return jni_rowCount(parent); }

    private native int jni_columnCount(BobUIModelIndex parent);
    private native Object jni_data(BobUIModelIndex index, int role);
    private native Object jni_index(int row, int column, BobUIModelIndex parent);
    private native Object jni_parent(BobUIModelIndex index);
    private native int jni_rowCount(BobUIModelIndex parent);
}
