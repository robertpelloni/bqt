// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#ifndef QPAGESETUPDIALOG_H
#define QPAGESETUPDIALOG_H

#include <BobUIPrintSupport/bobuiprintsupportglobal.h>

#include <BobUIWidgets/qdialog.h>

BOBUI_REQUIRE_CONFIG(printdialog);

BOBUI_BEGIN_NAMESPACE

class QPrinter;
class QPageSetupDialogPrivate;

class Q_PRINTSUPPORT_EXPORT QPageSetupDialog : public QDialog
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(QPageSetupDialog)

public:
    explicit QPageSetupDialog(QPrinter *printer, QWidget *parent = nullptr);
    explicit QPageSetupDialog(QWidget *parent = nullptr);
    ~QPageSetupDialog();

#if defined(Q_OS_MACOS) || defined(Q_OS_WIN) || defined(Q_QDOC)
    void setVisible(bool visible) override;
#endif
    int exec() override;

    using QDialog::open;
    void open(QObject *receiver, const char *member);

    void done(int result) override;

    QPrinter *printer();
};

BOBUI_END_NAMESPACE

#endif // QPAGESETUPDIALOG_H
