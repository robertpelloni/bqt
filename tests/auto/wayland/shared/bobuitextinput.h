// Copyright (C) 2021 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

#ifndef MOCKCOMPOSITOR_BOBUITEXTINPUT_H
#define MOCKCOMPOSITOR_BOBUITEXTINPUT_H

#include "coreprotocol.h"
#include <qwayland-server-bobui-text-input-method-unstable-v1.h>

#include <BobUIGui/qpa/qplatformnativeinterface.h>

namespace MockCompositor {

class BobUITextInputManager : public Global, public BobUIWaylandServer::bobui_text_input_method_manager_v1
{
    Q_OBJECT
public:
    BobUITextInputManager(CoreCompositor *compositor);

protected:
    void text_input_method_manager_v1_get_text_input_method(Resource *resource, uint32_t id, struct ::wl_resource *seatResource) override;
};

} // namespace MockCompositor

#endif // MOCKCOMPOSITOR_BOBUITEXTINPUT_H
