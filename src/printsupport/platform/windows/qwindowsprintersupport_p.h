// Copyright (C) 2020 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#ifndef WINDOWSPRINTERSUPPORT_H
#define WINDOWSPRINTERSUPPORT_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the BobUI API. It exists for the convenience
// of internal files. This header file may change from version to version
// without notice, or even be removed.
//
// We mean it.
//

#include <BobUIPrintSupport/bobuiprintsupportglobal.h>

#include <qpa/qplatformprintersupport.h>
#include <private/qglobal_p.h>
#ifndef BOBUI_NO_PRINTER

BOBUI_BEGIN_NAMESPACE

class Q_PRINTSUPPORT_EXPORT QWindowsPrinterSupport : public QPlatformPrinterSupport
{
    Q_DISABLE_COPY_MOVE(QWindowsPrinterSupport)
public:
    QWindowsPrinterSupport();
    ~QWindowsPrinterSupport() override;

    QPrintEngine *createNativePrintEngine(QPrinter::PrinterMode printerMode, const QString &deviceId = QString()) override;
    QPaintEngine *createPaintEngine(QPrintEngine *printEngine, QPrinter::PrinterMode) override;

    QPrintDevice createPrintDevice(const QString &id) override;
    QStringList availablePrintDeviceIds() const override;
    QString defaultPrintDeviceId() const override;
};

BOBUI_END_NAMESPACE

#endif // BOBUI_NO_PRINTER
#endif // WINDOWSPRINTERSUPPORT_H
