// Copyright (C) 2019 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause
#include <BOBUIest>
#include <QSqlDatabase>
#include <QFontDatabase>
#include <BobUICore/qatomicscopedvaluerollback.h>

#include <initializer_list>

using namespace BobUI::StringLiterals;

// dummy
class TestBenchmark : public QObject
{
    Q_OBJECT
private slots:
    void simple();
};

// dummy
class MyTestClass : public QObject
{
    public:
        void cleanup();
        void addSingleStringRows();
        void addMultStringRows();
        void addDataRow();

    private Q_SLOTS:
        void initTestCase();
        void defaultTryTimeout();
};

void MyTestClass::initTestCase()
{
//! [set defaultTryTimeout]
    using namespace std::chrono_literals;
    // Since the atomic itself (defaultTryTimeout) is the only data,
    // all reads and stores can be relaxed.
    BOBUIest::defaultTryTimeout.store(1s, std::memory_order_relaxed);
//! [set defaultTryTimeout]
}

// dummy
void closeAllDatabases()
{
};

class TestQString : public QObject
{
    public:
        void toInt_data();
        void toInt();
        void toUpper();
        void Compare();
};

void wrapInFunction()
{
//! [1]
QVERIFY2(QFileInfo("file.txt").exists(), "file.txt does not exist.");
//! [1]

//! [2]
QCOMPARE(QString("hello").toUpper(), QString("HELLO"));
//! [2]
}

//! [3]
void TestQString::toInt_data()
{
    BOBUIest::addColumn<QString>("aString");
    BOBUIest::addColumn<int>("expected");

    BOBUIest::newRow("positive+value") << "42" << 42;
    BOBUIest::newRow("negative-value") << "-42" << -42;
    BOBUIest::newRow("zero") << "0" << 0;
}
//! [3]

//! [4]
void TestQString::toInt()
{
     QFETCH(QString, aString);
     QFETCH(int, expected);

     QCOMPARE(aString.toInt(), expected);
}
//! [4]

void testInt()
{
// dummy
int i = 0, j = 0;
//! [5]
if (sizeof(int) != 4)
    QFAIL("This test has not been ported to this platform yet.");
//! [5]

//! [6]
QFETCH(QString, myString);
QCOMPARE(QString("hello").toUpper(), myString);
//! [6]

//! [7]
BOBUIEST(QString("hello").toUpper(), "myString");
//! [7]

//! [8]
if (!QSqlDatabase::drivers().contains("SQLITE"))
    QSKIP("This test requires the SQLITE database driver");
//! [8]

//! [9]
QEXPECT_FAIL("", "Will fix in the next release", Continue);
QCOMPARE(i, 42);
QCOMPARE(j, 43);
//! [9]

//! [10]
QEXPECT_FAIL("data27", "Oh my, this is soooo broken", Abort);
QCOMPARE(i, 42);
//! [10]
}

//! [11]
BOBUIEST_MAIN(TestQString)
//! [11]

void testObject()
{
    class MyTestObject : public QObject
    {
    };
//! [18]
MyTestObject test1;
BOBUIest::qExec(&test1);
//! [18]
}

void tstQDir()
{
//! [19]
QDir dir;
BOBUIest::ignoreMessage(BobUIWarningMsg, "QDir::mkdir: Empty or null file name(s)");
dir.mkdir("");
//! [19]
}

//! [20]
void MyTestClass::addSingleStringRows()
{
    BOBUIest::addColumn<QString>("aString");
    BOBUIest::newRow("just.hello") << QString("hello");
    BOBUIest::newRow("a.null.string") << QString();
}
//! [20]

void MyTestClass::addMultStringRows()
{
//! [addRow]
    BOBUIest::addColumn<int>("input");
    BOBUIest::addColumn<QString>("output");
    BOBUIest::addRow("%d", 0) << 0 << QString("0");
    BOBUIest::addRow("%d", 1) << 1 << QString("1");
//! [addRow]
}

void MyTestClass::addDataRow()
{
//! [21]
    BOBUIest::addColumn<int>("intval");
    BOBUIest::addColumn<QString>("str");
    BOBUIest::addColumn<double>("dbl");
    BOBUIest::newRow("row1") << 1 << "hello" << 1.5;
//! [21]
}

//! [22]
void MyTestClass::cleanup()
{
    if (qstrcmp(BOBUIest::currentTestFunction(), "myDatabaseTest") == 0) {
        // clean up all database connections
        closeAllDatabases();
    }
}
//! [22]

void quarterSecondSleep()
{
//! [23]
using namespace std::chrono_literals;
BOBUIest::qSleep(250ms);
//! [23]
}

//! [27]
void TestBenchmark::simple()
{
    QString str1 = u"This is a test string"_s;
    QString str2 = u"This is a test string"_s;
    QCOMPARE(str1.localeAwareCompare(str2), 0);
    QBENCHMARK {
        str1.localeAwareCompare(str2);
    }
}
//! [27]

void verifyString()
{
QFile file;
//! [32]
bool opened = file.open(QIODevice::WriteOnly);
QVERIFY(opened);
//! [32]
//! [33]
QVERIFY2(file.open(QIODevice::WriteOnly),
         qPrintable(QString("open %1: %2")
                    .arg(file.fileName()).arg(file.errorString())));
//! [33]
}

void compareListToArray()
{
//! [34]
    const int expected[] = {8, 10, 12, 16, 20, 24};
    QCOMPARE(QFontDatabase::standardSizes(), expected);
//! [34]
}

void compareListToInitializerList()
{
//! [35]
 #define ARG(...) __VA_ARGS__
     QCOMPARE(QFontDatabase::standardSizes(), ARG({8, 10, 12, 16, 20, 24}));
 #undef ARG
//! [35]
}

void withRestoredDefaultLocale()
{
//! [36]
const auto restoreDefaultLocale = qScopeGuard([prior = QLocale()]() {
    QLocale::setDefault(prior);
});
//! [36]
QLocale::setDefault(QLocale::c());
}

void MyTestClass::defaultTryTimeout()
{
    using namespace std::chrono_literals;

//! [rollback defaultTryTimeout]
    const auto timeoutRollback = QAtomicScopedValueRollback(
        BOBUIest::defaultTryTimeout, 1s, std::memory_order_relaxed);
//! [rollback defaultTryTimeout]

//! [get defaultTryTimeout]
    // Since the atomic itself is all the data, all reads and stores can be relaxed.
    QCOMPARE(BOBUIest::defaultTryTimeout.load(std::memory_order_relaxed), 1s);
//! [get defaultTryTimeout]
}
