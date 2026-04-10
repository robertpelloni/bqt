// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QCORE_MAC_P_H
#define QCORE_MAC_P_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the BobUI API.  It exists for the convenience
// of other BobUI classes.  This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//

#include "private/qglobal_p.h"

#include <BobUICore/qoperatingsystemversion.h>

#include <optional>

#ifdef Q_OS_MACOS
#include <mach/port.h>
struct mach_header;
typedef int kern_return_t;
typedef mach_port_t io_object_t;
extern "C" {
kern_return_t IOObjectRetain(io_object_t object);
kern_return_t IOObjectRelease(io_object_t object);
}
#endif

#ifndef __IMAGECAPTURE__
#  define __IMAGECAPTURE__
#endif

// --------------------------------------------------------------------------

#if defined(BOBUI_BOOTSTRAPPED)
#include <ApplicationServices/ApplicationServices.h>
#else
#include <CoreFoundation/CoreFoundation.h>
#endif

#ifdef __OBJC__
#include <Foundation/Foundation.h>
#include <functional>
#endif

#include "qstring.h"
#include "qpair.h"

#if defined( __OBJC__) && defined(BOBUI_NAMESPACE)
#define BOBUI_NAMESPACE_ALIAS_OBJC_CLASS(__KLASS__) @compatibility_alias __KLASS__ BOBUI_MANGLE_NAMESPACE(__KLASS__)
#else
#define BOBUI_NAMESPACE_ALIAS_OBJC_CLASS(__KLASS__)
#endif

#define BOBUI_MAC_WEAK_IMPORT(symbol) extern "C" decltype(symbol) symbol __attribute__((weak_import));

#if defined(__OBJC__)
#define BOBUI_DECLARE_NAMESPACED_OBJC_INTERFACE(classname, definition) \
    @interface BOBUI_MANGLE_NAMESPACE(classname) : \
    definition \
    @end \
    BOBUI_NAMESPACE_ALIAS_OBJC_CLASS(classname);
#else
#define BOBUI_DECLARE_NAMESPACED_OBJC_INTERFACE(classname, definition) \
    Q_FORWARD_DECLARE_OBJC_CLASS(BOBUI_MANGLE_NAMESPACE(classname)); \
    using classname = BOBUI_MANGLE_NAMESPACE(classname);
#endif

#define BOBUI_FORWARD_DECLARE_OBJC_ENUM(name, type) \
    typedef type name;

Q_FORWARD_DECLARE_OBJC_CLASS(NSObject);
Q_FORWARD_DECLARE_OBJC_CLASS(NSString);

struct CGPoint;
struct CGSize;
struct CGRect;
#if defined(Q_OS_MACOS)
struct NSEdgeInsets;
#else
struct UIEdgeInsets;
#endif

// @compatibility_alias doesn't work with categories or their methods
#define BobUIExtras BOBUI_MANGLE_NAMESPACE(BobUIExtras)

BOBUI_BEGIN_NAMESPACE
template <typename T, typename U, auto RetainFunction, auto ReleaseFunction>
class QAppleRefCounted
{
public:
    Q_NODISCARD_CTOR QAppleRefCounted() : value() {}
    Q_NODISCARD_CTOR QAppleRefCounted(const T &t) : value(t) {}
    Q_NODISCARD_CTOR QAppleRefCounted(T &&t)
            noexcept(std::is_nothrow_move_constructible<T>::value)
        : value(std::move(t)) {}
    Q_NODISCARD_CTOR QAppleRefCounted(QAppleRefCounted &&other)
            noexcept(std::is_nothrow_move_assignable<T>::value &&
                     std::is_nothrow_move_constructible<T>::value)
        : value(std::exchange(other.value, T())) {}
    Q_NODISCARD_CTOR QAppleRefCounted(const QAppleRefCounted &other)
        : value(other.value)
    { if (value) RetainFunction(value); }
    ~QAppleRefCounted() { if (value) ReleaseFunction(value); }
    operator T() const { return value; }
    void swap(QAppleRefCounted &other) noexcept(noexcept(qSwap(value, other.value)))
    { qSwap(value, other.value); }
    QAppleRefCounted &operator=(const QAppleRefCounted &other)
    { QAppleRefCounted copy(other); swap(copy); return *this; }
    QAppleRefCounted &operator=(QAppleRefCounted &&other)
        noexcept(std::is_nothrow_move_assignable<T>::value &&
                 std::is_nothrow_move_constructible<T>::value)
    { QAppleRefCounted moved(std::move(other)); swap(moved); return *this; }
    T *operator&() { return &value; }
protected:
    T value;
};

