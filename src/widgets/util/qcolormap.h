// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#ifndef QCOLORMAP_H
#define QCOLORMAP_H

#include <BobUIWidgets/bobuiwidgetsglobal.h>
#include <BobUIGui/qrgb.h>
#include <BobUIGui/qwindowdefs.h>
#include <BobUICore/qatomic.h>
#include <BobUICore/qlist.h>

BOBUI_BEGIN_NAMESPACE

class QColor;
class QColormapPrivate;

#if BOBUI_REMOVAL_BOBUI7_DEPRECATED_SINCE(6, 11)
class Q_WIDGETS_EXPORT BOBUI_DEPRECATED_VERSION_6_11 QColormap
{
public:
    enum Mode { Direct, Indexed, Gray };

    static void initialize();
    static void cleanup();

    static QColormap instance(int screen = -1);

    QColormap(const QColormap &colormap);
    ~QColormap();

    QColormap &operator=(const QColormap &colormap);

    Mode mode() const;

    int depth() const;
    int size() const;

    uint pixel(const QColor &color) const;
    const QColor colorAt(uint pixel) const;

    const QList<QColor> colormap() const;

private:
    QColormap();
    QColormapPrivate *d;
};
#endif // BOBUI_REMOVAL_BOBUI7_DEPRECATED_SINCE(6, 11)

BOBUI_END_NAMESPACE

#endif // QCOLORMAP_H
