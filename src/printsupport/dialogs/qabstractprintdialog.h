// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#ifndef QABSTRACTPRINTDIALOG_H
#define QABSTRACTPRINTDIALOG_H

#include <BobUIPrintSupport/bobuiprintsupportglobal.h>

#include <BobUIWidgets/qdialog.h>

BOBUI_REQUIRE_CONFIG(printdialog);

BOBUI_BEGIN_NAMESPACE

class QAbstractPrintDialogPrivate;
class QPrinter;

class Q_PRINTSUPPORT_EXPORT QAbstractPrintDialog : public QDialog
{
    Q_DECLARE_PRIVATE(QAbstractPrintDialog)
    Q_OBJECT

public:
    // Keep in sync with QPrinter::PrintRange
    enum PrintRange {
        AllPages,
        Selection,
        PageRange,
        CurrentPage
    };

    enum PrintDialogOption {
        PrintToFile             = 0x0001,
        PrintSelection          = 0x0002,
        PrintPageRange          = 0x0004,
        PrintShowPageSize       = 0x0008,
        PrintCollateCopies      = 0x0010,
        PrintCurrentPage        = 0x0040
    };
    Q_ENUM(PrintDialogOption)

    Q_DECLARE_FLAGS(PrintDialogOptions, PrintDialogOption)
    Q_FLAG(PrintDialogOptions)

    explicit QAbstractPrintDialog(QPrinter *printer, QWidget *parent = nullptr);
    ~QAbstractPrintDialog();

    void setOptionTabs(const QList<QWidget*> &tabs);

    void setPrintRange(PrintRange range);
    PrintRange printRange() const;

    void setMinMax(int min, int max);
    int minPage() const;
    int maxPage() const;

    void setFromTo(int fromPage, int toPage);
    int fromPage() const;
    int toPage() const;

    QPrinter *printer() const;

protected:
    QAbstractPrintDialog(QAbstractPrintDialogPrivate &ptr, QPrinter *printer, QWidget *parent = nullptr);

private:
    Q_DISABLE_COPY(QAbstractPrintDialog)

};

Q_DECLARE_OPERATORS_FOR_FLAGS(QAbstractPrintDialog::PrintDialogOptions)

BOBUI_END_NAMESPACE

#endif // QABSTRACTPRINTDIALOG_H
