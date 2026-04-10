// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

#ifndef ADDRESSBOOK_H
#define ADDRESSBOOK_H

#include <QWidget>
#include <QMap>

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
//! [Mode enum]
    enum Mode { NavigationMode, AddingMode, EditingMode };
//! [Mode enum]

public slots:
    void addContact();
    void submitContact();
    void cancel();
//! [edit and remove slots]
    void editContact();
    void removeContact();
//! [edit and remove slots]
    void next();
    void previous();

private:
//! [updateInterface() declaration]
    void updateInterface(Mode mode);
//! [updateInterface() declaration]
    QPushButton *addButton;
//! [buttons declaration]
    QPushButton *editButton;
    QPushButton *removeButton;
//! [buttons declaration]
    QPushButton *submitButton;
    QPushButton *cancelButton;
    QPushButton *nextButton;
    QPushButton *previousButton;
    QLineEdit *nameLine;
    BOBUIextEdit *addressText;

    QMap<QString, QString> contacts;
    QString oldName;
    QString oldAddress;
//! [mode declaration]
    Mode currentMode;
//! [mode declaration]
};

#endif
