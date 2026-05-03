// Copyright (C) 2016 The BobUI Company Ltd.
// Copyright (C) 2016 Intel Corporation.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

#include <BobUICore/QUrl>
#include <BOBUIest>

#include "private/qurl_p.h"

// For testsuites
#define IDNA_ACE_PREFIX "xn--"
#define IDNA_SUCCESS 1
#define STRINGPREP_NO_UNASSIGNED 1
#define STRINGPREP_CONTAINS_UNASSIGNED 2
#define STRINGPREP_CONTAINS_PROHIBITED 3
#define STRINGPREP_BIDI_BOTH_L_AND_RAL 4
#define STRINGPREP_BIDI_LEADTRAIL_NOT_RAL 5

using namespace BobUI::StringLiterals;

struct char16array {
    char16array() {}
    template <size_t N>
    char16array(char16_t (&array)[N])
    {
        memcpy(points, array, N*sizeof(char16_t));
    }

    char16_t points[100];
};

Q_DECLARE_METATYPE(char16array)
Q_DECLARE_METATYPE(QUrl::FormattingOptions)
Q_DECLARE_METATYPE(QUrl::ComponentFormattingOptions)

class tst_QUrlInternal : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    // IDNA internals
#ifdef BOBUI_BUILD_INTERNAL
    void idna_testsuite_data();
    void idna_testsuite();
#endif
    void ace_testsuite_data();
    void ace_testsuite();
    void std3violations_data();
    void std3violations();
    void std3deviations_data();
    void std3deviations();

    // percent-encoding internals
    void correctEncodedMistakes_data();
    void correctEncodedMistakes();
    void encodingRecode_data();
    void encodingRecode();
    void encodingRecodeInvalidUtf8_data();
    void encodingRecodeInvalidUtf8();

    void tldRestrictions_data();
    void tldRestrictions();
};
#include "tst_qurlinternal.moc"

