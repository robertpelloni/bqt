// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

#include <BOBUIest>
#include <BobUIXml/BobUIXml>
#include <BobUIGui/QFontInfo>
#include <BobUIGui/QFontMetrics>

#include "private/qcssparser_p.h"

class tst_QCssParser : public QObject
{
    Q_OBJECT

private slots:
    void scanner_data();
    void scanner();
    void term_data();
    void term();
    void expr_data();
    void expr();
    void import();
    void media();
    void animation();
    void page();
    void ruleset();
    void selector_data();
    void selector();
    void prio();
    void escapes();
    void malformedDeclarations_data();
    void malformedDeclarations();
    void invalidAtKeywords();
    void marginValue();
    void marginValue_data();
    void colorValue_data();
    void colorValue();
    void styleSelector_data();
    void styleSelector();
    void specificity_data();
    void specificity();
    void specificitySort_data();
    void specificitySort();
    void rulesForNode_data();
    void rulesForNode();
    void shorthandBackgroundProperty_data();
    void shorthandBackgroundProperty();
    void pseudoElement_data();
    void pseudoElement();
    void gradient_data();
    void gradient();
    void extractFontFamily_data();
    void extractFontFamily();
    void extractFontSize_data();
    void extractFontSize();
    void extractBorder_data();
    void extractBorder();
    void extractBorderImage_data();
    void extractBorderImage();
    void extractBorderImageCuts_data();
    void extractBorderImageCuts();
    void noTextDecoration();
    void quotedAndUnquotedIdentifiers();
    void whitespaceValues_data();
    void whitespaceValues();
    void strokeLineCapValues_data();
    void strokeLineCapValues();
    void strokeLineJoinValues_data();
    void strokeLineJoinValues();
    void borderColor_data();
    void borderColor();
};

void tst_QCssParser::scanner_data()
{
    BOBUIest::addColumn<QString>("input");
    BOBUIest::addColumn<QString>("output");

#if defined(Q_OS_ANDROID)
    QDir d(":/");
#else
    QDir d(BOBUI_TESTCASE_SOURCEDIR);
#endif
    d.cd("testdata");
    d.cd("scanner");
    const auto entries = d.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot);
    for (const QFileInfo &test : entries) {
        QString dir = test.absoluteFilePath() + QDir::separator();
        BOBUIest::newRow(qPrintable(test.baseName()))
            << dir + "input"
            << dir + "output"
            ;
    }
}


static const char *tokenName(QCss::TokenType t)
{
    switch (t) {
        case QCss::NONE: return "NONE";
        case QCss::S: return "S";
        case QCss::CDO: return "CDO";
        case QCss::CDC: return "CDC";
        case QCss::INCLUDES: return "INCLUDES";
        case QCss::DASHMATCH: return "DASHMATCH";
        case QCss::BEGINSWITH: return "BEGINSWITH";
        case QCss::ENDSWITH: return "ENDSWITH";
        case QCss::CONTAINS: return "CONTAINS";
        case QCss::LBRACE: return "LBRACE";
        case QCss::PLUS: return "PLUS";
        case QCss::GREATER: return "GREATER";
        case QCss::COMMA: return "COMMA";
        case QCss::TILDE: return "TILDE";
        case QCss::STRING: return "STRING";
        case QCss::INVALID: return "INVALID";
        case QCss::IDENT: return "IDENT";
        case QCss::HASH: return "HASH";
        case QCss::ATKEYWORD_SYM: return "ATKEYWORD_SYM";
        case QCss::EXCLAMATION_SYM: return "EXCLAMATION_SYM";
        case QCss::LENGTH: return "LENGTH";
        case QCss::PERCENTAGE: return "PERCENTAGE";
        case QCss::NUMBER: return "NUMBER";
        case QCss::FUNCTION: return "FUNCTION";
        case QCss::COLON: return "COLON";
        case QCss::SEMICOLON: return "SEMICOLON";
        case QCss::RBRACE: return "RBRACE";
        case QCss::SLASH: return "SLASH";
        case QCss::MINUS: return "MINUS";
        case QCss::DOT: return "DOT";
        case QCss::STAR: return "STAR";
        case QCss::LBRACKET: return "LBRACKET";
        case QCss::RBRACKET: return "RBRACKET";
        case QCss::EQUAL: return "EQUAL";
        case QCss::LPAREN: return "LPAREN";
        case QCss::RPAREN: return "RPAREN";
        case QCss::OR: return "OR";
    }
    return "";
}

static void debug(const QList<QCss::Symbol> &symbols, int index = -1)
{
    qDebug() << "all symbols:";
    for (int i = 0; i < symbols.size(); ++i)
        qDebug() << '(' << i << "); Token:" << tokenName(symbols.at(i).token) << "; Lexem:" << symbols.at(i).lexem();
    if (index != -1)
        qDebug() << "failure at index" << index;
}

//static void debug(const QCss::Parser &p) { debug(p.symbols); }

void tst_QCssParser::scanner()
{
    QFETCH(QString, input);
    QFETCH(QString, output);

    QFile inputFile(input);
    QVERIFY(inputFile.open(QIODevice::ReadOnly|QIODevice::Text));
    QList<QCss::Symbol> symbols;
    QCss::Scanner::scan(QCss::Scanner::preprocess(QString::fromUtf8(inputFile.readAll())), &symbols);

    QVERIFY(symbols.size() > 1);
    QCOMPARE(symbols.last().token, QCss::S);
    QCOMPARE(symbols.last().lexem(), QLatin1String("\n"));
    symbols.remove(symbols.size() - 1, 1);

    QFile outputFile(output);
    QVERIFY(outputFile.open(QIODevice::ReadOnly|QIODevice::Text));
    QStringList lines;
    while (!outputFile.atEnd()) {
        QString line = QString::fromUtf8(outputFile.readLine());
        if (line.endsWith(QLatin1Char('\n')))
            line.chop(1);
        lines.append(line);
    }

    if (lines.size() != symbols.size()) {
        debug(symbols);
        QCOMPARE(lines.size(), symbols.size());
    }

    for (int i = 0; i < lines.size(); ++i) {
        QStringList l = lines.at(i).split(QChar::fromLatin1('|'));
        QCOMPARE(l.size(), 2);
        const QString expectedToken = l.at(0);
        const QString expectedLexem = l.at(1);
        QString actualToken = QString::fromLatin1(tokenName(symbols.at(i).token));
        if (actualToken != expectedToken) {
            debug(symbols, i);
            QCOMPARE(actualToken, expectedToken);
        }
        if (symbols.at(i).lexem() != expectedLexem) {
            debug(symbols, i);
            QCOMPARE(symbols.at(i).lexem(), expectedLexem);
        }
    }
}

Q_DECLARE_METATYPE(QCss::Value)

void tst_QCssParser::term_data()
{
    BOBUIest::addColumn<bool>("parseSuccess");
    BOBUIest::addColumn<QString>("css");
    BOBUIest::addColumn<QCss::Value>("expectedValue");

    QCss::Value val;

    val.type = QCss::Value::Percentage;
    val.variant = QVariant(double(200));
    BOBUIest::newRow("percentage") << true << "200%" << val;

    val.type = QCss::Value::Length;
    val.variant = QString("10px");
    BOBUIest::newRow("px") << true << "10px" << val;

    val.type = QCss::Value::Length;
    val.variant = QString("10cm");
    BOBUIest::newRow("cm") << true << "10cm" << val;

    val.type = QCss::Value::Length;
    val.variant = QString("10mm");
    BOBUIest::newRow("mm") << true << "10mm" << val;

    val.type = QCss::Value::Length;
    val.variant = QString("10pt");
    BOBUIest::newRow("pt") << true << "10pt" << val;

    val.type = QCss::Value::Length;
    val.variant = QString("10pc");
    BOBUIest::newRow("pc") << true << "10pc" << val;

    val.type = QCss::Value::Length;
    val.variant = QString("42in");
    BOBUIest::newRow("inch") << true << "42in" << val;

    val.type = QCss::Value::Length;
    val.variant = QString("10deg");
    BOBUIest::newRow("deg") << true << "10deg" << val;

    val.type = QCss::Value::Length;
    val.variant = QString("10rad");
    BOBUIest::newRow("rad") << true << "10rad" << val;

    val.type = QCss::Value::Length;
    val.variant = QString("10grad");
    BOBUIest::newRow("grad") << true << "10grad" << val;

    val.type = QCss::Value::Length;
    val.variant = QString("10ms");
    BOBUIest::newRow("time") << true << "10ms" << val;

    val.type = QCss::Value::Length;
    val.variant = QString("10s");
    BOBUIest::newRow("times") << true << "10s" << val;

    val.type = QCss::Value::Length;
    val.variant = QString("10hz");
    BOBUIest::newRow("hz") << true << "10hz" << val;

    val.type = QCss::Value::Length;
    val.variant = QString("10khz");
    BOBUIest::newRow("khz") << true << "10khz" << val;

    val.type = QCss::Value::Length;
    val.variant = QString("10myunit");
    BOBUIest::newRow("dimension") << true << "10myunit" << val;

    val.type = QCss::Value::Percentage;

    val.type = QCss::Value::Percentage;
    val.variant = QVariant(double(-200));
    BOBUIest::newRow("minuspercentage") << true << "-200%" << val;

    val.type = QCss::Value::Length;
    val.variant = QString("10em");
    BOBUIest::newRow("ems") << true << "10em" << val;

    val.type = QCss::Value::String;
    val.variant = QVariant(QString("foo"));
    BOBUIest::newRow("string") << true << "\"foo\"" << val;

    val.type = QCss::Value::Function;
    val.variant = QVariant(QStringList() << "myFunc" << "23, (nested text)");
    BOBUIest::newRow("function") << true << "myFunc(23, (nested text))" << val;

    BOBUIest::newRow("function_failure") << false << "myFunction((blah)" << val;
    BOBUIest::newRow("function_failure2") << false << "+myFunc(23, (nested text))" << val;

    val.type = QCss::Value::Color;
    val.variant = QVariant(QColor("#12ff34"));
    BOBUIest::newRow("hexcolor") << true << "#12ff34" << val;

    val.type = QCss::Value::Color;
    val.variant = QVariant(QColor("#ffbb00"));
    BOBUIest::newRow("hexcolor2") << true << "#fb0" << val;

    val.type = QCss::Value::Uri;
    val.variant = QString("www.kde.org");
    BOBUIest::newRow("uri1") << true << "url(\"www.kde.org\")" << val;
    BOBUIest::newRow("uri2") << true << "url(www.kde.org)" << val;

    val.type = QCss::Value::Uri;
    val.variant = QString("www.kde.org/test?key=value&v=123");
    BOBUIest::newRow("uri_query_quoted") << true << "url(\"www.kde.org/test?key=value&v=123\")" << val;
    BOBUIest::newRow("uri_query_unquoted") << true << "url(www.kde.org/test?key=value&v=123)" << val;

    val.type = QCss::Value::KnownIdentifier;
    val.variant = int(QCss::Value_Italic);
    BOBUIest::newRow("italic") << true << "italic" << val;

    val.type = QCss::Value::KnownIdentifier;
    val.variant = int(QCss::Value_Italic);
    BOBUIest::newRow("ItaLIc") << true << "ItaLIc" << val;
}

void tst_QCssParser::term()
{
    QFETCH(bool, parseSuccess);
    QFETCH(QString, css);
    QFETCH(QCss::Value, expectedValue);

    QCss::Parser parser(css);
    QCss::Value val;
    QVERIFY(parser.testTerm());
    QEXPECT_FAIL("uri_query_unquoted", "BOBUIBUG-131842", Abort);
    QCOMPARE(parser.parseTerm(&val), parseSuccess);
    if (parseSuccess) {
        QCOMPARE(int(val.type), int(expectedValue.type));
        if (val.variant != expectedValue.variant) {
            qDebug() << "val.variant:" << val.variant << "expectedValue.variant:" << expectedValue.variant;
            QCOMPARE(val.variant, expectedValue.variant);
        }
    }
}

