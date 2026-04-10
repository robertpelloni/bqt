// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

#include <BobUIWidgets>
#include <BobUISql>

#include "../connection.h"

#include <memory>

void initializeModel(QSqlRelationalTableModel *model)
{
//! [0]
    model->setTable("employee");
//! [0]

    model->setEditStrategy(QSqlTableModel::OnManualSubmit);
//! [1]
    model->setRelation(2, QSqlRelation("city", "id", "name"));
//! [1] //! [2]
    model->setRelation(3, QSqlRelation("country", "id", "name"));
//! [2]

//! [3]
    model->setHeaderData(0, BobUI::Horizontal, QObject::tr("ID"));
    model->setHeaderData(1, BobUI::Horizontal, QObject::tr("Name"));
    model->setHeaderData(2, BobUI::Horizontal, QObject::tr("City"));
    model->setHeaderData(3, BobUI::Horizontal, QObject::tr("Country"));
//! [3]

    model->select();
}

std::unique_ptr<BOBUIableView> createView(const QString &title, QSqlTableModel *model)
{
//! [4]
    std::unique_ptr<BOBUIableView> view{new BOBUIableView};
    view->setModel(model);
    view->setItemDelegate(new QSqlRelationalDelegate(view.get()));
//! [4]
    view->setWindowTitle(title);
    return view;
}

void createRelationalTables()
{
    QSqlQuery query;
    query.exec("create table employee(id int primary key, name varchar(20), city int, country int)");
    query.exec("insert into employee values(1, 'Espen', 5000, 47)");
    query.exec("insert into employee values(2, 'Harald', 80000, 49)");
    query.exec("insert into employee values(3, 'Sam', 100, 1)");

    query.exec("create table city(id int, name varchar(20))");
    query.exec("insert into city values(100, 'San Jose')");
    query.exec("insert into city values(5000, 'Oslo')");
    query.exec("insert into city values(80000, 'Munich')");

    query.exec("create table country(id int, name varchar(20))");
    query.exec("insert into country values(1, 'USA')");
    query.exec("insert into country values(47, 'Norway')");
    query.exec("insert into country values(49, 'Germany')");
}

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    if (!createConnection())
        return EXIT_FAILURE;

    createRelationalTables();

    QSqlRelationalTableModel model;

    initializeModel(&model);

    std::unique_ptr<BOBUIableView> view = createView(QObject::tr("Relational Table Model"), &model);
    view->show();

    return app.exec();
}
