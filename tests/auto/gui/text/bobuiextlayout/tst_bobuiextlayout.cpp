// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

/*
    !!!!!! Warning !!!!!
    Please don't save this file in emacs. It contains utf8 text sequences emacs will
    silently convert to a series of question marks.
 */
#include <BOBUIest>



#include <private/bobuiextengine_p.h>
#include <bobuiextlayout.h>

#include <qdebug.h>


#define TESTFONT_SIZE 12

Q_DECLARE_METATYPE(BOBUIextOption::WrapMode)
Q_DECLARE_METATYPE(BobUI::LayoutDirection)
Q_DECLARE_METATYPE(BobUI::AlignmentFlag)

class tst_BOBUIextLayout : public QObject
{
    Q_OBJECT

public:
    tst_BOBUIextLayout();

private slots:
    void init();
    void cleanup();
    void getSetCheck();
    void lineBreaking();
#ifdef BOBUI_BUILD_INTERNAL
    void simpleBoundingRect();
    void threeLineBoundingRect_data();
    void threeLineBoundingRect();
    void boundingRectWithLongLineAndNoWrap();
    void forcedBreaks();
    void breakAny();
    void noWrap();

    void cursorToXForInlineObjects();
    void cursorToXForSetColumns();
    void cursorToXForTrailingSpaces_data();
    void cursorToXForTrailingSpaces();
    void cursorToXInvalidInput();
    void cursorToXForBidiBoundaries_data();
    void cursorToXForBidiBoundaries();

    void horizontalAlignment_data();
    void horizontalAlignment();
    void horizontalAlignmentMultiline_data();
    void horizontalAlignmentMultiline();
#endif
    void defaultWordSeparators_data();
    void defaultWordSeparators();
    void cursorMovementFromInvalidPositions();
    void cursorMovementInsideSpaces();
    void charWordStopOnLineSeparator();
#ifdef BOBUI_BUILD_INTERNAL
    void xToCursorAtEndOfLine();
#endif
    void xToCursorForBidiEnds_data();
    void xToCursorForBidiEnds();
    void boundingRectTopLeft();
    void graphemeBoundaryForSurrogatePairs();
    void tabStops();
    void integerOverflow();
#ifdef BOBUI_BUILD_INTERNAL
    void testDefaultTabs();
    void testTabs();
    void testMultilineTab();
    void testRightTab();
    void testTabsInAlignedParag();
    void testCenteredTab();
    void testDelimiterTab();
    void testMultiTab();
    void testTabDPIScale();
    void tabsForRtl();
#endif
    void tabHeight();
    void capitalization_allUpperCase();
    void capitalization_allUpperCase_newline();
    void capitalization_allLowerCase();
    void capitalization_smallCaps();
    void capitalization_capitalize();
    void longText();
    void widthOfTabs();
    void columnWrapWithTabs();
    void boundingRectForUnsetLineWidth();
    void boundingRectForSetLineWidth();
    void glyphLessItems();
    void justifyTrailingSpaces();
    void layoutWithCustomTabStops();

    // BOBUIextLine stuff
#ifdef BOBUI_BUILD_INTERNAL
    void setNumColumnsWrapAtWordBoundaryOrAnywhere();
    void setNumColumnsWordWrap();
    void smallTextLengthNoWrap();
    void smallTextLengthWordWrap();
    void smallTextLengthWrapAtWordBoundaryOrAnywhere();
    void testLineBreakingAllSpaces();
#endif
    void lineWidthFromBOM();
    void textWidthVsWIdth();
    void textWithSurrogates_bobuibug15679();
    void textWidthWithStackedTextEngine();
    void textWidthWithLineSeparator();
    void cursorInLigatureWithMultipleLines();
    void xToCursorForLigatures();
    void cursorInNonStopChars();
    void nbsp();
    void nbspWithFormat();
    void noModificationOfInputString();
    void superscriptCrash_bobuibug53911();
    void showLineAndParagraphSeparatorsCrash();
    void koreanWordWrap();
    void tooManyDirectionalCharctersCrash_bobuibug77819();
    void softHyphens_data();
    void softHyphens();
    void min_maximumWidth_data();
    void min_maximumWidth();
    void negativeLineWidth();
    void embeddedImageLineHeight();
    void unmatchedShapedSubstring();
    void maximumLayoutWidthInWrappedLayout();

private:
    QFont testFont;
};

// Testing get/set functions
void tst_BOBUIextLayout::getSetCheck()
{
    QString str("Bogus text");
    BOBUIextLayout layout(str);
    layout.beginLayout();
    BOBUIextEngine *engine = layout.engine();
    BOBUIextInlineObject obj1(0, engine);
    // qreal BOBUIextInlineObject::width()
    // void BOBUIextInlineObject::setWidth(qreal)
    obj1.setWidth(0.0);
    QCOMPARE(0.0, obj1.width());
    obj1.setWidth(1.2);
    QVERIFY(1.0 < obj1.width());

    // qreal BOBUIextInlineObject::ascent()
    // void BOBUIextInlineObject::setAscent(qreal)
    obj1.setAscent(0.0);
    QCOMPARE(0.0, obj1.ascent());
    obj1.setAscent(1.2);
    QVERIFY(1.0 < obj1.ascent());

    // qreal BOBUIextInlineObject::descent()
    // void BOBUIextInlineObject::setDescent(qreal)
    obj1.setDescent(0.0);
    QCOMPARE(0.0, obj1.descent());
    obj1.setDescent(1.2);
    QVERIFY(1.0 < obj1.descent());

    BOBUIextLayout obj2;
    // bool BOBUIextLayout::cacheEnabled()
    // void BOBUIextLayout::setCacheEnabled(bool)
    obj2.setCacheEnabled(false);
    QCOMPARE(false, obj2.cacheEnabled());
    obj2.setCacheEnabled(true);
    QCOMPARE(true, obj2.cacheEnabled());
}

#ifdef BOBUI_BUILD_INTERNAL
BOBUI_BEGIN_NAMESPACE
// qfontdatabase.cpp
Q_AUTOTEST_EXPORT void bobui_setBobUIEnableTestFont(bool value);
BOBUI_END_NAMESPACE
#endif

tst_BOBUIextLayout::tst_BOBUIextLayout()
{
#ifdef BOBUI_BUILD_INTERNAL
    bobui_setBobUIEnableTestFont(true);
#endif
}

void tst_BOBUIextLayout::init()
{
    testFont = QFont();
#ifdef BOBUI_BUILD_INTERNAL
    testFont.setFamily("__BobUI__Box__Engine__");
#endif
    testFont.setPixelSize(TESTFONT_SIZE);
    testFont.setWeight(QFont::Normal);
#ifdef BOBUI_BUILD_INTERNAL
    QCOMPARE(QFontMetrics(testFont).horizontalAdvance('a'), testFont.pixelSize());
#endif
}

void tst_BOBUIextLayout::cleanup()
{
    testFont = QFont();
}

void tst_BOBUIextLayout::lineBreaking()
{
#if 0
    struct Breaks {
        const char *utf8;
        uchar breaks[32];
    };
    Breaks brks[] = {
        { "11", { false, 0xff } },
        { "aa", { false, 0xff } },
        { "++", { false, 0xff } },
        { "--", { false, 0xff } },
        { "((", { false, 0xff } },
        { "))", { false, 0xff } },
        { "..", { false, 0xff } },
        { "\"\"", { false, 0xff } },
        { "$$", { false, 0xff } },
        { "!!", { false, 0xff } },
        { "??", { false, 0xff } },
        { ",,", { false, 0xff } },

        { ")()", { true, false, 0xff } },
        { "?!?", { false, false, 0xff } },
        { ".,.", { false, false, 0xff } },
        { "+-+", { false, false, 0xff } },
        { "+=+", { false, false, 0xff } },
        { "+(+", { false, false, 0xff } },
        { "+)+", { false, false, 0xff } },

        { "a b", { false, true, 0xff } },
        { "a(b", { false, false, 0xff } },
        { "a)b", { false, false, 0xff } },
        { "a-b", { false, true, 0xff } },
        { "a.b", { false, false, 0xff } },
        { "a+b", { false, false, 0xff } },
        { "a?b", { false, false, 0xff } },
        { "a!b", { false, false, 0xff } },
        { "a$b", { false, false, 0xff } },
        { "a,b", { false, false, 0xff } },
        { "a/b", { false, false, 0xff } },
        { "1/2", { false, false, 0xff } },
        { "./.", { false, false, 0xff } },
        { ",/,", { false, false, 0xff } },
        { "!/!", { false, false, 0xff } },
        { "\\/\\", { false, false, 0xff } },
        { "1 2", { false, true, 0xff } },
        { "1(2", { false, false, 0xff } },
        { "1)2", { false, false, 0xff } },
        { "1-2", { false, false, 0xff } },
        { "1.2", { false, false, 0xff } },
        { "1+2", { false, false, 0xff } },
        { "1?2", { false, true, 0xff } },
        { "1!2", { false, true, 0xff } },
        { "1$2", { false, false, 0xff } },
        { "1,2", { false, false, 0xff } },
        { "1/2", { false, false, 0xff } },
        { "\330\260\331\216\331\204\331\220\331\203\331\216", { false, false, false, false, false, 0xff } },
        { "\330\247\331\204\331\205 \330\247\331\204\331\205", { false, false, false, true, false, false, 0xff } },
        { "1#2", { false, false, 0xff } },
        { "!#!", { false, false, 0xff } },
        { 0, {} }
    };
    Breaks *b = brks;
    while (b->utf8) {
        QString str = QString::fromUtf8(b->utf8);
        BOBUIextEngine engine(str, QFont());
        const QCharAttributes *attrs = engine.attributes();
        QVERIFY(!attrs[0].lineBreak);
        int i;
        for (i = 0; i < (int)str.length() - 1; ++i) {
            QVERIFY(b->breaks[i] != 0xff);
            if ( attrs[i + 1].lineBreak != (bool)b->breaks[i] ) {
                qDebug("test case \"%s\" failed at char %d; break type: %d", b->utf8, i, attrs[i + 1].lineBreak);
                QCOMPARE( attrs[i + 1].lineBreak, (bool)b->breaks[i] );
            }
        }
        QCOMPARE(b->breaks[i], (uchar)0xff);
        ++b;
    }
#endif
}

#ifdef BOBUI_BUILD_INTERNAL
void tst_BOBUIextLayout::simpleBoundingRect()
{
    /* just check if boundingRect() gives sane values. The text is not broken. */

    QString hello("hello world");

    const int width = hello.size() * testFont.pixelSize();

    BOBUIextLayout layout(hello, testFont);
    layout.beginLayout();

    BOBUIextLine line = layout.createLine();
    line.setLineWidth(width);
    QCOMPARE(line.textLength(), hello.size());
    QCOMPARE(layout.boundingRect(), QRectF(0, 0, width, QFontMetrics(testFont).height()));
}

void tst_BOBUIextLayout::threeLineBoundingRect_data()
{
    BOBUIest::addColumn<QChar>("wordBoundary1");
    BOBUIest::addColumn<QChar>("wordBoundary2");
    BOBUIest::newRow("2x' '") << QChar(' ') << QChar(' ');
    BOBUIest::newRow("2x'\\n'") << QChar('\n') << QChar('\n');
    BOBUIest::newRow("' ' + '\\n'") << QChar(' ') << QChar('\n');
    BOBUIest::newRow("'\\n' + ' '") << QChar('\n') << QChar(' ');
    BOBUIest::newRow("2x'\\t'") << QChar('\t') << QChar('\t');
    BOBUIest::newRow("2xsoft hyphen") << QChar(0xad) << QChar(0xad);
    BOBUIest::newRow("2x'-'") << QChar('-') << QChar('-');
    BOBUIest::newRow("2x'/'") << QChar('/') << QChar('/');
    BOBUIest::newRow("soft hyphen + ' '") << QChar(0xad) << QChar(' ');
    BOBUIest::newRow("soft hyphen + '\\n'") << QChar(0xad) << QChar('\n');
    BOBUIest::newRow("soft hyphen + '-'") << QChar(0xad) << QChar('-');
    BOBUIest::newRow("' ' + soft hyphen") << QChar(' ') << QChar(0xad);
    BOBUIest::newRow("'\\n' + soft hyphen") << QChar('\n') << QChar(0xad);
    BOBUIest::newRow("'-' + soft hyphen") << QChar('-') << QChar(0xad);
}

