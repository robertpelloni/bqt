// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

#include <BOBUIest>
#include <QScopedValueRollback>

#include <bobuiextboundaryfinder.h>
#include <qfile.h>
#include <qdebug.h>
#include <qlist.h>
#include <qset.h>

#include <algorithm>

using namespace BobUI::StringLiterals;

class tst_BOBUIextBoundaryFinder : public QObject
{
    Q_OBJECT
private slots:
    void moveSemantics();

#ifdef BOBUI_BUILD_INTERNAL
    void graphemeBoundariesDefault_data();
    void graphemeBoundariesDefault();
    void wordBoundariesDefault_data();
    void wordBoundariesDefault();
    void sentenceBoundariesDefault_data();
    void sentenceBoundariesDefault();
    void lineBoundariesDefault_data();
    void lineBoundariesDefault();
#endif

    void graphemeBoundaries_manual_data();
    void graphemeBoundaries_manual();

    void wordBoundaries_manual_data();
    void wordBoundaries_manual();
    void sentenceBoundaries_manual_data();
    void sentenceBoundaries_manual();
    void lineBoundaries_manual_data();
    void lineBoundaries_manual();

    void emptyText_data();
    void emptyText();
    void fastConstructor();
    void assignmentOperator();
    void isAtSoftHyphen_data();
    void isAtSoftHyphen();
};


BOBUI_BEGIN_NAMESPACE
namespace BOBUIest {

template<>
inline char *toString(const BOBUIextBoundaryFinder::BoundaryReasons &flags)
{
    return qstrdup(QByteArray::number(int(flags)).constData());
}

template<>
inline char *toString(const QList<int> &list)
{
    QByteArray s;
    for (QList<int>::const_iterator it = list.constBegin(); it != list.constEnd(); ++it) {
        if (!s.isEmpty())
            s += ", ";
        s += QByteArray::number(*it);
    }
    s = "{ " + s + " }";
    return qstrdup(s.constData());
}

} // namespace BOBUIest
BOBUI_END_NAMESPACE

void tst_BOBUIextBoundaryFinder::moveSemantics()
{
    const QStringView string = u"Trivial text.";

    // move-construction:
    BOBUIextBoundaryFinder orig(BOBUIextBoundaryFinder::BoundaryType::Grapheme, string);
    BOBUIextBoundaryFinder moved = std::move(orig);

    QCOMPARE(moved.string(), string);

    moved.toStart();
    moved.toNextBoundary();
    QCOMPARE(moved.position(), 1);

    // Hinnant Criterion:
    {
        auto &alias = orig;
        alias = std::move(orig);
        alias.swap(orig);
    }

    // move-assignment:

    orig = std::move(moved);

    QCOMPARE(orig.position(), 1);
    orig.toNextBoundary();
    QCOMPARE(orig.position(), 2);

    // moved-from `moved` can be destroyed
}

#ifdef BOBUI_BUILD_INTERNAL
static void generateDataFromFile(const QString &fname, const QSet<QString> &skipSet = {})
{
    BOBUIest::addColumn<QString>("testString");
    BOBUIest::addColumn<QList<int> >("expectedBreakPositions");

    QString testFile = QFINDTESTDATA(fname);
    QVERIFY2(!testFile.isEmpty(), (fname.toLatin1() + QByteArray(" not found!")));
    QFile f(testFile);
    QVERIFY(f.open(QIODevice::ReadOnly));

    int linenum = 0;
    while (!f.atEnd()) {
        linenum++;

        QByteArray line = f.readLine();
        if (line.startsWith('#'))
            continue;

        QString test = QString::fromUtf8(line);
        QString comments;
        int hash = test.indexOf('#');
        if (hash > 0) {
            comments = test.mid(hash + 1).simplified();
            test = test.left(hash);
        }

        QString testString;
        QList<int> expectedBreakPositions;
        const QStringList parts = test.simplified().split(QLatin1Char(' '), BobUI::SkipEmptyParts);
        for (const QString &part : parts) {
            if (part.size() == 1) {
                if (part.at(0).unicode() == 0xf7)
                    expectedBreakPositions.append(testString.size());
                else
                    QVERIFY(part.at(0).unicode() == 0xd7);
                continue;
            }
            bool ok = true;
            uint ucs4 = part.toInt(&ok, 16);
            QVERIFY(ok && ucs4 > 0);
            if (QChar::requiresSurrogates(ucs4)) {
                testString.append(QChar::highSurrogate(ucs4));
                testString.append(QChar::lowSurrogate(ucs4));
            } else {
                testString.append(QChar(ucs4));
            }
        }
        QVERIFY(!testString.isEmpty());
        QVERIFY(!expectedBreakPositions.isEmpty());

        bool skip = false;

        if (!comments.isEmpty()) {
            const QStringList lst = comments.simplified().split(QLatin1Char(' '), BobUI::SkipEmptyParts);
            comments.clear();
            for (const QString &part : lst) {
                if (part.size() == 1) {
                    if (part.at(0).unicode() == 0xf7)
                        comments += QLatin1Char('+');
                    else if (part.at(0).unicode() == 0xd7)
                        comments += QLatin1Char('x');
                    continue;
                }
                if (part.startsWith(QLatin1Char('(')) && part.endsWith(QLatin1Char(')'))) {
                    skip |= skipSet.contains(part.sliced(1, part.length() - 2));
                    comments += part;
                }
            }
        }

        const QByteArray nm = "line #" + QByteArray::number(linenum) + ": " + comments.toLatin1();

        if (skip)
            qDebug() << "Skipping" << nm;
        else
            BOBUIest::newRow(nm.constData()) << testString << expectedBreakPositions;
    }
}
#endif

