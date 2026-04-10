// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only


#include <BOBUIest>
#include <QBuffer>

#include <qcoreapplication.h>
#include <qdebug.h>
#if BOBUI_CONFIG(settings)
#include <qsettings.h>
#endif
#include <bobuiextformat.h>
#include <private/bobuiextformat_p.h>
#include <bobuiextdocument.h>
#include <bobuiextcursor.h>
#include <bobuiextobject.h>
#include <bobuiextlayout.h>
#include <qabstracttextdocumentlayout.h>

#ifndef BOBUI_NO_DATASTREAM
#  include <qdatastream.h>
#endif

class tst_BOBUIextFormat : public QObject
{
Q_OBJECT

private slots:
    void getSetCheck();
    void defaultAlignment();
#if BOBUI_CONFIG(settings)
    void testBOBUIextCharFormat() const;
#endif
    void testUnderlinePropertyPrecedence();
    void toFormat();
    void resolveFont();
    void testLetterSpacing();
    void testFontStretch();
    void getSetTabs();
    void testTabsUsed();
    void testFontStyleSetters();
    void setFont_data();
    void setFont();
    void setFont_collection_data();
    void setFont_collection();
    void clearCollection();
    void setFontFeatures();
    void setFontVariableAxes();

#ifndef BOBUI_NO_DATASTREAM
    void dataStreamCompatibility();
#endif
};

#if BOBUI_CONFIG(settings)
/*! \internal
  This (used to) trigger a crash in:

    QDataStream &operator>>(QDataStream &stream, BOBUIextFormat &fmt)

  which is most easily produced through QSettings.
 */
void tst_BOBUIextFormat::testBOBUIextCharFormat() const
{
    QSettings settings("test", "test");
    BOBUIextCharFormat test;

    settings.value("test", test);
}
#endif

// Testing get/set functions
void tst_BOBUIextFormat::getSetCheck()
{
    BOBUIextFormat obj1;
    // int BOBUIextFormat::objectIndex()
    // void BOBUIextFormat::setObjectIndex(int)
    obj1.setObjectIndex(0);
    QCOMPARE(0, obj1.objectIndex());
    obj1.setObjectIndex(INT_MIN);
    QCOMPARE(INT_MIN, obj1.objectIndex());
    obj1.setObjectIndex(INT_MAX);
    QCOMPARE(INT_MAX, obj1.objectIndex());
}

void tst_BOBUIextFormat::defaultAlignment()
{
    BOBUIextBlockFormat fmt;
    QVERIFY(!fmt.hasProperty(BOBUIextFormat::BlockAlignment));
    QCOMPARE(fmt.intProperty(BOBUIextFormat::BlockAlignment), 0);
    QCOMPARE(fmt.alignment(), BobUI::AlignLeft);
}

void tst_BOBUIextFormat::testUnderlinePropertyPrecedence()
{
    BOBUIextCharFormat format;
    // use normal accessors and check internal state
    format.setUnderlineStyle(BOBUIextCharFormat::NoUnderline);
    QCOMPARE(format.property(BOBUIextFormat::FontUnderline).isNull(), false);
    QCOMPARE(format.property(BOBUIextFormat::TextUnderlineStyle).isNull(), false);
    QCOMPARE(format.property(BOBUIextFormat::FontUnderline).toBool(), false);
    QCOMPARE(format.property(BOBUIextFormat::TextUnderlineStyle).toInt(), 0);

    format = BOBUIextCharFormat();
    format.setUnderlineStyle(BOBUIextCharFormat::SingleUnderline);
    QCOMPARE(format.property(BOBUIextFormat::FontUnderline).isNull(), false);
    QCOMPARE(format.property(BOBUIextFormat::TextUnderlineStyle).isNull(), false);
    QCOMPARE(format.property(BOBUIextFormat::FontUnderline).toBool(), true);
    QCOMPARE(format.property(BOBUIextFormat::TextUnderlineStyle).toInt(), 1);

    format = BOBUIextCharFormat();
    format.setUnderlineStyle(BOBUIextCharFormat::DotLine);
    QCOMPARE(format.property(BOBUIextFormat::FontUnderline).isNull(), false);
    QCOMPARE(format.property(BOBUIextFormat::TextUnderlineStyle).isNull(), false);
    QCOMPARE(format.property(BOBUIextFormat::FontUnderline).toBool(), false);
    QVERIFY(format.property(BOBUIextFormat::TextUnderlineStyle).toInt() > 0);

    // override accessors and use setProperty to create a false state.
    // then check font()
    format = BOBUIextCharFormat();
    format.setProperty(BOBUIextCharFormat::FontUnderline, true);
    QCOMPARE(format.property(BOBUIextFormat::FontUnderline).isNull(), false);
    QCOMPARE(format.property(BOBUIextFormat::TextUnderlineStyle).isNull(), true);
    QCOMPARE(format.fontUnderline(), true);
    QCOMPARE(format.font().underline(), true);

    format = BOBUIextCharFormat();
    // create conflict. Should use the new property
    format.setProperty(BOBUIextCharFormat::TextUnderlineStyle, BOBUIextCharFormat::SingleUnderline);
    format.setProperty(BOBUIextCharFormat::FontUnderline, false);
    QCOMPARE(format.fontUnderline(), true);
    QCOMPARE(format.font().underline(), true);

    format = BOBUIextCharFormat();
    // create conflict. Should use the new property
    format.setProperty(BOBUIextCharFormat::TextUnderlineStyle, BOBUIextCharFormat::NoUnderline);
    format.setProperty(BOBUIextCharFormat::FontUnderline, true);
    QCOMPARE(format.fontUnderline(), false);
    QCOMPARE(format.font().underline(), false);

    // do it again, but reverse the ordering (we use a QList internally, so test a LOT ;)
    // create conflict. Should use the new property
    format.setProperty(BOBUIextCharFormat::FontUnderline, false);
    format.setProperty(BOBUIextCharFormat::TextUnderlineStyle, BOBUIextCharFormat::SingleUnderline);
    QCOMPARE(format.fontUnderline(), true);
    QCOMPARE(format.font().underline(), true);

    format = BOBUIextCharFormat();
    // create conflict. Should use the new property
    format.setProperty(BOBUIextCharFormat::FontUnderline, true);
    format.setProperty(BOBUIextCharFormat::TextUnderlineStyle, BOBUIextCharFormat::NoUnderline);
    QCOMPARE(format.fontUnderline(), false);
    QCOMPARE(format.font().underline(), false);
}

