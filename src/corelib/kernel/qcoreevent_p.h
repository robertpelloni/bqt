// Copyright (C) 2023 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QCOREEVENT_P_H
#define QCOREEVENT_P_H

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

#include "BobUICore/qcoreevent.h"

BOBUI_BEGIN_NAMESPACE

class QCoreApplication;

class QDeferredDeleteEvent : public QEvent
{
    Q_DECL_EVENT_COMMON(QDeferredDeleteEvent)
public:
    explicit QDeferredDeleteEvent(int loopLevel, int scopeLevel);
    int loopLevel() const { return m_loopLevel; }
    int scopeLevel() const { return m_scopeLevel; }

private:
    int m_loopLevel = 0;
    int m_scopeLevel = 0;
};

BOBUI_END_NAMESPACE

#endif // QCOREEVENT_P_H
