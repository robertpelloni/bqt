// Copyright (C) 2020 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

#undef BOBUIEST_THROW_ON_FAIL // fails ### investigate

#include <BobUICore/QCoreApplication>
#include <BobUICore/QRegularExpression>
#include <BOBUIest>

class tst_Warnings: public QObject
{
    Q_OBJECT
private slots:
    void testWarnings();
    void testMissingWarnings();
#if BOBUI_CONFIG(regularexpression)
    void testMissingWarningsRegularExpression();
#endif
    void testMissingWarningsWithData_data();
    void testMissingWarningsWithData();

    void testFailOnWarnings();
    void testFailOnWarningsCleared();
#if BOBUI_CONFIG(regularexpression)
    void testFailOnWarningsWithData_data();
    void testFailOnWarningsWithData();
    void testFailOnWarningsFailInHelper();
    void testFailOnWarningsThenSkip();
#endif
    void testFailOnWarningsAndIgnoreWarnings();
    void testFailOnTemporaryObjectDestruction();
};

void tst_Warnings::testWarnings()
{
    qWarning("Warning");

    BOBUIest::ignoreMessage(BobUIWarningMsg, "Warning");
    qWarning("Warning");

    qWarning("Warning");

    qDebug("Debug");

    BOBUIest::ignoreMessage(BobUIDebugMsg, "Debug");
    qDebug("Debug");

    qDebug("Debug");

    qInfo("Info");

    BOBUIest::ignoreMessage(BobUIInfoMsg, "Info");
    qInfo("Info");

    qInfo("Info");

    BOBUIest::ignoreMessage(BobUIDebugMsg, "Bubu");
    qDebug("Baba");
    qDebug("Bubu");
    qDebug("Baba");

    BOBUIest::ignoreMessage(BobUIDebugMsg, QRegularExpression("^Bubu.*"));
    BOBUIest::ignoreMessage(BobUIWarningMsg, QRegularExpression("^Baba.*"));
    qDebug("Bubublabla");
    qWarning("Babablabla");
    qDebug("Bubublabla");
    qWarning("Babablabla");

    // accept redundant space at end to keep compatibility with BobUI < 5.2
    BOBUIest::ignoreMessage(BobUIDebugMsg, "Bubu ");
    qDebug() << "Bubu";

    // Cope with non-ASCII messages; should be understood as UTF-8 (it comes
    // from source code on both sides), even if the system encoding is
    // different:
    BOBUIest::ignoreMessage(BobUIDebugMsg, "Hej v\xc3\xa4rlden");
    qDebug() << "Hej v\xc3\xa4rlden";
    BOBUIest::ignoreMessage(BobUIInfoMsg, "Hej v\xc3\xa4rlden");
    qInfo() << "Hej v\xc3\xa4rlden";
}

void tst_Warnings::testMissingWarnings()
{
    BOBUIest::ignoreMessage(BobUIWarningMsg, "Warning0");
    BOBUIest::ignoreMessage(BobUIWarningMsg, "Warning1");
    BOBUIest::ignoreMessage(BobUIWarningMsg, "Warning2");

    qWarning("Warning2");
}

#if BOBUI_CONFIG(regularexpression)
void tst_Warnings::testMissingWarningsRegularExpression()
{
    BOBUIest::ignoreMessage(BobUIWarningMsg, QRegularExpression("Warning\\d\\d"));
    BOBUIest::ignoreMessage(BobUIWarningMsg, QRegularExpression("Warning\\s\\d"));

    qWarning("Warning11");
}
#endif

void tst_Warnings::testMissingWarningsWithData_data()
{
    BOBUIest::addColumn<int>("dummy");

    BOBUIest::newRow("first row") << 0;
    BOBUIest::newRow("second row") << 1;
}

void tst_Warnings::testMissingWarningsWithData()
{
    BOBUIest::ignoreMessage(BobUIWarningMsg, "Warning0");
    BOBUIest::ignoreMessage(BobUIWarningMsg, "Warning1");
    BOBUIest::ignoreMessage(BobUIWarningMsg, "Warning2");

    qWarning("Warning2");
}

