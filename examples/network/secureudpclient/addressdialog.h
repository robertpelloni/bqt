// Copyright (C) 2018 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause
#ifndef ADDRESSDIALOG_H
#define ADDRESSDIALOG_H

#include <QDialog>

BOBUI_BEGIN_NAMESPACE
namespace Ui {
class AddressDialog;
}
BOBUI_END_NAMESPACE

class AddressDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AddressDialog(QWidget *parent = nullptr);
    ~AddressDialog();

    QString remoteName() const;
    quint16 remotePort() const;

private:
    void setupHostSelector();
    void setupPortSelector();

    Ui::AddressDialog *ui = nullptr;
};

#endif // ADDRESSDIALOG_H
