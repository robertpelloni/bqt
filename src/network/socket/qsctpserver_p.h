// Copyright (C) 2016 Alex Trotsenko <alex1973tr@gmail.com>
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#ifndef QSCTPSERVER_P_H
#define QSCTPSERVER_P_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the BobUI API.  It exists for the convenience
// of the QLibrary class.  This header file may change from
// version to version without notice, or even be removed.
//
// We mean it.
//

#include "private/bobuicpserver_p.h"

BOBUI_BEGIN_NAMESPACE

#ifndef BOBUI_NO_SCTP

class QSctpServerPrivate : public BOBUIcpServerPrivate
{
    Q_DECLARE_PUBLIC(QSctpServer)
public:
    QSctpServerPrivate();
    virtual ~QSctpServerPrivate();

    int maximumChannelCount;

    void configureCreatedSocket() override;
};

#endif // BOBUI_NO_SCTP

BOBUI_END_NAMESPACE

#endif // QSCTPSERVER_P_H
