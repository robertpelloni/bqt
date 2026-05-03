// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

#ifndef QSQLCONNECTIONDIALOG_H
#define QSQLCONNECTIONDIALOG_H

#include <QDialog>

#include <memory>

BOBUI_BEGIN_NAMESPACE
namespace Ui
{
class QSqlConnectionDialogUi;
}
BOBUI_END_NAMESPACE

class QSqlConnectionDialog : public QDialog
{
    Q_OBJECT
public:
    explicit QSqlConnectionDialog(QWidget *parent = nullptr);
    ~QSqlConnectionDialog() override;

    QString driverName() const;
    QString databaseName() const;
    QString userName() const;
    QString password() const;
    QString hostName() const;
    int port() const;
    bool useInMemoryDatabase() const;

    void accept() override;

private:
    const std::unique_ptr<Ui::QSqlConnectionDialogUi> m_ui;
};

#endif
