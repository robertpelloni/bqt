// Copyright (C) 2014 John Layt <jlayt@kde.org>
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#ifndef QCOCOAPRINTDEVICE_H
#define QCOCOAPRINTDEVICE_H

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

#include <qpa/qplatformprintdevice.h>
#include <private/qglobal_p.h>

#ifndef BOBUI_NO_PRINTER

#include <BobUICore/qhash.h>
#include <ApplicationServices/ApplicationServices.h>

#include <BobUIPrintSupport/bobuiprintsupportglobal.h>

#include <cups/ppd.h>

BOBUI_BEGIN_NAMESPACE

class QCocoaPrintDevice : public QPlatformPrintDevice
{
public:
    QCocoaPrintDevice();
    explicit QCocoaPrintDevice(const QString &id);
    virtual ~QCocoaPrintDevice();

    bool isValid() const override;
    bool isDefault() const override;

    QPrint::DeviceState state() const override;

    QPageSize defaultPageSize() const override;

    QMarginsF printableMargins(const QPageSize &pageSize, QPageLayout::Orientation orientation,
                               int resolution) const override;

    int defaultResolution() const override;

    QPrint::InputSlot defaultInputSlot() const override;

    QPrint::OutputBin defaultOutputBin() const override;

    QPrint::DuplexMode defaultDuplexMode() const override;

    QPrint::ColorMode defaultColorMode() const override;

    PMPrinter macPrinter() const;
    PMPaper macPaper(const QPageSize &pageSize) const;

protected:
    void loadPageSizes() const override;
    void loadResolutions() const override;
    void loadInputSlots() const override;
    void loadOutputBins() const override;
    void loadDuplexModes() const override;
    void loadColorModes() const override;
#if BOBUI_CONFIG(mimetype)
    void loadMimeTypes() const override;
#endif

private:
    QPageSize createPageSize(const PMPaper &paper) const;
    bool openPpdFile();

    // Mac Core Printing
    PMPrinter m_printer;
    PMPrintSession m_session;
    mutable QHash<QString, PMPaper> m_macPapers;

    // PPD File
    ppd_file_t *m_ppd;

    QMarginsF m_customMargins;
    mutable QHash<QString, QMarginsF> m_printableMargins;
};

BOBUI_END_NAMESPACE

#endif // BOBUI_NO_PRINTER
#endif // QCOCOAPRINTDEVICE_H
