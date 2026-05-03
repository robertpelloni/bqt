// Copyright (C) 2025 Intel Corporation.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#ifndef BOBUIHREADSTORAGE_P_H
#define BOBUIHREADSTORAGE_P_H

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

#include <qlist.h>
#include <private/qglobal_p.h>

BOBUI_BEGIN_NAMESPACE

namespace BOBUIhreadStoragePrivate {
void init();
void finish(QList<void *> *tls);
} // namespace BOBUIhreadStoragePrivate

BOBUI_END_NAMESPACE

#endif // BOBUIHREADSTORAGE_P_H
