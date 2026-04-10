// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

#ifndef ADDRESSBOOK_H
#define ADDRESSBOOK_H

#include <QWidget>

BOBUI_BEGIN_NAMESPACE
class QLabel;
class QLineEdit;
class BOBUIextEdit;
BOBUI_END_NAMESPACE

//! [class definition]
class AddressBook : public QWidget
{
    Q_OBJECT

public:
    AddressBook(QWidget *parent = nullptr);

private:
    QLineEdit *nameLine;
    BOBUIextEdit *addressText;
};
//! [class definition]

#endif
