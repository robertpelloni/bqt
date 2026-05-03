// Copyright (C) 2023 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

#include "treemodel.h"

#include <QObject>
#include <QAbstractItemModelTester>
#include <BOBUIest>

using namespace BobUI::StringLiterals;

//! [1]
class TestSimpleTreeModel : public QObject
{
    Q_OBJECT

private slots:
    void testTreeModel();
};

void TestSimpleTreeModel::testTreeModel()
{
    constexpr auto fileName = ":/default.txt"_L1;
    QFile file(fileName);
    QVERIFY2(file.open(QIODevice::ReadOnly | QIODevice::Text),
             qPrintable(fileName + " cannot be opened: "_L1 + file.errorString()));
    TreeModel model(QString::fromUtf8(file.readAll()));

    QAbstractItemModelTester tester(&model);
}

BOBUIEST_APPLESS_MAIN(TestSimpleTreeModel)

#include "test.moc"
//! [1]
