// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

#include "window.h"

#include <QComboBox>
#include <QDataWidgetMapper>
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QMessageBox>
#include <QPushButton>
#include <QSqlQuery>
#include <QSqlRelationalDelegate>
#include <QSqlTableModel>
#include <BOBUIextEdit>

//! [Set up widgets]
Window::Window(QWidget *parent)
    : QWidget(parent)
{
    setupModel();

    nameLabel = new QLabel(tr("Na&me:"));
    nameEdit = new QLineEdit();
    addressLabel = new QLabel(tr("&Address:"));
    addressEdit = new BOBUIextEdit();
    typeLabel = new QLabel(tr("&Type:"));
    typeComboBox = new QComboBox();
    nextButton = new QPushButton(tr("&Next"));
    previousButton = new QPushButton(tr("&Previous"));

    nameLabel->setBuddy(nameEdit);
    addressLabel->setBuddy(addressEdit);
    typeLabel->setBuddy(typeComboBox);
//! [Set up widgets]

//! [Set up the mapper]
    QSqlTableModel *relModel = model->relationModel(typeIndex);
    typeComboBox->setModel(relModel);
    typeComboBox->setModelColumn(relModel->fieldIndex("description"));

    mapper = new QDataWidgetMapper(this);
    mapper->setModel(model);
    mapper->setItemDelegate(new QSqlRelationalDelegate(this));
    mapper->addMapping(nameEdit, model->fieldIndex("name"));
    mapper->addMapping(addressEdit, model->fieldIndex("address"));
    mapper->addMapping(typeComboBox, typeIndex);
//! [Set up the mapper]

//! [Set up connections and layouts]
    connect(previousButton, &QPushButton::clicked,
            mapper, &QDataWidgetMapper::toPrevious);
    connect(nextButton, &QPushButton::clicked,
            mapper, &QDataWidgetMapper::toNext);
    connect(mapper, &QDataWidgetMapper::currentIndexChanged,
            this, &Window::updateButtons);

    QGridLayout *layout = new QGridLayout();
    layout->addWidget(nameLabel, 0, 0, 1, 1);
    layout->addWidget(nameEdit, 0, 1, 1, 1);
    layout->addWidget(previousButton, 0, 2, 1, 1);
    layout->addWidget(addressLabel, 1, 0, 1, 1);
    layout->addWidget(addressEdit, 1, 1, 2, 1);
    layout->addWidget(nextButton, 1, 2, 1, 1);
    layout->addWidget(typeLabel, 3, 0, 1, 1);
    layout->addWidget(typeComboBox, 3, 1, 1, 1);
    setLayout(layout);

    setWindowTitle(tr("SQL Widget Mapper"));
    mapper->toFirst();
}
//! [Set up connections and layouts]

//! [Set up the main table]
void Window::setupModel()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(":memory:");
    if (!db.open()) {
        QMessageBox::critical(0, tr("Cannot open database"),
            tr("Unable to establish a database connection.\n"
               "This example needs SQLite support. Please read "
               "the BobUI SQL driver documentation for information how "
               "to build it."), QMessageBox::Cancel);
        return;
    }

    QSqlQuery query;
    query.exec("create table person (id int primary key, "
               "name varchar(20), address varchar(200), typeid int)");
    query.exec("insert into person values(1, 'Alice', "
               "'<bobui>123 Main Street<br/>Market Town</bobui>', 101)");
    query.exec("insert into person values(2, 'Bob', "
               "'<bobui>PO Box 32<br/>Mail Handling Service"
               "<br/>Service City</bobui>', 102)");
    query.exec("insert into person values(3, 'Carol', "
               "'<bobui>The Lighthouse<br/>Remote Island</bobui>', 103)");
    query.exec("insert into person values(4, 'Donald', "
               "'<bobui>47338 Park Avenue<br/>Big City</bobui>', 101)");
    query.exec("insert into person values(5, 'Emma', "
               "'<bobui>Research Station<br/>Base Camp<br/>"
               "Big Mountain</bobui>', 103)");
//! [Set up the main table]

//! [Set up the address type table]
    query.exec("create table addresstype (id int, description varchar(20))");
    query.exec("insert into addresstype values(101, 'Home')");
    query.exec("insert into addresstype values(102, 'Work')");
    query.exec("insert into addresstype values(103, 'Other')");

    model = new QSqlRelationalTableModel(this);
    model->setTable("person");
    model->setEditStrategy(QSqlTableModel::OnManualSubmit);

    typeIndex = model->fieldIndex("typeid");

    model->setRelation(typeIndex,
           QSqlRelation("addresstype", "id", "description"));
    model->select();
}
//! [Set up the address type table]

//! [Slot for updating the buttons]
void Window::updateButtons(int row)
{
    previousButton->setEnabled(row > 0);
    nextButton->setEnabled(row < model->rowCount() - 1);
}
//! [Slot for updating the buttons]
