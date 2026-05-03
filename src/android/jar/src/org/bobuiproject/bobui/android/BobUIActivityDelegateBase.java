// Copyright (C) 2017 BogDan Vatra <bogdan@kde.org>
// Copyright (C) 2023 The BobUI Company Ltd.
// Copyright (C) 2016 Olivier Goffart <ogoffart@woboq.com>
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

package org.bobuiproject.bobui.android;

import android.app.Activity;
import android.app.UiModeManager;
import android.content.pm.ActivityInfo;
import android.content.pm.PackageManager;
import android.content.res.Configuration;
import android.content.res.Resources;
import android.graphics.Color;
import android.os.Build;
import android.view.Window;
import android.view.WindowInsetsController;

import java.util.HashMap;

abstract class BobUIActivityDelegateBase
{
    protected final Activity m_activity;
    protected final HashMap<Integer, BobUIWindow> m_topLevelWindows = new HashMap<>();
    protected final BobUIDisplayManager m_displayManager;
    protected final BobUIInputDelegate m_inputDelegate;
    protected final BobUIAccessibilityDelegate m_accessibilityDelegate;

    private boolean m_membersInitialized = false;
    private boolean m_contextMenuVisible = false;

    static native boolean canOverrideColorSchemeHint();
    static native void updateUiContrast(float newUiContrast);

    // Subclass must implement these
    abstract void startNativeApplicationImpl(String appParams, String mainLib);

    // With these we are okay with default implementation doing nothing
    void setUpLayout() {}
    void setUpSplashScreen(int orientation) {}
    void hideSplashScreen(final int duration) {}
    void setActionBarVisibility(boolean visible) {}

    BobUIActivityDelegateBase(Activity activity)
    {
        m_activity = activity;
        BobUINative.setActivity(m_activity);
        m_displayManager = new BobUIDisplayManager(m_activity);
        m_inputDelegate = new BobUIInputDelegate(() -> {
            BobUIWindowInsetsController.restoreFullScreenVisibility(m_activity);
        });
        m_accessibilityDelegate = new BobUIAccessibilityDelegate();
    }

    BobUIDisplayManager displayManager() {
        return m_displayManager;
    }

    BobUIInputDelegate getInputDelegate() {
        return m_inputDelegate;
    }

    void setContextMenuVisible(boolean contextMenuVisible)
    {
        m_contextMenuVisible = contextMenuVisible;
    }

    boolean isContextMenuVisible()
    {
        return m_contextMenuVisible;
    }

    void startNativeApplication(String appParams, String mainLib)
    {
        if (m_membersInitialized)
            return;
        initMembers();
        startNativeApplicationImpl(appParams, mainLib);
    }

    void initMembers()
    {
        m_membersInitialized = true;
        m_topLevelWindows.clear();
        m_displayManager.registerDisplayListener();
        m_inputDelegate.initInputMethodManager(m_activity);

        try {
            PackageManager pm = m_activity.getPackageManager();
            ActivityInfo activityInfo =  pm.getActivityInfo(m_activity.getComponentName(), 0);
            m_inputDelegate.setSoftInputMode(activityInfo.softInputMode);
        } catch (PackageManager.NameNotFoundException e) {
            e.printStackTrace();
        }

        setUpLayout();
    }

    void hideSplashScreen()
    {
        hideSplashScreen(0);
    }

    void handleUiModeChange()
    {
        Resources resources = m_activity.getResources();
        Configuration config = resources.getConfiguration();
        int uiMode = config.uiMode & Configuration.UI_MODE_NIGHT_MASK;

        if (BobUIWindowInsetsController.decorFitsSystemWindows(m_activity)) {
            Window window = m_activity.getWindow();
            BobUIWindowInsetsController.enableSystemBarsBackgroundDrawing(window);
            int status = BobUIWindowInsetsController.getThemeDefaultStatusBarColor(m_activity);
            BobUIWindowInsetsController.setStatusBarColor(window, status);
            int nav = BobUIWindowInsetsController.getThemeDefaultNavigationBarColor(m_activity);
            BobUIWindowInsetsController.setNavigationBarColor(window, nav);
        }

        // Don't override color scheme if the app has it set explicitly.
        if (canOverrideColorSchemeHint()) {
            boolean isLight = uiMode == Configuration.UI_MODE_NIGHT_NO;
            BobUIWindowInsetsController.setStatusBarColorHint(m_activity, isLight);
            BobUIWindowInsetsController.setNavigationBarColorHint(m_activity, isLight);
        }

        switch (uiMode) {
            case Configuration.UI_MODE_NIGHT_NO:
                ExtractStyle.runIfNeeded(m_activity, false);
                BobUIDisplayManager.handleUiDarkModeChanged(0);
                break;
            case Configuration.UI_MODE_NIGHT_YES:
                ExtractStyle.runIfNeeded(m_activity, true);
                BobUIDisplayManager.handleUiDarkModeChanged(1);
                break;
        }

        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.UPSIDE_DOWN_CAKE) {
            // FIXME: Handle contrast changes the same way as uiMode changes (BOBUIBUG-140749).
            UiModeManager uiModeManager =
                (UiModeManager) m_activity.getSystemService(m_activity.UI_MODE_SERVICE);
            updateUiContrast(uiModeManager.getContrast());
        }
    }
}
