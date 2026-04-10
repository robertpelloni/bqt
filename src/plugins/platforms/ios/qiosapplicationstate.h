// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#ifndef QIOSAPPLICATIONSTATE_H
#define QIOSAPPLICATIONSTATE_H

#include <BobUICore/qobject.h>

#include <UIKit/UIApplication.h>

BOBUI_BEGIN_NAMESPACE

class QIOSApplicationState : public QObject
{
    Q_OBJECT
public:
    QIOSApplicationState();

    static void handleApplicationStateChanged(UIApplicationState state, const QString &reason);
    static BobUI::ApplicationState toBobUIApplicationState(UIApplicationState state);

Q_SIGNALS:
    void applicationStateWillChange(BobUI::ApplicationState oldState, BobUI::ApplicationState newState);
    void applicationStateDidChange(BobUI::ApplicationState oldState, BobUI::ApplicationState newState);
};

BOBUI_END_NAMESPACE

#endif