void tst_BOBUIextLayout::threeLineBoundingRect()
{
    /* stricter check. break text into three lines */
    QFETCH(QChar, wordBoundary1);
    QFETCH(QChar, wordBoundary2);

    QString firstWord("hello");
    QString secondWord("test");
    QString thirdWord("world");
    QString text(firstWord + wordBoundary1 + secondWord + wordBoundary2 + thirdWord);

    int firstLineWidth = firstWord.size() * testFont.pixelSize();
    int secondLineWidth = secondWord.size() * testFont.pixelSize();
    int thirdLineWidth = thirdWord.size() * testFont.pixelSize();
    // Trailing spaces do not count to line width:
    if (!wordBoundary1.isSpace())
        firstLineWidth += testFont.pixelSize();
    if (!wordBoundary2.isSpace())
        secondLineWidth += testFont.pixelSize();
    // But trailing spaces do count to line length:
    const int firstLineLength = firstWord.size() + 1;
    const int secondLineLength = secondWord.size() + 1;
    const int thirdLineLength = thirdWord.size();

    const int longestLine = qMax(firstLineWidth, qMax(secondLineWidth, thirdLineWidth));

    BOBUIextLayout layout(text, testFont);
    layout.beginLayout();

    int pos = 0;
    int y = 0;
    BOBUIextLine line = layout.createLine();
    line.setLineWidth(firstLineWidth);
    line.setPosition(QPoint(0, y));
    QCOMPARE(line.textStart(), pos);
    QCOMPARE(line.textLength(), firstLineLength);
    QCOMPARE(qRound(line.naturalTextWidth()), firstLineWidth);

    pos += line.textLength();
    y += qRound(line.ascent() + line.descent());

    line = layout.createLine();
    line.setLineWidth(secondLineWidth);
    line.setPosition(QPoint(0, y));
    QCOMPARE(line.textStart(), pos);
    QCOMPARE(line.textLength(), secondLineLength);
    QCOMPARE(qRound(line.naturalTextWidth()), secondLineWidth);

    pos += line.textLength();
    y += qRound(line.ascent() + line.descent());

    line = layout.createLine();
    line.setLineWidth(secondLineWidth);
    line.setPosition(QPoint(0, y));
    QCOMPARE(line.textStart(), pos);
    QCOMPARE(line.textLength(), thirdLineLength);
    QCOMPARE(qRound(line.naturalTextWidth()), thirdLineWidth);
    y += qRound(line.ascent() + line.descent());

    QCOMPARE(layout.boundingRect(), QRectF(0, 0, longestLine, y));
}

void tst_BOBUIextLayout::boundingRectWithLongLineAndNoWrap()
{
    QString longString("thisisaverylongstringthatcannotbewrappedatallitjustgoesonandonlikeonebigword");

    const int width = longString.size() * testFont.pixelSize() / 20; // very small widthx

    BOBUIextLayout layout(longString, testFont);
    layout.beginLayout();
    BOBUIextLine line = layout.createLine();
    line.setLineWidth(width);

    QVERIFY(layout.boundingRect().width() >= line.width());
    QCOMPARE(layout.boundingRect().width(), line.naturalTextWidth());
}

void tst_BOBUIextLayout::forcedBreaks()
{
    QString text = "A\n\nB\nC";
    text.replace('\n', QChar::LineSeparator);

    BOBUIextLayout layout(text, testFont);

    layout.beginLayout();

    int pos = 0;

    BOBUIextLine line = layout.createLine();
    line.setLineWidth(0x10000);
    QCOMPARE(line.textStart(), pos);
    QCOMPARE(line.textLength(),2);
    QCOMPARE(qRound(line.naturalTextWidth()),testFont.pixelSize());
    QCOMPARE((int) line.height(), testFont.pixelSize());
    QCOMPARE(line.xToCursor(0), line.textStart());
    pos += line.textLength();

    line = layout.createLine();
    line.setLineWidth(0x10000);
    QCOMPARE(line.textStart(),pos);
    QCOMPARE(line.textLength(),1);
    QCOMPARE(qRound(line.naturalTextWidth()), 0);
    QCOMPARE((int) line.height(), testFont.pixelSize());
    QCOMPARE(line.xToCursor(0), line.textStart());
    pos += line.textLength();

    line = layout.createLine();
    line.setLineWidth(0x10000);
    QCOMPARE(line.textStart(),pos);
    QCOMPARE(line.textLength(),2);
    QCOMPARE(qRound(line.naturalTextWidth()),testFont.pixelSize());
    QCOMPARE(qRound(line.height()), testFont.pixelSize());
    QCOMPARE(line.xToCursor(0), line.textStart());
    pos += line.textLength();

    line = layout.createLine();
    line.setLineWidth(0x10000);
    QCOMPARE(line.textStart(),pos);
    QCOMPARE(line.textLength(),1);
    QCOMPARE(qRound(line.naturalTextWidth()), testFont.pixelSize());
    QCOMPARE((int) line.height(), testFont.pixelSize());
    QCOMPARE(line.xToCursor(0), line.textStart());

    layout.endLayout();
}

void tst_BOBUIextLayout::breakAny()
{
    QString text = "ABCD";

    BOBUIextLayout layout(text, testFont);
    layout.setCacheEnabled(true);
    BOBUIextLine line;

    BOBUIextOption opt;
    opt.setWrapMode(BOBUIextOption::WrapAnywhere);
    layout.setTextOption(opt);
    layout.beginLayout();

    line = layout.createLine();
    line.setLineWidth(testFont.pixelSize() * 2);
    QCOMPARE(line.textStart(), 0);
    QCOMPARE(line.textLength(), 2);

    line = layout.createLine();
    line.setLineWidth(testFont.pixelSize() * 2);
    QCOMPARE(line.textStart(), 2);
    QCOMPARE(line.textLength(), 2);

    line = layout.createLine();
    QVERIFY(!line.isValid());

    layout.endLayout();

    text = "ABCD EFGH";
    layout.setText(text);
    layout.beginLayout();

    line = layout.createLine();
    line.setLineWidth(testFont.pixelSize() * 7);
    QCOMPARE(line.textStart(), 0);
    QCOMPARE(line.textLength(), 7);

    layout.endLayout();
}

void tst_BOBUIextLayout::noWrap()
{
    QString text = "AB CD";

    BOBUIextLayout layout(text, testFont);
    BOBUIextLine line;

    BOBUIextOption opt;
    opt.setWrapMode(BOBUIextOption::NoWrap);
    layout.setTextOption(opt);
    layout.beginLayout();

    line = layout.createLine();
    line.setLineWidth(testFont.pixelSize() * 2);
    QCOMPARE(line.textStart(), 0);
    QCOMPARE(line.textLength(), 5);

    line = layout.createLine();
    QVERIFY(!line.isValid());

    layout.endLayout();
}

void tst_BOBUIextLayout::cursorToXForInlineObjects()
{
    QString text = QStringLiteral("<html><body><img src=\"\" width=\"32\" height=\"32\" /></body></html>");

    BOBUIextDocument document;
    document.setHtml(text);
    QCOMPARE(document.blockCount(), 1);

    // Trigger layout
    {
        QImage img(1, 1, QImage::Format_ARGB32_Premultiplied);
        QPainter p(&img);
        document.drawContents(&p);
    }

    BOBUIextLayout *layout = document.firstBlock().layout();
    QVERIFY(layout != nullptr);
    QCOMPARE(layout->lineCount(), 1);

    BOBUIextLine line = layout->lineAt(0);
    QCOMPARE(line.cursorToX(0), qreal(0));
    QCOMPARE(line.cursorToX(1), qreal(32));
}

void tst_BOBUIextLayout::cursorToXForSetColumns()
{
    BOBUIextLayout lay("abc", testFont);
    lay.setCacheEnabled(true);
    BOBUIextOption o = lay.textOption();
    o.setWrapMode(BOBUIextOption::WrapAnywhere);

    // enable/disable this line for full effect ;)
    o.setAlignment(BobUI::AlignHCenter);

    lay.setTextOption(o);
    lay.beginLayout();
    BOBUIextLine line = lay.createLine();
    line.setNumColumns(1);
    lay.endLayout();
    QCOMPARE(line.cursorToX(0), 0.);
    QCOMPARE(line.cursorToX(1), (qreal) TESTFONT_SIZE);
}

void tst_BOBUIextLayout::cursorToXForTrailingSpaces_data()
{
    qreal width = TESTFONT_SIZE * 4;

    BOBUIest::addColumn<BOBUIextOption::WrapMode>("wrapMode");
    BOBUIest::addColumn<BobUI::LayoutDirection>("textDirection");
    BOBUIest::addColumn<BobUI::AlignmentFlag>("alignment");
    BOBUIest::addColumn<qreal>("cursorAt0");
    BOBUIest::addColumn<qreal>("cursorAt4");
    BOBUIest::addColumn<qreal>("cursorAt6");

    // Aligned left from start of visible characters.
    BOBUIest::newRow("ltr nowrap lalign")
            << BOBUIextOption::NoWrap
            << BobUI::LeftToRight
            << BobUI::AlignLeft
            << qreal(0)
            << width
            << qreal(TESTFONT_SIZE * 6);

    // Aligned left from start of visible characters.
    BOBUIest::newRow("ltr wrap lalign")
            << BOBUIextOption::WrapAnywhere
            << BobUI::LeftToRight
            << BobUI::AlignLeft
            << qreal(0)
            << width
            << width;

    // Aligned right from end of whitespace characters.
    BOBUIest::newRow("ltr nowrap ralign")
            << BOBUIextOption::NoWrap
            << BobUI::LeftToRight
            << BobUI::AlignRight
            << qreal(TESTFONT_SIZE * -2)
            << qreal(TESTFONT_SIZE *  2)
            << width;

    // Aligned right from end of visible characters.
    BOBUIest::newRow("ltr wrap ralign")
            << BOBUIextOption::WrapAnywhere
            << BobUI::LeftToRight
            << BobUI::AlignRight
            << qreal(TESTFONT_SIZE)
            << width
            << width;

    // Aligned center of all characters
    BOBUIest::newRow("ltr nowrap calign")
            << BOBUIextOption::NoWrap
            << BobUI::LeftToRight
            << BobUI::AlignHCenter
            << qreal(TESTFONT_SIZE * -1)
            << qreal(TESTFONT_SIZE *  3)
            << qreal(TESTFONT_SIZE *  5);

    // Aligned center of visible characters
    BOBUIest::newRow("ltr wrap calign")
            << BOBUIextOption::WrapAnywhere
            << BobUI::LeftToRight
            << BobUI::AlignHCenter
            << qreal(TESTFONT_SIZE * 0.5)
            << qreal(width)
            << qreal(width);

    // Aligned right from start of visible characters
    BOBUIest::newRow("rtl nowrap ralign")
            << BOBUIextOption::NoWrap
            << BobUI::RightToLeft
            << BobUI::AlignRight
            << width
            << qreal(0)
            << qreal(TESTFONT_SIZE * -2);

    // Aligned right from start of visible characters
    BOBUIest::newRow("rtl wrap ralign")
            << BOBUIextOption::WrapAnywhere
            << BobUI::RightToLeft
            << BobUI::AlignRight
            << width
            << qreal(0)
            << qreal(0);

    // Aligned left from end of whitespace characters
    BOBUIest::newRow("rtl nowrap lalign")
            << BOBUIextOption::NoWrap
            << BobUI::RightToLeft
            << BobUI::AlignLeft
            << qreal(TESTFONT_SIZE * 6)
            << qreal(TESTFONT_SIZE * 2)
            << qreal(0);

    // Aligned left from end of visible characters
    BOBUIest::newRow("rtl wrap lalign")
            << BOBUIextOption::WrapAnywhere
            << BobUI::RightToLeft
            << BobUI::AlignLeft
            << qreal(TESTFONT_SIZE * 3)
            << qreal(0)
            << qreal(0);

    // Aligned center of all characters
    BOBUIest::newRow("rtl nowrap calign")
            << BOBUIextOption::NoWrap
            << BobUI::RightToLeft
            << BobUI::AlignHCenter
            << qreal(TESTFONT_SIZE *  5)
            << qreal(TESTFONT_SIZE *  1)
            << qreal(TESTFONT_SIZE * -1);

    // Aligned center of visible characters
    BOBUIest::newRow("rtl wrap calign")
            << BOBUIextOption::WrapAnywhere
            << BobUI::RightToLeft
            << BobUI::AlignHCenter
            << qreal(TESTFONT_SIZE * 3.5)
            << qreal(0)
            << qreal(0);
}

void tst_BOBUIextLayout::cursorToXForTrailingSpaces()
{
    QFETCH(BOBUIextOption::WrapMode, wrapMode);
    QFETCH(BobUI::LayoutDirection, textDirection);
    QFETCH(BobUI::AlignmentFlag, alignment);
    QFETCH(qreal, cursorAt0);
    QFETCH(qreal, cursorAt4);
    QFETCH(qreal, cursorAt6);

    BOBUIextLayout layout("%^&   ", testFont);

    BOBUIextOption o = layout.textOption();
    o.setTextDirection(textDirection);
    o.setAlignment(alignment);
    o.setWrapMode(wrapMode);
    layout.setTextOption(o);

    layout.beginLayout();
    BOBUIextLine line = layout.createLine();
    line.setLineWidth(TESTFONT_SIZE * 4);
    layout.endLayout();

    QCOMPARE(line.cursorToX(0), cursorAt0);
    QCOMPARE(line.cursorToX(4), cursorAt4);
    QCOMPARE(line.cursorToX(6), cursorAt6);
}

void tst_BOBUIextLayout::cursorToXInvalidInput()
{
    BOBUIextLayout layout("aaa", testFont);

    layout.beginLayout();
    BOBUIextLine line = layout.createLine();
    line.setLineWidth(5);
    layout.endLayout();

    int cursorPos;

    cursorPos = 0;
    layout.lineAt(0).cursorToX(&cursorPos);
    QCOMPARE(cursorPos, 0);
    cursorPos = -300;
    layout.lineAt(0).cursorToX(&cursorPos);
    QCOMPARE(cursorPos, 0);
    cursorPos = 300;
    layout.lineAt(0).cursorToX(&cursorPos);
    QCOMPARE(cursorPos, 3);
}

