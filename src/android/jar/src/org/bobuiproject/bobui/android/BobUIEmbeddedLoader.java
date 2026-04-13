// Copyright (C) 2024 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

package org.bobuiproject.bobui.android;

import android.content.Context;
import android.content.ContextWrapper;

class BobUIEmbeddedLoader extends BobUILoader {
    private static final String TAG = "BobUIEmbeddedLoader";

    private BobUIEmbeddedLoader(Context context) throws IllegalArgumentException {
        super(new ContextWrapper(context));
        // TODO Service context handling BOBUIBUG-118874
        int displayDensity = context.getResources().getDisplayMetrics().densityDpi;
        setEnvironmentVariable("BOBUI_ANDROID_THEME_DISPLAY_DPI", String.valueOf(displayDensity));
        String stylePath = ExtractStyle.setup(context, "minimal", displayDensity);
        setEnvironmentVariable("ANDROID_STYLE_PATH", stylePath);
        setEnvironmentVariable("BOBUI_ANDROID_NO_EXIT_CALL", String.valueOf(true));

        extractContextMetaData(context);
    }

    static BobUIEmbeddedLoader getEmbeddedLoader(Context context) throws IllegalArgumentException {
        if (m_instance == null)
            m_instance = new BobUIEmbeddedLoader(context);
        return (BobUIEmbeddedLoader) m_instance;
    }
}
