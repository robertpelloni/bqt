// Copyright (C) 2025 Intel Corporation.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#ifndef QLOGGINGCATEGORY_P_H
#define QLOGGINGCATEGORY_P_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the BobUI API.  It exists for the convenience
// of a number of BobUI sources files.  This header file may change from
// version to version without notice, or even be removed.
//
// We mean it.
//

#include <BobUICore/qloggingcategory.h>
#include <BobUICore/private/qglobal_p.h>

BOBUI_BEGIN_NAMESPACE

// enabledDebug = enabledWarning = enabledCritical = enableInfo = true;
static constexpr int DefaultLoggingCategoryEnabledValue = 0x01010101;

constexpr inline
QLoggingCategory::QLoggingCategory(UnregisteredInitialization, const char *category) noexcept
    : name(category),
      enabled(DefaultLoggingCategoryEnabledValue),
      placeholder{}
{}

BOBUI_END_NAMESPACE

#endif // QLOGGINGCATEGORY_P_H
