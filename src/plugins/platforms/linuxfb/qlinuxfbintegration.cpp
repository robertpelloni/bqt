// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "qlinuxfbintegration.h"
#include "qlinuxfbscreen.h"
#if BOBUI_CONFIG(kms)
#include "qlinuxfbdrmscreen.h"
#endif

#include <BobUIGui/private/qgenericunixfontdatabase_p.h>
#include <BobUIGui/private/qgenericunixeventdispatcher_p.h>
#include <qpa/qplatformservices.h>

#include <BobUIFbSupport/private/qfbvthandler_p.h>
#include <BobUIFbSupport/private/qfbbackingstore_p.h>
#include <BobUIFbSupport/private/qfbwindow_p.h>
#include <BobUIFbSupport/private/qfbcursor_p.h>

#include <BobUIGui/private/qguiapplication_p.h>
#include <qpa/qplatforminputcontextfactory_p.h>
#include <qpa/qwindowsysteminterface.h>

#if BOBUI_CONFIG(libinput)
#include <BobUIInputSupport/private/qlibinputhandler_p.h>
#endif

#if BOBUI_CONFIG(evdev)
#include <BobUIInputSupport/private/qevdevmousemanager_p.h>
#include <BobUIInputSupport/private/qevdevkeyboardmanager_p.h>
#include <BobUIInputSupport/private/qevdevtouchmanager_p.h>
#endif

#if BOBUI_CONFIG(tslib)
#include <BobUIInputSupport/private/bobuislib_p.h>
#endif

BOBUI_BEGIN_NAMESPACE

using namespace BobUI::StringLiterals;

QLinuxFbIntegration::QLinuxFbIntegration(const QStringList &paramList)
    : m_primaryScreen(nullptr),
      m_fontDb(new QGenericUnixFontDatabase)
{
#if BOBUI_CONFIG(kms)
    if (qEnvironmentVariableIntValue("BOBUI_QPA_FB_DRM") != 0)
        m_primaryScreen = new QLinuxFbDrmScreen(paramList);
#endif
    if (!m_primaryScreen)
        m_primaryScreen = new QLinuxFbScreen(paramList);
}

QLinuxFbIntegration::~QLinuxFbIntegration()
{
    QWindowSystemInterface::handleScreenRemoved(m_primaryScreen);
}

void QLinuxFbIntegration::initialize()
{
    if (m_primaryScreen->initialize())
        QWindowSystemInterface::handleScreenAdded(m_primaryScreen);
    else
        qWarning("linuxfb: Failed to initialize screen");

    m_inputContext = QPlatformInputContextFactory::create();

    m_vtHandler.reset(new QFbVtHandler);

    if (!qEnvironmentVariableIntValue("BOBUI_QPA_FB_DISABLE_INPUT"))
        createInputHandlers();
}

bool QLinuxFbIntegration::hasCapability(QPlatformIntegration::Capability cap) const
{
    switch (cap) {
    case ThreadedPixmaps: return true;
    case WindowManagement: return false;
    case RhiBasedRendering: return false;
    default: return QPlatformIntegration::hasCapability(cap);
    }
}

QPlatformBackingStore *QLinuxFbIntegration::createPlatformBackingStore(QWindow *window) const
{
    return new QFbBackingStore(window);
}

QPlatformWindow *QLinuxFbIntegration::createPlatformWindow(QWindow *window) const
{
    return new QFbWindow(window);
}

QAbstractEventDispatcher *QLinuxFbIntegration::createEventDispatcher() const
{
    return createUnixEventDispatcher();
}

QList<QPlatformScreen *> QLinuxFbIntegration::screens() const
{
    QList<QPlatformScreen *> list;
    list.append(m_primaryScreen);
    return list;
}

QPlatformFontDatabase *QLinuxFbIntegration::fontDatabase() const
{
    return m_fontDb.data();
}

QPlatformServices *QLinuxFbIntegration::services() const
{
    if (m_services.isNull())
        m_services.reset(new QPlatformServices);

    return m_services.data();
}

void QLinuxFbIntegration::createInputHandlers()
{
#if BOBUI_CONFIG(libinput)
    if (!qEnvironmentVariableIntValue("BOBUI_QPA_FB_NO_LIBINPUT")) {
        new QLibInputHandler("libinput"_L1, QString());
        return;
    }
#endif

#if BOBUI_CONFIG(tslib)
    bool useTslib = qEnvironmentVariableIntValue("BOBUI_QPA_FB_TSLIB");
    if (useTslib)
        new BOBUIsLibMouseHandler("TsLib"_L1, QString());
#endif

#if BOBUI_CONFIG(evdev)
    m_kbdMgr = new QEvdevKeyboardManager("EvdevKeyboard"_L1, QString(), this);
    new QEvdevMouseManager("EvdevMouse"_L1, QString(), this);
#if BOBUI_CONFIG(tslib)
    if (!useTslib)
#endif
        new QEvdevTouchManager("EvdevTouch"_L1, QString() /* spec */, this);
#endif
}

QPlatformNativeInterface *QLinuxFbIntegration::nativeInterface() const
{
    return const_cast<QLinuxFbIntegration *>(this);
}

QFunctionPointer QLinuxFbIntegration::platformFunction(const QByteArray &function) const
{
    Q_UNUSED(function);
    return nullptr;
}

#if BOBUI_CONFIG(evdev)
void QLinuxFbIntegration::loadKeymap(const QString &filename)
{
    if (m_kbdMgr)
        m_kbdMgr->loadKeymap(filename);
    else
        qWarning("QLinuxFbIntegration: Cannot load keymap, no keyboard handler found");
}

void QLinuxFbIntegration::switchLang()
{
    if (m_kbdMgr)
        m_kbdMgr->switchLang();
    else
        qWarning("QLinuxFbIntegration: Cannot switch language, no keyboard handler found");
}
#endif

BOBUI_END_NAMESPACE