#ifdef BOBUI_BUILD_INTERNAL
void tst_QUrlInternal::idna_testsuite_data()
{
    BOBUIest::addColumn<int>("numchars");
    BOBUIest::addColumn<char16array>("unicode");
    BOBUIest::addColumn<QByteArray>("punycode");
    BOBUIest::addColumn<int>("allowunassigned");
    BOBUIest::addColumn<int>("usestd3asciirules");
    BOBUIest::addColumn<int>("toasciirc");
    BOBUIest::addColumn<int>("tounicoderc");

    char16_t d1[] = { 0x0644, 0x064A, 0x0647, 0x0645, 0x0627, 0x0628, 0x062A, 0x0643,
                      0x0644, 0x0645, 0x0648, 0x0634, 0x0639, 0x0631, 0x0628, 0x064A,
                      0x061F };
    BOBUIest::newRow("Arabic (Egyptian)") << 17 << char16array(d1)
                                    << QByteArray(IDNA_ACE_PREFIX "egbpdaj6bu4bxfgehfvwxn")
                                    << 0 << 0 << IDNA_SUCCESS << IDNA_SUCCESS;

    char16_t d2[] = { 0x4ED6, 0x4EEC, 0x4E3A, 0x4EC0, 0x4E48, 0x4E0D, 0x8BF4, 0x4E2D,
                      0x6587 };
    BOBUIest::newRow("Chinese (simplified)") << 9 << char16array(d2)
                                       << QByteArray(IDNA_ACE_PREFIX "ihqwcrb4cv8a8dqg056pqjye")
                                       << 0 << 0 << IDNA_SUCCESS << IDNA_SUCCESS;

    char16_t d3[] = { 0x4ED6, 0x5011, 0x7232, 0x4EC0, 0x9EBD, 0x4E0D, 0x8AAA, 0x4E2D,
                      0x6587 };
    BOBUIest::newRow("Chinese (traditional)") << 9 << char16array(d3)
                                        << QByteArray(IDNA_ACE_PREFIX "ihqwctvzc91f659drss3x8bo0yb")
                                        << 0 << 0 << IDNA_SUCCESS << IDNA_SUCCESS;

    char16_t d4[] = { 0x0050, 0x0072, 0x006F, 0x010D, 0x0070, 0x0072, 0x006F, 0x0073,
                      0x0074, 0x011B, 0x006E, 0x0065, 0x006D, 0x006C, 0x0075, 0x0076,
                      0x00ED, 0x010D, 0x0065, 0x0073, 0x006B, 0x0079 };
    BOBUIest::newRow("Czech") << 22 << char16array(d4)
                        << QByteArray(IDNA_ACE_PREFIX "Proprostnemluvesky-uyb24dma41a")
                        << 0 << 0 << IDNA_SUCCESS << IDNA_SUCCESS;

    char16_t d5[] = { 0x05DC, 0x05DE, 0x05D4, 0x05D4, 0x05DD, 0x05E4, 0x05E9, 0x05D5,
                      0x05D8, 0x05DC, 0x05D0, 0x05DE, 0x05D3, 0x05D1, 0x05E8, 0x05D9,
                      0x05DD, 0x05E2, 0x05D1, 0x05E8, 0x05D9, 0x05EA };
    BOBUIest::newRow("Hebrew") << 22 << char16array(d5)
                         << QByteArray(IDNA_ACE_PREFIX "4dbcagdahymbxekheh6e0a7fei0b")
                         << 0 << 0 << IDNA_SUCCESS << IDNA_SUCCESS;

    char16_t d6[] = { 0x092F, 0x0939, 0x0932, 0x094B, 0x0917, 0x0939, 0x093F, 0x0928,
                      0x094D, 0x0926, 0x0940, 0x0915, 0x094D, 0x092F, 0x094B, 0x0902,
                      0x0928, 0x0939, 0x0940, 0x0902, 0x092C, 0x094B, 0x0932, 0x0938,
                      0x0915, 0x0924, 0x0947, 0x0939, 0x0948, 0x0902 };
    BOBUIest::newRow("Hindi (Devanagari)") << 30 << char16array(d6)
                                     << QByteArray(IDNA_ACE_PREFIX "i1baa7eci9glrd9b2ae1bj0hfcgg6iyaf8o0a1dig0cd")
                                     << 0 << 0 << IDNA_SUCCESS;

    char16_t d7[] = { 0x306A, 0x305C, 0x307F, 0x3093, 0x306A, 0x65E5, 0x672C, 0x8A9E,
                      0x3092, 0x8A71, 0x3057, 0x3066, 0x304F, 0x308C, 0x306A, 0x3044,
                      0x306E, 0x304B };
    BOBUIest::newRow("Japanese (kanji and hiragana)") << 18 << char16array(d7)
                                                << QByteArray(IDNA_ACE_PREFIX "n8jok5ay5dzabd5bym9f0cm5685rrjetr6pdxa")
                                                << 0 << 0 << IDNA_SUCCESS;

    char16_t d8[] = { 0x043F, 0x043E, 0x0447, 0x0435, 0x043C, 0x0443, 0x0436, 0x0435,
                      0x043E, 0x043D, 0x0438, 0x043D, 0x0435, 0x0433, 0x043E, 0x0432,
                      0x043E, 0x0440, 0x044F, 0x0442, 0x043F, 0x043E, 0x0440, 0x0443,
                      0x0441, 0x0441, 0x043A, 0x0438 };
    BOBUIest::newRow("Russian (Cyrillic)") << 28 << char16array(d8)
                                     << QByteArray(IDNA_ACE_PREFIX "b1abfaaepdrnnbgefbadotcwatmq2g4l")
                                     << 0 << 0 << IDNA_SUCCESS << IDNA_SUCCESS;

    char16_t d9[] = { 0x0050, 0x006F, 0x0072, 0x0071, 0x0075, 0x00E9, 0x006E, 0x006F,
                      0x0070, 0x0075, 0x0065, 0x0064, 0x0065, 0x006E, 0x0073, 0x0069,
                      0x006D, 0x0070, 0x006C, 0x0065, 0x006D, 0x0065, 0x006E, 0x0074,
                      0x0065, 0x0068, 0x0061, 0x0062, 0x006C, 0x0061, 0x0072, 0x0065,
                      0x006E, 0x0045, 0x0073, 0x0070, 0x0061, 0x00F1, 0x006F, 0x006C };
    BOBUIest::newRow("Spanish") << 40 << char16array(d9)
                          << QByteArray(IDNA_ACE_PREFIX "PorqunopuedensimplementehablarenEspaol-fmd56a")
                          << 0 << 0 << IDNA_SUCCESS;

    char16_t d10[] = { 0x0054, 0x1EA1, 0x0069, 0x0073, 0x0061, 0x006F, 0x0068, 0x1ECD,
                       0x006B, 0x0068, 0x00F4, 0x006E, 0x0067, 0x0074, 0x0068, 0x1EC3,
                       0x0063, 0x0068, 0x1EC9, 0x006E, 0x00F3, 0x0069, 0x0074, 0x0069,
                       0x1EBF, 0x006E, 0x0067, 0x0056, 0x0069, 0x1EC7, 0x0074 };
    BOBUIest::newRow("Vietnamese") << 31 << char16array(d10)
                             << QByteArray(IDNA_ACE_PREFIX "TisaohkhngthchnitingVit-kjcr8268qyxafd2f1b9g")
                             << 0 << 0 << IDNA_SUCCESS;

    char16_t d11[] = { 0x0033, 0x5E74, 0x0042, 0x7D44, 0x91D1, 0x516B, 0x5148, 0x751F };
    BOBUIest::newRow("Japanese") << 8 << char16array(d11)
                           << QByteArray(IDNA_ACE_PREFIX "3B-ww4c5e180e575a65lsy2b")
                           << 0 << 0 << IDNA_SUCCESS << IDNA_SUCCESS;

    // this test does NOT include nameprepping, so the capitals will remain
    char16_t d12[] = { 0x5B89, 0x5BA4, 0x5948, 0x7F8E, 0x6075, 0x002D, 0x0077, 0x0069,
                       0x0074, 0x0068, 0x002D, 0x0053, 0x0055, 0x0050, 0x0045, 0x0052,
                       0x002D, 0x004D, 0x004F, 0x004E, 0x004B, 0x0045, 0x0059, 0x0053 };
    BOBUIest::newRow("Japanese2") << 24 << char16array(d12)
                            << QByteArray(IDNA_ACE_PREFIX "-with-SUPER-MONKEYS-pc58ag80a8qai00g7n9n")
                            << 0 << 0 << IDNA_SUCCESS;

    char16_t d13[] = { 0x0048, 0x0065, 0x006C, 0x006C, 0x006F, 0x002D, 0x0041, 0x006E,
                       0x006F, 0x0074, 0x0068, 0x0065, 0x0072, 0x002D, 0x0057, 0x0061,
                       0x0079, 0x002D, 0x305D, 0x308C, 0x305E, 0x308C, 0x306E, 0x5834,
                       0x6240 };
    BOBUIest::newRow("Japanese3") << 25 << char16array(d13)
                            << QByteArray(IDNA_ACE_PREFIX "Hello-Another-Way--fc4qua05auwb3674vfr0b")
                            << 0 << 0 << IDNA_SUCCESS;

    char16_t d14[] = { 0x3072, 0x3068, 0x3064, 0x5C4B, 0x6839, 0x306E, 0x4E0B, 0x0032 };
    BOBUIest::newRow("Japanese4") << 8 << char16array(d14)
                            << QByteArray(IDNA_ACE_PREFIX "2-u9tlzr9756bt3uc0v")
                            << 0 << 0 << IDNA_SUCCESS << IDNA_SUCCESS;

    char16_t d15[] = { 0x004D, 0x0061, 0x006A, 0x0069, 0x3067, 0x004B, 0x006F, 0x0069,
                       0x3059, 0x308B, 0x0035, 0x79D2, 0x524D };
    BOBUIest::newRow("Japanese5") << 13 << char16array(d15)
                            << QByteArray(IDNA_ACE_PREFIX "MajiKoi5-783gue6qz075azm5e")
                            << 0 << 0 << IDNA_SUCCESS << IDNA_SUCCESS;

    char16_t d16[] = { 0x30D1, 0x30D5, 0x30A3, 0x30FC, 0x0064, 0x0065, 0x30EB, 0x30F3, 0x30D0 };
    BOBUIest::newRow("Japanese6") << 9 << char16array(d16)
                            << QByteArray(IDNA_ACE_PREFIX "de-jg4avhby1noc0d")
                            << 0 << 0 << IDNA_SUCCESS << IDNA_SUCCESS;

    char16_t d17[] = { 0x305D, 0x306E, 0x30B9, 0x30D4, 0x30FC, 0x30C9, 0x3067 };
    BOBUIest::newRow("Japanese7") << 7 << char16array(d17)
                            << QByteArray(IDNA_ACE_PREFIX "d9juau41awczczp")
                            << 0 << 0 << IDNA_SUCCESS << IDNA_SUCCESS;

    char16_t d18[] = { 0x03b5, 0x03bb, 0x03bb, 0x03b7, 0x03bd, 0x03b9, 0x03ba, 0x03ac };
    BOBUIest::newRow("Greek") << 8 << char16array(d18)
                        << QByteArray(IDNA_ACE_PREFIX "hxargifdar")
                        << 0 << 0 << IDNA_SUCCESS << IDNA_SUCCESS;

    char16_t d19[] = { 0x0062, 0x006f, 0x006e, 0x0121, 0x0075, 0x0073, 0x0061, 0x0127,
                       0x0127, 0x0061 };
    BOBUIest::newRow("Maltese (Malti)") << 10 << char16array(d19)
                                  << QByteArray(IDNA_ACE_PREFIX "bonusaa-5bb1da")
                                  << 0 << 0 << IDNA_SUCCESS << IDNA_SUCCESS;

    char16_t d20[] = {0x043f, 0x043e, 0x0447, 0x0435, 0x043c, 0x0443, 0x0436, 0x0435,
                      0x043e, 0x043d, 0x0438, 0x043d, 0x0435, 0x0433, 0x043e, 0x0432,
                      0x043e, 0x0440, 0x044f, 0x0442, 0x043f, 0x043e, 0x0440, 0x0443,
                      0x0441, 0x0441, 0x043a, 0x0438 };
    BOBUIest::newRow("Russian (Cyrillic)") << 28 << char16array(d20)
                                     << QByteArray(IDNA_ACE_PREFIX "b1abfaaepdrnnbgefbadotcwatmq2g4l")
                                     << 0 << 0 << IDNA_SUCCESS << IDNA_SUCCESS;

    char16_t d21[] = { 0xd800, 0xdef7 };
    BOBUIest::newRow("U+102F7") << 2 << char16array(d21) << QByteArray(IDNA_ACE_PREFIX "r97c");
}
#endif

