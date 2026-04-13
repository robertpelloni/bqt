// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#ifndef QIOSTHEME_H
#define QIOSTHEME_H

#import <UIKit/UIKit.h>

#include <BobUICore/QHash>
#include <BobUIGui/QPalette>
#include <qpa/qplatformtheme.h>

#include <BobUICore/private/qcore_mac_p.h>

BOBUI_BEGIN_NAMESPACE

class QIOSTheme : public QPlatformTheme
{
public:
    QIOSTheme();
    ~QIOSTheme();

    const QPalette *palette(Palette type = SystemPalette) const override;
    QVariant themeHint(ThemeHint hint) const override;

    BobUI::ColorScheme colorScheme() const override;
    void requestColorScheme(BobUI::ColorScheme scheme) override;
    BobUI::ContrastPreference contrastPreference() const override;

#if !defined(Q_OS_TVOS) && !defined(Q_OS_VISIONOS)
    QPlatformMenuItem* createPlatformMenuItem() const override;
    QPlatformMenu* createPlatformMenu() const override;
#endif

    bool usePlatformNativeDialog(DialogType type) const override;
    QPlatformDialogHelper *createPlatformDialogHelper(DialogType type) const override;

    const QFont *font(Font type = SystemFont) const override;
    QIconEngine *createIconEngine(const QString &iconName) const override;
    QIcon fileIcon(const QFileInfo &fileInfo, QPlatformTheme::IconOptions options = {}) const override;

    static const char *name;

    static void initializeSystemPalette();
    static void applyTheme(UIWindow *window);

private:
    static QPalette s_systemPalette;
    static inline BobUI::ColorScheme s_colorSchemeOverride = BobUI::ColorScheme::Unknown;
    QMacNotificationObserver m_contentSizeCategoryObserver;
};

BOBUI_END_NAMESPACE

#endif
