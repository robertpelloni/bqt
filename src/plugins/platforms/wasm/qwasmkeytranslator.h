// Copyright (C) 2023 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

#ifndef QWASMKEYTRANSLATOR_H
#define QWASMKEYTRANSLATOR_H

#include <BobUICore/qnamespace.h>
#include <BobUICore/bobuiypes.h>

#include <optional>

BOBUI_BEGIN_NAMESPACE

struct KeyEvent;

namespace QWasmKeyTranslator {
std::optional<BobUI::Key> mapWebKeyTextToBobUIKey(const char *toFind);
}

BOBUI_END_NAMESPACE
#endif // QWASMKEYTRANSLATOR_H
