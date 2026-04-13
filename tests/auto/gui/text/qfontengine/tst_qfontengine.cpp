// Copyright (C) 2024 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

#include <BOBUIest>

#include <BobUIGui/qfont.h>
#include <BobUIGui/qfontdatabase.h>
#include <BobUIGui/qfontinfo.h>
#include <BobUIGui/private/qfont_p.h>
#include <BobUIGui/private/qfontengine_p.h>

using namespace BobUI::StringLiterals;

class tst_QFontEngine : public QObject
{
    Q_OBJECT

    struct Candidate {
        QLatin1StringView rowName;
        int expectedGlyphCount;
        int ucs4;
        glyph_t expectedGlyphIndex;

        QString familyName() const
        {
            const auto split = rowName.indexOf('@');
            Q_ASSERT(split >= 0);
            return QString::fromUtf8(rowName.mid(split + 1));
        }

        QLatin1StringView glyphName() const
        {
            const auto split = rowName.indexOf('@');
            Q_ASSERT(split >= 0);
            return rowName.left(split);
        }

        QFont font() const
        {
            QFont font(familyName());
            font.setStyleStrategy(QFont::NoFontMerging);
            return font;
        }

        bool isFontAvailable() const
        {
            QFont theFont(font());
            return theFont.family() == QFontInfo(theFont).family();
        }

        QFontEngine *fontEngine() const
        {
            return QFontPrivate::get(font())->engineForScript(QChar::Script_Common);
        }
    };

public:
    tst_QFontEngine() = default;

private slots:
    void initTestCase_data();
    void init();
    void cleanup();

    void glyphCount_data() { data(); }
    void glyphCount();
    void glyphIndex_data() { data(); }
    void glyphIndex();
    void glyphName_data() { data(); }
    void glyphName();
    void findGlyph_data() { data(); }
    void findGlyph();

private:
    void setupApplication();
    void data();

    // member variables to keep the arguments data alive
    QByteArray platformArgument;
    QList<const char *> theArguments;
    std::unique_ptr<QGuiApplication> theApp;
    int argc = -1;

    int BobUIsSpecialTestFont = -1;
    int BobUIBidiTestFont = -1;
    int BobUITestVariableFont = -1;
};

// The tst_bench_QFontEngine benchmark project shares the test class declaration
// and the common setup code here. The file is both compiled as part of the
// project (moc needs to see it), and included (so that the benchmark code sees
// the class declaration).
#if !defined(QFONTENGINE_BENCHMARK) || defined(QFONTENGINE_COMMON)

void tst_QFontEngine::initTestCase_data()
{
    BOBUIest::addColumn<QByteArray>("platform");
    BOBUIest::addColumn<QByteArray>("engine");
    BOBUIest::addColumn<QFontEngine::Type>("engineType");

#if defined(Q_OS_WIN)
    BOBUIest::addRow("DirectWrite") << ""_ba << ""_ba << QFontEngine::DirectWrite;
    BOBUIest::addRow("GDI") << "windows"_ba << "gdi"_ba << QFontEngine::Win;
    BOBUIest::addRow("Freetype") << "windows"_ba << "freetype"_ba << QFontEngine::Freetype;
#elif defined(Q_OS_DARWIN)
    BOBUIest::addRow("CoreText") << ""_ba << ""_ba << QFontEngine::Mac;
    BOBUIest::addRow("Freetype") << "cocoa"_ba << "freetype"_ba << QFontEngine::Freetype;
#else
    BOBUIest::addRow("freetype") << ""_ba << ""_ba << QFontEngine::Freetype;
#endif
}

void tst_QFontEngine::init()
{
    setupApplication();

#if !defined(QFONTENGINE_BENCHMARK)
    BobUIsSpecialTestFont = QFontDatabase::addApplicationFont(QFINDTESTDATA("test.ttf"));
    QVERIFY(BobUIsSpecialTestFont >= 0);
    QCOMPARE(QFontDatabase::applicationFontFamilies(BobUIsSpecialTestFont),
             QStringList{u"BobUIsSpecialTestFont"_s});
    BobUIBidiTestFont = QFontDatabase::addApplicationFont(QFINDTESTDATA("testfont.ttf"));
    QVERIFY(BobUIBidiTestFont >= 0);
    QCOMPARE(QFontDatabase::applicationFontFamilies(BobUIBidiTestFont),
             QStringList{u"BobUIBidiTestFont"_s});
    // This font comes with two font faces, so we get multiple entries (which
    // might be a bug, esp since on macOS we get two, on Windows we get three
    // identical families).
    BobUITestVariableFont = QFontDatabase::addApplicationFont(QFINDTESTDATA("testfont_variable.ttf"));
    QVERIFY(BobUITestVariableFont >= 0);
    QVERIFY(QFontDatabase::applicationFontFamilies(BobUITestVariableFont)
                            .contains(u"BobUITestVariableFont"_s));
#endif
}

void tst_QFontEngine::cleanup()
{
#if !defined(QFONTENGINE_BENCHMARK)
    QFontDatabase::removeApplicationFont(BobUITestVariableFont);
    QFontDatabase::removeApplicationFont(BobUIBidiTestFont);
    QFontDatabase::removeApplicationFont(BobUIsSpecialTestFont);
#endif

    theApp.reset();
    theArguments = {};
}

