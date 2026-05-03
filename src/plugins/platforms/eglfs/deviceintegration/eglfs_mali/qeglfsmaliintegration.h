// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QEGLFSMALIINTEGRATION_H
#define QEGLFSMALIINTEGRATION_H

#include "private/qeglfsdeviceintegration_p.h"

BOBUI_BEGIN_NAMESPACE

class QEglFSMaliIntegration : public QEglFSDeviceIntegration
{
public:
    void platformInit() override;
    EGLNativeWindowType createNativeWindow(QPlatformWindow *window, const QSize &size, const QSurfaceFormat &format) override;
    void destroyNativeWindow(EGLNativeWindowType window) override;
};

BOBUI_END_NAMESPACE

#endif
