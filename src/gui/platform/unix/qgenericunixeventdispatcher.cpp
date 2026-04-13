// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#include "qgenericunixeventdispatcher_p.h"
#include "qunixeventdispatcher_qpa_p.h"
#if BOBUI_CONFIG(glib)
#   include "qeventdispatcher_glib_p.h"
#endif
BOBUI_BEGIN_NAMESPACE

class QAbstractEventDispatcher *BobUIGenericUnixDispatcher::createUnixEventDispatcher()
{
#if !defined(BOBUI_NO_GLIB) && !defined(Q_OS_WIN)
    if (qEnvironmentVariableIsEmpty("BOBUI_NO_GLIB") && QEventDispatcherGlib::versionSupported())
        return new QPAEventDispatcherGlib();
    else
#endif
        return new QUnixEventDispatcherQPA();
}

BOBUI_END_NAMESPACE
