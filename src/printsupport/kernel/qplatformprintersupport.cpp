// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#include "qplatformprintersupport.h"
#include "qplatformprintdevice.h"

#include <BobUIGui/qpagesize.h>
#include <BobUIPrintSupport/qprinterinfo.h>

#include <private/qprinterinfo_p.h>
#include <private/qprintdevice_p.h>

#ifndef BOBUI_NO_PRINTER

BOBUI_BEGIN_NAMESPACE

/*!
    \class QPlatformPrinterSupport
    \since 5.0
    \internal
    \preliminary
    \ingroup qpa

    \brief The QPlatformPrinterSupport class provides an abstraction for print support.
 */

QPlatformPrinterSupport::QPlatformPrinterSupport()
{
}

QPlatformPrinterSupport::~QPlatformPrinterSupport()
{
}

QPrintEngine *QPlatformPrinterSupport::createNativePrintEngine(QPrinter::PrinterMode, const QString &)
{
    return nullptr;
}

QPaintEngine *QPlatformPrinterSupport::createPaintEngine(QPrintEngine *, QPrinter::PrinterMode)
{
    return nullptr;
}

QPrintDevice QPlatformPrinterSupport::createPrintDevice(QPlatformPrintDevice *device)
{
    return QPrintDevice(device);
}

QPrintDevice QPlatformPrinterSupport::createPrintDevice(const QString &id)
{
    Q_UNUSED(id);
    return QPrintDevice();
}

QStringList QPlatformPrinterSupport::availablePrintDeviceIds() const
{
    return QStringList();
}

QString QPlatformPrinterSupport::defaultPrintDeviceId() const
{
    return QString();
}

QPageSize QPlatformPrinterSupport::createPageSize(const QString &id, QSize size, const QString &localizedName)
{
    Q_UNUSED(id);
    Q_UNUSED(size);
    Q_UNUSED(localizedName);
    return QPageSize();
}

BOBUI_END_NAMESPACE

#endif // BOBUI_NO_PRINTER
