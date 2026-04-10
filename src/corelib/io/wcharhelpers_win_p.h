// Copyright (C) 2024 Ahmad Samir <a.samirh78@gmail.com>
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#ifndef WCHARHELPERS_WIN_P_H
#define WCHARHELPERS_WIN_P_H

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


#include <BobUICore/qstring.h>

BOBUI_BEGIN_NAMESPACE

static inline const wchar_t *bobui_castToWchar(const QString &s)
{
    return reinterpret_cast<const wchar_t *>(s.constData());
}

BOBUI_END_NAMESPACE

#endif // WCHARHELPERS_WIN_P_H