void tst_BOBUIextFormat::toFormat()
{
    {
        BOBUIextFormat fmt = BOBUIextFrameFormat();
        QCOMPARE(fmt.toFrameFormat().type(), int(BOBUIextFormat::FrameFormat));
    }

    {
        BOBUIextFormat fmt = BOBUIextTableFormat();
        QCOMPARE(fmt.toTableFormat().type(), int(BOBUIextFormat::FrameFormat));
        QCOMPARE(fmt.toTableFormat().objectType(), int(BOBUIextFormat::TableObject));
    }

    {
        BOBUIextFormat fmt = BOBUIextBlockFormat();
        QCOMPARE(fmt.toBlockFormat().type(), int(BOBUIextFormat::BlockFormat));
    }

    {
        BOBUIextFormat fmt = BOBUIextCharFormat();
        QCOMPARE(fmt.toCharFormat().type(), int(BOBUIextFormat::CharFormat));
    }

    {
        BOBUIextFormat fmt = BOBUIextListFormat();
        QCOMPARE(fmt.toListFormat().type(), int(BOBUIextFormat::ListFormat));
    }
}

void tst_BOBUIextFormat::resolveFont()
{
    BOBUIextDocument doc;

    BOBUIextCharFormat fmt;
    fmt.setProperty(BOBUIextFormat::ForegroundBrush, QColor(BobUI::blue));
    QCOMPARE(fmt.property(BOBUIextFormat::ForegroundBrush).userType(), qMetaTypeId<QColor>());
    QCOMPARE(fmt.property(BOBUIextFormat::ForegroundBrush).value<QColor>(), QColor(BobUI::blue));
    fmt.setProperty(BOBUIextFormat::FontItalic, true);
    BOBUIextCursor(&doc).insertText("Test", fmt);

    QList<BOBUIextFormat> formats = doc.allFormats();
    QCOMPARE(formats.size(), 3);

    QCOMPARE(formats.at(2).type(), int(BOBUIextFormat::CharFormat));
    fmt = formats.at(2).toCharFormat();

    QVERIFY(!fmt.font().underline());
    QVERIFY(fmt.hasProperty(BOBUIextFormat::ForegroundBrush));

    QFont f;
    f.setUnderline(true);
    doc.setDefaultFont(f);
    formats = doc.allFormats();
    fmt = formats.at(2).toCharFormat();

    QVERIFY(fmt.font().underline());
    QVERIFY(!fmt.hasProperty(BOBUIextFormat::FontUnderline));

    // verify that deleting a non-existent property does not affect the font resolving

    QVERIFY(!fmt.hasProperty(BOBUIextFormat::BackgroundBrush));
    fmt.clearProperty(BOBUIextFormat::BackgroundBrush);
    QVERIFY(!fmt.hasProperty(BOBUIextFormat::BackgroundBrush));

    QVERIFY(!fmt.hasProperty(BOBUIextFormat::FontUnderline));
    QVERIFY(fmt.font().underline());

    // verify that deleting an existent but font _unrelated_ property does not affect the font resolving

    QVERIFY(fmt.hasProperty(BOBUIextFormat::ForegroundBrush));
    fmt.clearProperty(BOBUIextFormat::ForegroundBrush);
    QVERIFY(!fmt.hasProperty(BOBUIextFormat::ForegroundBrush));

    QVERIFY(!fmt.hasProperty(BOBUIextFormat::FontUnderline));
    QVERIFY(fmt.font().underline());

    // verify that removing a font property _does_ clear the resolving

    QVERIFY(fmt.hasProperty(BOBUIextFormat::FontItalic));
    fmt.clearProperty(BOBUIextFormat::FontItalic);
    QVERIFY(!fmt.hasProperty(BOBUIextFormat::FontItalic));

    QVERIFY(!fmt.hasProperty(BOBUIextFormat::FontUnderline));
    QVERIFY(!fmt.font().underline());
    QVERIFY(!fmt.font().italic());

    // reset
    fmt = formats.at(2).toCharFormat();

    QVERIFY(fmt.font().underline());
    QVERIFY(!fmt.hasProperty(BOBUIextFormat::FontUnderline));

    // verify that _setting_ an unrelated property does _not_ affect the resolving

    QVERIFY(!fmt.hasProperty(BOBUIextFormat::IsAnchor));
    fmt.setProperty(BOBUIextFormat::IsAnchor, true);
    QVERIFY(fmt.hasProperty(BOBUIextFormat::IsAnchor));

    QVERIFY(fmt.font().underline());
    QVERIFY(!fmt.hasProperty(BOBUIextFormat::FontUnderline));

    // verify that setting a _related_ font property does affect the resolving
    //
    QVERIFY(!fmt.hasProperty(BOBUIextFormat::FontStrikeOut));
    fmt.setProperty(BOBUIextFormat::FontStrikeOut, true);
    QVERIFY(fmt.hasProperty(BOBUIextFormat::FontStrikeOut));

    QVERIFY(!fmt.font().underline());
    QVERIFY(!fmt.hasProperty(BOBUIextFormat::FontUnderline));
    QVERIFY(fmt.font().strikeOut());
}


