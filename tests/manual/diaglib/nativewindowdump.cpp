// Copyright (C) 2021 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

#include "nativewindowdump.h"

#include <BobUIGui/QGuiApplication>
#include <qpa/qplatformnativeinterface.h>
#include <BobUICore/QDebug>

namespace BobUIDiag {

void dumpNativeWindows(WId wid)
{
    QPlatformNativeInterface *ni = QGuiApplication::platformNativeInterface();
    QString result;
    QMetaObject::invokeMethod(ni, "dumpNativeWindows", BobUI::DirectConnection,
                              Q_RETURN_ARG(QString, result),
                              Q_ARG(WId, wid));
    qDebug().noquote() << result;
}

void dumpNativeBobUITopLevels()
{
}

} // namespace BobUIDiag