void tst_BOBUIextLayout::cursorToXForBidiBoundaries_data()
{
    BOBUIest::addColumn<BobUI::LayoutDirection>("textDirection");
    BOBUIest::addColumn<QString>("text");
    BOBUIest::addColumn<int>("cursorPosition");
    BOBUIest::addColumn<int>("runsToInclude");

    BOBUIest::addRow("LTR, abcشزذabc, 0") << BobUI::LeftToRight << "abcشزذabc"
        << 0 << 0;
    BOBUIest::addRow("RTL, abcشزذabc, 9") << BobUI::RightToLeft << "abcشزذabc"
        << 9 << 1;
    BOBUIest::addRow("LTR, abcشزذabc, 3") << BobUI::LeftToRight << "abcشزذabc"
        << 0 << 0;
    BOBUIest::addRow("RTL, abcشزذabc, 6") << BobUI::RightToLeft << "abcشزذabc"
        << 9 << 1;

    BOBUIest::addRow("LTR, شزذabcشزذ, 0") << BobUI::LeftToRight << "شزذabcشزذ"
        << 0 << 1;
    BOBUIest::addRow("RTL, شزذabcشزذ, 9") << BobUI::RightToLeft << "شزذabcشزذ"
        << 9 << 0;
    BOBUIest::addRow("LTR, شزذabcشزذ, 3") << BobUI::LeftToRight << "شزذabcشزذ"
        << 3 << 1;
    BOBUIest::addRow("RTL, شزذabcشزذ, 3") << BobUI::RightToLeft << "شزذabcشزذ"
        << 3 << 2;
    BOBUIest::addRow("LTR, شزذabcشزذ, 6") << BobUI::LeftToRight << "شزذabcشزذ"
        << 6 << 2;
    BOBUIest::addRow("RTL, شزذabcشزذ, 6") << BobUI::RightToLeft << "شزذabcشزذ"
        << 6 << 1;
}

void tst_BOBUIextLayout::cursorToXForBidiBoundaries()
{
    QFETCH(BobUI::LayoutDirection, textDirection);
    QFETCH(QString, text);
    QFETCH(int, cursorPosition);
    QFETCH(int, runsToInclude);

    BOBUIextOption option;
    option.setTextDirection(textDirection);

    BOBUIextLayout layout(text, testFont);
    layout.setTextOption(option);
    layout.beginLayout();

    {
        BOBUIextLine line = layout.createLine();
        line.setLineWidth(0x10000);
    }
    layout.endLayout();

    BOBUIextLine line = layout.lineAt(0);
    QList<QGlyphRun> glyphRuns = line.glyphRuns(-1,
                                                -1,
                                                BOBUIextLayout::RetrieveStringIndexes
                                                | BOBUIextLayout::RetrieveGlyphIndexes);
    QVERIFY(runsToInclude <= glyphRuns.size());

    std::sort(glyphRuns.begin(), glyphRuns.end(),
              [](const QGlyphRun &first, const QGlyphRun &second) {
                    return first.stringIndexes().first() < second.stringIndexes().first();
                 });

    qreal expectedX = 0.0;
    for (int i = 0; i < runsToInclude; ++i) {
        expectedX += glyphRuns.at(i).boundingRect().width();
    }

    QCOMPARE(line.cursorToX(cursorPosition), expectedX);
}

void tst_BOBUIextLayout::horizontalAlignment_data()
{
    qreal width = TESTFONT_SIZE * 4;

    BOBUIest::addColumn<BOBUIextOption::WrapMode>("wrapMode");
    BOBUIest::addColumn<BobUI::LayoutDirection>("textDirection");
    BOBUIest::addColumn<BobUI::AlignmentFlag>("alignment");
    BOBUIest::addColumn<qreal>("naturalLeft");
    BOBUIest::addColumn<qreal>("naturalRight");

    // Aligned left from start of visible characters.
    BOBUIest::newRow("ltr nowrap lalign")
            << BOBUIextOption::NoWrap
            << BobUI::LeftToRight
            << BobUI::AlignLeft
            << qreal(0)
            << qreal(TESTFONT_SIZE * 6);

    // Aligned left from start of visible characters.
    BOBUIest::newRow("ltr wrap lalign")
            << BOBUIextOption::WrapAnywhere
            << BobUI::LeftToRight
            << BobUI::AlignLeft
            << qreal(0)
            << qreal(TESTFONT_SIZE * 3);

    // Aligned right from end of whitespace characters.
    BOBUIest::newRow("ltr nowrap ralign")
            << BOBUIextOption::NoWrap
            << BobUI::LeftToRight
            << BobUI::AlignRight
            << qreal(TESTFONT_SIZE *  - 2)
            << width;

    // Aligned right from end of visible characters.
    BOBUIest::newRow("ltr wrap ralign")
            << BOBUIextOption::WrapAnywhere
            << BobUI::LeftToRight
            << BobUI::AlignRight
            << qreal(TESTFONT_SIZE)
            << width;

    // Aligned center of all characters
    BOBUIest::newRow("ltr nowrap calign")
            << BOBUIextOption::NoWrap
            << BobUI::LeftToRight
            << BobUI::AlignHCenter
            << qreal(TESTFONT_SIZE * -1)
            << qreal(TESTFONT_SIZE *  5);

    // Aligned center of visible characters
    BOBUIest::newRow("ltr wrap calign")
            << BOBUIextOption::WrapAnywhere
            << BobUI::LeftToRight
            << BobUI::AlignHCenter
            << qreal(TESTFONT_SIZE * 0.5)
            << qreal(TESTFONT_SIZE * 3.5);

    // Aligned right from start of visible characters
    BOBUIest::newRow("rtl nowrap ralign")
            << BOBUIextOption::NoWrap
            << BobUI::RightToLeft
            << BobUI::AlignRight
            << qreal(TESTFONT_SIZE * -2)
            << width;

    // Aligned right from start of visible characters
    BOBUIest::newRow("rtl wrap ralign")
            << BOBUIextOption::WrapAnywhere
            << BobUI::RightToLeft
            << BobUI::AlignRight
            << qreal(TESTFONT_SIZE * 1)
            << width;

    // Aligned left from end of whitespace characters
    BOBUIest::newRow("rtl nowrap lalign")
            << BOBUIextOption::NoWrap
            << BobUI::RightToLeft
            << BobUI::AlignLeft
            << qreal(0)
            << qreal(TESTFONT_SIZE * 6);

    // Aligned left from end of visible characters
    BOBUIest::newRow("rtl wrap lalign")
            << BOBUIextOption::WrapAnywhere
            << BobUI::RightToLeft
            << BobUI::AlignLeft
            << qreal(0)
            << qreal(TESTFONT_SIZE * 3);

    // Aligned center of all characters
    BOBUIest::newRow("rtl nowrap calign")
            << BOBUIextOption::NoWrap
            << BobUI::RightToLeft
            << BobUI::AlignHCenter
            << qreal(TESTFONT_SIZE * -1)
            << qreal(TESTFONT_SIZE *  5);

    // Aligned center of visible characters
    BOBUIest::newRow("rtl wrap calign")
            << BOBUIextOption::WrapAnywhere
            << BobUI::RightToLeft
            << BobUI::AlignHCenter
            << qreal(TESTFONT_SIZE * 0.5)
            << qreal(TESTFONT_SIZE * 3.5);
}

void tst_BOBUIextLayout::horizontalAlignment()
{
    QFETCH(BOBUIextOption::WrapMode, wrapMode);
    QFETCH(BobUI::LayoutDirection, textDirection);
    QFETCH(BobUI::AlignmentFlag, alignment);
    QFETCH(qreal, naturalLeft);
    QFETCH(qreal, naturalRight);

    BOBUIextLayout layout("%^&   ", testFont);

    BOBUIextOption o = layout.textOption();
    o.setTextDirection(textDirection);
    o.setAlignment(alignment);
    o.setWrapMode(wrapMode);
    layout.setTextOption(o);

    layout.beginLayout();
    BOBUIextLine line = layout.createLine();
    line.setLineWidth(TESTFONT_SIZE * 4);
    layout.endLayout();

    QRectF naturalRect = line.naturalTextRect();
    QCOMPARE(naturalRect.left(), naturalLeft);
    QCOMPARE(naturalRect.right(), naturalRight);
}


void tst_BOBUIextLayout::horizontalAlignmentMultiline_data()
{
    qreal width = TESTFONT_SIZE * 8;

    const QString linebreakText = QStringLiteral("^%$&") + QChar(0x2028) + QStringLiteral("^%&*^$");
    QString wrappingText("^%$&^%&*^$");
    QString wrappingWhitespaceText("^%$&        ^%&*^$");

    BOBUIest::addColumn<QString>("text");
    BOBUIest::addColumn<BobUI::LayoutDirection>("textDirection");
    BOBUIest::addColumn<BobUI::AlignmentFlag>("alignment");
    BOBUIest::addColumn<qreal>("firstLeft");
    BOBUIest::addColumn<qreal>("firstRight");
    BOBUIest::addColumn<qreal>("lastLeft");
    BOBUIest::addColumn<qreal>("lastRight");

    BobUI::LayoutDirection textDirection[] = { BobUI::LeftToRight, BobUI::RightToLeft };
    QByteArray textDirectionText [] = { "ltr ", "rtl " };
    for (int i = 0; i < 2; ++i) {
        // Aligned left from start of visible characters.
        BOBUIest::newRow(textDirectionText[i] + "linebreak lalign")
                << linebreakText
                << textDirection[i]
                << BobUI::AlignLeft
                << qreal(0)
                << qreal(TESTFONT_SIZE * 4)
                << qreal(0)
                << qreal(TESTFONT_SIZE * 6);

        // Aligned left from start of visible characters.
        BOBUIest::newRow(textDirectionText[i] + "wrap-text lalign")
                << wrappingText
                << textDirection[i]
                << BobUI::AlignLeft
                << qreal(0)
                << width
                << qreal(0)
                << qreal(TESTFONT_SIZE * 2);

        // Aligned left from start of visible characters.
        BOBUIest::newRow(textDirectionText[i] + "wrap-ws lalign")
                << wrappingWhitespaceText
                << textDirection[i]
                << BobUI::AlignLeft
                << qreal(0)
                << qreal(TESTFONT_SIZE * 4)
                << qreal(0)
                << qreal(TESTFONT_SIZE * 6);

        // Aligned right from start of visible characters.
        BOBUIest::newRow(textDirectionText[i] + "linebreak ralign")
                << linebreakText
                << textDirection[i]
                << BobUI::AlignRight
                << qreal(TESTFONT_SIZE * 4)
                << width
                << qreal(TESTFONT_SIZE * 2)
                << width;

        // Aligned right from start of visible characters.
        BOBUIest::newRow(textDirectionText[i] + "wrap-text ralign")
                << wrappingText
                << textDirection[i]
                << BobUI::AlignRight
                << qreal(0)
                << width
                << qreal(TESTFONT_SIZE * 6)
                << width;

        // Aligned left from start of visible characters.
        BOBUIest::newRow(textDirectionText[i] + "wrap-ws ralign")
                << wrappingWhitespaceText
                << textDirection[i]
                << BobUI::AlignRight
                << qreal(TESTFONT_SIZE * 4)
                << width
                << qreal(TESTFONT_SIZE * 2)
                << width;

        // Aligned center from start of visible characters.
        BOBUIest::newRow(textDirectionText[i] + "linebreak calign")
                << linebreakText
                << textDirection[i]
                << BobUI::AlignCenter
                << qreal(TESTFONT_SIZE * 2)
                << qreal(TESTFONT_SIZE * 6)
                << qreal(TESTFONT_SIZE * 1)
                << qreal(TESTFONT_SIZE * 7);

        // Aligned center from start of visible characters.
        BOBUIest::newRow(textDirectionText[i] + "wrap-text calign")
                << wrappingText
                << textDirection[i]
                << BobUI::AlignCenter
                << qreal(0)
                << width
                << qreal(TESTFONT_SIZE * 3)
                << qreal(TESTFONT_SIZE * 5);

        // Aligned center from start of visible characters.
        BOBUIest::newRow(textDirectionText[i] + "wrap-ws calign")
                << wrappingWhitespaceText
                << textDirection[i]
                << BobUI::AlignCenter
                << qreal(TESTFONT_SIZE * 2)
                << qreal(TESTFONT_SIZE * 6)
                << qreal(TESTFONT_SIZE * 1)
                << qreal(TESTFONT_SIZE * 7);
    }
}

void tst_BOBUIextLayout::horizontalAlignmentMultiline()
{
    QFETCH(QString, text);
    QFETCH(BobUI::LayoutDirection, textDirection);
    QFETCH(BobUI::AlignmentFlag, alignment);
    QFETCH(qreal, firstLeft);
    QFETCH(qreal, firstRight);
    QFETCH(qreal, lastLeft);
    QFETCH(qreal, lastRight);

    BOBUIextLayout layout(text, testFont);

    BOBUIextOption o = layout.textOption();
    o.setTextDirection(textDirection);
    o.setAlignment(alignment);
    o.setWrapMode(BOBUIextOption::WrapAnywhere);
    layout.setTextOption(o);

    layout.beginLayout();
    BOBUIextLine firstLine = layout.createLine();
    BOBUIextLine lastLine;
    for (BOBUIextLine line = firstLine; line.isValid(); line = layout.createLine()) {
        line.setLineWidth(TESTFONT_SIZE * 8);
        lastLine = line;
    }
    layout.endLayout();

    qDebug() << firstLine.textLength() << firstLine.naturalTextRect() << lastLine.naturalTextRect();

    QRectF rect = firstLine.naturalTextRect();
    QCOMPARE(rect.left(), firstLeft);
    QCOMPARE(rect.right(), firstRight);

    rect = lastLine.naturalTextRect();
    QCOMPARE(rect.left(), lastLeft);
    QCOMPARE(rect.right(), lastRight);
}
#endif

