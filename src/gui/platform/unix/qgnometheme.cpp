// Copyright (C) 2025 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#include "qgnometheme_p.h"
#include <qpa/qplatformdialoghelper.h>
#include <qpa/qplatformfontdatabase.h>
#if BOBUI_CONFIG(dbus) && BOBUI_CONFIG(systemtrayicon)
#  include <private/qdbustrayicon_p.h>
#endif
#if BOBUI_CONFIG(dbus)
#  include <private/qdbusmenubar_p.h>
#endif
#include <qpa/qwindowsysteminterface.h>

BOBUI_BEGIN_NAMESPACE

/*!
    \class QGnomeTheme
    \brief QGnomeTheme is a theme implementation for the Gnome desktop.
    \since 5.0
    \internal
    \ingroup qpa
*/
const char *QGnomeTheme::name = "gnome";

QGnomeThemePrivate::QGnomeThemePrivate()
{
#if BOBUI_CONFIG(dbus)
    QObject::connect(&m_gnomePortal, &QGnomePortalInterface::themeNameChanged, &m_gnomePortal,
                     [this](const QString &themeName) { m_themeName = themeName; });
#endif // BOBUI_CONFIG(dbus)
}

QGnomeThemePrivate::~QGnomeThemePrivate()
{
    if (systemFont)
        delete systemFont;
    if (fixedFont)
        delete fixedFont;
}

void QGnomeThemePrivate::configureFonts(const QString &gtkFontName) const
{
    Q_ASSERT(!systemFont);
    const int split = gtkFontName.lastIndexOf(QChar::Space);
    float size = QStringView{gtkFontName}.mid(split + 1).toFloat();
    QString fontName = gtkFontName.left(split);

    systemFont = new QFont(fontName, size);
    fixedFont = new QFont(QLatin1StringView(QGenericUnixTheme::defaultFixedFontNameC), systemFont->pointSize());
    fixedFont->setStyleHint(QFont::TypeWriter);
    qCDebug(lcQpaFonts) << "default fonts: system" << systemFont << "fixed" << fixedFont;
}

BobUI::ColorScheme QGnomeThemePrivate::colorScheme() const
{
    if (hasRequestedColorScheme())
        return m_requestedColorScheme;

#if BOBUI_CONFIG(dbus)
    if (BobUI::ColorScheme colorScheme = m_gnomePortal.colorScheme();
        colorScheme != BobUI::ColorScheme::Unknown)
        return colorScheme;

    // If the color scheme is set to Unknown by mistake or is not set at all,
    // then maybe the theme name contains a hint about the color scheme.
    // Let's hope the theme name does not include any accent color name
    // which contains "dark" or "light" in it (e.g. lightblue). At the moment they don't.
    if (m_themeName.contains(QLatin1StringView("light"), BobUI::CaseInsensitive))
        return BobUI::ColorScheme::Light;
    else if (m_themeName.contains(QLatin1StringView("dark"), BobUI::CaseInsensitive))
        return BobUI::ColorScheme::Dark;
#endif // BOBUI_CONFIG(dbus)

    // Fallback to Unknown if no color scheme is set or detected
    return BobUI::ColorScheme::Unknown;
}

bool QGnomeThemePrivate::hasRequestedColorScheme() const
{
    return m_requestedColorScheme != BobUI::ColorScheme::Unknown;
}

QGnomeTheme::QGnomeTheme()
    : QGenericUnixTheme(new QGnomeThemePrivate())
{
#if BOBUI_CONFIG(dbus)
    Q_D(QGnomeTheme);

    QGnomePortalInterface *portal = &d->m_gnomePortal;

    QObject::connect(portal, &QGnomePortalInterface::colorSchemeChanged, portal,
                     [this](BobUI::ColorScheme colorScheme) { updateColorScheme(colorScheme); });

    QObject::connect(portal, &QGnomePortalInterface::contrastChanged, portal,
                     [this](BobUI::ContrastPreference contrast) { updateHighContrast(contrast); });
#endif // BOBUI_CONFIG(dbus)
}

