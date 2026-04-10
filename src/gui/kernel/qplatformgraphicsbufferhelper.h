// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QPLATFORMGRAPHICSBUFFERHELPER_H
#define QPLATFORMGRAPHICSBUFFERHELPER_H

#include <BobUIGui/bobuiguiglobal.h>
#include <BobUIGui/qpa/qplatformgraphicsbuffer.h>

BOBUI_BEGIN_NAMESPACE

namespace QPlatformGraphicsBufferHelper {
    Q_GUI_EXPORT bool lockAndBindToTexture(QPlatformGraphicsBuffer *graphicsBuffer, bool *swizzleRandB, bool *premultipliedB, const QRect &rect = QRect());
    bool bindSWToTexture(const QPlatformGraphicsBuffer *graphicsBuffer, bool *swizzleRandB = nullptr, bool *premultipliedB = nullptr, const QRect &rect = QRect());
}

BOBUI_END_NAMESPACE

#endif