void tst_BOBUIextLayout::defaultWordSeparators_data()
{
    BOBUIest::addColumn<QString>("text");
    BOBUIest::addColumn<int>("startPos");
    BOBUIest::addColumn<int>("endPos");

    QString separators(".,:;-<>[](){}=/+%&^*");
    separators += QLatin1String("!?");
    for (int i = 0; i < separators.size(); ++i) {
        BOBUIest::newRow(QString::number(i).toLatin1().data())
            << QString::fromLatin1("abcd") + separators.at(i) + QString::fromLatin1("efgh")
            <<  0 << 4;
    }

    BOBUIest::newRow("nbsp")
            << QString::fromLatin1("abcd") + QString(QChar::Nbsp) + QString::fromLatin1("efgh")
            << 0 << 5;

    BOBUIest::newRow("tab")
        << QString::fromLatin1("abcd") + QString::fromLatin1("\t") + QString::fromLatin1("efgh")
            << 0 << 5;

    BOBUIest::newRow("lineseparator")
            << QString::fromLatin1("abcd") + QString(QChar::LineSeparator) + QString::fromLatin1("efgh")
            << 0 << 5;

    BOBUIest::newRow("empty")
            << QString()
            << 0 << 0;
}

void tst_BOBUIextLayout::defaultWordSeparators()
{
    QFETCH(QString, text);
    QFETCH(int, startPos);
    QFETCH(int, endPos);
    BOBUIextLayout layout(text);

    QCOMPARE(layout.nextCursorPosition(startPos, BOBUIextLayout::SkipWords), endPos);
    QCOMPARE(layout.previousCursorPosition(endPos, BOBUIextLayout::SkipWords), startPos);
}

void tst_BOBUIextLayout::cursorMovementFromInvalidPositions()
{
    int badpos = 10000;

    BOBUIextLayout layout("ABC");

    QCOMPARE(layout.previousCursorPosition(-badpos, BOBUIextLayout::SkipCharacters), -badpos);
    QCOMPARE(layout.nextCursorPosition(-badpos, BOBUIextLayout::SkipCharacters), -badpos);

    QCOMPARE(layout.previousCursorPosition(badpos, BOBUIextLayout::SkipCharacters), badpos);
    QCOMPARE(layout.nextCursorPosition(badpos, BOBUIextLayout::SkipCharacters), badpos);
}

void tst_BOBUIextLayout::cursorMovementInsideSpaces()
{
    BOBUIextLayout layout("ABC            DEF");

    QCOMPARE(layout.previousCursorPosition(6, BOBUIextLayout::SkipWords), 0);
    QCOMPARE(layout.nextCursorPosition(6, BOBUIextLayout::SkipWords), 15);


    BOBUIextLayout layout2("ABC\t\t\t\t\t\t\t\t\t\t\t\tDEF");

    QCOMPARE(layout2.previousCursorPosition(6, BOBUIextLayout::SkipWords), 0);
    QCOMPARE(layout2.nextCursorPosition(6, BOBUIextLayout::SkipWords), 15);
}

void tst_BOBUIextLayout::charWordStopOnLineSeparator()
{
    const QChar lineSeparator(QChar::LineSeparator);
    QString txt;
    txt.append(lineSeparator);
    txt.append(lineSeparator);
    BOBUIextLayout layout(txt);
    BOBUIextEngine *engine = layout.engine();
    const QCharAttributes *attrs = engine->attributes();
    QVERIFY(attrs);
    QVERIFY(attrs[1].graphemeBoundary);
}

#ifdef BOBUI_BUILD_INTERNAL
void tst_BOBUIextLayout::xToCursorAtEndOfLine()
{
    QString text = "FirstLine SecondLine";
    text.replace('\n', QChar::LineSeparator);

    const qreal firstLineWidth = QString("FirstLine").size() * testFont.pixelSize();

    BOBUIextLayout layout(text, testFont);
    layout.setCacheEnabled(true);

    layout.beginLayout();
    BOBUIextLine line = layout.createLine();
    QVERIFY(line.isValid());
    line.setLineWidth(firstLineWidth);
    QVERIFY(layout.createLine().isValid());
    QVERIFY(!layout.createLine().isValid());
    layout.endLayout();

    line = layout.lineAt(0);
    QCOMPARE(line.xToCursor(100000), 9);
    line = layout.lineAt(1);
    QCOMPARE(line.xToCursor(100000), 20);
}
#endif


void tst_BOBUIextLayout::xToCursorForBidiEnds_data()
{
    BOBUIest::addColumn<BobUI::LayoutDirection>("textDirection");
    BOBUIest::addColumn<QString>("text");
    BOBUIest::addColumn<int>("leftPosition");
    BOBUIest::addColumn<int>("rightPosition");

    BOBUIest::addRow("LTR, abcشزذ") << BobUI::LeftToRight << "abcشزذ"
        << 0 << 6;
    BOBUIest::addRow("RTL, abcشزذ") << BobUI::RightToLeft << "abcشزذ"
        << 6 << 0;
    BOBUIest::addRow("LTR, شزذabc") << BobUI::LeftToRight << "شزذabc"
        << 0 << 6;
    BOBUIest::addRow("RTL, شزذabc") << BobUI::RightToLeft << "شزذabc"
        << 6 << 0;
    BOBUIest::addRow("LTR, شزذ123") << BobUI::LeftToRight << "شزذ123"
        << 0 << 6;
    BOBUIest::addRow("RTL, شزذ123") << BobUI::RightToLeft << "شزذ123"
        << 6 << 0;

    BOBUIest::addRow("LTR, abcشزذabc") << BobUI::LeftToRight << "abcشزذabc"
        << 0 << 9;
    BOBUIest::addRow("RTL, abcشزذabc") << BobUI::RightToLeft << "abcشزذabc"
        << 9 << 0;
    BOBUIest::addRow("LTR, شزذabcشزذ") << BobUI::LeftToRight << "شزذabcشزذ"
        << 0 << 9;
    BOBUIest::addRow("RTL, شزذabcشزذ") << BobUI::RightToLeft << "شزذabcشزذ"
        << 9 << 0;
}

void tst_BOBUIextLayout::xToCursorForBidiEnds()
{
    QFETCH(BobUI::LayoutDirection, textDirection);
    QFETCH(QString, text);
    QFETCH(int, leftPosition);
    QFETCH(int, rightPosition);

    BOBUIextOption option;
    option.setTextDirection(textDirection);

    BOBUIextLayout layout(text, testFont);
    layout.setTextOption(option);
    layout.beginLayout();

    BOBUIextLine line = layout.createLine();
    line.setLineWidth(0x10000);

    QCOMPARE(line.xToCursor(0), leftPosition);
    QCOMPARE(line.xToCursor(line.width()), rightPosition);

    layout.endLayout();
}

void tst_BOBUIextLayout::boundingRectTopLeft()
{
    QString text = "FirstLine\nSecondLine";
    text.replace('\n', QChar::LineSeparator);

    BOBUIextLayout layout(text);
    layout.setCacheEnabled(true);

    layout.beginLayout();
    BOBUIextLine firstLine = layout.createLine();
    QVERIFY(firstLine.isValid());
    firstLine.setPosition(QPointF(10, 10));
    BOBUIextLine secondLine = layout.createLine();
    QVERIFY(secondLine.isValid());
    secondLine.setPosition(QPointF(20, 20));
    layout.endLayout();

    QCOMPARE(layout.boundingRect().topLeft(), firstLine.position());
}

void tst_BOBUIextLayout::graphemeBoundaryForSurrogatePairs()
{
    QString txt;
    txt.append(QLatin1Char('a'));
    txt.append(QChar(0xd87e));
    txt.append(QChar(0xdc25));
    txt.append(QLatin1Char('b'));
    BOBUIextLayout layout(txt);
    BOBUIextEngine *engine = layout.engine();
    const QCharAttributes *attrs = engine->attributes();
    QVERIFY(attrs);
    QVERIFY(attrs[0].graphemeBoundary);
    QVERIFY(attrs[1].graphemeBoundary);
    QVERIFY(!attrs[2].graphemeBoundary);
    QVERIFY(attrs[3].graphemeBoundary);
}

void tst_BOBUIextLayout::tabStops()
{
    QString txt("Hello there\tworld");
    BOBUIextLayout layout(txt);
    layout.beginLayout();
    BOBUIextLine line = layout.createLine();

    QVERIFY(line.isValid());
    line.setNumColumns(11);
    QCOMPARE(line.textLength(), 12);

    line = layout.createLine();
    QVERIFY(line.isValid());
    line.setNumColumns(5);
    QCOMPARE(line.textLength(), 5);

    layout.endLayout();
}

void tst_BOBUIextLayout::integerOverflow()
{
    QString txt("Hello world... ");

    for (int i = 0; i < 8; ++i)
        txt += txt;

    BOBUIextLayout layout(txt);
    layout.beginLayout();
    BOBUIextLine line = layout.createLine();

    QVERIFY(line.isValid());
    line.setLineWidth(INT_MAX);
    QCOMPARE(line.textLength(), txt.size());

    QVERIFY(!layout.createLine().isValid());

    layout.endLayout();
}

#ifdef BOBUI_BUILD_INTERNAL
void tst_BOBUIextLayout::setNumColumnsWrapAtWordBoundaryOrAnywhere()
{
    QString txt("This is a small test text");
    BOBUIextLayout layout(txt, testFont);
    layout.setCacheEnabled(true);
    BOBUIextOption option = layout.textOption();
    option.setWrapMode(BOBUIextOption::WrapAtWordBoundaryOrAnywhere);
    layout.setTextOption(option);

    layout.beginLayout();
    BOBUIextLine line1 = layout.createLine();
    QVERIFY(line1.isValid());
    line1.setNumColumns(1);

    // qDebug() << line1.naturalTextWidth();
    QCOMPARE(line1.textLength(), 1);
    QVERIFY(line1.naturalTextWidth() == testFont.pixelSize()); // contains only one character

    BOBUIextLine line2 = layout.createLine();
    QVERIFY(line2.isValid());

    layout.endLayout();
}

void tst_BOBUIextLayout::setNumColumnsWordWrap()
{
    QString txt("This is a small test text");
    BOBUIextLayout layout(txt, testFont);
    layout.setCacheEnabled(true);
    BOBUIextOption option = layout.textOption();
    option.setWrapMode(BOBUIextOption::WordWrap);
    layout.setTextOption(option);

    layout.beginLayout();
    BOBUIextLine line1 = layout.createLine();
    QVERIFY(line1.isValid());
    line1.setNumColumns(1);

    // qDebug() << line1.naturalTextWidth();
    QCOMPARE(line1.textLength(), 5);
    QVERIFY(line1.naturalTextWidth() > 20.0); // contains the whole first word.

    BOBUIextLine line2 = layout.createLine();
    QVERIFY(line2.isValid());

    layout.endLayout();
}

void tst_BOBUIextLayout::smallTextLengthNoWrap()
{
    QString txt("This is a small test text");
    BOBUIextLayout layout(txt, testFont);
    layout.setCacheEnabled(true);
    BOBUIextOption option = layout.textOption();
    option.setWrapMode(BOBUIextOption::NoWrap);
    layout.setTextOption(option);

    /// NoWrap
    layout.beginLayout();
    BOBUIextLine line1 = layout.createLine();
    QVERIFY(line1.isValid());
    line1.setLineWidth(5); // most certainly too short for the word 'This' to fit.

    QCOMPARE(line1.width(), 5.0);
    QVERIFY(line1.naturalTextWidth() > 70); // contains all the text.

    BOBUIextLine line2 = layout.createLine();
    QVERIFY(! line2.isValid());

    layout.endLayout();
}

void tst_BOBUIextLayout::smallTextLengthWordWrap()
{
    QString txt("This is a small test text");
    BOBUIextLayout layout(txt, testFont);
    layout.setCacheEnabled(true);
    BOBUIextOption option = layout.textOption();
    option.setWrapMode(BOBUIextOption::WordWrap);
    layout.setTextOption(option);

    /// WordWrap
    layout.beginLayout();
    BOBUIextLine line1 = layout.createLine();
    QVERIFY(line1.isValid());
    line1.setLineWidth(5); // most certainly too short for the word 'This' to fit.

    QCOMPARE(line1.width(), 5.0);
    QVERIFY(line1.naturalTextWidth() > 20.0); // contains the whole first word.
    QCOMPARE(line1.textLength(), 5);

    BOBUIextLine line2 = layout.createLine();
    QVERIFY(line2.isValid());

    layout.endLayout();
}

void tst_BOBUIextLayout::smallTextLengthWrapAtWordBoundaryOrAnywhere()
{
    QString txt("This is a small test text");
    BOBUIextLayout layout(txt, testFont);
    layout.setCacheEnabled(true);
    BOBUIextOption option = layout.textOption();
    option.setWrapMode(BOBUIextOption::WrapAtWordBoundaryOrAnywhere);
    layout.setTextOption(option);

    layout.beginLayout();
    BOBUIextLine line1 = layout.createLine();
    QVERIFY(line1.isValid());
    line1.setLineWidth(5); // most certainly too short for the word 'This' to fit.

    QCOMPARE(line1.width(), 5.0);
    // qDebug() << line1.naturalTextWidth();
    QCOMPARE(line1.textLength(), 1);
    QVERIFY(line1.naturalTextWidth() == testFont.pixelSize()); // contains just the characters that fit.

    BOBUIextLine line2 = layout.createLine();
    QVERIFY(line2.isValid());

    layout.endLayout();
}

