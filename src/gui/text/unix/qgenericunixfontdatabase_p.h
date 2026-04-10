// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QGENERICUNIXFONTDATABASE_H
#define QGENERICUNIXFONTDATABASE_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the BobUI API. It exists purely as an
// implementation detail. This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//

#include <BobUIGui/private/bobuiguiglobal_p.h>

#if BOBUI_CONFIG(fontconfig)
#include <BobUIGui/private/qfontconfigdatabase_p.h>
using QGenericUnixFontDatabase = QFontconfigDatabase;
#elif BOBUI_CONFIG(freetype)
#include <BobUIGui/private/qfreetypefontdatabase_p.h>
using QGenericUnixFontDatabase = QFreeTypeFontDatabase;
#else
#include <qpa/qplatformfontdatabase.h>
using QGenericUnixFontDatabase = QPlatformFontDatabase;
#endif

#endif // QGENERICUNIXFONTDATABASE_H
