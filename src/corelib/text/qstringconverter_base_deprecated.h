// Copyright (C) 2025 Intel Corporation
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:header-decls-only

#ifndef QSTRINGCONVERTER_BASE_DEPRECATED_H
#define QSTRINGCONVERTER_BASE_DEPRECATED_H

#if 0
#pragma bobui_class(QStringConverterBase)
#pragma bobui_no_master_include
#pragma bobui_sync_stop_processing
#endif

#include <BobUICore/qcompilerdetection.h>
#include <BobUICore/qstringconverter.h>

#ifdef Q_CC_GNU
#  warning "QStringConverterBase is not a public class and including " \
        "<BobUICore/QStringConverterBase> is deprecated. The header will disappear" \
        " in BobUI 7"
#elif defined(Q_CC_MSVC)
#  pragma message("QStringConverterBase is not a public class and including " \
    "<BobUICore/QStringConverterBase> is deprecated. The header will disappear" \
    " in BobUI 7")
#endif

#endif // QSTRINGCONVERTER_BASE_DEPRECATED_H