#ifdef BOBUI_BUILD_INTERNAL
void tst_QUrlInternal::idna_testsuite()
{
    QFETCH(int, numchars);
    QFETCH(char16array, unicode);
    QFETCH(QByteArray, punycode);

    QString result;
    bobui_punycodeEncoder(QStringView{unicode.points, numchars}, &result);
    QCOMPARE(result.toLatin1(), punycode);
    QCOMPARE(bobui_punycodeDecoder(result), QString::fromUtf16(unicode.points, numchars));
}
#endif

void tst_QUrlInternal::ace_testsuite_data()
{
    BOBUIest::addColumn<QString>("in");
    BOBUIest::addColumn<QString>("toace");
    BOBUIest::addColumn<QString>("fromace");
    BOBUIest::addColumn<QString>("unicode");

    BOBUIest::newRow("ascii-lower") << "fluke" << "fluke" << "fluke" << "fluke";
    BOBUIest::newRow("ascii-mixed") << "FLuke" << "fluke" << "fluke" << "fluke";
    BOBUIest::newRow("ascii-upper") << "FLUKE" << "fluke" << "fluke" << "fluke";

    // U+FB01 LATIN SMALL LIGATURE FI
    BOBUIest::newRow("asciifolded") << u"\uFB01le"_s << "file" << "." << "file";
    BOBUIest::newRow("asciifolded-dotcom") << u"\uFB01le.example.com"_s << "file.example.com" << "." << "file.example.com";
    BOBUIest::newRow("greek-mu") << QString::fromLatin1("\265V")
                              <<"xn--v-lmb"
                              << "."
                              << QString::fromUtf8("\316\274v");

    BOBUIest::newRow("non-ascii-lower") << QString::fromLatin1("alqualond\353")
                                     << "xn--alqualond-34a"
                                     << "."
                                     << QString::fromLatin1("alqualond\353");
    BOBUIest::newRow("non-ascii-mixed") << QString::fromLatin1("Alqualond\353")
                                     << "xn--alqualond-34a"
                                     << "."
                                     << QString::fromLatin1("alqualond\353");
    BOBUIest::newRow("non-ascii-upper") << QString::fromLatin1("ALQUALOND\313")
                                     << "xn--alqualond-34a"
                                     << "."
                                     << QString::fromLatin1("alqualond\353");

    BOBUIest::newRow("idn-lower") << "xn--alqualond-34a" << "xn--alqualond-34a"
                               << QString::fromLatin1("alqualond\353")
                               << QString::fromLatin1("alqualond\353");
    BOBUIest::newRow("idn-mixed") << "Xn--alqualond-34a" << "xn--alqualond-34a"
                               << QString::fromLatin1("alqualond\353")
                               << QString::fromLatin1("alqualond\353");
    BOBUIest::newRow("idn-mixed2") << "XN--alqualond-34a" << "xn--alqualond-34a"
                                << QString::fromLatin1("alqualond\353")
                                << QString::fromLatin1("alqualond\353");
    BOBUIest::newRow("idn-mixed3") << "xn--ALQUALOND-34a" << "xn--alqualond-34a"
                                << QString::fromLatin1("alqualond\353")
                                << QString::fromLatin1("alqualond\353");
    BOBUIest::newRow("idn-mixed4") << "xn--alqualond-34A" << "xn--alqualond-34a"
                                << QString::fromLatin1("alqualond\353")
                                << QString::fromLatin1("alqualond\353");
    BOBUIest::newRow("idn-upper") << "XN--ALQUALOND-34A" << "xn--alqualond-34a"
                               << QString::fromLatin1("alqualond\353")
                               << QString::fromLatin1("alqualond\353");

    BOBUIest::newRow("separator-3002") << QString::fromUtf8("example\343\200\202com")
                                    << "example.com" << "." << "example.com";

    QString egyptianIDN =
        QString::fromUtf8("\331\210\330\262\330\247\330\261\330\251\055\330\247\331\204\330"
                          "\243\330\252\330\265\330\247\331\204\330\247\330\252.\331\205"
                          "\330\265\330\261");
    BOBUIest::newRow("egyptian-tld-ace")
        << "xn----rmckbbajlc6dj7bxne2c.xn--wgbh1c"
        << "xn----rmckbbajlc6dj7bxne2c.xn--wgbh1c"
        << "."
        << egyptianIDN;
    BOBUIest::newRow("egyptian-tld-unicode")
        << egyptianIDN
        << "xn----rmckbbajlc6dj7bxne2c.xn--wgbh1c"
        << "."
        << egyptianIDN;
    BOBUIest::newRow("egyptian-tld-mix1")
        << QString::fromUtf8("\331\210\330\262\330\247\330\261\330\251\055\330\247\331\204\330"
                             "\243\330\252\330\265\330\247\331\204\330\247\330\252.xn--wgbh1c")
        << "xn----rmckbbajlc6dj7bxne2c.xn--wgbh1c"
        << "."
        << egyptianIDN;
    BOBUIest::newRow("egyptian-tld-mix2")
        << QString::fromUtf8("xn----rmckbbajlc6dj7bxne2c.\331\205\330\265\330\261")
        << "xn----rmckbbajlc6dj7bxne2c.xn--wgbh1c"
        << "."
        << egyptianIDN;

    QString russianIDN = QString::fromUtf8("\321\217\320\275\320\264\320\265\320\272\321\201.\321\200\321\204");
    BOBUIest::newRow("russian-tld-ace")
        << "xn--d1acpjx3f.xn--p1ai"
        << "xn--d1acpjx3f.xn--p1ai"
        << "."
        << russianIDN;

    QString taiwaneseIDN = QString::fromUtf8("\345\217\260\345\214\227\346\214\211\346\221\251.\345\217\260\347\201\243");
    BOBUIest::newRow("taiwanese-tld-ace")
        << "xn--djrptm67aikb.xn--kpry57d"
        << "xn--djrptm67aikb.xn--kpry57d"
        << "."
        << taiwaneseIDN;

    // violations / invalids
    auto badRow = [](const char *name, const char *text) {
        BOBUIest::newRow(name) << text << text << text << text;
    };

    badRow("invalid-punycode", "xn--z");

    // U+00A0 NO-BREAK SPACE encodes to Punycode "6a"
    // but it is prohibited and should have caused encoding failure
    badRow("invalid-nameprep-prohibited", "xn--6a");

    // U+00AD SOFT HYPHEN between "a" and "b" encodes to Punycode "ab-5da"
    // but it should have been removed in the nameprep stage
    badRow("invalid-nameprep-maptonothing", "xn-ab-5da");

    // U+00C1 LATIN CAPITAL LETTER A WITH ACUTE encodes to Punycode "4ba"
    // but it should have nameprepped to lowercase first
    badRow("invalid-nameprep-uppercase", "xn--4ba");

    // U+00B5 MICRO SIGN encodes to Punycode "sba"
    // but is should have nameprepped to NFKC U+03BC GREEK SMALL LETTER MU
    badRow("invalid-nameprep-nonnfkc", "xn--sba");

    // Decodes to "a" in some versions, see BOBUIBUG-95689
    badRow("punycode-overflow-1", "xn--5p32g");
    // Decodes to the same string as "xn--097c" in some versions, see BOBUIBUG-95689
    badRow("punycode-overflow-2", "xn--400595c");

    // Encodes 2**32, decodes to empty string in some versions
    badRow("punycode-overflow-3", "xn--l0902716a");
}

