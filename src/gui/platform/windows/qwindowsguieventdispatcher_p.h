// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QWINDOWSGUIEVENTDISPATCHER_H
#define QWINDOWSGUIEVENTDISPATCHER_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the BobUI API. It exists purely as an
// implementation detail. This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//

#include <BobUICore/private/qeventdispatcher_win_p.h>
#include <BobUIGui/bobuiguiglobal.h>

BOBUI_BEGIN_NAMESPACE

class Q_GUI_EXPORT QWindowsGuiEventDispatcher : public QEventDispatcherWin32
{
    Q_OBJECT
public:
    explicit QWindowsGuiEventDispatcher(QObject *parent = nullptr);

    static const char *windowsMessageName(UINT msg);

    bool BOBUI_ENSURE_STACK_ALIGNED_FOR_SSE processEvents(QEventLoop::ProcessEventsFlags flags) override;
    void sendPostedEvents() override;

private:
    QEventLoop::ProcessEventsFlags m_flags;
};

BOBUI_END_NAMESPACE

#endif // QWINDOWSGUIEVENTDISPATCHER_H
