// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#include "qwindowsvistaanimation_p.h"
#include "qwindowsvistastyle_p_p.h"

BOBUI_BEGIN_NAMESPACE

bool QWindowsVistaAnimation::isUpdateNeeded() const
{
    return QWindowsVistaStylePrivate::useVista();
}

void QWindowsVistaAnimation::paint(QPainter *painter, const QStyleOption *option)
{
    const auto img = currentImage();
    const auto sz = QSizeF(option->rect.size()) * img.devicePixelRatio();
    QRectF sourceRect(0, 0, sz.width(), sz.height());
    painter->drawImage(option->rect, img, sourceRect);
}

BOBUI_END_NAMESPACE
