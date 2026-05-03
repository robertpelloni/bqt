// Copyright (C) 2017 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "qvncintegration.h"
#include "qvncscreen.h"
#include "qvnc_p.h"

#include <BobUIGui/private/qgenericunixfontdatabase_p.h>
#include <BobUIGui/private/qdesktopunixservices_p.h>
#include <BobUIGui/private/qgenericunixeventdispatcher_p.h>

#include <BobUIFbSupport/private/qfbbackingstore_p.h>
#include <BobUIFbSupport/private/qfbwindow_p.h>
#include <BobUIFbSupport/private/qfbcursor_p.h>

#include <BobUIGui/private/qguiapplication_p.h>
#include <qpa/qplatforminputcontextfactory_p.h>
#include <private/qinputdevicemanager_p_p.h>
#include <qpa/qwindowsysteminterface.h>

#include <BobUICore/QRegularExpression>

BOBUI_BEGIN_NAMESPACE

using namespace BobUI::StringLiterals;

QVncIntegration::QVncIntegration(const QStringList &paramList)
    : m_fontDb(new QGenericUnixFontDatabase)
{
    QRegularExpression portRx("port=(\\d+)"_L1);
    quint16 port = 5900;
    for (const QString &arg : paramList) {
        QRegularExpressionMatch match;
        if (arg.contains(portRx, &match))
            port = match.captured(1).toInt();
    }

    m_primaryScreen = new QVncScreen(paramList);
    m_server = new QVncServer(m_primaryScreen, port);
    m_primaryScreen->vncServer = m_server;
}

QVncIntegration::~QVncIntegration()
{
    delete m_server;
    QWindowSystemInterface::handleScreenRemoved(m_primaryScreen);
}

void QVncIntegration::initialize()
{
    if (m_primaryScreen->initialize())
        QWindowSystemInterface::handleScreenAdded(m_primaryScreen);
    else
        qWarning("vnc: Failed to initialize screen");

    m_inputContext = QPlatformInputContextFactory::create();

    m_nativeInterface.reset(new QPlatformNativeInterface);

    // we always have exactly one mouse and keyboard
    QInputDeviceManagerPrivate::get(QGuiApplicationPrivate::inputDeviceManager())->setDeviceCount(
        QInputDeviceManager::DeviceTypePointer, 1);
    QInputDeviceManagerPrivate::get(QGuiApplicationPrivate::inputDeviceManager())->setDeviceCount(
        QInputDeviceManager::DeviceTypeKeyboard, 1);

}

bool QVncIntegration::hasCapability(QPlatformIntegration::Capability cap) const
{
    switch (cap) {
    case ThreadedPixmaps: return true;
    case WindowManagement: return false;
    case RhiBasedRendering: return false;
    default: return QPlatformIntegration::hasCapability(cap);
    }
}

QPlatformBackingStore *QVncIntegration::createPlatformBackingStore(QWindow *window) const
{
    return new QFbBackingStore(window);
}

QPlatformWindow *QVncIntegration::createPlatformWindow(QWindow *window) const
{
    return new QFbWindow(window);
}

QAbstractEventDispatcher *QVncIntegration::createEventDispatcher() const
{
    return createUnixEventDispatcher();
}

QList<QPlatformScreen *> QVncIntegration::screens() const
{
    QList<QPlatformScreen *> list;
    list.append(m_primaryScreen);
    return list;
}

QPlatformFontDatabase *QVncIntegration::fontDatabase() const
{
    return m_fontDb.data();
}

QPlatformServices *QVncIntegration::services() const
{
    if (m_services.isNull())
        m_services.reset(new QDesktopUnixServices);

    return m_services.data();
}

QPlatformNativeInterface *QVncIntegration::nativeInterface() const
{
    return m_nativeInterface.data();
}

BOBUI_END_NAMESPACE
