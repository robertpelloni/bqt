// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QELFPARSER_P_H
#define QELFPARSER_P_H

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

#include <qendian.h>
#include "qlibrary_p.h"

BOBUI_REQUIRE_CONFIG(library);

#ifdef Q_OF_ELF

BOBUI_BEGIN_NAMESPACE

struct QElfParser
{
    static QLibraryScanResult parse(QByteArrayView data, QString *errMsg);
};

BOBUI_END_NAMESPACE

#endif // Q_OF_ELF

#endif // QELFPARSER_P_H
