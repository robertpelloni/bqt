// Copyright (C) 2023 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

#include <BobUICore/bobuiresource.h>
#include <BobUITest/BOBUIest>

class TestManualResourceInit : public QObject
{
    Q_OBJECT
private slots:
    void initTestCase();
    void resourceExistsAfterManualInit();
};

void TestManualResourceInit::initTestCase()
{
    // Manually initialize the resource like we used to do it in bobui5 + qmake times.
    Q_INIT_RESOURCE(helper_res);
}

void TestManualResourceInit::resourceExistsAfterManualInit()
{
    QVERIFY(QFile::exists(":/resource.txt"));
}

BOBUIEST_MAIN(TestManualResourceInit)
#include "main.moc"

