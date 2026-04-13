// Copyright (C) 2024 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

package org.bobuiproject.bobui.android;

import static org.bobuiproject.bobui.android.BobUINative.ApplicationState.*;

import android.app.Activity;
import android.app.Application;
import android.content.res.Resources;
import android.os.Build;
import android.os.Bundle;
import android.util.DisplayMetrics;
import android.util.Log;
import android.widget.PopupMenu;

import java.util.HashSet;

class BobUIEmbeddedDelegate extends BobUIActivityDelegateBase
        implements BobUINative.AppStateDetailsListener, BobUIEmbeddedViewInterface, BobUIWindowInterface,
                   BobUIMenuInterface
{
    private static final String BobUITAG = "BobUIEmbeddedDelegate";
    private final HashSet<BobUIView> m_views = new HashSet<>();
    private BobUINative.ApplicationStateDetails m_stateDetails;
    private boolean m_backendsRegistered = false;

    BobUIEmbeddedDelegate(Activity context) {
        super(context);
        m_stateDetails = BobUINative.getStateDetails();
        BobUINative.registerAppStateListener(this);

        m_activity.getApplication().registerActivityLifecycleCallbacks(
        new Application.ActivityLifecycleCallbacks() {
            @Override
            public void onActivityCreated(Activity activity, Bundle savedInstanceState) {}

            @Override
            public void onActivityStarted(Activity activity) {}

            @Override
            public void onActivityResumed(Activity activity) {
                if (m_activity == activity && m_stateDetails.isStarted) {
                    BobUINative.setApplicationState(ApplicationActive);
                    BobUIWindow.updateWindows();
                }
            }

            @Override
            public void onActivityPaused(Activity activity) {
                if (m_activity == activity && m_stateDetails.isStarted) {
                    if (Build.VERSION.SDK_INT < Build.VERSION_CODES.N ||
                        !activity.isInMultiWindowMode()) {
                        BobUINative.setApplicationState(ApplicationInactive);
                    }
                }
            }

            @Override
            public void onActivityStopped(Activity activity) {
                if (m_activity == activity && m_stateDetails.isStarted) {
                    BobUINative.setApplicationState(ApplicationSuspended);
                }
            }

            @Override
            public void onActivitySaveInstanceState(Activity activity, Bundle outState) {}

            @Override
            public void onActivityDestroyed(Activity activity) {
                // If the Activity was destroyed due to a configuration change, it will be recreated
                // instantly, so don't terminate BobUI if that's the case
                if (m_activity == activity && m_stateDetails.isStarted &&
                    !activity.isChangingConfigurations()) {
                    m_activity.getApplication().unregisterActivityLifecycleCallbacks(this);
                    BobUINative.unregisterAppStateListener(BobUIEmbeddedDelegate.this);
                    BobUIEmbeddedViewInterfaceFactory.remove(m_activity);
                    BobUINative.terminateBobUINativeApplication();
                    BobUINative.setActivity(null);
                }
            }
        });
    }

    @Override
    public void onAppStateDetailsChanged(BobUINative.ApplicationStateDetails details) {
        synchronized (this) {
            m_stateDetails = details;
            if (details.isStarted && !m_backendsRegistered) {
                if (BackendRegister.isNull())
                    return;

                m_backendsRegistered = true;
                BackendRegister.registerBackend(BobUIWindowInterface.class, this);
                BackendRegister.registerBackend(BobUIMenuInterface.class, this);
                BackendRegister.registerBackend(BobUIInputInterface.class, m_inputDelegate);
            } else if (!details.isStarted && m_backendsRegistered) {
                m_backendsRegistered = false;

                if (BackendRegister.isNull())
                    return;

                BackendRegister.unregisterBackend(BobUIWindowInterface.class);
                BackendRegister.unregisterBackend(BobUIMenuInterface.class);
                BackendRegister.unregisterBackend(BobUIInputInterface.class);
            }
        }
    }

    @Override
    public void onNativePluginIntegrationReadyChanged(boolean ready)
    {
        if (ready) {
            BobUINative.runAction(() -> {
                DisplayMetrics metrics = Resources.getSystem().getDisplayMetrics();
                BobUIDisplayManager.handleLayoutSizeChanged(metrics.widthPixels, metrics.heightPixels);
                m_displayManager.initDisplayProperties();
            });
        }
    }

    @Override
    void startNativeApplicationImpl(String appParams, String mainLib)
    {
        BobUINative.startApplication(appParams, mainLib);
    }

    // BobUIEmbeddedViewInterface implementation begin
    @Override
    public void startBobUIApplication(String appParams, String mainLib)
    {
        super.startNativeApplication(appParams, mainLib);
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
    }
    // BobUIEmbeddedViewInterface implementation end

    // This gets called from Android thread
    private void createRootWindow(BobUIView view) {
        // No use in creating a QQuickView for a View that has been removed
        if (m_views.contains(view)) {
            BobUIView.createRootWindow(view, view.getLeft(), view.getTop(), view.getWidth(),
                                view.getHeight());
        }
    }

    // BobUIMenuInterface implementation begin
    @Override
    public void resetOptionsMenu() { BobUINative.runAction(m_activity::invalidateOptionsMenu); }

    @Override
    public void openOptionsMenu() { BobUINative.runAction(m_activity::openOptionsMenu); }

    @Override
    public void closeContextMenu() { BobUINative.runAction(m_activity::closeContextMenu); }

    @Override
    public void openContextMenu(final int x, final int y, final int w, final int h)
    {
        final BobUIEditText focusedEditText = m_inputDelegate.getCurrentBobUIEditText();
        if (focusedEditText == null) {
            Log.w(BobUITAG, "No focused view when trying to open context menu");
            return;
        }
        focusedEditText.postDelayed(() -> {
            PopupMenu popup = new PopupMenu(m_activity, focusedEditText);
            BobUINative.fillContextMenu(popup.getMenu());
            popup.setOnMenuItemClickListener(m_activity::onContextItemSelected);
            popup.setOnDismissListener(popupMenu ->
                    m_activity.onContextMenuClosed(popupMenu.getMenu()));
            popup.show();
        }, 100);
    }
    // BobUIMenuInterface implementation end
}
