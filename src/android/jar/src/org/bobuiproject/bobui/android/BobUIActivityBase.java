// Copyright (C) 2023 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

package org.bobuiproject.bobui.android;

import android.annotation.SuppressLint;
import android.app.Activity;
import android.content.ComponentName;
import android.content.Intent;
import android.content.pm.ActivityInfo;
import android.content.res.Configuration;
import android.content.res.Resources;
import android.net.Uri;
import android.os.Build;
import android.os.Bundle;
import android.provider.Browser;
import android.view.ContextMenu;
import android.view.ContextMenu.ContextMenuInfo;
import android.view.KeyEvent;
import android.view.Menu;
import android.view.MenuItem;
import android.view.MotionEvent;
import android.view.View;
import android.view.Window;
import android.util.Log;
import java.lang.IllegalArgumentException;

import android.widget.Toast;

public class BobUIActivityBase extends Activity
{
    public static final String TAG = "BobUIActivityBase";
    public static final String EXTRA_SOURCE_INFO = "org.bobuiproject.bobui.android.sourceInfo";
    public static final String EXTRA_FATAL_MESSAGE = "org.bobuiproject.bobui.android.fatalMessage";

    private String m_applicationParams = "";
    private boolean m_isCustomThemeSet = false;
    private boolean m_retainNonConfigurationInstance = false;
    private Configuration m_prevConfig;
    private final BobUIActivityDelegate m_delegate;
    private boolean m_onCreateSucceeded = false;

    private void addReferrer(Intent intent)
    {
        Bundle extras = intent.getExtras();
        if (extras != null && extras.getString(EXTRA_SOURCE_INFO) != null)
            return;

        if (extras == null) {
            Uri referrer = getReferrer();
            if (referrer != null) {
                String cleanReferrer = referrer.toString().replaceFirst("android-app://", "");
                intent.putExtra(EXTRA_SOURCE_INFO, cleanReferrer);
            }
        } else {
            String applicationId = extras.getString(Browser.EXTRA_APPLICATION_ID);
            if (applicationId != null)
                intent.putExtra(EXTRA_SOURCE_INFO, applicationId);
        }
    }

    /**
     * Adds parameters to the list of arguments that will be passed to the
     * native BobUI application's main() function.
     *
     * Either a whitespace or a tab is accepted as a separator.
     */
    /**unused*/
    @SuppressWarnings("unused")
    public void appendApplicationParameters(String params)
    {
        if (params == null || params.isEmpty())
            return;

        if (!m_applicationParams.isEmpty())
            m_applicationParams += " ";
        m_applicationParams += params;
    }

    @Override
    public void setTheme(int resId) {
        super.setTheme(resId);
        m_isCustomThemeSet = true;
    }

    private void restartApplication() {
        Intent intent = Intent.makeRestartActivityTask(getComponentName());
        startActivity(intent);
        BobUINative.setStarted(false);
        // FIXME: calling exit() right after this gives no time to get onDestroy().
        finish();
        Runtime.getRuntime().exit(0);
    }

    public BobUIActivityBase()
    {
        m_delegate = new BobUIActivityDelegate(this);
    }

    @Override
    protected void onCreate(Bundle savedInstanceState)
    {
        super.onCreate(savedInstanceState);
        requestWindowFeature(Window.FEATURE_ACTION_BAR);

        if (!m_isCustomThemeSet) {
            @SuppressWarnings("deprecation")
            int themeId = Build.VERSION.SDK_INT >= Build.VERSION_CODES.Q
                ? android.R.style.Theme_DeviceDefault_DayNight
                : android.R.style.Theme_Holo_Light;
            setTheme(themeId);
        }

        if (BobUINative.getStateDetails().isStarted) {
            // We don't yet have a reliable way to keep the app
            // running properly in case of an Activity only restart,
            // so for now restart the whole app.
            restartApplication();
        }

        BobUINative.registerAppStateListener(m_delegate);
        addReferrer(getIntent());

        try {
            if (isLaunchedAsAlias()) {
                Log.d(TAG, "Starting an alias-activity, skipping loading of the BobUI libraries.");
            } else {
                BobUIActivityLoader loader = BobUIActivityLoader.getActivityLoader(this);
                BobUILoader.LoadingResult result = loader.loadBobUILibraries();

                if (result == BobUILoader.LoadingResult.Failed) {
                    showFatalFinishingToast();
                    return;
                }

                if (result == BobUILoader.LoadingResult.Succeeded) {
                    loader.appendApplicationParameters(m_applicationParams);
                    final String params = loader.getApplicationParameters();
                    m_delegate.startNativeApplication(params, loader.getMainLibraryPath());
                }
            }
        } catch (IllegalArgumentException e) {
            e.printStackTrace();
            showFatalFinishingToast();
            return;
        }

        m_prevConfig = new Configuration(getResources().getConfiguration());
        m_onCreateSucceeded = true;
    }