void tst_QCssParser::expr_data()
{
    BOBUIest::addColumn<bool>("parseSuccess");
    BOBUIest::addColumn<QString>("css");
    BOBUIest::addColumn<QList<QCss::Value>>("expectedValues");

    QList<QCss::Value> values;
    QCss::Value val;

    QCss::Value comma;
    comma.type = QCss::Value::TermOperatorComma;

    val.type = QCss::Value::Identifier;
    val.variant = QLatin1String("foo");
    values << val;
    values << comma;
    val.variant = QLatin1String("bar");
    values << val;
    values << comma;
    val.variant = QLatin1String("baz");
    values << val;
    BOBUIest::newRow("list") << true << "foo, bar, baz" << values;
    values.clear();
}

void tst_QCssParser::expr()
{
    QFETCH(bool, parseSuccess);
    QFETCH(QString, css);
    QFETCH(QList<QCss::Value>, expectedValues);

    QCss::Parser parser(css);
    QList<QCss::Value> values;
    QVERIFY(parser.testExpr());
    QCOMPARE(parser.parseExpr(&values), parseSuccess);
    if (parseSuccess) {
        QCOMPARE(values.size(), expectedValues.size());

        for (int i = 0; i < values.size(); ++i) {
            QCOMPARE(int(values.at(i).type), int(expectedValues.at(i).type));
            QCOMPARE(values.at(i).variant, expectedValues.at(i).variant);
        }
    }
}

void tst_QCssParser::import()
{
    QCss::Parser parser("@import \"plainstring\";");
    QVERIFY(parser.testImport());
    QCss::ImportRule rule;
    QVERIFY(parser.parseImport(&rule));
    QCOMPARE(rule.href, QString("plainstring"));

    parser = QCss::Parser("@import url(\"www.kde.org\") print/*comment*/,screen;");
    QVERIFY(parser.testImport());
    QVERIFY(parser.parseImport(&rule));
    QCOMPARE(rule.href, QString("www.kde.org"));
    QCOMPARE(rule.media.size(), 2);
    QCOMPARE(rule.media.at(0), QString("print"));
    QCOMPARE(rule.media.at(1), QString("screen"));
}

void tst_QCssParser::media()
{
    QCss::Parser parser("@media print/*comment*/,screen /*comment to ignore*/{ }");
    QVERIFY(parser.testMedia());
    QCss::MediaRule rule;
    QVERIFY(parser.parseMedia(&rule));
    QCOMPARE(rule.media.size(), 2);
    QCOMPARE(rule.media.at(0), QString("print"));
    QCOMPARE(rule.media.at(1), QString("screen"));
    QVERIFY(rule.styleRules.isEmpty());
}

void tst_QCssParser::animation()
{
    QCss::Parser parser("@keyframes emptyAnimation{} "
                        "@keyframes motion{from {x : 10;} to {x : 50;}} "
                        "@keyframes color{0% {fill : blue;} 25% {fill : yellow;} 100% {fill : red;}}");

    {
        QCss::AnimationRule rule;
        QVERIFY(parser.testAnimation());
        QVERIFY(parser.parseAnimation(&rule));
        QCOMPARE(rule.animName, QStringLiteral("emptyAnimation"));
        QCOMPARE(rule.ruleSets.size(), 0);
    }

    {
        QCss::AnimationRule rule;
        QVERIFY(parser.testAnimation());
        QVERIFY(parser.parseAnimation(&rule));
        QCOMPARE(rule.animName, QStringLiteral("motion"));
        QCOMPARE(rule.ruleSets.size(), 2);
        QCOMPARE(rule.ruleSets[0].keyFrame, 0);
        QCOMPARE(rule.ruleSets[1].keyFrame, 1);
        QCOMPARE(rule.ruleSets[0].declarations[0].d->property, QStringLiteral("x"));
        QCOMPARE(rule.ruleSets[0].declarations[0].d->values[0].toString(), QStringLiteral("10"));
        QCOMPARE(rule.ruleSets[1].declarations[0].d->property, QStringLiteral("x"));
        QCOMPARE(rule.ruleSets[1].declarations[0].d->values[0].toString(), QStringLiteral("50"));
    }

    {
        QCss::AnimationRule rule;
        QVERIFY(parser.testAnimation());
        QVERIFY(parser.parseAnimation(&rule));
        QCOMPARE(rule.animName, QStringLiteral("color"));
        QCOMPARE(rule.ruleSets.size(), 3);
        QCOMPARE(rule.ruleSets[0].keyFrame, 0);
        QCOMPARE(rule.ruleSets[1].keyFrame, 0.25);
        QCOMPARE(rule.ruleSets[2].keyFrame, 1);
        QCOMPARE(rule.ruleSets[0].declarations[0].d->property, QStringLiteral("fill"));
        QCOMPARE(rule.ruleSets[0].declarations[0].d->values[0].toString(), QStringLiteral("blue"));
        QCOMPARE(rule.ruleSets[1].declarations[0].d->property, QStringLiteral("fill"));
        QCOMPARE(rule.ruleSets[1].declarations[0].d->values[0].toString(), QStringLiteral("yellow"));
        QCOMPARE(rule.ruleSets[2].declarations[0].d->property, QStringLiteral("fill"));
        QCOMPARE(rule.ruleSets[2].declarations[0].d->values[0].toString(), QStringLiteral("red"));
    }
}

void tst_QCssParser::page()
{
    QCss::Parser parser("@page :first/*comment to ignore*/{ }");
    QVERIFY(parser.testPage());
    QCss::PageRule rule;
    QVERIFY(parser.parsePage(&rule));
    QCOMPARE(rule.selector, QString("first"));
    QVERIFY(rule.declarations.isEmpty());
}

void tst_QCssParser::ruleset()
{
    {
        QCss::Parser parser("p/*foo*/{ }");
        QVERIFY(parser.testRuleset());
        QCss::StyleRule rule;
        QVERIFY(parser.parseRuleset(&rule));
        QCOMPARE(rule.selectors.size(), 1);
        QCOMPARE(rule.selectors.at(0).basicSelectors.size(), 1);
        QCOMPARE(rule.selectors.at(0).basicSelectors.at(0).elementName, QString("p"));
        QVERIFY(rule.declarations.isEmpty());
    }

    {
        QCss::Parser parser("p/*comment*/,div{ }");
        QVERIFY(parser.testRuleset());
        QCss::StyleRule rule;
        QVERIFY(parser.parseRuleset(&rule));
        QCOMPARE(rule.selectors.size(), 2);
        QCOMPARE(rule.selectors.at(0).basicSelectors.size(), 1);
        QCOMPARE(rule.selectors.at(0).basicSelectors.at(0).elementName, QString("p"));
        QCOMPARE(rule.selectors.at(1).basicSelectors.size(), 1);
        QCOMPARE(rule.selectors.at(1).basicSelectors.at(0).elementName, QString("div"));
        QVERIFY(rule.declarations.isEmpty());
    }

    {
        QCss::Parser parser(":before, :after { }");
        QVERIFY(parser.testRuleset());
        QCss::StyleRule rule;
        QVERIFY(parser.parseRuleset(&rule));
        QCOMPARE(rule.selectors.size(), 2);

        QCOMPARE(rule.selectors.at(0).basicSelectors.size(), 1);
        QCOMPARE(rule.selectors.at(0).basicSelectors.at(0).pseudos.size(), 1);
        QCOMPARE(rule.selectors.at(0).basicSelectors.at(0).pseudos.at(0).name, QString("before"));

        QCOMPARE(rule.selectors.at(1).basicSelectors.size(), 1);
        QCOMPARE(rule.selectors.at(1).basicSelectors.at(0).pseudos.size(), 1);
        QCOMPARE(rule.selectors.at(1).basicSelectors.at(0).pseudos.at(0).name, QString("after"));

        QVERIFY(rule.declarations.isEmpty());
    }

}

Q_DECLARE_METATYPE(QCss::Selector)

void tst_QCssParser::selector_data()
{
    BOBUIest::addColumn<QString>("css");
    BOBUIest::addColumn<QCss::Selector>("expectedSelector");

    {
        QCss::Selector sel;
        QCss::BasicSelector basic;

        basic.elementName = "p";
        basic.relationToNext = QCss::BasicSelector::MatchNextSelectorIfDirectAdjecent;
        sel.basicSelectors << basic;

        basic = QCss::BasicSelector();
        basic.elementName = "div";
        sel.basicSelectors << basic;

        BOBUIest::newRow("comment") << QString("p/* */+ div") << sel;
    }

    {
        QCss::Selector sel;
        QCss::BasicSelector basic;

        basic.elementName = QString();
        sel.basicSelectors << basic;

        BOBUIest::newRow("any") << QString("*") << sel;
    }

    {
        QCss::Selector sel;
        QCss::BasicSelector basic;

        basic.elementName = "e";
        sel.basicSelectors << basic;

        BOBUIest::newRow("element") << QString("e") << sel;
    }

    {
        QCss::Selector sel;
        QCss::BasicSelector basic;

        basic.elementName = "e";
        basic.relationToNext = QCss::BasicSelector::MatchNextSelectorIfAncestor;
        sel.basicSelectors << basic;

        basic.elementName = "f";
        basic.relationToNext = QCss::BasicSelector::NoRelation;
        sel.basicSelectors << basic;

        BOBUIest::newRow("descendant") << QString("e f") << sel;
    }

    {
        QCss::Selector sel;
        QCss::BasicSelector basic;

        basic.elementName = "e";
        basic.relationToNext = QCss::BasicSelector::MatchNextSelectorIfParent;
        sel.basicSelectors << basic;

        basic.elementName = "f";
        basic.relationToNext = QCss::BasicSelector::NoRelation;
        sel.basicSelectors << basic;

        BOBUIest::newRow("parent") << QString("e > f") << sel;
    }

    {
        QCss::Selector sel;
        QCss::BasicSelector basic;

        basic.elementName = "e";
        QCss::Pseudo pseudo;
        pseudo.name = "first-child";
        basic.pseudos.append(pseudo);
        sel.basicSelectors << basic;

        BOBUIest::newRow("first-child") << QString("e:first-child") << sel;
    }

    {
        QCss::Selector sel;
        QCss::BasicSelector basic;

        basic.elementName = "e";
        QCss::Pseudo pseudo;
        pseudo.name = "c";
        pseudo.function = "lang";
        basic.pseudos.append(pseudo);
        sel.basicSelectors << basic;

        BOBUIest::newRow("lang") << QString("e:lang(c)") << sel;
    }

    {
        QCss::Selector sel;
        QCss::BasicSelector basic;

        basic.elementName = "e";
        basic.relationToNext = QCss::BasicSelector::MatchNextSelectorIfDirectAdjecent;
        sel.basicSelectors << basic;

        basic.elementName = "f";
        basic.relationToNext = QCss::BasicSelector::NoRelation;
        sel.basicSelectors << basic;

        BOBUIest::newRow("lastsibling") << QString("e + f") << sel;
    }

    {
        QCss::Selector sel;
        QCss::BasicSelector basic;

        basic.elementName = "e";
        basic.relationToNext = QCss::BasicSelector::MatchNextSelectorIfIndirectAdjecent;
        sel.basicSelectors << basic;

        basic.elementName = "f";
        basic.relationToNext = QCss::BasicSelector::NoRelation;
        sel.basicSelectors << basic;

        BOBUIest::newRow("previoussibling") << QString("e ~ f") << sel;
    }

    {
        QCss::Selector sel;
        QCss::BasicSelector basic;

        basic.elementName = "e";
        QCss::AttributeSelector attrSel;
        attrSel.name = "foo";
        basic.attributeSelectors << attrSel;
        sel.basicSelectors << basic;

        BOBUIest::newRow("attr") << QString("e[foo]") << sel;
    }

    {
        QCss::Selector sel;
        QCss::BasicSelector basic;

        basic.elementName = "e";
        QCss::AttributeSelector attrSel;
        attrSel.name = "foo";
        attrSel.value = "warning";
        attrSel.valueMatchCriterium = QCss::AttributeSelector::MatchEqual;
        basic.attributeSelectors << attrSel;
        sel.basicSelectors << basic;

        BOBUIest::newRow("attr-equal") << QString("e[foo=\"warning\"]") << sel;
    }

    {
        QCss::Selector sel;
        QCss::BasicSelector basic;

        basic.elementName = "e";
        QCss::AttributeSelector attrSel;
        attrSel.name = "foo";
        attrSel.value = "warning";
        attrSel.valueMatchCriterium = QCss::AttributeSelector::MatchIncludes;
        basic.attributeSelectors << attrSel;
        sel.basicSelectors << basic;

        BOBUIest::newRow("attr-includes") << QString("e[foo~=\"warning\"]") << sel;
    }

    {
        QCss::Selector sel;
        QCss::BasicSelector basic;

        basic.elementName = "e";
        QCss::AttributeSelector attrSel;
        attrSel.name = "lang";
        attrSel.value = "en";
        attrSel.valueMatchCriterium = QCss::AttributeSelector::MatchDashMatch;
        basic.attributeSelectors << attrSel;
        sel.basicSelectors << basic;

        BOBUIest::newRow("attr-dash") << QString("e[lang|=\"en\"]") << sel;
    }

    {
        QCss::Selector sel;
        QCss::BasicSelector basic;

        basic.elementName = "e";
        QCss::AttributeSelector attrSel;
        attrSel.name = "foo";
        attrSel.value = "warning";
        attrSel.valueMatchCriterium = QCss::AttributeSelector::MatchContains;
        basic.attributeSelectors << attrSel;
        sel.basicSelectors << basic;

        BOBUIest::newRow("attr-contains") << QString("e[foo*=\"warning\"]") << sel;
    }

    {
        QCss::Selector sel;
        QCss::BasicSelector basic;

        basic.elementName = "div";

        QCss::AttributeSelector attrSel;
        attrSel.name = "class";
        attrSel.valueMatchCriterium = QCss::AttributeSelector::MatchIncludes;
        attrSel.value = "warning";
        basic.attributeSelectors.append(attrSel);

        attrSel.value = "foo";
        basic.attributeSelectors.append(attrSel);

        sel.basicSelectors << basic;

        BOBUIest::newRow("class") << QString("div.warning.foo") << sel;
    }

    {
        QCss::Selector sel;
        QCss::BasicSelector basic;

        basic.elementName = "e";
        basic.ids << "myid";
        sel.basicSelectors << basic;

        BOBUIest::newRow("id") << QString("e#myid") << sel;
    }
}

