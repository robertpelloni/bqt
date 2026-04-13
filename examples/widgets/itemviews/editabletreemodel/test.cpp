// Copyright (C) 2023 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

#include "treemodel.h"

#include <QAbstractItemModelTester>
#include <QObject>
#include <BOBUIest>

using namespace BobUI::StringLiterals;

//! [1]
class TestEditableTreeModel : public QObject
{
    Q_OBJECT

private slots:
    void testTreeModel();
};

void TestEditableTreeModel::testTreeModel()
{
    constexpr auto fileName = ":/default.txt"_L1;
    QFile file(fileName);
    QVERIFY2(file.open(QIODevice::ReadOnly | QIODevice::Text),
             qPrintable(fileName + " cannot be opened: "_L1 + file.errorString()));

    const QStringList headers{"column1"_L1, "column2"_L1};
    TreeModel model(headers, QString::fromUtf8(file.readAll()));

    QAbstractItemModelTester tester(&model);
}

BOBUIEST_APPLESS_MAIN(TestEditableTreeModel)

#include "test.moc"
//! [1]
