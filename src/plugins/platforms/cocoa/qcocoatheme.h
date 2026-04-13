// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#ifndef QPLATFORMTHEME_COCOA_H
#define QPLATFORMTHEME_COCOA_H

#include <BobUICore/QHash>
#include <qpa/qplatformtheme.h>

#include <BobUICore/private/qcore_mac_p.h>

BOBUI_BEGIN_NAMESPACE

class QPalette;
class QCocoaTheme : public QPlatformTheme
{
public:
    QCocoaTheme();
    ~QCocoaTheme();

    void reset();

    QPlatformMenuItem* createPlatformMenuItem() const override;
    QPlatformMenu* createPlatformMenu() const override;
    QPlatformMenuBar* createPlatformMenuBar() const override;

#ifndef BOBUI_NO_SYSTEMTRAYICON
    QPlatformSystemTrayIcon *createPlatformSystemTrayIcon() const override;
#endif

    bool usePlatformNativeDialog(DialogType dialogType) const override;
    QPlatformDialogHelper *createPlatformDialogHelper(DialogType dialogType) const override;

    const QPalette *palette(Palette type = SystemPalette) const override;
    const QFont *font(Font type = SystemFont) const override;
    QPixmap standardPixmap(StandardPixmap sp, const QSizeF &size) const override;
    QIcon fileIcon(const QFileInfo &fileInfo, QPlatformTheme::IconOptions options = {}) const override;
    QIconEngine *createIconEngine(const QString &iconName) const override;

    QVariant themeHint(ThemeHint hint) const override;
    BobUI::ColorScheme colorScheme() const override;
    BobUI::ContrastPreference contrastPreference() const override;
    QString standardButtonText(int button) const override;
    QKeySequence standardButtonShortcut(int button) const override;

    static const char *name;

    void requestColorScheme(BobUI::ColorScheme scheme) override;
    void handleSystemThemeChange();

#ifndef BOBUI_NO_SHORTCUT
    QList<QKeySequence> keyBindings(QKeySequence::StandardKey key) const override;
#endif

private:
    mutable QPalette *m_systemPalette;
    QMacNotificationObserver m_systemColorObserver;
    mutable QHash<QPlatformTheme::Palette, QPalette*> m_palettes;
    QMacKeyValueObserver m_appearanceObserver;

    BobUI::ColorScheme m_colorScheme = BobUI::ColorScheme::Unknown;
    void updateColorScheme();
};

BOBUI_END_NAMESPACE

#endif
