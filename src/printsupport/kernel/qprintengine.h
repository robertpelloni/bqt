// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#ifndef QPRINTENGINE_H
#define QPRINTENGINE_H

#include <BobUIPrintSupport/bobuiprintsupportglobal.h>
#include <BobUICore/qvariant.h>
#include <BobUIPrintSupport/qprinter.h>

// ### move to qmargins.h
Q_DECLARE_METATYPE(QMarginsF)

BOBUI_BEGIN_NAMESPACE


#ifndef BOBUI_NO_PRINTER

class Q_PRINTSUPPORT_EXPORT QPrintEngine
{
public:
    virtual ~QPrintEngine();

    enum PrintEnginePropertyKey {
        PPK_CollateCopies,
        PPK_ColorMode,
        PPK_Creator,
        PPK_DocumentName,
        PPK_FullPage,
        PPK_NumberOfCopies,
        PPK_Orientation,
        PPK_OutputFileName,
        PPK_PageOrder,
        PPK_PageRect,
        PPK_PageSize,
        PPK_PaperRect,
        PPK_PaperSource,
        PPK_PrinterName,
        PPK_PrinterProgram,
        PPK_Resolution,
        PPK_SelectionOption,
        PPK_SupportedResolutions,

        PPK_WindowsPageSize,
        PPK_FontEmbedding,

        PPK_Duplex,

        PPK_PaperSources,
        PPK_CustomPaperSize,
        PPK_PageMargins,
        PPK_CopyCount,
        PPK_SupportsMultipleCopies,
        PPK_PaperName,
        PPK_QPageSize,
        PPK_QPageMargins,
        PPK_QPageLayout,
        PPK_PaperSize = PPK_PageSize,

        PPK_CustomBase = 0xff00
    };

    virtual void setProperty(PrintEnginePropertyKey key, const QVariant &value) = 0;
    virtual QVariant property(PrintEnginePropertyKey key) const = 0;

    virtual bool newPage() = 0;
    virtual bool abort() = 0;

    virtual int metric(QPaintDevice::PaintDeviceMetric) const = 0;

    virtual QPrinter::PrinterState printerState() const = 0;
};

#endif // BOBUI_NO_PRINTER

BOBUI_END_NAMESPACE

#endif // QPRINTENGINE_H
