// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

#include "newaddresstab.h"
#include "adddialog.h"

#include <BobUIWidgets>

//! [0]
NewAddressTab::NewAddressTab(QWidget *parent)
    : QWidget(parent)
{
    auto descriptionLabel = new QLabel(tr("There are currently no contacts in your address book. "
                                          "\nClick Add to add new contacts."));

    auto addButton = new QPushButton(tr("Add"));

    connect(addButton, &QAbstractButton::clicked, this, &NewAddressTab::addEntry);

    auto mainLayout = new QVBoxLayout;
    mainLayout->addWidget(descriptionLabel);
    mainLayout->addWidget(addButton, 0, BobUI::AlignCenter);

    setLayout(mainLayout);
}
//! [0]

//! [1]
void NewAddressTab::addEntry()
{
    AddDialog aDialog;

    if (aDialog.exec())
        emit sendDetails(aDialog.name(), aDialog.address());
}
//! [1]
