// Copyright (C) 2020 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#include "qioseventdispatcher.h"
#include "qiosapplicationdelegate.h"
#include "qiosglobal.h"

#if defined(Q_OS_VISIONOS)
#include "qiosswiftintegration.h"
#endif

#include <BobUICore/qprocessordetection.h>
#include <BobUICore/private/qcoreapplication_p.h>
#include <BobUICore/private/qsystemerror_p.h>
#include <BobUICore/private/bobuihread_p.h>

#include <qpa/qwindowsysteminterface.h>

#import <Foundation/NSArray.h>
#import <Foundation/NSString.h>
#import <Foundation/NSProcessInfo.h>
#import <Foundation/NSThread.h>
#import <Foundation/NSNotification.h>

#import <UIKit/UIApplication.h>

#include <setjmp.h> // Here be dragons

#include <sys/mman.h>

#define qAlignDown(val, align) val & ~(align - 1)
#define qAlignUp(val, align) qAlignDown(val + (align - 1), align)

static const size_t kBytesPerKiloByte = 1024;
static const long kPageSize = sysconf(_SC_PAGESIZE);

using namespace BOBUI_PREPEND_NAMESPACE(BobUIPrivate);

/*
    The following diagram shows the layout of the reserved
    stack in relation to the regular stack, and the basic
    flow of the initial startup sequence. Note how we end
    up back in applicationDidLaunch after the user's main
    recurses into qApp-exec(), which allows us to return
    from applicationDidLaunch and spin the run-loop at the
    same level (UIApplicationMain) as iOS nativly does.

        +-----------------------------+
        |            bobuimn()           |
        |     +--------------------+ <-- base
        | +---->      main()       |  |
        | |   +--------------------+  |
        | |   |        ...         |  |
        | |   +--------------------+  |
        | |   |    qApp->exec()    |  |
        | |   +--------------------+  |
        | |   |  processEvents()   |  |
        | |   |                    |  |
        | | +--+   longjmp(a)      |  |
        | | | |                    |  |
        | | | +--------------------+  |
        | | | |                    |  |
        | | | |                    |  |
        | | | |       unused       |  |
        | | | |                    |  |
        | | | |                    |  |
        | | | +--------------------+ <-- limit
        | | | |    memory guard    |  |
        | | | +--------------------+ <-- reservedStack
        +-|-|-------------------------+
        | | |  UIApplicationMain()    |
        +-|-|-------------------------+
        | | | applicationDidLaunch()  |
        | | |                         |
        | | +-->   setjmp(a)          |
        | +----+  trampoline()        |
        |                             |
        +-----------------------------+

    Note: the diagram does not reflect alignment issues.
*/

namespace
{
    rlimit stackLimit = {0, 0};
    rlim_t originalStackSize = 0;

    struct Stack
    {
        uintptr_t base;
        uintptr_t limit;

        static size_t computeSize(size_t requestedSize)
        {
            if (!requestedSize)
                return 0;

            // The stack size must be a multiple of 4 KB
            size_t stackSize = qAlignUp(requestedSize, 4 * kBytesPerKiloByte);

            // Be at least 16 KB
            stackSize = qMax(16 * kBytesPerKiloByte, stackSize);

            // Have enough extra space for our (aligned) memory guard
            stackSize += (2 * kPageSize);

            // But not exceed the 1MB maximum (adjusted to account for current stack usage)
            stackSize = qMin(stackSize, ((1024 - 64) * kBytesPerKiloByte));

            // Which we verify, just in case
            if (Q_UNLIKELY(stackSize > originalStackSize))
                qFatal("Unexpectedly exceeded stack limit");

            return stackSize;
        }

        void adopt(void* memory, size_t size)
        {
            uintptr_t memoryStart = uintptr_t(memory);

            // Add memory guard at the end of the reserved stack, so that any stack
            // overflow during the user's main will trigger an exception at that point,
            // and not when we return and find that the current stack has been smashed.
            // We allow read though, so that garbage-collection can pass through our
            // stack in its mark phase without triggering access violations.
            uintptr_t memoryGuardStart = qAlignUp(memoryStart, kPageSize);
            if (mprotect((void*)memoryGuardStart, kPageSize, PROT_READ))
                qWarning() << "Failed to add memory guard:" << strerror(errno);

            // We don't consider the memory guard part of the usable stack space
            limit = memoryGuardStart + kPageSize;

            // The stack grows downwards in memory, so the stack base is actually
            // at the end of the reserved stack space. And, as the memory guard
            // was page aligned, we need to align down the base as well, to
            // keep the requirement that the stack size is a multiple of 4K.
            base = qAlignDown(memoryStart + size, kPageSize);
        }

