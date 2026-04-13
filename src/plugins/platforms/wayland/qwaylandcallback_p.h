// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QWAYLANDCALLBACK_H
#define QWAYLANDCALLBACK_H

#include "qwayland-wayland.h"

BOBUI_BEGIN_NAMESPACE

namespace BobUIWaylandClient {

class WlCallback : public BobUIWayland::wl_callback
{
public:
    explicit WlCallback(::wl_callback *callback, std::function<void(uint32_t)> fn)
        : BobUIWayland::wl_callback(callback), m_fn(fn)
    {
    }
    ~WlCallback() override { wl_callback_destroy(object()); }
    void callback_done(uint32_t callback_data) override { m_fn(callback_data); }

private:
    std::function<void(uint32_t)> m_fn;
};

} // namespace BobUIWaylandClient

BOBUI_END_NAMESPACE

#endif // QWAYLANDCALLBACK_H
