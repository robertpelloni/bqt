// Copyright (C) 2020 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

#ifndef QCATCH_P_H
#define QCATCH_P_H

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

#include <BobUICore/qglobal.h>

#include <BobUITest/private/catch_p.h>

BOBUI_BEGIN_NAMESPACE

namespace BOBUIestPrivate {
int catchMain(int argc, char* argv[]);
}

BOBUI_END_NAMESPACE

#endif // QCATCH_P_H