void tst_QCssParser::selector()
{
    QFETCH(QString, css);
    QFETCH(QCss::Selector, expectedSelector);

    QCss::Parser parser(css);
    QVERIFY(parser.testSelector());
    QCss::Selector selector;
    QVERIFY(parser.parseSelector(&selector));

    QCOMPARE(selector.basicSelectors.size(), expectedSelector.basicSelectors.size());
    for (int i = 0; i < selector.basicSelectors.size(); ++i) {
        const QCss::BasicSelector sel = selector.basicSelectors.at(i);
        const QCss::BasicSelector expectedSel = expectedSelector.basicSelectors.at(i);
        QCOMPARE(sel.elementName, expectedSel.elementName);
        QCOMPARE(int(sel.relationToNext), int(expectedSel.relationToNext));

        QCOMPARE(sel.pseudos.size(), expectedSel.pseudos.size());
        for (int i = 0; i < sel.pseudos.size(); ++i) {
            QCOMPARE(sel.pseudos.at(i).name, expectedSel.pseudos.at(i).name);
            QCOMPARE(sel.pseudos.at(i).function, expectedSel.pseudos.at(i).function);
        }

        QCOMPARE(sel.attributeSelectors.size(), expectedSel.attributeSelectors.size());
        for (int i = 0; i < sel.attributeSelectors.size(); ++i) {
            QCOMPARE(sel.attributeSelectors.at(i).name, expectedSel.attributeSelectors.at(i).name);
            QCOMPARE(sel.attributeSelectors.at(i).value, expectedSel.attributeSelectors.at(i).value);
            QCOMPARE(int(sel.attributeSelectors.at(i).valueMatchCriterium), int(expectedSel.attributeSelectors.at(i).valueMatchCriterium));
        }
    }
}

void tst_QCssParser::prio()
{
    {
        QCss::Parser parser("!important");
        QVERIFY(parser.testPrio());
    }
    {
        QCss::Parser parser("!impOrTAnt");
        QVERIFY(parser.testPrio());
    }
    {
        QCss::Parser parser("!\"important\"");
        QVERIFY(!parser.testPrio());
        QCOMPARE(parser.index, 0);
    }
    {
        QCss::Parser parser("!importbleh");
        QVERIFY(!parser.testPrio());
        QCOMPARE(parser.index, 0);
    }
}

void tst_QCssParser::escapes()
{
    QCss::Parser parser("\\hello");
    parser.test(QCss::IDENT);
    QCOMPARE(parser.lexem(), QString("hello"));
}

void tst_QCssParser::malformedDeclarations_data()
{
    BOBUIest::addColumn<QString>("css");

    BOBUIest::newRow("1") << QString("p { color:green }");
    BOBUIest::newRow("2") << QString("p { color:green; color }  /* malformed declaration missing ':', value */");
    BOBUIest::newRow("3") << QString("p { color:red;   color; color:green }  /* same with expected recovery */");
    BOBUIest::newRow("4") << QString("p { color:green; color: } /* malformed declaration missing value */");
    BOBUIest::newRow("5") << QString("p { color:red;   color:; color:green } /* same with expected recovery */");
    BOBUIest::newRow("6") << QString("p { color:green; color{;color:maroon} } /* unexpected tokens { } */");
    BOBUIest::newRow("7") << QString("p { color:red;   color{;color:maroon}; color:green } /* same with recovery */");
}

void tst_QCssParser::malformedDeclarations()
{
    QFETCH(QString, css);
    QCss::Parser parser(css);
    QVERIFY(parser.testRuleset());
    QCss::StyleRule rule;
    QVERIFY(parser.parseRuleset(&rule));

    QCOMPARE(rule.selectors.size(), 1);
    QCOMPARE(rule.selectors.at(0).basicSelectors.size(), 1);
    QCOMPARE(rule.selectors.at(0).basicSelectors.at(0).elementName, QString("p"));

    QVERIFY(rule.declarations.size() >= 1);
    QCOMPARE(int(rule.declarations.last().d->propertyId), int(QCss::Color));
    QCOMPARE(rule.declarations.last().d->values.size(), 1);
    QCOMPARE(int(rule.declarations.last().d->values.at(0).type), int(QCss::Value::Identifier));
    QCOMPARE(rule.declarations.last().d->values.at(0).variant.toString(), QString("green"));
}

void tst_QCssParser::invalidAtKeywords()
{
    QCss::Parser parser(""
    "@three-dee {"
    "  @background-lighting {"
    "    azimuth: 30deg;"
    "    elevation: 190deg;"
    "  }"
    "  h1 { color: red }"
    "}"
    "h1 { color: blue }");

    QCss::StyleSheet sheet;
    QVERIFY(parser.parse(&sheet));

    QCOMPARE(sheet.styleRules.size() + sheet.nameIndex.size(), 1);
    QCss::StyleRule rule =  (!sheet.styleRules.isEmpty()) ?
            sheet.styleRules.at(0) : *sheet.nameIndex.begin();

    QCOMPARE(rule.selectors.size(), 1);
    QCOMPARE(rule.selectors.at(0).basicSelectors.size(), 1);
    QCOMPARE(rule.selectors.at(0).basicSelectors.at(0).elementName, QString("h1"));

    QCOMPARE(rule.declarations.size(), 1);
    QCOMPARE(int(rule.declarations.at(0).d->propertyId), int(QCss::Color));
    QCOMPARE(rule.declarations.at(0).d->values.size(), 1);
    QCOMPARE(int(rule.declarations.at(0).d->values.at(0).type), int(QCss::Value::Identifier));
    QCOMPARE(rule.declarations.at(0).d->values.at(0).variant.toString(), QString("blue"));
}


void tst_QCssParser::colorValue_data()
{
    BOBUIest::addColumn<QString>("css");
    BOBUIest::addColumn<QColor>("expectedColor");

    BOBUIest::newRow("identifier") << "color: black" << QColor("black");
    BOBUIest::newRow("string") << "color: \"green\"" << QColor("green");
    BOBUIest::newRow("hexcolor") << "color: #12af0e" << QColor(0x12, 0xaf, 0x0e);
    BOBUIest::newRow("functional1") << "color: rgb(21, 45, 73)" << QColor(21, 45, 73);
    BOBUIest::newRow("functional2") << "color: rgb(100%, 0%, 100%)" << QColor(0xff, 0, 0xff);
    BOBUIest::newRow("rgb") << "color: rgb(10, 20, 30)" << QColor(10, 20, 30);
    BOBUIest::newRow("rgba") << "color: rgba(10, 20, 30, 40)" << QColor(10, 20, 30, 40);
    BOBUIest::newRow("rgbaf") << "color: rgba(10, 20, 30, 0.5)" << QColor(10, 20, 30, 127);
    BOBUIest::newRow("hsv") << "color: hsv(10, 20, 30)" << QColor::fromHsv(10, 20, 30);
    BOBUIest::newRow("hsva") << "color: hsva(10, 20, 30, 40)" << QColor::fromHsv(10, 20, 30, 40);
    // the percent and float values are well chosen to not get in trouble due to rounding errors
    BOBUIest::newRow("hsva-percent") << "color: hsva(100%, 20%, 40%, 60%)" << QColor::fromHsv(359, 51, 102, 153);
    BOBUIest::newRow("hsva-float") << "color: hsva(180, 20%, 40%, 0.6)" << QColor::fromHsvF(0.5f, 0.2f, 0.4f, 0.6f);
    BOBUIest::newRow("hsl") << "color: hsl(60, 100%, 50%)" << QColor::fromHsl(60., 255, 127);
    BOBUIest::newRow("hsla") << "color: hsla(240, 255, 127, 192)" << QColor::fromHsl(240, 255, 127, 192);
    BOBUIest::newRow("hsla-percent") << "color: hsla(100%, 80%, 40%, 0%)" << QColor::fromHsl(359, 204, 102, 0);
    BOBUIest::newRow("hsla-float") << "color: hsla(252, 40%, 60%, 0.2)" << QColor::fromHslF(0.7f, 0.4f, 0.6f, 0.2f);
    BOBUIest::newRow("invalid1") << "color: rgb(why, does, it, always, rain, on, me)" << QColor();
    BOBUIest::newRow("invalid2") << "color: rgba(i, meant, norway)" << QColor();
    BOBUIest::newRow("invalid3") << "color: rgb(21)" << QColor();
    BOBUIest::newRow("invalid4") << "color: rgbx(1, 2, 3)" << QColor();
    BOBUIest::newRow("invalid5") << "color: rgbax(1, 2, 3, 4)" << QColor();
    BOBUIest::newRow("invalid6") << "color: hsv(360, 0, 0)" << QColor();
    BOBUIest::newRow("invalid7") << "color: hsla(1, a, 1, 21)" << QColor();
    BOBUIest::newRow("role") << "color: palette(base)" << qApp->palette().color(QPalette::Base);
    BOBUIest::newRow("role2") << "color: palette( window-text ) " << qApp->palette().color(QPalette::WindowText);
    BOBUIest::newRow("role3") << "color: palette(accent)" << qApp->palette().color(QPalette::Accent);
    BOBUIest::newRow("transparent") << "color: transparent" << QColor(BobUI::transparent);

    BOBUIest::newRow("rgb-invalid") << "color: rgb(10, 20, 30, 40)" << QColor();
    BOBUIest::newRow("rgba-invalid") << "color: rgba(10, 20, 30)" << QColor();
}

void tst_QCssParser::colorValue()
{
    QFETCH(QString, css);
    QFETCH(QColor, expectedColor);

    QCss::Parser parser(css);
    QCss::Declaration decl;
    QVERIFY(parser.parseNextDeclaration(&decl));
    const QColor col = decl.colorValue();
    QCOMPARE(expectedColor.isValid(), col.isValid());
    QCOMPARE(col, expectedColor);
}

class DomStyleSelector : public QCss::StyleSelector
{
public:
    inline DomStyleSelector(const QDomDocument &doc, const QCss::StyleSheet &sheet)
        : doc(doc)
    {
        styleSheets.append(sheet);
    }

