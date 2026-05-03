// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QWINDOWSNATIVEINTERFACE_H
#define QWINDOWSNATIVEINTERFACE_H

#include <BobUIGui/qfont.h>
#include <BobUIGui/qpa/qplatformnativeinterface.h>

BOBUI_BEGIN_NAMESPACE

/*!
    \class QWindowsNativeInterface
    \brief Provides access to native handles.

    Currently implemented keys
    \list
    \li handle (HWND)
    \li getDC (DC)
    \li releaseDC Releases the previously acquired DC and returns 0.
    \endlist

    \internal
*/

class QWindowsNativeInterface : public QPlatformNativeInterface
{
    Q_OBJECT

public:
    void *nativeResourceForIntegration(const QByteArray &resource) override;
#ifndef BOBUI_NO_OPENGL
    void *nativeResourceForContext(const QByteArray &resource, QOpenGLContext *context) override;
#endif
    void *nativeResourceForWindow(const QByteArray &resource, QWindow *window) override;
    void *nativeResourceForScreen(const QByteArray &resource, QScreen *screen) override;
#ifndef BOBUI_NO_CURSOR
    void *nativeResourceForCursor(const QByteArray &resource, const QCursor &cursor) override;
#endif
};

BOBUI_END_NAMESPACE

#endif // QWINDOWSNATIVEINTERFACE_H
