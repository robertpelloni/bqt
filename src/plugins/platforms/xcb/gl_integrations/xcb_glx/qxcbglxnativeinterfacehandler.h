// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#pragma once

#include "qxcbnativeinterfacehandler.h"

BOBUI_BEGIN_NAMESPACE

class QXcbGlxNativeInterfaceHandler : public QXcbNativeInterfaceHandler
{
public:
    enum ResourceType {
        GLXConfig,
        GLXContext,
    };

    QXcbGlxNativeInterfaceHandler(QXcbNativeInterface *nativeInterface);
    QPlatformNativeInterface::NativeResourceForContextFunction nativeResourceFunctionForContext(const QByteArray &resource) const override;

private:
    static void *glxContextForContext(QOpenGLContext *context);
    static void *glxConfigForContext(QOpenGLContext *context);
};

BOBUI_END_NAMESPACE
