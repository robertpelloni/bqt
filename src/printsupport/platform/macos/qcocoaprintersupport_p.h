// Copyright (C) 2020 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#ifndef QCOCOAPRINTERSUPPORT_H
#define QCOCOAPRINTERSUPPORT_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the BobUI API.  It exists for the convenience
// of internal files.  This header file may change from version to version
// without notice, or even be removed.
//
// We mean it.
//

#include <qpa/qplatformprintersupport.h>
#include <private/qglobal_p.h>
#ifndef BOBUI_NO_PRINTER

#include <BobUIPrintSupport/bobuiprintsupportglobal.h>

BOBUI_BEGIN_NAMESPACE

class Q_PRINTSUPPORT_EXPORT QCocoaPrinterSupport : public QPlatformPrinterSupport
{
public:
    QCocoaPrinterSupport();
    ~QCocoaPrinterSupport();

    QPrintEngine *createNativePrintEngine(QPrinter::PrinterMode printerMode, const QString &deviceId = QString()) override;
    QPaintEngine *createPaintEngine(QPrintEngine *, QPrinter::PrinterMode printerMode) override;

    QPrintDevice createPrintDevice(const QString &id) override;
    QStringList availablePrintDeviceIds() const override;
    QString defaultPrintDeviceId() const override;
};

BOBUI_END_NAMESPACE

#endif // BOBUI_NO_PRINTER
#endif // QCOCOAPRINTERSUPPORT_H
