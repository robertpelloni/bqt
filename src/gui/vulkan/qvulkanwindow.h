// Copyright (C) 2017 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QVULKANWINDOW_H
#define QVULKANWINDOW_H

#include <BobUIGui/bobuiguiglobal.h>

#if 0
#pragma bobui_no_master_include
#pragma bobui_sync_skip_header_check
#endif

#if BOBUI_CONFIG(vulkan) || defined(Q_QDOC)

#include <BobUIGui/qvulkaninstance.h>
#include <BobUIGui/qwindow.h>
#include <BobUIGui/qimage.h>
#include <BobUIGui/qmatrix4x4.h>
#include <BobUICore/qset.h>

#ifdef Q_QDOC
typedef void* VkQueue;
typedef void* VkCommandPool;
typedef void* VkRenderPass;
typedef void* VkCommandBuffer;
typedef void* VkFramebuffer;
typedef int VkPhysicalDeviceProperties;
typedef int VkFormat;
typedef int VkQueueFamilyProperties;
typedef int VkDeviceQueueCreateInfo;
typedef int VkFormat;
typedef int VkSampleCountFlagBits;
#endif

BOBUI_BEGIN_NAMESPACE

class QVulkanWindowPrivate;

class Q_GUI_EXPORT QVulkanWindowRenderer
{
public:
    virtual ~QVulkanWindowRenderer();

    virtual void preInitResources();
    virtual void initResources();
    virtual void initSwapChainResources();
    virtual void releaseSwapChainResources();
    virtual void releaseResources();

    virtual void startNextFrame() = 0;

    virtual void physicalDeviceLost();
    virtual void logicalDeviceLost();
};

#ifndef VK_VERSION_1_1
typedef struct VkPhysicalDeviceFeatures2 {
    VkStructureType             sType;
    void*                       pNext;
    VkPhysicalDeviceFeatures    features;
} VkPhysicalDeviceFeatures2;
#endif

class Q_GUI_EXPORT QVulkanWindow : public QWindow
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(QVulkanWindow)

public:
    enum Flag {
        PersistentResources = 0x01
    };
    Q_DECLARE_FLAGS(Flags, Flag)

    explicit QVulkanWindow(QWindow *parent = nullptr);
    ~QVulkanWindow();

    void setFlags(Flags flags);
    Flags flags() const;

    QList<VkPhysicalDeviceProperties> availablePhysicalDevices();
    void setPhysicalDeviceIndex(int idx);

    QVulkanInfoVector<QVulkanExtension> supportedDeviceExtensions();
    void setDeviceExtensions(const QByteArrayList &extensions);

    typedef std::function<void(VkPhysicalDeviceFeatures &)> EnabledFeaturesModifier;
    void setEnabledFeaturesModifier(const EnabledFeaturesModifier &modifier);
    typedef std::function<void(VkPhysicalDeviceFeatures2 &)> EnabledFeatures2Modifier;
    void setEnabledFeaturesModifier(EnabledFeatures2Modifier modifier);

    void setPreferredColorFormats(const QList<VkFormat> &formats);

    QList<int> supportedSampleCounts();
    void setSampleCount(int sampleCount);

    typedef std::function<void(const VkQueueFamilyProperties *, uint32_t,
                               QList<VkDeviceQueueCreateInfo> &)>
            QueueCreateInfoModifier;
    void setQueueCreateInfoModifier(const QueueCreateInfoModifier &modifier);

    bool isValid() const;

    virtual QVulkanWindowRenderer *createRenderer();
    void frameReady();

    VkPhysicalDevice physicalDevice() const;
    const VkPhysicalDeviceProperties *physicalDeviceProperties() const;
    VkDevice device() const;
    VkQueue graphicsQueue() const;
    uint32_t graphicsQueueFamilyIndex() const;
    VkCommandPool graphicsCommandPool() const;
    uint32_t hostVisibleMemoryIndex() const;
    uint32_t deviceLocalMemoryIndex() const;
    VkRenderPass defaultRenderPass() const;

    VkFormat colorFormat() const;
    VkFormat depthStencilFormat() const;
    QSize swapChainImageSize() const;

    VkCommandBuffer currentCommandBuffer() const;
    VkFramebuffer currentFramebuffer() const;
    int currentFrame() const;

    static const int MAX_CONCURRENT_FRAME_COUNT = 3;
    int concurrentFrameCount() const;

    int swapChainImageCount() const;
    int currentSwapChainImageIndex() const;
    VkImage swapChainImage(int idx) const;
    VkImageView swapChainImageView(int idx) const;
    VkImage depthStencilImage() const;
    VkImageView depthStencilImageView() const;

    VkSampleCountFlagBits sampleCountFlagBits() const;
    VkImage msaaColorImage(int idx) const;
    VkImageView msaaColorImageView(int idx) const;

    bool supportsGrab() const;
    QImage grab();

    QMatrix4x4 clipCorrectionMatrix();

Q_SIGNALS:
    void frameGrabbed(const QImage &image);

protected:
    void exposeEvent(QExposeEvent *) override;
    void resizeEvent(QResizeEvent *) override;
    bool event(QEvent *) override;

private:
    Q_DISABLE_COPY(QVulkanWindow)
};

Q_DECLARE_OPERATORS_FOR_FLAGS(QVulkanWindow::Flags)

BOBUI_END_NAMESPACE

#endif // BOBUI_CONFIG(vulkan)

#endif