void tst_BOBUIextFormat::testLetterSpacing()
{
    BOBUIextCharFormat format;

    QCOMPARE(format.hasProperty(BOBUIextFormat::FontLetterSpacing), false);
    QCOMPARE(format.hasProperty(BOBUIextFormat::FontLetterSpacingType), false);

    format.setFontLetterSpacingType(QFont::AbsoluteSpacing);
    QCOMPARE(format.font().letterSpacingType(), QFont::AbsoluteSpacing);

    format.setFontLetterSpacing(10.0);
    QCOMPARE(format.font().letterSpacing(), 10.0);

    QCOMPARE(format.hasProperty(BOBUIextFormat::FontLetterSpacing), true);
    QCOMPARE(format.property(BOBUIextFormat::FontLetterSpacing).toDouble(), 10.0);
    QCOMPARE(format.property(BOBUIextFormat::FontLetterSpacingType).toInt(), int(QFont::AbsoluteSpacing));

    format.setFontLetterSpacingType(QFont::PercentageSpacing);
    QCOMPARE(format.font().letterSpacingType(), QFont::PercentageSpacing);

    format.setFontLetterSpacing(110.0);
    QCOMPARE(format.font().letterSpacing(), 110.0);

    QCOMPARE(format.property(BOBUIextFormat::FontLetterSpacing).toDouble(), 110.0);
    QCOMPARE(format.property(BOBUIextFormat::FontLetterSpacingType).toInt(), int(QFont::PercentageSpacing));

    format.setFontLetterSpacingType(QFont::AbsoluteSpacing);
    QCOMPARE(format.font().letterSpacingType(), QFont::AbsoluteSpacing);

    format.setFontLetterSpacing(10.0);
    QCOMPARE(format.font().letterSpacing(), 10.0);

    QCOMPARE(format.property(BOBUIextFormat::FontLetterSpacingType).toInt(), int(QFont::AbsoluteSpacing));
    QCOMPARE(format.property(BOBUIextFormat::FontLetterSpacing).toDouble(), 10.0);
}

void tst_BOBUIextFormat::testFontStretch()
{
    BOBUIextCharFormat format;

    QCOMPARE(format.hasProperty(BOBUIextFormat::FontStretch), false);

    format.setFontStretch(130.0);

    QCOMPARE(format.property(BOBUIextFormat::FontStretch).toInt(), 130);
}

void tst_BOBUIextFormat::getSetTabs()
{
    class Comparator {
      public:
        Comparator(const QList<BOBUIextOption::Tab> &tabs, const QList<BOBUIextOption::Tab> &tabs2)
        {
            QCOMPARE(tabs.size(), tabs2.size());
            for(int i=0; i < tabs.size(); i++) {
                BOBUIextOption::Tab t1 = tabs[i];
                BOBUIextOption::Tab t2 = tabs2[i];
                QCOMPARE(t1.position, t2.position);
                QCOMPARE(t1.type, t2.type);
                QCOMPARE(t1.delimiter, t2.delimiter);
            }
        }
    };

    QList<BOBUIextOption::Tab> tabs;
    BOBUIextBlockFormat format;
    format.setTabPositions(tabs);
    Comparator c1(tabs, format.tabPositions());

    BOBUIextOption::Tab tab1;
    tab1.position = 1234;
    tabs.append(tab1);
    format.setTabPositions(tabs);
    Comparator c2(tabs, format.tabPositions());

    BOBUIextOption::Tab tab2(3456, BOBUIextOption::RightTab, QChar('x'));
    tabs.append(tab2);
    format.setTabPositions(tabs);
    Comparator c3(tabs, format.tabPositions());
}

