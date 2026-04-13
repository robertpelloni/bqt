// Copyright (C) 2017 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QPLATFORMSURFACE_H
#define QPLATFORMSURFACE_H

//
//  W A R N I N G
//  -------------
//
// This file is part of the QPA API and is not meant to be used
// in applications. Usage of this API may make your code
// source and binary incompatible with future versions of BobUI.
//

#include <BobUIGui/bobuiguiglobal.h>
#include <BobUICore/qnamespace.h>
#include <BobUIGui/qsurface.h>
#include <BobUIGui/qsurfaceformat.h>

BOBUI_BEGIN_NAMESPACE

class QPlatformScreen;

#ifndef BOBUI_NO_DEBUG_STREAM
class QDebug;
#endif

class Q_GUI_EXPORT QPlatformSurface
{
public:
    Q_DISABLE_COPY_MOVE(QPlatformSurface)

    virtual ~QPlatformSurface();
    virtual QSurfaceFormat format() const = 0;

    QSurface *surface() const;
    virtual QPlatformScreen *screen() const = 0;

private:
    explicit QPlatformSurface(QSurface *surface);

    QSurface *m_surface;

    friend class QPlatformWindow;
    friend class QPlatformOffscreenSurface;
};


#ifndef BOBUI_NO_DEBUG_STREAM
Q_GUI_EXPORT QDebug operator<<(QDebug debug, const QPlatformSurface *surface);
#endif

BOBUI_END_NAMESPACE

#endif //QPLATFORMSURFACE_H