void tst_BOBUIextLayout::testDefaultTabs()
{
    BOBUIextLayout layout("Foo\tBar\ta slightly longer text\tend.", testFont);

    QFont font = layout.font();
    QFontPrivate *fd = QFontPrivate::get(font);
    qreal dpiScale = qreal(fd->dpi) / qreal(bobui_defaultDpiY());
    if (!qFuzzyCompare(dpiScale, 1.0))
        QSKIP("Test logic does not work when tabs are scaled by dpi");

    layout.setCacheEnabled(true);
    layout.beginLayout();
    BOBUIextLine line = layout.createLine();
    line.setLineWidth(1000);
    layout.endLayout();

    //qDebug() << "After the tab: " << line.cursorToX(4);
    QCOMPARE(line.cursorToX(4), 80.); // default tab is 80
    QCOMPARE(line.cursorToX(8), 160.);
    QCOMPARE(line.cursorToX(31), 480.);

    BOBUIextOption option = layout.textOption();
    option.setTabStopDistance(90);
    layout.setTextOption(option);
    layout.beginLayout();
    line = layout.createLine();
    line.setLineWidth(1000);
    layout.endLayout();

    QCOMPARE(line.cursorToX(4), 90.);
    QCOMPARE(line.cursorToX(8), 180.);
    QCOMPARE(line.cursorToX(31), 450.);

    QList<BOBUIextOption::Tab> tabs;
    BOBUIextOption::Tab tab;
    tab.position = 110; // set one tab to 110, but since the rest is unset they will be at the normal interval again.
    tabs.append(tab);
    option.setTabs(tabs);
    layout.setTextOption(option);
    layout.beginLayout();
    line = layout.createLine();
    line.setLineWidth(1000);
    layout.endLayout();

    QCOMPARE(line.cursorToX(4), 110.);
    QCOMPARE(line.cursorToX(8), 180.);
    QCOMPARE(line.cursorToX(31), 450.);
}

void tst_BOBUIextLayout::testTabs()
{
    BOBUIextLayout layout("Foo\tBar.", testFont);

    QFont font = layout.font();
    QFontPrivate *fd = QFontPrivate::get(font);
    qreal dpiScale = qreal(fd->dpi) / qreal(bobui_defaultDpiY());
    if (!qFuzzyCompare(dpiScale, 1.0))
        QSKIP("Test logic does not work when tabs are scaled by dpi");

    layout.setCacheEnabled(true);
    BOBUIextOption option = layout.textOption();
    option.setTabStopDistance(150);
    layout.setTextOption(option);

    layout.beginLayout();
    BOBUIextLine line = layout.createLine();
    line.setLineWidth(200.);
    layout.endLayout();

    QVERIFY(line.naturalTextWidth() > 150);
    QCOMPARE(line.cursorToX(4), 150.);
}

void tst_BOBUIextLayout::testMultilineTab()
{
    BOBUIextLayout layout("Lorem ipsum dolor sit\tBar.", testFont);

    QFont font = layout.font();
    QFontPrivate *fd = QFontPrivate::get(font);
    qreal dpiScale = qreal(fd->dpi) / qreal(bobui_defaultDpiY());
    if (!qFuzzyCompare(dpiScale, 1.0))
        QSKIP("Test logic does not work when tabs are scaled by dpi");

    layout.setCacheEnabled(true);
    // test if this works on the second line.
    layout.beginLayout();
    BOBUIextLine line = layout.createLine();
    line.setLineWidth(220.); // moves 'sit' to next line.
    line = layout.createLine();
    line.setLineWidth(220.);
    layout.endLayout();


    QCOMPARE(line.cursorToX(22), 80.);
}

void tst_BOBUIextLayout::testMultiTab()
{
    BOBUIextLayout layout("Foo\t\t\tBar.", testFont);

    QFont font = layout.font();
    QFontPrivate *fd = QFontPrivate::get(font);
    qreal dpiScale = qreal(fd->dpi) / qreal(bobui_defaultDpiY());
    if (!qFuzzyCompare(dpiScale, 1.0))
        QSKIP("Test logic does not work when tabs are scaled by dpi");

    layout.setCacheEnabled(true);
    layout.beginLayout();
    BOBUIextLine line = layout.createLine();
    line.setLineWidth(1000.);
    layout.endLayout();

    QCOMPARE(line.cursorToX(6), 80. * 3);
}

void tst_BOBUIextLayout::testTabsInAlignedParag()
{
    BOBUIextLayout layout("Foo\tsome more words", testFont);
    layout.setCacheEnabled(true);

    QFont font = layout.font();
    QFontPrivate *fd = QFontPrivate::get(font);
    qreal dpiScale = qreal(fd->dpi) / qreal(bobui_defaultDpiY());
    if (!qFuzzyCompare(dpiScale, 1.0))
        QSKIP("Test logic does not work when tabs are scaled by dpi");

    BOBUIextOption option = layout.textOption();
    // right
    option.setAlignment(BobUI::AlignRight);
    layout.setTextOption(option);

    layout.beginLayout();
    BOBUIextLine line = layout.createLine();
    line.setLineWidth(300.);
    layout.endLayout();

    const qreal textWidth = 80 + 15 * TESTFONT_SIZE; // 15 chars right of the tab
    QCOMPARE(line.naturalTextWidth(), textWidth);
    QCOMPARE(line.cursorToX(0), 300. - textWidth);

    // centered
    option.setAlignment(BobUI::AlignCenter);
    layout.setTextOption(option);

    layout.beginLayout();
    line = layout.createLine();
    line.setLineWidth(300.);
    layout.endLayout();

    QCOMPARE(line.naturalTextWidth(), textWidth);
    QCOMPARE(line.cursorToX(0), (300. - textWidth) / 2.);

    // justified
    option.setAlignment(BobUI::AlignJustify);
    layout.setTextOption(option);

    layout.beginLayout();
    line = layout.createLine();
    line.setLineWidth(textWidth - 10); // make the last word slip to the next line so justification actually happens.
    layout.endLayout();

    QCOMPARE(line.cursorToX(0), 0.);
    QCOMPARE(line.cursorToX(4), 80.);

    //BOBUIextLayout layout2("Foo\tUt wisi enim ad minim veniam, quis nostrud exerci tation ullamcorper suscipit lobortis", testFont); // means it will be more then one line long.
}

void tst_BOBUIextLayout::testRightTab()
{
    BOBUIextLayout layout("Foo\tLorem ipsum te sit\tBar baz\tText\tEnd", testFont);
    /*                     ^ a                 ^ b      ^ c   ^ d
     a = 4, b = 22, c = 30, d = 35 (position)

     I expect the output to be:
        Foo Lorem ipsum te
        sit      Bar Baz
        Text         End

     a) tab replaced with a single space due to the text not fitting before the tab.
     b) tab takes space so the text until the 3th tab fits to the tab pos.
     c) tab is after last tab (both auto and defined) and thus moves text to start of next line.
     d) tab takes space so text until enter fits to tab pos.
    */
    layout.setCacheEnabled(true);

    QFont font = layout.font();
    QFontPrivate *fd = QFontPrivate::get(font);
    qreal dpiScale = qreal(fd->dpi) / qreal(bobui_defaultDpiY());
    if (!qFuzzyCompare(dpiScale, 1.0))
        QSKIP("Test logic does not work when tabs are scaled by dpi");

    BOBUIextOption option = layout.textOption();
    QList<BOBUIextOption::Tab> tabs;
    BOBUIextOption::Tab tab;
    tab.type = BOBUIextOption::RightTab;
    tab.position = 190; // which means only 15(.8) chars of our test font fit left of it.
    tabs.append(tab);
    option.setTabs(tabs);
    layout.setTextOption(option);

    layout.beginLayout();
    BOBUIextLine line1 = layout.createLine();
    line1.setLineWidth(220.);
    // qDebug() << "=====line 2";
    BOBUIextLine line2 = layout.createLine();
    QVERIFY(line2.isValid());
    line2.setLineWidth(220.);
    // qDebug() << "=====line 3";
    BOBUIextLine line3 = layout.createLine();
    QVERIFY(line3.isValid());
    line3.setLineWidth(220.);
    // qDebug() << "=====line 4";
    BOBUIextLine line4 = layout.createLine();
    QVERIFY(! line4.isValid());
    layout.endLayout();
    // qDebug() << "--------";

    QCOMPARE(line1.cursorToX(4), 3. * TESTFONT_SIZE ); // a
    QCOMPARE(line1.textLength(), 19);
    QCOMPARE(line2.cursorToX(23), 190. - 7. * TESTFONT_SIZE); // b
    QCOMPARE(line2.textLength(), 12);
    QCOMPARE(line3.cursorToX(31), 0.); // c
    QCOMPARE(line3.cursorToX(36), 190 - 3. * TESTFONT_SIZE); // d
}

void tst_BOBUIextLayout::testCenteredTab()
{
    BOBUIextLayout layout("Foo\tBar", testFont);

    QFont font = layout.font();
    QFontPrivate *fd = QFontPrivate::get(font);
    qreal dpiScale = qreal(fd->dpi) / qreal(bobui_defaultDpiY());
    if (!qFuzzyCompare(dpiScale, 1.0))
        QSKIP("Test logic does not work when tabs are scaled by dpi");

    layout.setCacheEnabled(true);
    // test if centering the tab works.  We expect the center of 'Bar.' to be at the tab point.
    BOBUIextOption option = layout.textOption();
    QList<BOBUIextOption::Tab> tabs;
    BOBUIextOption::Tab tab(150, BOBUIextOption::CenterTab);
    tabs.append(tab);
    option.setTabs(tabs);
    layout.setTextOption(option);

    layout.beginLayout();
    BOBUIextLine line = layout.createLine();
    line.setLineWidth(200.);
    layout.endLayout();

    const qreal wordLength = 3 * TESTFONT_SIZE; // the length of 'Bar'
    QCOMPARE(line.cursorToX(4), 150 - wordLength / 2.);
}

void tst_BOBUIextLayout::testDelimiterTab()
{
    BOBUIextLayout layout("Foo\tBar. Barrabas", testFont);

    QFont font = layout.font();
    QFontPrivate *fd = QFontPrivate::get(font);
    qreal dpiScale = qreal(fd->dpi) / qreal(bobui_defaultDpiY());
    if (!qFuzzyCompare(dpiScale, 1.0))
        QSKIP("Test logic does not work when tabs are scaled by dpi");

    layout.setCacheEnabled(true);
    // try the different delimiter characters to see if the alignment works there.
    BOBUIextOption option = layout.textOption();
    QList<BOBUIextOption::Tab> tabs;
    BOBUIextOption::Tab tab(100, BOBUIextOption::DelimiterTab, QChar('.'));
    tabs.append(tab);
    option.setTabs(tabs);
    layout.setTextOption(option);

    layout.beginLayout();
    BOBUIextLine line = layout.createLine();
    line.setLineWidth(200.);
    layout.endLayout();

    const qreal distanceBeforeTab = 3.5 * TESTFONT_SIZE; // the length of 'bar' and half the width of the dot.
    QCOMPARE(line.cursorToX(4), 100 - distanceBeforeTab);
}

void tst_BOBUIextLayout::testLineBreakingAllSpaces()
{
    BOBUIextLayout layout("                    123", testFont); // thats 20 spaces
    layout.setCacheEnabled(true);
    const qreal firstLineWidth = 17 * TESTFONT_SIZE;
    layout.beginLayout();
    BOBUIextLine line1 = layout.createLine();
    line1.setLineWidth(firstLineWidth);
    BOBUIextLine line2 = layout.createLine();
    line2.setLineWidth(1000); // the rest
    layout.endLayout();
    QCOMPARE(line1.width(), firstLineWidth);
    QCOMPARE(line1.naturalTextWidth(), 0.); // spaces don't take space
    QCOMPARE(line1.textLength(), 20);
    QCOMPARE(line2.textLength(), 3);
    QCOMPARE(line2.naturalTextWidth(), 3. * TESTFONT_SIZE);
}

void tst_BOBUIextLayout::tabsForRtl()
{
    QString word(QChar(0x5e9)); // a hebrew character
    word = word + word + word;  // 3 hebrew characters ;)

    BOBUIextLayout layout(word +'\t'+ word +'\t'+ word +'\t'+ word, testFont);
//BOBUIextLayout layout(word +' '+ word +' '+ word +' '+ word, testFont);// tester ;)
    /*                             ^ a         ^ b         ^ c
     a = 4, b = 8, c = 12, d = 16 (position)

     a) Left tab in RTL is a righ tab; so a is at width - 80
     b) Like a
     c) right tab on RTL is a left tab; so its at width - 240
     d) center tab is still a centered tab.
    */
    layout.setCacheEnabled(true);

    QFont font = layout.font();
    QFontPrivate *fd = QFontPrivate::get(font);
    qreal dpiScale = qreal(fd->dpi) / qreal(bobui_defaultDpiY());
    if (!qFuzzyCompare(dpiScale, 1.0))
        QSKIP("Test logic does not work when tabs are scaled by dpi");

    BOBUIextOption option = layout.textOption();
    QList<BOBUIextOption::Tab> tabs;
    BOBUIextOption::Tab tab;
    tab.position = 80;
    tabs.append(tab);
    tab.position = 160;
    tabs.append(tab);
    tab.position = 240;
    tab.type = BOBUIextOption::RightTab;
    tabs.append(tab);
    option.setTabs(tabs);
    option.setTextDirection(BobUI::RightToLeft);
    option.setAlignment(BobUI::AlignRight);
    layout.setTextOption(option);

    layout.beginLayout();
    BOBUIextLine line = layout.createLine();
    const qreal WIDTH = 400.;
    line.setLineWidth(WIDTH);
    layout.endLayout();

//qDebug() << "layout ended --------------";

    QCOMPARE(line.cursorToX(0), WIDTH);
    QCOMPARE(line.cursorToX(1), WIDTH - TESTFONT_SIZE); // check its right-aligned
    QCOMPARE(line.cursorToX(4), WIDTH - 80 + 3 * TESTFONT_SIZE);
    QCOMPARE(line.cursorToX(8), WIDTH - 160 + 3 * TESTFONT_SIZE);
    QCOMPARE(line.cursorToX(12), WIDTH - 240);
}

