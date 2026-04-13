// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#ifndef QPAGESETUPDIALOG_P_H
#define QPAGESETUPDIALOG_P_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the BobUI API.  It exists purely as an
// implementation detail.  This header file may change from version to
// to version without notice, or even be removed.
//
// We mean it.
//
//

#include <BobUIPrintSupport/private/bobuiprintsupportglobal_p.h>

#include "private/qdialog_p.h"

#include "qbytearray.h"
#include "qpagesetupdialog.h"
#include "qpointer.h"

BOBUI_REQUIRE_CONFIG(printdialog);

BOBUI_BEGIN_NAMESPACE

class QPrinter;

class QPageSetupDialogPrivate : public QDialogPrivate
{
    Q_DECLARE_PUBLIC(QPageSetupDialog)

public:
    explicit QPageSetupDialogPrivate(QPrinter *printer);

    void setPrinter(QPrinter *newPrinter);

    QPrinter *printer;
    bool ownsPrinter;
    QPointer<QObject> receiverToDisconnectOnClose;
    QByteArray memberToDisconnectOnClose;
};

BOBUI_END_NAMESPACE

#endif // QPAGESETUPDIALOG_P_H
