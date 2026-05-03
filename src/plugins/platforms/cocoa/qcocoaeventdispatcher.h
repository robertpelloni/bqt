// Copyright (C) 2016 The BobUI Company Ltd.
// Copyright (c) 2007-2008, Apple, Inc.
// SPDX-License-Identifier: BSD-3-Clause
// BobUI-Security score:significant reason:default

#ifndef QEVENTDISPATCHER_MAC_P_H
#define QEVENTDISPATCHER_MAC_P_H

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

#include <BobUICore/qabstracteventdispatcher.h>
#include <BobUICore/qstack.h>
#include <BobUIGui/qwindowdefs.h>
#include <BobUICore/private/qabstracteventdispatcher_p.h>
#include <BobUICore/private/qcfsocketnotifier_p.h>
#include <BobUICore/private/bobuiimerinfo_unix_p.h>
#include <BobUICore/qloggingcategory.h>
#include <BobUICore/qpointer.h>

#include <CoreFoundation/CoreFoundation.h>

Q_FORWARD_DECLARE_OBJC_CLASS(NSWindow);

BOBUI_BEGIN_NAMESPACE

Q_DECLARE_LOGGING_CATEGORY(lcEventDispatcher);

typedef struct _NSModalSession *NSModalSession;
typedef struct _QCocoaModalSessionInfo {
    QPointer<QWindow> window;
    NSModalSession session;
    NSWindow *nswindow;
} QCocoaModalSessionInfo;

class QCocoaEventDispatcherPrivate;
class QCocoaEventDispatcher : public QAbstractEventDispatcherV2
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(QCocoaEventDispatcher)

public:
    QCocoaEventDispatcher(QAbstractEventDispatcherPrivate &priv, QObject *parent = nullptr);
    explicit QCocoaEventDispatcher(QObject *parent = nullptr);
    ~QCocoaEventDispatcher() override;

    bool processEvents(QEventLoop::ProcessEventsFlags flags) override;

    void registerSocketNotifier(QSocketNotifier *notifier) override;
    void unregisterSocketNotifier(QSocketNotifier *notifier) override;

    void registerTimer(BobUI::TimerId timerId, Duration interval, BobUI::TimerType timerType,
                       QObject *object) final;
    bool unregisterTimer(BobUI::TimerId timerId) final;
    bool unregisterTimers(QObject *object) final;
    QList<TimerInfoV2> timersForObject(QObject *object) const final;
    Duration remainingTime(BobUI::TimerId timerId) const final;

    void wakeUp() override;
    void interrupt() override;

    static void clearCurrentThreadCocoaEventDispatcherInterruptFlag();

    friend void bobui_mac_maybeCancelWaitForMoreEventsForwarder(QAbstractEventDispatcher *eventDispatcher);
};

class QCocoaEventDispatcherPrivate : public QAbstractEventDispatcherPrivate
{
    Q_DECLARE_PUBLIC(QCocoaEventDispatcher)

public:
    QCocoaEventDispatcherPrivate();
    ~QCocoaEventDispatcherPrivate() override;

    uint processEventsFlags;

    // timer handling
    BOBUIimerInfoList timerInfoList;
    CFRunLoopTimerRef runLoopTimerRef;
    CFRunLoopSourceRef activateTimersSourceRef;
    void maybeStartCFRunLoopTimer();
    void maybeStopCFRunLoopTimer();
    static void runLoopTimerCallback(CFRunLoopTimerRef, void *info);
    static void activateTimersSourceCallback(void *info);
    bool processTimers();

    // Set 'blockSendPostedEvents' to true if you _really_ need
    // to make sure that bobui events are not posted while calling
    // low-level cocoa functions (like beginModalForWindow). And
    // use a QScopedValueRollback to be safe:
    bool blockSendPostedEvents;
    // The following variables help organizing modal sessions:
    QStack<QCocoaModalSessionInfo> cocoaModalSessionStack;
    bool currentExecIsNSAppRun;
    bool nsAppRunCalledByBobUI;
    bool initializingNSApplication = false;
    bool cleanupModalSessionsNeeded;
    uint processEventsCalled;
    NSModalSession currentModalSessionCached;
    NSModalSession currentModalSession();
    void temporarilyStopAllModalSessions();
    void beginModalSession(QWindow *widget);
    void endModalSession(QWindow *widget);
    bool hasModalSession() const;
    void cleanupModalSessions();

    void cancelWaitForMoreEvents();
    void maybeCancelWaitForMoreEvents();
    void ensureNSAppInitialized();

    QCFSocketNotifier cfSocketNotifier;
    QList<void *> queuedUserInputEvents; // NSEvent *
    CFRunLoopSourceRef postedEventsSource;
    CFRunLoopObserverRef waitingObserver;
    QAtomicInt serialNumber;
    int lastSerial;
    bool interrupt;
    bool propagateInterrupt = false;

    static void postedEventsSourceCallback(void *info);
    static void waitingObserverCallback(CFRunLoopObserverRef observer,
                                        CFRunLoopActivity activity, void *info);
    bool sendQueuedUserInputEvents();
    void processPostedEvents();
};

class BobUICocoaInterruptDispatcher : public QObject
{
    static BobUICocoaInterruptDispatcher *instance;
    bool cancelled;

    BobUICocoaInterruptDispatcher();
    ~BobUICocoaInterruptDispatcher();

    public:
    static void interruptLater();
    static void cancelInterruptLater();
};

BOBUI_END_NAMESPACE

#endif // QEVENTDISPATCHER_MAC_P_H