BOBUI_BEGIN_NAMESPACE
Q_GUI_EXPORT int bobui_defaultDpiY();
BOBUI_END_NAMESPACE

void tst_BOBUIextLayout::testTabDPIScale()
{
    class MyPaintDevice : public QPaintDevice {
        QPaintEngine *paintEngine () const override { return 0; }
        int metric (QPaintDevice::PaintDeviceMetric metric) const override {
            switch(metric) {
            case QPaintDevice::PdmWidth:
            case QPaintDevice::PdmHeight:
            case QPaintDevice::PdmWidthMM:
            case QPaintDevice::PdmHeightMM:
            case QPaintDevice::PdmNumColors:
                return INT_MAX;
            case QPaintDevice::PdmDepth:
                return 32;
            case QPaintDevice::PdmDpiX:
            case QPaintDevice::PdmDpiY:
            case QPaintDevice::PdmPhysicalDpiX:
            case QPaintDevice::PdmPhysicalDpiY:
                return 72;
            case QPaintDevice::PdmDevicePixelRatio:
            case QPaintDevice::PdmDevicePixelRatioScaled:
            case QPaintDevice::PdmDevicePixelRatioF_EncodedA:
            case QPaintDevice::PdmDevicePixelRatioF_EncodedB:
                ; // fall through
            }
            return 0;
        }
    };

    MyPaintDevice pd;

    BOBUIextLayout layout("text1\ttext2\ttext3\tend", testFont, &pd);
    layout.setCacheEnabled(true);

    BOBUIextOption option = layout.textOption();
    QList<BOBUIextOption::Tab> tabs;
    BOBUIextOption::Tab tab;
    tab.position = 300;
    tabs.append(tab);

    tab.position = 600;
    tab.type = BOBUIextOption::RightTab;
    tabs.append(tab);

    tab.position = 800;
    tab.type = BOBUIextOption::CenterTab;
    tabs.append(tab);
    option.setTabs(tabs);
    layout.setTextOption(option);

    layout.beginLayout();
    BOBUIextLine line = layout.createLine();
    line.setLineWidth(1500.);
    layout.endLayout();
    QCOMPARE(line.cursorToX(0), 0.);
    QCOMPARE(line.cursorToX(1), (double) TESTFONT_SIZE); // check that the font does not resize
    qreal scale = 72 / (qreal) bobui_defaultDpiY();
    // lets do the transformation of deminishing resolution that QFixed has as effect.
    int fixedScale = (int)( scale * qreal(64)); // into a QFixed
    scale = ((qreal)fixedScale)/(qreal)64;      // and out of a QFixed

    QCOMPARE(line.cursorToX(6),  tabs.at(0).position * scale);
    QCOMPARE(line.cursorToX(12), tabs.at(1).position * scale - TESTFONT_SIZE * 5);
    QCOMPARE(line.cursorToX(18), tabs.at(2).position * scale - TESTFONT_SIZE * 3 / 2.0);
}
#endif

void tst_BOBUIextLayout::tabHeight()
{
    BOBUIextLayout layout("\t");
    layout.setCacheEnabled(true);
    layout.beginLayout();
    BOBUIextLine line = layout.createLine();
    layout.endLayout();

    QCOMPARE(qRound(line.ascent()), QFontMetrics(layout.font()).ascent());
    QCOMPARE(qRound(line.descent()), QFontMetrics(layout.font()).descent());
}

void tst_BOBUIextLayout::capitalization_allUpperCase()
{
    QFont font;
    font.setCapitalization(QFont::AllUppercase);
    BOBUIextLayout layout("Test", font);
    layout.setCacheEnabled(true);
    layout.beginLayout();
    layout.createLine();
    layout.endLayout();

    BOBUIextEngine *engine = layout.engine();
    engine->itemize();
    QCOMPARE(engine->layoutData->items.size(), 1);
    QCOMPARE(engine->layoutData->items.at(0).analysis.flags, ushort(QScriptAnalysis::Uppercase));
}

void tst_BOBUIextLayout::capitalization_allUpperCase_newline()
{
    QFont font;
    font.setCapitalization(QFont::AllUppercase);

    QString tmp = "hello\nworld!";
    tmp.replace(QLatin1Char('\n'), QChar::LineSeparator);

    BOBUIextLayout layout(tmp, font);
    layout.setCacheEnabled(true);
    layout.beginLayout();
    layout.createLine();
    layout.endLayout();

    BOBUIextEngine *engine = layout.engine();
    engine->itemize();
    QCOMPARE(engine->layoutData->items.size(), 3);
    QCOMPARE(engine->layoutData->items.at(0).analysis.flags, ushort(QScriptAnalysis::Uppercase));
    QCOMPARE(engine->layoutData->items.at(1).analysis.flags, ushort(QScriptAnalysis::LineOrParagraphSeparator));
    QCOMPARE(engine->layoutData->items.at(2).analysis.flags, ushort(QScriptAnalysis::Uppercase));
}

void tst_BOBUIextLayout::capitalization_allLowerCase()
{
    QFont font;
    font.setCapitalization(QFont::AllLowercase);
    BOBUIextLayout layout("Test", font);
    layout.setCacheEnabled(true);
    layout.beginLayout();
    layout.createLine();
    layout.endLayout();

    BOBUIextEngine *engine = layout.engine();
    engine->itemize();
    QCOMPARE(engine->layoutData->items.size(), 1);
    QCOMPARE(engine->layoutData->items.at(0).analysis.flags, ushort(QScriptAnalysis::Lowercase));
}

void tst_BOBUIextLayout::capitalization_smallCaps()
{
    QFont font;
    font.setCapitalization(QFont::SmallCaps);
    BOBUIextLayout layout("Test", font);
    layout.setCacheEnabled(true);
    layout.beginLayout();
    layout.createLine();
    layout.endLayout();

    BOBUIextEngine *engine = layout.engine();
    engine->itemize();
    QCOMPARE(engine->layoutData->items.size(), 2);
    QCOMPARE(engine->layoutData->items.at(0).analysis.flags, ushort(QScriptAnalysis::None));
    QCOMPARE(engine->layoutData->items.at(1).analysis.flags, ushort(QScriptAnalysis::SmallCaps));
}

void tst_BOBUIextLayout::capitalization_capitalize()
{
    QFont font;
    font.setCapitalization(QFont::Capitalize);
    BOBUIextLayout layout("hello\tworld", font);
    layout.setCacheEnabled(true);
    layout.beginLayout();
    layout.createLine();
    layout.endLayout();

    BOBUIextEngine *engine = layout.engine();
    engine->itemize();
    QCOMPARE(engine->layoutData->items.size(), 5);
    QCOMPARE(engine->layoutData->items.at(0).analysis.flags, ushort(QScriptAnalysis::Uppercase));
    QCOMPARE(engine->layoutData->items.at(1).analysis.flags, ushort(QScriptAnalysis::None));
    QCOMPARE(engine->layoutData->items.at(2).analysis.flags, ushort(QScriptAnalysis::Tab));
    QCOMPARE(engine->layoutData->items.at(3).analysis.flags, ushort(QScriptAnalysis::Uppercase));
    QCOMPARE(engine->layoutData->items.at(4).analysis.flags, ushort(QScriptAnalysis::None));
}

void tst_BOBUIextLayout::longText()
{
    QString longText(128000, 'a');

    {
        BOBUIextLayout layout(longText);
        layout.setCacheEnabled(true);
        layout.beginLayout();
        BOBUIextLine line = layout.createLine();
        layout.endLayout();
        QVERIFY(line.isValid());
        QVERIFY(line.cursorToX(line.textLength() - 1) > 0);
    }

    for (int cap = QFont::MixedCase; cap < QFont::Capitalize + 1; ++cap) {
        QFont f;
        f.setCapitalization(QFont::Capitalization(cap));
        BOBUIextLayout layout(longText, f);
        layout.setCacheEnabled(true);
        layout.beginLayout();
        BOBUIextLine line = layout.createLine();
        layout.endLayout();
        QVERIFY(line.isValid());
        QVERIFY(line.cursorToX(line.textLength() - 1) > 0);
    }

    {
        BOBUIextLayout layout(longText);
        layout.setCacheEnabled(true);
        layout.setFlags(BobUI::TextForceLeftToRight);
        layout.beginLayout();
        BOBUIextLine line = layout.createLine();
        layout.endLayout();
        QVERIFY(line.isValid());
        QVERIFY(line.cursorToX(line.textLength() - 1) > 0);
    }

    {
        BOBUIextLayout layout(longText);
        layout.setCacheEnabled(true);
        layout.setFlags(BobUI::TextForceRightToLeft);
        layout.beginLayout();
        BOBUIextLine line = layout.createLine();
        layout.endLayout();
        QVERIFY(line.isValid());
        QVERIFY(line.cursorToX(line.textLength() - 1) > 0);
    }

    {
        BOBUIextLayout layout(QString("BobUI rocks! ").repeated(200000));
        layout.setCacheEnabled(true);
        layout.beginLayout();
        forever {
            BOBUIextLine line = layout.createLine();
            if (!line.isValid())
                break;
        }
        layout.endLayout();
        QFontMetricsF fm(layout.font());
        QVERIFY(layout.maximumWidth() - fm.horizontalAdvance(' ') <= QFIXED_MAX);
    }

    {
        BOBUIextLayout layout(QString("AAAAAAAA").repeated(200000));
        layout.setCacheEnabled(true);
        layout.beginLayout();
        forever {
            BOBUIextLine line = layout.createLine();
            if (!line.isValid())
                break;
        }
        layout.endLayout();
        QFontMetricsF fm(layout.font());
        QVERIFY(layout.maximumWidth() - fm.horizontalAdvance('A') <= QFIXED_MAX);
    }
}

void tst_BOBUIextLayout::widthOfTabs()
{
    BOBUIextEngine engine;
    engine.text = "ddd\t\t";
    engine.ignoreBidi = true;
    engine.itemize();
    QCOMPARE(qRound(engine.width(0, 5)), qRound(engine.boundingBox(0, 5).width));
}

void tst_BOBUIextLayout::columnWrapWithTabs()
{
    BOBUIextLayout textLayout;
    {
        BOBUIextOption textOption;
        textOption.setWrapMode(BOBUIextOption::WordWrap);
        textLayout.setTextOption(textOption);
    }

    // Make sure string with spaces does not break
    {
        QString text = "Foo bar foo bar foo bar";
        textLayout.setText(text);

        textLayout.beginLayout();
        BOBUIextLine line = textLayout.createLine();
        line.setNumColumns(30);
        QCOMPARE(line.textLength(), text.size());
        textLayout.endLayout();
    }

    // Make sure string with tabs breaks
    {
        QString text = "Foo\tbar\tfoo\tbar\tfoo\tbar";
        textLayout.setText(text);
        textLayout.beginLayout();
        BOBUIextLine line = textLayout.createLine();
        line.setNumColumns(30);
        QVERIFY(line.textLength() < text.size());
        textLayout.endLayout();
    }

}

void tst_BOBUIextLayout::boundingRectForUnsetLineWidth()
{
    BOBUIextLayout layout("FOOBAR");
    layout.setCacheEnabled(true);
    layout.beginLayout();
    BOBUIextLine line = layout.createLine();
    layout.endLayout();

    QCOMPARE(layout.boundingRect().width(), line.naturalTextWidth());
}

void tst_BOBUIextLayout::boundingRectForSetLineWidth()
{
    BOBUIextLayout layout("FOOBAR");
    layout.setCacheEnabled(true);
    layout.beginLayout();
    BOBUIextLine line = layout.createLine();
    line.setLineWidth(QFIXED_MAX - 1);
    layout.endLayout();

    QCOMPARE(layout.boundingRect().width(), qreal(QFIXED_MAX - 1));
}

void tst_BOBUIextLayout::lineWidthFromBOM()
{
    const QString string(QChar(0xfeff)); // BYTE ORDER MARK
    BOBUIextLayout layout(string);
    layout.beginLayout();
    BOBUIextLine line = layout.createLine();
    line.setLineWidth(INT_MAX / 256);
    layout.endLayout();

    // Don't spin into an infinite loop
 }

void tst_BOBUIextLayout::glyphLessItems()
{
    {
        BOBUIextLayout layout;
        layout.setText("\t\t");
        layout.beginLayout();
        layout.createLine();
        layout.endLayout();
    }

    {
        BOBUIextLayout layout;
        layout.setText(QString::fromLatin1("AA") + QChar(QChar::LineSeparator));
        layout.beginLayout();
        layout.createLine();
        layout.endLayout();
    }
}

