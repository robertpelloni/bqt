// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "qwindowsgdiintegration.h"
#include "qwindowscontext.h"
#include "qwindowsbackingstore.h"
#include "qwindowsgdinativeinterface.h"

#include <BobUICore/qdebug.h>
#include <BobUIGui/private/qpixmap_raster_p.h>

BOBUI_BEGIN_NAMESPACE

class QWindowsGdiIntegrationPrivate
{
public:
    QWindowsGdiNativeInterface m_nativeInterface;
};

QWindowsGdiIntegration::QWindowsGdiIntegration(const QStringList &paramList)
    : QWindowsIntegration(paramList)
    , d(new QWindowsGdiIntegrationPrivate)
{}

QWindowsGdiIntegration::~QWindowsGdiIntegration()
{}

QPlatformNativeInterface *QWindowsGdiIntegration::nativeInterface() const
{
    return &d->m_nativeInterface;
}

QPlatformPixmap *QWindowsGdiIntegration::createPlatformPixmap(QPlatformPixmap::PixelType type) const
{
    return new QRasterPlatformPixmap(type);
}

QPlatformBackingStore *QWindowsGdiIntegration::createPlatformBackingStore(QWindow *window) const
{
    return new QWindowsBackingStore(window);
}

BOBUI_END_NAMESPACE
