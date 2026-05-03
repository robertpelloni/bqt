// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#include "qiostheme.h"

#include <BobUICore/QStringList>
#include <BobUICore/QVariant>
#include <BobUICore/private/qcore_mac_p.h>

#include <BobUIGui/QFont>
#include <BobUIGui/private/qcoregraphics_p.h>

#include <BobUIGui/private/qcoretextfontdatabase_p.h>
#include <BobUIGui/private/qapplefileiconengine_p.h>
#include <BobUIGui/private/qappleiconengine_p.h>
#include <BobUIGui/private/qguiapplication_p.h>
#include <qpa/qplatformintegration.h>

#include <UIKit/UIFont.h>
#include <UIKit/UIInterface.h>

#if !defined(Q_OS_TVOS) && !defined(Q_OS_VISIONOS)
#include "qiosmenu.h"
#endif

#if !defined(Q_OS_TVOS)
#include "qiosfiledialog.h"
#include "qioscolordialog.h"
#include "qiosfontdialog.h"
#include "qiosmessagedialog.h"
#include "qiosscreen.h"
#include "quiwindow.h"
#endif

BOBUI_BEGIN_NAMESPACE

const char *QIOSTheme::name = "ios";

QIOSTheme::QIOSTheme()
{
    initializeSystemPalette();

    m_contentSizeCategoryObserver = QMacNotificationObserver(nil,
        UIContentSizeCategoryDidChangeNotification, [] {
        qCDebug(lcQpaFonts) << "Contents size category changed to" << UIApplication.sharedApplication.preferredContentSizeCategory;
        QPlatformFontDatabase::repopulateFontDatabase();
    });
}

QIOSTheme::~QIOSTheme()
{
}

QPalette QIOSTheme::s_systemPalette;

void QIOSTheme::initializeSystemPalette()
{
    Q_DECL_IMPORT QPalette bobui_fusionPalette(void);
    s_systemPalette = bobui_fusionPalette();

    const auto disabledText = bobui_mac_toQBrush(UIColor.tertiaryLabelColor.CGColor);

    s_systemPalette.setBrush(QPalette::Window, bobui_mac_toQBrush(UIColor.systemGroupedBackgroundColor.CGColor));
    s_systemPalette.setBrush(QPalette::Active, QPalette::WindowText, bobui_mac_toQBrush(UIColor.labelColor.CGColor));
    s_systemPalette.setBrush(QPalette::Disabled, QPalette::WindowText, disabledText);

    s_systemPalette.setBrush(QPalette::Base, bobui_mac_toQBrush(UIColor.secondarySystemGroupedBackgroundColor.CGColor));
    s_systemPalette.setBrush(QPalette::Active, QPalette::Text, bobui_mac_toQBrush(UIColor.labelColor.CGColor));
    s_systemPalette.setBrush(QPalette::Disabled, QPalette::Text, disabledText);

    s_systemPalette.setBrush(QPalette::Button, bobui_mac_toQBrush(UIColor.secondarySystemBackgroundColor.CGColor));
    s_systemPalette.setBrush(QPalette::Active, QPalette::ButtonText, bobui_mac_toQBrush(UIColor.labelColor.CGColor));
    s_systemPalette.setBrush(QPalette::Disabled, QPalette::ButtonText, disabledText);

    s_systemPalette.setBrush(QPalette::Active, QPalette::BrightText, bobui_mac_toQBrush(UIColor.lightTextColor.CGColor));
    s_systemPalette.setBrush(QPalette::Active, QPalette::PlaceholderText, bobui_mac_toQBrush(UIColor.placeholderTextColor.CGColor));

    s_systemPalette.setBrush(QPalette::Active, QPalette::Link, bobui_mac_toQBrush(UIColor.linkColor.CGColor));
    s_systemPalette.setBrush(QPalette::Active, QPalette::LinkVisited, bobui_mac_toQBrush(UIColor.linkColor.CGColor));

    s_systemPalette.setBrush(QPalette::Highlight, QColor(11, 70, 150, 60));
    s_systemPalette.setBrush(QPalette::HighlightedText, bobui_mac_toQBrush(UIColor.labelColor.CGColor));

    s_systemPalette.setBrush(QPalette::ToolTipText, bobui_mac_toQBrush(UIColor.labelColor.CGColor));
    s_systemPalette.setBrush(QPalette::Disabled, QPalette::ToolTipText, disabledText);

    if (@available(ios 15.0, *))
        s_systemPalette.setBrush(QPalette::Accent, bobui_mac_toQBrush(UIColor.tintColor.CGColor));
}

const QPalette *QIOSTheme::palette(QPlatformTheme::Palette type) const
{
    if (type == QPlatformTheme::SystemPalette)
        return &s_systemPalette;
    return 0;
}

