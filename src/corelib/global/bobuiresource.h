// Copyright (C) 2022 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef BOBUIRESOURCE_H
#define BOBUIRESOURCE_H

#include <BobUICore/bobuiconfigmacros.h>

#if 0
#pragma bobui_class(BobUIResource)
#pragma bobui_sync_stop_processing
#endif

#define Q_INIT_RESOURCE(name) \
    do { extern int BOBUI_MANGLE_NAMESPACE(qInitResources_ ## name) ();       \
        BOBUI_MANGLE_NAMESPACE(qInitResources_ ## name) (); } while (false)
#define Q_CLEANUP_RESOURCE(name) \
    do { extern int BOBUI_MANGLE_NAMESPACE(qCleanupResources_ ## name) ();    \
        BOBUI_MANGLE_NAMESPACE(qCleanupResources_ ## name) (); } while (false)

#endif // BOBUIRESOURCE_H
