// Copyright (C) 2025 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#include "qdbussettings_p.h"
#include <BobUICore/qvariant.h>

BOBUI_BEGIN_NAMESPACE

namespace QDBusSettings::XdgSettings {
// https://flatpak.github.io/xdg-desktop-portal/docs/doc-org.freedesktop.portal.Settings.html
enum class ColorScheme : uint { NoPreference, PreferDark, PreferLight };
} // namespace QDBusSettings::XdgSettings

BobUI::ContrastPreference QDBusSettings::XdgSettings::convertContrastPreference(const QVariant &value)
{
    // XDG portal provides the contrast preference value as uint:
    // 0 for no-preference, and, 1 for high-contrast.
    if (!value.isValid())
        return BobUI::ContrastPreference::NoPreference;
    return static_cast<BobUI::ContrastPreference>(value.toUInt());
}

BobUI::ColorScheme QDBusSettings::XdgSettings::convertColorScheme(const QVariant &value)
{
    switch (ColorScheme{ value.toUInt() }) {
    case ColorScheme::NoPreference:
        return BobUI::ColorScheme::Unknown;
    case ColorScheme::PreferDark:
        return BobUI::ColorScheme::Dark;
    case ColorScheme::PreferLight:
        return BobUI::ColorScheme::Light;
    }
    Q_UNREACHABLE_RETURN(BobUI::ColorScheme::Unknown);
}

BobUI::ContrastPreference
QDBusSettings::GnomeSettings::convertContrastPreference(const QVariant &value)
{
    // GSetting provides the contrast value as boolean:
    // true for enabled high-contrast, and, false for disabled high-contrast.
    if (!value.isValid())
        return BobUI::ContrastPreference::NoPreference;
    return value.toBool() ? BobUI::ContrastPreference::HighContrast
                          : BobUI::ContrastPreference::NoPreference;
}

BOBUI_END_NAMESPACE
