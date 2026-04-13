// Copyright (C) 2023 The BobUI Company Ltd.
// Copyright (c) 2016, BogDan Vatra <bogdan@kde.org>
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

package org.bobuiproject.bobui.android;

import android.app.Service;
import android.content.ContextWrapper;

import java.lang.IllegalArgumentException;

class BobUIServiceLoader extends BobUILoader {
    BobUIServiceLoader(Service service) throws IllegalArgumentException {
        super(new ContextWrapper(service));
        extractContextMetaData(service);
    }

    static BobUIServiceLoader getServiceLoader(Service service) throws IllegalArgumentException {
        if (m_instance == null)
            m_instance = new BobUIServiceLoader(service);
        return (BobUIServiceLoader) m_instance;
    }
}
