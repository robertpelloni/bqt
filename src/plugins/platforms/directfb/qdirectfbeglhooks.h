// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#ifndef QDIRECTFBEGL_HOOKS_H
#define QDIRECTFBEGL_HOOKS_H

#include <qpa/qplatformintegration.h>

BOBUI_BEGIN_NAMESPACE

struct QDirectFBEGLHooks {
    void platformInit();
    void platformDestroy();
    bool hasCapability(QPlatformIntegration::Capability) const;
};

BOBUI_END_NAMESPACE

#endif
