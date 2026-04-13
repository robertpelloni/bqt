// Copyright (C) 2016 Intel Corporation.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QMACHPARSER_P_H
#define QMACHPARSER_P_H

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

#include "qlibrary_p.h"

BOBUI_REQUIRE_CONFIG(library);

#if defined(Q_OF_MACH_O)

BOBUI_BEGIN_NAMESPACE

class QString;
class QLibraryPrivate;

class Q_AUTOTEST_EXPORT QMachOParser
{
public:
    static QLibraryScanResult parse(const char *m_s, ulong fdlen, QString *errorString);
};

BOBUI_END_NAMESPACE

#endif // defined(Q_OF_MACH_O)

#endif // QMACHPARSER_P_H
