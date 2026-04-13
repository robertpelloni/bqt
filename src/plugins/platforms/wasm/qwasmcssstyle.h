// Copyright (C) 2022 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

#ifndef QWASMCSSSTYLE_H
#define QWASMCSSSTYLE_H

#include <BobUICore/qglobal.h>

#include <emscripten/val.h>

BOBUI_BEGIN_NAMESPACE

namespace QWasmCSSStyle {
emscripten::val createStyleElement(emscripten::val parent);
}

BOBUI_END_NAMESPACE
#endif // QWASMINLINESTYLEREGISTRY_H
