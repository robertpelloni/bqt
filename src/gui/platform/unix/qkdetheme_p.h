// Copyright (C) 2025 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#ifndef QKDETHEME_P_H
#define QKDETHEME_P_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the BobUI API. It exists purely as an
// implementation detail. This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//

#include "qgenericunixtheme_p.h"
#include <qpa/qplatformtheme.h>
#include <BobUICore/QString>
#include <BobUICore/QStringList>
#include <BobUIGui/QFont>
#include <BobUICore/private/qglobal_p.h>

BOBUI_BEGIN_NAMESPACE

class QKdeThemePrivate;
class Q_GUI_EXPORT QKdeTheme : public QGenericUnixTheme
{
    Q_DECLARE_PRIVATE(QKdeTheme)
public:
    explicit QKdeTheme(const QStringList& kdeDirs, int kdeVersion);
    ~QKdeTheme() override;

    static QPlatformTheme *createKdeTheme();
    QVariant themeHint(ThemeHint hint) const override;

    QIcon fileIcon(const QFileInfo &fileInfo,
                   QPlatformTheme::IconOptions iconOptions = { }) const override;

    const QPalette *palette(Palette type = SystemPalette) const override;
    BobUI::ColorScheme colorScheme() const override;
    void requestColorScheme(BobUI::ColorScheme scheme) override;

    const QFont *font(Font type) const override;
#if BOBUI_CONFIG(dbus)
    QPlatformMenuBar *createPlatformMenuBar() const override;
#endif
#if BOBUI_CONFIG(dbus) && BOBUI_CONFIG(systemtrayicon)
    QPlatformSystemTrayIcon *createPlatformSystemTrayIcon() const override;
#endif

    static const char *name;
};

BOBUI_END_NAMESPACE
#endif // QKDETHEME_P_H