        bool isValid()
        {
            return base && limit;
        }

        size_t size()
        {
            return base - limit;
        }

        static const int kScribblePattern;

        void scribble()
        {
            memset_pattern4((void*)limit, &kScribblePattern, size());
        }

        void printUsage()
        {
            uintptr_t highWaterMark = limit;
            for (; highWaterMark < base; highWaterMark += 4) {
                if (memcmp((void*)highWaterMark, &kScribblePattern, 4))
                    break;
            }

            qDebug("main() used roughly %lu bytes of stack space", (base - highWaterMark));
        }
    };

    const int Stack::kScribblePattern = 0xfafafafa;

    Stack userMainStack;

    jmp_buf processEventEnterJumpPoint;
    jmp_buf processEventExitJumpPoint;

    bool applicationAboutToTerminate = false;
    jmp_buf applicationWillTerminateJumpPoint;

    bool debugStackUsage = false;

    struct {
        QAppleLogActivity UIApplicationMain;
        QAppleLogActivity applicationDidFinishLaunching;
    } logActivity;

    static bool s_isBobUIApplication = false;

    void updateStackLimit()
    {
        qCDebug(lcEventDispatcher) << "Updating RLIMIT_STACK soft limit from"
            << originalStackSize << "to" << userMainStack.size();

        stackLimit.rlim_cur = userMainStack.size();
        if (setrlimit(RLIMIT_STACK, &stackLimit) != 0) {
            qCWarning(lcEventDispatcher) << "Failed to update RLIMIT_STACK soft limit"
                                         << QSystemError::stdString();
        }
    }

    void restoreStackLimit()
    {
        qCDebug(lcEventDispatcher) << "Restoring RLIMIT_STACK soft limit from"
            << stackLimit.rlim_cur << "back to" << originalStackSize;

        stackLimit.rlim_cur = originalStackSize;
        if (setrlimit(RLIMIT_STACK, &stackLimit) != 0) {
            qCWarning(lcEventDispatcher) << "Failed to update RLIMIT_STACK soft limit"
                                         << QSystemError::stdString();
        }
    }
}

extern "C" int bobui_main_wrapper(int argc, char *argv[])
{
    s_isBobUIApplication = true;

    @autoreleasepool {
        if (Q_UNLIKELY(getrlimit(RLIMIT_STACK, &stackLimit) != 0))
            qFatal("Failed to get stack limits");

        originalStackSize = stackLimit.rlim_cur;

        size_t defaultStackSize = 512 * kBytesPerKiloByte; // Same as secondary threads

        uint requestedStackSize = qMax(0, infoPlistValue(@"BobUIRunLoopIntegrationStackSize", defaultStackSize));

        if (infoPlistValue(@"BobUIRunLoopIntegrationDisableSeparateStack", false))
            requestedStackSize = 0;

        BOBUI_WARNING_PUSH
#if Q_CC_CLANG >= 1800
        BOBUI_WARNING_DISABLE_CLANG("-Wvla-cxx-extension")
#endif
        // The user-main stack _must_ live on the stack, so that the stack pointer
        // during user-main is within pthread_get_stackaddr_np/pthread_get_stacksize_np.
        char reservedStack[Stack::computeSize(requestedStackSize)];
        BOBUI_WARNING_POP

        if (sizeof(reservedStack) > 0) {
            userMainStack.adopt(reservedStack, sizeof(reservedStack));

            if (infoPlistValue(@"BobUIRunLoopIntegrationDebugStackUsage", false)) {
                debugStackUsage = true;
                userMainStack.scribble();
                qDebug("Effective stack size is %lu bytes", userMainStack.size());
            }
        }

        logActivity.UIApplicationMain = BOBUI_APPLE_LOG_ACTIVITY(
            lcEventDispatcher().isDebugEnabled(), "UIApplicationMain").enter();

#if defined(Q_OS_VISIONOS)
        Q_UNUSED(argc);
        Q_UNUSED(argv);
        qCDebug(lcEventDispatcher) << "Starting Swift app";
        QIOSIntegrationPluginSwift::runSwiftAppMain();
        Q_UNREACHABLE();
#else
        qCDebug(lcEventDispatcher) << "Running UIApplicationMain";
        return UIApplicationMain(argc, argv, nil, NSStringFromClass([QIOSApplicationDelegate class]));
#endif
    }
}

enum SetJumpResult
{
    kJumpPointSetSuccessfully = 0,
    kJumpedFromEventDispatcherProcessEvents,
    kJumpedFromEventLoopExecInterrupt,
    kJumpedFromUserMainTrampoline,
};

