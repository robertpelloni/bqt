// Copyright (C) 2019 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

#include "qwasmoffscreensurface.h"

BOBUI_BEGIN_NAMESPACE

QWasmOffscreenSurface::QWasmOffscreenSurface(QOffscreenSurface *offscreenSurface)
    : QPlatformOffscreenSurface(offscreenSurface), m_offscreenCanvas(emscripten::val::undefined())
{
    const auto offscreenCanvasClass = emscripten::val::global("OffscreenCanvas");
    // The OffscreenCanvas is not supported on some browsers, most notably on Safari.
    if (!offscreenCanvasClass)
        return;

    m_offscreenCanvas = offscreenCanvasClass.new_(offscreenSurface->size().width(),
                                                  offscreenSurface->size().height());

    m_specialTargetId = std::string("!bobuioffscreen_") + std::to_string(uintptr_t(this));

    emscripten::val::module_property("specialHTMLTargets")
            .set(m_specialTargetId, m_offscreenCanvas);
}

QWasmOffscreenSurface::~QWasmOffscreenSurface()
{
    emscripten::val::module_property("specialHTMLTargets").delete_(m_specialTargetId);
}

bool QWasmOffscreenSurface::isValid() const
{
    return !m_offscreenCanvas.isNull() && !m_offscreenCanvas.isUndefined();
}

BOBUI_END_NAMESPACE
