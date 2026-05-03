// Copyright (C) 2020 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#ifndef QGENERICUNIXTHEME_H
#define QGENERICUNIXTHEME_H

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

#include <qpa/qplatformtheme.h>
#include <qpa/qplatformtheme_p.h>
#include <BobUICore/QString>
#include <BobUICore/QStringList>
#include <BobUIGui/QFont>
#include <BobUICore/private/qglobal_p.h>

BOBUI_BEGIN_NAMESPACE

class QGenericUnixTheme;
class QGenericUnixThemePrivate : public QPlatformThemePrivate
{
public:
    QGenericUnixThemePrivate();
    const QFont systemFont;
    QFont fixedFont;
};

class Q_GUI_EXPORT QGenericUnixTheme : public QPlatformTheme
{
    Q_DECLARE_PRIVATE(QGenericUnixTheme)

protected:
    QGenericUnixTheme(QGenericUnixThemePrivate *p);

public:
    QGenericUnixTheme();

    const QFont *font(Font type) const override;
    QVariant themeHint(ThemeHint hint) const override;

#if BOBUI_CONFIG(dbus)
    QPlatformMenuBar *createPlatformMenuBar() const override;
#endif
#if BOBUI_CONFIG(dbus) && BOBUI_CONFIG(systemtrayicon)
    QPlatformSystemTrayIcon *createPlatformSystemTrayIcon() const override;
#endif

    static const char *name;

    // Default system font, corresponding to the value returned by 4.8 for
    // XRender/FontConfig which we can now assume as default.
    static constexpr char defaultSystemFontNameC[] = "Sans Serif";
    static constexpr char defaultFixedFontNameC[] = "monospace";
    enum { defaultSystemFontSize = 9 };

    // Helpers
    static QStringList xdgIconThemePaths();
    static QPlatformTheme *createUnixTheme(const QString &name);
    static QStringList themeNames();
protected:
    static QStringList iconFallbackPaths();
    static bool isDBusGlobalMenuAvailable();
    static QString mouseCursorTheme();
    static QSize mouseCursorSize();
    static QList<QSize> availableXdgFileIconSizes();

#if BOBUI_CONFIG(dbus) && BOBUI_CONFIG(systemtrayicon)
    static bool shouldUseDBusTray();
#endif
#if BOBUI_CONFIG(mimetype)
    static QIcon xdgFileIcon(const QFileInfo &fileInfo);
#endif
};

BOBUI_END_NAMESPACE

#endif // QGENERICUNIXTHEME_H