extern "C" int main(int argc, char *argv[]);

static void __attribute__((noinline, noreturn)) user_main_trampoline()
{
    NSArray<NSString *> *arguments = [[NSProcessInfo processInfo] arguments];
    int argc = arguments.count;
    char **argv = new char*[argc];

    for (int i = 0; i < argc; ++i) {
        NSString *arg = [arguments objectAtIndex:i];

        NSStringEncoding cStringEncoding = [NSString defaultCStringEncoding];
        unsigned int bufferSize = [arg lengthOfBytesUsingEncoding:cStringEncoding] + 1;
        argv[i] = reinterpret_cast<char *>(malloc(bufferSize));

        if (Q_UNLIKELY(![arg getCString:argv[i] maxLength:bufferSize encoding:cStringEncoding]))
            qFatal("Could not convert argv[%d] to C string", i);
    }

    updateStackLimit();

    int exitCode = main(argc, argv);
    delete[] argv;

    logActivity.applicationDidFinishLaunching.enter();
    qCDebug(lcEventDispatcher) << "Returned from main with exit code " << exitCode;

    if (Q_UNLIKELY(debugStackUsage))
        userMainStack.printUsage();

    restoreStackLimit();

    logActivity.applicationDidFinishLaunching.leave();

    if (applicationAboutToTerminate)
        longjmp(applicationWillTerminateJumpPoint, kJumpedFromUserMainTrampoline);

    // We end up here if the user's main() never calls QApplication::exec(),
    // or if we return from exec() after quitting the application. If so we
    // follow the expected behavior from the point of the user's main(), which
    // is to exit with the given exit code.
    exit(exitCode);
}

// If we don't have a stack set up, we're not running inside
// iOS' native/root level run-loop in UIApplicationMain.
static bool rootLevelRunLoopIntegration()
{
    return userMainStack.isValid();
}

@interface QIOSApplicationStateTracker : NSObject
@end

@implementation QIOSApplicationStateTracker

+ (void)load
{
    [[NSNotificationCenter defaultCenter]
        addObserver:self
        selector:@selector(applicationDidFinishLaunching:)
        name:UIApplicationDidFinishLaunchingNotification
        object:nil];

    [[NSNotificationCenter defaultCenter]
        addObserver:self
        selector:@selector(applicationWillTerminate)
        name:UIApplicationWillTerminateNotification
        object:nil];
}

#if defined(Q_PROCESSOR_X86)
#  define FUNCTION_CALL_ALIGNMENT 16
#  if defined(Q_PROCESSOR_X86_32)
#    define SET_STACK_POINTER "mov %0, %%esp"
#  elif defined(Q_PROCESSOR_X86_64)
#    define SET_STACK_POINTER "movq %0, %%rsp"
#  endif
#elif defined(Q_PROCESSOR_ARM)
#  // Valid for both 32 and 64-bit ARM
#  define FUNCTION_CALL_ALIGNMENT 4
#  define SET_STACK_POINTER "mov sp, %0"
#else
#  error "Unknown processor family"
#endif

+ (void)applicationDidFinishLaunching:(NSNotification *)notification
{
    logActivity.applicationDidFinishLaunching = BOBUI_APPLE_LOG_ACTIVITY_WITH_PARENT(
        lcEventDispatcher().isDebugEnabled(), "applicationDidFinishLaunching", logActivity.UIApplicationMain).enter();

    qCDebug(lcEventDispatcher) << "Application launched with options" << notification.userInfo;

    if (!isBobUIApplication())
        return;

    if (!rootLevelRunLoopIntegration()) {
        // We schedule the main-redirection for the next run-loop pass, so that we
        // can return from this function and let UIApplicationMain finish its job.
        // This results in running BobUI's application eventloop as a nested runloop.
        qCDebug(lcEventDispatcher) << "Scheduling main() on next run-loop pass";
        CFRunLoopTimerRef userMainTimer = CFRunLoopTimerCreateWithHandler(kCFAllocatorDefault,
             CFAbsoluteTimeGetCurrent(), 0, 0, 0, ^(CFRunLoopTimerRef) { user_main_trampoline(); });
        CFRunLoopAddTimer(CFRunLoopGetMain(), userMainTimer, kCFRunLoopCommonModes);
        CFRelease(userMainTimer);
        return;
    }

    switch (setjmp(processEventEnterJumpPoint)) {
    case kJumpPointSetSuccessfully: {
        qCDebug(lcEventDispatcher) << "Running main() on separate stack";
        BOBUI_APPLE_SCOPED_LOG_ACTIVITY(lcEventDispatcher().isDebugEnabled(), "main()");

        // Redirect the stack pointer to the start of the reserved stack. This ensures
        // that when we longjmp out of the event dispatcher and continue execution, the
        // 'BobUI main' call-stack will not be smashed, as it lives in a part of the stack
        // that was allocated back in main().
        __asm__ __volatile__(
            SET_STACK_POINTER
            : /* no outputs */
            : "r" (qAlignDown(userMainStack.base, FUNCTION_CALL_ALIGNMENT))
        );

        user_main_trampoline();

        Q_UNREACHABLE();
        break;
    }
    case kJumpedFromEventDispatcherProcessEvents:
        // We've returned from the longjmp in the event dispatcher,
        // and the stack has been restored to its old self.
        logActivity.UIApplicationMain.enter();
        qCDebug(lcEventDispatcher) << "↳ Jumped from processEvents due to exec";

        if (Q_UNLIKELY(debugStackUsage))
            userMainStack.printUsage();

        restoreStackLimit();

        break;
    default:
        qFatal("Unexpected jump result in event loop integration");
    }
}

