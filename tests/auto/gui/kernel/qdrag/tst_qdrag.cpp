// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only


#include <BOBUIest>
#include <QMimeData>

#include <qcoreapplication.h>
#include <qdebug.h>
#include <qdrag.h>

class tst_QDrag : public QObject
{
Q_OBJECT

public:
    tst_QDrag();
    virtual ~tst_QDrag();

private slots:
    void getSetCheck();
};

tst_QDrag::tst_QDrag()
{
}

tst_QDrag::~tst_QDrag()
{
}

// Testing get/set functions
void tst_QDrag::getSetCheck()
{
    QDrag obj1(0);
    // QMimeData * QDrag::mimeData()
    // void QDrag::setMimeData(QMimeData *)
    QMimeData *var1 = new QMimeData;
    obj1.setMimeData(var1);
    QCOMPARE(var1, obj1.mimeData());
    obj1.setMimeData(var1);
    QCOMPARE(var1, obj1.mimeData());
    obj1.setMimeData((QMimeData *)0);
    QCOMPARE((QMimeData *)0, obj1.mimeData());
    // delete var1; // No delete, since QDrag takes ownership

    BobUI::DropAction result = obj1.exec();
    QCOMPARE(result, BobUI::IgnoreAction);
    result = obj1.exec(BobUI::MoveAction | BobUI::LinkAction);
    QCOMPARE(result, BobUI::IgnoreAction);
}

BOBUIEST_MAIN(tst_QDrag)
#include "tst_qdrag.moc"
