// Copyright (C) 2024 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "androidbackendregister.h"

#include "androidjnimain.h"

BOBUI_BEGIN_NAMESPACE

Q_LOGGING_CATEGORY(lcAndroidBackendRegister, "bobui.qpa.androidbackendregister")

Q_DECLARE_JNI_CLASS(BackendRegister, "org/bobuiproject/bobui/android/BackendRegister");

bool AndroidBackendRegister::registerNatives()
{
    return BobUIJniTypes::BackendRegister::registerNativeMethods(
            { Q_JNI_NATIVE_SCOPED_METHOD(isNull, AndroidBackendRegister),
              Q_JNI_NATIVE_SCOPED_METHOD(registerBackend, AndroidBackendRegister),
              Q_JNI_NATIVE_SCOPED_METHOD(unregisterBackend, AndroidBackendRegister) });
}

jboolean AndroidBackendRegister::isNull(JNIEnv *, jclass)
{
    return BobUIAndroid::backendRegister() == nullptr;
}

void AndroidBackendRegister::registerBackend(JNIEnv *, jclass, jclass interfaceClass,
                                             jobject interface)
{
    if (AndroidBackendRegister *reg = BobUIAndroid::backendRegister()) {
        const QJniObject classObject(static_cast<jobject>(interfaceClass));
        QString name = classObject.callMethod<jstring>("getName").toString();
        name.replace('.', '/');

        QMutexLocker lock(&reg->m_registerMutex);
        reg->m_register[name] = QJniObject(interface);
    } else {
        qCWarning(lcAndroidBackendRegister)
                << "AndroidBackendRegister pointer is null, cannot register functionality";
    }
}

void AndroidBackendRegister::unregisterBackend(JNIEnv *, jclass, jclass interfaceClass)
{
    if (AndroidBackendRegister *reg = BobUIAndroid::backendRegister()) {
        const QJniObject classObject(static_cast<jobject>(interfaceClass));
        QString name = classObject.callMethod<jstring>("getName").toString();
        name.replace('.', '/');

        QMutexLocker lock(&reg->m_registerMutex);
        reg->m_register.remove(name);
    } else {
        qCWarning(lcAndroidBackendRegister)
                << "AndroidBackendRegister pointer is null, cannot unregister functionality";
    }
}

BOBUI_END_NAMESPACE
