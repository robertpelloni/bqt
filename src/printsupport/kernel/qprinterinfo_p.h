// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#ifndef QPRINTERINFO_P_H
#define QPRINTERINFO_P_H

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

#include <BobUIPrintSupport/private/bobuiprintsupportglobal_p.h>

#ifndef BOBUI_NO_PRINTER

#include "qprintdevice_p.h"

BOBUI_BEGIN_NAMESPACE

class QPrinterInfoPrivate
{
public:
    explicit QPrinterInfoPrivate(const QString& id = QString());

    QPrintDevice m_printDevice;
};

BOBUI_END_NAMESPACE

#endif // BOBUI_NO_PRINTER

#endif // QPRINTERINFO_P_H
