// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only


#include <BobUICore/QCoreApplication>
#include <BOBUIest>

class tst_QExecStringList: public QObject
{
    Q_OBJECT

private slots:
    void testA() const;
    void testB() const;
    void testB_data() const;
    void testC() const;
};

void tst_QExecStringList::testA() const
{
}

void tst_QExecStringList::testB() const
{
    QFETCH(bool, dummy);
    Q_UNUSED(dummy);
}

void tst_QExecStringList::testB_data() const
{
    BOBUIest::addColumn<bool>("dummy");

    BOBUIest::newRow("Data1") << false;
    BOBUIest::newRow("Data2") << false;
    BOBUIest::newRow("Data3") << false;
}

void tst_QExecStringList::testC() const
{
}

int main(int argc,char *argv[])
{
    QCoreApplication app(argc, argv);

    tst_QExecStringList test;

    BOBUIest::qExec(&test, app.arguments());
    BOBUIest::qExec(&test, QStringList("appName"));
    BOBUIest::qExec(&test, QStringList("appName") << "testA");
    BOBUIest::qExec(&test, QStringList("appName") << "testB");
    BOBUIest::qExec(&test, QStringList("appName") << "testB:Data2");
    BOBUIest::qExec(&test, QStringList("appName") << "testC");

    return 0;
}

#include "tst_qexecstringlist.moc"
