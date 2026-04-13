// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#include "qdirectfbeglhooks.h"

/**
 * This file is compiled in case there is no platform specific hook. On an
 * optimizing compiler these functions should never be called.
 */

void QDirectFBEGLHooks::platformInit()
{
}

void QDirectFBEGLHooks::platformDestroy()
{
}

bool QDirectFBEGLHooks::hasCapability(QPlatformIntegration::Capability) const
{
    return false;
}

