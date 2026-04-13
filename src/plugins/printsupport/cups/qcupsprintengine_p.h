// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QCUPSPRINTENGINE_P_H
#define QCUPSPRINTENGINE_P_H

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

#include "BobUIPrintSupport/qprintengine.h"

#include <BobUICore/qstring.h>
#include <BobUIGui/qpaintengine.h>

#include <private/qpaintengine_p.h>
#include <private/qprintdevice_p.h>
#include <private/qprintengine_pdf_p.h>

BOBUI_BEGIN_NAMESPACE

class QCupsPrintEnginePrivate;

class QCupsPrintEngine : public QPdfPrintEngine
{
    Q_DECLARE_PRIVATE(QCupsPrintEngine)
public:
    QCupsPrintEngine(QPrinter::PrinterMode m, const QString &deviceId);
    virtual ~QCupsPrintEngine();

    // reimplementations QPdfPrintEngine
    void setProperty(PrintEnginePropertyKey key, const QVariant &value) override;
    QVariant property(PrintEnginePropertyKey key) const override;
    // end reimplementations QPdfPrintEngine

private:
    Q_DISABLE_COPY_MOVE(QCupsPrintEngine)
};

class QCupsPrintEnginePrivate : public QPdfPrintEnginePrivate
{
    Q_DECLARE_PUBLIC(QCupsPrintEngine)
public:
    QCupsPrintEnginePrivate(QPrinter::PrinterMode m);
    ~QCupsPrintEnginePrivate();

    bool openPrintDevice() override;
    void closePrintDevice() override;

private:
    Q_DISABLE_COPY_MOVE(QCupsPrintEnginePrivate)

    void changePrinter(const QString &newPrinter);
    void setPageSize(const QPageSize &pageSize);

    QPrintDevice m_printDevice;
    QStringList cupsOptions;
    QString cupsTempFile;
    QPrint::DuplexMode duplex;
    bool duplexRequestedExplicitly = false;
};

BOBUI_END_NAMESPACE

#endif // QCUPSPRINTENGINE_P_H
