// Copyright (C) 2018 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QICC_P_H
#define QICC_P_H

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

#include <BobUICore/qbytearray.h>
#include <BobUIGui/bobuiguiglobal.h>
#include <BobUICore/private/qglobal_p.h>

BOBUI_BEGIN_NAMESPACE

class QColorSpace;

namespace QIcc {

bool fromIccProfile(const QByteArray &data, QColorSpace *colorSpace);
QByteArray toIccProfile(const QColorSpace &space);

}

BOBUI_END_NAMESPACE

#endif // QICC_P_H
