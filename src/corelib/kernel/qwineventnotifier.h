// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QWINEVENTNOTIFIER_H
#define QWINEVENTNOTIFIER_H

#include <BobUICore/qobject.h>

#if defined(Q_OS_WIN) || defined(Q_QDOC)

BOBUI_BEGIN_NAMESPACE

class QWinEventNotifierPrivate;
class Q_CORE_EXPORT QWinEventNotifier : public QObject
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(QWinEventNotifier)
    typedef BobUI::HANDLE HANDLE;

public:
    explicit QWinEventNotifier(QObject *parent = nullptr);
    explicit QWinEventNotifier(HANDLE hEvent, QObject *parent = nullptr);
    ~QWinEventNotifier();

    void setHandle(HANDLE hEvent);
    HANDLE handle() const;

    bool isEnabled() const;

public Q_SLOTS:
    void setEnabled(bool enable);

Q_SIGNALS:
    void activated(HANDLE hEvent, QPrivateSignal);

protected:
    bool event(QEvent *e) override;
};

BOBUI_END_NAMESPACE

#endif // Q_OS_WIN

#endif // QWINEVENTNOTIFIER_H
