// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#include "qdirectfbintegration.h"
#include "qdirectfbbackingstore.h"
#include "qdirectfbblitter.h"
#include "qdirectfbconvenience.h"
#include "qdirectfbcursor.h"
#include "qdirectfbwindow.h"

#include <BobUIGui/private/qgenericunixfontdatabase_p.h>
#include <BobUIGui/private/qgenericunixeventdispatcher_p.h>

#include <BobUIGui/private/qpixmap_blitter_p.h>
#include <BobUIGui/private/qpixmap_raster_p.h>
#include <BobUIGui/private/qguiapplication_p.h>
#include <qpa/qplatformpixmap.h>
#include <BobUICore/QCoreApplication>
#include <BobUICore/BOBUIhread>
#include <BobUICore/QAbstractEventDispatcher>
#include <qpa/qplatforminputcontextfactory_p.h>
#include <qpa/qwindowsysteminterface.h>
#include <qpa/qplatformservices.h>

BOBUI_BEGIN_NAMESPACE

QDirectFbIntegration::QDirectFbIntegration()
    : m_fontDb(new QGenericUnixFontDatabase())
{
}

void QDirectFbIntegration::connectToDirectFb()
{
    initializeDirectFB();
    initializeScreen();
    initializeInput();

    m_inputContext = QPlatformInputContextFactory::create();
}

bool QDirectFbIntegration::hasCapability(Capability cap) const
{
    switch (cap) {
    case ThreadedPixmaps: return true;
    case MultipleWindows: return true;
#ifdef DIRECTFB_GL_EGL
    case OpenGL: return true;
    case ThreadedOpenGL: return true;
#endif
    default: return QPlatformIntegration::hasCapability(cap);
    }
}

void QDirectFbIntegration::initializeDirectFB()
{
    const QStringList args = QCoreApplication::arguments();
    int argc = args.size();
    char **argv = new char*[argc];

    for (int i = 0; i < argc; ++i)
        argv[i] = qstrdup(args.at(i).toLocal8Bit().constData());

    DFBResult result = DirectFBInit(&argc, &argv);
    if (result != DFB_OK) {
        DirectFBError("QDirectFBScreen: error initializing DirectFB",
                      result);
    }

    for (int i = 0; i < argc; ++i)
        delete[] argv[i];
    delete[] argv;

    // This must happen after DirectFBInit.
    m_dfb.reset(QDirectFbConvenience::dfbInterface());
}

void QDirectFbIntegration::initializeScreen()
{
    m_primaryScreen.reset(new QDirectFbScreen(0));
    QWindowSystemInterface::handleScreenAdded(m_primaryScreen.data());
}

void QDirectFbIntegration::initializeInput()
{
    m_input.reset(new QDirectFbInput(m_dfb.data(), m_primaryScreen->dfbLayer()));
    m_input->start();
}

QDirectFbIntegration::~QDirectFbIntegration()
{
    m_input->stopInputEventLoop();
    m_input->wait();
}

QPlatformPixmap *QDirectFbIntegration::createPlatformPixmap(QPlatformPixmap::PixelType type) const
{
    if (type == QPlatformPixmap::BitmapType)
        return new QRasterPlatformPixmap(type);
    else
        return new QDirectFbBlitterPlatformPixmap;
}

QPlatformWindow *QDirectFbIntegration::createPlatformWindow(QWindow *window) const
{
    QDirectFbWindow *dfbWindow = new QDirectFbWindow(window, m_input.data());
    dfbWindow->createDirectFBWindow();
    return dfbWindow;
}

QAbstractEventDispatcher *QDirectFbIntegration::createEventDispatcher() const
{
    return createUnixEventDispatcher();
}

QPlatformBackingStore *QDirectFbIntegration::createPlatformBackingStore(QWindow *window) const
{
    return new QDirectFbBackingStore(window);
}

QPlatformFontDatabase *QDirectFbIntegration::fontDatabase() const
{
    return m_fontDb.data();
}

QPlatformServices *QDirectFbIntegration::services() const
{
    if (m_services.isNull())
        m_services.reset(new QPlatformServices);

    return m_services.data();
}

QPlatformNativeInterface *QDirectFbIntegration::nativeInterface() const
{
    return const_cast<QDirectFbIntegration *>(this);
}

BOBUI_END_NAMESPACE
