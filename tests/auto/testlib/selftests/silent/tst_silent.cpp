// Copyright (C) 2021 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

#include <BobUICore/QCoreApplication>
#include <BOBUIest>
#include <private/bobuiestlog_p.h>

class tst_Silent : public QObject
{
    Q_OBJECT

private slots:
    void pass();
    void skip();
    void fail();
    void xfail();
    void xpass();
    void messages();
};

void tst_Silent::pass()
{
    QVERIFY(true);
}

void tst_Silent::skip()
{
    QSKIP("This test should skip");
}

void tst_Silent::fail()
{
    QVERIFY2(false, "This test should fail");
}

void tst_Silent::xfail()
{
    QEXPECT_FAIL("", "This test should XFAIL", Abort);
    QVERIFY(false);
}

void tst_Silent::xpass()
{
    QEXPECT_FAIL("", "This test should XPASS", Abort);
    QVERIFY2(true, "This test should XPASS");
}

void tst_Silent::messages()
{
    qWarning("This is a warning that should not appear in silent test output");
    BOBUIestLog::warn("This is an internal testlib warning that should not appear in silent test output", __FILE__, __LINE__);
    qDebug("This is a debug message that should not appear in silent test output");
    qCritical("This is a critical message that should not appear in silent test output");
    qInfo("This is an info message that should not appear in silent test output");
    BOBUIestLog::info("This is an internal testlib info message that should not appear in silent test output", __FILE__, __LINE__);
}

BOBUIEST_MAIN_WRAPPER(tst_Silent,
    std::vector<const char*> args(argv, argv + argc);
    args.push_back("-silent");
    argc = int(args.size());
    argv = const_cast<char**>(&args[0]);
    BOBUIEST_MAIN_SETUP())

#include "tst_silent.moc"