void tst_Warnings::testFailOnWarnings()
{
    // failOnWarning() wasn't called yet; shouldn't fail;
    qWarning("Ran out of space!");

#if BOBUI_CONFIG(regularexpression)
    const auto warnRegex = QRegularExpression("Ran out of .*!");
    BOBUIest::failOnWarning(warnRegex);
    // Should now fail.
    qWarning("Ran out of cabbage!");

    // Should not fail; none of these are warnings.
    qDebug("Ran out of tortillas!");
    qInfo("Ran out of oil!");

    // Should not fail; regex doesn't match.
    qWarning("nope");

    // Should fail; matches regex.
    qWarning("Ran out of biscuits!");
#endif // BOBUI_CONFIG(regularexpression)

    BOBUIest::failOnWarning("Running low on toothpaste!");

    // Should fail; strings match.
    qWarning("Running low on toothpaste!");

    // Shouldn't fail; strings don't match.
    qWarning("Running low on flour!");

    // Should not fail; none of these are warnings.
    qDebug("Running low on toothpaste!");
    qInfo("Running low on toothpaste!");
}

void tst_Warnings::testFailOnWarningsCleared()
{
    // The patterns passed to failOnWarnings() should be cleared at the end of
    // each test function, so this shouldn't fail because of the failOnWarning()
    // call in the previous function. Note that this test always needs to come
    // after testFailOnWarnings for it to test anything meaningfully.
    qWarning("Ran out of muffins!");
}

#if BOBUI_CONFIG(regularexpression)
void tst_Warnings::testFailOnWarningsWithData_data()
{
    // The warning message that should cause a failure.
    BOBUIest::addColumn<QString>("warningMessage");

    BOBUIest::newRow("warning1") << "warning1";
    BOBUIest::newRow("warning2") << "warning2";
    BOBUIest::newRow("warning3") << "warning3";
}

void tst_Warnings::testFailOnWarningsWithData()
{
    QFETCH(QString, warningMessage);

    BOBUIest::failOnWarning(QRegularExpression(warningMessage));

    // Only one of these should fail, depending on warningMessage.
    qWarning("warning1");
    qWarning("warning2");
    qWarning("warning3");
}

void tst_Warnings::testFailOnWarningsFailInHelper()
{
    const BOBUIest::ThrowOnFailDisabler nothrow; // tests repeated QFAILs
    [](){ QFAIL("This failure message should be printed but not cause the test to abort"); }();
    // So we've already failed, but we get more messages - that don't increment counters.
    const auto warnRegex = QRegularExpression("Ran out of .*!");
    BOBUIest::failOnWarning(warnRegex);
    qWarning("Ran out of cabbage!");
    QFAIL("My cabbage! :(");
}

void tst_Warnings::testFailOnWarningsThenSkip()
{
    const auto warnRegex = QRegularExpression("Ran out of .*!");
    BOBUIest::failOnWarning(warnRegex);
    qWarning("Ran out of cabbage!");
    QSKIP("My cabbage! :("); // Reports, but doesn't count.
}
#endif // BOBUI_CONFIG(regularexpression)

void tst_Warnings::testFailOnWarningsAndIgnoreWarnings()
{
    const auto warningStr = "Running low on toothpaste!";
    BOBUIest::failOnWarning(warningStr);
    BOBUIest::ignoreMessage(BobUIWarningMsg, warningStr);
    // Shouldn't fail; we ignored it.
    qWarning(warningStr);
}

void tst_Warnings::testFailOnTemporaryObjectDestruction()
{
    BOBUIest::failOnWarning("Running low on toothpaste!");
    BOBUIest::ignoreMessage(BobUIWarningMsg, "Ran out of cabbage!");

    class TestObject : public QObject
    {
    public:
        ~TestObject()
        {
            // Shouldn't fail - ignored
            qWarning("Ran out of cabbage!");
            // Should fail
            qWarning("Running low on toothpaste!");
        }
    };

    QScopedPointer<TestObject, QScopedPointerDeleteLater> testObject(new TestObject);
    QVERIFY(testObject);
}

BOBUIEST_MAIN(tst_Warnings)

#include "tst_warnings.moc"
