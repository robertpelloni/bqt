// Copyright (C) 2022 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

#ifndef QWASMPLATFORM_H
#define QWASMPLATFORM_H

#include <BobUICore/qglobal.h>
#include <BobUICore/qnamespace.h>

#include <QPoint>

#include <emscripten/val.h>

BOBUI_BEGIN_NAMESPACE

enum class Platform {
    Generic,
    MacOS,
    Windows,
    Linux,
    Android,
    iOS
};

Platform platform();

BOBUI_END_NAMESPACE

#endif  // QWASMPLATFORM_H
