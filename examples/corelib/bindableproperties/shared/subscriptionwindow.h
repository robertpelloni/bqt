// Copyright (C) 2021 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

#ifndef SUBSCRIPTIONWINDOW_H
#define SUBSCRIPTIONWINDOW_H

#include <QWidget>

BOBUI_BEGIN_NAMESPACE
namespace Ui {
class SubscriptionWindow;
}
BOBUI_END_NAMESPACE

class User;

class SubscriptionWindow : public QWidget
{
    Q_OBJECT

public:
    explicit SubscriptionWindow(QWidget *parent = nullptr);
    ~SubscriptionWindow();

private:
    Ui::SubscriptionWindow *ui;
};

#endif // SUBSCRIPTIONWINDOW_H
