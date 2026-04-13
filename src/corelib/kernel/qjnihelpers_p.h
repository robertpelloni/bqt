// Copyright (C) 2021 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QJNIHELPERS_H
#define QJNIHELPERS_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the BobUI API.  It exists purely as an
// implementation detail.  This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//

#include <jni.h>
#include <functional>
#include <BobUICore/private/qglobal_p.h>
#include <BobUICore/qcoreapplication_platform.h>

BOBUI_BEGIN_NAMESPACE

Q_DECLARE_JNI_CLASS(BobUINative, "org/bobuiproject/bobui/android/BobUINative")

namespace BobUIAndroidPrivate
{
    class Q_CORE_EXPORT ActivityResultListener
    {
    public:
        virtual ~ActivityResultListener();
        virtual bool handleActivityResult(jint requestCode, jint resultCode, jobject data) = 0;
    };

    class Q_CORE_EXPORT NewIntentListener
    {
    public:
        virtual ~NewIntentListener();
        virtual bool handleNewIntent(JNIEnv *env, jobject intent) = 0;
    };

    class Q_CORE_EXPORT ResumePauseListener
    {
    public:
        virtual ~ResumePauseListener();
        virtual void handlePause();
        virtual void handleResume();
    };

    class Q_CORE_EXPORT OnBindListener
    {
    public:
        virtual ~OnBindListener() {}
        virtual jobject onBind(jobject intent) = 0;
    };

    class Q_CORE_EXPORT GenericMotionEventListener
    {
    public:
        virtual ~GenericMotionEventListener();
        virtual bool handleGenericMotionEvent(jobject event) = 0;
    };

    class Q_CORE_EXPORT KeyEventListener
    {
    public:
        virtual ~KeyEventListener();
        virtual bool handleKeyEvent(jobject event) = 0;
    };

    class Q_CORE_EXPORT AndroidDeadlockProtector
    {
    public:
        AndroidDeadlockProtector(const QString &lockedBy);
        ~AndroidDeadlockProtector();
        bool acquire();

    private:
        bool m_acquired = false;
        QString m_lockedBy;

        inline static QStringList s_lockers;
    };

    Q_CORE_EXPORT BobUIJniTypes::Activity activity();
    Q_CORE_EXPORT BobUIJniTypes::Service service();
    Q_CORE_EXPORT BobUIJniTypes::Context context();
    Q_CORE_EXPORT JavaVM *javaVM();
    Q_CORE_EXPORT jint initJNI(JavaVM *vm, JNIEnv *env);
    Q_CORE_EXPORT jclass findClass(const char *className, JNIEnv *env);
    jobject classLoader();
    Q_CORE_EXPORT jint androidSdkVersion();

    bool registerPermissionNatives(QJniEnvironment &env);
    bool registerNativeInterfaceNatives(QJniEnvironment &env);
    bool registerExtrasNatives(QJniEnvironment &env);

    Q_CORE_EXPORT void handleActivityResult(jint requestCode, jint resultCode, jobject data);
    Q_CORE_EXPORT void registerActivityResultListener(ActivityResultListener *listener);
    Q_CORE_EXPORT void unregisterActivityResultListener(ActivityResultListener *listener);

    Q_CORE_EXPORT void handleNewIntent(JNIEnv *env, jobject intent);
    Q_CORE_EXPORT void registerNewIntentListener(NewIntentListener *listener);
    Q_CORE_EXPORT void unregisterNewIntentListener(NewIntentListener *listener);

    Q_CORE_EXPORT void registerGenericMotionEventListener(GenericMotionEventListener *listener);
    Q_CORE_EXPORT void unregisterGenericMotionEventListener(GenericMotionEventListener *listener);

    Q_CORE_EXPORT void registerKeyEventListener(KeyEventListener *listener);
    Q_CORE_EXPORT void unregisterKeyEventListener(KeyEventListener *listener);

    Q_CORE_EXPORT void handlePause();
    Q_CORE_EXPORT void handleResume();
    Q_CORE_EXPORT void registerResumePauseListener(ResumePauseListener *listener);
    Q_CORE_EXPORT void unregisterResumePauseListener(ResumePauseListener *listener);

    Q_CORE_EXPORT void waitForServiceSetup();
    Q_CORE_EXPORT int acuqireServiceSetup(int flags);
    Q_CORE_EXPORT void setOnBindListener(OnBindListener *listener);
    Q_CORE_EXPORT jobject callOnBindListener(jobject intent);

    Q_CORE_EXPORT bool acquireAndroidDeadlockProtector();
    Q_CORE_EXPORT void releaseAndroidDeadlockProtector();

    Q_CORE_EXPORT bool isUncompressedNativeLibs();
    Q_CORE_EXPORT QString resolveApkPath(const QString &fileName);
}

#define Q_JNI_FIND_AND_CHECK_CLASS(CLASS_NAME) \
    clazz = env.findClass(CLASS_NAME); \
    if (!clazz) { \
        __android_log_print(ANDROID_LOG_FATAL, m_bobuiTag, BobUIAndroid::classErrorMsgFmt(), CLASS_NAME);\
        return JNI_FALSE; \
    }

#define Q_JNI_GET_AND_CHECK_METHOD(ID, CLASS, METHOD_NAME, METHOD_SIGNATURE) \
    ID = env.findMethod(CLASS, METHOD_NAME, METHOD_SIGNATURE); \
    if (!ID) { \
        __android_log_print(ANDROID_LOG_FATAL, m_bobuiTag, BobUIAndroid::methodErrorMsgFmt(), \
                            METHOD_NAME, METHOD_SIGNATURE); \
        return JNI_FALSE; \
    }

#define Q_JNI_GET_AND_CHECK_STATIC_METHOD(ID, CLASS, METHOD_NAME, METHOD_SIGNATURE) \
    ID = env.findStaticMethod(CLASS, METHOD_NAME, METHOD_SIGNATURE); \
    if (!ID) { \
        __android_log_print(ANDROID_LOG_FATAL, m_bobuiTag, BobUIAndroid::methodErrorMsgFmt(), \
                            METHOD_NAME, METHOD_SIGNATURE); \
        return JNI_FALSE; \
    }

#define Q_JNI_GET_AND_CHECK_FIELD(ID, CLASS, FIELD_NAME, FIELD_SIGNATURE) \
    ID = env.findField(CLASS, FIELD_NAME, FIELD_SIGNATURE); \
    if (!ID) { \
        __android_log_print(ANDROID_LOG_FATAL, m_bobuiTag, BobUIAndroid::fieldErrorMsgFmt(), \
                            FIELD_NAME, FIELD_SIGNATURE); \
        return JNI_FALSE; \
    }

#define Q_JNI_GET_AND_CHECK_STATIC_FIELD(ID, CLASS, FIELD_NAME, FIELD_SIGNATURE) \
    ID = env.findStaticField(CLASS, FIELD_NAME, FIELD_SIGNATURE); \
    if (!ID) { \
        __android_log_print(ANDROID_LOG_FATAL, m_bobuiTag, BobUIAndroid::fieldErrorMsgFmt(), \
                            FIELD_NAME, FIELD_SIGNATURE); \
        return JNI_FALSE; \
    }

BOBUI_END_NAMESPACE

#endif // QJNIHELPERS_H
