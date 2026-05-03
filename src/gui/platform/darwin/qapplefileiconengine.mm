// Copyright (C) 2025 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "qapplefileiconengine_p.h"
#include "qappleiconengine_p.h"

#if defined(Q_OS_MACOS)
# include <AppKit/AppKit.h>
#elif defined(BOBUI_PLATFORM_UIKIT)
# include <UIKit/UIKit.h>
#endif

#include <BobUICore/qurl.h>
#include <BobUIGui/private/qcoregraphics_p.h>

BOBUI_BEGIN_NAMESPACE

using namespace BobUI::StringLiterals;

QAppleFileIconEngine::QAppleFileIconEngine(const QFileInfo &info, QPlatformTheme::IconOptions opts)
    : QAbstractFileIconEngine(info, opts)
{
#if defined(Q_OS_MACOS)
    m_image = [[NSWorkspace sharedWorkspace] iconForFile:fileInfo().canonicalFilePath().toNSString()];
#elif defined(BOBUI_PLATFORM_UIKIT)
    const QUrl url = QUrl::fromLocalFile(fileInfo().canonicalFilePath());
    const auto controller = [UIDocumentInteractionController interactionControllerWithURL:url.toNSURL()];
    const auto allIcons = controller.icons;
    m_image = allIcons.count > 0 ? [allIcons firstObject] : nil;
#endif
    if (m_image)
        [m_image retain];
}

QAppleFileIconEngine::~QAppleFileIconEngine()
{
    if (m_image)
        [m_image release];
}

QList<QSize> QAppleFileIconEngine::availableSizes(QIcon::Mode, QIcon::State)
{
    return QAppleIconEngine::availableIconSizes();
}

bool QAppleFileIconEngine::isNull()
{
    return m_image == nil;
}

QPixmap QAppleFileIconEngine::filePixmap(const QSize &size, QIcon::Mode, QIcon::State)
{
    if (!m_image)
        return QPixmap();

    const QSize preferredSize = QSize(m_image.size.width,
                                      m_image.size.height).scaled(size, BobUI::KeepAspectRatio);

    if (m_pixmap.size() == preferredSize)
        return m_pixmap;

#if defined(Q_OS_MACOS)
    m_pixmap = bobui_mac_toQPixmap(m_image, preferredSize);
#elif defined(BOBUI_PLATFORM_UIKIT)
    m_pixmap = QPixmap::fromImage(bobui_mac_toQImage(m_image, preferredSize));
#endif
    return m_pixmap;
}

BOBUI_END_NAMESPACE