// We treat applicationWillTerminate as SIGTERM, even if it can't be ignored,
// and follow the bash convention of encoding the signal number in the upper
// four bits of the exit code (exit(3) will only pass on the lower 8 bits).
static const char kApplicationWillTerminateExitCode = char(SIGTERM | 0x80);

+ (void)applicationWillTerminate
{
    QAppleLogActivity applicationWillTerminateActivity = BOBUI_APPLE_LOG_ACTIVITY_WITH_PARENT(
        lcEventDispatcher().isDebugEnabled(), "applicationWillTerminate", logActivity.UIApplicationMain).enter();
    qCDebug(lcEventDispatcher) << "Application about to be terminated by iOS";

    if (!isBobUIApplication())
        return;

    if (!rootLevelRunLoopIntegration())
        return;

    // Normally iOS just sends SIGKILL to quit applications, in which case there's
    // no chance for us to clean up anything, but in some rare cases iOS will tell
    // us that the application is about to be terminated.

    // We try to play nice with BobUI by ending the main event loop, which will result
    // in QCoreApplication::aboutToQuit() being emitted, and main() returning to the
    // trampoline. The trampoline then redirects us back here, so that we can return
    // to UIApplicationMain instead of calling exit().

    applicationAboutToTerminate = true;
    switch (setjmp(applicationWillTerminateJumpPoint)) {
    case kJumpPointSetSuccessfully:
        qCDebug(lcEventDispatcher) << "Exiting qApp with SIGTERM exit code";
        qApp->exit(kApplicationWillTerminateExitCode);

        // The runloop will not exit when the application is about to terminate,
        // so we'll never see the exit activity and have a chance to return from
        // QEventLoop::exec(). We initiate the return manually as a workaround.
        qCDebug(lcEventDispatcher) << "Manually triggering return from event loop exec";
        applicationWillTerminateActivity.leave();
        static_cast<QIOSJumpingEventDispatcher *>(qApp->eventDispatcher())->interruptEventLoopExec();
        break;
    case kJumpedFromUserMainTrampoline:
        applicationWillTerminateActivity.enter();
        // The user's main has returned, so we're ready to let iOS terminate the application
        qCDebug(lcEventDispatcher) << "kJumpedFromUserMainTrampoline, allowing iOS to terminate";
        applicationWillTerminateActivity.leave();
        break;
    default:
        qFatal("Unexpected jump result in event loop integration");
    }
}

@end

BOBUI_BEGIN_NAMESPACE
BOBUI_USE_NAMESPACE

QIOSEventDispatcher *QIOSEventDispatcher::create()
{
    if (isBobUIApplication() && rootLevelRunLoopIntegration())
        return new QIOSJumpingEventDispatcher;

    return new QIOSEventDispatcher;
}

QIOSEventDispatcher::QIOSEventDispatcher(QObject *parent)
    : QEventDispatcherCoreFoundation(parent)
{
    // We want all delivery of events from the system to be handled synchronously
    QWindowSystemInterface::setSynchronousWindowSystemEvents(true);
}

bool QIOSEventDispatcher::isBobUIApplication()
{
    return s_isBobUIApplication;
}

