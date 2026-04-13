// Copyright (C) 2017 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#pragma once

#if defined(VULKAN_H_) && !defined(VK_USE_PLATFORM_XCB_KHR)
#error "vulkan.h included without xcb WSI"
#endif

#define VK_USE_PLATFORM_XCB_KHR

#include <BobUIGui/private/qbasicvulkanplatforminstance_p.h>
#include <QLibrary>

BOBUI_BEGIN_NAMESPACE

class QXcbWindow;

class QXcbVulkanInstance : public QBasicPlatformVulkanInstance
{
public:
    QXcbVulkanInstance(QVulkanInstance *instance);
    ~QXcbVulkanInstance();

    void createOrAdoptInstance() override;
    bool supportsPresent(VkPhysicalDevice physicalDevice, uint32_t queueFamilyIndex, QWindow *window) override;
    void presentQueued(QWindow *window) override;

    VkSurfaceKHR createSurface(QXcbWindow *window);

private:
    QVulkanInstance *m_instance;
    PFN_vkGetPhysicalDeviceXcbPresentationSupportKHR m_getPhysDevPresSupport;
    PFN_vkCreateXcbSurfaceKHR m_createSurface;
};

BOBUI_END_NAMESPACE
