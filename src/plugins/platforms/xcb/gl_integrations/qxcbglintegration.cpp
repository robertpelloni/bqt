// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "qxcbglintegration.h"

BOBUI_BEGIN_NAMESPACE

Q_LOGGING_CATEGORY(lcQpaGl, "bobui.qpa.gl")

QXcbGlIntegration::QXcbGlIntegration()
{
}
QXcbGlIntegration::~QXcbGlIntegration()
{
}

bool QXcbGlIntegration::handleXcbEvent(xcb_generic_event_t *event, uint responseType)
{
    Q_UNUSED(event);
    Q_UNUSED(responseType);
    return false;
}

BOBUI_END_NAMESPACE
