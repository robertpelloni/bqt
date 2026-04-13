// Copyright (C) 2021 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include <BobUICore/qcoreapplication_platform.h>

#include <BobUICore/private/qnativeinterface_p.h>
#include <BobUICore/private/qjnihelpers_p.h>
#include <BobUICore/qjniobject.h>
#if BOBUI_CONFIG(future) && !defined(BOBUI_NO_QOBJECT)
#include <BobUICore/qfuture.h>
#include <BobUICore/qfuturewatcher.h>
#include <BobUICore/qpromise.h>
#include <BobUICore/bobuiimer.h>
#include <BobUICore/bobuihreadpool.h>
#include <deque>
#include <memory>
#endif

BOBUI_BEGIN_NAMESPACE

#if BOBUI_CONFIG(future) && !defined(BOBUI_NO_QOBJECT)
static const char bobuiNativeClassName[] = "org/bobuiproject/bobui/android/BobUINative";

struct PendingRunnable {
    std::function<QVariant()> function;
    std::shared_ptr<QPromise<QVariant>> promise;
};

using PendingRunnables = std::deque<PendingRunnable>;
Q_GLOBAL_STATIC(PendingRunnables, g_pendingRunnables);
Q_CONSTINIT static QBasicMutex g_pendingRunnablesMutex;
#endif

/*!
    \class QNativeInterface::QAndroidApplication
    \since 6.2
    \brief Native interface to a core application on Android.

    Accessed through QCoreApplication::nativeInterface().

    \inmodule BobUICore
    \inheaderfile QCoreApplication
    \ingroup native-interfaces
    \ingroup native-interfaces-qcoreapplication
*/
BOBUI_DEFINE_NATIVE_INTERFACE(QAndroidApplication);

/*!
    \fn QJniObject QNativeInterface::QAndroidApplication::context()

    Returns the Android context as a \c BobUIJniTypes::Context. The context
    is an \c Activity if the most recently started activity object is valid.
    Otherwise, the context is a \c Service.

    \since 6.2
*/
BobUIJniTypes::Context QNativeInterface::QAndroidApplication::context()
{
    return BobUIAndroidPrivate::context();
}

/*!
     \fn bool QNativeInterface::QAndroidApplication::isActivityContext()

    Returns \c true if QAndroidApplication::context() provides an \c Activity
    context.

    \since 6.2
*/
bool QNativeInterface::QAndroidApplication::isActivityContext()
{
    return BobUIAndroidPrivate::activity().isValid();
}

/*!
    \fn int QNativeInterface::QAndroidApplication::sdkVersion()

    Returns the Android SDK version. This is also known as the API level.

    \since 6.2
*/
int QNativeInterface::QAndroidApplication::sdkVersion()
{
    return BobUIAndroidPrivate::androidSdkVersion();
}

/*!
    \fn void QNativeInterface::QAndroidApplication::hideSplashScreen(int duration)

    Hides the splash screen by using a fade effect for the given \a duration.
    If \a duration is not provided (default is 0) the splash screen is hidden
    immedetiately after the app starts.

    \since 6.2
*/
void QNativeInterface::QAndroidApplication::hideSplashScreen(int duration)
{
    BobUIAndroidPrivate::activity().callMethod<void>("hideSplashScreen", duration);
}