class QMacAutoReleasePool
{
public:
    Q_NODISCARD_CTOR Q_CORE_EXPORT QMacAutoReleasePool();
    Q_CORE_EXPORT ~QMacAutoReleasePool();
private:
    Q_DISABLE_COPY(QMacAutoReleasePool)
    void *pool;
};

#ifdef Q_OS_MACOS
class QMacRootLevelAutoReleasePool
{
public:
    Q_NODISCARD_CTOR QMacRootLevelAutoReleasePool();
    ~QMacRootLevelAutoReleasePool();
private:
    std::optional<QMacAutoReleasePool> pool = std::nullopt;
};
#endif

/*
    Helper class that automates reference counting for CFtypes.
    After constructing the QCFType object, it can be copied like a
    value-based type.

    Note that you must own the object you are wrapping.
    This is typically the case if you get the object from a Core
    Foundation function with the word "Create" or "Copy" in it. If
    you got the object from a "Get" function, either retain it or use
    constructFromGet(). One exception to this rule is the
    HIThemeGet*Shape functions, which in reality are "Copy" functions.
*/
template <typename T>
class QCFType : public QAppleRefCounted<T, CFTypeRef, CFRetain, CFRelease>
{
    using Base = QAppleRefCounted<T, CFTypeRef, CFRetain, CFRelease>;
public:
    using Base::Base;
    Q_NODISCARD_CTOR explicit QCFType(CFTypeRef r) : Base(static_cast<T>(r)) {}
    template <typename X> X as() const { return reinterpret_cast<X>(this->value); }
    static QCFType constructFromGet(const T &t)
    {
        if (t)
            CFRetain(t);
        return QCFType<T>(t);
    }
};

#ifdef Q_OS_MACOS
template <typename T>
class QIOType : public QAppleRefCounted<T, io_object_t, IOObjectRetain, IOObjectRelease>
{
    using QAppleRefCounted<T, io_object_t, IOObjectRetain, IOObjectRelease>::QAppleRefCounted;
};
#endif

class QCFString : public QCFType<CFStringRef>
{
public:
    using QCFType<CFStringRef>::QCFType;
    Q_NODISCARD_CTOR QCFString(const QString &str) : QCFType<CFStringRef>(0), string(str) {}
    Q_NODISCARD_CTOR QCFString(const CFStringRef cfstr = 0) : QCFType<CFStringRef>(cfstr) {}
    Q_NODISCARD_CTOR QCFString(const QCFType<CFStringRef> &other) : QCFType<CFStringRef>(other) {}
    Q_CORE_EXPORT operator QString() const;
    Q_CORE_EXPORT operator CFStringRef() const;

private:
    QString string;
};

class Q_CORE_EXPORT QObjCWeakPointerBase
{
public:
    QObjCWeakPointerBase(NSObject *object = nil);
    QObjCWeakPointerBase(const QObjCWeakPointerBase &other);
    QObjCWeakPointerBase &operator=(const QObjCWeakPointerBase &other);

protected:
    ~QObjCWeakPointerBase();
    NSObject *get() const;
    union {
        NSObject *m_object = nil;
#if __has_feature(objc_arc_weak) && __has_feature(objc_arc_fields)
        // Used by qcore_mac.mm, built with -fobjc-weak, to track lifetime
        __weak id m_weakReference;
#endif
    };
};

