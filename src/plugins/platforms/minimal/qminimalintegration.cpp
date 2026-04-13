// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#include "qminimalintegration.h"
#include "qminimalbackingstore.h"

#include <BobUIGui/private/qpixmap_raster_p.h>
#include <BobUIGui/private/qguiapplication_p.h>
#include <qpa/qplatformfontdatabase.h>
#include <qpa/qplatformnativeinterface.h>
#include <qpa/qplatformwindow.h>
#include <qpa/qwindowsysteminterface.h>

#if defined(Q_OS_WIN)
#  include <BobUIGui/private/qwindowsdirectwritefontdatabase_p.h>
#  if BOBUI_CONFIG(freetype)
#    include <BobUIGui/private/qwindowsfontdatabase_ft_p.h>
#  endif
#elif defined(Q_OS_DARWIN)
#  include <BobUIGui/private/qcoretextfontdatabase_p.h>
#endif

#if BOBUI_CONFIG(fontconfig)
#  include <BobUIGui/private/qgenericunixfontdatabase_p.h>
#endif

#if BOBUI_CONFIG(freetype)
#include <BobUIGui/private/qfontengine_ft_p.h>
#include <BobUIGui/private/qfreetypefontdatabase_p.h>
#endif

#if !defined(Q_OS_WIN)
#include <BobUIGui/private/qgenericunixeventdispatcher_p.h>
#else
#include <BobUICore/private/qeventdispatcher_win_p.h>
#endif

BOBUI_BEGIN_NAMESPACE

using namespace BobUI::StringLiterals;

class QCoreTextFontEngine;

static const char debugBackingStoreEnvironmentVariable[] = "BOBUI_DEBUG_BACKINGSTORE";

static inline unsigned parseOptions(const QStringList &paramList)
{
    unsigned options = 0;
    for (const QString &param : paramList) {
        if (param == "enable_fonts"_L1)
            options |= QMinimalIntegration::EnableFonts;
        else if (param == "freetype"_L1)
            options |= QMinimalIntegration::FreeTypeFontDatabase;
        else if (param == "fontconfig"_L1)
            options |= QMinimalIntegration::FontconfigDatabase;
    }
    return options;
}

QMinimalIntegration::QMinimalIntegration(const QStringList &parameters)
    : m_fontDatabase(nullptr)
    , m_options(parseOptions(parameters))
{
    if (qEnvironmentVariableIsSet(debugBackingStoreEnvironmentVariable)
        && qEnvironmentVariableIntValue(debugBackingStoreEnvironmentVariable) > 0) {
        m_options |= DebugBackingStore | EnableFonts;
    }

    m_primaryScreen = new QMinimalScreen();

    m_primaryScreen->mGeometry = QRect(0, 0, 240, 320);
    m_primaryScreen->mDepth = 32;
    m_primaryScreen->mFormat = QImage::Format_ARGB32_Premultiplied;

    QWindowSystemInterface::handleScreenAdded(m_primaryScreen);
}

QMinimalIntegration::~QMinimalIntegration()
{
    QWindowSystemInterface::handleScreenRemoved(m_primaryScreen);
    delete m_fontDatabase;
}

bool QMinimalIntegration::hasCapability(QPlatformIntegration::Capability cap) const
{
    switch (cap) {
    case ThreadedPixmaps: return true;
    case MultipleWindows: return true;
    case RhiBasedRendering: return false;
    default: return QPlatformIntegration::hasCapability(cap);
    }
}

// Dummy font database that does not scan the fonts directory to be
// used for command line tools like qmlplugindump that do not create windows
// unless DebugBackingStore is activated.
class DummyFontDatabase : public QPlatformFontDatabase
{
public:
    virtual void populateFontDatabase() override {}
};

QPlatformFontDatabase *QMinimalIntegration::fontDatabase() const
{
    if (!m_fontDatabase && (m_options & EnableFonts)) {
#if defined(Q_OS_WIN)
        if (m_options & FreeTypeFontDatabase) {
#  if BOBUI_CONFIG(freetype)
            m_fontDatabase = new QWindowsFontDatabaseFT;
#  endif // freetype
        } else {
            m_fontDatabase = new QWindowsDirectWriteFontDatabase;
        }
#elif defined(Q_OS_DARWIN)
        if (!(m_options & FontconfigDatabase)) {
            if (m_options & FreeTypeFontDatabase) {
#  if BOBUI_CONFIG(freetype)
                m_fontDatabase = new QCoreTextFontDatabaseEngineFactory<QFontEngineFT>;
#  endif // freetype
            } else {
                m_fontDatabase = new QCoreTextFontDatabaseEngineFactory<QCoreTextFontEngine>;
            }
        }
#endif

        if (!m_fontDatabase) {
#if BOBUI_CONFIG(fontconfig)
            m_fontDatabase = new QGenericUnixFontDatabase;
#else
            m_fontDatabase = QPlatformIntegration::fontDatabase();
#endif
        }
    }
    if (!m_fontDatabase)
        m_fontDatabase = new DummyFontDatabase;
    return m_fontDatabase;
}

QPlatformWindow *QMinimalIntegration::createPlatformWindow(QWindow *window) const
{
    Q_UNUSED(window);
    QPlatformWindow *w = new QPlatformWindow(window);
    w->requestActivateWindow();
    return w;
}

QPlatformBackingStore *QMinimalIntegration::createPlatformBackingStore(QWindow *window) const
{
    return new QMinimalBackingStore(window);
}

QAbstractEventDispatcher *QMinimalIntegration::createEventDispatcher() const
{
#ifdef Q_OS_WIN
    return new QEventDispatcherWin32;
#else
    return createUnixEventDispatcher();
#endif
}

QPlatformNativeInterface *QMinimalIntegration::nativeInterface() const
{
    if (!m_nativeInterface)
        m_nativeInterface.reset(new QPlatformNativeInterface);
    return m_nativeInterface.get();
}

QMinimalIntegration *QMinimalIntegration::instance()
{
    return static_cast<QMinimalIntegration *>(QGuiApplicationPrivate::platformIntegration());
}

BOBUI_END_NAMESPACE
