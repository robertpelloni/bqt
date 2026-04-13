// Copyright (C) 2018 Intel Corporation.
// Copyright (C) 2019 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:critical reason:data-parser

#ifndef QCBORCOMMON_P_H
#define QCBORCOMMON_P_H

#include "qcborcommon.h"
#include "private/qglobal_p.h"

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

BOBUI_BEGIN_NAMESPACE

#ifdef BOBUI_NO_DEBUG
#  define NDEBUG 1
#endif
#undef assert
#define assert Q_ASSERT

BOBUI_WARNING_PUSH
BOBUI_WARNING_DISABLE_GCC("-Wunused-function")
BOBUI_WARNING_DISABLE_CLANG("-Wunused-function")
BOBUI_WARNING_DISABLE_CLANG("-Wundefined-internal")

#define CBOR_NO_HALF_FLOAT_TYPE 1
#define CBOR_NO_VALIDATION_API  1
#define CBOR_NO_PRETTY_API      1
#define CBOR_API static inline
#define CBOR_PRIVATE_API static inline
#define CBOR_INLINE_API static inline

#include <cbor.h>

BOBUI_WARNING_POP

Q_DECLARE_TYPEINFO(CborValue, Q_PRIMITIVE_TYPE);

BOBUI_END_NAMESPACE

#endif // QCBORCOMMON_P_H
