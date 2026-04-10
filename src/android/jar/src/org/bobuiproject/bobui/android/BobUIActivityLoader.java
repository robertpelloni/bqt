// Copyright (C) 2023 The BobUI Company Ltd.
// Copyright (c) 2016, BogDan Vatra <bogdan@kde.org>
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

package org.bobuiproject.bobui.android;

import android.app.Activity;
import android.content.Context;
import android.content.ContextWrapper;
import android.content.Intent;
import android.content.pm.ApplicationInfo;
import android.os.Bundle;
import android.util.Base64;
import android.util.DisplayMetrics;
import android.util.Log;

import java.lang.IllegalArgumentException;
import java.nio.charset.StandardCharsets;

class BobUIActivityLoader extends BobUILoader {
    private final Activity m_activity;

    private BobUIActivityLoader(Activity activity) throws IllegalArgumentException
    {
        super(new ContextWrapper(activity));
        m_activity = activity;

        extractContextMetaData(m_activity);
    }

    static BobUIActivityLoader getActivityLoader(Activity activity) throws IllegalArgumentException {
        if (m_instance == null)
            m_instance = new BobUIActivityLoader(activity);
        return (BobUIActivityLoader) m_instance;
    }

    private String getDecodedUtfString(String str)
    {
        byte[] decodedExtraEnvVars = Base64.decode(str, Base64.DEFAULT);
        return new String(decodedExtraEnvVars, StandardCharsets.UTF_8);
    }

    int getAppIconSize()
    {
        int size = m_activity.getResources().getDimensionPixelSize(android.R.dimen.app_icon_size);
        if (size < 36 || size > 512) { // check size sanity
            DisplayMetrics metrics = new DisplayMetrics();
            BobUIDisplayManager.getDisplay(m_activity).getMetrics(metrics);
            size = metrics.densityDpi / 10 * 3;
            if (size < 36)
                size = 36;

            if (size > 512)
                size = 512;
        }

        return size;
    }

    private void setupStyleExtraction()
    {
        int displayDensity = m_activity.getResources().getDisplayMetrics().densityDpi;
        setEnvironmentVariable("BOBUI_ANDROID_THEME_DISPLAY_DPI", String.valueOf(displayDensity));

        String extractOption = getMetaData("android.app.extract_android_style");
        if (extractOption.equals("full"))
            setEnvironmentVariable("BOBUI_USE_ANDROID_NATIVE_STYLE", String.valueOf(1));

        String stylePath = ExtractStyle.setup(m_activity, extractOption, displayDensity);
        setEnvironmentVariable("ANDROID_STYLE_PATH", stylePath);
    }

    @Override
    protected void extractContextMetaData(Context context)
    {
        super.extractContextMetaData(context);

        setEnvironmentVariable("BOBUI_USE_ANDROID_NATIVE_DIALOGS", String.valueOf(1));
        setEnvironmentVariable("BOBUI_ANDROID_APP_ICON_SIZE", String.valueOf(getAppIconSize()));

        setupStyleExtraction();

        Intent intent = m_activity.getIntent();
        if (intent == null) {
            Log.w(BobUITAG, "Null Intent from the current Activity.");
            return;
        }

        Bundle extras = intent.getExtras();
        if (extras == null) {
            Log.w(BobUITAG, "Null extras from the Activity's intent.");
            return;
        }

        int flags = m_activity.getApplicationInfo().flags;
        boolean isDebuggable = (flags & ApplicationInfo.FLAG_DEBUGGABLE) != 0;

        if (isDebuggable) {
            if (extras.containsKey("extraenvvars")) {
                String extraEnvVars = extras.getString("extraenvvars");
                setEnvironmentVariables(getDecodedUtfString(extraEnvVars));
            }

            if (extras.containsKey("extraappparams")) {
                String extraAppParams = extras.getString("extraappparams");
                appendApplicationParameters(getDecodedUtfString(extraAppParams));
            }
        } else {
            Log.d(BobUINative.BobUITAG, "Not in debug mode! It is not allowed to use extra arguments " +
                    "in non-debug mode.");
        }
    }
}