static void doTestData(const QString &testString, const QList<int> &expectedBreakPositions,
                       BOBUIextBoundaryFinder::BoundaryType type,
                       BOBUIextBoundaryFinder::BoundaryReasons reasons = BOBUIextBoundaryFinder::BreakOpportunity)
{
    QVERIFY(!testString.isEmpty());

    BOBUIextBoundaryFinder boundaryFinder(type, testString);

    // test toNextBoundary()
    {
        QList<int> actualBreakPositions;
        do {
            QVERIFY(boundaryFinder.isAtBoundary());
            if (boundaryFinder.boundaryReasons() & reasons)
                actualBreakPositions.append(boundaryFinder.position());
        } while (boundaryFinder.toNextBoundary() != -1);
        QString comment;
        QDebug format(&comment);
        format << actualBreakPositions << "vs" << expectedBreakPositions;
        QVERIFY2(actualBreakPositions == expectedBreakPositions, qPrintable(comment));
    }
    QCOMPARE(boundaryFinder.position(), -1);
    QVERIFY(!boundaryFinder.isAtBoundary());
    QVERIFY(boundaryFinder.boundaryReasons() == BOBUIextBoundaryFinder::NotAtBoundary);

    // test toPreviousBoundary()
    {
        QList<int> expectedBreakPositionsRev = expectedBreakPositions;
        std::sort(expectedBreakPositionsRev.begin(), expectedBreakPositionsRev.end(), std::greater<int>());

        QList<int> actualBreakPositions;
        boundaryFinder.toEnd();
        do {
            QVERIFY(boundaryFinder.isAtBoundary());
            if (boundaryFinder.boundaryReasons() & reasons)
                actualBreakPositions.append(boundaryFinder.position());
        } while (boundaryFinder.toPreviousBoundary() != -1);
        QCOMPARE(actualBreakPositions, expectedBreakPositionsRev);
    }
    QCOMPARE(boundaryFinder.position(), -1);
    QVERIFY(!boundaryFinder.isAtBoundary());
    QVERIFY(boundaryFinder.boundaryReasons() == BOBUIextBoundaryFinder::NotAtBoundary);

    auto moved = std::move(boundaryFinder);

    // test boundaryReasons()
    for (int i = 0; i <= testString.size(); ++i) {
        moved.setPosition(i);
        QCOMPARE(!!(moved.boundaryReasons() & reasons), expectedBreakPositions.contains(i));
    }
}

#ifdef BOBUI_BUILD_INTERNAL

BOBUI_BEGIN_NAMESPACE
extern Q_AUTOTEST_EXPORT int bobui_initcharattributes_default_algorithm_only;
BOBUI_END_NAMESPACE

void tst_BOBUIextBoundaryFinder::graphemeBoundariesDefault_data()
{

    // BOBUIBUG-121907: We are not using Unicode grapheme segmentation for Indic scripts.
    QSet<QString> skipSet = {u"ConjunctLinkingScripts_LinkingConsonant"_s};
    generateDataFromFile("data/GraphemeBreakTest.txt", skipSet);
}

void tst_BOBUIextBoundaryFinder::graphemeBoundariesDefault()
{
    QFETCH(QString, testString);
    QFETCH(QList<int>, expectedBreakPositions);

    QScopedValueRollback<int> default_algorithm(bobui_initcharattributes_default_algorithm_only);
    bobui_initcharattributes_default_algorithm_only++;

    doTestData(testString, expectedBreakPositions, BOBUIextBoundaryFinder::Grapheme);
}

void tst_BOBUIextBoundaryFinder::wordBoundariesDefault_data()
{
    generateDataFromFile("data/WordBreakTest.txt");
}

void tst_BOBUIextBoundaryFinder::wordBoundariesDefault()
{
    QFETCH(QString, testString);
    QFETCH(QList<int>, expectedBreakPositions);

    QScopedValueRollback<int> default_algorithm(bobui_initcharattributes_default_algorithm_only);
    bobui_initcharattributes_default_algorithm_only++;

    doTestData(testString, expectedBreakPositions, BOBUIextBoundaryFinder::Word);
}

void tst_BOBUIextBoundaryFinder::sentenceBoundariesDefault_data()
{
    generateDataFromFile("data/SentenceBreakTest.txt");
}

void tst_BOBUIextBoundaryFinder::sentenceBoundariesDefault()
{
    QFETCH(QString, testString);
    QFETCH(QList<int>, expectedBreakPositions);

    QScopedValueRollback<int> default_algorithm(bobui_initcharattributes_default_algorithm_only);
    bobui_initcharattributes_default_algorithm_only++;

    doTestData(testString, expectedBreakPositions, BOBUIextBoundaryFinder::Sentence);
}

void tst_BOBUIextBoundaryFinder::lineBoundariesDefault_data()
{
    generateDataFromFile("data/LineBreakTest.txt", {});
}