    private void showFatalFinishingToast() {
        Resources resources = getResources();
        String packageName = getPackageName();
        String message = null;
        try {
            @SuppressLint("DiscouragedApi")
            int id = resources.getIdentifier("fatal_error_msg", "string", packageName);
            message = resources.getString(id);
            Toast.makeText(this, message, Toast.LENGTH_LONG).show();
        } catch (Resources.NotFoundException ignored) { }
        Intent fatalIntent = new Intent();
        if (message != null)
            fatalIntent.putExtra(EXTRA_FATAL_MESSAGE, message);
        setResult(Activity.RESULT_CANCELED, fatalIntent);
        super.finish();
    }

    private boolean isLaunchedAsAlias() {
        final ComponentName component = getIntent().getComponent();
        if (component == null)
            return false;

        final String launchedClassName = component.getClassName();
        final String runtimeClassName = this.getClass().getName();

        return !launchedClassName.equals(runtimeClassName);
    }

    @Override
    protected void onPause()
    {
        super.onPause();
        if (Build.VERSION.SDK_INT < 24 || !isInMultiWindowMode())
            BobUINative.setApplicationState(BobUINative.ApplicationState.ApplicationInactive);
        m_delegate.displayManager().unregisterDisplayListener();
    }

    @Override
    protected void onResume()
    {
        super.onResume();
        BobUINative.setApplicationState(BobUINative.ApplicationState.ApplicationActive);
        if (BobUINative.getStateDetails().isStarted) {
            m_delegate.displayManager().registerDisplayListener();
            BobUIWindow.updateWindows();
            // Suspending the app clears the immersive mode, so we need to set it again.
            BobUIWindowInsetsController.restoreFullScreenVisibility(this);
        }
    }

    @Override
    protected void onStop()
    {
        super.onStop();
        BobUINative.setApplicationState(BobUINative.ApplicationState.ApplicationSuspended);
    }

    @Override
    protected void onDestroy()
    {
        super.onDestroy();

        if (!m_onCreateSucceeded)
            System.exit(-1);

        if (!m_retainNonConfigurationInstance) {
            BobUINative.unregisterAppStateListener(m_delegate);
            BobUINative.terminateBobUINativeApplication();
            BobUINative.setActivity(null);
            System.exit(0);
        }
    }

    @Override
    public void onConfigurationChanged(Configuration newConfig)
    {
        super.onConfigurationChanged(newConfig);

        int diff = newConfig.diff(m_prevConfig);
        if ((diff & ActivityInfo.CONFIG_UI_MODE) != 0)
            m_delegate.handleUiModeChange();

        if ((diff & ActivityInfo.CONFIG_LOCALE) != 0)
            BobUINative.updateLocale();

        m_prevConfig = new Configuration(newConfig);
    }

    @Override
    public boolean onContextItemSelected(MenuItem item)
    {
        m_delegate.setContextMenuVisible(false);
        return BobUINative.onContextItemSelected(item.getItemId(), item.isChecked());
    }

    @Override
    public void onContextMenuClosed(Menu menu)
    {
        if (!m_delegate.isContextMenuVisible())
            return;
        m_delegate.setContextMenuVisible(false);
        BobUINative.onContextMenuClosed(menu);
    }