void tst_BOBUIextLayout::textWidthVsWIdth()
{
    BOBUIextLayout layout;
    layout.setCacheEnabled(true);
    BOBUIextOption opt;
    opt.setWrapMode(BOBUIextOption::WrapAnywhere);
#if defined(Q_OS_WIN)
    layout.setFont(QFont(QString::fromLatin1("Arial")));
#endif
    layout.setTextOption(opt);
    layout.setText(QString::fromLatin1(
                       "g++ -c -m64 -pipe -g -fvisibility=hidden -fvisibility-inlines-hidden -Wall -W -D_REENTRANT -fPIC -DCORE_LIBRARY -DIDE_LIBRARY_BASENAME=\"lib\" -DWITH_TESTS "
                       "-DBOBUI_NO_CAST_TO_ASCII -DBOBUI_USE_FAST_OPERATOR_PLUS -DBOBUI_USE_FAST_CONCATENATION -DBOBUI_PLUGIN -DBOBUI_TESTLIB_LIB -DBOBUI_SCRIPT_LIB -DBOBUI_SVG_LIB -DBOBUI_SQL_LIB -DBOBUI_XM"
                       "L_LIB -DBOBUI_GUI_LIB -DBOBUI_NETWORK_LIB -DBOBUI_CORE_LIB -DBOBUI_SHARED -I../../../../bobui-qml/mkspecs/linux-g++-64 -I. -I../../../../bobui-qml/include/BobUICore -I../../../."
                       "./bobui-qml/include/BobUINetwork -I../../../../bobui-qml/include/BobUIGui -I../../../../bobui-qml/include/BobUIXml -I../../../../bobui-qml/include/BobUISql -I../../../../bobui-qml/inc"
                       "lude/BobUISvg -I../../../../bobui-qml/include/BobUIScript -I../../../../bobui-qml/include/BobUITest -I../../../../bobui-qml/include -I../../../../bobui-qml/include/BobUIHelp -I../."
                       "./libs -I/home/ettrich/dev/creator/tools -I../../plugins -I../../shared/scriptwrapper -I../../libs/3rdparty/botan/build -Idialogs -Iactionmanager -Ieditorma"
                       "nager -Iprogressmanager -Iscriptmanager -I.moc/debug-shared -I.uic -o .obj/debug-shared/sidebar.o sidebar.cpp"));

    // The naturalTextWidth includes right bearing, but should never be LARGER than line width if
    // there is space for at least one character. Unfortunately that assumption may not hold if the
    // font engine fails to report an accurate minimum right bearing for the font, eg. when the
    // minimum right bearing reported by the font engine doesn't cover all the glyphs in the font.
    // The result is that this test may fail in some cases. We should fix this by running the test
    // with a font that we know have no suprising right bearings. See bobuiextlayout.cpp for details.
    QFontMetricsF fontMetrics(layout.font());
    QSet<char16_t> checked;
    qreal minimumRightBearing = 0.0;
    for (int i = 0; i < layout.text().size(); ++i) {
        QChar c = layout.text().at(i);
        if (!checked.contains(c.unicode())) {
            qreal rightBearing = fontMetrics.rightBearing(c);
            if (rightBearing < minimumRightBearing)
                minimumRightBearing = rightBearing;
            checked.insert(c.unicode());
        }
    }
    if (minimumRightBearing < fontMetrics.minRightBearing())
        QSKIP("Font reports invalid minimum right bearing, and can't be used for this test.");

    for (int width = 100; width < 1000; ++width) {
        layout.beginLayout();
        BOBUIextLine line = layout.createLine();
        line.setLineWidth(width);
        layout.endLayout();

        qreal textWidthIsLargerBy = qMax(qreal(0), line.naturalTextWidth() - line.width());
        qreal thisMustBeZero = 0;
        QCOMPARE(textWidthIsLargerBy, thisMustBeZero);
    }
}

void tst_BOBUIextLayout::textWithSurrogates_bobuibug15679()
{
    QString str = QString::fromUtf8("🀀a🀀");
    BOBUIextLayout layout(str);
    layout.setCacheEnabled(true);
    layout.beginLayout();
    BOBUIextLine line = layout.createLine();
    layout.endLayout();

    qreal x[6];
    for (int i = 0; i < 6; i++)
        x[i] = line.cursorToX(i);

    // If the first and third character are using the same
    // font, they must have the same advance (since they
    // are surrogate pairs, we need to add two for each
    // character)
    QCOMPARE(x[2] - x[0], x[5] - x[3]);
}

void tst_BOBUIextLayout::textWidthWithStackedTextEngine()
{
    QString text = QString::fromUtf8("คลิก ถัดไป เพื่อดำเนินการต่อ");

    BOBUIextLayout layout(text);
    layout.setCacheEnabled(true);
    layout.beginLayout();
    BOBUIextLine line = layout.createLine();
    layout.endLayout();

    QStackTextEngine layout2(text, layout.font());

    QVERIFY(layout2.width(0, text.size()).toReal() >= line.naturalTextWidth());
}

void tst_BOBUIextLayout::textWidthWithLineSeparator()
{
    QString s1("Save Project"), s2("Save Project\ntest");
    s2.replace('\n', QChar::LineSeparator);

    BOBUIextLayout layout1(s1), layout2(s2);
    layout1.beginLayout();
    layout2.beginLayout();

    BOBUIextLine line1 = layout1.createLine();
    BOBUIextLine line2 = layout2.createLine();
    line1.setLineWidth(0x1000);
    line2.setLineWidth(0x1000);
    QCOMPARE(line1.naturalTextWidth(), line2.naturalTextWidth());
}

void tst_BOBUIextLayout::cursorInLigatureWithMultipleLines()
{
    BOBUIextLayout layout("first line finish", QFont("Times", 20));
    layout.setCacheEnabled(true);
    layout.beginLayout();
    BOBUIextLine line = layout.createLine();
    line.setNumColumns(10);
    BOBUIextLine line2 = layout.createLine();
    layout.endLayout();

    // The second line will be "finish"
    QCOMPARE(layout.text().mid(line2.textStart(), line2.textLength()), QString::fromLatin1("finish"));

    QVERIFY(line.cursorToX(1) != line.cursorToX(0));
    QCOMPARE(line2.cursorToX(line2.textStart()), line.cursorToX(0));
    QCOMPARE(line2.cursorToX(line2.textStart() + 1), line.cursorToX(1));
}

void tst_BOBUIextLayout::xToCursorForLigatures()
{
#if defined(Q_OS_WIN32)
    BOBUIextLayout layout("fi", QFont("Calibri", 20));
#else
    BOBUIextLayout layout("fi", QFont("Times", 20));
#endif

    layout.setCacheEnabled(true);
    layout.beginLayout();
    BOBUIextLine line = layout.createLine();
    layout.endLayout();

    QVERIFY(line.xToCursor(0) != line.xToCursor(line.naturalTextWidth() / 2));

    // U+0061 U+0308
    BOBUIextLayout layout2(QString::fromUtf8("\x61\xCC\x88"), QFont("Times", 20));
    layout2.setCacheEnabled(true);
    layout2.beginLayout();
    line = layout2.createLine();
    layout2.endLayout();

    qreal width = line.naturalTextWidth();
    QVERIFY(line.xToCursor(0) == line.xToCursor(width / 2) ||
            line.xToCursor(width) == line.xToCursor(width / 2));
}

void tst_BOBUIextLayout::cursorInNonStopChars()
{
    BOBUIextLayout layout(QString::fromUtf8("\xE0\xA4\xA4\xE0\xA5\x8D\xE0\xA4\xA8"));
    layout.setCacheEnabled(true);
    layout.beginLayout();
    BOBUIextLine line = layout.createLine();
    layout.endLayout();

    QCOMPARE(line.cursorToX(1), line.cursorToX(3));
    QCOMPARE(line.cursorToX(2), line.cursorToX(3));
}

void tst_BOBUIextLayout::justifyTrailingSpaces()
{
    BOBUIextLayout layout(QStringLiteral("     t"));
    layout.setTextOption(BOBUIextOption(BobUI::AlignJustify));
    layout.beginLayout();

    BOBUIextLine line = layout.createLine();
    line.setLineWidth(5);

    layout.endLayout();

    QVERIFY(qFuzzyIsNull(layout.lineAt(0).cursorToX(0)));
}

void tst_BOBUIextLayout::nbsp()
{
    QString s = QString() + QChar(' ') + QChar('a') + QString(10, QChar::Nbsp) + QChar('a') + QChar(' ') + QChar('A');
    QString text = s + s + s + s + s + s + s + s + s + s + s + s + s + s;
    BOBUIextLayout layout(text);
    layout.setCacheEnabled(true);
    layout.beginLayout();
    layout.createLine();
    layout.endLayout();

    int naturalWidth = qCeil(layout.lineAt(0).naturalTextWidth());
    int lineWidth = naturalWidth;

    layout.beginLayout();
    BOBUIextLine line = layout.createLine();
    while (lineWidth-- > naturalWidth / 2) {
        line.setLineWidth(lineWidth);
        QVERIFY(text.at(line.textLength()-1).unicode() != QChar::Nbsp);
    }

    layout.endLayout();
}

void tst_BOBUIextLayout::layoutWithCustomTabStops()
{
    QScopedPointer<BOBUIextLayout> textLayout(new BOBUIextLayout);
    QList<BOBUIextOption::Tab> tabStops;

    const int tabWidth = 18;
    const int maxTabPos = 2500;
    for (int tabPos = tabWidth; tabPos < maxTabPos; tabPos += tabWidth)
        tabStops << BOBUIextOption::Tab(tabPos, BOBUIextOption::LeftTab);

    BOBUIextOption textOption;
    textOption.setTabs(tabStops);
    textLayout->setTextOption(textOption);

    textLayout->setText(QStringLiteral("\ta aa aa aa aa aa aa"));

    textLayout->beginLayout();
    textLayout->createLine();
    textLayout->endLayout();

    qreal shortWidth = textLayout->maximumWidth();

    textLayout->setText(QStringLiteral("\ta aa aa aa aa aa aa aa aa aa aa aa aa a"));

    textLayout->beginLayout();
    textLayout->createLine();
    textLayout->endLayout();

    qreal longWidth = textLayout->maximumWidth();

    QVERIFY(longWidth > shortWidth);
}

void tst_BOBUIextLayout::noModificationOfInputString()
{
    QString s = QString(QChar(QChar::LineSeparator));
    {
        BOBUIextLayout layout;
        layout.setText(s);

        layout.beginLayout();
        layout.createLine();
        layout.endLayout();

        QCOMPARE(s.size(), 1);
        QCOMPARE(s.at(0), QChar(QChar::LineSeparator));
    }

    {
        BOBUIextLayout layout;
        layout.setText(s);

        BOBUIextOption option;
        option.setFlags(BOBUIextOption::ShowLineAndParagraphSeparators);
        layout.setTextOption(option);

        layout.beginLayout();
        layout.createLine();
        layout.endLayout();

        QCOMPARE(s.size(), 1);
        QCOMPARE(s.at(0), QChar(QChar::LineSeparator));
    }
}

void tst_BOBUIextLayout::showLineAndParagraphSeparatorsCrash()
{
    QString s = QString(100000, QChar('a')) + QChar(QChar::LineSeparator);
    {
        BOBUIextLayout layout;
        layout.setText(s);

        BOBUIextOption option;
        option.setFlags(BOBUIextOption::ShowLineAndParagraphSeparators);
        layout.setTextOption(option);

        layout.beginLayout();
        layout.createLine();
        layout.endLayout();
    }
}

void tst_BOBUIextLayout::superscriptCrash_bobuibug53911()
{
    static int fontSizes = 64;
    static QString layoutText = "THIS IS SOME EXAMPLE TEXT THIS IS SOME EXAMPLE TEXT";

    QList<BOBUIextLayout*> textLayouts;
    for (int i = 0; i < fontSizes; ++i) {
        for (int j = 0; j < 4; ++j) {
            BOBUIextLayout* newTextLayout = new BOBUIextLayout();
            newTextLayout->setText(layoutText);
            BOBUIextLayout::FormatRange formatRange;

            formatRange.format.setFont(QFont());
            formatRange.format.setFontPointSize(i + 5);

            switch (j) {
            case 0:
                formatRange.format.setFontWeight(QFont::Normal);
                formatRange.format.setFontItalic(false);
                break;
            case 1:
                formatRange.format.setFontWeight(QFont::Bold);
                formatRange.format.setFontItalic(false);
                break;
            case 2:
                formatRange.format.setFontWeight(QFont::Bold);
                formatRange.format.setFontItalic(true);
                break;
            case 3:
                formatRange.format.setFontWeight(QFont::Normal);
                formatRange.format.setFontItalic(true);
                break;
            }

            formatRange.format.setVerticalAlignment( BOBUIextCharFormat::AlignSuperScript);

            formatRange.start = 0;
            formatRange.length = layoutText.size();
            newTextLayout->setFormats({formatRange});

            textLayouts.push_front(newTextLayout);
        }
    }

    // This loop would crash before fix for BOBUIBUG-53911
    for (BOBUIextLayout *textLayout : std::as_const(textLayouts)) {
        textLayout->beginLayout();
        while (textLayout->createLine().isValid());
        textLayout->endLayout();
    }

    qDeleteAll(textLayouts);
}

void tst_BOBUIextLayout::nbspWithFormat()
{
    QString s1 = QLatin1String("ABCDEF ");
    QString s2 = QLatin1String("GHI");
    QChar nbsp(QChar::Nbsp);
    QString s3 = QLatin1String("JKLMNOPQRSTUVWXYZ");

    BOBUIextLayout layout;
    layout.setText(s1 + s2 + nbsp + s3);

    BOBUIextLayout::FormatRange formatRange;
    formatRange.start = s1.size() + s2.size();
    formatRange.length = 1;
    formatRange.format.setFontUnderline(true);

    layout.setFormats({formatRange});

    layout.beginLayout();
    forever {
        BOBUIextLine line = layout.createLine();
        if (!line.isValid())
            break;
        line.setLineWidth(1);
    }
    layout.endLayout();

    QCOMPARE(layout.lineCount(), 2);
    QCOMPARE(layout.lineAt(0).textStart(), 0);
    QCOMPARE(layout.lineAt(0).textLength(), s1.size());
    QCOMPARE(layout.lineAt(1).textStart(), s1.size());
    QCOMPARE(layout.lineAt(1).textLength(), s2.size() + 1 + s3.size());
}

