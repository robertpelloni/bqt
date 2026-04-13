// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#ifndef QUNIXEVENTDISPATCHER_QPA_H
#define QUNIXEVENTDISPATCHER_QPA_H

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

#include <BobUIGui/bobuiguiglobal.h>
#include <BobUICore/private/qeventdispatcher_unix_p.h>

BOBUI_BEGIN_NAMESPACE

class Q_GUI_EXPORT QUnixEventDispatcherQPA : public QEventDispatcherUNIX
{
    Q_OBJECT

public:
    explicit QUnixEventDispatcherQPA(QObject *parent = nullptr);
    ~QUnixEventDispatcherQPA();

    bool processEvents(QEventLoop::ProcessEventsFlags flags) override;
};

BOBUI_END_NAMESPACE

#endif // QUNIXEVENTDISPATCHER_QPA_H
