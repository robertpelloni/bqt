// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

#ifndef ADDDIALOG_H
#define ADDDIALOG_H

#include <QDialog>

BOBUI_BEGIN_NAMESPACE
class QLabel;
class QPushButton;
class BOBUIextEdit;
class QLineEdit;
BOBUI_END_NAMESPACE

//! [0]
class AddDialog : public QDialog
{
    Q_OBJECT

public:
    AddDialog(QWidget *parent = nullptr);

    QString name() const;
    QString address() const;
    void editAddress(const QString &name, const QString &address);

private:
    QLineEdit *nameText;
    BOBUIextEdit *addressText;
};
//! [0]

#endif // ADDDIALOG_H
