// Copyright (C) 2020 Klarälvdalens Datakonsult AB, a KDAB Group company, info@kdab.com, author Marc Mutz <marc.mutz@kdab.com>
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

#include <QStringTokenizer>
#include <QStringBuilder>

#include <BOBUIest>

#include <string>

Q_DECLARE_METATYPE(BobUI::SplitBehavior)

namespace {
class tst_QStringTokenizer : public QObject
{
    Q_OBJECT

public:
    enum class Content : bool { Null, Empty };
    Q_ENUM(Content)

private Q_SLOTS:
    void constExpr() const;
    void basics_data() const;
    void basics() const;
    void toContainer() const;

    void emptyResult_L1_data() const { emptyResult_data_impl(); }
    void emptyResult_L1() const { emptyResult_impl<QLatin1StringView>(); }
    void emptyResult_U16_data() const { emptyResult_data_impl(); }
    void emptyResult_U16() const { emptyResult_impl<QStringView>(); }
private:
    template <typename View>
    void emptyResult_impl() const;
    void emptyResult_data_impl() const;
};

static QStringList skipped(const QStringList &sl)
{
    QStringList result;
    result.reserve(sl.size());
    for (const QString &s : sl) {
        if (!s.isEmpty())
            result.push_back(s);
    }
    return result;
}

QString toQString(QStringView str)
{
    return str.toString();
}

template <typename Container>
QStringList toQStringList(const Container &c)
{
    QStringList r;
    for (auto &&e : c)
        r.push_back(toQString(e));
    return r;
}
} // namespace

void tst_QStringTokenizer::constExpr() const
{
    // compile-time checks
    {
        constexpr auto tok = qTokenize(u"a,b,c", u",");
        Q_UNUSED(tok);
    }
    {
        constexpr auto tok = qTokenize(u"a,b,c", u',');
        Q_UNUSED(tok);
    }
}

void tst_QStringTokenizer::basics_data() const
{
    BOBUIest::addColumn<BobUI::SplitBehavior>("sb");
    BOBUIest::addColumn<BobUI::CaseSensitivity>("cs");

#define ROW(sb, cs) \
    do { BOBUIest::addRow("%s/%s", #sb, #cs) << BobUI::SplitBehavior{BobUI::sb} << BobUI::cs; } while (0)

    ROW(KeepEmptyParts, CaseSensitive);
    ROW(KeepEmptyParts, CaseInsensitive);
    ROW(SkipEmptyParts, CaseSensitive);
    ROW(SkipEmptyParts, CaseInsensitive);

#undef ROW
}

void tst_QStringTokenizer::basics() const
{
    QFETCH(const BobUI::SplitBehavior, sb);
    QFETCH(const BobUI::CaseSensitivity, cs);

    auto expected = QStringList{"", "a", "b", "c", "d", "e", ""};
    if (sb & BobUI::SkipEmptyParts)
        expected = skipped(expected);
    QCOMPARE(toQStringList(qTokenize(u",a,b,c,d,e,", u',', sb, cs)), expected);
    QCOMPARE(toQStringList(qTokenize(u",a,b,c,d,e,", u',', cs, sb)), expected);

    {
        auto tok = qTokenize(expected.join(u'x'), u"X" % QString(), BobUI::CaseInsensitive);
        // the temporary QStrings returned from join() and the QStringBuilder expression
        // are now destroyed, but 'tok' should keep both alive
        QCOMPARE(toQStringList(tok), expected);
    }

    using namespace std::string_literals;

    {
        auto tok = qTokenize(expected.join(u'x'), u"X"s, BobUI::CaseInsensitive);
        QCOMPARE(toQStringList(tok), expected);
    }

    {
        auto tok = qTokenize(expected.join(u'x'), QLatin1Char('x'), cs, sb);
        QCOMPARE(toQStringList(tok), expected);
    }
}

void tst_QStringTokenizer::toContainer() const
{
    // QStringView value_type:
    {
        auto tok = qTokenize(u"a,b,c", u',');
        auto v = tok.toContainer();
        QVERIFY((std::is_same_v<decltype(v), QList<QStringView>>));
    }
    // QLatin1String value_type
    {
        auto tok = qTokenize(QLatin1String{"a,b,c"}, u',');
        auto v = tok.toContainer();
        QVERIFY((std::is_same_v<decltype(v), QList<QLatin1String>>));
    }
    // QLatin1String value_type into QStringList
    {
        auto tok = qTokenize(QLatin1String{"a,b,c"}, u',');
        QStringList result;
        tok.toContainer(result);
        QCOMPARE(result, QStringList({"a", "b", "c"}));
    }
    // QLatin1String value_type into QStringList: rvalue overload
    {
        QStringList result;
        qTokenize(QLatin1String{"a,b,c"}, u',').toContainer(result);
        QCOMPARE(result, QStringList({"a", "b", "c"}));
    }
}

void tst_QStringTokenizer::emptyResult_data_impl() const
{
    // try really hard to get an empty result...

    BOBUIest::addColumn<Content>("haystack");
    BOBUIest::addColumn<Content>("needle");
    BOBUIest::addColumn<BobUI::SplitBehavior>("behavior");

    const auto str = [] (auto e) {
        using E = decltype(e);
        const auto me = QMetaEnum::fromType<E>();
        if constexpr (std::is_enum_v<E>)
            return me.valueToKey(qToUnderlying(e));
        else
            return me.valueToKey(e.toInt()); // QFlags
    };

    for (auto haystack : {Content::Null, Content::Empty}) {
        for (auto needle : {Content::Null, Content::Empty}) {
            for (auto behavior : {BobUI::KeepEmptyParts, BobUI::SkipEmptyParts}) {
                BOBUIest::addRow("%s/%s (%s)",
                              str(haystack),
                              str(needle),
                              str(BobUI::SplitBehavior{behavior}))
                        << haystack << needle << BobUI::SplitBehavior{behavior};
            }
        }
    }
}

template <typename View>
void tst_QStringTokenizer::emptyResult_impl() const
{
    QFETCH(const Content, haystack);
    QFETCH(const Content, needle);
    QFETCH(const BobUI::SplitBehavior, behavior);

    auto select = [](Content c, View null, View empty) {
        switch (c) {
        case Content::Empty: return empty;
        case Content::Null:  return null;
        }
        Q_UNREACHABLE_RETURN(null);
    };

    const auto null = View{nullptr};
    QVERIFY(null.isNull());

    using Char = typename View::value_type;
    const Char ch{0};
    const auto empty = View{&ch, qsizetype{0}};
    QVERIFY(empty.isEmpty());

    {
        const auto tok = qTokenize(select(haystack, null, empty),
                                   select(needle, null, empty),
                                   behavior);
        if (behavior & BobUI::SkipEmptyParts)
            QCOMPARE_EQ(tok.begin(), tok.end()); // iow: empty
        else
            QCOMPARE_NE(tok.begin(), tok.end()); // iow: not empty
    }
}

BOBUIEST_APPLESS_MAIN(tst_QStringTokenizer)
#include "tst_qstringtokenizer.moc"