    QStringList nodeNames(NodePtr node) const override
    { return QStringList(reinterpret_cast<QDomElement *>(node.ptr)->tagName()); }
    QString attributeValue(NodePtr node, const QCss::AttributeSelector &aSel) const override
    { return reinterpret_cast<QDomElement *>(node.ptr)->attribute(aSel.name); }
    bool hasAttribute(NodePtr node, const QString &name) const
    { return reinterpret_cast<QDomElement *>(node.ptr)->hasAttribute(name); }
    bool hasAttributes(NodePtr node) const override
    { return reinterpret_cast<QDomElement *>(node.ptr)->hasAttributes(); }

    bool isNullNode(NodePtr node) const override
    { return reinterpret_cast<QDomElement *>(node.ptr)->isNull(); }
    NodePtr parentNode(NodePtr node) const override {
        NodePtr parent;
        parent.ptr = new QDomElement(reinterpret_cast<QDomElement *>(node.ptr)->parentNode().toElement());
        return parent;
    }
    NodePtr duplicateNode(NodePtr node) const override {
        NodePtr n;
        n.ptr = new QDomElement(*reinterpret_cast<QDomElement *>(node.ptr));
        return n;
    }
    NodePtr previousSiblingNode(NodePtr node) const override {
        NodePtr sibling;
        sibling.ptr = new QDomElement(reinterpret_cast<QDomElement *>(node.ptr)->previousSiblingElement());
        return sibling;
    }
    void freeNode(NodePtr node) const override
    { delete reinterpret_cast<QDomElement *>(node.ptr); }

private:
    QDomDocument doc;
};

Q_DECLARE_METATYPE(QDomDocument)

void tst_QCssParser::marginValue_data()
{
    BOBUIest::addColumn<QString>("css");
    BOBUIest::addColumn<QString>("expectedMargin");

    QFont f;
    int ex = QFontMetrics(f).xHeight();
    int em = QFontMetrics(f).height();

    const QString ex1234 = QString::number(ex) + QLatin1Char(' ') + QString::number(2 * ex)
        + QLatin1Char(' ') + QString::number(3 * ex) + QLatin1Char(' ')
        + QString::number(4 * ex);
    const QString em2ex4 = QLatin1String("1 ") + QString::number(2*em) + QLatin1String(" 3 ")
        + QString::number(4 * ex);

    BOBUIest::newRow("one value") << "margin: 1px" << "1 1 1 1";
    BOBUIest::newRow("two values") << "margin: 1px 2px" << "1 2 1 2";
    BOBUIest::newRow("three value") << "margin: 1px 2px 3px" << "1 2 3 2";
    BOBUIest::newRow("four values") << "margin: 1px 2px 3px 4px" << "1 2 3 4";
    BOBUIest::newRow("default px") << "margin: 1 2 3 4" << "1 2 3 4";
    BOBUIest::newRow("no unit") << "margin: 1 2 3 4" << "1 2 3 4";
    BOBUIest::newRow("em") << "margin: 1ex 2ex 3ex 4ex"
        << (QString::number(ex) + QLatin1Char(' ') + QString::number(2 * ex)
            + QLatin1Char(' ') + QString::number(3 * ex) + QLatin1Char(' ')
            + QString::number(4 * ex));
    BOBUIest::newRow("ex") << "margin: 1 2em 3px 4ex"
        << (QLatin1String("1 ") + QString::number(2 * em) + QLatin1String(" 3 ")
           + QString::number(4 * ex));

    f.setPointSize(20);
    f.setBold(true);
    ex = QFontMetrics(f).xHeight();
    em = QFontMetrics(f).height();
    BOBUIest::newRow("em2") << "font: bold 20pt; margin: 1ex 2ex 3ex 4ex"
        << (QString::number(ex) + QLatin1Char(' ') + QString::number(2 * ex)
            + QLatin1Char(' ') + QString::number(3 * ex) + QLatin1Char(' ')
            + QString::number(4 * ex));
    BOBUIest::newRow("ex2") << "margin: 1 2em 3px 4ex; font-size: 20pt; font-weight: bold;"
        << (QLatin1String("1 ") + QString::number(2 * em) + QLatin1String(" 3 ")
            + QString::number(4 * ex));

    BOBUIest::newRow("crap") << "margin: crap" << "0 0 0 0";
}

void tst_QCssParser::marginValue()
{
    QFETCH(QString, css);
    QFETCH(QString, expectedMargin);

    QDomDocument doc;
    QVERIFY(doc.setContent(QLatin1String("<!DOCTYPE test><test> <dummy/> </test>")));

    css.prepend("dummy {");
    css.append("}");

    QCss::Parser parser(css);
    QCss::StyleSheet sheet;
    QVERIFY(parser.parse(&sheet));

    DomStyleSelector testSelector(doc, sheet);
    QDomElement e = doc.documentElement().firstChildElement();
    QCss::StyleSelector::NodePtr n;
    n.ptr = &e;
    QList<QCss::StyleRule> rules = testSelector.styleRulesForNode(n);
    QList<QCss::Declaration> decls = rules.at(0).declarations;
    QCss::ValueExtractor v(decls);

    {
    int m[4];
    int p[4];
    int spacing;
    v.extractBox(m, p, &spacing);
    QString str = QString::number(m[0]) + QLatin1Char(' ') + QString::number(m[1])
        + QLatin1Char(' ') + QString::number(m[2]) + QLatin1Char(' ') + QString::number(m[3]);
    QCOMPARE(str, expectedMargin);
    }
}

void tst_QCssParser::styleSelector_data()
{
    BOBUIest::addColumn<bool>("match");
    BOBUIest::addColumn<QString>("selector");
    BOBUIest::addColumn<QString>("xml");
    BOBUIest::addColumn<QString>("elementToCheck");

    BOBUIest::newRow("plain") << true << QString("p") << QString("<p />") << QString();
    BOBUIest::newRow("noplain") << false << QString("bar") << QString("<p />") << QString();

    BOBUIest::newRow("class") << true << QString(".foo") << QString("<p class=\"foo\" />") << QString();
    BOBUIest::newRow("noclass") << false << QString(".bar") << QString("<p class=\"foo\" />") << QString();

    BOBUIest::newRow("attrset") << true << QString("[justset]") << QString("<p justset=\"bar\" />") << QString();
    BOBUIest::newRow("notattrset") << false << QString("[justset]") << QString("<p otherattribute=\"blub\" />") << QString();

    BOBUIest::newRow("attrmatch") << true << QString("[foo=bar]") << QString("<p foo=\"bar\" />") << QString();
    BOBUIest::newRow("noattrmatch") << false << QString("[foo=bar]") << QString("<p foo=\"xyz\" />") << QString();

    BOBUIest::newRow("includes") << true << QString("[foo~=bar]") << QString("<p foo=\"baz bleh bar\" />") << QString();
    BOBUIest::newRow("notincludes") << false << QString("[foo~=bar]") << QString("<p foo=\"bazblehbar\" />") << QString();

    BOBUIest::newRow("dashmatch") << true << QString("[foo|=bar]") << QString("<p foo=\"bar-bleh\" />") << QString();
    BOBUIest::newRow("nodashmatch") << false << QString("[foo|=bar]") << QString("<p foo=\"barbleh\" />") << QString();

    BOBUIest::newRow("beginswith") << true << QString("[foo^=bar]") << QString("<p foo=\"barbleh\" />") << QString();
    BOBUIest::newRow("nobeginswith") << false << QString("[foo^=bar]") << QString("<p foo=\"blehbleh\" />") << QString();

    BOBUIest::newRow("endswith") << true << QString("[foo$=bar]") << QString("<p foo=\"barbar\" />") << QString();
    BOBUIest::newRow("noendswith") << false << QString("[foo$=bar]") << QString("<p foo=\"blehbleh\" />") << QString();

    BOBUIest::newRow("contains") << true << QString("[foo*=bar]") << QString("<p foo=\"blehbarbleh\" />") << QString();
    BOBUIest::newRow("nocontains") << false << QString("[foo*=bar]") << QString("<p foo=\"blehbleh\" />") << QString();

    BOBUIest::newRow("attr2") << true << QString("[bar=foo]") << QString("<p bleh=\"bar\" bar=\"foo\" />") << QString();

    BOBUIest::newRow("universal1") << true << QString("*") << QString("<p />") << QString();

    BOBUIest::newRow("universal3") << false << QString("*[foo=bar]") << QString("<p foo=\"bleh\" />") << QString();
    BOBUIest::newRow("universal4") << true << QString("*[foo=bar]") << QString("<p foo=\"bar\" />") << QString();

    BOBUIest::newRow("universal5") << false << QString("[foo=bar]") << QString("<p foo=\"bleh\" />") << QString();
    BOBUIest::newRow("universal6") << true << QString("[foo=bar]") << QString("<p foo=\"bar\" />") << QString();

    BOBUIest::newRow("universal7") << true << QString(".charfmt1") << QString("<p class=\"charfmt1\" />") << QString();

    BOBUIest::newRow("id") << true << QString("#blub") << QString("<p id=\"blub\" />") << QString();
    BOBUIest::newRow("noid") << false << QString("#blub") << QString("<p id=\"other\" />") << QString();

    BOBUIest::newRow("childselector") << true << QString("parent > child")
                                   << QString("<parent><child /></parent>")
                                   << QString("parent/child");

    BOBUIest::newRow("nochildselector2") << false << QString("parent > child")
                                   << QString("<child><parent /></child>")
                                   << QString("child/parent");

    BOBUIest::newRow("nochildselector3") << false << QString("parent > child")
                                   << QString("<parent><intermediate><child /></intermediate></parent>")
                                   << QString("parent/intermediate/child");

    BOBUIest::newRow("childselector2") << true << QString("parent[foo=bar] > child")
                                   << QString("<parent foo=\"bar\"><child /></parent>")
                                   << QString("parent/child");

    BOBUIest::newRow("nochildselector4") << false << QString("parent[foo=bar] > child")
                                   << QString("<parent><child /></parent>")
                                   << QString("parent/child");

    BOBUIest::newRow("nochildselector5") << false << QString("parent[foo=bar] > child")
                                   << QString("<parent foo=\"bar\"><parent><child /></parent></parent>")
                                   << QString("parent/parent/child");

    BOBUIest::newRow("childselectors") << true << QString("grandparent > parent > child")
                                   << QString("<grandparent><parent><child /></parent></grandparent>")
                                   << QString("grandparent/parent/child");

    BOBUIest::newRow("descendant") << true << QString("grandparent child")
                                   << QString("<grandparent><parent><child /></parent></grandparent>")
                                   << QString("grandparent/parent/child");

    BOBUIest::newRow("nodescendant") << false << QString("grandparent child")
                                   << QString("<other><parent><child /></parent></other>")
                                   << QString("other/parent/child");

    BOBUIest::newRow("descendant2") << true << QString("grandgrandparent grandparent child")
                                   << QString("<grandgrandparent><inbetween><grandparent><parent><child /></parent></grandparent></inbetween></grandgrandparent>")
                                   << QString("grandgrandparent/inbetween/grandparent/parent/child");

    BOBUIest::newRow("combined") << true << QString("grandparent parent > child")
                              << QString("<grandparent><inbetween><parent><child /></parent></inbetween></grandparent>")
                              << QString("grandparent/inbetween/parent/child");

    BOBUIest::newRow("combined2") << true << QString("grandparent > parent child")
                              << QString("<grandparent><parent><inbetween><child /></inbetween></parent></grandparent>")
                              << QString("grandparent/parent/inbetween/child");

    BOBUIest::newRow("combined3") << true << QString("grandparent > parent child")
                              << QString("<grandparent><parent><inbetween><child /></inbetween></parent></grandparent>")
                              << QString("grandparent/parent/inbetween/child");

    BOBUIest::newRow("nocombined") << false << QString("grandparent parent > child")
                              << QString("<inbetween><parent><child /></parent></inbetween>")
                              << QString("inbetween/parent/child");

    BOBUIest::newRow("nocombined2") << false << QString("grandparent parent > child")
                              << QString("<parent><child /></parent>")
                              << QString("parent/child");

    BOBUIest::newRow("previoussibling") << true << QString("p1 + p2")
                                     << QString("<p1 /><p2 />")
                                     << QString("p2");

    BOBUIest::newRow("notprevioussibling") << false << QString("p2 + p1")
                                     << QString("<p1 /><p2 />")
                                     << QString("p2");

    BOBUIest::newRow("anyprevioussibling") << true << QString("p1 ~ p3")
                                     << QString("<p1 /><p2 /><p3 />")
                                     << QString("p3");

    BOBUIest::newRow("noprevioussibling") << false << QString("p3 ~ p2")
                                     << QString("<p1 /><p2 /><p3 />")
                                     << QString("p3");


    BOBUIest::newRow("ancestry_firstmismatch") << false << QString("parent child[foo=bar]")
                                            << QString("<parent><child /></parent>")
                                            << QString("parent/child");

    BOBUIest::newRow("unknown-pseudo") << false << QString("p:enabled:foobar") << QString("<p/>") << QString();
}

