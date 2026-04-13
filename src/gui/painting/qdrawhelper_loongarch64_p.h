// Copyright (C) 2024 Loongson Technology Corporation Limited.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QDRAWHELPER_LOONGARCH64_P_H
#define QDRAWHELPER_LOONGARCH64_P_H

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

#include <BobUIGui/private/bobuiguiglobal_p.h>
#include <private/qdrawhelper_p.h>

BOBUI_BEGIN_NAMESPACE

#ifdef BOBUI_COMPILER_SUPPORTS_LSX
void bobui_memfill64_lsx(quint64 *dest, quint64 value, qsizetype count);
void bobui_memfill32_lsx(quint32 *dest, quint32 value, qsizetype count);
void bobui_bitmapblit32_lsx(QRasterBuffer *rasterBuffer, int x, int y,
                         const QRgba64 &color,
                         const uchar *src, int width, int height, int stride);
void bobui_bitmapblit8888_lsx(QRasterBuffer *rasterBuffer, int x, int y,
                           const QRgba64 &color,
                           const uchar *src, int width, int height, int stride);
void bobui_bitmapblit16_lsx(QRasterBuffer *rasterBuffer, int x, int y,
                         const QRgba64 &color,
                         const uchar *src, int width, int height, int stride);
void bobui_blend_argb32_on_argb32_lsx(uchar *destPixels, int dbpl,
                                   const uchar *srcPixels, int sbpl,
                                   int w, int h,
                                   int const_alpha);
void bobui_blend_rgb32_on_rgb32_lsx(uchar *destPixels, int dbpl,
                                 const uchar *srcPixels, int sbpl,
                                 int w, int h,
                                 int const_alpha);

#endif // BOBUI_COMPILER_SUPPORTS_LSX

#ifdef BOBUI_COMPILER_SUPPORTS_LASX
void bobui_memfill64_lasx(quint64 *dest, quint64 value, qsizetype count);
void bobui_memfill32_lasx(quint32 *dest, quint32 value, qsizetype count);
#endif // BOBUI_COMPILER_SUPPORTS_LASX

BOBUI_END_NAMESPACE

#endif // QDRAWHELPER_LOONGARCH64_P_H
