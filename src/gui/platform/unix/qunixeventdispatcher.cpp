// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#include "qplatformdefs.h"
#include "qcoreapplication.h"
#include "qunixeventdispatcher_qpa_p.h"
#include "private/qguiapplication_p.h"

#include <qpa/qwindowsysteminterface.h>

#include <BobUICore/QDebug>

BOBUI_BEGIN_NAMESPACE

BOBUI_USE_NAMESPACE


QUnixEventDispatcherQPA::QUnixEventDispatcherQPA(QObject *parent)
    : QEventDispatcherUNIX(parent)
{ }

QUnixEventDispatcherQPA::~QUnixEventDispatcherQPA()
{ }

bool QUnixEventDispatcherQPA::processEvents(QEventLoop::ProcessEventsFlags flags)
{
    const bool didSendEvents = QEventDispatcherUNIX::processEvents(flags);
    return QWindowSystemInterface::sendWindowSystemEvents(flags) || didSendEvents;
}

BOBUI_END_NAMESPACE
