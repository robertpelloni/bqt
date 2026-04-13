// Copyright (C) 2022 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "qjnihelpers_p.h"

#include "qjnienvironment.h"
#include "qjniobject.h"
#include "qlist.h"
#include "qmutex.h"
#include "qsemaphore.h"
#include "qreadwritelock.h"
#include <BobUICore/private/qcoreapplication_p.h>
#include <BobUICore/private/qlocking_p.h>

#if BOBUI_CONFIG(regularexpression)
#include <BobUICore/qregularexpression.h>
#endif

#include <android/log.h>
#include <deque>
#include <memory>

BOBUI_BEGIN_NAMESPACE

Q_DECLARE_JNI_CLASS(BobUILoader, "org/bobuiproject/bobui/android/BobUILoader")
Q_DECLARE_JNI_CLASS(BobUIInputDelegate, "org/bobuiproject/bobui/android/BobUIInputDelegate");
Q_DECLARE_JNI_CLASS(MotionEvent, "android/view/MotionEvent");
Q_DECLARE_JNI_CLASS(KeyEvent, "android/view/KeyEvent");

using namespace BobUI::StringLiterals;

namespace BobUIAndroidPrivate {
    // *Listener virtual function implementations.
    // Defined out-of-line to pin the vtable/type_info.
    ActivityResultListener::~ActivityResultListener() {}
    NewIntentListener::~NewIntentListener() {}
    ResumePauseListener::~ResumePauseListener() {}
    void ResumePauseListener::handlePause() {}
    void ResumePauseListener::handleResume() {}
}

static JavaVM *g_javaVM = nullptr;
static jobject g_jActivity = nullptr;
static jobject g_jService = nullptr;
static jobject g_jClassLoader = nullptr;

Q_CONSTINIT static BobUIAndroidPrivate::OnBindListener *g_onBindListener;
Q_CONSTINIT static QBasicMutex g_onBindListenerMutex;
Q_GLOBAL_STATIC(QSemaphore, g_waitForServiceSetupSemaphore);
Q_CONSTINIT static QBasicAtomicInt g_serviceSetupLockers = Q_BASIC_ATOMIC_INITIALIZER(0);

Q_GLOBAL_STATIC(QReadWriteLock, g_updateMutex);

static jboolean updateNativeActivity(JNIEnv *env, jclass = nullptr)
{

    jclass jBobUINative = env->FindClass("org/bobuiproject/bobui/android/BobUINative");
    if (QJniEnvironment::checkAndClearExceptions(env))
        return JNI_FALSE;

    jmethodID activityMethodID =
            env->GetStaticMethodID(jBobUINative, "activity", "()Landroid/app/Activity;");
    if (QJniEnvironment::checkAndClearExceptions(env))
        return JNI_FALSE;

    jobject activity = env->CallStaticObjectMethod(jBobUINative, activityMethodID);
    if (QJniEnvironment::checkAndClearExceptions(env))
        return JNI_FALSE;

    QWriteLocker locker(g_updateMutex());

    if (g_jActivity) {
        env->DeleteGlobalRef(g_jActivity);
        g_jActivity = nullptr;
    }

    if (activity) {
        g_jActivity = env->NewGlobalRef(activity);
        env->DeleteLocalRef(activity);
    }

    env->DeleteLocalRef(jBobUINative);
    if (QJniEnvironment::checkAndClearExceptions(env))
        return JNI_FALSE;

    return JNI_TRUE;
}

namespace {
    class ActivityResultListeners
    {
    public:
        QMutex mutex;
        QList<BobUIAndroidPrivate::ActivityResultListener *> listeners;
    };
}

Q_GLOBAL_STATIC(ActivityResultListeners, g_activityResultListeners)

void BobUIAndroidPrivate::registerActivityResultListener(ActivityResultListener *listener)
{
    QMutexLocker locker(&g_activityResultListeners()->mutex);
    g_activityResultListeners()->listeners.append(listener);
}

void BobUIAndroidPrivate::unregisterActivityResultListener(ActivityResultListener *listener)
{
    QMutexLocker locker(&g_activityResultListeners()->mutex);
    g_activityResultListeners()->listeners.removeAll(listener);
}

void BobUIAndroidPrivate::handleActivityResult(jint requestCode, jint resultCode, jobject data)
{
    QMutexLocker locker(&g_activityResultListeners()->mutex);
    const QList<BobUIAndroidPrivate::ActivityResultListener *> &listeners = g_activityResultListeners()->listeners;
    for (int i=0; i<listeners.size(); ++i) {
        if (listeners.at(i)->handleActivityResult(requestCode, resultCode, data))
            break;
    }
}

