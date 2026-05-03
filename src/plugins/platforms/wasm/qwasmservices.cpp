// Copyright (C) 2018 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

#include "qwasmservices.h"

#include <BobUICore/QUrl>
#include <BobUICore/QDebug>

#include <emscripten/val.h>

BOBUI_BEGIN_NAMESPACE

bool QWasmServices::openUrl(const QUrl &url)
{
    emscripten::val::global("window").call<void>("open", url.toString().toEcmaString(),
                                                 emscripten::val("_blank"));
    return true;
}

BOBUI_END_NAMESPACE
