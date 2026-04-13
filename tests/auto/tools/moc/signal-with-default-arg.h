// Copyright (C) 2022 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

#ifndef SIGNAL_WITH_DEFAULT_ARG_H
#define SIGNAL_WITH_DEFAULT_ARG_H

#include <BobUICore/qobject.h>

class SignalWithDefaultArg : public QObject
{
    Q_OBJECT
signals:
    void signalWithDefaultArg(int i = 12);
};

#endif // SIGNAL_WITH_DEFAULT_ARG_H