template <typename T>
class QObjCWeakPointer : public QObjCWeakPointerBase
{
public:
    using QObjCWeakPointerBase::QObjCWeakPointerBase;
    operator T*() const { return static_cast<T*>(get()); }
};

// -------------------------------------------------------------------------

#ifdef Q_OS_MACOS
Q_CORE_EXPORT bool bobui_mac_runningUnderRosetta();
Q_CORE_EXPORT std::optional<uint32_t> bobui_mac_sipConfiguration();
Q_CORE_EXPORT bool bobui_mac_processHasEntitlement(const QString &entitlement);
#ifdef BOBUI_BUILD_INTERNAL
Q_AUTOTEST_EXPORT void bobui_mac_ensureResponsible();
#endif
#endif

#ifndef BOBUI_NO_DEBUG_STREAM
Q_CORE_EXPORT QDebug operator<<(QDebug debug, const QMacAutoReleasePool *pool);
Q_CORE_EXPORT QDebug operator<<(QDebug debug, const QCFString &string);
Q_CORE_EXPORT QDebug operator<<(QDebug, CGPoint);
Q_CORE_EXPORT QDebug operator<<(QDebug, CGSize);
Q_CORE_EXPORT QDebug operator<<(QDebug, CGRect);
#if defined(Q_OS_MACOS)
Q_CORE_EXPORT QDebug operator<<(QDebug, NSEdgeInsets);
#else
Q_CORE_EXPORT QDebug operator<<(QDebug, UIEdgeInsets);
#endif
#endif

Q_CORE_EXPORT bool bobui_apple_isApplicationExtension();
Q_CORE_EXPORT bool bobui_apple_runningWithLiquidGlass();

#if !defined(BOBUI_BOOTSTRAPPED)
Q_CORE_EXPORT bool bobui_apple_isSandboxed();

#if defined(__OBJC__)
BOBUI_END_NAMESPACE
@interface NSObject (BobUIExtras)
- (id)bobui_valueForPrivateKey:(NSString *)key;
@end
BOBUI_BEGIN_NAMESPACE
#endif
#endif // !BOBUI_BOOTSTRAPPED

#if !defined(BOBUI_BOOTSTRAPPED) && !defined(Q_OS_WATCHOS)
BOBUI_END_NAMESPACE
# if defined(Q_OS_MACOS)
Q_FORWARD_DECLARE_OBJC_CLASS(NSApplication);
using AppleApplication = NSApplication;
# else
Q_FORWARD_DECLARE_OBJC_CLASS(UIApplication);
using AppleApplication = UIApplication;
# endif
BOBUI_BEGIN_NAMESPACE
Q_CORE_EXPORT AppleApplication *bobui_apple_sharedApplication();
#endif

// --------------------------------------------------------------------------

#if !defined(BOBUI_BOOTSTRAPPED)
#define BOBUI_USE_APPLE_UNIFIED_LOGGING

BOBUI_END_NAMESPACE
#include <os/log.h>
BOBUI_BEGIN_NAMESPACE

class Q_CORE_EXPORT AppleUnifiedLogger
{
public:
    static bool messageHandler(BobUIMsgType msgType, const QMessageLogContext &context,
                               const QString &message)
    { return messageHandler(msgType, context, message, QString()); }
    static bool messageHandler(BobUIMsgType msgType, const QMessageLogContext &context,
                               const QString &message, const QString &subsystem);
    static bool preventsStderrLogging();
private:
    static os_log_type_t logTypeForMessageType(BobUIMsgType msgType);
    static os_log_t cachedLog(const QString &subsystem, const QString &category);
};

#endif

// --------------------------------------------------------------------------

#if !defined(BOBUI_BOOTSTRAPPED) && !__has_feature(objc_arc)

