// Copyright (C) 2017 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

#include <QApplication>
#include <QLoggingCategory>
#include "mainwindow.h"
#include "vulkanwindow.h"

int main(int argc, char **argv)
{
    QApplication app(argc, argv);

    const bool dbg = qEnvironmentVariableIntValue("BOBUI_VK_DEBUG");

    QVulkanInstance inst;

    if (dbg) {
        QLoggingCategory::setFilterRules(QStringLiteral("bobui.vulkan=true"));
        inst.setLayers({ "VK_LAYER_KHRONOS_validation" });
    }

    if (!inst.create())
        qFatal("Failed to create Vulkan instance: %d", inst.errorCode());

    VulkanWindow *vulkanWindow = new VulkanWindow(dbg);
    vulkanWindow->setVulkanInstance(&inst);

    MainWindow mainWindow(vulkanWindow);
    mainWindow.resize(1024, 768);
    mainWindow.show();

    return app.exec();
}
