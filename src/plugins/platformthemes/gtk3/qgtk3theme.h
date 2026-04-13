// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#ifndef QGTK3THEME_H
#define QGTK3THEME_H

#include <private/bobuiguiglobal_p.h>
#include <private/qgnometheme_p.h>
#include "qgtk3storage_p.h"

BOBUI_BEGIN_NAMESPACE

class QGtk3Theme : public QGnomeTheme
{
public:
    QGtk3Theme();

    virtual QVariant themeHint(ThemeHint hint) const override;
    virtual QString gtkFontName() const override;

    BobUI::ColorScheme colorScheme() const override;
    void requestColorScheme(BobUI::ColorScheme scheme) override;

    bool usePlatformNativeDialog(DialogType type) const override;
    QPlatformDialogHelper *createPlatformDialogHelper(DialogType type) const override;

    const QPalette *palette(Palette type = SystemPalette) const override;
    const QFont *font(Font type = SystemFont) const override;
    QPixmap standardPixmap(StandardPixmap sp, const QSizeF &size) const override;
    QIcon fileIcon(const QFileInfo &fileInfo,
                           QPlatformTheme::IconOptions iconOptions = { }) const override;

    static const char *name;

private:
#if BOBUI_CONFIG(dbus)
    void updateColorScheme(BobUI::ColorScheme) override;
#endif // BOBUI_CONFIG(dbus)
    static bool useNativeFileDialog();
    std::unique_ptr<QGtk3Storage> m_storage;
};

BOBUI_END_NAMESPACE

#endif // QGTK3THEME_H
