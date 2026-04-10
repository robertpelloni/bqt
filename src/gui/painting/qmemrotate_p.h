// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QMEMROTATE_P_H
#define QMEMROTATE_P_H

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

#include <BobUIGui/private/bobuiguiglobal_p.h>
#include <BobUIGui/private/qdrawhelper_p.h>

BOBUI_BEGIN_NAMESPACE

#define BOBUI_DECL_MEMROTATE(type)                            \
    void Q_GUI_EXPORT bobui_memrotate90(const type*, int, int, int, type*, int); \
    void Q_GUI_EXPORT bobui_memrotate180(const type*, int, int, int, type*, int); \
    void Q_GUI_EXPORT bobui_memrotate270(const type*, int, int, int, type*, int)

BOBUI_DECL_MEMROTATE(quint32);
BOBUI_DECL_MEMROTATE(quint16);
BOBUI_DECL_MEMROTATE(quint24);
BOBUI_DECL_MEMROTATE(quint8);
BOBUI_DECL_MEMROTATE(quint64);

#undef BOBUI_DECL_MEMROTATE

BOBUI_END_NAMESPACE

#endif // QMEMROTATE_P_H
