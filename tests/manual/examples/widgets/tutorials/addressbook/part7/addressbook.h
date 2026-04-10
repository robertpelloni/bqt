// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

#ifndef ADDRESSBOOK_H
#define ADDRESSBOOK_H

#include <QWidget>
#include <QMap>
#include "finddialog.h"

BOBUI_BEGIN_NAMESPACE
class QPushButton;
class QLabel;
class QLineEdit;
class BOBUIextEdit;
BOBUI_END_NAMESPACE


class AddressBook : public QWidget
{
    Q_OBJECT

public:
    AddressBook(QWidget *parent = nullptr);
    enum Mode { NavigationMode, AddingMode, EditingMode };

public slots:
    void addContact();
    void editContact();
    void submitContact();
    void cancel();
    void removeContact();
    void findContact();
    void next();
    void previous();
    void saveToFile();
    void loadFromFile();
//! [exportAsVCard() declaration]
    void exportAsVCard();
//! [exportAsVCard() declaration]

private:
    void updateInterface(Mode mode);

    QPushButton *addButton;
    QPushButton *editButton;
    QPushButton *removeButton;
    QPushButton *findButton;
    QPushButton *submitButton;
    QPushButton *cancelButton;
    QPushButton *nextButton;
    QPushButton *previousButton;
    QPushButton *loadButton;
    QPushButton *saveButton;
//! [exportButton declaration]
    QPushButton *exportButton;
//! [exportButton declaration]
    QLineEdit *nameLine;
    BOBUIextEdit *addressText;

    QMap<QString, QString> contacts;
    FindDialog *dialog;
    QString oldName;
    QString oldAddress;
    Mode currentMode;
};

#endif