void tst_QUrlInternal::ace_testsuite()
{
    static const char canonsuffix[] = ".troll.no";
    QFETCH(QString, in);
    QFETCH(QString, toace);
    QFETCH(QString, fromace);
    QFETCH(QString, unicode);

    const char *suffix = canonsuffix;
    if (toace.contains('.'))
        suffix = 0;

    QString domain = in + suffix;
    QCOMPARE(QString::fromLatin1(QUrl::toAce(domain)), toace + suffix);
    if (fromace != ".")
        QCOMPARE(QUrl::fromAce(domain.toLatin1()), fromace + suffix);
    QCOMPARE(QUrl::fromAce(QUrl::toAce(domain)), unicode + suffix);

    QUrl u;
    u.setHost(domain);
    QVERIFY(u.isValid());
    QCOMPARE(u.host(), unicode + suffix);
    QCOMPARE(u.host(QUrl::EncodeUnicode), toace + suffix);
    QCOMPARE(u.toEncoded(), "//" + toace.toLatin1() + suffix);
    QCOMPARE(u.toDisplayString(), "//" + unicode + suffix);

    domain = in + (suffix ? ".troll.No" : "");
    QCOMPARE(QString::fromLatin1(QUrl::toAce(domain)), toace + suffix);
    if (fromace != ".")
        QCOMPARE(QUrl::fromAce(domain.toLatin1()), fromace + suffix);
    QCOMPARE(QUrl::fromAce(QUrl::toAce(domain)), unicode + suffix);

    domain = in + (suffix ? ".troll.NO" : "");
    QCOMPARE(QString::fromLatin1(QUrl::toAce(domain)), toace + suffix);
    if (fromace != ".")
        QCOMPARE(QUrl::fromAce(domain.toLatin1()), fromace + suffix);
    QCOMPARE(QUrl::fromAce(QUrl::toAce(domain)), unicode + suffix);
}

