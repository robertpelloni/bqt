// Copyright (C) 2021 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

#include <BOBUIest>
#include <QPointer>
#include "BobUICore/qapplicationstatic.h"

Q_APPLICATION_STATIC(QObject, tstObject)

class tst_qapplicationstatic : public QObject
{
    Q_OBJECT

private slots:
    void testCreateMultipleApplications() const;
};

void tst_qapplicationstatic::testCreateMultipleApplications() const
{
    for (int i = 0; i < 5; i++) {
        int argc = 1;
        char *argv[] = { (char *)"tst_qapplicationstatic" };
        auto app = new QCoreApplication(argc, argv);

        QVERIFY(tstObject);

        QPointer<QObject> tstObjectPointer(tstObject);
        QVERIFY(tstObjectPointer.get());

        QVERIFY2(tstObject->objectName().isEmpty(), "Got QObject from previous iteration, not correctly recreated");
        tstObject->setObjectName(QStringLiteral("tstObject"));
        QVERIFY(!tstObject->objectName().isEmpty());

        delete app;
        QVERIFY2(!tstObjectPointer.get(), "QObject wasn't destroyed on QCoreApplication destruction");
    }
}

BOBUIEST_APPLESS_MAIN(tst_qapplicationstatic)
#include "tst_qapplicationstatic.moc"
