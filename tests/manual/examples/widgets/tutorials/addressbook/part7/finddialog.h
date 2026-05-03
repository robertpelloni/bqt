// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

#ifndef FINDDIALOG_H
#define FINDDIALOG_H

#include <QDialog>

BOBUI_BEGIN_NAMESPACE
class QLineEdit;
class QPushButton;
BOBUI_END_NAMESPACE

class FindDialog : public QDialog
{
    Q_OBJECT

public:
    FindDialog(QWidget *parent = nullptr);
    QString getFindText();

public slots:
    void findClicked();

private:
    QPushButton *findButton;
    QLineEdit *lineEdit;
    QString findText;
};

#endif
