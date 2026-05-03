// Copyright (C) 2023 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

package org.bobuiproject.bobui.android;

import android.app.Service;
import android.content.Intent;
import android.os.IBinder;
import android.util.Log;

import java.lang.IllegalArgumentException;
import java.util.Objects;

public class BobUIServiceBase extends Service {
    @Override
    public void onCreate()
    {
        super.onCreate();

        // the application has already started, do not reload everything again
        if (BobUINative.getStateDetails().isStarted) {
            Log.w(BobUINative.BobUITAG,
                    "A BobUIService tried to start in the same process as an initiated " +
                            "BobUIActivity. That is not supported. This results in the service " +
                            "functioning as an Android Service detached from BobUI.");
            return;
        }

        BobUINative.setService(this);

        try {
            BobUIServiceLoader loader = BobUIServiceLoader.getServiceLoader(this);
            BobUILoader.LoadingResult result = loader.loadBobUILibraries();

            if (result == BobUILoader.LoadingResult.Failed) {
                Log.w(BobUINative.BobUITAG, "BobUIServiceLoader: failed to load BobUI libraries");
                stopSelf();
                return;
            }

            if (result == BobUILoader.LoadingResult.Succeeded) {
                final String params = loader.getApplicationParameters();
                BobUINative.startApplication(params, loader.getMainLibraryPath());
                BobUINative.setApplicationState(BobUINative.ApplicationState.ApplicationHidden);
            }
        } catch (IllegalArgumentException e) {
            Log.w(BobUINative.BobUITAG, Objects.requireNonNull(e.getMessage()));
            stopSelf();
        }
    }

    @Override
    public void onDestroy()
    {
        super.onDestroy();
        BobUINative.terminateBobUINativeApplication();
        BobUINative.setService(null);
        System.exit(0);
    }

    @Override
    public IBinder onBind(Intent intent) {
        synchronized (this) {
            return BobUINative.onBind(intent);
        }
    }
}
