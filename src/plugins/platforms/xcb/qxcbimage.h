// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#pragma once

#include "qxcbscreen.h"
#include <BobUIGui/QImage>
#include <BobUIGui/QPixmap>
#include <xcb/xcb_image.h>

BOBUI_BEGIN_NAMESPACE

bool bobui_xcb_imageFormatForVisual(QXcbConnection *connection, uint8_t depth, const xcb_visualtype_t *visual,
                                 QImage::Format *imageFormat, bool *needsRgbSwap = nullptr);
QPixmap bobui_xcb_pixmapFromXPixmap(QXcbConnection *connection, xcb_pixmap_t pixmap,
                                 int width, int height, int depth,
                                 const xcb_visualtype_t *visual);
xcb_pixmap_t bobui_xcb_XPixmapFromBitmap(QXcbScreen *screen, const QImage &image);
xcb_cursor_t bobui_xcb_createCursorXRender(QXcbScreen *screen, const QImage &image,
                                        const QPoint &spot);

BOBUI_END_NAMESPACE
