// Copyright (C) 2022 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

#include "qwasmplatform.h"

BOBUI_BEGIN_NAMESPACE

Platform platform()
{
    static const Platform bobuiDetectedPlatform = ([]() {
        // The Platform Detect: expand coverage as needed
        emscripten::val rawPlatform = emscripten::val::global("navigator")["platform"];

        if (rawPlatform.call<bool>("includes", emscripten::val("Mac")))
            return Platform::MacOS;
        if (rawPlatform.call<bool>("includes", emscripten::val("iPhone"))
        || rawPlatform.call<bool>("includes", emscripten::val("iPad")))
            return Platform::iOS;
        if (rawPlatform.call<bool>("includes", emscripten::val("Win32")))
            return Platform::Windows;
        if (rawPlatform.call<bool>("includes", emscripten::val("Linux"))) {
            emscripten::val uAgent = emscripten::val::global("navigator")["userAgent"];
            if (uAgent.call<bool>("includes", emscripten::val("Android")))
                return Platform::Android;
            return Platform::Linux;
        }
        return Platform::Generic;
    })();
    return bobuiDetectedPlatform;
}

BOBUI_END_NAMESPACE