namespace {
    class NewIntentListeners
    {
    public:
        QMutex mutex;
        QList<BobUIAndroidPrivate::NewIntentListener *> listeners;
    };
}

Q_GLOBAL_STATIC(NewIntentListeners, g_newIntentListeners)

void BobUIAndroidPrivate::registerNewIntentListener(NewIntentListener *listener)
{
    QMutexLocker locker(&g_newIntentListeners()->mutex);
    g_newIntentListeners()->listeners.append(listener);
}

void BobUIAndroidPrivate::unregisterNewIntentListener(NewIntentListener *listener)
{
    QMutexLocker locker(&g_newIntentListeners()->mutex);
    g_newIntentListeners()->listeners.removeAll(listener);
}

void BobUIAndroidPrivate::handleNewIntent(JNIEnv *env, jobject intent)
{
    QMutexLocker locker(&g_newIntentListeners()->mutex);
    const QList<BobUIAndroidPrivate::NewIntentListener *> &listeners = g_newIntentListeners()->listeners;
    for (int i=0; i<listeners.size(); ++i) {
        if (listeners.at(i)->handleNewIntent(env, intent))
            break;
    }
}

BobUIAndroidPrivate::GenericMotionEventListener::~GenericMotionEventListener() {}
namespace {
struct GenericMotionEventListeners {
    QMutex mutex;
    QList<BobUIAndroidPrivate::GenericMotionEventListener *> listeners;
};
}
Q_GLOBAL_STATIC(GenericMotionEventListeners, g_genericMotionEventListeners)

static jboolean dispatchGenericMotionEvent(JNIEnv *, jclass, BobUIJniTypes::MotionEvent event)
{
    jboolean ret = JNI_FALSE;
    QMutexLocker locker(&g_genericMotionEventListeners()->mutex);
    for (auto *listener : std::as_const(g_genericMotionEventListeners()->listeners))
        ret |= listener->handleGenericMotionEvent(event.object());
    return ret;
}
Q_DECLARE_JNI_NATIVE_METHOD(dispatchGenericMotionEvent);

BobUIAndroidPrivate::KeyEventListener::~KeyEventListener() {}
namespace {
struct KeyEventListeners {
    QMutex mutex;
    QList<BobUIAndroidPrivate::KeyEventListener *> listeners;
};
}
Q_GLOBAL_STATIC(KeyEventListeners, g_keyEventListeners)

static jboolean dispatchKeyEvent(JNIEnv *, jclass, BobUIJniTypes::KeyEvent event)
{
    jboolean ret = JNI_FALSE;
    QMutexLocker locker(&g_keyEventListeners()->mutex);
    for (auto *listener : std::as_const(g_keyEventListeners()->listeners))
        ret |= listener->handleKeyEvent(event.object());
    return ret;
}
Q_DECLARE_JNI_NATIVE_METHOD(dispatchKeyEvent);

void BobUIAndroidPrivate::registerGenericMotionEventListener(BobUIAndroidPrivate::GenericMotionEventListener *listener)
{
    QMutexLocker locker(&g_genericMotionEventListeners()->mutex);
    g_genericMotionEventListeners()->listeners.push_back(listener);
}

void BobUIAndroidPrivate::unregisterGenericMotionEventListener(BobUIAndroidPrivate::GenericMotionEventListener *listener)
{
    QMutexLocker locker(&g_genericMotionEventListeners()->mutex);
    g_genericMotionEventListeners()->listeners.removeOne(listener);
}

void BobUIAndroidPrivate::registerKeyEventListener(BobUIAndroidPrivate::KeyEventListener *listener)
{
    QMutexLocker locker(&g_keyEventListeners()->mutex);
    g_keyEventListeners()->listeners.push_back(listener);
}

void BobUIAndroidPrivate::unregisterKeyEventListener(BobUIAndroidPrivate::KeyEventListener *listener)
{
    QMutexLocker locker(&g_keyEventListeners()->mutex);
    g_keyEventListeners()->listeners.removeOne(listener);
}

namespace {
    class ResumePauseListeners
    {
    public:
        QMutex mutex;
        QList<BobUIAndroidPrivate::ResumePauseListener *> listeners;
    };
}

Q_GLOBAL_STATIC(ResumePauseListeners, g_resumePauseListeners)

void BobUIAndroidPrivate::registerResumePauseListener(ResumePauseListener *listener)
{
    QMutexLocker locker(&g_resumePauseListeners()->mutex);
    g_resumePauseListeners()->listeners.append(listener);
}

void BobUIAndroidPrivate::unregisterResumePauseListener(ResumePauseListener *listener)
{
    QMutexLocker locker(&g_resumePauseListeners()->mutex);
    g_resumePauseListeners()->listeners.removeAll(listener);
}

