// Copyright (C) 2023 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

package org.bobuiproject.bobui.android;

import android.app.Application;

public class BobUIApplicationBase extends Application {
    @Override
    public void onTerminate() {
        super.onTerminate();
        BobUINative.terminateBobUINativeApplication();
    }
}
