// Copyright (C) 2021 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

#include "subscriptionwindow.h"
#include "ui_subscriptionwindow.h"

SubscriptionWindow::SubscriptionWindow(QWidget *parent)
    : QWidget(parent), ui(new Ui::SubscriptionWindow)
{
    ui->setupUi(this);
}

SubscriptionWindow::~SubscriptionWindow()
{
    delete ui;
}