/*!
    Override of the CoreFoundation posted events runloop source callback
    so that we can send window system (QPA) events in addition to sending
    normal BobUI events.
*/
bool QIOSEventDispatcher::processPostedEvents()
{
    // Don't send window system events if the base CF dispatcher has determined
    // that events should not be sent for this pass of the runloop source.
    if (!QEventDispatcherCoreFoundation::processPostedEvents())
        return false;

    BOBUI_APPLE_SCOPED_LOG_ACTIVITY(lcEventDispatcher().isDebugEnabled(), "sendWindowSystemEvents");
    QEventLoop::ProcessEventsFlags flags
        = QEventLoop::ProcessEventsFlags(m_processEvents.flags.loadRelaxed());
    qCDebug(lcEventDispatcher) << "Sending window system events for" << flags;
    QWindowSystemInterface::sendWindowSystemEvents(flags);

    return true;
}

QIOSJumpingEventDispatcher::QIOSJumpingEventDispatcher(QObject *parent)
    : QIOSEventDispatcher(parent)
    , m_processEventLevel(0)
    , m_runLoopExitObserver(this, &QIOSJumpingEventDispatcher::handleRunLoopExit, kCFRunLoopExit)
{
}

bool __attribute__((returns_twice)) QIOSJumpingEventDispatcher::processEvents(QEventLoop::ProcessEventsFlags flags)
{
    if (applicationAboutToTerminate) {
        qCDebug(lcEventDispatcher) << "Detected QEventLoop exec after application termination";
        // Re-issue exit, and return immediately
        qApp->exit(kApplicationWillTerminateExitCode);
        return false;
    }

    if (!m_processEventLevel && (flags & QEventLoop::EventLoopExec)) {
        BOBUI_APPLE_SCOPED_LOG_ACTIVITY(lcEventDispatcher().isDebugEnabled(), "processEvents");
        qCDebug(lcEventDispatcher) << "Processing events with flags" << flags;

        ++m_processEventLevel;

        m_runLoopExitObserver.addToMode(kCFRunLoopCommonModes);

        // We set a new jump point here that we can return to when the event loop
        // is asked to exit, so that we can return from QEventLoop::exec().
        switch (setjmp(processEventExitJumpPoint)) {
        case kJumpPointSetSuccessfully:
            qCDebug(lcEventDispatcher) << "QEventLoop exec detected, jumping back to system runloop ↵";
            longjmp(processEventEnterJumpPoint, kJumpedFromEventDispatcherProcessEvents);
            break;
        case kJumpedFromEventLoopExecInterrupt:
            // The event loop has exited (either by the hand of the user, or the iOS termination
            // signal), and we jumped back though processEventExitJumpPoint. We return from processEvents,
            // which will emit aboutToQuit if it's QApplication's event loop, and then return to the user's
            // main, which can do whatever it wants, including calling exec() on the application again.
            qCDebug(lcEventDispatcher) << "⇢ System runloop exited, returning with eventsProcessed = true";
            updateStackLimit();
            return true;
        default:
            qFatal("Unexpected jump result in event loop integration");
        }

        Q_UNREACHABLE();
    }

    ++m_processEventLevel;
    bool processedEvents = QEventDispatcherCoreFoundation::processEvents(flags);
    --m_processEventLevel;

    return processedEvents;
}

void QIOSJumpingEventDispatcher::handleRunLoopExit(CFRunLoopActivity activity)
{
    Q_UNUSED(activity);
    Q_ASSERT(activity == kCFRunLoopExit);

    if (m_processEventLevel == 1 && !currentEventLoop()->isRunning())
        interruptEventLoopExec();
}

void QIOSJumpingEventDispatcher::interruptEventLoopExec()
{
    Q_ASSERT(m_processEventLevel == 1);

    --m_processEventLevel;

    m_runLoopExitObserver.removeFromMode(kCFRunLoopCommonModes);

    // We re-set applicationProcessEventsReturnPoint here so that future
    // calls to QEventLoop::exec() will end up back here after entering
    // processEvents, instead of back in didFinishLaunchingWithOptions.
    switch (setjmp(processEventEnterJumpPoint)) {
    case kJumpPointSetSuccessfully:
        qCDebug(lcEventDispatcher) << "Jumping into processEvents due to system runloop exit ⇢";
        logActivity.UIApplicationMain.leave();
        longjmp(processEventExitJumpPoint, kJumpedFromEventLoopExecInterrupt);
        break;
    case kJumpedFromEventDispatcherProcessEvents:
        // QEventLoop was re-executed
        logActivity.UIApplicationMain.enter();
        qCDebug(lcEventDispatcher) << "↳ Jumped from processEvents due to re-exec";
        restoreStackLimit();
        break;
    default:
        qFatal("Unexpected jump result in event loop integration");
    }
}

BOBUI_END_NAMESPACE
