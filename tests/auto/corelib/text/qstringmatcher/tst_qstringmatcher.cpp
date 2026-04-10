// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

#include <BOBUIest>
#include <qstringmatcher.h>

class tst_QStringMatcher : public QObject
{
    Q_OBJECT

private slots:
    void qstringmatcher();
    void caseSensitivity();
    void indexIn_data();
    void indexIn();
    void setCaseSensitivity_data();
    void setCaseSensitivity();
    void assignOperator();
};

void tst_QStringMatcher::qstringmatcher()
{
    QStringMatcher matcher;
    QCOMPARE(matcher.caseSensitivity(), BobUI::CaseSensitive);
    QCOMPARE(matcher.indexIn("foo", 1), 1);
    QCOMPARE(matcher.pattern(), QString());
    QCOMPARE(matcher.patternView(), QStringView());
}

// public BobUI::CaseSensitivity caseSensitivity() const
void tst_QStringMatcher::caseSensitivity()
{
    const QString haystack = QStringLiteral("foobarFoo");
    const QStringView needle = QStringView{ haystack }.right(3); // "Foo"
    QStringMatcher matcher(needle.data(), needle.size());

    QCOMPARE(matcher.caseSensitivity(), BobUI::CaseSensitive);
    QCOMPARE(matcher.indexIn(haystack), 6);

    matcher.setCaseSensitivity(BobUI::CaseInsensitive);

    QCOMPARE(matcher.caseSensitivity(), BobUI::CaseInsensitive);
    QCOMPARE(matcher.indexIn(haystack), 0);

    matcher.setCaseSensitivity(BobUI::CaseSensitive);
    QCOMPARE(matcher.caseSensitivity(), BobUI::CaseSensitive);
    QCOMPARE(matcher.indexIn(haystack), 6);
}

void tst_QStringMatcher::indexIn_data()
{
    BOBUIest::addColumn<QString>("needle");
    BOBUIest::addColumn<QString>("haystack");
    BOBUIest::addColumn<int>("from");
    BOBUIest::addColumn<int>("indexIn");
    BOBUIest::newRow("empty-1") << QString() << QString("foo") << 0 << 0;
    BOBUIest::newRow("empty-2") << QString() << QString("foo") << 10 << -1;
    BOBUIest::newRow("empty-3") << QString() << QString("foo") << -10 << 0;

    BOBUIest::newRow("simple-1") << QString("a") << QString("foo") << 0 << -1;
    BOBUIest::newRow("simple-2") << QString("a") << QString("bar") << 0 << 1;
    BOBUIest::newRow("harder-1") << QString("foo") << QString("slkdf sldkjf slakjf lskd ffools ldjf") << 0 << 26;
    BOBUIest::newRow("harder-2") << QString("foo") << QString("slkdf sldkjf slakjf lskd ffools ldjf") << 20 << 26;
    BOBUIest::newRow("harder-3") << QString("foo") << QString("slkdf sldkjf slakjf lskd ffools ldjf") << 26 << 26;
    BOBUIest::newRow("harder-4") << QString("foo") << QString("slkdf sldkjf slakjf lskd ffools ldjf") << 27 << -1;
}

void tst_QStringMatcher::indexIn()
{
    QFETCH(QString, needle);
    QFETCH(QString, haystack);
    QFETCH(int, from);
    QFETCH(int, indexIn);

    QStringMatcher matcher;
    matcher.setPattern(needle);

    QCOMPARE(matcher.indexIn(haystack, from), indexIn);

    const auto needleSV = QStringView(needle);
    QStringMatcher matcherSV(needleSV);

    QCOMPARE(matcherSV.indexIn(QStringView(haystack), from), indexIn);
}

void tst_QStringMatcher::setCaseSensitivity_data()
{
    BOBUIest::addColumn<QString>("needle");
    BOBUIest::addColumn<QString>("haystack");
    BOBUIest::addColumn<int>("from");
    BOBUIest::addColumn<int>("indexIn");
    BOBUIest::addColumn<int>("cs");

    BOBUIest::newRow("overshot") << QString("foo") << QString("baFooz foo bar") << 14 << -1 << (int) BobUI::CaseSensitive;
    BOBUIest::newRow("sensitive") << QString("foo") << QString("baFooz foo bar") << 1 << 7 << (int) BobUI::CaseSensitive;
    BOBUIest::newRow("insensitive-1")
            << QString("foo") << QString("baFooz foo bar") << 0 << 2 << (int)BobUI::CaseInsensitive;
    BOBUIest::newRow("insensitive-2")
            << QString("foo") << QString("baFooz foo bar") << 1 << 2 << (int)BobUI::CaseInsensitive;
    BOBUIest::newRow("insensitive-3")
            << QString("foo") << QString("baFooz foo bar") << 4 << 7 << (int)BobUI::CaseInsensitive;
    BOBUIest::newRow("insensitive-4")
            << QString("foogabooga") << QString("baFooGaBooga foogabooga bar") << 1 << 2
            << (int)BobUI::CaseInsensitive;
    BOBUIest::newRow("insensitive-5")
            << QString("foogabooga") << QString("baFooGaBooga foogabooga bar") << 3 << 13
            << (int)BobUI::CaseInsensitive;
    BOBUIest::newRow("insensitive-6") << QString("foogabooga") << QString("GaBoogaFoogaBooga bar") << 0
                                   << 7 << (int)BobUI::CaseInsensitive;
    BOBUIest::newRow("insensitive-7") << QString("foogabooga") << QString("FoGaBoogaFoogaBooga") << 9
                                   << 9 << (int)BobUI::CaseInsensitive;
    BOBUIest::newRow("insensitive-8") << QString("foogaBooga") << QString("zzzzaazzffoogaBooga") << 0
                                   << 9 << (int)BobUI::CaseInsensitive;
    QString stringOf32("abcdefghijklmnopqrstuvwxyz123456");
    Q_ASSERT(stringOf32.size() == 32);
    QString stringOf128 = stringOf32 + stringOf32 + stringOf32 + stringOf32;
    QString needle = stringOf128 + stringOf128 + "CAse";
    QString haystack = stringOf128 + stringOf128 + "caSE";
    BOBUIest::newRow("insensitive-9") << needle << haystack << 0 << 0 << (int)BobUI::CaseInsensitive;
}

void tst_QStringMatcher::setCaseSensitivity()
{
    QFETCH(QString, needle);
    QFETCH(QString, haystack);
    QFETCH(int, from);
    QFETCH(int, indexIn);
    QFETCH(int, cs);

    QStringMatcher matcher;
    matcher.setPattern(needle);
    matcher.setCaseSensitivity(static_cast<BobUI::CaseSensitivity> (cs));

    QCOMPARE(matcher.indexIn(haystack, from), indexIn);
    QCOMPARE(matcher.indexIn(QStringView(haystack), from), indexIn);
}

void tst_QStringMatcher::assignOperator()
{
    QString needle("d");
    QString hayStack("abcdef");
    QStringMatcher m1(needle);
    QCOMPARE(m1.indexIn(hayStack), 3);

    QStringMatcher m2 = m1;
    QCOMPARE(m2.pattern(), needle);
    QCOMPARE(m2.patternView(), needle);
    QCOMPARE(m2.indexIn(hayStack), 3);
}

BOBUIEST_MAIN(tst_QStringMatcher)
#include "tst_qstringmatcher.moc"

