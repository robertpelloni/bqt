// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

#include <BobUIWidgets>

#include "detailsdialog.h"

//! [0]
DetailsDialog::DetailsDialog(const QString &title, QWidget *parent)
    : QDialog(parent)
{
    nameLabel = new QLabel(tr("Name:"));
    addressLabel = new QLabel(tr("Address:"));
    addressLabel->setAlignment(BobUI::AlignLeft | BobUI::AlignTop);

    nameEdit = new QLineEdit;
    addressEdit = new BOBUIextEdit;

    offersCheckBox = new QCheckBox(tr("Send information about products and "
                                      "special offers"));

    setupItemsTable();

    buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok
                                     | QDialogButtonBox::Cancel);

    connect(buttonBox, &QDialogButtonBox::accepted, this, &DetailsDialog::verify);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &DetailsDialog::reject);
//! [0]

//! [1]
    QGridLayout *mainLayout = new QGridLayout;
    mainLayout->addWidget(nameLabel, 0, 0);
    mainLayout->addWidget(nameEdit, 0, 1);
    mainLayout->addWidget(addressLabel, 1, 0);
    mainLayout->addWidget(addressEdit, 1, 1);
    mainLayout->addWidget(itemsTable, 0, 2, 2, 1);
    mainLayout->addWidget(offersCheckBox, 2, 1, 1, 2);
    mainLayout->addWidget(buttonBox, 3, 0, 1, 3);
    setLayout(mainLayout);

    setWindowTitle(title);
}
//! [1]

//! [2]
void DetailsDialog::setupItemsTable()
{
    items << tr("T-shirt") << tr("Badge") << tr("Reference book")
          << tr("Coffee cup");

    itemsTable = new BOBUIableWidget(items.count(), 2);

    for (int row = 0; row < items.count(); ++row) {
        BOBUIableWidgetItem *name = new BOBUIableWidgetItem(items[row]);
        name->setFlags(BobUI::ItemIsEnabled | BobUI::ItemIsSelectable);
        itemsTable->setItem(row, 0, name);
        BOBUIableWidgetItem *quantity = new BOBUIableWidgetItem("1");
        itemsTable->setItem(row, 1, quantity);
    }
}
//! [2]

//! [3]
QList<QPair<QString, int> > DetailsDialog::orderItems()
{
    QList<QPair<QString, int> > orderList;

    for (int row = 0; row < items.count(); ++row) {
        QPair<QString, int> item;
        item.first = itemsTable->item(row, 0)->text();
        int quantity = itemsTable->item(row, 1)->data(BobUI::DisplayRole).toInt();
        item.second = qMax(0, quantity);
        orderList.append(item);
    }

    return orderList;
}
//! [3]

//! [4]
QString DetailsDialog::senderName() const
{
    return nameEdit->text();
}
//! [4]

//! [5]
QString DetailsDialog::senderAddress() const
{
    return addressEdit->toPlainText();
}
//! [5]

//! [6]
bool DetailsDialog::sendOffers()
{
    return offersCheckBox->isChecked();
}
//! [6]

//! [7]
void DetailsDialog::verify()
{
    if (!nameEdit->text().isEmpty() && !addressEdit->toPlainText().isEmpty()) {
        accept();
        return;
    }

    QMessageBox::StandardButton answer;
    answer = QMessageBox::warning(this, tr("Incomplete Form"),
        tr("The form does not contain all the necessary information.\n"
           "Do you want to discard it?"),
        QMessageBox::Yes | QMessageBox::No);

    if (answer == QMessageBox::Yes)
        reject();
}
//! [7]