void tst_BOBUIextBoundaryFinder::lineBoundariesDefault()
{
    QFETCH(QString, testString);
    QFETCH(QList<int>, expectedBreakPositions);

    QScopedValueRollback<int> default_algorithm(bobui_initcharattributes_default_algorithm_only);
    bobui_initcharattributes_default_algorithm_only++;

    expectedBreakPositions.prepend(0); // ### BOBUIBF generates a boundary at start of text
    doTestData(testString, expectedBreakPositions, BOBUIextBoundaryFinder::Line);
}
#endif // BOBUI_BUILD_INTERNAL

void tst_BOBUIextBoundaryFinder::graphemeBoundaries_manual_data()
{
    BOBUIest::addColumn<QString>("testString");
    BOBUIest::addColumn<QList<int>>("expectedBreakPositions");

    {
        // BOBUIBUG-94951
        QChar s[] = { QChar(0x2764), // U+2764 HEAVY BLACK HEART
                      QChar(0xFE0F), // U+FE0F VARIATION SELECTOR-16
                      QChar(0xD83D), QChar(0xDCF2), // U+1F4F2 MOBILE PHONE WITH RIGHTWARDS ARROW AT LEFT
                      QChar(0xD83D), QChar(0xDCE9), // U+1F4E9 ENVELOPE WITH DOWNWARDS ARROW ABOVE
                    };
        QString testString(s, sizeof(s)/sizeof(s[0]));

        QList<int> expectedBreakPositions{0, 2, 4, 6};
        BOBUIest::newRow("+EXTPICxEXT+EXTPIC+EXTPIC+") << testString << expectedBreakPositions;
    }

    {
        QChar s[] = { QChar(0x2764), // U+2764 HEAVY BLACK HEART
                      QChar(0xFE0F), // U+FE0F VARIATION SELECTOR-16
                      QChar(0x2764), // U+2764 HEAVY BLACK HEART
                      QChar(0xFE0F), // U+FE0F VARIATION SELECTOR-16
                    };
        QString testString(s, sizeof(s)/sizeof(s[0]));

        QList<int> expectedBreakPositions{0, 2, 4};
        BOBUIest::newRow("+EXTPICxEXT+EXTPICxEXT+") << testString << expectedBreakPositions;
    }

    {
        QChar s[] = { QChar(0x2764), // U+2764 HEAVY BLACK HEART
                      QChar(0xFE0F), // U+FE0F VARIATION SELECTOR-16
                      QChar(0xFE0F), // U+FE0F VARIATION SELECTOR-16
                      QChar(0xFE0F), // U+FE0F VARIATION SELECTOR-16
                      QChar(0x2764), // U+2764 HEAVY BLACK HEART
                      QChar(0xFE0F), // U+FE0F VARIATION SELECTOR-16
                      QChar(0xFE0F), // U+FE0F VARIATION SELECTOR-16
                    };
        QString testString(s, sizeof(s)/sizeof(s[0]));

        QList<int> expectedBreakPositions{0, 4, 7};
        BOBUIest::newRow("+EXTPICxEXTxEXTxEXT+EXTPICxEXTxEXT+") << testString << expectedBreakPositions;
    }

    {
        QChar s[] = { QChar(0x2764), // U+2764 HEAVY BLACK HEART
                      QChar(0xFE0F), // U+FE0F VARIATION SELECTOR-16
                      QChar(0xFE0F), // U+FE0F VARIATION SELECTOR-16
                      QChar(0x200D), // U+200D ZERO WIDTH JOINER
                      QChar(0xD83D), QChar(0xDCF2), // U+1F4F2 MOBILE PHONE WITH RIGHTWARDS ARROW AT LEFT
                      QChar(0xFE0F), // U+FE0F VARIATION SELECTOR-16
                    };
        QString testString(s, sizeof(s)/sizeof(s[0]));

        QList<int> expectedBreakPositions{0, 7};
        BOBUIest::newRow("+EXTPICxEXTxEXTxZWJxEXTPICxEXTxEXT+") << testString << expectedBreakPositions;
    }

    {
        QChar s[] = { QChar(0x2764), // U+2764 HEAVY BLACK HEART
                      QChar(0xFE0F), // U+FE0F VARIATION SELECTOR-16
                      QChar(0xFE0F), // U+FE0F VARIATION SELECTOR-16
                      QChar(0x200D), // U+200D ZERO WIDTH JOINER
                      QChar(0x0041), // U+0041 LATIN CAPITAL LETTER A
                      QChar(0xD83D), QChar(0xDCF2), // U+1F4F2 MOBILE PHONE WITH RIGHTWARDS ARROW AT LEFT
                    };
        QString testString(s, sizeof(s)/sizeof(s[0]));

        QList<int> expectedBreakPositions{0, 4, 5, 7};
        BOBUIest::newRow("+EXTPICxEXTxEXTxZWJ+Any+EXTPIC+") << testString << expectedBreakPositions;
    }

    {
        QChar s[] = { QChar(0x2764), // U+2764 HEAVY BLACK HEART
                      QChar(0xFE0F), // U+FE0F VARIATION SELECTOR-16
                      QChar(0xD83C), QChar(0xDDEA), // U+1F1EA REGIONAL INDICATOR SYMBOL LETTER E
                      QChar(0xD83C), QChar(0xDDFA), // U+1F1FA REGIONAL INDICATOR SYMBOL LETTER U
                      QChar(0xD83C), QChar(0xDDEA), // U+1F1EA REGIONAL INDICATOR SYMBOL LETTER E
                      QChar(0xD83C), QChar(0xDDFA), // U+1F1FA REGIONAL INDICATOR SYMBOL LETTER U
                      QChar(0xD83C), QChar(0xDDEA), // U+1F1EA REGIONAL INDICATOR SYMBOL LETTER E
                      QChar(0x0041), // U+0041 LATIN CAPITAL LETTER A
                    };
        QString testString(s, sizeof(s)/sizeof(s[0]));

        QList<int> expectedBreakPositions{0, 2, 6, 10, 12, 13};
        BOBUIest::newRow("+EXTPICxEXT+RIxRI+RIxRI+RI+ANY+") << testString << expectedBreakPositions;
    }
}

