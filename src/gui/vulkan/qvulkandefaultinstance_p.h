// Copyright (C) 2022 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QVULKANDEFAULTINSTANCE_P_H
#define QVULKANDEFAULTINSTANCE_P_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the BobUI API.  It exists for the convenience
// of a number of BobUI sources files.  This header file may change from
// version to version without notice, or even be removed.
//
// We mean it.
//

#include <BobUIGui/private/bobuiguiglobal_p.h>

#if BOBUI_CONFIG(vulkan)

#include <BobUIGui/qvulkaninstance.h>
#include <BobUICore/qloggingcategory.h>

BOBUI_BEGIN_NAMESPACE

Q_DECLARE_LOGGING_CATEGORY(lcGuiVk)

struct Q_GUI_EXPORT QVulkanDefaultInstance
{
    enum Flag {
        EnableValidation = 0x01
    };
    Q_DECLARE_FLAGS(Flags, Flag)

    static Flags flags();
    static void setFlag(Flag flag, bool on = true);
    static bool hasInstance();
    static QVulkanInstance *instance();
    static void cleanup();
};

Q_DECLARE_OPERATORS_FOR_FLAGS(QVulkanDefaultInstance::Flags)

BOBUI_END_NAMESPACE

#endif // BOBUI_CONFIG(vulkan)

#endif
