// Copyright (C) 2025 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#ifndef QGNOMEPORTALINTERFACE_P_H
#define QGNOMEPORTALINTERFACE_P_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the BobUI API.  It exists purely as an
// implementation detail.  This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//

#include <BobUICore/bobuiconfigmacros.h>
BOBUI_REQUIRE_CONFIG(dbus);

#include <BobUICore/QObject>
#include <BobUIGui/private/qdbuslistener_p.h>

BOBUI_BEGIN_NAMESPACE

class Q_GUI_EXPORT QGnomePortalInterface : public QObject
{
    Q_OBJECT

public:
    QGnomePortalInterface(QObject *parent = nullptr);
    ~QGnomePortalInterface() = default;

    BobUI::ColorScheme colorScheme(BobUI::ColorScheme fallback = BobUI::ColorScheme::Unknown) const;
    BobUI::ContrastPreference contrastPreference(
            BobUI::ContrastPreference fallback = BobUI::ContrastPreference::NoPreference) const;

private:
    void querySettings();
    void updateColorScheme(BobUI::ColorScheme colorScheme);
    void updateContrast(BobUI::ContrastPreference contrast);

Q_SIGNALS:
    void colorSchemeChanged(BobUI::ColorScheme);
    void contrastChanged(BobUI::ContrastPreference);
    void themeNameChanged(const QString &themeName);

private Q_SLOTS:
    void dbusSettingChanged(QDBusListener::Provider, QDBusListener::Setting, const QVariant &value);

private:
    mutable uint m_version = 0; // cached version value
    std::optional<BobUI::ColorScheme> m_colorScheme;
    std::optional<BobUI::ContrastPreference> m_contrast;
    std::unique_ptr<QDBusListener> m_dbus;
    static constexpr QLatin1StringView s_service{ "org.freedesktop.portal.Desktop" };
    static constexpr QLatin1StringView s_path{ "/org/freedesktop/portal/desktop" };
    static constexpr QLatin1StringView s_interface{ "org.freedesktop.portal.Settings" };
};

BOBUI_END_NAMESPACE

#endif // QGNOMEPORTALINTERFACE_P_H
