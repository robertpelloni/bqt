// Copyright (C) 2024 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

package org.bobuiproject.bobui.android;

import android.content.Context;
import android.app.Activity;
import android.app.Service;

import java.util.HashMap;

class BobUIEmbeddedViewInterfaceFactory {
    private static final HashMap<Context, BobUIEmbeddedViewInterface> m_interfaces = new HashMap<>();
    private static final Object m_interfaceLock = new Object();

    static BobUIEmbeddedViewInterface create(Context context) {
        synchronized (m_interfaceLock) {
            if (!m_interfaces.containsKey(context)) {
                if (context instanceof Activity)
                    m_interfaces.put(context, new BobUIEmbeddedDelegate((Activity)context));
                else if (context instanceof Service)
                    m_interfaces.put(context, new BobUIServiceEmbeddedDelegate((Service)context));
            }

            return m_interfaces.get(context);
        }
    }

    static void remove(Context context) {
        synchronized (m_interfaceLock) {
            m_interfaces.remove(context);
        }
    }
}