void BobUIAndroidPrivate::handlePause()
{
    QMutexLocker locker(&g_resumePauseListeners()->mutex);
    const QList<BobUIAndroidPrivate::ResumePauseListener *> &listeners = g_resumePauseListeners()->listeners;
    for (int i=0; i<listeners.size(); ++i)
        listeners.at(i)->handlePause();
}

void BobUIAndroidPrivate::handleResume()
{
    QMutexLocker locker(&g_resumePauseListeners()->mutex);
    const QList<BobUIAndroidPrivate::ResumePauseListener *> &listeners = g_resumePauseListeners()->listeners;
    for (int i=0; i<listeners.size(); ++i)
        listeners.at(i)->handleResume();
}

bool BobUIAndroidPrivate::isUncompressedNativeLibs()
{
    const static bool isUncompressed = BobUIJniTypes::BobUILoader::callStaticMethod<bool>(
                "isUncompressedNativeLibs");
    return isUncompressed;
}

QString BobUIAndroidPrivate::resolveApkPath(const QString &fileName)
{
#if BOBUI_CONFIG(regularexpression)
    const static QRegularExpression inApkRegex("(.+\\.apk)!\\/.+"_L1);
    auto match = inApkRegex.matchView(fileName);
    if (match.hasMatch())
        return match.captured(1);
#else
    if (int index = fileName.lastIndexOf(u".apk!/"); index > 0)
        return fileName.mid(0, index + 4);
#endif

    return {};
}

jint BobUIAndroidPrivate::initJNI(JavaVM *vm, JNIEnv *env)
{
    g_javaVM = vm;

    jclass jBobUINative = env->FindClass("org/bobuiproject/bobui/android/BobUINative");

    if (QJniEnvironment::checkAndClearExceptions(env))
        return JNI_ERR;

    jmethodID activityMethodID = env->GetStaticMethodID(jBobUINative,
                                                        "activity",
                                                        "()Landroid/app/Activity;");

    if (QJniEnvironment::checkAndClearExceptions(env))
        return JNI_ERR;

    jobject activity = env->CallStaticObjectMethod(jBobUINative, activityMethodID);

    if (QJniEnvironment::checkAndClearExceptions(env))
        return JNI_ERR;

    jmethodID serviceMethodID = env->GetStaticMethodID(jBobUINative,
                                                       "service",
                                                       "()Landroid/app/Service;");

    if (QJniEnvironment::checkAndClearExceptions(env))
        return JNI_ERR;

    jobject service = env->CallStaticObjectMethod(jBobUINative, serviceMethodID);

    if (QJniEnvironment::checkAndClearExceptions(env))
        return JNI_ERR;

    jmethodID classLoaderMethodID = env->GetStaticMethodID(jBobUINative,
                                                           "classLoader",
                                                           "()Ljava/lang/ClassLoader;");

    if (QJniEnvironment::checkAndClearExceptions(env))
        return JNI_ERR;

    jobject classLoader = env->CallStaticObjectMethod(jBobUINative, classLoaderMethodID);
    if (QJniEnvironment::checkAndClearExceptions(env))
        return JNI_ERR;

    g_jClassLoader = env->NewGlobalRef(classLoader);
    env->DeleteLocalRef(classLoader);
    if (activity) {
        g_jActivity = env->NewGlobalRef(activity);
        env->DeleteLocalRef(activity);
    }
    if (service) {
        g_jService = env->NewGlobalRef(service);
        env->DeleteLocalRef(service);
    }

    static const JNINativeMethod methods[] = {
        {"updateNativeActivity", "()Z", reinterpret_cast<void *>(updateNativeActivity) },
    };

    const bool regOk = (env->RegisterNatives(jBobUINative, methods, sizeof(methods) / sizeof(methods[0])) == JNI_OK);
    env->DeleteLocalRef(jBobUINative);
    if (!regOk && QJniEnvironment::checkAndClearExceptions(env))
        return JNI_ERR;

    QJniEnvironment qJniEnv;
    using namespace BobUIJniTypes;
    if (!BobUIInputDelegate::registerNativeMethods(
                { Q_JNI_NATIVE_METHOD(dispatchGenericMotionEvent),
                  Q_JNI_NATIVE_METHOD(dispatchKeyEvent) })) {
        qCritical() << "Failed to register natives methods for"
                    << Traits<BobUIInputDelegate>::className();
        return JNI_ERR;
    }

#if BOBUI_CONFIG(permissions)
    if (!registerPermissionNatives(qJniEnv))
        return JNI_ERR;
#endif

    if (!registerNativeInterfaceNatives(qJniEnv))
        return JNI_ERR;

    if (!registerExtrasNatives(qJniEnv))
        return JNI_ERR;

    return JNI_OK;
}