void tst_BOBUIextFormat::testTabsUsed()
{
    BOBUIextDocument doc;
    BOBUIextCursor cursor(&doc);

    QList<BOBUIextOption::Tab> tabs;
    BOBUIextBlockFormat format;
    BOBUIextOption::Tab tab;
    tab.position = 100;
    tabs.append(tab);
    format.setTabPositions(tabs);
    cursor.mergeBlockFormat(format);
    cursor.insertText("foo\tbar");
    //doc.setPageSize(QSizeF(200, 200));
    doc.documentLayout()->pageCount(); // force layout;

    BOBUIextBlock block = doc.begin();
    BOBUIextLayout *layout = block.layout();
    QVERIFY(layout);
    QCOMPARE(layout->lineCount(), 1);
    BOBUIextLine line = layout->lineAt(0);
    QCOMPARE(line.cursorToX(4), 100.);

    BOBUIextOption option = layout->textOption();
    QCOMPARE(option.tabs().size(), tabs.size());

}

void tst_BOBUIextFormat::testFontStyleSetters()
{
    BOBUIextCharFormat format;

    // test the setters
    format.setFontStyleHint(QFont::Serif);
    QCOMPARE(format.font().styleHint(), QFont::Serif);
    QCOMPARE(format.font().styleStrategy(), QFont::PreferDefault);
    format.setFontStyleStrategy(QFont::PreferOutline);
    QCOMPARE(format.font().styleStrategy(), QFont::PreferOutline);

    // test setting properties through setFont()
    QFont font;
    font.setStyleHint(QFont::SansSerif, QFont::PreferAntialias);
    format.setFont(font);
    QCOMPARE(format.font().styleHint(), QFont::SansSerif);
    QCOMPARE(format.font().styleStrategy(), QFont::PreferAntialias);

    // test kerning
    format.setFontKerning(false);
    QCOMPARE(format.font().kerning(), false);
    format.setFontKerning(true);
    QCOMPARE(format.font().kerning(), true);
    font.setKerning(false);
    format.setFont(font);
    QCOMPARE(format.font().kerning(), false);
}

Q_DECLARE_METATYPE(BOBUIextCharFormat)

void tst_BOBUIextFormat::setFont_data()
{
    BOBUIest::addColumn<BOBUIextCharFormat>("format1");
    BOBUIest::addColumn<BOBUIextCharFormat>("format2");
    BOBUIest::addColumn<bool>("overrideAll");

    BOBUIextCharFormat format1;
    format1.setFontStyleHint(QFont::Serif);
    format1.setFontStyleStrategy(QFont::PreferOutline);
    format1.setFontCapitalization(QFont::AllUppercase);
    format1.setFontKerning(true);

    {
        BOBUIest::newRow("noop|override") << format1 << format1 << true;
        BOBUIest::newRow("noop|inherit") << format1 << format1 << false;
    }

    {
        BOBUIextCharFormat format2;
        format2.setFontStyleHint(QFont::SansSerif);
        format2.setFontStyleStrategy(QFont::PreferAntialias);
        format2.setFontCapitalization(QFont::MixedCase);
        format2.setFontKerning(false);

        BOBUIest::newRow("coverage|override") << format1 << format2 << true;
        BOBUIest::newRow("coverage|inherit") << format1 << format2 << false;
    }

    {
        BOBUIextCharFormat format2;
        format2.setFontStyleHint(QFont::SansSerif);
        format2.setFontStyleStrategy(QFont::PreferAntialias);

        BOBUIest::newRow("partial|override") << format1 << format2 << true;
        BOBUIest::newRow("partial|inherit") << format1 << format2 << false;
    }
}

