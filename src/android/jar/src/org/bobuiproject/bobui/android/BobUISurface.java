// Copyright (C) 2014 BogDan Vatra <bogdan@kde.org>
// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

package org.bobuiproject.bobui.android;

import android.annotation.SuppressLint;
import android.content.Context;
import android.graphics.PixelFormat;
import android.view.SurfaceHolder;
import android.view.SurfaceView;

@SuppressLint("ViewConstructor")
class BobUISurface extends SurfaceView implements SurfaceHolder.Callback
{
    private final BobUISurfaceInterface m_surfaceCallback;

    BobUISurface(Context context, BobUISurfaceInterface surfaceCallback, boolean onTop, int imageDepth)
    {
        super(context);
        setFocusable(false);
        setFocusableInTouchMode(false);
        setZOrderMediaOverlay(onTop);
        m_surfaceCallback = surfaceCallback;
        SurfaceHolder holder = getHolder();
        holder.setFormat(imageDepth == 16 ? PixelFormat.RGB_565 : PixelFormat.RGBA_8888);
        holder.addCallback(this);
    }

    @Override
    public void surfaceCreated(SurfaceHolder holder)
    {
        if (m_surfaceCallback != null)
            m_surfaceCallback.onSurfaceChanged(holder.getSurface());
    }

    @Override
    public void surfaceChanged(SurfaceHolder holder, int format, int width, int height)
    {
        // FIXME: this can sometimes cause flicker on orientation change
        if (m_surfaceCallback != null)
            m_surfaceCallback.onSurfaceChanged(holder.getSurface());
    }

    @Override
    public void surfaceDestroyed(SurfaceHolder holder)
    {
        // Once we return from this function, the Surface will be destroyed,
        // so everything holding a reference to it needs to clean it up before we do that
        if (m_surfaceCallback != null)
            m_surfaceCallback.onSurfaceChanged(null);
    }
}