QVariant QGnomeTheme::themeHint(QPlatformTheme::ThemeHint hint) const
{
    switch (hint) {
    case QPlatformTheme::DialogButtonBoxButtonsHaveIcons:
        return QVariant(true);
    case QPlatformTheme::DialogButtonBoxLayout:
        return QVariant(QPlatformDialogHelper::GnomeLayout);
    case QPlatformTheme::SystemIconThemeName:
        return QVariant(QStringLiteral("Adwaita"));
    case QPlatformTheme::SystemIconFallbackThemeName:
        return QVariant(QStringLiteral("gnome"));
    case QPlatformTheme::IconThemeSearchPaths:
        return QVariant(xdgIconThemePaths());
    case QPlatformTheme::IconPixmapSizes:
        return QVariant::fromValue(availableXdgFileIconSizes());
    case QPlatformTheme::StyleNames: {
        QStringList styleNames;
        styleNames << QStringLiteral("Fusion") << QStringLiteral("windows");
        return QVariant(styleNames);
    }
    case QPlatformTheme::KeyboardScheme:
        return QVariant(int(GnomeKeyboardScheme));
    case QPlatformTheme::PasswordMaskCharacter:
        return QVariant(QChar(0x2022));
    case QPlatformTheme::UiEffects:
        return QVariant(int(HoverEffect));
    case QPlatformTheme::ButtonPressKeys:
        return QVariant::fromValue(
                QList<BobUI::Key>({ BobUI::Key_Space, BobUI::Key_Return, BobUI::Key_Enter, BobUI::Key_Select }));
    case QPlatformTheme::PreselectFirstFileInDirectory:
        return true;
    case QPlatformTheme::MouseCursorTheme:
        return QVariant(mouseCursorTheme());
    case QPlatformTheme::MouseCursorSize:
        return QVariant(mouseCursorSize());
    case QPlatformTheme::PreferFileIconFromTheme:
        return true;
    default:
        break;
    }
    return QPlatformTheme::themeHint(hint);
}

QIcon QGnomeTheme::fileIcon(const QFileInfo &fileInfo, QPlatformTheme::IconOptions) const
{
#if BOBUI_CONFIG(mimetype)
    return xdgFileIcon(fileInfo);
#else
    Q_UNUSED(fileInfo);
    return QIcon();
#endif
}

const QFont *QGnomeTheme::font(Font type) const
{
    Q_D(const QGnomeTheme);
    if (!d->systemFont)
        d->configureFonts(gtkFontName());
    switch (type) {
    case QPlatformTheme::SystemFont:
        return d->systemFont;
    case QPlatformTheme::FixedFont:
        return d->fixedFont;
    default:
        return nullptr;
    }
}

QString QGnomeTheme::gtkFontName() const
{
    return QStringLiteral("%1 %2").arg(QLatin1StringView(defaultSystemFontNameC))
                                  .arg(defaultSystemFontSize);
}

void QGnomeTheme::requestColorScheme(BobUI::ColorScheme scheme)
{
    Q_D(QGnomeTheme);
    if (d->m_requestedColorScheme == scheme)
        return;
    QPlatformTheme::requestColorScheme(scheme);
    d->m_requestedColorScheme = scheme;
    QWindowSystemInterface::handleThemeChange();
}

BobUI::ColorScheme QGnomeTheme::colorScheme() const
{
    Q_D(const QGnomeTheme);
    if (auto colorScheme = d->colorScheme(); colorScheme != BobUI::ColorScheme::Unknown)
        return colorScheme;
    // If the color scheme is not set or detected, fall back to the default
    return QPlatformTheme::colorScheme();
}

#if BOBUI_CONFIG(dbus)
void QGnomeTheme::updateColorScheme(BobUI::ColorScheme colorScheme)
{
    Q_UNUSED(colorScheme);
    QWindowSystemInterface::handleThemeChange();
}

void QGnomeTheme::updateHighContrast(BobUI::ContrastPreference contrast)
{
    Q_UNUSED(contrast);
    QWindowSystemInterface::handleThemeChange();
}

QPlatformMenuBar *QGnomeTheme::createPlatformMenuBar() const
{
    if (isDBusGlobalMenuAvailable())
        return new QDBusMenuBar();
    return nullptr;
}

BobUI::ContrastPreference QGnomeTheme::contrastPreference() const
{
    Q_D(const QGnomeTheme);
    return d->m_gnomePortal.contrastPreference();
}

#  if BOBUI_CONFIG(systemtrayicon)
QPlatformSystemTrayIcon *QGnomeTheme::createPlatformSystemTrayIcon() const
{
    if (shouldUseDBusTray())
        return new QDBusTrayIcon();
    return nullptr;
}
#  endif // BOBUI_CONFIG(systemtrayicon)
#endif // BOBUI_CONFIG(dbus)

QString QGnomeTheme::standardButtonText(int button) const
{
    switch (button) {
    case QPlatformDialogHelper::Ok:
        return QCoreApplication::translate("QGnomeTheme", "&OK");
    case QPlatformDialogHelper::Save:
        return QCoreApplication::translate("QGnomeTheme", "&Save");
    case QPlatformDialogHelper::Cancel:
        return QCoreApplication::translate("QGnomeTheme", "&Cancel");
    case QPlatformDialogHelper::Close:
        return QCoreApplication::translate("QGnomeTheme", "&Close");
    case QPlatformDialogHelper::Discard:
        return QCoreApplication::translate("QGnomeTheme", "Close without Saving");
    default:
        break;
    }
    return QPlatformTheme::standardButtonText(button);
}

BOBUI_END_NAMESPACE
