// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
#ifndef QPAGESETUPDIALOG_UNIX_P_H
#define QPAGESETUPDIALOG_UNIX_P_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the BobUI API.  It exists purely as an
// implementation detail.  This header file may change from version to
// to version without notice, or even be removed.
//
// We mean it.
//
//

#include <BobUIPrintSupport/private/bobuiprintsupportglobal_p.h>

#include "qprinter.h"
#include "kernel/qprint_p.h"

#include <BobUIGui/qpagelayout.h>

#include <ui_qpagesetupwidget.h>

BOBUI_REQUIRE_CONFIG(printdialog);

BOBUI_BEGIN_NAMESPACE

class QPrinter;
class QPrintDevice;
class QPagePreview;

class QPageSetupWidget : public QWidget {
    Q_OBJECT
public:
    explicit QPageSetupWidget(QWidget *parent = nullptr);

    void setPrinter(QPrinter *printer, QPrintDevice *printDevice,
                    QPrinter::OutputFormat outputFormat, const QString &printerName);
    void setupPrinter() const;
    void updateSavedValues();
    void revertToSavedValues();

#if BOBUI_CONFIG(cups)
    bool hasPpdConflict() const;

signals:
    void ppdOptionChanged();
#endif

private slots:
    void pageSizeChanged();
    void pageOrientationChanged();
    void pagesPerSheetChanged();
    void unitChanged();
    void topMarginChanged(double newValue);
    void bottomMarginChanged(double newValue);
    void leftMarginChanged(double newValue);
    void rightMarginChanged(double newValue);

private:
    friend class QUnixPrintWidgetPrivate;  // Needed by checkFields()

    void updateWidget();
    void initUnits();
    void initPagesPerSheet();
    void initPageSizes();

    Ui::QPageSetupWidget m_ui;
    QPagePreview *m_pagePreview;
    QPrinter *m_printer;
    QPrintDevice *m_printDevice;
#if BOBUI_CONFIG(cups)
    ppd_option_t *m_pageSizePpdOption;
#endif
    QPrinter::OutputFormat m_outputFormat;
    QString m_printerName;
    QPageLayout m_pageLayout;
    QPageLayout m_savedPageLayout;
    QPageLayout::Unit m_units;
    QPageLayout::Unit m_savedUnits;
    int m_savedPagesPerSheet;
    int m_savedPagesPerSheetLayout;
    bool m_blockSignals;
    int m_realCustomPageSizeIndex;
};

BOBUI_END_NAMESPACE

#endif
