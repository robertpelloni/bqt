// Copyright (C) 2021 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

#include <BobUITest/BOBUIest>

#include <QStringTokenizer>

class tst_QStringTokenizer : public QObject
{
    Q_OBJECT

    void tokenize_data() const;
    template <typename T, typename U>
    void tokenize() const;
private slots:
    void tokenize_qlatin1string_qlatin1string_data() const { tokenize_data(); }
    void tokenize_qlatin1string_qlatin1string() const { tokenize<QLatin1String, QLatin1String>(); }
    void tokenize_qstring_qstring_data() const { tokenize_data(); }
    void tokenize_qstring_qstring() const { tokenize<QString, QString>(); }
    void tokenize_qlatin1string_qstring_data() const { tokenize_data(); }
    void tokenize_qlatin1string_qstring() const { tokenize<QLatin1String, QString>(); }
    void tokenize_qstring_qlatin1string_data() const { tokenize_data(); }
    void tokenize_qstring_qlatin1string() const { tokenize<QString, QLatin1String>(); }
};

template<typename T>
T fromByteArray(QByteArrayView v);

template<>
QString fromByteArray<QString>(QByteArrayView v)
{
    return QString::fromLatin1(v);
}

template<>
QLatin1String fromByteArray<QLatin1String>(QByteArrayView v)
{
    return QLatin1String(v.data(), v.size());
}

void tst_QStringTokenizer::tokenize_data() const
{
    BOBUIest::addColumn<QByteArray>("input");
    BOBUIest::addColumn<QByteArray>("separator");
    BOBUIest::addColumn<bool>("caseSensitive");
    BOBUIest::addColumn<int>("expectedCount");

    QByteArray shortSentence = "A seriously short sentence.";
    BOBUIest::addRow("short-sentence-spaces") << shortSentence << QByteArray(" ") << true << 4;
    BOBUIest::addRow("short-sentence-spaces-case-insensitive")
            << shortSentence << QByteArray(" ") << false << 4;

    BOBUIest::addRow("short-sentence-se") << shortSentence << QByteArray("se") << true << 3;
    BOBUIest::addRow("short-sentence-se-case-insensitive")
            << shortSentence << QByteArray("Se") << false << 3;

    QFile file(":/data/lorem.txt");
    if (!file.open(QFile::ReadOnly))
        qFatal("Can't open lorem.txt");

    const QByteArray content = file.readAll();
    BOBUIest::addRow("lorem-ipsum-spaces") << content << QByteArray(" ") << true << 3250;
    BOBUIest::addRow("lorem-ipsum-spaces-case-insensitive")
            << content << QByteArray(" ") << false << 3250;

    BOBUIest::addRow("lorem-ipsum-l") << content << QByteArray("l") << true << 771;
    BOBUIest::addRow("lorem-ipsum-l-case-insensitive")
            << content << QByteArray("l") << false << 772;

    BOBUIest::addRow("lorem-ipsum-lo") << content << QByteArray("lo") << true << 130;
    BOBUIest::addRow("lorem-ipsum-lo-case-insensitive")
            << content << QByteArray("lo") << false << 131;

    BOBUIest::addRow("lorem-ipsum-lor") << content << QByteArray("lor") << true << 122;
    BOBUIest::addRow("lorem-ipsum-lor-case-insensitive")
            << content << QByteArray("lor") << false << 123;

    BOBUIest::addRow("lorem-ipsum-lore") << content << QByteArray("lore") << true << 73;
    BOBUIest::addRow("lorem-ipsum-lore-case-insensitive")
            << content << QByteArray("lore") << false << 74;

    BOBUIest::addRow("lorem-ipsum-lorem") << content << QByteArray("lorem") << true << 34;
    BOBUIest::addRow("lorem-ipsum-lorem-case-insensitive")
            << content << QByteArray("lorem") << false << 35;

    BOBUIest::addRow("lorem-ipsum-lorem i") << content << QByteArray("lorem i") << true << 5;
    BOBUIest::addRow("lorem-ipsum-lorem i-case-insensitive")
            << content << QByteArray("lorem i") << false << 6;

    BOBUIest::addRow("lorem-ipsum-et explicabo s") << content << QByteArray("et explicabo s") << true << 3;
    BOBUIest::addRow("lorem-ipsum-et explicabo s-case-insensitive")
            << content << QByteArray("et explicabo s") << false << 3;
}

template<typename T, typename U>
void tst_QStringTokenizer::tokenize() const
{
    QFETCH(QByteArray, input);
    QFETCH(QByteArray, separator);
    QFETCH(bool, caseSensitive);
    QFETCH(int, expectedCount);

    T haystack = fromByteArray<T>(input);
    U needle = fromByteArray<U>(separator);

    const BobUI::CaseSensitivity sensitivity = caseSensitive ? BobUI::CaseSensitive : BobUI::CaseInsensitive;
    QBENCHMARK {
        QStringTokenizer tok(haystack, needle, sensitivity);
        qsizetype count = 0;
        for (auto res : tok) {
            count++;
            Q_UNUSED(res);
        }
        QCOMPARE(count, expectedCount);
    }
}

BOBUIEST_MAIN(tst_QStringTokenizer)

#include "tst_bench_qstringtokenizer.moc"
