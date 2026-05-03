// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#ifndef QABSTRACTPRINTDIALOG_P_H
#define QABSTRACTPRINTDIALOG_P_H

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

#include <BobUIPrintSupport/private/bobuiprintsupportglobal_p.h>

#include "private/qdialog_p.h"
#include "BobUIPrintSupport/qabstractprintdialog.h"

#include <BobUICore/qpointer.h>

BOBUI_REQUIRE_CONFIG(printdialog);

BOBUI_BEGIN_NAMESPACE

class QPrinter;
class QPrinterPrivate;

class QAbstractPrintDialogPrivate : public QDialogPrivate
{
    Q_DECLARE_PUBLIC(QAbstractPrintDialog)

public:
    QAbstractPrintDialogPrivate()
        : printer(nullptr), pd(nullptr)
        , options(QAbstractPrintDialog::PrintToFile | QAbstractPrintDialog::PrintPageRange |
                QAbstractPrintDialog::PrintCollateCopies | QAbstractPrintDialog::PrintShowPageSize),
          minPage(0), maxPage(INT_MAX), ownsPrinter(false)
    {
    }

    QPrinter *printer;
    QPrinterPrivate *pd;
    QPointer<QObject> receiverToDisconnectOnClose;
    QByteArray memberToDisconnectOnClose;

    QAbstractPrintDialog::PrintDialogOptions options;

    virtual void setTabs(const QList<QWidget *> &) {}
    void setPrinter(QPrinter *newPrinter);
    int minPage;
    int maxPage;

    bool ownsPrinter;
};

BOBUI_END_NAMESPACE

#endif // QABSTRACTPRINTDIALOG_P_H
