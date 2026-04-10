// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "qdirectfbeglhooks.h"
#include "qdirectfbconvenience.h"

#include "default_directfb.h"

BOBUI_BEGIN_NAMESPACE

// Exported to the directfb plugin
QDirectFBEGLHooks platform_hook;
static void *dbpl_handle;

void QDirectFBEGLHooks::platformInit()
{
    DBPL_RegisterDirectFBDisplayPlatform(&dbpl_handle, QDirectFbConvenience::dfbInterface());
}

void QDirectFBEGLHooks::platformDestroy()
{
    DBPL_UnregisterDirectFBDisplayPlatform(dbpl_handle);
    dbpl_handle = 0;
}

bool QDirectFBEGLHooks::hasCapability(QPlatformIntegration::Capability cap) const
{
    switch (cap) {
    case QPlatformIntegration::ThreadedOpenGL:
        return true;
    default:
        return false;
    }
}

BOBUI_END_NAMESPACE
