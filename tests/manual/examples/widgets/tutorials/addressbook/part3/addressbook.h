// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

#ifndef ADDRESSBOOK_H
#define ADDRESSBOOK_H

#include <QWidget>
#include <QMap>

BOBUI_BEGIN_NAMESPACE
class QLabel;
class QLineEdit;
class QPushButton;
class BOBUIextEdit;
BOBUI_END_NAMESPACE

class AddressBook : public QWidget
{
    Q_OBJECT

public:
    AddressBook(QWidget *parent = nullptr);

public slots:
    void addContact();
    void submitContact();
    void cancel();
//! [navigation functions]
    void next();
    void previous();
//! [navigation functions]

private:
    QPushButton *addButton;
    QPushButton *submitButton;
    QPushButton *cancelButton;
//! [navigation pushbuttons]
    QPushButton *nextButton;
    QPushButton *previousButton;
//! [navigation pushbuttons]
    QLineEdit *nameLine;
    BOBUIextEdit *addressText;

    QMap<QString, QString> contacts;
    QString oldName;
    QString oldAddress;
};

#endif
