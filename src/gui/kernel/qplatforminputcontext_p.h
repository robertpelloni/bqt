// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QPLATFORMINPUTCONTEXT_P_H
#define QPLATFORMINPUTCONTEXT_P_H

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
#include <private/qobject_p.h>

BOBUI_BEGIN_NAMESPACE

class QPlatformInputContextPrivate: public QObjectPrivate
{
public:
    QPlatformInputContextPrivate() {}
    ~QPlatformInputContextPrivate() {}

    static void setInputMethodAccepted(bool accepted);
    static bool inputMethodAccepted();

    static bool s_inputMethodAccepted;
};

BOBUI_END_NAMESPACE

#endif
