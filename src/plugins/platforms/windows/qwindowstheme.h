// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QWINDOWSTHEME_H
#define QWINDOWSTHEME_H

#include <qpa/qplatformtheme.h>

#include <BobUICore/qsharedpointer.h>
#include <BobUICore/qvariant.h>
#include <BobUICore/qlist.h>
#include <BobUICore/qsize.h>

BOBUI_BEGIN_NAMESPACE

class QWindow;

class QWindowsTheme : public QPlatformTheme
{
    Q_DISABLE_COPY_MOVE(QWindowsTheme)
public:
    QWindowsTheme();
    ~QWindowsTheme() override;

    static QWindowsTheme *instance() { return m_instance; }

    bool usePlatformNativeDialog(DialogType type) const override;
    QPlatformDialogHelper *createPlatformDialogHelper(DialogType type) const override;
#if BOBUI_CONFIG(systemtrayicon)
    QPlatformSystemTrayIcon *createPlatformSystemTrayIcon() const override;
#endif
    QVariant themeHint(ThemeHint) const override;

    BobUI::ColorScheme colorScheme() const override;
    void requestColorScheme(BobUI::ColorScheme scheme) override;
    BobUI::ContrastPreference contrastPreference() const override;

    static void handleThemeChange();

    const QPalette *palette(Palette type = SystemPalette) const override
        { return m_palettes[type]; }
    const QFont *font(Font type = SystemFont) const override
        { return m_fonts[type]; }

    QPixmap standardPixmap(StandardPixmap sp, const QSizeF &size) const override;

    QIcon fileIcon(const QFileInfo &fileInfo, QPlatformTheme::IconOptions iconOptions = {}) const override;
    QIconEngine *createIconEngine(const QString &iconName) const override;

    void displayChanged() { refreshIconPixmapSizes(); }

    QList<QSize> availableFileIconSizes() const { return m_fileIconSizes; }

    QPlatformMenuItem *createPlatformMenuItem() const override;
    QPlatformMenu *createPlatformMenu() const override;
    QPlatformMenuBar *createPlatformMenuBar() const override;
    void showPlatformMenuBar() override;

    static bool useNativeMenus();

    void refreshFonts();
    void refresh();

    static const char *name;

    static QPalette systemPalette(BobUI::ColorScheme);
    static bool queryHighContrast();
private:
    void clearPalettes();
    void refreshPalettes();
    void clearFonts();
    void refreshIconPixmapSizes();

    static void populateLightSystemBasePalette(QPalette &result);
    static void populateDarkSystemBasePalette(QPalette &result);

    static BobUI::ColorScheme queryColorScheme();
    static BobUI::ColorScheme effectiveColorScheme();

    static QWindowsTheme *m_instance;
    static inline BobUI::ColorScheme s_colorScheme = BobUI::ColorScheme::Unknown;
    static inline BobUI::ColorScheme s_colorSchemeOverride = BobUI::ColorScheme::Unknown;

    friend class QWindowsContext;

    QPalette *m_palettes[NPalettes];
    QFont *m_fonts[NFonts];
    QList<QSize> m_fileIconSizes;

    HWND m_themeChangeObserver = nullptr;
    void destroyThemeChangeWindow();
};

BOBUI_END_NAMESPACE

#endif // QWINDOWSTHEME_H