void tst_QCssParser::styleSelector()
{
    QFETCH(bool, match);
    QFETCH(QString, selector);
    QFETCH(QString, xml);
    QFETCH(QString, elementToCheck);

    const QString css = selector + QLatin1String(" { background-color: green }");
    QCss::Parser parser(css);
    QCss::StyleSheet sheet;
    QVERIFY(parser.parse(&sheet));

    QDomDocument doc;
    xml.prepend("<!DOCTYPE test><test>");
    xml.append("</test>");
    QVERIFY(doc.setContent(xml));

    DomStyleSelector testSelector(doc, sheet);

    QDomElement e = doc.documentElement();
    if (elementToCheck.isEmpty()) {
        e = e.firstChildElement();
    } else {
        QStringList path = elementToCheck.split(QLatin1Char('/'));
        do {
            e = e.namedItem(path.takeFirst()).toElement();
        } while (!path.isEmpty());
    }
    QVERIFY(!e.isNull());
    QCss::StyleSelector::NodePtr n;
    n.ptr = &e;
    QList<QCss::Declaration> decls = testSelector.declarationsForNode(n);

    if (match) {
        QCOMPARE(decls.size(), 1);
        QCOMPARE(int(decls.at(0).d->propertyId), int(QCss::BackgroundColor));
        QCOMPARE(decls.at(0).d->values.size(), 1);
        QCOMPARE(int(decls.at(0).d->values.at(0).type), int(QCss::Value::Identifier));
        QCOMPARE(decls.at(0).d->values.at(0).variant.toString(), QString("green"));
    } else {
        QVERIFY(decls.isEmpty());
    }
}

void tst_QCssParser::specificity_data()
{
    BOBUIest::addColumn<QString>("selector");
    BOBUIest::addColumn<int>("specificity");

    BOBUIest::newRow("universal") << QString("*") << 0;

    BOBUIest::newRow("elements+pseudos1") << QString("foo") << 1;
    BOBUIest::newRow("elements+pseudos2") << QString("foo *[blah]") << 1 + (1 * 0x10);

    // should strictly speaking be '2', but we don't support pseudo-elements yet,
    // only pseudo-classes
    BOBUIest::newRow("elements+pseudos3") << QString("li:first-line") << 1 + (1 * 0x10);

    BOBUIest::newRow("elements+pseudos4") << QString("ul li") << 2;
    BOBUIest::newRow("elements+pseudos5") << QString("ul ol+li") << 3;
    BOBUIest::newRow("elements+pseudos6") << QString("h1 + *[rel=up]") << 1 + (1 * 0x10);

    BOBUIest::newRow("elements+pseudos7") << QString("ul ol li.red") << 3 + (1 * 0x10);
    BOBUIest::newRow("elements+pseudos8") << QString("li.red.level") << 1 + (2 * 0x10);
    BOBUIest::newRow("id") << QString("#x34y") << 1 * 0x100;
}

void tst_QCssParser::specificity()
{
    QFETCH(QString, selector);

    QString css = selector + QLatin1String(" { }");
    QCss::Parser parser(css);
    QCss::StyleSheet sheet;
    QVERIFY(parser.parse(&sheet));

    QCOMPARE(sheet.styleRules.size() + sheet.nameIndex.size() + sheet.idIndex.size() , 1);
    QCss::StyleRule rule =  (!sheet.styleRules.isEmpty()) ? sheet.styleRules.at(0)
                        :  (!sheet.nameIndex.isEmpty())  ? *sheet.nameIndex.begin()
                        :  *sheet.idIndex.begin();
    QCOMPARE(rule.selectors.size(), 1);
    BOBUIEST(rule.selectors.at(0).specificity(), "specificity");
}

void tst_QCssParser::specificitySort_data()
{
    BOBUIest::addColumn<QString>("firstSelector");
    BOBUIest::addColumn<QString>("secondSelector");
    BOBUIest::addColumn<QString>("xml");

    BOBUIest::newRow("universal1") << QString("*") << QString("p") << QString("<p />");
    BOBUIest::newRow("attr") << QString("p") << QString("p[foo=bar]") << QString("<p foo=\"bar\" />");
    BOBUIest::newRow("id") << QString("p") << QString("#hey") << QString("<p id=\"hey\" />");
    BOBUIest::newRow("id2") << QString("[id=hey]") << QString("#hey") << QString("<p id=\"hey\" />");
    BOBUIest::newRow("class") << QString("p") << QString(".hey") << QString("<p class=\"hey\" />");
}

void tst_QCssParser::specificitySort()
{
    QFETCH(QString, firstSelector);
    QFETCH(QString, secondSelector);
    QFETCH(QString, xml);

    firstSelector.append(" { color: green; }");
    secondSelector.append(" { color: red; }");

    QDomDocument doc;
    xml.prepend("<!DOCTYPE test><test>");
    xml.append("</test>");
    QVERIFY(doc.setContent(xml));

    for (int i = 0; i < 2; ++i) {
        QString css;
        if (i == 0)
            css = firstSelector + secondSelector;
        else
            css = secondSelector + firstSelector;

        QCss::Parser parser(css);
        QCss::StyleSheet sheet;
        QVERIFY(parser.parse(&sheet));

        DomStyleSelector testSelector(doc, sheet);

        QDomElement e = doc.documentElement().firstChildElement();
        QCss::StyleSelector::NodePtr n;
        n.ptr = &e;
        QList<QCss::Declaration> decls = testSelector.declarationsForNode(n);

        QCOMPARE(decls.size(), 2);

        QCOMPARE(int(decls.at(0).d->propertyId), int(QCss::Color));
        QCOMPARE(decls.at(0).d->values.size(), 1);
        QCOMPARE(int(decls.at(0).d->values.at(0).type), int(QCss::Value::Identifier));
        QCOMPARE(decls.at(0).d->values.at(0).variant.toString(), QString("green"));

        QCOMPARE(int(decls.at(1).d->propertyId), int(QCss::Color));
        QCOMPARE(decls.at(1).d->values.size(), 1);
        QCOMPARE(int(decls.at(1).d->values.at(0).type), int(QCss::Value::Identifier));
        QCOMPARE(decls.at(1).d->values.at(0).variant.toString(), QString("red"));
    }
}

void tst_QCssParser::rulesForNode_data()
{
    BOBUIest::addColumn<QString>("xml");
    BOBUIest::addColumn<QString>("css");
    BOBUIest::addColumn<quint64>("pseudoClass");
    BOBUIest::addColumn<int>("declCount");
    BOBUIest::addColumn<QString>("value0");
    BOBUIest::addColumn<QString>("value1");

    BOBUIest::newRow("universal1") << QString("<p/>") << QString("* { color: red }")
                                << (quint64)QCss::PseudoClass_Unspecified << 1 << "red" << "";

    BOBUIest::newRow("basic") << QString("<p/>") << QString("p:enabled { color: red; bg:blue; }")
        << (quint64)QCss::PseudoClass_Enabled << 2 << "red" << "blue";

    BOBUIest::newRow("single") << QString("<p/>")
        << QString("p:enabled { color: red; } *:hover { color: white }")
        << (quint64)QCss::PseudoClass_Hover << 1 << "white" << "";

    BOBUIest::newRow("multisel") << QString("<p/>")
        << QString("p:enabled { color: red; } p:hover { color: gray } *:hover { color: white } ")
        << (quint64)QCss::PseudoClass_Hover << 2 << "white" << "gray";

    BOBUIest::newRow("multisel2") << QString("<p/>")
        << QString("p:enabled { color: red; } p:hover:focus { color: gray } *:hover { color: white } ")
        << quint64(QCss::PseudoClass_Hover|QCss::PseudoClass_Focus) << 2 << "white" << "gray";

    BOBUIest::newRow("multisel3-diffspec") << QString("<p/>")
        << QString("p:enabled { color: red; } p:hover:focus { color: gray } *:hover { color: white } ")
        << quint64(QCss::PseudoClass_Hover) << 1 << "white" << "";

    BOBUIest::newRow("!-1") << QString("<p/>")
        << QString("p:checked:!hover { color: red; } p:checked:hover { color: gray } p:checked { color: white }")
        << quint64(QCss::PseudoClass_Hover|QCss::PseudoClass_Checked) << 2 << "white" << "gray";

    BOBUIest::newRow("!-2") << QString("<p/>")
        << QString("p:checked:!hover:!pressed { color: red; } p:!checked:hover { color: gray } p:!focus { color: blue }")
        << quint64(QCss::PseudoClass_Focus) << 0 << "" << "";

    BOBUIest::newRow("!-3") << QString("<p/>")
        << QString("p:checked:!hover:!pressed { color: red; } p:!checked:hover { color: gray } p:!focus { color: blue; }")
        << quint64(QCss::PseudoClass_Pressed) << 1 << "blue" << "";
}

void tst_QCssParser::rulesForNode()
{
    QFETCH(QString, xml);
    QFETCH(QString, css);
    QFETCH(quint64, pseudoClass);
    QFETCH(int, declCount);
    QFETCH(QString, value0);
    QFETCH(QString, value1);

    QDomDocument doc;
    xml.prepend("<!DOCTYPE test><test>");
    xml.append("</test>");
    QVERIFY(doc.setContent(xml));

    QCss::Parser parser(css);
    QCss::StyleSheet sheet;
    QVERIFY(parser.parse(&sheet));

    DomStyleSelector testSelector(doc, sheet);
    QDomElement e = doc.documentElement().firstChildElement();
    QCss::StyleSelector::NodePtr n;
    n.ptr = &e;
    QList<QCss::StyleRule> rules = testSelector.styleRulesForNode(n);

    QList<QCss::Declaration> decls;
    for (int i = 0; i < rules.size(); i++) {
        const QCss::Selector &selector = rules.at(i).selectors.at(0);
        quint64 negated = 0;
        quint64 cssClass = selector.pseudoClass(&negated);
        if ((cssClass == QCss::PseudoClass_Unspecified)
            || ((((cssClass & pseudoClass) == cssClass)) && ((negated & pseudoClass) == 0)))
            decls += rules.at(i).declarations;
    }

    QCOMPARE(decls.size(), declCount);

    if (declCount > 0)
        QCOMPARE(decls.at(0).d->values.at(0).variant.toString(), value0);
    if (declCount > 1)
        QCOMPARE(decls.at(1).d->values.at(0).variant.toString(), value1);
}

