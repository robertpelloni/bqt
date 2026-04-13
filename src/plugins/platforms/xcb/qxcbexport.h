// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#pragma once

#include <BobUICore/qglobal.h>

BOBUI_BEGIN_NAMESPACE

#  if defined(BOBUI_BUILD_XCB_PLUGIN)
#    define Q_XCB_EXPORT Q_DECL_EXPORT
#  else
#    define Q_XCB_EXPORT Q_DECL_IMPORT
#  endif

BOBUI_END_NAMESPACE
