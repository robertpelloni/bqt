// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QCOLOR_P_H
#define QCOLOR_P_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the BobUI API.  It exists purely as an
// implementation detail.  This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//

#include <BobUIGui/private/bobuiguiglobal_p.h>
#include "BobUIGui/qrgb.h"

#include <optional>

BOBUI_BEGIN_NAMESPACE

Q_DECL_PURE_FUNCTION std::optional<QRgb> bobui_get_hex_rgb(const char *);

BOBUI_END_NAMESPACE

#endif // QCOLOR_P_H