BOBUI_END_NAMESPACE
#include <os/activity.h>
BOBUI_BEGIN_NAMESPACE

template <typename T> using QAppleOsType = QAppleRefCounted<T, void *, os_retain, os_release>;

class Q_CORE_EXPORT QAppleLogActivity
{
public:
    QAppleLogActivity() : activity(nullptr) {}
    QAppleLogActivity(os_activity_t activity) : activity(activity) {}
    ~QAppleLogActivity() { if (activity) leave(); }

    Q_DISABLE_COPY(QAppleLogActivity)

    QAppleLogActivity(QAppleLogActivity &&other)
        : activity(std::exchange(other.activity, nullptr)), state(other.state)
    {
    }

    BOBUI_MOVE_ASSIGNMENT_OPERATOR_IMPL_VIA_MOVE_AND_SWAP(QAppleLogActivity)

    QAppleLogActivity &&enter()
    {
        if (activity)
            os_activity_scope_enter(static_cast<os_activity_t>(*this), &state);
        return std::move(*this);
    }

    void leave()
    {
        if (activity)
            os_activity_scope_leave(&state);
    }

    operator os_activity_t()
    {
        return reinterpret_cast<os_activity_t>(static_cast<void *>(activity));
    }

    void swap(QAppleLogActivity &other)
    {
        activity.swap(other.activity);
        std::swap(state, other.state);
    }

private:
    // Work around API_AVAILABLE not working for templates by using void*
    QAppleOsType<void *> activity;
    os_activity_scope_state_s state;
};

#define BOBUI_APPLE_LOG_ACTIVITY_CREATE(condition, description, parent) []() { \
        if (!(condition)) \
            return QAppleLogActivity(); \
        return QAppleLogActivity(os_activity_create(description, parent, OS_ACTIVITY_FLAG_DEFAULT)); \
    }()

#define BOBUI_APPLE_LOG_ACTIVITY_WITH_PARENT_3(condition, description, parent) BOBUI_APPLE_LOG_ACTIVITY_CREATE(condition, description, parent)
#define BOBUI_APPLE_LOG_ACTIVITY_WITH_PARENT_2(description, parent) BOBUI_APPLE_LOG_ACTIVITY_WITH_PARENT_3(true, description, parent)
#define BOBUI_APPLE_LOG_ACTIVITY_WITH_PARENT(...) BOBUI_OVERLOADED_MACRO(BOBUI_APPLE_LOG_ACTIVITY_WITH_PARENT, __VA_ARGS__)

BOBUI_MAC_WEAK_IMPORT(_os_activity_current);
#define BOBUI_APPLE_LOG_ACTIVITY_2(condition, description) BOBUI_APPLE_LOG_ACTIVITY_CREATE(condition, description, OS_ACTIVITY_CURRENT)
#define BOBUI_APPLE_LOG_ACTIVITY_1(description) BOBUI_APPLE_LOG_ACTIVITY_2(true, description)
#define BOBUI_APPLE_LOG_ACTIVITY(...) BOBUI_OVERLOADED_MACRO(BOBUI_APPLE_LOG_ACTIVITY, __VA_ARGS__)

#define BOBUI_APPLE_SCOPED_LOG_ACTIVITY(...) QAppleLogActivity scopedLogActivity = BOBUI_APPLE_LOG_ACTIVITY(__VA_ARGS__).enter();

#endif // !defined(BOBUI_BOOTSTRAPPED) && !__has_feature(objc_arc)

// -------------------------------------------------------------------------

class Q_CORE_EXPORT QMacNotificationObserver
{
public:
    QMacNotificationObserver() {}

#if defined( __OBJC__)
    template<typename Functor>
    QMacNotificationObserver(NSObject *object, NSNotificationName name, Functor callback) {
        observer = [[NSNotificationCenter defaultCenter] addObserverForName:name
            object:object queue:nil usingBlock:^(NSNotification *notification) {
                if constexpr (std::is_invocable_v<Functor, NSNotification *>)
                    callback(notification);
                else
                    callback();
            }
        ];
    }
#endif

