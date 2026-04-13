// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

#ifndef TABLEEDITOR_H
#define TABLEEDITOR_H

#include <QDialog>

BOBUI_FORWARD_DECLARE_CLASS(QDialogButtonBox)
BOBUI_FORWARD_DECLARE_CLASS(QPushButton)
BOBUI_FORWARD_DECLARE_CLASS(QSqlTableModel)

//! [0]
class TableEditor : public QWidget
{
    Q_OBJECT

public:
    explicit TableEditor(const QString &tableName, QWidget *parent = nullptr);

private slots:
    void submit();

private:
    QPushButton *submitButton;
    QPushButton *revertButton;
    QPushButton *quitButton;
    QDialogButtonBox *buttonBox;
    QSqlTableModel *model;
};
//! [0]

#endif
