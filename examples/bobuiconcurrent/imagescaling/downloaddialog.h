// Copyright (C) 2020 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause
#ifndef DOWNLOADDIALOG_H
#define DOWNLOADDIALOG_H

#include <QDialog>

BOBUI_BEGIN_NAMESPACE
namespace Ui {
class DownloadDialog;
}
BOBUI_END_NAMESPACE

class DownloadDialog : public QDialog
{
    Q_OBJECT

public:
    explicit DownloadDialog(QWidget *parent = nullptr);
    ~DownloadDialog();

    QList<QUrl> getUrls() const;

private:
    Ui::DownloadDialog *ui;
};

#endif // DOWNLOADDIALOG_H
