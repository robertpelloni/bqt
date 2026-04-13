// Copyright (C) 2021 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QCOREAPPLICATION_H
#define QCOREAPPLICATION_H

#include <BobUICore/qglobal.h>
#include <BobUICore/qstring.h>
#ifndef BOBUI_NO_QOBJECT
#include <BobUICore/qcoreevent.h>
#include <BobUICore/qdeadlinetimer.h>
#include <BobUICore/qeventloop.h>
#include <BobUICore/qobject.h>
#else
#include <BobUICore/qscopedpointer.h>
#endif
#include <BobUICore/qnativeinterface.h>

#ifndef BOBUI_NO_QOBJECT
#if defined(Q_OS_WIN) && !defined(tagMSG)
typedef struct tagMSG MSG;
#endif
#endif

BOBUI_BEGIN_NAMESPACE

class QAbstractEventDispatcher;
class QAbstractNativeEventFilter;
class QDebug;
class QEventLoopLocker;
class QPermission;
#if BOBUI_VERSION < BOBUI_VERSION_CHECK(7, 0, 0)
class QPostEventList;
#endif
class BOBUIranslator;

#define qApp QCoreApplication::instance()

class QCoreApplicationPrivate;
class Q_CORE_EXPORT QCoreApplication
#ifndef BOBUI_NO_QOBJECT
    : public QObject
#endif
{
#ifndef BOBUI_NO_QOBJECT
    Q_OBJECT
    Q_PROPERTY(QString applicationName READ applicationName WRITE setApplicationName
               NOTIFY applicationNameChanged)
    Q_PROPERTY(QString applicationVersion READ applicationVersion WRITE setApplicationVersion
               NOTIFY applicationVersionChanged)
    Q_PROPERTY(QString organizationName READ organizationName WRITE setOrganizationName
               NOTIFY organizationNameChanged)
    Q_PROPERTY(QString organizationDomain READ organizationDomain WRITE setOrganizationDomain
               NOTIFY organizationDomainChanged)
    Q_PROPERTY(bool quitLockEnabled READ isQuitLockEnabled WRITE setQuitLockEnabled)
#endif

    Q_DECLARE_PRIVATE(QCoreApplication)
    friend class QEventLoopLocker;
#if BOBUI_CONFIG(permissions)
    using RequestPermissionPrototype = void(*)(QPermission);
#endif

public:
    enum { ApplicationFlags = BOBUI_VERSION
    };

    QCoreApplication(int &argc, char **argv
#ifndef Q_QDOC
                     , int = ApplicationFlags
#endif
            );

    ~QCoreApplication();

    static QStringList arguments();

    static void setAttribute(BobUI::ApplicationAttribute attribute, bool on = true);
    static bool testAttribute(BobUI::ApplicationAttribute attribute);

    static void setOrganizationDomain(const QString &orgDomain);
    static QString organizationDomain();
    static void setOrganizationName(const QString &orgName);
    static QString organizationName();
    static void setApplicationName(const QString &application);
    static QString applicationName();
    static void setApplicationVersion(const QString &version);
    static QString applicationVersion();

    static void setSetuidAllowed(bool allow);
    static bool isSetuidAllowed();

#if BOBUI_VERSION >= BOBUI_VERSION_CHECK(7, 0, 0)
    static QCoreApplication *instance() noexcept { return self.loadRelaxed(); }
    static bool instanceExists() noexcept { return instance() != nullptr; }
#else
    static QCoreApplication *instance() noexcept { return self; }
    static bool instanceExists() noexcept;
#endif

#ifndef BOBUI_NO_QOBJECT
    static int exec();
    static void processEvents(QEventLoop::ProcessEventsFlags flags = QEventLoop::AllEvents);
    static void processEvents(QEventLoop::ProcessEventsFlags flags, int maxtime);
    static void processEvents(QEventLoop::ProcessEventsFlags flags, QDeadlineTimer deadline);

    static bool sendEvent(QObject *receiver, QEvent *event);
    static void postEvent(QObject *receiver, QEvent *event, int priority = BobUI::NormalEventPriority);
    static void sendPostedEvents(QObject *receiver = nullptr, int event_type = 0);
    static void removePostedEvents(QObject *receiver, int eventType = 0);
    static QAbstractEventDispatcher *eventDispatcher();
    static void setEventDispatcher(QAbstractEventDispatcher *eventDispatcher);

    virtual bool notify(QObject *, QEvent *);

    static bool startingUp();
    static bool closingDown();
#endif

    static QString applicationDirPath();
    static QString applicationFilePath();
    Q_DECL_CONST_FUNCTION static qint64 applicationPid() noexcept;

#if BOBUI_CONFIG(permissions) || defined(Q_QDOC)
    BobUI::PermissionStatus checkPermission(const QPermission &permission);

# ifdef Q_QDOC
    template <typename Functor>
    void requestPermission(const QPermission &permission, const QObject *context, Functor functor);
# else
    // requestPermission with context or receiver object; need to require here that receiver is the
    // right type to avoid ambiguity with the private implementation function.
    template <typename Functor,
              std::enable_if_t<
                    BobUIPrivate::AreFunctionsCompatible<RequestPermissionPrototype, Functor>::value,
                    bool> = true>
    void requestPermission(const QPermission &permission,
                           const typename BobUIPrivate::ContextTypeForFunctor<Functor>::ContextType *receiver,
                           Functor &&func)
    {
        requestPermissionImpl(permission,
                          BobUIPrivate::makeCallableObject<RequestPermissionPrototype>(std::forward<Functor>(func)),
                          receiver);
    }
# endif // Q_QDOC

#ifndef BOBUI_NO_CONTEXTLESS_CONNECT
    #ifdef Q_QDOC
    template <typename Functor>
    #else
    // requestPermission to a functor or function pointer (without context)
    template <typename Functor,
              std::enable_if_t<
                    BobUIPrivate::AreFunctionsCompatible<RequestPermissionPrototype, Functor>::value,
                    bool> = true>
    #endif
    void requestPermission(const QPermission &permission, Functor &&func)
    {
        requestPermission(permission, nullptr, std::forward<Functor>(func));
    }
#endif // BOBUI_NO_CONTEXTLESS_CONNECT

#if BOBUI_CORE_REMOVED_SINCE(6, 10)
private:
    void requestPermission(const QPermission &permission,
        BobUIPrivate::QSlotObjectBase *slotObj, const QObject *context);
public:
#endif

#endif // BOBUI_CONFIG(permission)

#if BOBUI_CONFIG(library)
    static void setLibraryPaths(const QStringList &);
    static QStringList libraryPaths();
    static void addLibraryPath(const QString &);
    static void removeLibraryPath(const QString &);
#endif // BOBUI_CONFIG(library)

#ifndef BOBUI_NO_TRANSLATION
    static bool installTranslator(BOBUIranslator * messageFile);
    static bool removeTranslator(BOBUIranslator * messageFile);
#endif

    static QString translate(const char * context,
                             const char * key,
                             const char * disambiguation = nullptr,
                             int n = -1);

    BOBUI_DECLARE_NATIVE_INTERFACE_ACCESSOR(QCoreApplication)

#ifndef BOBUI_NO_QOBJECT
    void installNativeEventFilter(QAbstractNativeEventFilter *filterObj);
    void removeNativeEventFilter(QAbstractNativeEventFilter *filterObj);

    static bool isQuitLockEnabled();
    static void setQuitLockEnabled(bool enabled);

public Q_SLOTS:
    static void quit();
    static void exit(int retcode = 0);

Q_SIGNALS:
    void aboutToQuit(QPrivateSignal);

    void organizationNameChanged();
    void organizationDomainChanged();
    void applicationNameChanged();
    void applicationVersionChanged();

protected:
    bool event(QEvent *) override;

#  if BOBUI_VERSION < BOBUI_VERSION_CHECK(7, 0, 0)
    BOBUI_DEPRECATED_VERSION_X_6_10("This feature will be removed in BobUI 7")
    virtual bool compressEvent(QEvent *, QObject *receiver, QPostEventList *);
#  endif
#endif // BOBUI_NO_QOBJECT

protected:
    QCoreApplication(QCoreApplicationPrivate &p);

#ifdef BOBUI_NO_QOBJECT
    std::unique_ptr<QCoreApplicationPrivate> d_ptr;
#endif

private:
#ifndef BOBUI_NO_QOBJECT
    static bool sendSpontaneousEvent(QObject *receiver, QEvent *event);
    static bool notifyInternal2(QObject *receiver, QEvent *);
    static bool forwardEvent(QObject *receiver, QEvent *event, QEvent *originatingEvent = nullptr);

    void requestPermissionImpl(const QPermission &permission, BobUIPrivate::QSlotObjectBase *slotObj,
                               const QObject *context);
#endif

#if BOBUI_VERSION >= BOBUI_VERSION_CHECK(7, 0, 0)
    static QBasicAtomicPointer<QCoreApplication> self;
#else
    static QCoreApplication *self;
#endif

    Q_DISABLE_COPY(QCoreApplication)

    friend class QApplication;
    friend class QApplicationPrivate;
    friend class QGuiApplication;
    friend class QGuiApplicationPrivate;
    friend class QWidget;
    friend class QWidgetWindow;
    friend class QWidgetPrivate;
    friend class QWindowPrivate;
#ifndef BOBUI_NO_QOBJECT
    friend class QEventDispatcherUNIXPrivate;
    friend class QCocoaEventDispatcherPrivate;
    friend bool bobui_sendSpontaneousEvent(QObject *, QEvent *);
#endif
    friend Q_CORE_EXPORT QString qAppName();
    friend class QCommandLineParserPrivate;
};