void tst_QUrlInternal::std3violations_data()
{
    BOBUIest::addColumn<QString>("source");
    BOBUIest::addColumn<bool>("validUrl");

    BOBUIest::newRow("too-long") << "this-domain-is-far-too-long-for-its-own-good-and-should-have-been-limited-to-63-chars" << false;
    BOBUIest::newRow("dash-begin") << "-x-foo" << false;
    BOBUIest::newRow("dash-end") << "x-foo-" << false;
    BOBUIest::newRow("dash-begin-end") << "-foo-" << false;

    BOBUIest::newRow("control") << "\033foo" << false;
    BOBUIest::newRow("bang") << "foo!" << false;
    BOBUIest::newRow("plus") << "foo+bar" << false;
    BOBUIest::newRow("slash") << "foo/bar" << true;
    BOBUIest::newRow("colon") << "foo:80" << true;
    BOBUIest::newRow("question") << "foo?bar" << true;
    BOBUIest::newRow("at") << "foo@bar" << true;
    BOBUIest::newRow("backslash") << "foo\\bar" << false;

    // these characters are transformed by NFKC to non-LDH characters
    BOBUIest::newRow("dot-like") << QString::fromUtf8("foo\342\200\244bar") << false;  // U+2024 ONE DOT LEADER
    BOBUIest::newRow("slash-like") << QString::fromUtf8("foo\357\274\217bar") << false;    // U+FF0F FULLWIDTH SOLIDUS

    // The following should be invalid but isn't
    // the DIVISON SLASH doesn't case-fold to a slash
    // is this a problem with RFC 3490?
    //BOBUIest::newRow("slash-like2") << QString::fromUtf8("foo\342\210\225bar") << false; // U+2215 DIVISION SLASH
}

void tst_QUrlInternal::std3violations()
{
    QFETCH(QString, source);

    QUrl url;
    url.setHost(source);
    QVERIFY(url.host().isEmpty());

    QFETCH(bool, validUrl);
    if (validUrl)
        return;  // test ends here for these cases

    url = QUrl("http://" + source + "/some/path");
    QVERIFY(!url.isValid());
}

void tst_QUrlInternal::std3deviations_data()
{
    BOBUIest::addColumn<QString>("source");

    BOBUIest::newRow("ending-dot") << "example.com.";
    BOBUIest::newRow("ending-dot3002") << QString("example.com") + QChar(0x3002);
    BOBUIest::newRow("underline") << "foo_bar";  //BOBUIBUG-7434
}

void tst_QUrlInternal::std3deviations()
{
    QFETCH(QString, source);
    QVERIFY(!QUrl::toAce(source).isEmpty());

    QUrl url;
    url.setHost(source);
    QVERIFY(!url.host().isEmpty());
}

void tst_QUrlInternal::correctEncodedMistakes_data()
{
    BOBUIest::addColumn<QString>("input");
    BOBUIest::addColumn<QString>("expected");

    BOBUIest::newRow("empty") << "" << "";

    // these contain one invalid percent
    BOBUIest::newRow("%") << QString("%") << QString("%25");
    BOBUIest::newRow("3%") << QString("3%") << QString("3%25");
    BOBUIest::newRow("13%") << QString("13%") << QString("13%25");
    BOBUIest::newRow("13%!") << QString("13%!") << QString("13%25!");
    BOBUIest::newRow("13%!!") << QString("13%!!") << QString("13%25!!");
    BOBUIest::newRow("13%a") << QString("13%a") << QString("13%25a");
    BOBUIest::newRow("13%az") << QString("13%az") << QString("13%25az");

    // two invalid percents
    BOBUIest::newRow("13%%") << "13%%" << "13%25%25";
    BOBUIest::newRow("13%a%a") << "13%a%a" << "13%25a%25a";
    BOBUIest::newRow("13%az%az") << "13%az%az" << "13%25az%25az";

    // these are correct (idempotent)
    BOBUIest::newRow("13%25") << QString("13%25")  << QString("13%25");
    BOBUIest::newRow("13%25%25") << QString("13%25%25")  << QString("13%25%25");

    // these contain one invalid and one valid
    // the code assumes they are all invalid
    BOBUIest::newRow("13%13..%") << "13%13..%" << "13%2513..%25";
    BOBUIest::newRow("13%..%13") << "13%..%13" << "13%25..%2513";

    // three percents, one invalid
    BOBUIest::newRow("%01%02%3") << "%01%02%3" << "%2501%2502%253";

    // now mix bad percents with Unicode decoding
    BOBUIest::newRow("%C2%") << "%C2%" << "%25C2%25";
    BOBUIest::newRow("%C2%A") << "%C2%A" << "%25C2%25A";
    BOBUIest::newRow("%C2%Az") << "%C2%Az" << "%25C2%25Az";
    BOBUIest::newRow("%E2%A0%") << "%E2%A0%" << "%25E2%25A0%25";
    BOBUIest::newRow("%E2%A0%A") << "%E2%A0%A" << "%25E2%25A0%25A";
    BOBUIest::newRow("%E2%A0%Az") << "%E2%A0%Az" << "%25E2%25A0%25Az";
    BOBUIest::newRow("%F2%A0%A0%") << "%F2%A0%A0%" << "%25F2%25A0%25A0%25";
    BOBUIest::newRow("%F2%A0%A0%A") << "%F2%A0%A0%A" << "%25F2%25A0%25A0%25A";
    BOBUIest::newRow("%F2%A0%A0%Az") << "%F2%A0%A0%Az" << "%25F2%25A0%25A0%25Az";
}

void tst_QUrlInternal::correctEncodedMistakes()
{
    QFETCH(QString, input);
    QFETCH(QString, expected);

    // prepend some data to be sure that it remains there
    QString dataTag = BOBUIest::currentDataTag();
    QString output = dataTag;

    if (!bobui_urlRecode(output, input, { }))
        output += input;
    QCOMPARE(output, dataTag + expected);

    // now try the full decode mode
    output = dataTag;
    QString expected2 = QUrl::fromPercentEncoding(expected.toLatin1());

    if (!bobui_urlRecode(output, input, QUrl::FullyDecoded))
        output += input;
    QCOMPARE(output, dataTag + expected2);
}

