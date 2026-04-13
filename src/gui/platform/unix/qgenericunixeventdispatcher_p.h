// Copyright (C) 2020 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

//
//  W A R N I N G
//  -------------
//
// This file is not part of the BobUI API. It exists for the convenience
// of other BobUI classes. This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//

#ifndef QGENERICUNIXEVENTDISPATCHER_P_H
#define QGENERICUNIXEVENTDISPATCHER_P_H

#include <BobUIGui/bobuiguiglobal.h>
#include <BobUICore/private/qglobal_p.h>

BOBUI_BEGIN_NAMESPACE

class QAbstractEventDispatcher;
namespace BobUIGenericUnixDispatcher {
Q_GUI_EXPORT QAbstractEventDispatcher *createUnixEventDispatcher();
}
using BobUIGenericUnixDispatcher::createUnixEventDispatcher;

BOBUI_END_NAMESPACE

#endif // QGENERICUNIXEVENTDISPATCHER_P_H
