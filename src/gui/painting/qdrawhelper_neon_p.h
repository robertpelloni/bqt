// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QDRAWHELPER_NEON_P_H
#define QDRAWHELPER_NEON_P_H

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

#include <private/qdrawhelper_p.h>

BOBUI_BEGIN_NAMESPACE

#ifdef __ARM_NEON__

void bobui_blend_argb32_on_argb32_neon(uchar *destPixels, int dbpl,
                                            const uchar *srcPixels, int sbpl,
                                            int w, int h,
                                            int const_alpha);

void bobui_blend_rgb32_on_rgb32_neon(uchar *destPixels, int dbpl,
                                  const uchar *srcPixels, int sbpl,
                                  int w, int h,
                                  int const_alpha);

void bobui_blend_argb32_on_rgb16_neon(uchar *destPixels, int dbpl,
                                   const uchar *srcPixels, int sbpl,
                                   int w, int h,
                                   int const_alpha);

void bobui_blend_argb32_on_argb32_scanline_neon(uint *dest,
                                             const uint *src,
                                             int length,
                                             uint const_alpha);

void bobui_blend_rgb16_on_argb32_neon(uchar *destPixels, int dbpl,
                                   const uchar *srcPixels, int sbpl,
                                   int w, int h,
                                   int const_alpha);

void bobui_blend_rgb16_on_rgb16_neon(uchar *destPixels, int dbpl,
                                  const uchar *srcPixels, int sbpl,
                                  int w, int h,
                                  int const_alpha);

void bobui_alphamapblit_quint16_neon(QRasterBuffer *rasterBuffer,
                                  int x, int y, const QRgba64 &color,
                                  const uchar *bitmap,
                                  int mapWidth, int mapHeight, int mapStride,
                                  const QClipData *clip, bool /*useGammaCorrection*/);

void bobui_scale_image_argb32_on_rgb16_neon(uchar *destPixels, int dbpl,
                                         const uchar *srcPixels, int sbpl, int srch,
                                         const QRectF &targetRect,
                                         const QRectF &sourceRect,
                                         const QRect &clip,
                                         int const_alpha);

void bobui_scale_image_rgb16_on_rgb16_neon(uchar *destPixels, int dbpl,
                                        const uchar *srcPixels, int sbpl, int srch,
                                        const QRectF &targetRect,
                                        const QRectF &sourceRect,
                                        const QRect &clip,
                                        int const_alpha);

void bobui_transform_image_argb32_on_rgb16_neon(uchar *destPixels, int dbpl,
                                             const uchar *srcPixels, int sbpl,
                                             const QRectF &targetRect,
                                             const QRectF &sourceRect,
                                             const QRect &clip,
                                             const BOBUIransform &targetRectTransform,
                                             int const_alpha);

void bobui_transform_image_rgb16_on_rgb16_neon(uchar *destPixels, int dbpl,
                                            const uchar *srcPixels, int sbpl,
                                            const QRectF &targetRect,
                                            const QRectF &sourceRect,
                                            const QRect &clip,
                                            const BOBUIransform &targetRectTransform,
                                            int const_alpha);

void bobui_memfill32_neon(quint32 *dest, quint32 value, qsizetype count);
void bobui_memrotate90_16_neon(const uchar *srcPixels, int w, int h, int sbpl, uchar *destPixels, int dbpl);
void bobui_memrotate270_16_neon(const uchar *srcPixels, int w, int h, int sbpl, uchar *destPixels, int dbpl);

uint * BOBUI_FASTCALL bobui_destFetchRGB16_neon(uint *buffer,
                                          QRasterBuffer *rasterBuffer,
                                          int x, int y, int length);

void BOBUI_FASTCALL bobui_destStoreRGB16_neon(QRasterBuffer *rasterBuffer,
                                        int x, int y, const uint *buffer, int length);

void BOBUI_FASTCALL comp_func_solid_SourceOver_neon(uint *destPixels, int length, uint color, uint const_alpha);
void BOBUI_FASTCALL comp_func_Plus_neon(uint *dst, const uint *src, int length, uint const_alpha);

const uint * BOBUI_FASTCALL bobui_fetchUntransformed_888_neon(uint *buffer, const Operator *, const QSpanData *data,
                                                       int y, int x, int length);

#endif // __ARM_NEON__

BOBUI_END_NAMESPACE

#endif // QDRAWHELPER_NEON_P_H
