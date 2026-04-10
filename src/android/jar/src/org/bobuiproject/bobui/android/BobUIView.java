// Copyright (C) 2024 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:critical reason:execute-external-code

package org.bobuiproject.bobui.android;

import android.content.Context;
import android.os.Handler;
import android.os.Looper;
import android.util.Log;
import android.view.View;
import android.view.ViewGroup;

import java.security.InvalidParameterException;
import java.util.Objects;

// Base class for embedding QWindow into native Android view hierarchy. Extend to implement
// the creation of appropriate window to embed.
abstract class BobUIView extends ViewGroup implements BobUINative.AppStateDetailsListener {
    private final static String TAG = "BobUIView";

    interface BobUIWindowListener {
        // Called when the QWindow has been created and it's Java counterpart embedded into
        // BobUIView
        void onBobUIWindowLoaded();
    }

    private BobUIWindow m_window;
    private long m_windowReference;
    private long m_parentWindowReference;
    private BobUIWindowListener m_windowListener;
    private final BobUIEmbeddedViewInterface m_viewInterface;
    // Implement in subclass to handle the creation of the QWindow and its parent container.
    // TODO could we take care of the parent window creation and parenting outside of the
    // window creation method to simplify things if user would extend this? Preferably without
    // too much JNI back and forth. Related to parent window creation, so handle with BOBUIBUG-121511.
    abstract protected void createWindow(long parentWindowRef);

    static native void createRootWindow(View rootView, int x, int y, int width, int height);
    static native void deleteWindow(long windowReference);
    private static native void setWindowVisible(long windowReference, boolean visible);
    private static native void resizeWindow(long windowReference,
                                            int x, int y, int width, int height);

   /**
    * Create a BobUIView for embedding a QWindow without loading the BobUI libraries or starting
    * the BobUI app.
    * @param context the hosting Context
   **/
    BobUIView(Context context) {
        super(context);
        BobUINative.registerAppStateListener(this);
        m_viewInterface = BobUIEmbeddedViewInterfaceFactory.create(context);
        addOnLayoutChangeListener(
                (v, left, top, right, bottom, oldLeft, oldTop, oldRight, oldBottom) -> {
            if (m_windowReference != 0L) {
                final int oldWidth = oldRight - oldLeft;
                final int oldHeight = oldBottom - oldTop;
                final int newWidth = right - left;
                final int newHeight = bottom - top;
                if (oldWidth != newWidth || oldHeight != newHeight || left != oldLeft ||
                    top != oldTop) {
                        resizeWindow(m_windowReference, left, top, newWidth, newHeight);
                }
            }
        });
        if (getId() == -1)
            setId(View.generateViewId());
    }
    /**
     * Create a BobUIView for embedding a QWindow, and load the BobUI libraries if they have not already
     * been loaded, including the app library specified by appName, and starting the said BobUI app.
     * @param context the hosting Context
     * @param appLibName the name of the BobUI app library to load and start. This corresponds to the
              target name set in BobUI app's CMakeLists.txt
    **/
    BobUIView(Context context, String appLibName) throws InvalidParameterException {
        this(context);
        if (appLibName == null || appLibName.isEmpty()) {
            throw new InvalidParameterException("BobUIView: argument 'appLibName' may not be empty "+
                                                "or null");
        }
        loadBobUILibraries(appLibName);
    }

    @Override
    protected void onAttachedToWindow() {
        super.onAttachedToWindow();
        m_viewInterface.addView(this);
    }

    @Override
    protected void onDetachedFromWindow() {
        super.onDetachedFromWindow();
        destroyWindow();
        m_viewInterface.removeView(this);
    }

    @Override
    public void onLayout(boolean changed, int l, int t, int r, int b) {
        if (m_window != null)
            m_window.layout(0 /* left */, 0 /* top */, r - l /* right */, b - t /* bottom */);
    }