void tst_BOBUIextBoundaryFinder::graphemeBoundaries_manual()
{
    QFETCH(QString, testString);
    QFETCH(QList<int>, expectedBreakPositions);

    doTestData(testString, expectedBreakPositions, BOBUIextBoundaryFinder::Grapheme);
}

void tst_BOBUIextBoundaryFinder::wordBoundaries_manual_data()
{
    BOBUIest::addColumn<QString>("testString");
    BOBUIest::addColumn<QList<int> >("expectedBreakPositions");
    BOBUIest::addColumn<QList<int> >("expectedStartPositions");
    BOBUIest::addColumn<QList<int> >("expectedEndPositions");

    {
        QChar s[] = { QChar(0x000D), QChar(0x000A), QChar(0x000A) };
        QString testString(s, sizeof(s)/sizeof(s[0]));
        QList<int> expectedBreakPositions, expectedStartPositions, expectedEndPositions;
        expectedBreakPositions << 0 << 2 << 3;

        BOBUIest::newRow("+CRxLF+LF+") << testString << expectedBreakPositions
                                    << expectedStartPositions << expectedEndPositions;
    }
    {
        QChar s[] = { QChar(0x000D), QChar(0x0308), QChar(0x000A), QChar(0x000A) };
        QString testString(s, sizeof(s)/sizeof(s[0]));
        QList<int> expectedBreakPositions, expectedStartPositions, expectedEndPositions;
        expectedBreakPositions << 0 << 1 << 2 << 3 << 4;

        BOBUIest::newRow("+CR+FE+LF+LF+") << testString << expectedBreakPositions
                                       << expectedStartPositions << expectedEndPositions;
    }
    {
        QString testString(QString::fromUtf8("Aaa bbb ccc.\r\nDdd eee fff."));
        QList<int> expectedBreakPositions, expectedStartPositions, expectedEndPositions;
        expectedBreakPositions << 0 << 3 << 4 << 7 << 8 << 11 << 12 << 14 << 17 << 18 << 21 << 22 << 25 << 26;
        expectedStartPositions << 0 << 4 << 8  << 14 << 18 << 22;
        expectedEndPositions   << 3 << 7 << 11 << 17 << 21 << 25;

        BOBUIest::newRow("words1") << testString << expectedBreakPositions
                                << expectedStartPositions << expectedEndPositions;
    }
    {
        QString testString(QString::fromUtf8("Hello (sad) world !"));
        QList<int> expectedBreakPositions, expectedStartPositions, expectedEndPositions;
        expectedBreakPositions << 0 << 5 << 6 << 7 << 10 << 11 << 12 << 17 << 18 << 19;
        expectedStartPositions << 0 << 7  << 12;
        expectedEndPositions   << 5 << 10 << 17;

        BOBUIest::newRow("words2") << testString << expectedBreakPositions
                                << expectedStartPositions << expectedEndPositions;
    }
    {
        QString testString(QString::fromUtf8("mr.Hamster"));
        QList<int> expectedBreakPositions, expectedStartPositions, expectedEndPositions;
        expectedBreakPositions << 0 << 2 << 3 << 10;
        expectedStartPositions << 0 << 3;
        expectedEndPositions   << 2 << 10;

        BOBUIest::newRow("words3") << testString << expectedBreakPositions
                                << expectedStartPositions << expectedEndPositions;
    }
    {
        QString testString(QString::fromUtf8("This is     a sample buffer.Please test me .     He's don't Le'Clerk."));
        QList<int> expectedBreakPositions, expectedStartPositions, expectedEndPositions;
        expectedBreakPositions << 0 << 4 << 5 << 7 << 12 << 13 << 14 << 20 << 21 << 27 << 28 << 34
                               << 35 << 39 << 40 << 42 << 43 << 44 << 49 << 53 << 54 << 59 << 60
                               << 68 << 69;
        expectedStartPositions << 0 << 5 << 12 << 14 << 21 << 28 << 35 << 40 << 49 << 54 << 60;
        expectedEndPositions   << 4 << 7 << 13 << 20 << 27 << 34 << 39 << 42 << 53 << 59 << 68;

        BOBUIest::newRow("words4") << testString << expectedBreakPositions
                                << expectedStartPositions << expectedEndPositions;
    }
    {
        // text with trailing space
        QString testString(QString::fromUtf8("Please test me. Finish "));
        QList<int> expectedBreakPositions, expectedStartPositions, expectedEndPositions;
        expectedBreakPositions << 0 << 6 << 7 << 11 << 12 << 14 << 15 << 16 << 22 << 23;
        expectedStartPositions << 0 << 7  << 12 << 16;
        expectedEndPositions   << 6 << 11 << 14 << 22;

        BOBUIest::newRow("bobuibug6498") << testString << expectedBreakPositions
                                   << expectedStartPositions << expectedEndPositions;
    }

    // Sample Strings from WordBreakTest.html
    {
        QChar s[] = { QChar(0x0063), QChar(0x0061), QChar(0x006E), QChar(0x0027), QChar(0x0074) };
        QString testString(s, sizeof(s)/sizeof(s[0]));
        QList<int> expectedBreakPositions, expectedStartPositions, expectedEndPositions;
        expectedBreakPositions << 0 << 5;
        expectedStartPositions << 0;
        expectedEndPositions   << 5;

        BOBUIest::newRow("ts 1") << testString << expectedBreakPositions
                              << expectedStartPositions << expectedEndPositions;
    }
    {
        QChar s[] = { QChar(0x0063), QChar(0x0061), QChar(0x006E), QChar(0x2019), QChar(0x0074) };
        QString testString(s, sizeof(s)/sizeof(s[0]));
        QList<int> expectedBreakPositions, expectedStartPositions, expectedEndPositions;
        expectedBreakPositions << 0 << 5;
        expectedStartPositions << 0;
        expectedEndPositions   << 5;

        BOBUIest::newRow("ts 2") << testString << expectedBreakPositions
                              << expectedStartPositions << expectedEndPositions;
    }
    {
        QChar s[] = { QChar(0x0061), QChar(0x0062), QChar(0x00AD), QChar(0x0062), QChar(0x0061) };
        QString testString(s, sizeof(s)/sizeof(s[0]));
        QList<int> expectedBreakPositions, expectedStartPositions, expectedEndPositions;
        expectedBreakPositions << 0 << 5;
        expectedStartPositions << 0;
        expectedEndPositions   << 5;

        BOBUIest::newRow("ts 3") << testString << expectedBreakPositions
                              << expectedStartPositions << expectedEndPositions;
    }
    {
        QChar s[] = { QChar(0x0061), QChar(0x0024), QChar(0x002D), QChar(0x0033),
                      QChar(0x0034), QChar(0x002C), QChar(0x0035), QChar(0x0036),
                      QChar(0x0037), QChar(0x002E), QChar(0x0031), QChar(0x0034),
                      QChar(0x0025), QChar(0x0062) };
        QString testString(s, sizeof(s)/sizeof(s[0]));
        QList<int> expectedBreakPositions, expectedStartPositions, expectedEndPositions;
        expectedBreakPositions << 0 << 1 << 2 << 3 << 12 << 13 << 14;
        expectedStartPositions << 0 << 3  << 13;
        expectedEndPositions   << 1 << 12 << 14;

        BOBUIest::newRow("ts 4") << testString << expectedBreakPositions
                              << expectedStartPositions << expectedEndPositions;
    }
    {
        QChar s[] = { QChar(0x0033), QChar(0x0061) };
        QString testString(s, sizeof(s)/sizeof(s[0]));
        QList<int> expectedBreakPositions, expectedStartPositions, expectedEndPositions;
        expectedBreakPositions << 0 << 2;
        expectedStartPositions << 0;
        expectedEndPositions   << 2;

        BOBUIest::newRow("ts 5") << testString << expectedBreakPositions
                              << expectedStartPositions << expectedEndPositions;
    }
    {
        QChar s[] = { QChar(0x2060), QChar(0x0063), QChar(0x2060), QChar(0x0061),
                      QChar(0x2060), QChar(0x006E), QChar(0x2060), QChar(0x0027),
                      QChar(0x2060), QChar(0x0074), QChar(0x2060), QChar(0x2060) };
        QString testString(s, sizeof(s)/sizeof(s[0]));
        QList<int> expectedBreakPositions, expectedStartPositions, expectedEndPositions;
        expectedBreakPositions << 0 << 1 << 12;
        expectedStartPositions << 1;
        expectedEndPositions   << 12;

        BOBUIest::newRow("ts 1e") << testString << expectedBreakPositions
                               << expectedStartPositions << expectedEndPositions;
    }
    {
        QChar s[] = { QChar(0x2060), QChar(0x0063), QChar(0x2060), QChar(0x0061),
                      QChar(0x2060), QChar(0x006E), QChar(0x2060), QChar(0x2019),
                      QChar(0x2060), QChar(0x0074), QChar(0x2060), QChar(0x2060) };
        QString testString(s, sizeof(s)/sizeof(s[0]));
        QList<int> expectedBreakPositions, expectedStartPositions, expectedEndPositions;
        expectedBreakPositions << 0 << 1 << 12;
        expectedStartPositions << 1;
        expectedEndPositions   << 12;

        BOBUIest::newRow("ts 2e") << testString << expectedBreakPositions
                               << expectedStartPositions << expectedEndPositions;
    }
    {
        QChar s[] = { QChar(0x2060), QChar(0x0061), QChar(0x2060), QChar(0x0062),
                      QChar(0x2060), QChar(0x00AD), QChar(0x2060), QChar(0x0062),
                      QChar(0x2060), QChar(0x0061), QChar(0x2060), QChar(0x2060) };
        QString testString(s, sizeof(s)/sizeof(s[0]));
        QList<int> expectedBreakPositions, expectedStartPositions, expectedEndPositions;
        expectedBreakPositions << 0 << 1 << 12;
        expectedStartPositions << 1;
        expectedEndPositions   << 12;

        BOBUIest::newRow("ts 3e") << testString << expectedBreakPositions
                               << expectedStartPositions << expectedEndPositions;
    }
    {
        QChar s[] = { QChar(0x2060), QChar(0x0061), QChar(0x2060), QChar(0x0024),
                      QChar(0x2060), QChar(0x002D), QChar(0x2060), QChar(0x0033),
                      QChar(0x2060), QChar(0x0034), QChar(0x2060), QChar(0x002C),
                      QChar(0x2060), QChar(0x0035), QChar(0x2060), QChar(0x0036),
                      QChar(0x2060), QChar(0x0037), QChar(0x2060), QChar(0x002E),
                      QChar(0x2060), QChar(0x0031), QChar(0x2060), QChar(0x0034),
                      QChar(0x2060), QChar(0x0025), QChar(0x2060), QChar(0x0062),
                      QChar(0x2060), QChar(0x2060) };
        QString testString(s, sizeof(s)/sizeof(s[0]));
        QList<int> expectedBreakPositions, expectedStartPositions, expectedEndPositions;
        expectedBreakPositions << 0 << 1 << 3 << 5 << 7 << 25 << 27 << 30;
        expectedStartPositions << 1 << 7  << 27;
        expectedEndPositions   << 3 << 25 << 30;

        BOBUIest::newRow("ts 4e") << testString << expectedBreakPositions
                               << expectedStartPositions << expectedEndPositions;
    }
    {
        QChar s[] = { QChar(0x2060), QChar(0x0033), QChar(0x2060), QChar(0x0061),
                      QChar(0x2060), QChar(0x2060) };
        QString testString(s, sizeof(s)/sizeof(s[0]));
        QList<int> expectedBreakPositions, expectedStartPositions, expectedEndPositions;
        expectedBreakPositions << 0 << 1 << 6;
        expectedStartPositions << 1;
        expectedEndPositions   << 6;

        BOBUIest::newRow("ts 5e") << testString << expectedBreakPositions
                               << expectedStartPositions << expectedEndPositions;
    }
}