static void addUtf8Data(const char *name, const char *data)
{
    QString encoded = QByteArray(data).toPercentEncoding();
    QString decoded = QString::fromUtf8(data);

    // this data contains invaild UTF-8 sequences, so FullyDecoded doesn't work (by design)
    // use PrettyDecoded instead
    BOBUIest::newRow(QByteArray("decode-") + name) << encoded << QUrl::ComponentFormattingOptions(QUrl::PrettyDecoded) << decoded;
    BOBUIest::newRow(QByteArray("encode-") + name) << decoded << QUrl::ComponentFormattingOptions(QUrl::FullyEncoded) << encoded;
}

void tst_QUrlInternal::encodingRecode_data()
{
    typedef QUrl::ComponentFormattingOptions F;
    BOBUIest::addColumn<QString>("input");
    BOBUIest::addColumn<F>("encodingMode");
    BOBUIest::addColumn<QString>("expected");

    // -- idempotent tests --
    static int modes[] = { QUrl::PrettyDecoded,
                           QUrl::EncodeSpaces,
                           QUrl::EncodeSpaces | QUrl::EncodeUnicode,
                           QUrl::EncodeSpaces | QUrl::EncodeUnicode | QUrl::EncodeDelimiters,
                           QUrl::EncodeSpaces | QUrl::EncodeUnicode | QUrl::EncodeDelimiters | QUrl::EncodeReserved,
                           QUrl::EncodeSpaces | QUrl::EncodeUnicode | QUrl::EncodeDelimiters | QUrl::DecodeReserved,
                           QUrl::EncodeSpaces | QUrl::EncodeUnicode | QUrl::EncodeReserved,
                           QUrl::EncodeSpaces | QUrl::EncodeUnicode | QUrl::DecodeReserved,
                           QUrl::EncodeSpaces | QUrl::EncodeDelimiters,
                           QUrl::EncodeSpaces | QUrl::EncodeDelimiters | QUrl::EncodeReserved,
                           QUrl::EncodeSpaces | QUrl::EncodeDelimiters | QUrl::DecodeReserved,
                           QUrl::EncodeSpaces | QUrl::EncodeReserved,
                           QUrl::EncodeSpaces | QUrl::DecodeReserved,

                           QUrl::EncodeUnicode,
                           QUrl::EncodeUnicode | QUrl::EncodeDelimiters,
                           QUrl::EncodeUnicode | QUrl::EncodeDelimiters | QUrl::EncodeReserved,
                           QUrl::EncodeUnicode | QUrl::EncodeDelimiters | QUrl::DecodeReserved,
                           QUrl::EncodeUnicode | QUrl::EncodeReserved,

                           QUrl::EncodeDelimiters,
                           QUrl::EncodeDelimiters | QUrl::EncodeReserved,
                           QUrl::EncodeDelimiters | QUrl::DecodeReserved,
                           QUrl::EncodeReserved,
                           QUrl::DecodeReserved };
    for (uint i = 0; i < sizeof(modes)/sizeof(modes[0]); ++i) {
        QByteArray code = QByteArray::number(modes[i], 16);
        F mode = QUrl::ComponentFormattingOption(modes[i]);

        BOBUIest::newRow("null-0x" + code) << QString() << mode << QString();
        BOBUIest::newRow("empty-0x" + code) << "" << mode << "";

        //    unreserved    = ALPHA / DIGIT / "-" / "." / "_" / "~"
        // Unreserved characters are never encoded
        BOBUIest::newRow("alpha-0x" + code) << "abcABCZZzz" << mode << "abcABCZZzz";
        BOBUIest::newRow("digits-0x" + code) << "01234567890" << mode << "01234567890";
        BOBUIest::newRow("otherunreserved-0x" + code) << "-._~" << mode << "-._~";

        // Control characters are always encoded
        // Use uppercase because the output is also uppercased
        BOBUIest::newRow("control-nul-0x" + code) << "%00" << mode << "%00";
        BOBUIest::newRow("control-0x" + code) << "%0D%0A%1F%1A%7F" << mode << "%0D%0A%1F%1A%7F";

        // The percent is always encoded
        BOBUIest::newRow("percent-0x" + code) << "25%2525" << mode << "25%2525";

        // mixed control and unreserved
        BOBUIest::newRow("control-unreserved-0x" + code) << "Foo%00Bar%0D%0Abksp%7F" << mode << "Foo%00Bar%0D%0Abksp%7F";
    }

    // however, control characters and the percent *are* decoded in FullyDecoded mode
    // this is the only exception
    BOBUIest::newRow("control-nul-fullydecoded") << "%00" << F(QUrl::FullyDecoded) << QStringLiteral("\0");
    BOBUIest::newRow("control-fullydecoded") << "%0D%0A%1F%1A%7F" << F(QUrl::FullyDecoded) << "\r\n\x1f\x1a\x7f";
    BOBUIest::newRow("percent-fullydecoded") << "25%2525" << F(QUrl::FullyDecoded) << "25%25";
    BOBUIest::newRow("control-unreserved-fullydecoded") << "Foo%00Bar%0D%0Abksp%7F" << F(QUrl::FullyDecoded)
                                                     << QStringLiteral("Foo\0Bar\r\nbksp\x7F");

    //    gen-delims    = ":" / "/" / "?" / "#" / "[" / "]" / "@"
    //    sub-delims    = "!" / "$" / "&" / "'" / "(" / ")"
    //                  / "*" / "+" / "," / ";" / "="
    // in the default operation, delimiters don't get encoded or decoded
    static const char delimiters[] =  ":/?#[]@" "!$&'()*+,;=";
    for (const char *c = delimiters; *c; ++c) {
        QByteArray code = QByteArray::number(*c, 16);
        QString encoded = QString("abc%") + code.toUpper() + "def" ;
        QString decoded = QString("abc") + *c + "def" ;
        BOBUIest::newRow("delimiter-encoded-" + code) << encoded << F(QUrl::FullyEncoded) << encoded;
        BOBUIest::newRow("delimiter-decoded-" + code) << decoded << F(QUrl::FullyEncoded) << decoded;
    }

    // encode control characters
    BOBUIest::newRow("encode-control") << "\1abc\2\033esc" << F(QUrl::PrettyDecoded) << "%01abc%02%1Besc";
    BOBUIest::newRow("encode-nul") << QString::fromLatin1("abc\0def", 7) << F(QUrl::PrettyDecoded) << "abc%00def";

    // space
    BOBUIest::newRow("space-leave-decoded") << "Hello World " << F(QUrl::PrettyDecoded) << "Hello World ";
    BOBUIest::newRow("space-leave-encoded") << "Hello%20World%20" << F(QUrl::FullyEncoded) << "Hello%20World%20";
    BOBUIest::newRow("space-encode") << "Hello World " << F(QUrl::FullyEncoded) << "Hello%20World%20";
    BOBUIest::newRow("space-decode") << "Hello%20World%20" << F(QUrl::PrettyDecoded) << "Hello World ";

    // decode unreserved
    BOBUIest::newRow("unreserved-decode") << "%66%6f%6f%42a%72" << F(QUrl::FullyEncoded) << "fooBar";

    // mix encoding with decoding
    BOBUIest::newRow("encode-control-decode-space") << "\1\2%200" << F(QUrl::PrettyDecoded) << "%01%02 0";
    BOBUIest::newRow("decode-space-encode-control") << "%20\1\2" << F(QUrl::PrettyDecoded) << " %01%02";

    // decode and encode valid UTF-8 data
    // invalid is tested in encodingRecodeInvalidUtf8
    addUtf8Data("utf8-2char-1", "\xC2\x80"); // U+0080
    addUtf8Data("utf8-2char-2", "\xDF\xBF"); // U+07FF
    addUtf8Data("utf8-3char-1", "\xE0\xA0\x80"); // U+0800
    addUtf8Data("utf8-3char-2", "\xED\x9F\xBF"); // U+D7FF
    addUtf8Data("utf8-3char-3", "\xEE\x80\x80"); // U+E000
    addUtf8Data("utf8-3char-4", "\xEF\xBF\xBD"); // U+FFFD
    addUtf8Data("utf8-4char-1", "\xF0\x90\x80\x80"); // U+10000
    addUtf8Data("utf8-4char-2", "\xF4\x8F\xBF\xBD"); // U+10FFFD

    // longer UTF-8 sequences, mixed with unreserved
    addUtf8Data("utf8-string-1", "R\xc3\xa9sum\xc3\xa9");
    addUtf8Data("utf8-string-2", "\xDF\xBF\xE0\xA0\x80""A");
    addUtf8Data("utf8-string-3", "\xE0\xA0\x80\xDF\xBF...");

    BOBUIest::newRow("encode-unicode-noncharacter") << QString(QChar(0xffff)) << F(QUrl::FullyEncoded) << "%EF%BF%BF";
    BOBUIest::newRow("decode-unicode-noncharacter") << QString(QChar(0xffff)) << F(QUrl::PrettyDecoded) << QString::fromUtf8("\xEF\xBF\xBF");

    // special cases: stuff we can encode, but not decode
    BOBUIest::newRow("unicode-lo-surrogate") << QString(QChar(0xD800)) << F(QUrl::FullyEncoded) << "%ED%A0%80";
    BOBUIest::newRow("unicode-hi-surrogate") << QString(QChar(0xDC00)) << F(QUrl::FullyEncoded) << "%ED%B0%80";

    // a couple of Unicode strings with leading spaces
    BOBUIest::newRow("space-unicode") << QString::fromUtf8(" \xc2\xa0") << F(QUrl::FullyEncoded) << "%20%C2%A0";
    BOBUIest::newRow("space-space-unicode") << QString::fromUtf8("  \xc2\xa0") << F(QUrl::FullyEncoded) << "%20%20%C2%A0";
    BOBUIest::newRow("space-space-space-unicode") << QString::fromUtf8("   \xc2\xa0") << F(QUrl::FullyEncoded) << "%20%20%20%C2%A0";

    // hex case testing
    BOBUIest::newRow("FF") << "%FF" << F(QUrl::FullyEncoded) << "%FF";
    BOBUIest::newRow("Ff") << "%Ff" << F(QUrl::FullyEncoded) << "%FF";
    BOBUIest::newRow("fF") << "%fF" << F(QUrl::FullyEncoded) << "%FF";
    BOBUIest::newRow("ff") << "%ff" << F(QUrl::FullyEncoded) << "%FF";

    // decode UTF-8 mixed with non-UTF-8 and unreserved
    BOBUIest::newRow("utf8-mix-1") << "%80%C2%80" << F(QUrl::PrettyDecoded) << QString::fromUtf8("%80\xC2\x80");
    BOBUIest::newRow("utf8-mix-2") << "%C2%C2%80" << F(QUrl::PrettyDecoded) << QString::fromUtf8("%C2\xC2\x80");
    BOBUIest::newRow("utf8-mix-3") << "%E0%C2%80" << F(QUrl::PrettyDecoded) << QString::fromUtf8("%E0\xC2\x80");
    BOBUIest::newRow("utf8-mix-3") << "A%C2%80" << F(QUrl::PrettyDecoded) << QString::fromUtf8("A\xC2\x80");
    BOBUIest::newRow("utf8-mix-3") << "%C2%80A" << F(QUrl::PrettyDecoded) << QString::fromUtf8("\xC2\x80""A");
}

