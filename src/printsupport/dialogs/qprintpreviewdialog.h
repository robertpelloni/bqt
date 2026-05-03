// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#ifndef QPRINTPREVIEWDIALOG_H
#define QPRINTPREVIEWDIALOG_H

#include <BobUIPrintSupport/bobuiprintsupportglobal.h>

#include <BobUIWidgets/qdialog.h>

BOBUI_REQUIRE_CONFIG(printpreviewdialog);

BOBUI_BEGIN_NAMESPACE

class QGraphicsView;
class QPrintPreviewDialogPrivate;
class QPrinter;

class Q_PRINTSUPPORT_EXPORT QPrintPreviewDialog : public QDialog
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(QPrintPreviewDialog)

public:
    explicit QPrintPreviewDialog(QWidget *parent = nullptr, BobUI::WindowFlags flags = BobUI::WindowFlags());
    explicit QPrintPreviewDialog(QPrinter *printer, QWidget *parent = nullptr,
                                 BobUI::WindowFlags flags = BobUI::WindowFlags());
    ~QPrintPreviewDialog();

    using QDialog::open;
    void open(QObject *receiver, const char *member);

    QPrinter *printer();

    void setVisible(bool visible) override;
    void done(int result) override;

Q_SIGNALS:
    void paintRequested(QPrinter *printer);

private:
    Q_PRIVATE_SLOT(d_func(), void _q_fit(QAction *action))
    Q_PRIVATE_SLOT(d_func(), void _q_zoomIn())
    Q_PRIVATE_SLOT(d_func(), void _q_zoomOut())
    Q_PRIVATE_SLOT(d_func(), void _q_navigate(QAction *action))
    Q_PRIVATE_SLOT(d_func(), void _q_setMode(QAction *action))
    Q_PRIVATE_SLOT(d_func(), void _q_pageNumEdited())
    Q_PRIVATE_SLOT(d_func(), void _q_print())
    Q_PRIVATE_SLOT(d_func(), void _q_pageSetup())
    Q_PRIVATE_SLOT(d_func(), void _q_previewChanged())
    Q_PRIVATE_SLOT(d_func(), void _q_zoomFactorChanged())
};


BOBUI_END_NAMESPACE

#endif // QPRINTPREVIEWDIALOG_H