void tst_BOBUIextBoundaryFinder::wordBoundaries_manual()
{
    QFETCH(QString, testString);
    QFETCH(QList<int>, expectedBreakPositions);
    QFETCH(QList<int>, expectedStartPositions);
    QFETCH(QList<int>, expectedEndPositions);

    doTestData(testString, expectedBreakPositions, BOBUIextBoundaryFinder::Word);
    doTestData(testString, expectedStartPositions, BOBUIextBoundaryFinder::Word, BOBUIextBoundaryFinder::StartOfItem);
    doTestData(testString, expectedEndPositions, BOBUIextBoundaryFinder::Word, BOBUIextBoundaryFinder::EndOfItem);
}

void tst_BOBUIextBoundaryFinder::sentenceBoundaries_manual_data()
{
    BOBUIest::addColumn<QString>("testString");
    BOBUIest::addColumn<QList<int> >("expectedBreakPositions");

    {
        QChar s[] = { QChar(0x000D), QChar(0x000A), QChar(0x000A) };
        QString testString(s, sizeof(s)/sizeof(s[0]));
        QList<int> expectedBreakPositions;
        expectedBreakPositions << 0 << 2 << 3;

        BOBUIest::newRow("+CRxLF+LF+") << testString << expectedBreakPositions;
    }
    {
        QChar s[] = { QChar(0x000D), QChar(0x0308), QChar(0x000A), QChar(0x000A) };
        QString testString(s, sizeof(s)/sizeof(s[0]));
        QList<int> expectedBreakPositions;
        expectedBreakPositions << 0 << 1 << 3 << 4;

        BOBUIest::newRow("+CR+FExLF+LF+") << testString << expectedBreakPositions;
    }
    {
        QString testString(QString::fromUtf8("Aaa bbb ccc.\r\nDdd eee fff."));
        QList<int> expectedBreakPositions;
        expectedBreakPositions << 0 << 14 << 26;

        BOBUIest::newRow("data1") << testString << expectedBreakPositions;
    }
    {
        QString testString(QString::fromUtf8("Diga-nos qualé a sua opinião"));
        QList<int> expectedBreakPositions;
        expectedBreakPositions << 0 << 28;

        BOBUIest::newRow("data2") << testString << expectedBreakPositions;
    }
    {
        QString testString(QString::fromUtf8("mr.Hamster"));
        QList<int> expectedBreakPositions;
        expectedBreakPositions << 0 << 3 << 10;

        BOBUIest::newRow("data3") << testString << expectedBreakPositions;
    }
    {
        QString testString(QString::fromUtf8("Doing TEST, doing another test."));
        QList<int> expectedBreakPositions;
        expectedBreakPositions << 0 << 31;

        BOBUIest::newRow("data4") << testString << expectedBreakPositions;
    }
}

