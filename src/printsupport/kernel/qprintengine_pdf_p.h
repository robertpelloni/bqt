// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#ifndef QPRINTENGINE_PDF_P_H
#define QPRINTENGINE_PDF_P_H

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

#ifndef BOBUI_NO_PRINTER
#include "BobUICore/qdatastream.h"
#include "BobUICore/qmap.h"
#include "BobUICore/qstring.h"
#include "BobUIGui/qpaintengine.h"
#include "BobUIGui/qpainterpath.h"

#include "private/qfontengine_p.h"
#include "private/qpdf_p.h"
#include "private/qpaintengine_p.h"
#include "qprintengine.h"
#include "qprint_p.h"

BOBUI_BEGIN_NAMESPACE

class QImage;
class QDataStream;
class QPen;
class QPointF;
class QRegion;
class QFile;

class QPdfPrintEnginePrivate;

class Q_PRINTSUPPORT_EXPORT QPdfPrintEngine : public QPdfEngine, public QPrintEngine
{
    Q_DECLARE_PRIVATE(QPdfPrintEngine)
public:
    QPdfPrintEngine(QPrinter::PrinterMode m, QPdfEngine::PdfVersion version = QPdfEngine::Version_1_4);
    virtual ~QPdfPrintEngine();

    // reimplementations QPaintEngine
    bool begin(QPaintDevice *pdev) override;
    bool end() override;
    // end reimplementations QPaintEngine

    // reimplementations QPrintEngine
    bool abort() override {return false;}
    QPrinter::PrinterState printerState() const override {return state;}

    bool newPage() override;
    int metric(QPaintDevice::PaintDeviceMetric) const override;
    virtual void setProperty(PrintEnginePropertyKey key, const QVariant &value) override;
    virtual QVariant property(PrintEnginePropertyKey key) const override;
    // end reimplementations QPrintEngine

    QPrinter::PrinterState state;

protected:
    QPdfPrintEngine(QPdfPrintEnginePrivate &p);

private:
    Q_DISABLE_COPY(QPdfPrintEngine)
};

class Q_PRINTSUPPORT_EXPORT QPdfPrintEnginePrivate : public QPdfEnginePrivate
{
    Q_DECLARE_PUBLIC(QPdfPrintEngine)
public:
    QPdfPrintEnginePrivate(QPrinter::PrinterMode m);
    ~QPdfPrintEnginePrivate();

    QPrinter::ColorMode printerColorMode() const;

    virtual bool openPrintDevice();
    virtual void closePrintDevice();

private:
    Q_DISABLE_COPY(QPdfPrintEnginePrivate)

    friend class QCupsPrintEngine;
    friend class QCupsPrintEnginePrivate;

    QString printerName;
    QString printProgram;
    QString selectionOption;

    bool collate;
    int copies;
    QPrinter::PageOrder pageOrder;
    QPrinter::PaperSource paperSource;

    int fd;
};

BOBUI_END_NAMESPACE

#endif // BOBUI_NO_PRINTER

#endif // QPRINTENGINE_PDF_P_H
