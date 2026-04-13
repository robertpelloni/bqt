// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#ifndef QPRINTERINFO_H
#define QPRINTERINFO_H

#include <BobUIPrintSupport/bobuiprintsupportglobal.h>
#include <BobUIPrintSupport/qprinter.h>

#include <BobUICore/QList>
#include <BobUIGui/qpagesize.h>

BOBUI_BEGIN_NAMESPACE


#ifndef BOBUI_NO_PRINTER
class QPrinterInfoPrivate;
class QPrinterInfoPrivateDeleter;
class QDebug;
class Q_PRINTSUPPORT_EXPORT QPrinterInfo
{
public:
    QPrinterInfo();
    QPrinterInfo(const QPrinterInfo &other);
    explicit QPrinterInfo(const QPrinter &printer);
    ~QPrinterInfo();

    QPrinterInfo &operator=(const QPrinterInfo &other);

    QString printerName() const;
    QString description() const;
    QString location() const;
    QString makeAndModel() const;

    bool isNull() const;
    bool isDefault() const;
    bool isRemote() const;

    QPrinter::PrinterState state() const;

    QList<QPageSize> supportedPageSizes() const;
    QPageSize defaultPageSize() const;

    bool supportsCustomPageSizes() const;

    QPageSize minimumPhysicalPageSize() const;
    QPageSize maximumPhysicalPageSize() const;

    QList<int> supportedResolutions() const;

    QPrinter::DuplexMode defaultDuplexMode() const;
    QList<QPrinter::DuplexMode> supportedDuplexModes() const;

    QPrinter::ColorMode defaultColorMode() const;
    QList<QPrinter::ColorMode> supportedColorModes() const;

    static QStringList availablePrinterNames();
    static QList<QPrinterInfo> availablePrinters();

    static QString defaultPrinterName();
    static QPrinterInfo defaultPrinter();

    static QPrinterInfo printerInfo(const QString &printerName);

private:
    explicit QPrinterInfo(const QString &name);

private:
    friend class QPlatformPrinterSupport;
#  ifndef BOBUI_NO_DEBUG_STREAM
    friend Q_PRINTSUPPORT_EXPORT QDebug operator<<(QDebug debug, const QPrinterInfo &);
#  endif
    Q_DECLARE_PRIVATE(QPrinterInfo)
    QScopedPointer<QPrinterInfoPrivate, QPrinterInfoPrivateDeleter> d_ptr;
};

#endif // BOBUI_NO_PRINTER

BOBUI_END_NAMESPACE

#endif // QPRINTERINFO_H