void tst_QCssParser::shorthandBackgroundProperty_data()
{
    BOBUIest::addColumn<QString>("css");
    BOBUIest::addColumn<QBrush>("expectedBrush");
    BOBUIest::addColumn<QString>("expectedImage");
    BOBUIest::addColumn<int>("expectedRepeatValue");
    BOBUIest::addColumn<int>("expectedAlignment");

    BOBUIest::newRow("simple color") << "background: red" << QBrush(QColor("red")) << QString() << int(QCss::Repeat_XY) << int(BobUI::AlignLeft | BobUI::AlignTop);
    BOBUIest::newRow("plain color") << "background-color: red" << QBrush(QColor("red")) << QString() << int(QCss::Repeat_XY) << int(BobUI::AlignLeft | BobUI::AlignTop);
    BOBUIest::newRow("palette color") << "background-color: palette(mid)" << qApp->palette().mid() << QString() << int(QCss::Repeat_XY) << int(BobUI::AlignLeft | BobUI::AlignTop);
    BOBUIest::newRow("multiple") << "background: url(chess.png) blue repeat-y" << QBrush(QColor("blue")) << QString("chess.png") << int(QCss::Repeat_Y) << int(BobUI::AlignLeft | BobUI::AlignTop);
    BOBUIest::newRow("plain alignment") << "background-position: center" << QBrush() << QString() << int(QCss::Repeat_XY) << int(BobUI::AlignCenter);
    BOBUIest::newRow("plain alignment2") << "background-position: left top" << QBrush() << QString() << int(QCss::Repeat_XY) << int(BobUI::AlignLeft | BobUI::AlignTop);
    BOBUIest::newRow("plain alignment3") << "background-position: left" << QBrush() << QString() << int(QCss::Repeat_XY) << int(BobUI::AlignLeft | BobUI::AlignVCenter);
    BOBUIest::newRow("multi") << "background: left url(blah.png) repeat-x" << QBrush() << QString("blah.png") << int(QCss::Repeat_X) << int(BobUI::AlignLeft | BobUI::AlignVCenter);
    BOBUIest::newRow("multi2") << "background: url(blah.png) repeat-x top" << QBrush() << QString("blah.png") << int(QCss::Repeat_X) << int(BobUI::AlignTop | BobUI::AlignHCenter);
    BOBUIest::newRow("multi3") << "background: url(blah.png) top right" << QBrush() << QString("blah.png") << int(QCss::Repeat_XY) << int(BobUI::AlignTop | BobUI::AlignRight);
    BOBUIest::newRow("url-query-quoted") << "background: url(\"https://placecats.com/300/200?fit=contain&position=top\")"
                                                                    << QBrush() << QString("https://placecats.com/300/200?fit=contain&position=top")
                                                                    << int(QCss::Repeat_XY) << int(BobUI::AlignTop | BobUI::AlignLeft);
    BOBUIest::newRow("url-query-unquoted") << "background: url(https://placecats.com/300/200?fit=contain&position=top)"
                                                                    << QBrush() << QString("https://placecats.com/300/200?fit=contain&position=top")
                                                                    << int(QCss::Repeat_XY) << int(BobUI::AlignTop | BobUI::AlignLeft);
}

void tst_QCssParser::shorthandBackgroundProperty()
{
    QFETCH(QString, css);

    QDomDocument doc;
    QVERIFY(doc.setContent(QLatin1String("<!DOCTYPE test><test> <dummy/> </test>")));

    css.prepend("dummy {");
    css.append(QLatin1Char('}'));

    QCss::Parser parser(css);
    QCss::StyleSheet sheet;
    QEXPECT_FAIL("url-query-unquoted", "BOBUIBUG-131842", Abort);
    QVERIFY(parser.parse(&sheet));

    DomStyleSelector testSelector(doc, sheet);
    QDomElement e = doc.documentElement().firstChildElement();
    QCss::StyleSelector::NodePtr n;
    n.ptr = &e;
    QList<QCss::StyleRule> rules = testSelector.styleRulesForNode(n);
    QList<QCss::Declaration> decls = rules.at(0).declarations;
    QCss::ValueExtractor v(decls);

    QBrush brush;
    QString image;
    QCss::Repeat repeat = QCss::Repeat_XY;
    BobUI::Alignment alignment = BobUI::AlignTop | BobUI::AlignLeft;
    QCss::Origin origin = QCss::Origin_Padding;
    QCss::Attachment attachment;
    QCss::Origin ignoredOrigin;
    v.extractBackground(&brush, &image, &repeat, &alignment, &origin, &attachment, &ignoredOrigin);

    QFETCH(QBrush, expectedBrush);
    QCOMPARE(expectedBrush.color(), brush.color());

    BOBUIEST(image, "expectedImage");
    BOBUIEST(int(repeat), "expectedRepeatValue");
    BOBUIEST(int(alignment), "expectedAlignment");

    //BOBUIBUG-9674  : a second evaluation should give the same results
    QVERIFY(v.extractBackground(&brush, &image, &repeat, &alignment, &origin, &attachment, &ignoredOrigin));
    QCOMPARE(expectedBrush.color(), brush.color());
    BOBUIEST(image, "expectedImage");
    BOBUIEST(int(repeat), "expectedRepeatValue");
    BOBUIEST(int(alignment), "expectedAlignment");
}

void tst_QCssParser::pseudoElement_data()
{
    BOBUIest::addColumn<QString>("css");
    BOBUIest::addColumn<QString>("pseudoElement");
    BOBUIest::addColumn<int>("declCount");

    // QComboBox::dropDown { border-image: blah; }
    BOBUIest::newRow("no pseudo-elements") << QString("dummy:hover { color: red }") << "" << 1;
    BOBUIest::newRow("no pseudo-elements") << QString("dummy:hover { color: red }") << "pe" << 0;

    BOBUIest::newRow("1 pseudo-element (1)") << QString("dummy::pe:hover { color: red }") << "pe" << 1;
    BOBUIest::newRow("1 pseudo-element (2)") << QString("dummy::pe:hover { color: red }") << "x" << 0;
    BOBUIest::newRow("1 pseudo-element (2)") << QString("whatever::pe:hover { color: red }") << "pe" << 0;

    BOBUIest::newRow("1 pseudo-element (3)")
        << QString("dummy { color: white; } dummy::pe:hover { color: red }") << "x" << 0;
    BOBUIest::newRow("1 pseudo-element (4)")
        << QString("dummy { color: white; } dummy::pe:hover { color: red } dummy { x:y }") << "" << 2;
    BOBUIest::newRow("1 pseudo-element (5)")
        << QString("dummy { color: white; } dummy::pe:hover { color: red }") << "pe" << 1;
    BOBUIest::newRow("1 pseudo-element (6)")
      << QString("dummy { color: white; } dummy::pe:hover { color: red } dummy::pe:checked { x: y} ") << "pe" << 2;

    BOBUIest::newRow("2 pseudo-elements (1)")
      << QString("dummy { color: white; } dummy::pe1:hover { color: red } dummy::pe2:checked { x: y} ")
      << "" << 1;
    BOBUIest::newRow("2 pseudo-elements (1)")
      << QString("dummy { color: white; } dummy::pe1:hover { color: red } dummy::pe2:checked { x: y} ")
      << "pe1" << 1;
    BOBUIest::newRow("2 pseudo-elements (2)")
      << QString("dummy { color: white; } dummy::pe1:hover { color: red } dummy::pe2:checked { x: y} ")
      << "pe2" << 1;
}

void tst_QCssParser::pseudoElement()
{
    QFETCH(QString, css);
    QFETCH(QString, pseudoElement);
    QFETCH(int, declCount);

    QDomDocument doc;
    QVERIFY(doc.setContent(QLatin1String("<!DOCTYPE test><test> <dummy/> </test>")));

    QCss::Parser parser(css);
    QCss::StyleSheet sheet;
    QVERIFY(parser.parse(&sheet));

    DomStyleSelector testSelector(doc, sheet);
    QDomElement e = doc.documentElement().firstChildElement();
    QCss::StyleSelector::NodePtr n;
    n.ptr = &e;
    QList<QCss::StyleRule> rules = testSelector.styleRulesForNode(n);
    QList<QCss::Declaration> decls;
    for (int i = 0; i < rules.size(); i++) {
        const QCss::Selector& selector = rules.at(i).selectors.at(0);
        if (pseudoElement.compare(selector.pseudoElement(), BobUI::CaseInsensitive) != 0)
            continue;
        decls += rules.at(i).declarations;

    }
    QCOMPARE(decls.size(), declCount);
}

void tst_QCssParser::gradient_data()
{
    BOBUIest::addColumn<QString>("css");
    BOBUIest::addColumn<QString>("type");
    BOBUIest::addColumn<QPointF>("start");
    BOBUIest::addColumn<QPointF>("finalStop");
    BOBUIest::addColumn<int>("spread");
    BOBUIest::addColumn<qreal>("stop0");
    BOBUIest::addColumn<QColor>("color0");
    BOBUIest::addColumn<qreal>("stop1");
    BOBUIest::addColumn<QColor>("color1");

    BOBUIest::newRow("color-string") <<
     "selection-background-color: qlineargradient(x1:1, y1:2, x2:3, y2:4, "
         "stop:0.2 red, stop:0.5 green)" << "linear" << QPointF(1, 2) << QPointF(3, 4)
                                  << 0 << qreal(0.2) << QColor("red") << qreal(0.5) << QColor("green");

    BOBUIest::newRow("color-#") <<
     "selection-background-color: qlineargradient(x1:0, y1:0, x2:0, y2:1, "
         "spread: reflect, stop:0.2 #123, stop:0.5 #456)" << "linear" << QPointF(0, 0) << QPointF(0, 1)
                             << 1 << qreal(0.2) << QColor("#123") << qreal(0.5) << QColor("#456");

    BOBUIest::newRow("color-rgb") <<
     "selection-background-color: qlineargradient(x1:0, y1:0, x2:0, y2:1, "
         "spread: reflect, stop:0.2 rgb(1, 2, 3), stop:0.5 rgba(1, 2, 3, 4))" << "linear" << QPointF(0, 0) << QPointF(0, 1)
                             << 1 << qreal(0.2) << QColor(1, 2, 3) << qreal(0.5) << QColor(1, 2, 3, 4);

    BOBUIest::newRow("color-spaces") <<
     "selection-background-color: qlineargradient(x1: 0, y1 :0,x2:0, y2 : 1 , "
         "spread: reflect, stop:0.2 rgb(1, 2, 3), stop: 0.5   rgba(1, 2, 3, 4))" << "linear" << QPointF(0, 0) << QPointF(0, 1)
                             << 1 << qreal(0.2) << QColor(1, 2, 3) << qreal(0.5) << QColor(1, 2, 3, 4);

    BOBUIest::newRow("conical gradient") <<
     "selection-background-color: qconicalgradient(cx: 4, cy : 2, angle: 23, "
         "spread: repeat, stop:0.2 rgb(1, 2, 3), stop:0.5 rgba(1, 2, 3, 4))" << "conical" << QPointF(4, 2) << QPointF()
                             << 2 << qreal(0.2) << QColor(1, 2, 3) << qreal(0.5) << QColor(1, 2, 3, 4);

    // spaces before first function parameter lead to parser errors
    BOBUIest::newRow("BOBUIBUG-61795") <<
     "selection-background-color: qconicalgradient( cx: 4, cy : 2, angle: 23, "
         "spread: repeat, stop:0.2 rgb( 1, 2, 3), stop:0.5 rgba( 1, 2, 3, 4))" << "conical" << QPointF(4, 2) << QPointF()
                             << 2 << qreal(0.2) << QColor(1, 2, 3) << qreal(0.5) << QColor(1, 2, 3, 4);

    /* won't pass: stop values are expected to be sorted
     BOBUIest::newRow("unsorted-stop") <<
     "selection-background: lineargradient(x1:0, y1:0, x2:0, y2:1, "
         "stop:0.5 green, stop:0.2 red)" << QPointF(0, 0) << QPointF(0, 1)
         0 << 0.2 << QColor("red") << 0.5 << QColor("green");
    */
}

void tst_QCssParser::gradient()
{
    QFETCH(QString, css);
    QFETCH(QString, type);
    QFETCH(QPointF, finalStop);
    QFETCH(QPointF, start);
    QFETCH(int, spread);
    QFETCH(qreal, stop0); QFETCH(QColor, color0);
    QFETCH(qreal, stop1); QFETCH(QColor, color1);

    QDomDocument doc;
    QVERIFY(doc.setContent(QLatin1String("<!DOCTYPE test><test> <dummy/> </test>")));

    css.prepend("dummy {");
    css.append(QLatin1Char('}'));

    QCss::Parser parser(css);
    QCss::StyleSheet sheet;
    QVERIFY(parser.parse(&sheet));

    DomStyleSelector testSelector(doc, sheet);
    QDomElement e = doc.documentElement().firstChildElement();
    QCss::StyleSelector::NodePtr n;
    n.ptr = &e;
    QList<QCss::StyleRule> rules = testSelector.styleRulesForNode(n);
    QList<QCss::Declaration> decls = rules.at(0).declarations;
    QCss::ValueExtractor ve(decls);
    QBrush foreground;
    QBrush selectedForeground;
    QBrush selectedBackground;
    QBrush alternateBackground;
    QBrush placeHolderTextForeground;
    QBrush accent;
    QVERIFY(ve.extractPalette(&foreground, &selectedForeground, &selectedBackground,
                              &alternateBackground, &placeHolderTextForeground, &accent));

    if (type == "linear") {
        QCOMPARE(selectedBackground.style(), BobUI::LinearGradientPattern);
        const auto *lg = static_cast<const QLinearGradient *>(selectedBackground.gradient());
        QCOMPARE(lg->start(), start);
        QCOMPARE(lg->finalStop(), finalStop);
    } else if (type == "conical") {
        QCOMPARE(selectedBackground.style(), BobUI::ConicalGradientPattern);
        const auto *cg = static_cast<const QConicalGradient *>(selectedBackground.gradient());
        QCOMPARE(cg->center(), start);
    }
    const QGradient *g = selectedBackground.gradient();
    QCOMPARE(g->spread(), QGradient::Spread(spread));
    QCOMPARE(g->stops().at(0).first, stop0);
    QCOMPARE(g->stops().at(0).second, color0);
    QCOMPARE(g->stops().at(1).first, stop1);
    QCOMPARE(g->stops().at(1).second, color1);
}