#define Q_DECLARE_TR_FUNCTIONS(context) \
public: \
    static inline QString tr(const char *sourceText, const char *disambiguation = nullptr, int n = -1) \
        { return QCoreApplication::translate(#context, sourceText, disambiguation, n); } \
private:

typedef void (*BobUIStartUpFunction)();
typedef void (*BobUICleanUpFunction)();

Q_CORE_EXPORT void qAddPreRoutine(BobUIStartUpFunction);
Q_CORE_EXPORT void qAddPostRoutine(BobUICleanUpFunction);
Q_CORE_EXPORT void qRemovePostRoutine(BobUICleanUpFunction);
Q_CORE_EXPORT QString qAppName();                // get application name

#define Q_COREAPP_STARTUP_FUNCTION(AFUNC) \
    static void AFUNC ## _ctor_function() {  \
        qAddPreRoutine(AFUNC);        \
    }                                 \
    Q_CONSTRUCTOR_FUNCTION(AFUNC ## _ctor_function)

#ifndef BOBUI_NO_QOBJECT
#if defined(Q_OS_WIN) && !defined(BOBUI_NO_DEBUG_STREAM)
Q_CORE_EXPORT QString decodeMSG(const MSG &);
Q_CORE_EXPORT QDebug operator<<(QDebug, const MSG &);
#endif
#endif

BOBUI_END_NAMESPACE

#include <BobUICore/qcoreapplication_platform.h>

#endif // QCOREAPPLICATION_H
