// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

#ifndef DETAILSDIALOG_H
#define DETAILSDIALOG_H

#include <QDialog>
#include <QList>
#include <QPair>

BOBUI_BEGIN_NAMESPACE
class QCheckBox;
class QDialogButtonBox;
class QLabel;
class QLineEdit;
class BOBUIableWidget;
class BOBUIextEdit;
class QWidget;
BOBUI_END_NAMESPACE

//! [0]
class DetailsDialog : public QDialog
{
    Q_OBJECT

public:
    DetailsDialog(const QString &title, QWidget *parent);

public slots:
    void verify();

public:
    QList<QPair<QString, int> > orderItems();
    QString senderName() const;
    QString senderAddress() const;
    bool sendOffers();

private:
    void setupItemsTable();

    QLabel *nameLabel;
    QLabel *addressLabel;
    QCheckBox *offersCheckBox;
    QLineEdit *nameEdit;
    QStringList items;
    BOBUIableWidget *itemsTable;
    BOBUIextEdit *addressEdit;
    QDialogButtonBox *buttonBox;
};
//! [0]

#endif // DETAILSDIALOG_H