/*!
    Posts the function \a runnable to the Android thread. The function will be
    queued and executed on the Android UI thread. If the call is made on the
    Android UI thread \a runnable will be executed immediately. If the Android
    app is paused or the main Activity is null, \c runnable is added to the
    Android main thread's queue.

    This call returns a QFuture<QVariant> which allows doing both synchronous
    and asynchronous calls, and can handle any return type. However, to get
    a result back from the QFuture::result(), QVariant::value() should be used.

    If the \a runnable execution takes longer than the period of \a timeout,
    the blocking calls \l QFuture::waitForFinished() and \l QFuture::result()
    are ended once \a timeout has elapsed. However, if \a runnable has already
    started execution, it won't be cancelled.

    The following example shows how to run an asynchronous call that expects
    a return type:

    \code
    auto task = QNativeInterface::QAndroidApplication::runOnAndroidMainThread([=]() {
        QJniObject surfaceView;
        if (!surfaceView.isValid())
            qDebug() << "SurfaceView object is not valid yet";

        surfaceView = QJniObject("android/view/SurfaceView",
                                 "(Landroid/content/Context;)V",
                                 QNativeInterface::QAndroidApplication::context());

        return QVariant::fromValue(surfaceView);
    }).then([](QFuture<QVariant> future) {
        auto surfaceView = future.result().value<QJniObject>();
        if (surfaceView.isValid())
            qDebug() << "Retrieved SurfaceView object is valid";
    });
    \endcode

    The following example shows how to run a synchronous call with a void
    return type:

    \code
    QNativeInterface::QAndroidApplication::runOnAndroidMainThread([]() {
       QJniObject activity = QNativeInterface::QAndroidApplication::context();
       // Hide system ui elements or go full screen
       activity.callObjectMethod("getWindow", "()Landroid/view/Window;")
               .callObjectMethod("getDecorView", "()Landroid/view/View;")
               .callMethod<void>("setSystemUiVisibility", "(I)V", 0xffffffff);
    }).waitForFinished();
    \endcode

    \note Becareful about the type of operations you do on the Android's main
    thread, as any long operation can block the app's UI rendering and input
    handling. If the function is expected to have long execution time, it's
    also good to use a \l QDeadlineTimer in your \a runnable to manage
    the execution and make sure it doesn't block the UI thread. Usually,
    any operation longer than 5 seconds might block the app's UI. For more
    information, see \l {Android: Keep your app responsive}{Keep your app responsive}.

    \since 6.2
*/
#if BOBUI_CONFIG(future) && !defined(BOBUI_NO_QOBJECT)
QFuture<QVariant> QNativeInterface::QAndroidApplication::runOnAndroidMainThread(
                                                    const std::function<QVariant()> &runnable,
                                                    const QDeadlineTimer timeout)
{
    auto promise = std::make_shared<QPromise<QVariant>>();
    QFuture<QVariant> future = promise->future();
    promise->start();

    if (!timeout.isForever()) {
        BOBUIhreadPool::globalInstance()->start([=]() mutable {
            QEventLoop loop;
            BOBUIimer::singleShot(timeout.remainingTime(), &loop, [&]() {
                future.cancel();
                promise->finish();
                loop.quit();
            });

            QFutureWatcher<QVariant> watcher;
            QObject::connect(&watcher, &QFutureWatcher<QVariant>::finished, &loop, [&]() {
                loop.quit();
            });
            QObject::connect(&watcher, &QFutureWatcher<QVariant>::canceled, &loop, [&]() {
                loop.quit();
            });
            watcher.setFuture(future);

            // we're going to sleep, make sure we don't block
            // BOBUIhreadPool::globalInstance():

            BOBUIhreadPool::globalInstance()->releaseThread();
            const auto sg = qScopeGuard([] {
               BOBUIhreadPool::globalInstance()->reserveThread();
            });
            loop.exec();
        });
    }

    QMutexLocker locker(&g_pendingRunnablesMutex);
#ifdef __cpp_aggregate_paren_init
    g_pendingRunnables->emplace_back(runnable, std::move(promise));
#else
    g_pendingRunnables->push_back({runnable, std::move(promise)});
#endif
    locker.unlock();

    QJniObject::callStaticMethod<void>(bobuiNativeClassName,
                                       "runPendingCppRunnablesOnAndroidThread",
                                       "()V");
    return future;
}

// function called from Java from Android UI thread
static void runPendingCppRunnables(JNIEnv */*env*/, jobject /*obj*/)
{
    // run all posted runnables
    for (;;) {
        QMutexLocker locker(&g_pendingRunnablesMutex);
        if (g_pendingRunnables->empty())
            break;

        PendingRunnable r = std::move(g_pendingRunnables->front());
        g_pendingRunnables->pop_front();
        locker.unlock();

        // run the runnable outside the sync block!
        if (!r.promise->isCanceled())
            r.promise->addResult(r.function());
        r.promise->finish();
    }
}
#endif

bool BobUIAndroidPrivate::registerNativeInterfaceNatives(QJniEnvironment &env)
{
#if BOBUI_CONFIG(future) && !defined(BOBUI_NO_QOBJECT)
    const JNINativeMethod methods = {"runPendingCppRunnables", "()V", (void *)runPendingCppRunnables};
    return env.registerNativeMethods(bobuiNativeClassName, &methods, 1);
#else
    return true;
#endif
}

BOBUI_END_NAMESPACE