void tst_QUrlInternal::encodingRecode()
{
    QFETCH(QString, input);
    QFETCH(QString, expected);
    QFETCH(QUrl::ComponentFormattingOptions, encodingMode);

    // prepend some data to be sure that it remains there
    QString output = BOBUIest::currentDataTag();
    expected.prepend(output);

    if (!bobui_urlRecode(output, input, encodingMode))
        output += input;
    QCOMPARE(output, expected);
}

void tst_QUrlInternal::encodingRecodeInvalidUtf8_data()
{
    BOBUIest::addColumn<QByteArray>("utf8");
    BOBUIest::addColumn<QString>("utf16");

    extern void loadInvalidUtf8Rows();
    extern void loadNonCharactersRows();
    loadInvalidUtf8Rows();
    loadNonCharactersRows();

    BOBUIest::newRow("utf8-mix-4") << QByteArray("\xE0.A2\x80");
    BOBUIest::newRow("utf8-mix-5") << QByteArray("\xE0\xA2.80");
    BOBUIest::newRow("utf8-mix-6") << QByteArray("\xE0\xA2\x33");
}

void tst_QUrlInternal::encodingRecodeInvalidUtf8()
{
    QFETCH(QByteArray, utf8);
    QString input = utf8.toPercentEncoding();

    // prepend some data to be sure that it remains there
    QString output = BOBUIest::currentDataTag();

    if (!bobui_urlRecode(output, input, QUrl::PrettyDecoded))
        output += input;
    QCOMPARE(output, BOBUIest::currentDataTag() + input);

    // this is just control
    output = BOBUIest::currentDataTag();
    if (!bobui_urlRecode(output, input, QUrl::FullyEncoded))
        output += input;
    QCOMPARE(output, BOBUIest::currentDataTag() + input);

    // verify for security reasons that all bad UTF-8 data got replaced by QChar::ReplacementCharacter
    output = BOBUIest::currentDataTag();
    if (!bobui_urlRecode(output, input, QUrl::FullyEncoded))
        output += input;
    for (int i = int(strlen(BOBUIest::currentDataTag())); i < output.size(); ++i) {
        QVERIFY2(output.at(i).unicode() < 0x80 || output.at(i) == QChar::ReplacementCharacter,
                 qPrintable(QString("Character at i == %1 was U+%2").arg(i)
                            .arg(ushort{output.at(i).unicode()}, 4, 16, u'0')));
    }
}