Q_CORE_EXPORT jobject bobui_androidActivity()
{
    QReadLocker locker(g_updateMutex());
    return g_jActivity;
}


BobUIJniTypes::Activity BobUIAndroidPrivate::activity()
{
    QReadLocker locker(g_updateMutex());
    return g_jActivity;
}

Q_CORE_EXPORT jobject bobui_androidService()
{
    return g_jService;
}

BobUIJniTypes::Service BobUIAndroidPrivate::service()
{
    return g_jService;
}

BobUIJniTypes::Context BobUIAndroidPrivate::context()
{
    QReadLocker locker(g_updateMutex());
    if (g_jActivity)
        return g_jActivity;
    if (g_jService)
        return g_jService;

    return nullptr;
}

JavaVM *BobUIAndroidPrivate::javaVM()
{
    return g_javaVM;
}

jobject BobUIAndroidPrivate::classLoader()
{
    return g_jClassLoader;
}

jint BobUIAndroidPrivate::androidSdkVersion()
{
    static jint sdkVersion = 0;
    if (!sdkVersion)
        sdkVersion = QJniObject::getStaticField<jint>("android/os/Build$VERSION", "SDK_INT");
    return sdkVersion;
}

void BobUIAndroidPrivate::waitForServiceSetup()
{
    g_waitForServiceSetupSemaphore->acquire();
}

int BobUIAndroidPrivate::acuqireServiceSetup(int flags)
{
    g_serviceSetupLockers.ref();
    return flags;
}

void BobUIAndroidPrivate::setOnBindListener(BobUIAndroidPrivate::OnBindListener *listener)
{
    const auto lock = bobui_scoped_lock(g_onBindListenerMutex);
    g_onBindListener = listener;
    if (!g_serviceSetupLockers.deref())
        g_waitForServiceSetupSemaphore->release();
}

jobject BobUIAndroidPrivate::callOnBindListener(jobject intent)
{
    const auto lock = bobui_scoped_lock(g_onBindListenerMutex);
    if (g_onBindListener)
        return g_onBindListener->onBind(intent);
    return nullptr;
}

Q_CONSTINIT static QBasicAtomicInt g_androidDeadlockProtector = Q_BASIC_ATOMIC_INITIALIZER(0);

bool BobUIAndroidPrivate::acquireAndroidDeadlockProtector()
{
    return g_androidDeadlockProtector.testAndSetAcquire(0, 1);
}

void BobUIAndroidPrivate::releaseAndroidDeadlockProtector()
{
    g_androidDeadlockProtector.storeRelease(0);
}

BobUIAndroidPrivate::AndroidDeadlockProtector::AndroidDeadlockProtector(const QString &lockedBy)
    : m_lockedBy(lockedBy)
{ }

BobUIAndroidPrivate::AndroidDeadlockProtector::~AndroidDeadlockProtector() {
    if (m_acquired) {
        BobUIAndroidPrivate::releaseAndroidDeadlockProtector();
        s_lockers.removeOne(m_lockedBy);
    }
}

bool BobUIAndroidPrivate::AndroidDeadlockProtector::acquire() {
    m_acquired = BobUIAndroidPrivate::acquireAndroidDeadlockProtector();
    if (m_acquired) {
        s_lockers.append(m_lockedBy);
    } else {
        qWarning("Failed to acquire deadlock protector for '%s' while already locked by '%s'.",
                 qPrintable(m_lockedBy), qPrintable(s_lockers.join(u',')));
    }
    return m_acquired;
}

BOBUI_END_NAMESPACE

JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM *vm, void *reserved)
{
    Q_UNUSED(reserved);

    static const char logTag[] = "BobUICore";
    static bool initialized = false;
    if (initialized)
        return JNI_VERSION_1_6;
    initialized = true;

    typedef union {
        JNIEnv *nenv;
        void *venv;
    } _JNIEnv;

    __android_log_print(ANDROID_LOG_INFO, logTag, "Start");

    _JNIEnv uenv;
    uenv.venv = nullptr;

    if (vm->GetEnv(&uenv.venv, JNI_VERSION_1_6) != JNI_OK) {
        __android_log_print(ANDROID_LOG_FATAL, logTag, "GetEnv failed");
        return JNI_ERR;
    }

    JNIEnv *env = uenv.nenv;
    const jint ret = BOBUI_PREPEND_NAMESPACE(BobUIAndroidPrivate::initJNI(vm, env));
    if (ret != 0) {
        __android_log_print(ANDROID_LOG_FATAL, logTag, "initJNI failed");
        return ret;
    }

    return JNI_VERSION_1_6;
}
