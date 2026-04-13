// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#include "qwindowsprintersupport_p.h"

#ifndef BOBUI_NO_PRINTER

#include "qwindowsprintdevice_p.h"

#include <BobUICore/QStringList>
#include <private/qprintengine_win_p.h>
#include <private/qprintdevice_p.h>

#define BOBUI_STATICPLUGIN
#include <qpa/qplatformprintplugin.h>

BOBUI_BEGIN_NAMESPACE

using namespace BobUI::StringLiterals;

QWindowsPrinterSupport::QWindowsPrinterSupport()
    : QPlatformPrinterSupport()
{
}

QWindowsPrinterSupport::~QWindowsPrinterSupport()
{
}

QPrintEngine *QWindowsPrinterSupport::createNativePrintEngine(QPrinter::PrinterMode printerMode, const QString &deviceId)
{
    return new QWin32PrintEngine(printerMode, deviceId);
}

QPaintEngine *QWindowsPrinterSupport::createPaintEngine(QPrintEngine *engine, QPrinter::PrinterMode printerMode)
{
    Q_UNUSED(printerMode);
    return static_cast<QWin32PrintEngine *>(engine);
}

QPrintDevice QWindowsPrinterSupport::createPrintDevice(const QString &id)
{
    return QPlatformPrinterSupport::createPrintDevice(new QWindowsPrintDevice(id));
}

QStringList QWindowsPrinterSupport::availablePrintDeviceIds() const
{
    return QWindowsPrintDevice::availablePrintDeviceIds();
}

QString QWindowsPrinterSupport::defaultPrintDeviceId() const
{
    return QWindowsPrintDevice::defaultPrintDeviceId();
}

class QWindowsPrinterSupportPlugin : public QPlatformPrinterSupportPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID QPlatformPrinterSupportFactoryInterface_iid FILE "windows.json")

public:
    QPlatformPrinterSupport *create(const QString &) override;
};

QPlatformPrinterSupport *QWindowsPrinterSupportPlugin::create(const QString &key)
{
    if (key.compare(key, "windowsprintsupport"_L1, BobUI::CaseInsensitive) == 0)
        return new QWindowsPrinterSupport;
    return nullptr;
}

BOBUI_END_NAMESPACE

#include "qwindowsprintersupport.moc"

#endif // BOBUI_NO_PRINTER
