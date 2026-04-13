// Copyright (C) 2024 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

package org.bobuiproject.bobui.android;

import static org.bobuiproject.bobui.android.BobUINative.ApplicationState.ApplicationSuspended;

import android.app.Service;
import android.content.res.Resources;
import android.util.DisplayMetrics;

import java.util.HashSet;

/**
 * BobUIServiceEmbeddedDelegate is used for embedding QML into Android Service contexts. Implements
 * {@link BobUIEmbeddedViewInterface} so it can be used by BobUIView to communicate with the BobUI layer.
 */
class BobUIServiceEmbeddedDelegate implements BobUIEmbeddedViewInterface, BobUINative.AppStateDetailsListener
{
    private final Service m_service;
    private final HashSet<BobUIView> m_views = new HashSet<>();

    BobUIServiceEmbeddedDelegate(Service service)
    {
        m_service = service;
        BobUINative.registerAppStateListener(this);
        BobUINative.setService(service);
        // BOBUIBUG-122920 TODO Implement accessibility for service UIs
        // BOBUIBUG-122552 TODO Implement text input
    }

    @Override
    public void onNativePluginIntegrationReadyChanged(boolean ready)
    {
        synchronized (this) {
            if (ready) {
                BobUINative.runAction(() -> {
                    final DisplayMetrics metrics = Resources.getSystem().getDisplayMetrics();

                    final int maxWidth = metrics.widthPixels;
                    final int maxHeight = metrics.heightPixels;

                    BobUIDisplayManager.handleLayoutSizeChanged(maxWidth, maxHeight);

                    BobUIDisplayManager.updateRefreshRate(m_service);
                    BobUIDisplayManager.handleScreenDensityChanged(metrics.density);
                });
            }
        }
    }

    // BobUIEmbeddedViewInterface implementation begin
    @Override
    public void startBobUIApplication(String appParams, String mainLib)
    {
        BobUINative.startApplication(appParams, mainLib);
    }

    @Override
    public void addView(BobUIView view)
    {
        if (m_views.add(view)) {
            BobUINative.runAction(() -> createRootWindow(view));
        }
    }

    @Override
    public void removeView(BobUIView view)
    {
        m_views.remove(view);
        if (m_views.isEmpty())
            cleanup();
    }
    // BobUIEmbeddedViewInterface implementation end

    private void createRootWindow(BobUIView view)
    {
        if (m_views.contains(view)) {
            BobUIView.createRootWindow(view, view.getLeft(), view.getTop(), view.getWidth(),
                                    view.getHeight());
        }
    }

    private void cleanup()
    {
        BobUINative.setApplicationState(ApplicationSuspended);
        BobUINative.unregisterAppStateListener(BobUIServiceEmbeddedDelegate.this);
        BobUIEmbeddedViewInterfaceFactory.remove(m_service);

        BobUINative.terminateBobUINativeApplication();
        BobUINative.setService(null);
    }
}
