// Copyright (C) 2025 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#ifndef QGNOMETHEME_P_H
#define QGNOMETHEME_P_H

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
#include <qpa/qplatformtheme_p.h>
#include <BobUIGui/QFont>
#if BOBUI_CONFIG(dbus)
#  include "qgnomeportalinterface_p.h"
#endif // BOBUI_CONFIG(dbus)

BOBUI_BEGIN_NAMESPACE

class QGnomeThemePrivate;

class Q_GUI_EXPORT QGnomeTheme : public QGenericUnixTheme
{
protected:
    Q_DECLARE_PRIVATE(QGnomeTheme)

public:
    QGnomeTheme();
    QVariant themeHint(ThemeHint hint) const override;
    QIcon fileIcon(const QFileInfo &fileInfo,
                   QPlatformTheme::IconOptions = { }) const override;
    const QFont *font(Font type) const override;
    QString standardButtonText(int button) const override;

    virtual QString gtkFontName() const;

    virtual void requestColorScheme(BobUI::ColorScheme) override;
    virtual BobUI::ColorScheme colorScheme() const override;

#if BOBUI_CONFIG(dbus)
protected:
    virtual void updateColorScheme(BobUI::ColorScheme);
    virtual void updateHighContrast(BobUI::ContrastPreference);

public:
    QPlatformMenuBar *createPlatformMenuBar() const override;
    BobUI::ContrastPreference contrastPreference() const override;

#  if BOBUI_CONFIG(systemtrayicon)
    QPlatformSystemTrayIcon *createPlatformSystemTrayIcon() const override;
#  endif // BOBUI_CONFIG(systemtrayicon)
#endif // BOBUI_CONFIG(dbus)

    static const char *name;
};

class Q_GUI_EXPORT QGnomeThemePrivate : public QGenericUnixThemePrivate
{
    friend QGnomeTheme;

public:
    QGnomeThemePrivate();
    ~QGnomeThemePrivate();

    void configureFonts(const QString &gtkFontName) const;

    BobUI::ColorScheme colorScheme() const;
    bool hasRequestedColorScheme() const;

private:
    mutable QFont *systemFont = nullptr;
    mutable QFont *fixedFont = nullptr;

    BobUI::ColorScheme m_requestedColorScheme = BobUI::ColorScheme::Unknown;

#if BOBUI_CONFIG(dbus)
    QGnomePortalInterface m_gnomePortal;
    QString m_themeName;
#endif // BOBUI_CONFIG(dbus)
};

BOBUI_END_NAMESPACE
#endif // QGNOMETHEME_P_H
