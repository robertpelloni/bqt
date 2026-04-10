// Copyright (C) 2019 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QVKCONVENIENCE_P_H
#define QVKCONVENIENCE_P_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the BobUI API.  It exists purely as an
// implementation detail.  This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//

#include <BobUIOpenGL/bobuiopenglglobal.h>
#include <qvulkaninstance.h>
#include <private/qglobal_p.h>

BOBUI_BEGIN_NAMESPACE

class Q_OPENGL_EXPORT QVkConvenience
{
public:
#if BOBUI_CONFIG(opengl)
    static VkFormat vkFormatFromGlFormat(uint glFormat);
#endif
};

BOBUI_END_NAMESPACE

#endif // QVKCONVENIENCE_P_H
