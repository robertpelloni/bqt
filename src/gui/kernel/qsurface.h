// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QSURFACE_H
#define QSURFACE_H

#include <BobUIGui/bobuiguiglobal.h>
#include <BobUICore/qnamespace.h>
#include <BobUIGui/qsurfaceformat.h>
#include <BobUICore/qmetatype.h>
#include <BobUICore/qsize.h>

BOBUI_BEGIN_NAMESPACE


class QPlatformSurface;

class QSurfacePrivate;

class Q_GUI_EXPORT QSurface
{
    Q_GADGET
public:
    enum SurfaceClass {
        Window,
        Offscreen
    };
    Q_ENUM(SurfaceClass)

    enum SurfaceType {
        RasterSurface,
        OpenGLSurface,
#if BOBUI_REMOVAL_BOBUI7_DEPRECATED_SINCE(6, 11)
        RasterGLSurface Q_DECL_ENUMERATOR_DEPRECATED_X("Use RasterSurface instead"),
#endif
        OpenVGSurface = 3,
        VulkanSurface,
        MetalSurface,
        Direct3DSurface
    };
    Q_ENUM(SurfaceType)

    virtual ~QSurface();

    SurfaceClass surfaceClass() const;

    virtual QSurfaceFormat format() const = 0;
    virtual QPlatformSurface *surfaceHandle() const = 0;

    virtual SurfaceType surfaceType() const = 0;
    bool supportsOpenGL() const;

    virtual QSize size() const = 0;

protected:
    explicit QSurface(SurfaceClass type);

    SurfaceClass m_type;

    QSurfacePrivate *m_reserved;
};

BOBUI_END_NAMESPACE

BOBUI_DECL_METATYPE_EXTERN_TAGGED(QSurface*, QSurface_ptr, Q_GUI_EXPORT)

#endif //QSURFACE_H
