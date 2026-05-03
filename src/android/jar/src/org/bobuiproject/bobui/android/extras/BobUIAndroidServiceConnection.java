// Copyright (C) 2017 BogDan Vatra <bogdan@kde.org>
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

package org.bobuiproject.bobui.android.extras;

import android.content.ComponentName;
import android.content.ServiceConnection;
import android.os.IBinder;

import org.bobuiproject.bobui.android.UsedFromNativeCode;

class BobUIAndroidServiceConnection implements ServiceConnection
{
    @UsedFromNativeCode
    BobUIAndroidServiceConnection(long id)
    {
        m_id = id;
    }

    void setId(long id)
    {
        synchronized(this)
        {
            m_id = id;
        }
    }

    @Override
    public void onServiceConnected(ComponentName name, IBinder service)
    {
        synchronized(this) {
            BobUINative.onServiceConnected(m_id, name.flattenToString(), service);
        }
    }

    @Override
    public void onServiceDisconnected(ComponentName name)
    {
        synchronized(this) {
            BobUINative.onServiceDisconnected(m_id, name.flattenToString());
        }
    }

    private long m_id;
}