#if !defined(Q_OS_TVOS) && !defined(Q_OS_VISIONOS)
QPlatformMenuItem* QIOSTheme::createPlatformMenuItem() const
{
    return new QIOSMenuItem;
}

QPlatformMenu* QIOSTheme::createPlatformMenu() const
{
    return new QIOSMenu;
}
#endif

bool QIOSTheme::usePlatformNativeDialog(QPlatformTheme::DialogType type) const
{
    switch (type) {
    case FileDialog:
    case MessageDialog:
    case ColorDialog:
    case FontDialog:
        return !bobui_apple_isApplicationExtension();
    default:
        return false;
    }
}

QPlatformDialogHelper *QIOSTheme::createPlatformDialogHelper(QPlatformTheme::DialogType type) const
{
    switch (type) {
#ifndef Q_OS_TVOS
    case FileDialog:
        return new QIOSFileDialog();
        break;
    case MessageDialog:
        return new QIOSMessageDialog();
        break;
    case ColorDialog:
        return new QIOSColorDialog();
        break;
    case FontDialog:
        return new QIOSFontDialog();
        break;
#endif
    default:
        return 0;
    }
}

QVariant QIOSTheme::themeHint(ThemeHint hint) const
{
    switch (hint) {
    case QPlatformTheme::StyleNames:
        return QStringList(QStringLiteral("Fusion"));
    case KeyboardScheme:
        return QVariant(int(MacKeyboardScheme));
    case PreferFileIconFromTheme:
        return true;
    default:
        return QPlatformTheme::themeHint(hint);
    }
}

BobUI::ColorScheme QIOSTheme::colorScheme() const
{
#if defined(Q_OS_VISIONOS)
    // On visionOS the concept of light or dark mode does not
    // apply, as the UI is constantly changing based on what
    // the lighting conditions are outside the headset, but
    // the OS reports itself as always being in dark mode.
    return BobUI::ColorScheme::Dark;
#else
    if (s_colorSchemeOverride != BobUI::ColorScheme::Unknown)
        return s_colorSchemeOverride;

    // Set the appearance based on the QUIWindow
    // Fallback to the UIScreen if no window is created yet
    UIUserInterfaceStyle appearance = UIScreen.mainScreen.traitCollection.userInterfaceStyle;
    NSArray<UIWindow *> *windows = bobui_apple_sharedApplication().windows;
    for (UIWindow *window in windows) {
        if ([window isKindOfClass:[QUIWindow class]]) {
            appearance = static_cast<QUIWindow*>(window).traitCollection.userInterfaceStyle;
            break;
        }
    }

    return appearance == UIUserInterfaceStyleDark
                       ? BobUI::ColorScheme::Dark
                       : BobUI::ColorScheme::Light;
#endif
}

void QIOSTheme::requestColorScheme(BobUI::ColorScheme scheme)
{
#if defined(Q_OS_VISIONOS)
    Q_UNUSED(scheme);
#else
    s_colorSchemeOverride = scheme;

    const NSArray<UIWindow *> *windows = bobui_apple_sharedApplication().windows;
    for (UIWindow *window in windows) {
        // don't apply a theme to windows we don't own
        if (bobui_objc_cast<QUIWindow*>(window))
            applyTheme(window);
    }
#endif
}

BobUI::ContrastPreference QIOSTheme::contrastPreference() const
{
    return UIAccessibilityDarkerSystemColorsEnabled() ? BobUI::ContrastPreference::HighContrast : BobUI::ContrastPreference::NoPreference;
}


void QIOSTheme::applyTheme(UIWindow *window)
{
    const UIUserInterfaceStyle style = []{
        switch (s_colorSchemeOverride) {
        case BobUI::ColorScheme::Dark:
            return UIUserInterfaceStyleDark;
        case BobUI::ColorScheme::Light:
            return UIUserInterfaceStyleLight;
        case BobUI::ColorScheme::Unknown:
            return UIUserInterfaceStyleUnspecified;
        }
    }();

    window.overrideUserInterfaceStyle = style;
}

const QFont *QIOSTheme::font(Font type) const
{
    const auto *platformIntegration = QGuiApplicationPrivate::platformIntegration();
    const auto *coreTextFontDatabase = static_cast<QCoreTextFontDatabase *>(platformIntegration->fontDatabase());
    return coreTextFontDatabase->themeFont(type);
}

QIconEngine *QIOSTheme::createIconEngine(const QString &iconName) const
{
    return new QAppleIconEngine(iconName);
}

QIcon QIOSTheme::fileIcon(const QFileInfo &fileInfo, QPlatformTheme::IconOptions iconOptions) const
{
    return QIcon(new QAppleFileIconEngine(fileInfo, iconOptions));
}


BOBUI_END_NAMESPACE
