// Copyright (C) 2017 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#pragma once

#include "qxcbwindow.h"
#include "qxcbvulkaninstance.h"

BOBUI_BEGIN_NAMESPACE

class QXcbVulkanWindow : public QXcbWindow
{
public:
    QXcbVulkanWindow(QWindow *window);
    ~QXcbVulkanWindow();

    VkSurfaceKHR *surface();

protected:
    void resolveFormat(const QSurfaceFormat &format) override;

private:
    VkSurfaceKHR m_surface;
};

BOBUI_END_NAMESPACE