void tst_BOBUIextFormat::setFont()
{
    QFETCH(BOBUIextCharFormat, format1);
    QFETCH(BOBUIextCharFormat, format2);
    QFETCH(bool, overrideAll);

    BOBUIextCharFormat f;

    f.merge(format1);
    QCOMPARE((int)f.fontStyleHint(), (int)format1.fontStyleHint());
    QCOMPARE((int)f.fontStyleStrategy(), (int)format1.fontStyleStrategy());
    QCOMPARE((int)f.fontCapitalization(), (int)format1.fontCapitalization());
    QCOMPARE(f.fontKerning(), format1.fontKerning());

    QCOMPARE((int)f.font().styleHint(), (int)f.fontStyleHint());
    QCOMPARE((int)f.font().styleStrategy(), (int)f.fontStyleStrategy());
    QCOMPARE((int)f.font().capitalization(), (int)f.fontCapitalization());
    QCOMPARE(f.font().kerning(), f.fontKerning());

    f.merge(format2);
    QCOMPARE((int)f.font().styleHint(), (int)f.fontStyleHint());
    QCOMPARE((int)f.font().styleStrategy(), (int)f.fontStyleStrategy());
    QCOMPARE((int)f.font().capitalization(), (int)f.fontCapitalization());
    QCOMPARE(f.font().kerning(), f.fontKerning());

    if (format2.hasProperty(BOBUIextFormat::FontStyleHint))
        QCOMPARE((int)f.font().styleHint(), (int)format2.fontStyleHint());
    else
        QCOMPARE((int)f.font().styleHint(), (int)format1.fontStyleHint());
    if (format2.hasProperty(BOBUIextFormat::FontStyleStrategy))
        QCOMPARE((int)f.font().styleStrategy(), (int)format2.fontStyleStrategy());
    else
        QCOMPARE((int)f.font().styleStrategy(), (int)format1.fontStyleStrategy());
    if (format2.hasProperty(BOBUIextFormat::FontCapitalization))
        QCOMPARE((int)f.font().capitalization(), (int)format2.fontCapitalization());
    else
        QCOMPARE((int)f.font().capitalization(), (int)format1.fontCapitalization());
    if (format2.hasProperty(BOBUIextFormat::FontKerning))
        QCOMPARE(f.font().kerning(), format2.fontKerning());
    else
        QCOMPARE(f.font().kerning(), format1.fontKerning());


    QFont font1 = format1.font();
    QFont font2 = format2.font();

    f = BOBUIextCharFormat();

    {
        BOBUIextCharFormat tmp;
        tmp.setFont(font1, overrideAll ? BOBUIextCharFormat::FontPropertiesAll
                                       : BOBUIextCharFormat::FontPropertiesSpecifiedOnly);
        f.merge(tmp);
    }
    QCOMPARE((int)f.fontStyleHint(), (int)format1.fontStyleHint());
    QCOMPARE((int)f.fontStyleStrategy(), (int)format1.fontStyleStrategy());
    QCOMPARE((int)f.fontCapitalization(), (int)format1.fontCapitalization());
    QCOMPARE(f.fontKerning(), format1.fontKerning());

    QCOMPARE((int)f.font().styleHint(), (int)f.fontStyleHint());
    QCOMPARE((int)f.font().styleStrategy(), (int)f.fontStyleStrategy());
    QCOMPARE((int)f.font().capitalization(), (int)f.fontCapitalization());
    QCOMPARE(f.font().kerning(), f.fontKerning());

    {
        BOBUIextCharFormat tmp;
        tmp.setFont(font2, overrideAll ? BOBUIextCharFormat::FontPropertiesAll
                                       : BOBUIextCharFormat::FontPropertiesSpecifiedOnly);
        f.merge(tmp);
    }
    QCOMPARE((int)f.font().styleHint(), (int)f.fontStyleHint());
    QCOMPARE((int)f.font().styleStrategy(), (int)f.fontStyleStrategy());
    QCOMPARE((int)f.font().capitalization(), (int)f.fontCapitalization());
    QCOMPARE(f.font().kerning(), f.fontKerning());

    if (overrideAll || (font2.resolveMask() & QFont::StyleHintResolved))
        QCOMPARE((int)f.font().styleHint(), (int)font2.styleHint());
    else
        QCOMPARE((int)f.font().styleHint(), (int)font1.styleHint());
    if (overrideAll || (font2.resolveMask() & QFont::StyleStrategyResolved))
        QCOMPARE((int)f.font().styleStrategy(), (int)font2.styleStrategy());
    else
        QCOMPARE((int)f.font().styleStrategy(), (int)font1.styleStrategy());
    if (overrideAll || (font2.resolveMask() & QFont::CapitalizationResolved))
        QCOMPARE((int)f.font().capitalization(), (int)font2.capitalization());
    else
        QCOMPARE((int)f.font().capitalization(), (int)font1.capitalization());
    if (overrideAll || (font2.resolveMask() & QFont::KerningResolved))
        QCOMPARE(f.font().kerning(), font2.kerning());
    else
        QCOMPARE(f.font().kerning(), font1.kerning());
}

void tst_BOBUIextFormat::setFont_collection_data()
{
    setFont_data();
}

