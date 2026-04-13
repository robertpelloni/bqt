// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QEGLFSEMULATORINTEGRATION_H
#define QEGLFSEMULATORINTEGRATION_H

#include "private/qeglfsdeviceintegration_p.h"

#include <BobUICore/QLoggingCategory>
#include <BobUICore/QFunctionPointer>

typedef QByteArray (EGLAPIENTRYP PFNQGSGETDISPLAYSPROC) ();
typedef void (EGLAPIENTRYP PFNQGSSETDISPLAYPROC) (uint screen);

BOBUI_BEGIN_NAMESPACE

Q_DECLARE_LOGGING_CATEGORY(qLcEglfsEmuDebug)

class QEglFSEmulatorIntegration : public QEglFSDeviceIntegration
{
public:
    QEglFSEmulatorIntegration();

    void platformInit() override;
    void platformDestroy() override;
    bool usesDefaultScreen() override;
    void screenInit() override;
    bool hasCapability(QPlatformIntegration::Capability cap) const override;

    PFNQGSGETDISPLAYSPROC getDisplays;
    PFNQGSSETDISPLAYPROC setDisplay;

    EGLNativeWindowType createNativeWindow(QPlatformWindow *platformWindow, const QSize &size, const QSurfaceFormat &format) override;
};

BOBUI_END_NAMESPACE

#endif // QEGLFSEMULATORINTEGRATION_H