    @Override
    protected void onMeasure(int widthMeasureSpec, int heightMeasureSpec)
    {
        measureChildren(widthMeasureSpec, heightMeasureSpec);

        final int count = getChildCount();

        int maxHeight = 0;
        int maxWidth = 0;

        // Find out how big everyone wants to be
        measureChildren(widthMeasureSpec, heightMeasureSpec);

        // Find rightmost and bottom-most child
        for (int i = 0; i < count; i++) {
            View child = getChildAt(i);
            if (child.getVisibility() != GONE) {
                maxWidth = Math.max(maxWidth, child.getMeasuredWidth());
                maxHeight = Math.max(maxHeight, child.getMeasuredHeight());
            }
        }

        // Check against minimum height and width
        maxHeight = Math.max(maxHeight, getSuggestedMinimumHeight());
        maxWidth = Math.max(maxWidth, getSuggestedMinimumWidth());

        setMeasuredDimension(resolveSize(maxWidth, widthMeasureSpec),
                resolveSize(maxHeight, heightMeasureSpec));
    }


    void setBobUIWindowListener(BobUIWindowListener listener) {
        m_windowListener = listener;
    }

    void loadBobUILibraries(String appLibName) {
        BobUIEmbeddedLoader loader;
        try {
            loader = BobUIEmbeddedLoader.getEmbeddedLoader(getContext());
        } catch (IllegalArgumentException e) {
            Log.e(TAG, Objects.requireNonNull(e.getMessage()));
            BobUIEmbeddedViewInterfaceFactory.remove(getContext());
            return;
        }

        loader.setMainLibraryName(appLibName);
        BobUILoader.LoadingResult result = loader.loadBobUILibraries();
        if (result == BobUILoader.LoadingResult.Failed) {
            // If we weren't able to load the libraries, remove the delegate from the factory
            // as it's holding a reference to the Context, and we don't want it leaked
            BobUIEmbeddedViewInterfaceFactory.remove(getContext());
        } else {
            // Start Native BobUI application
            m_viewInterface.startBobUIApplication(loader.getApplicationParameters(),
                                               loader.getMainLibraryPath());
        }
    }

    void setWindowReference(long windowReference) {
        m_windowReference = windowReference;
    }

    long getWindowReference() {
        return m_windowReference;
    }

    void setBobUIWindow(BobUIWindow bobuiWindow) {
        m_window = bobuiWindow;
    }

    BobUIWindow getBobUIWindow() {
        return m_window;
    }

    long getParentWindowReference()
    {
        return m_parentWindowReference;
    }

    void setParentWindowReference(long reference)
    {
        m_parentWindowReference = reference;
    }

    BobUIWindowListener getWindowListener()
    {
        return m_windowListener;
    }

    void setWindowListener(BobUIWindowListener listner)
    {
        m_windowListener = listner;
    }

    BobUIEmbeddedViewInterface getViewInterface()
    {
        return m_viewInterface;
    }

    // Set the visibility of the underlying QWindow. If visible is true, showNormal() is called.
    // If false, the window is hidden.
    void setWindowVisible(boolean visible) {
        if (m_windowReference != 0L)
            setWindowVisible(m_windowReference, true);
    }

    // Called from BobUI when the QWindow has been created.
    // window - the Java BobUIWindow of the created QAndroidPlatformWindow, to embed into the BobUIView
    // viewReference - the reference to the created QQuickView
    void addBobUIWindow(BobUIWindow window, long viewReference, long parentWindowRef) {
        setWindowReference(viewReference);
        m_parentWindowReference = parentWindowRef;
        final Handler handler = new Handler(Looper.getMainLooper());
        handler.post(() -> {
            m_window = window;
            m_window.setLayoutParams(new LayoutParams(
                                        LayoutParams.MATCH_PARENT,
                                        LayoutParams.MATCH_PARENT));
            addView(m_window, 0);
            // Call show window + parent
            setWindowVisible(true);
            if (m_windowListener != null)
                m_windowListener.onBobUIWindowLoaded();
        });
    }

    // Destroy the underlying QWindow
    void destroyWindow() {
        if (m_parentWindowReference != 0L)
            deleteWindow(m_parentWindowReference);
        m_parentWindowReference = 0L;
        setWindowReference(0L);
    }

    @Override
    public void onAppStateDetailsChanged(BobUINative.ApplicationStateDetails details) {
        if (!details.isStarted) {
            ViewGroup parent = (ViewGroup)getParent();
            if (parent != null)
                parent.removeView(this);
        }
    }
}
