// Copyright (C) 2017 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QANDROIDPLATFORMVULKANINSTANCE_H
#define QANDROIDPLATFORMVULKANINSTANCE_H

#include <BobUIGui/private/qbasicvulkanplatforminstance_p.h>
#include <QLibrary>

BOBUI_BEGIN_NAMESPACE

class QAndroidPlatformVulkanInstance : public QBasicPlatformVulkanInstance
{
public:
    QAndroidPlatformVulkanInstance(QVulkanInstance *instance);
    ~QAndroidPlatformVulkanInstance();

    void createOrAdoptInstance() override;

private:
    QVulkanInstance *m_instance;
    QLibrary m_lib;
};

BOBUI_END_NAMESPACE

#endif // QANDROIDPLATFORMVULKANINSTANCE_H
