// Copyright (C) 2025 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only
#pragma once

#include <BobUICore/QByteArray>
#include <BobUICore/QList>

#ifdef BUILD_MYLIB
#  define MYLIB_EXPORT Q_DECL_EXPORT
#else
#  define MYLIB_EXPORT Q_DECL_IMPORT
#endif

MYLIB_EXPORT QList<QByteArray> getSupportedImageFormats();