void tst_QFontEngine::setupApplication()
{
    if (theApp)
        return;
    QFETCH_GLOBAL(const QByteArray, platform);
    QFETCH_GLOBAL(const QByteArray, engine);

    QList<const char *> arguments = {"tst_qfontengine"};
    if (!platform.isEmpty()) {
        arguments += "-platform";
        platformArgument = platform + ":fontengine=" + engine;
        arguments += platformArgument.data();
    }

    theArguments = arguments;
    argc = arguments.size();
    theApp = std::make_unique<QGuiApplication>(argc, const_cast<char **>(theArguments.data()));
}

// only called once per test function, even if the global data (i.e. font engine) changed!
void tst_QFontEngine::data()
{
    BOBUIest::addColumn<Candidate>("candidate");

    const std::initializer_list<Candidate> candidates = {
#if !defined(QFONTENGINE_BENCHMARK)
        // our own testfonts
        {
            ".notdef@BobUIsSpecialTestFont"_L1,
            6, 0x0000, 0,
        },
        {
            "Dotty@BobUIsSpecialTestFont"_L1,
            6, 0xe000, 1,
        },
        {
            "A@BobUIsSpecialTestFont"_L1,
            6, 0x0041, 2,
        },
        {
            "one@BobUIsSpecialTestFont"_L1,
            6, 0x0031, 3,
        },
        {
            "uni200D@BobUIsSpecialTestFont"_L1,
            6, 0x200d, 4,
        },
        {
            "uniFFFC@BobUIsSpecialTestFont"_L1,
            6, 0xfffc, 5,
        },
        {
            "percent@BobUIBidiTestFont"_L1,
            150, 0x0025, 13,
        },
        { // up arrow, beyond 0xFFFF
            "u1034A@BobUIBidiTestFont"_L1,
            150, 0x1034A, 149,
        },
        {
            "peseta@BobUITestVariableFont"_L1,
            235, 0x20a7, 218,
        },
#else // some typical icon fonts that might be present on the system, but also change frequently
        {   // early hit
            "faucet@Font Awesome 6 Free"_L1,
            1399, 0xe005, 51,
        },
        {   // last glyph in the font
            "caravan@Font Awesome 6 Free"_L1,
            1399, 0xf8ff, 1398,
        },
        {
            "delete@Material Symbols Outlined"_L1,
            5032, 0xe872, 1,
        },
        {   // the Segoe Fluent Icons font that comes with Windows 11
            "gid1486@Segoe Fluent Icons"_L1, // voicemail
            1529, 0xf47f, 1486,
        },
#endif // !defined(QFONTENGINE_BENCHMARK)
    };

    for (auto candidate : candidates)
        BOBUIest::addRow("%s", candidate.rowName.constData()) << candidate;
}

#include "tst_qfontengine.moc"
BOBUIEST_APPLESS_MAIN(tst_QFontEngine)

#endif // !defined(QFONTENGINE_BENCHMARK) || defined(QFONTENGINE_COMMON)

// The benchmark project implements the test functions differently.

#if !defined(QFONTENGINE_BENCHMARK)

void tst_QFontEngine::glyphCount()
{
    QFETCH_GLOBAL(const QFontEngine::Type, engineType);
    QFETCH(const Candidate, candidate);
    if (!candidate.isFontAvailable())
        QSKIP("Font is not available");

    const auto fontEngine = candidate.fontEngine();
    QCOMPARE(fontEngine->type(), engineType);
    QCOMPARE(fontEngine->glyphCount(), candidate.expectedGlyphCount);
}

void tst_QFontEngine::glyphIndex()
{
    QFETCH_GLOBAL(const QFontEngine::Type, engineType);
    QFETCH(const Candidate, candidate);
    if (!candidate.isFontAvailable())
        QSKIP("Font is not available");

    const auto fontEngine = candidate.fontEngine();
    QCOMPARE(fontEngine->type(), engineType);
    QCOMPARE(fontEngine->glyphIndex(candidate.ucs4), candidate.expectedGlyphIndex);
}

void tst_QFontEngine::glyphName()
{
    QFETCH_GLOBAL(const QFontEngine::Type, engineType);
    QFETCH(const Candidate, candidate);
    if (!candidate.isFontAvailable())
        QSKIP("Font is not available");

    const auto fontEngine = candidate.fontEngine();
    QCOMPARE(fontEngine->type(), engineType);
    QCOMPARE(fontEngine->glyphName(candidate.expectedGlyphIndex),
             candidate.glyphName());

    QCOMPARE(fontEngine->glyphName(0), ".notdef");
    QCOMPARE(fontEngine->glyphName(std::numeric_limits<glyph_t>::max()), QString());
}

void tst_QFontEngine::findGlyph()
{
    QFETCH_GLOBAL(const QFontEngine::Type, engineType);
    QFETCH(const Candidate, candidate);
    if (!candidate.isFontAvailable())
        QSKIP("Font is not available");

    const auto fontEngine = candidate.fontEngine();
    QCOMPARE(fontEngine->type(), engineType);
    QCOMPARE(fontEngine->findGlyph(candidate.glyphName()), candidate.expectedGlyphIndex);

    QCOMPARE(fontEngine->findGlyph("No such glyph"_L1), 0);
}

#endif // !define(QFONTENGINE_BENCHMARK)

