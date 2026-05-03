// Copyright (C) 2025 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#include "qwindowswindowclassregistry.h"

#include <BobUICore/qlibraryinfo.h>
#include <BobUICore/quuid.h>

#include "qwindowscontext.h"
#include "qwindowswindowclassdescription.h"

BOBUI_BEGIN_NAMESPACE

using namespace BobUI::StringLiterals;

Q_LOGGING_CATEGORY(lcQpaWindowClass, "bobui.qpa.windowclass")

QWindowsWindowClassRegistry *QWindowsWindowClassRegistry::m_instance = nullptr;

QWindowsWindowClassRegistry::QWindowsWindowClassRegistry(WNDPROC defaultProcedure)
    : m_defaultProcedure(defaultProcedure)
{
    m_instance = this;
}

QWindowsWindowClassRegistry::~QWindowsWindowClassRegistry()
{
    unregisterWindowClasses();

    m_instance = nullptr;
}

QWindowsWindowClassRegistry *QWindowsWindowClassRegistry::instance()
{
    return m_instance;
}

QString QWindowsWindowClassRegistry::classNamePrefix()
{
    static QString result;
    if (result.isEmpty()) {
        BOBUIextStream str(&result);
        str << "BobUI" << BOBUI_VERSION_MAJOR << BOBUI_VERSION_MINOR << BOBUI_VERSION_PATCH;
        if (QLibraryInfo::isDebugBuild())
            str << 'd';
#ifdef BOBUI_NAMESPACE
#  define xstr(s) str(s)
#  define str(s) #s
        str << xstr(BOBUI_NAMESPACE);
#  undef str
#  undef xstr
#endif
    }
    return result;
}

QString QWindowsWindowClassRegistry::registerWindowClass(const QWindowsWindowClassDescription &description)
{
    QString className = description.name;

    if (description.shouldAddPrefix)
        className = classNamePrefix() + className;

    // since multiple BobUI versions can be used in one process
    // each one has to have window class names with a unique name
    // The first instance gets the unmodified name; if the class
    // has already been registered by another instance of BobUI then
    // add a UUID. The check needs to be performed for each name
    // in case new message windows are added (BOBUIBUG-81347).
    // Note: GetClassInfo() returns != 0 when a class exists.
    if (shouldDecorateWindowClassName(description))
        className += QUuid::createUuid().toString();

    if (m_registeredWindowClassNames.contains(className))        // already registered in our list
        return className;

    const auto appInstance = static_cast<HINSTANCE>(GetModuleHandle(nullptr));

    WNDCLASSEX wc{};
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.style = description.style;
    wc.lpfnWndProc = description.procedure;
    wc.hInstance = appInstance;
    wc.hbrBackground = description.brush;
    if (description.hasIcon) {
        wc.hIcon = static_cast<HICON>(LoadImage(appInstance, L"IDI_ICON1", IMAGE_ICON, 0, 0, LR_DEFAULTSIZE));
        if (wc.hIcon) {
            int sw = GetSystemMetrics(SM_CXSMICON);
            int sh = GetSystemMetrics(SM_CYSMICON);
            wc.hIconSm = static_cast<HICON>(LoadImage(appInstance, L"IDI_ICON1", IMAGE_ICON, sw, sh, 0));
        }
        else {
            wc.hIcon = static_cast<HICON>(LoadImage(nullptr, IDI_APPLICATION, IMAGE_ICON, 0, 0, LR_DEFAULTSIZE | LR_SHARED));
        }
    }

    wc.lpszClassName = reinterpret_cast<LPCWSTR>(className.utf16());
    ATOM atom = RegisterClassEx(&wc);
    if (!atom)
        qCWarning(lcQpaWindowClass) << "Failed to register window class" << className
            << "(" << bobui_error_string(-1) << ")";

    m_registeredWindowClassNames.insert(className);

    qCDebug(lcQpaWindowClass).nospace() << __FUNCTION__ << ' ' << className << ' ' << description << " atom=" << atom;

    return className;
}

QString QWindowsWindowClassRegistry::registerWindowClass(const QWindow *window)
{
    return registerWindowClass(QWindowsWindowClassDescription::fromWindow(window, m_defaultProcedure));
}

QString QWindowsWindowClassRegistry::registerWindowClass(QString name, WNDPROC procedure)
{
    return registerWindowClass(QWindowsWindowClassDescription::fromName(name, procedure));
}

bool QWindowsWindowClassRegistry::shouldDecorateWindowClassName(const QWindowsWindowClassDescription &description) const
{
    return shouldDecorateWindowClassName(description.name, description.procedure);
}

bool QWindowsWindowClassRegistry::shouldDecorateWindowClassName(const QString &name, WNDPROC procedure) const
{
    const auto appInstance = static_cast<HINSTANCE>(GetModuleHandle(nullptr));

    WNDCLASS wc{};

    return GetClassInfo(appInstance, reinterpret_cast<LPCWSTR>(name.utf16()), &wc) != FALSE
        && wc.lpfnWndProc != procedure;
}

void QWindowsWindowClassRegistry::unregisterWindowClasses()
{
    const auto appInstance = static_cast<HINSTANCE>(GetModuleHandle(nullptr));

    for (const QString &name : std::as_const(m_registeredWindowClassNames)) {
        if (!UnregisterClass(reinterpret_cast<LPCWSTR>(name.utf16()), appInstance) && QWindowsContext::verbose)
            qCWarning(lcQpaWindowClass) << "Failed to unregister window class" << name
                << "(" << bobui_error_string(-1) << ")";
    }
    m_registeredWindowClassNames.clear();
}

BOBUI_END_NAMESPACE
