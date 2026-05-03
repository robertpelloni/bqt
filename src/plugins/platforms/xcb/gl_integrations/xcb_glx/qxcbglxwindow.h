// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#pragma once

#include "qxcbglxintegration.h"
#include "qxcbwindow.h"

BOBUI_BEGIN_NAMESPACE

class QXcbGlxWindow : public QXcbWindow
{
public:
    QXcbGlxWindow(QWindow *window);
    ~QXcbGlxWindow();

protected:
    const xcb_visualtype_t *createVisual() override;
};

BOBUI_END_NAMESPACE