void tst_QCssParser::extractFontFamily_data()
{
    if (QFontInfo(QFont("Times New Roman")).family() != "Times New Roman")
        QSKIP("'Times New Roman' font not found");

    BOBUIest::addColumn<QString>("css");
    BOBUIest::addColumn<QString>("expectedFamily");

    BOBUIest::newRow("quoted-family-name") << "font-family: 'Times New Roman'" << QString("Times New Roman");
    BOBUIest::newRow("unquoted-family-name") << "font-family: Times New Roman" << QString("Times New Roman");
    BOBUIest::newRow("unquoted-family-name2") << "font-family: Times        New     Roman" << QString("Times New Roman");
    BOBUIest::newRow("multiple") << "font-family: Times New Roman  , foobar, 'baz'" << QString("Times New Roman");
    BOBUIest::newRow("multiple2") << "font-family: invalid,  Times New   Roman " << QString("Times New Roman");
    BOBUIest::newRow("invalid") << "font-family: invalid" << QFontInfo(QFont("invalid font")).family();
    BOBUIest::newRow("shorthand") << "font: 12pt Times New Roman" << QString("Times New Roman");
    BOBUIest::newRow("shorthand multiple quote") << "font: 12pt invalid, \"Times New Roman\" " << QString("Times New Roman");
    BOBUIest::newRow("shorthand multiple") << "font: 12pt invalid, Times New Roman " << QString("Times New Roman");
    BOBUIest::newRow("invalid spaces") << "font-family: invalid spaces, Times New Roman " << QString("Times New Roman");
    BOBUIest::newRow("invalid spaces quotes") << "font-family: 'invalid spaces', 'Times New Roman' " << QString("Times New Roman");
}


void tst_QCssParser::extractFontFamily()
{
    QFETCH(QString, css);
    css.prepend("dummy {");
    css.append(QLatin1Char('}'));

    QCss::Parser parser(css);
    QCss::StyleSheet sheet;
    QVERIFY(parser.parse(&sheet));

    QCOMPARE(sheet.styleRules.size() + sheet.nameIndex.size(), 1);
    QCss::StyleRule rule =  (!sheet.styleRules.isEmpty()) ?
            sheet.styleRules.at(0) : *sheet.nameIndex.begin();

    const QList<QCss::Declaration> decls = rule.declarations;
    QVERIFY(!decls.isEmpty());
    QCss::ValueExtractor extractor(decls);

    int adjustment = 0;
    QFont fnt;
    extractor.extractFont(&fnt, &adjustment);
    QFontInfo info(fnt);

    BOBUIEST(info.family(), "expectedFamily");
}

void tst_QCssParser::extractFontSize_data()
{
    BOBUIest::addColumn<QString>("css");
    BOBUIest::addColumn<int>("expectedPixelSize");
    BOBUIest::addColumn<int>("expectedPointSize");
    BOBUIest::addColumn<qreal>("expectedPointSizeF");

    BOBUIest::newRow("integer point size") << "font-size: 12pt" << -1 << 12 << 12.0;
    BOBUIest::newRow("float point size round down") << "font-size: 12.3pt" << -1 << 12 << 12.3;
    BOBUIest::newRow("float point size midpoint") << "font-size: 12.5pt" << -1 << 13 << 12.5;
    BOBUIest::newRow("float point size round up") << "font-size: 12.7pt" << -1 << 13 << 12.7;

    BOBUIest::newRow("integer pixel size") << "font-size: 12px" << 12 << -1 << -1.0;
    BOBUIest::newRow("float pixel size round down") << "font-size: 12.3px" << 12 << -1 << -1.0;
    BOBUIest::newRow("float pixel size midpoint") << "font-size: 12.5px" << 13 << -1 << -1.0;
    BOBUIest::newRow("float pixel size round up") << "font-size: 12.7px" << 13 << -1 << -1.0;

    BOBUIest::newRow("shorthand integer point size") << "font: 12pt Arial" << -1 << 12 << 12.0;
    BOBUIest::newRow("shorthand float point size round down") << "font: 12.3pt Arial" << -1 << 12 << 12.3;
    BOBUIest::newRow("shorthand float point size midpoint") << "font: 12.5pt Arial" << -1 << 13 << 12.5;
    BOBUIest::newRow("shorthand float point size round up") << "font: 12.7pt Arial" << -1 << 13 << 12.7;

    BOBUIest::newRow("shorthand integer pixel size") << "font: 12px Arial" << 12 << -1 << -1.0;
    BOBUIest::newRow("shorthand float pixel size round down") << "font: 12.3px Arial" << 12 << -1 << -1.0;
    BOBUIest::newRow("shorthand float pixel size midpoint") << "font: 12.5px Arial" << 13 << -1 << -1.0;
    BOBUIest::newRow("shorthand float pixel size round up") << "font: 12.7px Arial" << 13 << -1 << -1.0;
}

void tst_QCssParser::extractFontSize()
{
    QFETCH(QString, css);
    css.prepend("dummy {");
    css.append(QLatin1Char('}'));

    QCss::Parser parser(css);
    QCss::StyleSheet sheet;
    QVERIFY(parser.parse(&sheet));

    QCOMPARE(sheet.styleRules.size() + sheet.nameIndex.size(), 1);
    QCss::StyleRule rule = (!sheet.styleRules.isEmpty()) ?
            sheet.styleRules.at(0) : *sheet.nameIndex.begin();

    const QList<QCss::Declaration> decls = rule.declarations;
    QVERIFY(!decls.isEmpty());
    QCss::ValueExtractor extractor(decls);

    int adjustment = 0;
    QFont font;
    extractor.extractFont(&font, &adjustment);

    QFETCH(int, expectedPixelSize);
    QFETCH(int, expectedPointSize);
    QFETCH(qreal, expectedPointSizeF);

    QCOMPARE(font.pixelSize(), expectedPixelSize);
    QCOMPARE(font.pointSize(), expectedPointSize);
    QCOMPARE(font.pointSizeF(), expectedPointSizeF);
}

void tst_QCssParser::extractBorder_data()
{
    BOBUIest::addColumn<QString>("css");
    BOBUIest::addColumn<int>("expectedTopWidth");
    BOBUIest::addColumn<int>("expectedTopStyle");
    BOBUIest::addColumn<QColor>("expectedTopColor");

    BOBUIest::newRow("all values") << "border: 2px solid green" << 2 << (int)QCss::BorderStyle_Solid << QColor("green");
    BOBUIest::newRow("palette") << "border: 2px solid palette(highlight)" << 2 << (int)QCss::BorderStyle_Solid << qApp->palette().color(QPalette::Highlight);
    BOBUIest::newRow("just width") << "border: 2px" << 2 << (int)QCss::BorderStyle_None << QColor();
    BOBUIest::newRow("just style") << "border: solid" << 0 << (int)QCss::BorderStyle_Solid << QColor();
    BOBUIest::newRow("just color") << "border: green" << 0 << (int)QCss::BorderStyle_None << QColor("green");
    BOBUIest::newRow("width+style") << "border: 2px solid" << 2 << (int)QCss::BorderStyle_Solid << QColor();
    BOBUIest::newRow("style+color") << "border: solid green" << 0 << (int)QCss::BorderStyle_Solid << QColor("green");
    BOBUIest::newRow("width+color") << "border: 3px green" << 3 << (int)QCss::BorderStyle_None << QColor("green");
    BOBUIest::newRow("groove style") << "border: groove" << 0 << (int)QCss::BorderStyle_Groove << QColor();
    BOBUIest::newRow("ridge style") << "border: ridge" << 0 << (int)QCss::BorderStyle_Ridge << QColor();
    BOBUIest::newRow("double style") << "border: double" << 0 << (int)QCss::BorderStyle_Double << QColor();
    BOBUIest::newRow("inset style") << "border: inset" << 0 << (int)QCss::BorderStyle_Inset << QColor();
    BOBUIest::newRow("outset style") << "border: outset" << 0 << (int)QCss::BorderStyle_Outset << QColor();
    BOBUIest::newRow("dashed style") << "border: dashed" << 0 << (int)QCss::BorderStyle_Dashed << QColor();
    BOBUIest::newRow("dotted style") << "border: dotted" << 0 << (int)QCss::BorderStyle_Dotted << QColor();
    BOBUIest::newRow("dot-dash style") << "border: dot-dash" << 0 << (int)QCss::BorderStyle_DotDash << QColor();
    BOBUIest::newRow("dot-dot-dash style") << "border: dot-dot-dash" << 0 << (int)QCss::BorderStyle_DotDotDash << QColor();

    BOBUIest::newRow("top-width+color") << "border-top: 3px green" << 3 << (int)QCss::BorderStyle_None << QColor("green");
}

void tst_QCssParser::extractBorder()
{
    QFETCH(QString, css);
    QFETCH(int, expectedTopWidth);
    QFETCH(int, expectedTopStyle);
    QFETCH(QColor, expectedTopColor);

    css.prepend("dummy {");
    css.append(QLatin1Char('}'));

    QCss::Parser parser(css);
    QCss::StyleSheet sheet;
    QVERIFY(parser.parse(&sheet));

    QCOMPARE(sheet.styleRules.size() + sheet.nameIndex.size(), 1);
    QCss::StyleRule rule =  (!sheet.styleRules.isEmpty()) ?
            sheet.styleRules.at(0) : *sheet.nameIndex.begin();
    const QList<QCss::Declaration> decls = rule.declarations;
    QVERIFY(!decls.isEmpty());
    QCss::ValueExtractor extractor(decls);

    int widths[4];
    QBrush colors[4];
    QCss::BorderStyle styles[4];
    QSize radii[4];

    extractor.extractBorder(widths, colors, styles, radii);
    QCOMPARE(widths[QCss::TopEdge], expectedTopWidth);
    QCOMPARE(int(styles[QCss::TopEdge]), expectedTopStyle);
    QCOMPARE(colors[QCss::TopEdge].color(), expectedTopColor);

    //BOBUIBUG-9674  : a second evaluation should give the same results
    QVERIFY(extractor.extractBorder(widths, colors, styles, radii));
    QCOMPARE(widths[QCss::TopEdge], expectedTopWidth);
    QCOMPARE(int(styles[QCss::TopEdge]), expectedTopStyle);
    QCOMPARE(colors[QCss::TopEdge].color(), expectedTopColor);
}

void tst_QCssParser::extractBorderImage_data()
{
    BOBUIest::addColumn<QString>("css");
    BOBUIest::addColumn<QString>("imgUrl");
    BOBUIest::addColumn<QCss::TileMode>("tileMode1");
    BOBUIest::addColumn<QCss::TileMode>("tileMode2");

    BOBUIest::newRow("no valid url, 1 stretch")
        << "border-image: stretch" << QString()
        << QCss::TileMode::TileMode_Stretch
        << QCss::TileMode::TileMode_Stretch;
    BOBUIest::newRow("tilemode stretch")
        << "border-image: url(:/image.png) 1 stretch"  << ":/image.png"
        << QCss::TileMode::TileMode_Stretch
        << QCss::TileMode::TileMode_Stretch;
    BOBUIest::newRow("tilemode repeat")
        << "border-image: url(:/image.png) 1 2 repeat" << ":/image.png"
        << QCss::TileMode::TileMode_Repeat
        << QCss::TileMode::TileMode_Repeat;
    BOBUIest::newRow("tilemode repeat and stretch")
        << "border-image: url(:/image.png) 1 2 3 repeat stretch" << ":/image.png"
        << QCss::TileMode::TileMode_Repeat
        << QCss::TileMode::TileMode_Stretch;
}