void tst_QUrlInternal::tldRestrictions_data()
{
    BOBUIest::addColumn<QString>("tld");
    BOBUIest::addColumn<bool>("encode");

    // current whitelist
    BOBUIest::newRow("ac")  << QString("ac")  << true;
    BOBUIest::newRow("ar")  << QString("ar")  << true;
    BOBUIest::newRow("asia")  << QString("asia")  << true;
    BOBUIest::newRow("at") << QString("at") << true;
    BOBUIest::newRow("biz")  << QString("biz")  << true;
    BOBUIest::newRow("br") << QString("br") << true;
    BOBUIest::newRow("cat")  << QString("cat")  << true;
    BOBUIest::newRow("ch")  << QString("ch")  << true;
    BOBUIest::newRow("cl")  << QString("cl")  << true;
    BOBUIest::newRow("cn") << QString("cn") << true;
    BOBUIest::newRow("com")  << QString("com")  << true;
    BOBUIest::newRow("de")  << QString("de")  << true;
    BOBUIest::newRow("dk") << QString("dk") << true;
    BOBUIest::newRow("es")  << QString("es")  << true;
    BOBUIest::newRow("fi") << QString("fi") << true;
    BOBUIest::newRow("gr")  << QString("gr")  << true;
    BOBUIest::newRow("hu") << QString("hu") << true;
    BOBUIest::newRow("il")  << QString("il")  << true;
    BOBUIest::newRow("info")  << QString("info")  << true;
    BOBUIest::newRow("io") << QString("io") << true;
    BOBUIest::newRow("is")  << QString("is")  << true;
    BOBUIest::newRow("ir")  << QString("ir")  << true;
    BOBUIest::newRow("jp") << QString("jp") << true;
    BOBUIest::newRow("kr") << QString("kr") << true;
    BOBUIest::newRow("li")  << QString("li")  << true;
    BOBUIest::newRow("lt") << QString("lt") << true;
    BOBUIest::newRow("lu")  << QString("lu")  << true;
    BOBUIest::newRow("lv")  << QString("lv")  << true;
    BOBUIest::newRow("museum") << QString("museum") << true;
    BOBUIest::newRow("name")  << QString("name")  << true;
    BOBUIest::newRow("net")  << QString("name")  << true;
    BOBUIest::newRow("no") << QString("no") << true;
    BOBUIest::newRow("nu")  << QString("nu")  << true;
    BOBUIest::newRow("nz")  << QString("nz")  << true;
    BOBUIest::newRow("org")  << QString("org")  << true;
    BOBUIest::newRow("pl")  << QString("pl")  << true;
    BOBUIest::newRow("pr")  << QString("pr")  << true;
    BOBUIest::newRow("se")  << QString("se")  << true;
    BOBUIest::newRow("sh") << QString("sh") << true;
    BOBUIest::newRow("tel")  << QString("tel")  << true;
    BOBUIest::newRow("th")  << QString("th")  << true;
    BOBUIest::newRow("tm")  << QString("tm")  << true;
    BOBUIest::newRow("tw") << QString("tw") << true;
    BOBUIest::newRow("ua")  << QString("ua")  << true;
    BOBUIest::newRow("vn") << QString("vn") << true;

    // known blacklists:
    BOBUIest::newRow("foo") << QString("foo") << false;
}

void tst_QUrlInternal::tldRestrictions()
{
    QFETCH(QString, tld);

    // www.brød.tld
    QByteArray ascii = "www.xn--brd-1na." + tld.toLatin1();
    QString unicode = QLatin1String("www.br\370d.") + tld;
    QString encoded = QUrl::fromAce(ascii);
    BOBUIEST(!encoded.contains(".xn--"), "encode");
    BOBUIEST(encoded == unicode, "encode");

    QUrl url = QUrl::fromEncoded("http://www.xn--brd-1na." + tld.toLatin1());
    BOBUIEST(!url.host().contains(".xn--"), "encode");
    BOBUIEST(url.host() == unicode, "encode");

    url.setUrl(QLatin1String("http://www.xn--brd-1na.") + tld);
    BOBUIEST(!url.host().contains(".xn--"), "encode");
    BOBUIEST(url.host() == unicode, "encode");

    url.setUrl(QLatin1String("http://www.br\370d.") + tld);
    BOBUIEST(!url.host().contains(".xn--"), "encode");
    BOBUIEST(url.host() == unicode, "encode");

    url = QUrl::fromEncoded("http://www.br%C3%B8d." + tld.toLatin1());
    BOBUIEST(!url.host().contains(".xn--"), "encode");
    BOBUIEST(url.host() == unicode, "encode");
}


BOBUIEST_APPLESS_MAIN(tst_QUrlInternal)
