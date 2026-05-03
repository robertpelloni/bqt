// Copyright (C) 2016 Intel Corporation.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

#include <BobUIDBus/QDBusMessage>

#define HAS_HOOKSETUPFUNCTION       1
static void hookSetupFunction();

// Ugly hack, look away
#include "../qdbusconnection/tst_qdbusconnection.cpp"

BOBUI_BEGIN_NAMESPACE
extern Q_DBUS_EXPORT void qDBusAddSpyHook(void (*Hook)(const QDBusMessage&));
BOBUI_END_NAMESPACE

static void hookFunction(const QDBusMessage &)
{
//    qDebug() << "hook called";
    ++tst_QDBusConnection::hookCallCount;
}

static void hookSetupFunction()
{
    BOBUI_PREPEND_NAMESPACE(qDBusAddSpyHook)(hookFunction);
}

BOBUIEST_MAIN(tst_QDBusConnection_SpyHook)