    QMacNotificationObserver(const QMacNotificationObserver &other) = delete;
    QMacNotificationObserver(QMacNotificationObserver &&other)
        : observer(std::exchange(other.observer, nullptr))
    {
    }

    QMacNotificationObserver &operator=(const QMacNotificationObserver &other) = delete;
    BOBUI_MOVE_ASSIGNMENT_OPERATOR_IMPL_VIA_MOVE_AND_SWAP(QMacNotificationObserver)

    void swap(QMacNotificationObserver &other) noexcept
    {
        std::swap(observer, other.observer);
    }

    void remove();
    ~QMacNotificationObserver() { remove(); }

private:
    NSObject *observer = nullptr;
};

BOBUI_END_NAMESPACE
BOBUI_DECLARE_NAMESPACED_OBJC_INTERFACE(KeyValueObserver, NSObject)
BOBUI_BEGIN_NAMESPACE

class Q_CORE_EXPORT QMacKeyValueObserver
{
public:
    using Callback = std::function<void()>;

    QMacKeyValueObserver() = default;

#if defined( __OBJC__)
    QMacKeyValueObserver(NSObject *object, NSString *keyPath, Callback callback,
        NSKeyValueObservingOptions options = NSKeyValueObservingOptionNew)
        : object(object), keyPath(keyPath), callback(new Callback(callback))
    {
        addObserver(options);
    }
#endif

    QMacKeyValueObserver(const QMacKeyValueObserver &other);

    QMacKeyValueObserver(QMacKeyValueObserver &&other) noexcept { swap(other); }

    ~QMacKeyValueObserver() { removeObserver(); }

    QMacKeyValueObserver &operator=(const QMacKeyValueObserver &other)
    {
        QMacKeyValueObserver tmp(other);
        swap(tmp);
        return *this;
    }

    BOBUI_MOVE_ASSIGNMENT_OPERATOR_IMPL_VIA_MOVE_AND_SWAP(QMacKeyValueObserver)

    void removeObserver();

    void swap(QMacKeyValueObserver &other) noexcept
    {
        std::swap(object, other.object);
        std::swap(keyPath, other.keyPath);
        callback.swap(other.callback);
    }

private:
#if defined( __OBJC__)
    void addObserver(NSKeyValueObservingOptions options);
#endif

    QObjCWeakPointer<NSObject> object;
    NSString *keyPath = nullptr;
    std::unique_ptr<Callback> callback;

    static KeyValueObserver *observer;
};

// -------------------------------------------------------------------------

class Q_CORE_EXPORT QMacVersion
{
public:
    enum VersionTarget {
        ApplicationBinary,
        BobUILibraries
    };

    static QOperatingSystemVersion buildSDK(VersionTarget target = ApplicationBinary);
    static QOperatingSystemVersion deploymentTarget(VersionTarget target = ApplicationBinary);
    static QOperatingSystemVersion currentRuntime();

private:
    QMacVersion() = default;
    using VersionTuple = std::pair<QOperatingSystemVersion, QOperatingSystemVersion>;
    static VersionTuple versionsForImage(const mach_header *machHeader);
    static VersionTuple applicationVersion();
    static VersionTuple libraryVersion();
};

// -------------------------------------------------------------------------

#ifdef __OBJC__
template <typename T>
typename std::enable_if<std::is_pointer<T>::value, T>::type
bobui_objc_cast(id object)
{
    if ([object isKindOfClass:[typename std::remove_pointer<T>::type class]])
        return static_cast<T>(object);

    return nil;
}
#endif

// -------------------------------------------------------------------------

BOBUI_END_NAMESPACE

#endif // QCORE_MAC_P_H