void tst_BOBUIextLayout::koreanWordWrap()
{
    QString s = QString::fromUtf8("안녕하세요 여러분!");
    BOBUIextLayout layout;
    BOBUIextOption option = layout.textOption();
    option.setWrapMode(BOBUIextOption::WordWrap);
    option.setFlags(BOBUIextOption::Flag(BOBUIextOption::IncludeTrailingSpaces));
    layout.setTextOption(option);
    layout.setText(s);

    QFontMetrics metrics(layout.font());

    layout.beginLayout();
    forever {
        BOBUIextLine line = layout.createLine();
        if (!line.isValid())
            break;
        line.setLineWidth(metrics.horizontalAdvance(s) * 0.8);
    }
    layout.endLayout();
    QCOMPARE(layout.lineCount(), 2);
    QCOMPARE(layout.lineAt(0).textLength(), 6);
    QCOMPARE(layout.lineAt(1).textLength(), 4);
}

void tst_BOBUIextLayout::tooManyDirectionalCharctersCrash_bobuibug77819()
{
    QString data;
    data += QString::fromUtf8("\xe2\x81\xa8"); // U+2068 FSI character
    data += QString::fromUtf8("\xe2\x81\xa7"); // U+2067 RLI character

    // duplicating the text
    for (int i = 0; i < 10; i++)
        data += data;

    // Nothing to test. It must not crash in beginLayout().
    BOBUIextLayout tl(data);
    tl.beginLayout();
    tl.endLayout();
}

void tst_BOBUIextLayout::softHyphens_data()
{
    BOBUIest::addColumn<int>("fontSize");

    BOBUIest::newRow("12") << 12;
    BOBUIest::newRow("14") << 14;
    BOBUIest::newRow("16") << 16;
}

void tst_BOBUIextLayout::softHyphens()
{
    QFETCH(int, fontSize);
    QString text = QStringLiteral("xxxx\u00ad") + QStringLiteral("xxxx\u00ad");

    QFont font;
    font.setPixelSize(fontSize);
    font.setHintingPreference(QFont::PreferNoHinting);
    font.setKerning(false);
    const float xAdvance = QFontMetricsF(font).horizontalAdvance(QChar::fromLatin1('x'));
    float shyWidth = 0.0f;
    BOBUIextLayout layout(text, font);
    BOBUIextOption option;
    option.setWrapMode(BOBUIextOption::WrapAtWordBoundaryOrAnywhere);
    layout.setTextOption(option);
    {
        // Calculate the effective width of a line-ending hyphen
        // This calculation is currently done to work-around odditities on
        // macOS 11 (see BOBUIBUG-90698).
        BOBUIextLayout test(QStringLiteral("x\u00ad"), font);
        // Note: This only works because BobUI show the soft-hyphen when ending a text.
        // This _could_ be considered a bug and the test would need to be changed
        // if we stop doing that.
        test.beginLayout();
        BOBUIextLine line = test.createLine();
        line.setLineWidth(10 * xAdvance);
        line.setPosition(QPoint(0, 0));
        shyWidth = line.naturalTextWidth() - xAdvance;
        test.endLayout();
    }
    qreal linefit;
    // Loose fit
    // xxxx- |
    // xxxx- |
    {
        int pos = 0;
        int y = 0;
        layout.beginLayout();
        BOBUIextLine line = layout.createLine();
        line.setLineWidth(qCeil(5 * xAdvance + shyWidth) + 1);
        line.setPosition(QPoint(0, y));
        QCOMPARE(line.textStart(), pos);
        QCOMPARE(line.textLength(), 5);
        linefit = line.naturalTextWidth();
        QVERIFY(qAbs(linefit - qCeil(4 * xAdvance + shyWidth)) <= 1.0);

        pos += line.textLength();
        y += qRound(line.ascent() + line.descent());

        line = layout.createLine();
        line.setLineWidth(qCeil(5 * xAdvance + shyWidth) + 1);
        line.setPosition(QPoint(0, y));
        QCOMPARE(line.textStart(), pos);
        QCOMPARE(line.textLength(), 5);
        QVERIFY(qAbs(line.naturalTextWidth() - linefit) <= 1.0);
        layout.endLayout();
    }

    // Tight fit
    // xxxx-|
    // xxxx-|
    {
        int pos = 0;
        int y = 0;
        layout.beginLayout();
        BOBUIextLine line = layout.createLine();
        line.setLineWidth(qCeil(linefit) + 1);
        line.setPosition(QPoint(0, y));
        QCOMPARE(line.textStart(), pos);
        QCOMPARE(line.textLength(), 5);
        QVERIFY(qAbs(line.naturalTextWidth() - linefit) <= 1.0);

        pos += line.textLength();
        y += qRound(line.ascent() + line.descent());

        line = layout.createLine();
        line.setLineWidth(qCeil(linefit) + 1);
        line.setPosition(QPoint(0, y));
        QCOMPARE(line.textStart(), pos);
        QCOMPARE(line.textLength(), 5);
        QVERIFY(qAbs(line.naturalTextWidth() - linefit) <= 1.0);
        layout.endLayout();
    }

    // Very tight fit
    // xxxx|
    // xxxx|
    // -   |
    {
        int pos = 0;
        int y = 0;
        layout.beginLayout();
        BOBUIextLine line = layout.createLine();
        line.setLineWidth(qCeil(4 * xAdvance) + 2);
        line.setPosition(QPoint(0, y));
        QCOMPARE(line.textStart(), pos);
        QCOMPARE(line.textLength(), 4);
        QVERIFY(qAbs(line.naturalTextWidth() - qCeil(4 * xAdvance)) <= 1.0);

        pos += line.textLength();
        y += qRound(line.ascent() + line.descent());

        line = layout.createLine();
        line.setLineWidth(qCeil(4 * xAdvance) + 2);
        line.setPosition(QPoint(0, y));
        QCOMPARE(line.textStart(), pos);
        QCOMPARE(line.textLength(), 5);
        QVERIFY(qAbs(line.naturalTextWidth() - qCeil(4 * xAdvance)) <= 1.0);

        pos += line.textLength();
        y += qRound(line.ascent() + line.descent());

        line = layout.createLine();
        line.setLineWidth(qCeil(4 * xAdvance) + 2);
        line.setPosition(QPoint(0, y));
        QCOMPARE(line.textStart(), pos);
        QCOMPARE(line.textLength(), 1);
        QVERIFY(qAbs(line.naturalTextWidth() - shyWidth) <= 1.0);
        layout.endLayout();
    }
}

void tst_BOBUIextLayout::min_maximumWidth_data()
{
    BOBUIest::addColumn<QString>("text");

    BOBUIest::newRow("long string") << QStringLiteral("lmong_long_crazy_87235982735_23857239682376923876923876-fuwhfhfw-names-AAAA-deeaois2019-03-03.and.more");
    BOBUIest::newRow("BOBUIBUG-106947") << QStringLiteral("text                                text");
    BOBUIest::newRow("spaces") << QStringLiteral("                text                text                ");
    BOBUIest::newRow("BOBUIBUG-104986") << QStringLiteral("text\ntext\ntext");
    BOBUIest::newRow("spaces + line breaks") << QStringLiteral("       \n         text\n                \ntext       \n         ");
}

void tst_BOBUIextLayout::min_maximumWidth()
{
    QFETCH(QString, text);
    text.replace('\n', QChar::LineSeparator);

    BOBUIextLayout layout(text, testFont);
    layout.setCacheEnabled(true);

    BOBUIextOption opt;
    opt.setWrapMode(BOBUIextOption::NoWrap);
    layout.setTextOption(opt);
    layout.beginLayout();
    while (layout.createLine().isValid()) { }
    layout.endLayout();

    const qreal nonWrappedMaxWidth = layout.maximumWidth();

    for (int wrapMode = BOBUIextOption::NoWrap; wrapMode <= BOBUIextOption::WrapAtWordBoundaryOrAnywhere; ++wrapMode) {
        opt.setWrapMode((BOBUIextOption::WrapMode)wrapMode);
        layout.setTextOption(opt);
        layout.beginLayout();
        while (layout.createLine().isValid()) { }
        layout.endLayout();
        const qreal minWidth = layout.minimumWidth();
        const qreal maxWidth = layout.maximumWidth();

        QCOMPARE_LE(minWidth, maxWidth);
        QCOMPARE_LE(maxWidth, nonWrappedMaxWidth); // maxWidth for wrapped text shouldn't exceed maxWidth for the text without wrapping.

        // Try the layout from slightly wider than the widest (maxWidth)
        // and narrow it down to slighly narrower than minWidth
        // layout.maximumWidth() should return the same regardless
        qreal width = qCeil(maxWidth/10)*10 + 10;    // begin a bit wider
        const qreal stepSize = 20;
        while (width >= minWidth - stepSize) {
            layout.beginLayout();
            for (;;) {
                BOBUIextLine line = layout.createLine();
                if (!line.isValid())
                    break;
                line.setLineWidth(width);
            }
            layout.endLayout();
            QCOMPARE(layout.minimumWidth(), minWidth);
            QCOMPARE(layout.maximumWidth(), maxWidth);
            width -= stepSize;
        }
    }
}

void tst_BOBUIextLayout::negativeLineWidth()
{
    {
        BOBUIextLayout layout;
        layout.setText("Foo bar");
        layout.beginLayout();
        BOBUIextLine line = layout.createLine();
        line.setLineWidth(-1);
        QVERIFY(line.textLength() > 0);
        layout.endLayout();
    }

    {
        BOBUIextLayout layout;
        layout.setText("Foo bar");
        layout.beginLayout();
        BOBUIextLine line = layout.createLine();
        line.setNumColumns(2, -1);
        QVERIFY(line.textLength() > 0);
        layout.endLayout();
    }
}

void tst_BOBUIextLayout::embeddedImageLineHeight()
{
    QString s1 = QStringLiteral("Foobar Foobar Foobar Foobar");
    QString s2 = QStringLiteral("<img height=\"80\" width=\"80\" />Foobar Foobar Foobar Foobar");

    qreal s1Width;
    qreal s1Height;
    {
        BOBUIextDocument document;
        document.setHtml(s1);
        QCOMPARE(document.blockCount(), 1);

        // Trigger layout
        {
            QImage img(1, 1, QImage::Format_ARGB32_Premultiplied);
            QPainter p(&img);
            document.drawContents(&p);
        }

        BOBUIextLayout *layout = document.firstBlock().layout();
        QVERIFY(layout != nullptr);
        QCOMPARE(layout->lineCount(), 1);
        BOBUIextLine line = layout->lineAt(0);
        s1Width = document.idealWidth();
        s1Height = line.ascent() + line.descent();
    }

    {
        BOBUIextDocument document;
        document.setHtml(s1 + s2);
        document.setTextWidth(std::ceil(s1Width));
        QCOMPARE(document.blockCount(), 1);

        // Trigger layout
        {
            QImage img(1, 1, QImage::Format_ARGB32_Premultiplied);
            QPainter p(&img);
            document.drawContents(&p);
        }

        BOBUIextLayout *layout = document.firstBlock().layout();
        QVERIFY(layout != nullptr);
        QVERIFY(layout->lineCount() > 1);
        BOBUIextLine line = layout->lineAt(0);
        QCOMPARE(line.ascent() + line.descent(), s1Height);
    }
}

void tst_BOBUIextLayout::unmatchedShapedSubstring()
{
    QString s;
    s += QChar(9977);
    s += QChar(65039);
    s += QChar(8205);
    s += QChar(9794);
    s += QChar(65039);

    BOBUIextLayout lout;

    BOBUIextOption opt;
    opt.setFlags(BOBUIextOption::DisableEmojiParsing);
    lout.setTextOption(opt);

    // Note: Shaping of this string would previously assert on some platforms
    lout.setText(s);
    lout.beginLayout();
    lout.createLine();
    lout.endLayout();

    QList<QGlyphRun> glyphRuns = lout.glyphRuns();
    QVERIFY(glyphRuns.size() > 0);
}

void tst_BOBUIextLayout::maximumLayoutWidthInWrappedLayout()
{
    QString s = QString::fromUtf8("Lorem ipsum dolor sit amet, consectetur adipiscing elit.\n"
                "Integer at ante dui Curabitur ante est, pulvinar quis adipiscing a, iaculis id ipsum. Nunc blandit\n"
                "condimentum odio vel egestas. in ipsum lacinia sit amet\n"
                "mattis orci interdum. Quisque vitae accumsan lectus. Ut nisi turpis,\n"
                "sollicitudin ut dignissim id, fermentum ac est. Maecenas nec libero leo. Sed ac\n"
                "mattis orci interdum. Quisque vitae accumsan lectus. Ut nisi turpis,\n"
                "sollicitudin ut dignissim id, fermentum ac est. Maecenas nec libero leo. Sed ac\n"
                "leo eget ipsum ultricies viverra sit amet eu orci. Praesent et tortor risus,\n"
                "viverra accumsan sapien. Sed faucibus eleifend lectus, sed euismod urna porta\n"
                "eu. Quisque vitae accumsan lectus.");
    s.replace(QChar::LineFeed, QChar::LineSeparator);

    BOBUIextLayout reference;
    reference.setText(s);
    reference.beginLayout();
    forever {
        BOBUIextLine line = reference.createLine();
        if (!line.isValid())
            break;
    }
    reference.endLayout();

    BOBUIextLayout breakByWidth;
    breakByWidth.setText(s);
    breakByWidth.beginLayout();
    forever {
        BOBUIextLine line = breakByWidth.createLine();
        if (!line.isValid())
            break;
        line.setLineWidth(100);
    }
    breakByWidth.endLayout();

    QCOMPARE(reference.maximumWidth(), breakByWidth.maximumWidth());
}

BOBUIEST_MAIN(tst_BOBUIextLayout)
#include "tst_bobuiextlayout.moc"
