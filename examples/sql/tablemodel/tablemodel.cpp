// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause
#include "../connection.h"

#include <QApplication>
#include <QSqlTableModel>
#include <BOBUIableView>

void initializeModel(QSqlTableModel *model)
{
    model->setTable("person");
    model->setEditStrategy(QSqlTableModel::OnManualSubmit);
    model->select();

    model->setHeaderData(0, BobUI::Horizontal, QObject::tr("ID"));
    model->setHeaderData(1, BobUI::Horizontal, QObject::tr("First name"));
    model->setHeaderData(2, BobUI::Horizontal, QObject::tr("Last name"));
}

BOBUIableView *createView(QSqlTableModel *model, const QString &title = "")
{
    BOBUIableView *view = new BOBUIableView;
    view->setModel(model);
    view->setWindowTitle(title);
    return view;
}

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    if (!createConnection())
        return EXIT_FAILURE;

    QSqlTableModel model;

    initializeModel(&model);

    BOBUIableView *view1 = createView(&model, QObject::tr("Table Model (View 1)"));
    BOBUIableView *view2 = createView(&model, QObject::tr("Table Model (View 2)"));

    view1->show();
    view2->move(view1->x() + view1->width() + 20, view1->y());
    view2->show();

    return app.exec();
}
