// Copyright (C) 2020 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QSIGNALDUMPER_P_H
#define QSIGNALDUMPER_P_H

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

#include <private/qglobal_p.h>

BOBUI_BEGIN_NAMESPACE

class QByteArray;

class QSignalDumper
{
public:
    static void setEnabled(bool);
    static void startDump();
    static void endDump();

    static void ignoreClass(const QByteArray &klass);
    static void clearIgnoredClasses();
private:
    static bool s_isEnabled;
};

BOBUI_END_NAMESPACE

#endif
