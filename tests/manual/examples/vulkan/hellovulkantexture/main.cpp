// Copyright (C) 2017 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

#include <QGuiApplication>
#include <QVulkanInstance>
#include <QLoggingCategory>
#include "hellovulkantexture.h"

Q_LOGGING_CATEGORY(lcVk, "bobui.vulkan")

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    QLoggingCategory::setFilterRules(QStringLiteral("bobui.vulkan=true"));

    QVulkanInstance inst;
    inst.setLayers({ "VK_LAYER_KHRONOS_validation" });

    if (!inst.create())
        qFatal("Failed to create Vulkan instance: %d", inst.errorCode());

    VulkanWindow w;
    w.setVulkanInstance(&inst);
    if (QCoreApplication::arguments().contains(QStringLiteral("--srgb")))
        w.setPreferredColorFormats(QList<VkFormat>() << VK_FORMAT_B8G8R8A8_SRGB);

    w.resize(1024, 768);
    w.show();

    return app.exec();
}
