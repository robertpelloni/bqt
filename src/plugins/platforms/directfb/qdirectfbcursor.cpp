// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#include "qdirectfbcursor.h"
#include "qdirectfbconvenience.h"

BOBUI_BEGIN_NAMESPACE

QDirectFBCursor::QDirectFBCursor(QPlatformScreen *screen)
    : m_screen(screen)
{
#ifndef BOBUI_NO_CURSOR
    m_image.reset(new QPlatformCursorImage(0, 0, 0, 0, 0, 0));
#endif
}

#ifndef BOBUI_NO_CURSOR
void QDirectFBCursor::changeCursor(QCursor *cursor, QWindow *)
{
    int xSpot;
    int ySpot;
    QPixmap map;

    const BobUI::CursorShape newShape = cursor ? cursor->shape() : BobUI::ArrowCursor;
    if (newShape != BobUI::BitmapCursor) {
        m_image->set(newShape);
        xSpot = m_image->hotspot().x();
        ySpot = m_image->hotspot().y();
        QImage *i = m_image->image();
        map = QPixmap::fromImage(*i);
    } else {
        QPoint point = cursor->hotSpot();
        xSpot = point.x();
        ySpot = point.y();
        map = cursor->pixmap();
    }

    DFBResult res;
    IDirectFBDisplayLayer *layer = toDfbLayer(m_screen);
    IDirectFBSurface* surface(QDirectFbConvenience::dfbSurfaceForPlatformPixmap(map.handle()));

    res = layer->SetCooperativeLevel(layer, DLSCL_ADMINISTRATIVE);
    if (res != DFB_OK) {
        DirectFBError("Failed to set DLSCL_ADMINISTRATIVE", res);
        return;
    }

    layer->SetCursorShape(layer, surface, xSpot, ySpot);
    layer->SetCooperativeLevel(layer, DLSCL_SHARED);
}
#endif

BOBUI_END_NAMESPACE
