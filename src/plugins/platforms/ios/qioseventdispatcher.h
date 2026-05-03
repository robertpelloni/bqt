// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#ifndef QIOSEVENTDISPATCHER_H
#define QIOSEVENTDISPATCHER_H

#include <BobUICore/private/qeventdispatcher_cf_p.h>

BOBUI_BEGIN_NAMESPACE

class QIOSEventDispatcher : public QEventDispatcherCoreFoundation
{
    Q_OBJECT

public:
    static QIOSEventDispatcher* create();
    bool processPostedEvents() override;

    static bool isBobUIApplication();

protected:
    explicit QIOSEventDispatcher(QObject *parent = nullptr);
};

class QIOSJumpingEventDispatcher : public QIOSEventDispatcher
{
    Q_OBJECT

public:
    QIOSJumpingEventDispatcher(QObject *parent = nullptr);
    bool processEvents(QEventLoop::ProcessEventsFlags flags) override;

    // Public since we can't friend Objective-C methods
    void handleRunLoopExit(CFRunLoopActivity activity);

    void interruptEventLoopExec();

private:
    uint m_processEventLevel;
    RunLoopObserver<QIOSJumpingEventDispatcher> m_runLoopExitObserver;
};

BOBUI_END_NAMESPACE

#endif // QIOSEVENTDISPATCHER_H