void tst_QCssParser::extractBorderImage()
{
    QFETCH(QString, css);
    QFETCH(QString, imgUrl);
    QFETCH(QCss::TileMode, tileMode1);
    QFETCH(QCss::TileMode, tileMode2);

    css.prepend("dummy {");
    css.append(QLatin1Char('}'));

    QCss::Parser parser(css);
    QCss::StyleSheet sheet;
    QVERIFY(parser.parse(&sheet));

    QCOMPARE(sheet.styleRules.size() + sheet.nameIndex.size(), 1);
    QCss::StyleRule rule =  (!sheet.styleRules.isEmpty()) ?
            sheet.styleRules.at(0) : *sheet.nameIndex.begin();
    const QList<QCss::Declaration> decls = rule.declarations;
    QVERIFY(!decls.isEmpty());

    QString uri;
    QCss::TileMode horizStretch, vertStretch;
    int cuts[4];
    for (const auto& decl : decls) {
        if (decl.d->propertyId == QCss::BorderImage) {
            decl.borderImageValue(&uri, cuts, &horizStretch, &vertStretch);
            QCOMPARE(uri, imgUrl);
            QCOMPARE(horizStretch, tileMode1);
            QCOMPARE(vertStretch, tileMode2);
        }
    }
}
void tst_QCssParser::extractBorderImageCuts_data()
{
    BOBUIest::addColumn<QString>("css");
    BOBUIest::addColumn<int>("expCut1");
    BOBUIest::addColumn<int>("expCut2");
    BOBUIest::addColumn<int>("expCut3");
    BOBUIest::addColumn<int>("expCut4");

    const QString url = "border-image: url(:/image.png)";
    BOBUIest::newRow("no cuts") << url << -1 << -1 << -1 << -1;
    BOBUIest::newRow("1 cut, valid") << url + " 2" << 2 << 2 << 2 << 2;
    BOBUIest::newRow("1 cut, invalid") << url + " -42" << -1 << -1 << -1 << -1;
    BOBUIest::newRow("2 cuts, valid") << url + " 2 3" << 2 << 3 << 2 << 3;
    BOBUIest::newRow("2 cuts, invalid") << url + " 2 -3" << -1 << -1 << -1 << -1;
    BOBUIest::newRow("3 cuts, valid") << url + " 2 3 4" << 2 << 3 << 4 << 3;
    BOBUIest::newRow("3 cuts, invalid") << url + " 2 3 -4" << -1 << -1 << -1 << -1;
    BOBUIest::newRow("4 cuts, valid") << url + " 2 3 4 5" << 2 << 3 << 4 << 5;
    BOBUIest::newRow("4 cuts, invalid") << url + " 2 3 4 -5" << -1 << -1 << -1 << -1;
}

void tst_QCssParser::extractBorderImageCuts()
{
    QFETCH(QString, css);
    QFETCH(int, expCut1);
    QFETCH(int, expCut2);
    QFETCH(int, expCut3);
    QFETCH(int, expCut4);

    css.prepend("dummy {");
    css.append(QLatin1Char('}'));

    QCss::Parser parser(css);
    QCss::StyleSheet sheet;
    QVERIFY(parser.parse(&sheet));

    QCOMPARE(sheet.styleRules.size() + sheet.nameIndex.size(), 1);
    QCss::StyleRule rule =  (!sheet.styleRules.isEmpty()) ?
            sheet.styleRules.at(0) : *sheet.nameIndex.begin();
    const QList<QCss::Declaration> decls = rule.declarations;
    QVERIFY(!decls.isEmpty());

    QString uri;
    QCss::TileMode horizStretch, vertStretch;
    int cuts[4];
    for (const auto& decl : decls) {
        if (decl.d->propertyId == QCss::BorderImage) {
            decl.borderImageValue(&uri, cuts, &horizStretch, &vertStretch);
            QCOMPARE(cuts[0], expCut1);
            QCOMPARE(cuts[1], expCut2);
            QCOMPARE(cuts[2], expCut3);
            QCOMPARE(cuts[3], expCut4);
        }
    }
}

void tst_QCssParser::noTextDecoration()
{
    QCss::Parser parser("dummy { text-decoration: none; }");
    QCss::StyleSheet sheet;
    QVERIFY(parser.parse(&sheet));

    QCOMPARE(sheet.styleRules.size() + sheet.nameIndex.size(), 1);
    QCss::StyleRule rule =  (!sheet.styleRules.isEmpty()) ?
            sheet.styleRules.at(0) : *sheet.nameIndex.begin();
    const QList<QCss::Declaration> decls = rule.declarations;
    QVERIFY(!decls.isEmpty());
    QCss::ValueExtractor extractor(decls);

    int adjustment = 0;
    QFont f;
    f.setUnderline(true);
    f.setOverline(true);
    f.setStrikeOut(true);
    QVERIFY(extractor.extractFont(&f, &adjustment));

    QVERIFY(!f.underline());
    QVERIFY(!f.overline());
    QVERIFY(!f.strikeOut());
}

void tst_QCssParser::quotedAndUnquotedIdentifiers()
{
    QCss::Parser parser("foo { font-style: \"italic\"; font-weight: bold }");
    QCss::StyleSheet sheet;
    QVERIFY(parser.parse(&sheet));

    QCOMPARE(sheet.styleRules.size() + sheet.nameIndex.size(), 1);
    QCss::StyleRule rule = (!sheet.styleRules.isEmpty()) ?
           sheet.styleRules.at(0) : *sheet.nameIndex.begin();
    const QList<QCss::Declaration> decls = rule.declarations;
    QCOMPARE(decls.size(), 2);

    QCOMPARE(decls.at(0).d->values.first().type, QCss::Value::String);
    QCOMPARE(decls.at(0).d->property, QLatin1String("font-style"));
    QCOMPARE(decls.at(0).d->values.first().toString(), QLatin1String("italic"));

    QCOMPARE(decls.at(1).d->values.first().type, QCss::Value::KnownIdentifier);
    QCOMPARE(decls.at(1).d->property, QLatin1String("font-weight"));
    QCOMPARE(decls.at(1).d->values.first().toString(), QLatin1String("bold"));
}

void tst_QCssParser::whitespaceValues_data()
{
    BOBUIest::addColumn<QString>("value");

    BOBUIest::newRow("normal") << "normal";
    BOBUIest::newRow("inherit") << "inherit";
    BOBUIest::newRow("nowrap") << "nowrap";
    BOBUIest::newRow("pre") << "pre";
    BOBUIest::newRow("pre-wrap") << "pre-wrap";
    BOBUIest::newRow("pre-line") << "pre-line";
}

void tst_QCssParser::whitespaceValues()
{
    QFETCH(QString, value);
    QCss::Parser parser(QString("foo { white-space: %1 }").arg(value));
    QCss::StyleSheet sheet;
    QVERIFY(parser.parse(&sheet));

    QCss::StyleRule rule = (!sheet.styleRules.isEmpty()) ?
           sheet.styleRules.at(0) : *sheet.nameIndex.begin();
    QCOMPARE(rule.declarations.size(), 1);

    QCOMPARE(rule.declarations.at(0).d->property, QLatin1String("white-space"));
    QCOMPARE(rule.declarations.at(0).d->values.first().toString(), value);
}

void tst_QCssParser::strokeLineCapValues_data()
{
    BOBUIest::addColumn<QString>("value");

    BOBUIest::newRow("flatcap") << "flatcap";
    BOBUIest::newRow("roundcap") << "roundcap";
    BOBUIest::newRow("squarecap") << "squarecap";
}

void tst_QCssParser::strokeLineCapValues()
{
    QFETCH(QString, value);
    QCss::Parser parser(QString("foo { -bobui-stroke-linecap: %1 }").arg(value));
    QCss::StyleSheet sheet;
    QVERIFY(parser.parse(&sheet));

    QCss::StyleRule rule = (!sheet.styleRules.isEmpty()) ?
            sheet.styleRules.at(0) : *sheet.nameIndex.begin();
    QCOMPARE(rule.declarations.size(), 1);

    QCOMPARE(rule.declarations.at(0).d->property, QLatin1String("-bobui-stroke-linecap"));
    QCOMPARE(rule.declarations.at(0).d->values.first().type, QCss::Value::KnownIdentifier);
    QCOMPARE(rule.declarations.at(0).d->values.first().toString(), value);
}

void tst_QCssParser::strokeLineJoinValues_data()
{
    BOBUIest::addColumn<QString>("value");

    BOBUIest::newRow("beveljoin") << "beveljoin";
    BOBUIest::newRow("miterjoin") << "miterjoin";
    BOBUIest::newRow("roundjoin") << "roundjoin";
    BOBUIest::newRow("svgmiterjoin") << "svgmiterjoin";
}

void tst_QCssParser::strokeLineJoinValues()
{
    QFETCH(QString, value);
    QCss::Parser parser(QString("foo { -bobui-stroke-linejoin: %1 }").arg(value));
    QCss::StyleSheet sheet;
    QVERIFY(parser.parse(&sheet));

    QCss::StyleRule rule = (!sheet.styleRules.isEmpty()) ?
            sheet.styleRules.at(0) : *sheet.nameIndex.begin();
    QCOMPARE(rule.declarations.size(), 1);

    QCOMPARE(rule.declarations.at(0).d->property, QLatin1String("-bobui-stroke-linejoin"));
    QCOMPARE(rule.declarations.at(0).d->values.first().type, QCss::Value::KnownIdentifier);
    QCOMPARE(rule.declarations.at(0).d->values.first().toString(), value);
}

void tst_QCssParser::borderColor_data()
{
    BOBUIest::addColumn<QString>("css");
    BOBUIest::addColumn<QColor>("expectedTopColor");
    BOBUIest::addColumn<QColor>("expectedRightColor");
    BOBUIest::addColumn<QColor>("expectedBottomColor");
    BOBUIest::addColumn<QColor>("expectedLeftColor");

    BOBUIest::newRow("four values") << "border-color: red green blue white" << QColor("red") << QColor("green") << QColor("blue") << QColor("white");
    BOBUIest::newRow("three values") << "border-color: red green blue" << QColor("red") << QColor("green") << QColor("blue") << QColor("green");
    BOBUIest::newRow("two values") << "border-color: red green" << QColor("red") << QColor("green") << QColor("red") << QColor("green");
    BOBUIest::newRow("one value") << "border-color: red" << QColor("red") << QColor("red") << QColor("red") << QColor("red");
}

void tst_QCssParser::borderColor()
{
    QFETCH(QString, css);
    QFETCH(QColor, expectedTopColor);
    QFETCH(QColor, expectedRightColor);
    QFETCH(QColor, expectedBottomColor);
    QFETCH(QColor, expectedLeftColor);

    css.prepend("dummy {");
    css.append(QLatin1Char('}'));

    QCss::Parser parser(css);
    QCss::StyleSheet sheet;
    QVERIFY(parser.parse(&sheet));

    QCOMPARE(sheet.styleRules.size() + sheet.nameIndex.size(), 1);
    QCss::StyleRule rule =  (!sheet.styleRules.isEmpty()) ?
            sheet.styleRules.at(0) : *sheet.nameIndex.begin();
    const QList<QCss::Declaration> decls = rule.declarations;
    QVERIFY(decls.size() == 1);
    QVERIFY(decls[0].d->propertyId == QCss::BorderColor);

    QBrush colors[4];

    decls[0].brushValues(colors);
    QCOMPARE(colors[QCss::TopEdge].color(), expectedTopColor);
    QCOMPARE(colors[QCss::RightEdge].color(), expectedRightColor);
    QCOMPARE(colors[QCss::BottomEdge].color(), expectedBottomColor);
    QCOMPARE(colors[QCss::LeftEdge].color(), expectedLeftColor);

    //BOBUIBUG-126381 : a second evaluation should give the same results
    QCOMPARE(colors[QCss::TopEdge].color(), expectedTopColor);
    QCOMPARE(colors[QCss::RightEdge].color(), expectedRightColor);
    QCOMPARE(colors[QCss::BottomEdge].color(), expectedBottomColor);
    QCOMPARE(colors[QCss::LeftEdge].color(), expectedLeftColor);
}

BOBUIEST_MAIN(tst_QCssParser)
#include "tst_qcssparser.moc"

