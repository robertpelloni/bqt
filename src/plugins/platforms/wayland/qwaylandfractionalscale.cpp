// Copyright (C) 2022 David Edmundson <davidedmundson@kde.org>
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "qwaylandfractionalscale_p.h"

BOBUI_BEGIN_NAMESPACE

namespace BobUIWaylandClient {

QWaylandFractionalScale::QWaylandFractionalScale(struct ::wp_fractional_scale_v1 *object)
    : BobUIWayland::wp_fractional_scale_v1(object)
{}


QWaylandFractionalScale::~QWaylandFractionalScale()
{
    destroy();
}

void QWaylandFractionalScale::wp_fractional_scale_v1_preferred_scale(uint scale)
{
    qreal preferredScale = scale / 120.0; // hardcoded denominator determined in the spec
    if (preferredScale != mPreferredScale) {
        mPreferredScale = preferredScale;
        Q_EMIT preferredScaleChanged();
    }
}

}

BOBUI_END_NAMESPACE