void tst_BOBUIextBoundaryFinder::sentenceBoundaries_manual()
{
    QFETCH(QString, testString);
    QFETCH(QList<int>, expectedBreakPositions);

    QVERIFY(expectedBreakPositions.size() >= 2);
    QList<int> expectedStartPositions = expectedBreakPositions; expectedStartPositions.removeLast();
    QList<int> expectedEndPositions = expectedBreakPositions; expectedEndPositions.removeFirst();

    doTestData(testString, expectedBreakPositions, BOBUIextBoundaryFinder::Sentence);
    doTestData(testString, expectedStartPositions, BOBUIextBoundaryFinder::Sentence, BOBUIextBoundaryFinder::StartOfItem);
    doTestData(testString, expectedEndPositions, BOBUIextBoundaryFinder::Sentence, BOBUIextBoundaryFinder::EndOfItem);
}

void tst_BOBUIextBoundaryFinder::lineBoundaries_manual_data()
{
    BOBUIest::addColumn<QString>("testString");
    BOBUIest::addColumn<QList<int> >("expectedBreakPositions");
    BOBUIest::addColumn<QList<int> >("expectedMandatoryBreakPositions");

    {
        QString testString(QString::fromUtf8("Aaa bbb ccc.\r\nDdd eee fff."));
        QList<int> expectedBreakPositions, expectedMandatoryBreakPositions;
        expectedBreakPositions << 0 << 4 << 8 << 14 << 18 << 22 << 26;
        expectedMandatoryBreakPositions << 0 << 14 << 26;

        BOBUIest::newRow("data1") << testString << expectedBreakPositions
                               << expectedMandatoryBreakPositions;
    }
    {
        QString testString(QString::fromUtf8("Diga-nos qualé a sua opinião"));
        QList<int> expectedBreakPositions, expectedMandatoryBreakPositions;
        expectedBreakPositions << 0 << 5 << 9 << 15 << 17 << 21 << 28;
        expectedMandatoryBreakPositions << 0 << 28;

        BOBUIest::newRow("data2") << testString << expectedBreakPositions
                               << expectedMandatoryBreakPositions;
    }

    {
        QChar s[] = { QChar(0x000D), QChar(0x0308), QChar(0x000A), QChar(0x000A), QChar(0x0020) };
        QString testString(s, sizeof(s)/sizeof(s[0]));
        QList<int> expectedBreakPositions, expectedMandatoryBreakPositions;
        expectedBreakPositions << 0 << 1 << 3 << 4 << 5;
        expectedMandatoryBreakPositions << 0 << 1 << 3 << 4 << 5;

        BOBUIest::newRow("x(CR)+(FE)x(LF)+(LF)+(SP)+") << testString << expectedBreakPositions
                                                    << expectedMandatoryBreakPositions;
    }
    {
        QChar s[] = { QChar(0x000A), QChar(0x2E80), QChar(0x0308), QChar(0x0023), QChar(0x0023) };
        QString testString(s, sizeof(s)/sizeof(QChar));
        QList<int> expectedBreakPositions, expectedMandatoryBreakPositions;
        expectedBreakPositions << 0 << 1 << 3 << 5;
        expectedMandatoryBreakPositions << 0 << 1 << 5;

        BOBUIest::newRow("x(LF)+(ID)x(CM)+(AL)x(AL)+") << testString << expectedBreakPositions
                                                    << expectedMandatoryBreakPositions;
    }
    {
        QChar s[] = { QChar(0x000A), QChar(0x0308), QChar(0x0023), QChar(0x0023) };
        QString testString(s, sizeof(s)/sizeof(QChar));
        QList<int> expectedBreakPositions, expectedMandatoryBreakPositions;
        expectedBreakPositions << 0 << 1 << 4;
        expectedMandatoryBreakPositions << 0 << 1 << 4;

        BOBUIest::newRow("x(LF)+(CM)x(AL)x(AL)+") << testString << expectedBreakPositions
                                               << expectedMandatoryBreakPositions;
    }

    {
        QChar s[] = { QChar(0x0061), QChar(0x00AD), QChar(0x0062), QChar(0x0009), QChar(0x0063), QChar(0x0064) };
        QString testString(s, sizeof(s)/sizeof(s[0]));
        QList<int> expectedBreakPositions, expectedMandatoryBreakPositions;
        expectedBreakPositions << 0 << 2 << 4 << 6;
        expectedMandatoryBreakPositions << 0 << 6;

        BOBUIest::newRow("x(AL)x(BA)+(AL)x(BA)+(AL)x(AL)+") << testString << expectedBreakPositions
                                                         << expectedMandatoryBreakPositions;
    }
}