    @Override
    public void onCreateContextMenu(ContextMenu menu, View v, ContextMenuInfo menuInfo)
    {
        menu.clearHeader();
        BobUINative.onCreateContextMenu(menu);
        m_delegate.setContextMenuVisible(true);
    }

    @Override
    public boolean dispatchKeyEvent(KeyEvent event)
    {
        boolean handleResult = m_delegate.getInputDelegate().handleDispatchKeyEvent(event);
        if (BobUINative.getStateDetails().isStarted && handleResult)
            return true;

        return super.dispatchKeyEvent(event);
    }

    @Override
    public boolean dispatchGenericMotionEvent(MotionEvent event)
    {
        boolean handled = m_delegate.getInputDelegate().handleDispatchGenericMotionEvent(event);
        if (BobUINative.getStateDetails().isStarted && handled)
            return true;

        return super.dispatchGenericMotionEvent(event);
    }

    @Override
    public boolean onKeyDown(int keyCode, KeyEvent event)
    {
        BobUINative.ApplicationStateDetails stateDetails = BobUINative.getStateDetails();
        if (!stateDetails.isStarted || !stateDetails.nativePluginIntegrationReady)
            return false;

        return m_delegate.getInputDelegate().onKeyDown(keyCode, event);
    }

    @Override
    public boolean onKeyUp(int keyCode, KeyEvent event)
    {
        BobUINative.ApplicationStateDetails stateDetails = BobUINative.getStateDetails();
        if (!stateDetails.isStarted || !stateDetails.nativePluginIntegrationReady)
            return false;

        return m_delegate.getInputDelegate().onKeyUp(keyCode, event);
    }

    @Override
    public boolean onCreateOptionsMenu(Menu menu)
    {
        menu.clear();
        return true;
    }

    @Override
    public boolean onPrepareOptionsMenu(Menu menu)
    {
        boolean res = BobUINative.onPrepareOptionsMenu(menu);
        m_delegate.setActionBarVisibility(res && menu.size() > 0);
        return res;
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item)
    {
        return BobUINative.onOptionsItemSelected(item.getItemId(), item.isChecked());
    }

    @Override
    public void onOptionsMenuClosed(Menu menu)
    {
        BobUINative.onOptionsMenuClosed(menu);
    }

    @Override
    protected void onRestoreInstanceState(Bundle savedInstanceState)
    {
        super.onRestoreInstanceState(savedInstanceState);

        // only restore when this Activity is being recreated for a config change
        if (getLastNonConfigurationInstance() == null)
            return;

        BobUINative.setStarted(savedInstanceState.getBoolean("Started"));
        BobUIWindowInsetsController.restoreFullScreenVisibility(this);
        // FIXME restore all surfaces
    }

    @Override
    public Object onRetainNonConfigurationInstance()
    {
        super.onRetainNonConfigurationInstance();
        m_retainNonConfigurationInstance = true;
        return true;
    }

    @Override
    protected void onSaveInstanceState(Bundle outState)
    {
        super.onSaveInstanceState(outState);
        outState.putBoolean("Started", BobUINative.getStateDetails().isStarted);
    }

    @Override
    public void onWindowFocusChanged(boolean hasFocus)
    {
        super.onWindowFocusChanged(hasFocus);
        if (hasFocus)
            BobUIWindowInsetsController.restoreFullScreenVisibility(this);
    }

    @Override
    protected void onNewIntent(Intent intent)
    {
        addReferrer(intent);
        BobUINative.onNewIntent(intent);
    }

    @Override
    protected void onActivityResult(int requestCode, int resultCode, Intent data)
    {
        super.onActivityResult(requestCode, resultCode, data);
        BobUINative.onActivityResult(requestCode, resultCode, data);
    }

    @Override
    public void onRequestPermissionsResult(int requestCode, String[] permissions, int[] grantResults)
    {
        BobUINative.sendRequestPermissionsResult(requestCode, grantResults);
    }

    @UsedFromNativeCode
    public void hideSplashScreen(final int duration)
    {
        m_delegate.hideSplashScreen(duration);
    }
}