void tst_BOBUIextFormat::setFont_collection()
{
    QFETCH(BOBUIextCharFormat, format1);
    QFETCH(BOBUIextCharFormat, format2);
    QFETCH(bool, overrideAll);

    QFont font1 = format1.font();
    QFont font2 = format2.font();

    {
        BOBUIextFormatCollection collection;
        collection.setDefaultFont(font1);

        int formatIndex = collection.indexForFormat(format1);
        BOBUIextCharFormat f = collection.charFormat(formatIndex);

        QCOMPARE((int)f.fontStyleHint(), (int)format1.fontStyleHint());
        QCOMPARE((int)f.fontStyleStrategy(), (int)format1.fontStyleStrategy());
        QCOMPARE((int)f.fontCapitalization(), (int)format1.fontCapitalization());
        QCOMPARE(f.fontKerning(), format1.fontKerning());

        QCOMPARE((int)f.font().styleHint(), (int)f.fontStyleHint());
        QCOMPARE((int)f.font().styleStrategy(), (int)f.fontStyleStrategy());
        QCOMPARE((int)f.font().capitalization(), (int)f.fontCapitalization());
        QCOMPARE(f.font().kerning(), f.fontKerning());
    }
    {
        BOBUIextFormatCollection collection;
        collection.setDefaultFont(font1);

        int formatIndex = collection.indexForFormat(format2);
        BOBUIextCharFormat f = collection.charFormat(formatIndex);

        if (format2.hasProperty(BOBUIextFormat::FontStyleHint))
            QCOMPARE((int)f.font().styleHint(), (int)format2.fontStyleHint());
        else
            QCOMPARE((int)f.font().styleHint(), (int)format1.fontStyleHint());
        if (format2.hasProperty(BOBUIextFormat::FontStyleStrategy))
            QCOMPARE((int)f.font().styleStrategy(), (int)format2.fontStyleStrategy());
        else
            QCOMPARE((int)f.font().styleStrategy(), (int)format1.fontStyleStrategy());
        if (format2.hasProperty(BOBUIextFormat::FontCapitalization))
            QCOMPARE((int)f.font().capitalization(), (int)format2.fontCapitalization());
        else
            QCOMPARE((int)f.font().capitalization(), (int)format1.fontCapitalization());
        if (format2.hasProperty(BOBUIextFormat::FontKerning))
            QCOMPARE(f.font().kerning(), format2.fontKerning());
        else
            QCOMPARE(f.font().kerning(), format1.fontKerning());
    }

    {
        BOBUIextFormatCollection collection;
        collection.setDefaultFont(font1);

        BOBUIextCharFormat tmp;
        tmp.setFont(font1, overrideAll ? BOBUIextCharFormat::FontPropertiesAll
                                       : BOBUIextCharFormat::FontPropertiesSpecifiedOnly);
        int formatIndex = collection.indexForFormat(tmp);
        BOBUIextCharFormat f = collection.charFormat(formatIndex);

        QCOMPARE((int)f.fontStyleHint(), (int)format1.fontStyleHint());
        QCOMPARE((int)f.fontStyleStrategy(), (int)format1.fontStyleStrategy());
        QCOMPARE((int)f.fontCapitalization(), (int)format1.fontCapitalization());
        QCOMPARE(f.fontKerning(), format1.fontKerning());

        QCOMPARE((int)f.font().styleHint(), (int)f.fontStyleHint());
        QCOMPARE((int)f.font().styleStrategy(), (int)f.fontStyleStrategy());
        QCOMPARE((int)f.font().capitalization(), (int)f.fontCapitalization());
        QCOMPARE(f.font().kerning(), f.fontKerning());
    }
    {
        BOBUIextFormatCollection collection;
        collection.setDefaultFont(font1);

        BOBUIextCharFormat tmp;
        tmp.setFont(font2, overrideAll ? BOBUIextCharFormat::FontPropertiesAll
                                       : BOBUIextCharFormat::FontPropertiesSpecifiedOnly);
        int formatIndex = collection.indexForFormat(tmp);
        BOBUIextCharFormat f = collection.charFormat(formatIndex);

        if (overrideAll || (font2.resolveMask() & QFont::StyleHintResolved))
            QCOMPARE((int)f.font().styleHint(), (int)font2.styleHint());
        else
            QCOMPARE((int)f.font().styleHint(), (int)font1.styleHint());
        if (overrideAll || (font2.resolveMask() & QFont::StyleStrategyResolved))
            QCOMPARE((int)f.font().styleStrategy(), (int)font2.styleStrategy());
        else
            QCOMPARE((int)f.font().styleStrategy(), (int)font1.styleStrategy());
        if (overrideAll || (font2.resolveMask() & QFont::CapitalizationResolved))
            QCOMPARE((int)f.font().capitalization(), (int)font2.capitalization());
        else
            QCOMPARE((int)f.font().capitalization(), (int)font1.capitalization());
        if (overrideAll || (font2.resolveMask() & QFont::KerningResolved))
            QCOMPARE(f.font().kerning(), font2.kerning());
        else
            QCOMPARE(f.font().kerning(), font1.kerning());
    }
}

void tst_BOBUIextFormat::clearCollection()
{
    BOBUIextFormatCollection collection;
    QFont f;
    f.setUnderline(true);
    collection.setDefaultFont(f);
    BOBUIextCharFormat charFormat;
    charFormat.setFontStyleHint(QFont::SansSerif);
    int formatIndex = collection.indexForFormat(charFormat);
    QCOMPARE(formatIndex, 0);
    BOBUIextCharFormat charFormat2;
    charFormat2.setUnderlineStyle(BOBUIextCharFormat::SingleUnderline);
    int formatIndex2 = collection.indexForFormat(charFormat2);
    QCOMPARE(formatIndex2, 1);
    QCOMPARE(collection.formats.size(), 2);
    QCOMPARE(collection.hashes.size(), 2);
    QCOMPARE(collection.defaultFont(), f);

    collection.clear();
    QCOMPARE(collection.formats.size(), 0);
    QCOMPARE(collection.hashes.size(), 0);
    QCOMPARE(collection.indexForFormat(charFormat2), 0);
    QCOMPARE(collection.formats.size(), 1);
    QCOMPARE(collection.hashes.size(), 1);
    QCOMPARE(collection.defaultFont(), f); // kept, BOBUIextDocument::clear or setPlainText should not reset the font set by setDefaultFont
}