void tst_BOBUIextBoundaryFinder::lineBoundaries_manual()
{
    QFETCH(QString, testString);
    QFETCH(QList<int>, expectedBreakPositions);
    QFETCH(QList<int>, expectedMandatoryBreakPositions);

    QVERIFY(expectedMandatoryBreakPositions.size() >= 2);
    QList<int> expectedStartPositions = expectedMandatoryBreakPositions; expectedStartPositions.removeLast();
    QList<int> expectedEndPositions = expectedMandatoryBreakPositions; expectedEndPositions.removeFirst();

    doTestData(testString, expectedBreakPositions, BOBUIextBoundaryFinder::Line);
    doTestData(testString, expectedMandatoryBreakPositions, BOBUIextBoundaryFinder::Line, BOBUIextBoundaryFinder::MandatoryBreak);
    doTestData(testString, expectedStartPositions, BOBUIextBoundaryFinder::Line, BOBUIextBoundaryFinder::StartOfItem);
    doTestData(testString, expectedEndPositions, BOBUIextBoundaryFinder::Line, BOBUIextBoundaryFinder::EndOfItem);
}

Q_DECLARE_METATYPE(BOBUIextBoundaryFinder)

void tst_BOBUIextBoundaryFinder::emptyText_data()
{
    BOBUIest::addColumn<BOBUIextBoundaryFinder>("boundaryFinder");

    QString empty;
    QString notEmpty(QLatin1String("not empty"));
    uchar attrs[11];

    BOBUIextBoundaryFinder invalidFinder(BOBUIextBoundaryFinder::Word, empty);
    BOBUIest::newRow("empty1") << invalidFinder;
    BOBUIextBoundaryFinder finder(invalidFinder);
    BOBUIest::newRow("empty2") << finder;
    finder = BOBUIextBoundaryFinder(BOBUIextBoundaryFinder::Grapheme, notEmpty);
    finder = invalidFinder;
    BOBUIest::newRow("empty3") << finder;
    BOBUIest::newRow("empty4") << BOBUIextBoundaryFinder(BOBUIextBoundaryFinder::Word, notEmpty.constData(), 0, 0, 0);
    BOBUIest::newRow("empty5") << BOBUIextBoundaryFinder(BOBUIextBoundaryFinder::Word, notEmpty.constData(), 0, attrs, 11);
}

