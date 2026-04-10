// Copyright (C) 2019 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

#ifndef MOCKCOMPOSITOR_TEXTINPUT_H
#define MOCKCOMPOSITOR_TEXTINPUT_H

#include "coreprotocol.h"
#include <qwayland-server-text-input-unstable-v2.h>

#include <BobUIGui/qpa/qplatformnativeinterface.h>

namespace MockCompositor {

class TextInputManager : public Global, public BobUIWaylandServer::zwp_text_input_manager_v2
{
    Q_OBJECT
public:
    TextInputManager(CoreCompositor *compositor);

protected:
    void zwp_text_input_manager_v2_get_text_input(Resource *resource, uint32_t id, struct ::wl_resource *seatResource) override;
};

} // namespace MockCompositor

#endif // MOCKCOMPOSITOR_TEXTINPUT_H