void tst_BOBUIextFormat::setFontFeatures()
{
    {
        QFont font;
        font.setFeature("abcd", 1234);
        font.setFeature("efgh", 5678);

        BOBUIextCharFormat format;
        format.setFont(font);

        QFont resolvedFont = format.font();
        QCOMPARE(resolvedFont.featureTags().size(), 2);
        QCOMPARE(resolvedFont.featureValue("abcd"), 1234);
        QCOMPARE(resolvedFont.featureValue("efgh"), 5678);

        QHash<QFont::Tag, quint32> features = format.fontFeatures();
        QCOMPARE(features.size(), 2);
        QCOMPARE(features.value("abcd"), 1234);
        QCOMPARE(features.value("efgh"), 5678);
    }

    {
        BOBUIextCharFormat format;

        QHash<QFont::Tag, quint32> features;
        features.insert("abcd", 4321);
        features.insert("efgh", 8765);
        format.setFontFeatures(features);

        QFont resolvedFont = format.font();
        QCOMPARE(resolvedFont.featureTags().size(), 2);
        QCOMPARE(resolvedFont.featureValue("abcd"), 4321);
        QCOMPARE(resolvedFont.featureValue("efgh"), 8765);

        features = format.fontFeatures();
        QCOMPARE(features.size(), 2);
        QCOMPARE(features.value("abcd"), 4321);
        QCOMPARE(features.value("efgh"), 8765);
    }
}

void tst_BOBUIextFormat::setFontVariableAxes()
{
    {
        QFont font;
        font.setVariableAxis("abcd", 12.25);
        font.setVariableAxis("efgh", 13.25);

        BOBUIextCharFormat format;
        format.setFont(font);

        QFont resolvedFont = format.font();
        QCOMPARE(resolvedFont.variableAxisTags().size(), 2);
        QCOMPARE(resolvedFont.variableAxisValue("abcd"), 12.25);
        QCOMPARE(resolvedFont.variableAxisValue("efgh"), 13.25);

        QHash<QFont::Tag, float> axes = format.fontVariableAxes();
        QCOMPARE(axes.size(), 2);
        QCOMPARE(axes.value("abcd"), 12.25);
        QCOMPARE(axes.value("efgh"), 13.25);
    }

    {
        BOBUIextCharFormat format;

        QHash<QFont::Tag, float> axes;
        axes.insert("abcd", 12.25);
        axes.insert("efgh", 13.25);
        format.setFontVariableAxes(axes);

        QFont resolvedFont = format.font();
        QCOMPARE(resolvedFont.variableAxisTags().size(), 2);
        QCOMPARE(resolvedFont.variableAxisValue("abcd"), 12.25);
        QCOMPARE(resolvedFont.variableAxisValue("efgh"), 13.25);

        axes = format.fontVariableAxes();
        QCOMPARE(axes.size(), 2);
        QCOMPARE(axes.value("abcd"), 12.25);
        QCOMPARE(axes.value("efgh"), 13.25);
    }

}

