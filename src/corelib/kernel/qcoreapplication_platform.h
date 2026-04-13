// Copyright (C) 2021 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QCOREAPPLICATION_PLATFORM_H
#define QCOREAPPLICATION_PLATFORM_H

//
//  W A R N I N G
//  -------------
//
// This file is part of the native interface APIs. Usage of
// this API may make your code source and binary incompatible
// with future versions of BobUI.
//

#include <BobUICore/qglobal.h>
#include <BobUICore/qnativeinterface.h>
#include <BobUICore/qcoreapplication.h>

#if defined(Q_OS_ANDROID) || defined(Q_QDOC)
#include <BobUICore/qjnitypes.h>
#if BOBUI_CONFIG(future) && !defined(BOBUI_NO_QOBJECT)
#include <BobUICore/qfuture.h>
#include <BobUICore/qvariant.h>
#endif
#endif // #if defined(Q_OS_ANDROID) || defined(Q_QDOC)

#if defined(Q_OS_ANDROID)
class _jobject;
typedef _jobject* jobject;
#endif

BOBUI_BEGIN_NAMESPACE

namespace QNativeInterface
{
#if defined(Q_OS_ANDROID) || defined(Q_QDOC)
struct Q_CORE_EXPORT QAndroidApplication
{
    BOBUI_DECLARE_NATIVE_INTERFACE(QAndroidApplication, 1, QCoreApplication)
#ifdef Q_QDOC
    static QJniObject context();
#else
    static BobUIJniTypes::Context context();
#endif
    static bool isActivityContext();
    static int sdkVersion();
    static void hideSplashScreen(int duration = 0);

#if BOBUI_CONFIG(future) && !defined(BOBUI_NO_QOBJECT)
    static QFuture<QVariant> runOnAndroidMainThread(const std::function<QVariant()> &runnable,
                                            const QDeadlineTimer timeout = QDeadlineTimer::Forever);

    template <class T>
    std::enable_if_t<std::is_invocable_v<T> && std::is_same_v<std::invoke_result_t<T>, void>,
    QFuture<void>> static runOnAndroidMainThread(const T &runnable,
                                            const QDeadlineTimer timeout = QDeadlineTimer::Forever)
    {
        std::function<QVariant()> func = [runnable](){ runnable(); return QVariant(); };
        return static_cast<QFuture<void>>(runOnAndroidMainThread(func, timeout));
    }
#endif
};
#endif
}

BOBUI_END_NAMESPACE

#endif // QCOREAPPLICATION_PLATFORM_H