void tst_BOBUIextBoundaryFinder::emptyText()
{
    QFETCH(BOBUIextBoundaryFinder, boundaryFinder);

    QCOMPARE(boundaryFinder.position(), 0);
    QCOMPARE(boundaryFinder.boundaryReasons(), BOBUIextBoundaryFinder::NotAtBoundary);

    boundaryFinder.toNextBoundary();
    QCOMPARE(boundaryFinder.position(), -1);
    QCOMPARE(boundaryFinder.boundaryReasons(), BOBUIextBoundaryFinder::NotAtBoundary);
}

void tst_BOBUIextBoundaryFinder::fastConstructor()
{
    QString text("Hello World");
    BOBUIextBoundaryFinder finder(BOBUIextBoundaryFinder::Word, text.constData(), text.size(), /*buffer*/0, /*buffer size*/0);

    QCOMPARE(finder.position(), 0);
    QVERIFY(finder.boundaryReasons() & BOBUIextBoundaryFinder::StartOfItem);

    finder.toNextBoundary();
    QCOMPARE(finder.position(), 5);
    QVERIFY(finder.boundaryReasons() & BOBUIextBoundaryFinder::EndOfItem);

    finder.toNextBoundary();
    QCOMPARE(finder.position(), 6);
    QVERIFY(finder.boundaryReasons() & BOBUIextBoundaryFinder::StartOfItem);

    finder.toNextBoundary();
    QCOMPARE(finder.position(), text.size());
    QVERIFY(finder.boundaryReasons() & BOBUIextBoundaryFinder::EndOfItem);

    finder.toNextBoundary();
    QCOMPARE(finder.position(), -1);
    QCOMPARE(finder.boundaryReasons(), BOBUIextBoundaryFinder::NotAtBoundary);
}

void tst_BOBUIextBoundaryFinder::assignmentOperator()
{
    QString text(QLatin1String("Hello World"));

    BOBUIextBoundaryFinder invalidFinder;
    QVERIFY(!invalidFinder.isValid());
    QCOMPARE(invalidFinder.string(), QString());

    BOBUIextBoundaryFinder validFinder(BOBUIextBoundaryFinder::Word, text);
    QVERIFY(validFinder.isValid());
    QCOMPARE(validFinder.string(), text);

    BOBUIextBoundaryFinder finder(BOBUIextBoundaryFinder::Line, QLatin1String("dummy"));
    QVERIFY(finder.isValid());

    finder = invalidFinder;
    QVERIFY(!finder.isValid());
    QCOMPARE(finder.string(), QString());

    finder = validFinder;
    QVERIFY(finder.isValid());
    QCOMPARE(finder.string(), text);
}

void tst_BOBUIextBoundaryFinder::isAtSoftHyphen_data()
{
    BOBUIest::addColumn<QString>("testString");
    BOBUIest::addColumn<QList<int> >("expectedBreakPositions");
    BOBUIest::addColumn<QList<int> >("expectedSoftHyphenPositions");

    {
        QString testString = QString::fromUtf8("I a-m break-able");
        testString.replace(QLatin1Char('-'), QChar(QChar::SoftHyphen));
        QList<int> expectedBreakPositions, expectedSoftHyphenPositions;
        expectedBreakPositions << 0 << 2 << 4 << 6 << 12 << 16;
        expectedSoftHyphenPositions << 4 << 12;

        BOBUIest::newRow("Soft Hyphen") << testString << expectedBreakPositions
                                     << expectedSoftHyphenPositions;
    }
}

void tst_BOBUIextBoundaryFinder::isAtSoftHyphen()
{
    QFETCH(QString, testString);
    QFETCH(QList<int>, expectedBreakPositions);
    QFETCH(QList<int>, expectedSoftHyphenPositions);

    doTestData(testString, expectedBreakPositions, BOBUIextBoundaryFinder::Line);
    doTestData(testString, expectedSoftHyphenPositions, BOBUIextBoundaryFinder::Line, BOBUIextBoundaryFinder::SoftHyphen);
}

BOBUIEST_MAIN(tst_BOBUIextBoundaryFinder)
#include "tst_bobuiextboundaryfinder.moc"