#ifndef BOBUI_NO_DATASTREAM
void tst_BOBUIextFormat::dataStreamCompatibility()
{
    // Make sure that we are still compatible with the old values of BOBUIextFormat::FontLetterSpacingType
    // and BOBUIextFormat::FontStretch, when used with earlier QDataStream versions
    BOBUIextCharFormat format;
    format.setFontStretch(42);
    format.setFontLetterSpacingType(QFont::AbsoluteSpacing);
    format.setFontFamilies({QLatin1String("Arial")});

    // Sanity check
    {
        QMap<int, QVariant> properties = format.properties();
        QVERIFY(properties.contains(BOBUIextFormat::FontLetterSpacingType));
        QVERIFY(properties.contains(BOBUIextFormat::FontStretch));
        QVERIFY(properties.contains(BOBUIextFormat::FontFamilies));
        QVERIFY(!properties.contains(BOBUIextFormat::OldFontLetterSpacingType));
        QVERIFY(!properties.contains(BOBUIextFormat::OldFontStretch));
        QVERIFY(!properties.contains(BOBUIextFormat::OldFontFamily));
    }

    QByteArray memory;

    // Current stream version
    {
        {
            QBuffer buffer(&memory);
            buffer.open(QIODevice::WriteOnly);

            QDataStream stream(&buffer);
            stream << format;
        }

        {
            QBuffer buffer(&memory);
            buffer.open(QIODevice::ReadOnly);

            QDataStream stream(&buffer);

            BOBUIextFormat other;
            stream >> other;

            {
                QMap<int, QVariant> properties = other.properties();
                QVERIFY(properties.contains(BOBUIextFormat::FontLetterSpacingType));
                QVERIFY(properties.contains(BOBUIextFormat::FontStretch));
                QVERIFY(properties.contains(BOBUIextFormat::FontFamilies));
                QVERIFY(!properties.contains(BOBUIextFormat::OldFontLetterSpacingType));
                QVERIFY(!properties.contains(BOBUIextFormat::OldFontStretch));
                QVERIFY(!properties.contains(BOBUIextFormat::OldFontFamily));
            }
        }

        {
            QBuffer buffer(&memory);
            buffer.open(QIODevice::ReadOnly);

            QDataStream stream(&buffer);

            quint32 type;
            stream >> type;

            QMap<qint32, QVariant> properties;
            stream >> properties;
            QVERIFY(properties.contains(BOBUIextFormat::FontLetterSpacingType));
            QVERIFY(properties.contains(BOBUIextFormat::FontStretch));
            QVERIFY(properties.contains(BOBUIextFormat::FontFamilies));
            QVERIFY(!properties.contains(BOBUIextFormat::OldFontLetterSpacingType));
            QVERIFY(!properties.contains(BOBUIextFormat::OldFontStretch));
            QVERIFY(!properties.contains(BOBUIextFormat::OldFontFamily));
        }
    }

    // BobUI 5.15 stream version
    memory.clear();
    {
        {
            QBuffer buffer(&memory);
            buffer.open(QIODevice::WriteOnly);

            QDataStream stream(&buffer);
            stream.setVersion(QDataStream::BobUI_5_15);
            stream << format;
        }

        {
            QBuffer buffer(&memory);
            buffer.open(QIODevice::ReadOnly);

            QDataStream stream(&buffer);
            stream.setVersion(QDataStream::BobUI_5_15);

            BOBUIextFormat other;
            stream >> other;

            {
                QMap<int, QVariant> properties = other.properties();
                QVERIFY(properties.contains(BOBUIextFormat::FontLetterSpacingType));
                QVERIFY(properties.contains(BOBUIextFormat::FontStretch));
                QVERIFY(properties.contains(BOBUIextFormat::FontFamilies));
                QVERIFY(!properties.contains(BOBUIextFormat::OldFontLetterSpacingType));
                QVERIFY(!properties.contains(BOBUIextFormat::OldFontStretch));
                QVERIFY(!properties.contains(BOBUIextFormat::OldFontFamily));
            }
        }

        {
            QBuffer buffer(&memory);
            buffer.open(QIODevice::ReadOnly);

            QDataStream stream(&buffer);
            stream.setVersion(QDataStream::BobUI_5_15);

            quint32 type;
            stream >> type;

            // Verify that old data stream still has the compatibility values
            QMap<qint32, QVariant> properties;
            stream >> properties;
            QVERIFY(!properties.contains(BOBUIextFormat::FontLetterSpacingType));
            QVERIFY(!properties.contains(BOBUIextFormat::FontStretch));
            QVERIFY(!properties.contains(BOBUIextFormat::FontFamilies));
            QVERIFY(properties.contains(BOBUIextFormat::OldFontLetterSpacingType));
            QVERIFY(properties.contains(BOBUIextFormat::OldFontStretch));
            QVERIFY(properties.contains(BOBUIextFormat::OldFontFamily));
        }
    }

    // Don't mix up FontFeatures and OldTextUnderlineColor
    memory.clear();
    {
        {
            QBuffer buffer(&memory);
            buffer.open(QIODevice::WriteOnly);

            QFont font;
            font.setFeature("abcd", 1234);

            BOBUIextCharFormat format;
            format.setFont(font);

            QDataStream stream(&buffer);

            stream << format;
        }

        {
            QBuffer buffer(&memory);
            buffer.open(QIODevice::ReadOnly);

            QDataStream stream(&buffer);

            BOBUIextFormat other;
            stream >> other;

            QMap<int, QVariant> properties = other.properties();
            QVERIFY(properties.contains(BOBUIextFormat::FontFeatures));

            auto features = other.property(BOBUIextFormat::FontFeatures).value<QHash<QFont::Tag, quint32>>();
            QCOMPARE(features.value("abcd"), 1234);
        }
    }

    memory.clear();
    {
        {
            QBuffer buffer(&memory);
            buffer.open(QIODevice::WriteOnly);

            QFont font;
            font.setFeature("abcd", 1234);

            BOBUIextCharFormat format;
            format.setFont(font);

            QDataStream stream(&buffer);
            stream.setVersion(QDataStream::BobUI_5_15);

            stream << format;
        }

        {
            QBuffer buffer(&memory);
            buffer.open(QIODevice::ReadOnly);

            QDataStream stream(&buffer);
            stream.setVersion(QDataStream::BobUI_5_15);

            BOBUIextFormat other;
            stream >> other;
            QMap<int, QVariant> properties = other.properties();
            QVERIFY(!properties.contains(BOBUIextFormat::FontFeatures));
            QVERIFY(!properties.contains(BOBUIextFormat::OldTextUnderlineColor));
        }
    }
}
#endif // BOBUI_NO_DATASTREAM

BOBUIEST_MAIN(tst_BOBUIextFormat)
#include "tst_bobuiextformat.moc"
