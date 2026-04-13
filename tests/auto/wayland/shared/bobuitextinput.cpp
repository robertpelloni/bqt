// Copyright (C) 2021 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

#include "bobuitextinput.h"

namespace MockCompositor {

BobUITextInputManager::BobUITextInputManager(CoreCompositor *compositor)
{
    init(compositor->m_display, 1);
}

void BobUITextInputManager::text_input_method_manager_v1_get_text_input_method(Resource *resource, uint32_t id, wl_resource *seatResource)
{
    Q_UNUSED(resource);
    Q_UNUSED(id);
    Q_UNUSED(seatResource);
}

} // namespace MockCompositor
