// Copyright (C) 2021 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef BOBUIEXTFORMAT_H
#define BOBUIEXTFORMAT_H

#include <BobUIGui/qbrush.h>
#include <BobUIGui/qcolor.h>
#include <BobUIGui/qfont.h>
#include <BobUIGui/qpen.h>
#include <BobUIGui/bobuiextoption.h>
#include <BobUIGui/bobuiguiglobal.h>

#include <BobUICore/qlist.h>
#include <BobUICore/qshareddata.h>
#include <BobUICore/qvariant.h>
#include <BobUICore/qhash.h>

BOBUI_BEGIN_NAMESPACE


class QString;
class QVariant;
class QFont;

class BOBUIextFormatCollection;
class BOBUIextFormatPrivate;
class BOBUIextBlockFormat;
class BOBUIextCharFormat;
class BOBUIextListFormat;
class BOBUIextTableFormat;
class BOBUIextFrameFormat;
class BOBUIextImageFormat;
class BOBUIextTableCellFormat;
class BOBUIextFormat;
class BOBUIextObject;
class BOBUIextCursor;
class BOBUIextDocument;
class BOBUIextLength;

#ifndef BOBUI_NO_DEBUG_STREAM
Q_GUI_EXPORT QDebug operator<<(QDebug, const BOBUIextLength &);
#endif

class Q_GUI_EXPORT BOBUIextLength
{
public:
    enum Type { VariableLength = 0, FixedLength, PercentageLength };

    inline BOBUIextLength() : lengthType(VariableLength), fixedValueOrPercentage(0) {}

    inline explicit BOBUIextLength(Type type, qreal value);

    inline Type type() const { return lengthType; }
    inline qreal value(qreal maximumLength) const
    {
        switch (lengthType) {
            case FixedLength: return fixedValueOrPercentage;
            case VariableLength: return maximumLength;
            case PercentageLength: return fixedValueOrPercentage * maximumLength / qreal(100);
        }
        return -1;
    }

    inline qreal rawValue() const { return fixedValueOrPercentage; }

    inline bool operator==(const BOBUIextLength &other) const
    { return lengthType == other.lengthType
             && qFuzzyCompare(fixedValueOrPercentage, other.fixedValueOrPercentage); }
    inline bool operator!=(const BOBUIextLength &other) const
    { return lengthType != other.lengthType
             || !qFuzzyCompare(fixedValueOrPercentage, other.fixedValueOrPercentage); }
    operator QVariant() const;

private:
    Type lengthType;
    qreal fixedValueOrPercentage;
    friend Q_GUI_EXPORT QDataStream &operator<<(QDataStream &, const BOBUIextLength &);
    friend Q_GUI_EXPORT QDataStream &operator>>(QDataStream &, BOBUIextLength &);
};
Q_DECLARE_TYPEINFO(BOBUIextLength, Q_PRIMITIVE_TYPE);

inline BOBUIextLength::BOBUIextLength(Type atype, qreal avalue)
    : lengthType(atype), fixedValueOrPercentage(avalue) {}

#ifndef BOBUI_NO_DEBUG_STREAM
Q_GUI_EXPORT QDebug operator<<(QDebug, const BOBUIextFormat &);
#endif

class Q_GUI_EXPORT BOBUIextFormat
{
    Q_GADGET
public:
    enum FormatType {
        InvalidFormat = -1,
        BlockFormat = 1,
        CharFormat = 2,
        ListFormat = 3,
        FrameFormat = 5,

        UserFormat = 100
    };
    Q_ENUM(FormatType)

    enum Property {
        ObjectIndex = 0x0,

        // paragraph and char
        CssFloat = 0x0800,
        LayoutDirection = 0x0801,

        OutlinePen = 0x810,
        BackgroundBrush = 0x820,
        ForegroundBrush = 0x821,
        // Internal to bobuiextlayout.cpp: ObjectSelectionBrush = 0x822
        BackgroundImageUrl = 0x823,

        // paragraph
        BlockAlignment = 0x1010,
        BlockTopMargin = 0x1030,
        BlockBottomMargin = 0x1031,
        BlockLeftMargin = 0x1032,
        BlockRightMargin = 0x1033,
        TextIndent = 0x1034,
        TabPositions = 0x1035,
        BlockIndent = 0x1040,
        LineHeight = 0x1048,
        LineHeightType = 0x1049,
        BlockNonBreakableLines = 0x1050,
        BlockTrailingHorizontalRulerWidth = 0x1060,
        HeadingLevel = 0x1070,
        BlockQuoteLevel = 0x1080,
        BlockCodeLanguage = 0x1090,
        BlockCodeFence = 0x1091,
        BlockMarker = 0x10A0,

        // character properties
        FirstFontProperty = 0x1FE0,
        FontCapitalization = FirstFontProperty,
        FontLetterSpacing = 0x1FE1,
        FontWordSpacing = 0x1FE2,
        FontStyleHint = 0x1FE3,
        FontStyleStrategy = 0x1FE4,
        FontKerning = 0x1FE5,
        FontHintingPreference = 0x1FE6,
        FontFamilies = 0x1FE7,
        FontStyleName = 0x1FE8,
        FontLetterSpacingType = 0x1FE9,
        FontStretch = 0x1FEA,
#if BOBUI_DEPRECATED_SINCE(6, 0)
        FontFamily = 0x2000,
#endif
        FontPointSize = 0x2001,
        FontSizeAdjustment = 0x2002,
        FontSizeIncrement = FontSizeAdjustment, // old name, compat
        FontWeight = 0x2003,
        FontItalic = 0x2004,
        FontUnderline = 0x2005, // deprecated, use TextUnderlineStyle instead
        FontOverline = 0x2006,
        FontStrikeOut = 0x2007,
        FontFixedPitch = 0x2008,
        FontPixelSize = 0x2009,
        FontFeatures = 0x2010, // Note: Same as OldTextUnderlineColor
        FontVariableAxes = 0x2011,
        LastFontProperty = FontVariableAxes,

        TextUnderlineColor = 0x2020,
        TextVerticalAlignment = 0x2021,
        TextOutline = 0x2022,
        TextUnderlineStyle = 0x2023,
        TextToolTip = 0x2024,
        TextSuperScriptBaseline = 0x2025,
        TextSubScriptBaseline = 0x2026,
        TextBaselineOffset = 0x2027,

        IsAnchor = 0x2030,
        AnchorHref = 0x2031,
        AnchorName = 0x2032,

        // Included for backwards compatibility with old QDataStreams.
        // Should not be referenced in user code.
        OldFontLetterSpacingType = 0x2033,
        OldFontStretch = 0x2034,
        OldTextUnderlineColor = 0x2010,
        OldFontFamily = 0x2000, // same as FontFamily

        ObjectType = 0x2f00,

        // list properties
        ListStyle = 0x3000,
        ListIndent = 0x3001,
        ListNumberPrefix = 0x3002,
        ListNumberSuffix = 0x3003,
        ListStart = 0x3004,

        // table and frame properties
        FrameBorder = 0x4000,
        FrameMargin = 0x4001,
        FramePadding = 0x4002,
        FrameWidth = 0x4003,
        FrameHeight = 0x4004,
        FrameTopMargin    = 0x4005,
        FrameBottomMargin = 0x4006,
        FrameLeftMargin   = 0x4007,
        FrameRightMargin  = 0x4008,
        FrameBorderBrush = 0x4009,
        FrameBorderStyle = 0x4010,

        TableColumns = 0x4100,
        TableColumnWidthConstraints = 0x4101,
        TableCellSpacing = 0x4102,
        TableCellPadding = 0x4103,
        TableHeaderRowCount = 0x4104,
        TableBorderCollapse = 0x4105,

        // table cell properties
        TableCellRowSpan = 0x4810,
        TableCellColumnSpan = 0x4811,

        TableCellTopPadding = 0x4812,
        TableCellBottomPadding = 0x4813,
        TableCellLeftPadding = 0x4814,
        TableCellRightPadding = 0x4815,

        TableCellTopBorder = 0x4816,
        TableCellBottomBorder = 0x4817,
        TableCellLeftBorder = 0x4818,
        TableCellRightBorder = 0x4819,

        TableCellTopBorderStyle = 0x481a,
        TableCellBottomBorderStyle = 0x481b,
        TableCellLeftBorderStyle = 0x481c,
        TableCellRightBorderStyle = 0x481d,

        TableCellTopBorderBrush = 0x481e,
        TableCellBottomBorderBrush = 0x481f,
        TableCellLeftBorderBrush = 0x4820,
        TableCellRightBorderBrush = 0x4821,

        // image properties
        ImageName = 0x5000,
        ImageTitle = 0x5001,
        ImageAltText = 0x5002,
        ImageWidth = 0x5010,
        ImageHeight = 0x5011,
        ImageQuality = 0x5014,
        ImageMaxWidth = 0x5015,

        // internal
        /*
           SuppressText = 0x5012,
           SuppressBackground = 0x513
        */

        // selection properties
        FullWidthSelection = 0x06000,

        // page break properties
        PageBreakPolicy = 0x7000,

        // --
        UserProperty = 0x100000
    };
    Q_ENUM(Property)

    enum ObjectTypes {
        NoObject,
        ImageObject,
        TableObject,
        TableCellObject,

        UserObject = 0x1000
    };
    Q_ENUM(ObjectTypes)

    enum PageBreakFlag {
        PageBreak_Auto = 0,
        PageBreak_AlwaysBefore = 0x001,
        PageBreak_AlwaysAfter  = 0x010
        // PageBreak_AlwaysInside = 0x100
    };
    Q_DECLARE_FLAGS(PageBreakFlags, PageBreakFlag)

    BOBUIextFormat();

    explicit BOBUIextFormat(int type);

    BOBUIextFormat(const BOBUIextFormat &rhs);
    BOBUIextFormat &operator=(const BOBUIextFormat &rhs);
    ~BOBUIextFormat();

    void swap(BOBUIextFormat &other)
    { d.swap(other.d); std::swap(format_type, other.format_type); }

    void merge(const BOBUIextFormat &other);

    inline bool isValid() const { return type() != InvalidFormat; }
    inline bool isEmpty() const { return propertyCount() == 0; }

    int type() const;

    int objectIndex() const;
    void setObjectIndex(int object);

    QVariant property(int propertyId) const;
    void setProperty(int propertyId, const QVariant &value);
    void clearProperty(int propertyId);
    bool hasProperty(int propertyId) const;

    bool boolProperty(int propertyId) const;
    int intProperty(int propertyId) const;
    qreal doubleProperty(int propertyId) const;
    QString stringProperty(int propertyId) const;
    QColor colorProperty(int propertyId) const;
    QPen penProperty(int propertyId) const;
    QBrush brushProperty(int propertyId) const;
    BOBUIextLength lengthProperty(int propertyId) const;
    QList<BOBUIextLength> lengthVectorProperty(int propertyId) const;

    void setProperty(int propertyId, const QList<BOBUIextLength> &lengths);

    QMap<int, QVariant> properties() const;
    int propertyCount() const;

    inline void setObjectType(int type);
    inline int objectType() const
    { return intProperty(ObjectType); }

    inline bool isCharFormat() const { return type() == CharFormat; }
    inline bool isBlockFormat() const { return type() == BlockFormat; }
    inline bool isListFormat() const { return type() == ListFormat; }
    inline bool isFrameFormat() const { return type() == FrameFormat; }
    inline bool isImageFormat() const { return type() == CharFormat && objectType() == ImageObject; }
    inline bool isTableFormat() const { return type() == FrameFormat && objectType() == TableObject; }
    inline bool isTableCellFormat() const { return type() == CharFormat && objectType() == TableCellObject; }

    BOBUIextBlockFormat toBlockFormat() const;
    BOBUIextCharFormat toCharFormat() const;
    BOBUIextListFormat toListFormat() const;
    BOBUIextTableFormat toTableFormat() const;
    BOBUIextFrameFormat toFrameFormat() const;
    BOBUIextImageFormat toImageFormat() const;
    BOBUIextTableCellFormat toTableCellFormat() const;

    bool operator==(const BOBUIextFormat &rhs) const;
    inline bool operator!=(const BOBUIextFormat &rhs) const { return !operator==(rhs); }
    operator QVariant() const;

    inline void setLayoutDirection(BobUI::LayoutDirection direction)
        { setProperty(BOBUIextFormat::LayoutDirection, direction); }
    inline BobUI::LayoutDirection layoutDirection() const
        { return BobUI::LayoutDirection(intProperty(BOBUIextFormat::LayoutDirection)); }

    inline void setBackground(const QBrush &brush)
    { setProperty(BackgroundBrush, brush); }
    inline QBrush background() const
    { return brushProperty(BackgroundBrush); }
    inline void clearBackground()
    { clearProperty(BackgroundBrush); }

    inline void setForeground(const QBrush &brush)
    { setProperty(ForegroundBrush, brush); }
    inline QBrush foreground() const
    { return brushProperty(ForegroundBrush); }
    inline void clearForeground()
    { clearProperty(ForegroundBrush); }

private:
    QSharedDataPointer<BOBUIextFormatPrivate> d;
    qint32 format_type;

    friend class BOBUIextFormatCollection;
    friend class BOBUIextCharFormat;
    friend Q_GUI_EXPORT QDataStream &operator<<(QDataStream &, const BOBUIextFormat &);
    friend Q_GUI_EXPORT QDataStream &operator>>(QDataStream &, BOBUIextFormat &);
};

Q_DECLARE_SHARED(BOBUIextFormat)

inline void BOBUIextFormat::setObjectType(int atype)
{ setProperty(ObjectType, atype); }

Q_DECLARE_OPERATORS_FOR_FLAGS(BOBUIextFormat::PageBreakFlags)

class Q_GUI_EXPORT BOBUIextCharFormat : public BOBUIextFormat
{
public:
    enum VerticalAlignment {
        AlignNormal = 0,
        AlignSuperScript,
        AlignSubScript,
        AlignMiddle,
        AlignTop,
        AlignBottom,
        AlignBaseline
    };
    enum UnderlineStyle { // keep in sync with BobUI::PenStyle!
        NoUnderline,
        SingleUnderline,
        DashUnderline,
        DotLine,
        DashDotLine,
        DashDotDotLine,
        WaveUnderline,
        SpellCheckUnderline
    };

    BOBUIextCharFormat();

    bool isValid() const { return isCharFormat(); }

    enum FontPropertiesInheritanceBehavior {
        FontPropertiesSpecifiedOnly,
        FontPropertiesAll
    };
    void setFont(const QFont &font, FontPropertiesInheritanceBehavior behavior = FontPropertiesAll);
    QFont font() const;

#if BOBUI_DEPRECATED_SINCE(6, 1)
    BOBUI_DEPRECATED_VERSION_X_6_1("Use setFontFamilies instead") inline void setFontFamily(const QString &family)
    { setProperty(FontFamilies, QVariant(QStringList(family))); }
    BOBUI_DEPRECATED_VERSION_X_6_1("Use fontFamilies instead") inline QString fontFamily() const
    { return property(FontFamilies).toStringList().first(); }
#endif

    inline void setFontFamilies(const QStringList &families)
    { setProperty(FontFamilies, QVariant(families)); }
#if BOBUI_VERSION < BOBUI_VERSION_CHECK(7, 0, 0)
    inline QVariant fontFamilies() const
    { return property(FontFamilies); }
#else
    inline QStringList fontFamilies() const
    { return property(FontFamilies).toStringList(); }
#endif

    inline void setFontStyleName(const QString &styleName)
    { setProperty(FontStyleName, styleName); }
#if BOBUI_VERSION < BOBUI_VERSION_CHECK(7, 0, 0)
    inline QVariant fontStyleName() const
    { return property(FontStyleName); }
#else
    inline QStringList fontStyleName() const
    { return property(FontStyleName).toStringList(); }
#endif

    inline void setFontPointSize(qreal size)
    { setProperty(FontPointSize, size); }
    inline qreal fontPointSize() const
    { return doubleProperty(FontPointSize); }

    inline void setFontWeight(int weight)
    { setProperty(FontWeight, weight); }
    inline int fontWeight() const
    { return hasProperty(FontWeight) ? intProperty(FontWeight) : QFont::Normal; }
    inline void setFontItalic(bool italic)
    { setProperty(FontItalic, italic); }
    inline bool fontItalic() const
    { return boolProperty(FontItalic); }
    inline void setFontCapitalization(QFont::Capitalization capitalization)
    { setProperty(FontCapitalization, capitalization); }
    inline QFont::Capitalization fontCapitalization() const
    { return static_cast<QFont::Capitalization>(intProperty(FontCapitalization)); }
    inline void setFontLetterSpacingType(QFont::SpacingType letterSpacingType)
    { setProperty(FontLetterSpacingType, letterSpacingType); }
    inline QFont::SpacingType fontLetterSpacingType() const
    { return static_cast<QFont::SpacingType>(intProperty(FontLetterSpacingType)); }
    inline void setFontLetterSpacing(qreal spacing)
    { setProperty(FontLetterSpacing, spacing); }
    inline qreal fontLetterSpacing() const
    { return doubleProperty(FontLetterSpacing); }
    inline void setFontWordSpacing(qreal spacing)
    { setProperty(FontWordSpacing, spacing); }
    inline qreal fontWordSpacing() const
    { return doubleProperty(FontWordSpacing); }

    inline void setFontUnderline(bool underline)
    { setProperty(TextUnderlineStyle, underline ? SingleUnderline : NoUnderline); }
    bool fontUnderline() const;

    inline void setFontOverline(bool overline)
    { setProperty(FontOverline, overline); }
    inline bool fontOverline() const
    { return boolProperty(FontOverline); }

    inline void setFontStrikeOut(bool strikeOut)
    { setProperty(FontStrikeOut, strikeOut); }
    inline bool fontStrikeOut() const
    { return boolProperty(FontStrikeOut); }

    inline void setUnderlineColor(const QColor &color)
    { setProperty(TextUnderlineColor, color); }
    inline QColor underlineColor() const
    { return colorProperty(TextUnderlineColor); }

    inline void setFontFixedPitch(bool fixedPitch)
    { setProperty(FontFixedPitch, fixedPitch); }
    inline bool fontFixedPitch() const
    { return boolProperty(FontFixedPitch); }

    inline void setFontStretch(int factor)
    { setProperty(FontStretch, factor); }
    inline int fontStretch() const
    { return intProperty(FontStretch); }

    inline void setFontStyleHint(QFont::StyleHint hint, QFont::StyleStrategy strategy = QFont::PreferDefault)
    { setProperty(FontStyleHint, hint); setProperty(FontStyleStrategy, strategy); }
    inline void setFontStyleStrategy(QFont::StyleStrategy strategy)
    { setProperty(FontStyleStrategy, strategy); }
    QFont::StyleHint fontStyleHint() const
    { return static_cast<QFont::StyleHint>(intProperty(FontStyleHint)); }
    QFont::StyleStrategy fontStyleStrategy() const
    { return static_cast<QFont::StyleStrategy>(intProperty(FontStyleStrategy)); }

    inline void setFontHintingPreference(QFont::HintingPreference hintingPreference)
    {
        setProperty(FontHintingPreference, hintingPreference);
    }

    inline QFont::HintingPreference fontHintingPreference() const
    {
        return static_cast<QFont::HintingPreference>(intProperty(FontHintingPreference));
    }

    void setFontFeatures(const QHash<QFont::Tag, quint32> &fontFeatures);
    QHash<QFont::Tag, quint32> fontFeatures() const;
    void setFontVariableAxes(const QHash<QFont::Tag, float> &fontVariableAxes);
    QHash<QFont::Tag, float> fontVariableAxes() const;

    inline void setFontKerning(bool enable)
    { setProperty(FontKerning, enable); }
    inline bool fontKerning() const
    { return boolProperty(FontKerning); }

    void setUnderlineStyle(UnderlineStyle style);
    inline UnderlineStyle underlineStyle() const
    { return static_cast<UnderlineStyle>(intProperty(TextUnderlineStyle)); }

    inline void setVerticalAlignment(VerticalAlignment alignment)
    { setProperty(TextVerticalAlignment, alignment); }
    inline VerticalAlignment verticalAlignment() const
    { return static_cast<VerticalAlignment>(intProperty(TextVerticalAlignment)); }

    inline void setTextOutline(const QPen &pen)
    { setProperty(TextOutline, pen); }
    inline QPen textOutline() const
    { return penProperty(TextOutline); }

    inline void setToolTip(const QString &tip)
    { setProperty(TextToolTip, tip); }
    inline QString toolTip() const
    { return stringProperty(TextToolTip); }

    inline void setSuperScriptBaseline(qreal baseline)
    { setProperty(TextSuperScriptBaseline, baseline); }
    inline qreal superScriptBaseline() const
    { return hasProperty(TextSuperScriptBaseline) ? doubleProperty(TextSuperScriptBaseline) : 50.0; }

    inline void setSubScriptBaseline(qreal baseline)
    { setProperty(TextSubScriptBaseline, baseline); }
    inline qreal subScriptBaseline() const
    { return hasProperty(TextSubScriptBaseline) ? doubleProperty(TextSubScriptBaseline) : 100.0 / 6.0; }

    inline void setBaselineOffset(qreal baseline)
    { setProperty(TextBaselineOffset, baseline); }
    inline qreal baselineOffset() const
    { return hasProperty(TextBaselineOffset) ? doubleProperty(TextBaselineOffset) : 0.0; }

    inline void setAnchor(bool anchor)
    { setProperty(IsAnchor, anchor); }
    inline bool isAnchor() const
    { return boolProperty(IsAnchor); }

    inline void setAnchorHref(const QString &value)
    { setProperty(AnchorHref, value); }
    inline QString anchorHref() const
    { return stringProperty(AnchorHref); }

    inline void setAnchorNames(const QStringList &names)
    { setProperty(AnchorName, names); }
    QStringList anchorNames() const;

    inline void setTableCellRowSpan(int tableCellRowSpan);
    inline int tableCellRowSpan() const
    { int s = intProperty(TableCellRowSpan); if (s == 0) s = 1; return s; }
    inline void setTableCellColumnSpan(int tableCellColumnSpan);
    inline int tableCellColumnSpan() const
    { int s = intProperty(TableCellColumnSpan); if (s == 0) s = 1; return s; }

protected:
    explicit BOBUIextCharFormat(const BOBUIextFormat &fmt);
    friend class BOBUIextFormat;
    friend Q_GUI_EXPORT QDataStream &operator<<(QDataStream &, const BOBUIextCharFormat &);
    friend Q_GUI_EXPORT QDataStream &operator>>(QDataStream &, BOBUIextCharFormat &);
};

Q_DECLARE_SHARED(BOBUIextCharFormat)

inline void BOBUIextCharFormat::setTableCellRowSpan(int _tableCellRowSpan)
{
    if (_tableCellRowSpan <= 1)
        clearProperty(TableCellRowSpan); // the getter will return 1 here.
    else
        setProperty(TableCellRowSpan, _tableCellRowSpan);
}

inline void BOBUIextCharFormat::setTableCellColumnSpan(int _tableCellColumnSpan)
{
    if (_tableCellColumnSpan <= 1)
        clearProperty(TableCellColumnSpan); // the getter will return 1 here.
    else
        setProperty(TableCellColumnSpan, _tableCellColumnSpan);
}

class Q_GUI_EXPORT BOBUIextBlockFormat : public BOBUIextFormat
{
public:
    enum LineHeightTypes {
        SingleHeight = 0,
        ProportionalHeight = 1,
        FixedHeight = 2,
        MinimumHeight = 3,
        LineDistanceHeight = 4
    };

    enum class MarkerType {
        NoMarker = 0,
        Unchecked = 1,
        Checked = 2
    };

    BOBUIextBlockFormat();

    bool isValid() const { return isBlockFormat(); }

    inline void setAlignment(BobUI::Alignment alignment);
    inline BobUI::Alignment alignment() const
    { int a = intProperty(BlockAlignment); if (a == 0) a = BobUI::AlignLeft; return QFlag(a); }

    inline void setTopMargin(qreal margin)
    { setProperty(BlockTopMargin, margin); }
    inline qreal topMargin() const
    { return doubleProperty(BlockTopMargin); }

    inline void setBottomMargin(qreal margin)
    { setProperty(BlockBottomMargin, margin); }
    inline qreal bottomMargin() const
    { return doubleProperty(BlockBottomMargin); }

    inline void setLeftMargin(qreal margin)
    { setProperty(BlockLeftMargin, margin); }
    inline qreal leftMargin() const
    { return doubleProperty(BlockLeftMargin); }

    inline void setRightMargin(qreal margin)
    { setProperty(BlockRightMargin, margin); }
    inline qreal rightMargin() const
    { return doubleProperty(BlockRightMargin); }

    inline void setTextIndent(qreal aindent)
    { setProperty(TextIndent, aindent); }
    inline qreal textIndent() const
    { return doubleProperty(TextIndent); }

    inline void setIndent(int indent);
    inline int indent() const
    { return intProperty(BlockIndent); }

    inline void setHeadingLevel(int alevel)
    { setProperty(HeadingLevel, alevel); }
    inline int headingLevel() const
    { return intProperty(HeadingLevel); }

    inline void setLineHeight(qreal height, int heightType)
    { setProperty(LineHeight, height); setProperty(LineHeightType, heightType); }
    inline qreal lineHeight(qreal scriptLineHeight, qreal scaling) const;
    inline qreal lineHeight() const
    { return doubleProperty(LineHeight); }
    inline int lineHeightType() const
    { return intProperty(LineHeightType); }

    inline void setNonBreakableLines(bool b)
    { setProperty(BlockNonBreakableLines, b); }
    inline bool nonBreakableLines() const
    { return boolProperty(BlockNonBreakableLines); }

    inline void setPageBreakPolicy(PageBreakFlags flags)
    { setProperty(PageBreakPolicy, int(flags.toInt())); }
    inline PageBreakFlags pageBreakPolicy() const
    { return PageBreakFlags(intProperty(PageBreakPolicy)); }

    void setTabPositions(const QList<BOBUIextOption::Tab> &tabs);
    QList<BOBUIextOption::Tab> tabPositions() const;

    inline void setMarker(MarkerType marker)
    { setProperty(BlockMarker, int(marker)); }
    inline MarkerType marker() const
    { return MarkerType(intProperty(BlockMarker)); }

protected:
    explicit BOBUIextBlockFormat(const BOBUIextFormat &fmt);
    friend class BOBUIextFormat;
    friend Q_GUI_EXPORT QDataStream &operator<<(QDataStream &, const BOBUIextBlockFormat &);
    friend Q_GUI_EXPORT QDataStream &operator>>(QDataStream &, BOBUIextBlockFormat &);
};

Q_DECLARE_SHARED(BOBUIextBlockFormat)

inline void BOBUIextBlockFormat::setAlignment(BobUI::Alignment aalignment)
{ setProperty(BlockAlignment, int(aalignment.toInt())); }

inline void BOBUIextBlockFormat::setIndent(int aindent)
{ setProperty(BlockIndent, aindent); }

inline qreal BOBUIextBlockFormat::lineHeight(qreal scriptLineHeight, qreal scaling = 1.0) const
{
  switch(intProperty(LineHeightType)) {
    case SingleHeight:
      return(scriptLineHeight);
    case ProportionalHeight:
      return(scriptLineHeight * doubleProperty(LineHeight) / 100.0);
    case FixedHeight:
      return(doubleProperty(LineHeight) * scaling);
    case MinimumHeight:
      return(qMax(scriptLineHeight, doubleProperty(LineHeight) * scaling));
    case LineDistanceHeight:
      return(scriptLineHeight + doubleProperty(LineHeight) * scaling);
  }
  return(0);
}

class Q_GUI_EXPORT BOBUIextListFormat : public BOBUIextFormat
{
public:
    BOBUIextListFormat();

    bool isValid() const { return isListFormat(); }

    enum Style {
        ListDisc = -1,
        ListCircle = -2,
        ListSquare = -3,
        ListDecimal = -4,
        ListLowerAlpha = -5,
        ListUpperAlpha = -6,
        ListLowerRoman = -7,
        ListUpperRoman = -8,
        ListStyleUndefined = 0
    };

    inline void setStyle(Style style);
    inline Style style() const
    { return static_cast<Style>(intProperty(ListStyle)); }

    inline void setIndent(int indent);
    inline int indent() const
    { return intProperty(ListIndent); }

    inline void setNumberPrefix(const QString &numberPrefix);
    inline QString numberPrefix() const
    { return stringProperty(ListNumberPrefix); }

    inline void setNumberSuffix(const QString &numberSuffix);
    inline QString numberSuffix() const
    { return stringProperty(ListNumberSuffix); }

    inline void setStart(int indent);
    inline int start() const { return intProperty(ListStart); }

protected:
    explicit BOBUIextListFormat(const BOBUIextFormat &fmt);
    friend class BOBUIextFormat;
};

Q_DECLARE_SHARED(BOBUIextListFormat)

inline void BOBUIextListFormat::setStyle(Style astyle)
{ setProperty(ListStyle, astyle); }

inline void BOBUIextListFormat::setIndent(int aindent)
{ setProperty(ListIndent, aindent); }

inline void BOBUIextListFormat::setNumberPrefix(const QString &np)
{ setProperty(ListNumberPrefix, np); }

inline void BOBUIextListFormat::setNumberSuffix(const QString &ns)
{ setProperty(ListNumberSuffix, ns); }

inline void BOBUIextListFormat::setStart(int astart)
{
    setProperty(ListStart, astart);
}

class Q_GUI_EXPORT BOBUIextImageFormat : public BOBUIextCharFormat
{
public:
    BOBUIextImageFormat();

    bool isValid() const { return isImageFormat(); }

    inline void setName(const QString &name);
    inline QString name() const
    { return stringProperty(ImageName); }

    inline void setWidth(qreal width);
    inline qreal width() const
    { return doubleProperty(ImageWidth); }

    inline void setMaximumWidth(BOBUIextLength maxWidth);
    inline BOBUIextLength maximumWidth() const
    { return lengthProperty(ImageMaxWidth); }

    inline void setHeight(qreal height);
    inline qreal height() const
    { return doubleProperty(ImageHeight); }

    inline void setQuality(int quality);
#if BOBUI_DEPRECATED_SINCE(6, 3)
    BOBUI_DEPRECATED_VERSION_X_6_3("Pass a quality value, the default is 100") inline void setQuality()
    { setQuality(100); }
#endif
    inline int quality() const
    { return intProperty(ImageQuality); }

protected:
    explicit BOBUIextImageFormat(const BOBUIextFormat &format);
    friend class BOBUIextFormat;
    friend Q_GUI_EXPORT QDataStream &operator<<(QDataStream &, const BOBUIextListFormat &);
    friend Q_GUI_EXPORT QDataStream &operator>>(QDataStream &, BOBUIextListFormat &);
};

Q_DECLARE_SHARED(BOBUIextImageFormat)

inline void BOBUIextImageFormat::setName(const QString &aname)
{ setProperty(ImageName, aname); }

inline void BOBUIextImageFormat::setWidth(qreal awidth)
{ setProperty(ImageWidth, awidth); }

inline void BOBUIextImageFormat::setMaximumWidth(BOBUIextLength maxWidth)
{ setProperty(ImageMaxWidth, maxWidth); }

inline void BOBUIextImageFormat::setHeight(qreal aheight)
{ setProperty(ImageHeight, aheight); }

inline void BOBUIextImageFormat::setQuality(int aquality)
{ setProperty(ImageQuality, aquality); }

class Q_GUI_EXPORT BOBUIextFrameFormat : public BOBUIextFormat
{
public:
    BOBUIextFrameFormat();

    bool isValid() const { return isFrameFormat(); }

    enum Position {
        InFlow,
        FloatLeft,
        FloatRight
        // ######
//        Absolute
    };

    enum BorderStyle {
        BorderStyle_None,
        BorderStyle_Dotted,
        BorderStyle_Dashed,
        BorderStyle_Solid,
        BorderStyle_Double,
        BorderStyle_DotDash,
        BorderStyle_DotDotDash,
        BorderStyle_Groove,
        BorderStyle_Ridge,
        BorderStyle_Inset,
        BorderStyle_Outset
    };

    inline void setPosition(Position f)
    { setProperty(CssFloat, f); }
    inline Position position() const
    { return static_cast<Position>(intProperty(CssFloat)); }

    inline void setBorder(qreal border);
    inline qreal border() const
    { return doubleProperty(FrameBorder); }

    inline void setBorderBrush(const QBrush &brush)
    { setProperty(FrameBorderBrush, brush); }
    inline QBrush borderBrush() const
    { return brushProperty(FrameBorderBrush); }

    inline void setBorderStyle(BorderStyle style)
    { setProperty(FrameBorderStyle, style); }
    inline BorderStyle borderStyle() const
    { return static_cast<BorderStyle>(intProperty(FrameBorderStyle)); }

    void setMargin(qreal margin);
    inline qreal margin() const
    { return doubleProperty(FrameMargin); }

    inline void setTopMargin(qreal margin);
    qreal topMargin() const;

    inline void setBottomMargin(qreal margin);
    qreal bottomMargin() const;

    inline void setLeftMargin(qreal margin);
    qreal leftMargin() const;

    inline void setRightMargin(qreal margin);
    qreal rightMargin() const;

    inline void setPadding(qreal padding);
    inline qreal padding() const
    { return doubleProperty(FramePadding); }

    inline void setWidth(qreal width);
    inline void setWidth(const BOBUIextLength &length)
    { setProperty(FrameWidth, length); }
    inline BOBUIextLength width() const
    { return lengthProperty(FrameWidth); }

    inline void setHeight(qreal height);
    inline void setHeight(const BOBUIextLength &height);
    inline BOBUIextLength height() const
    { return lengthProperty(FrameHeight); }

    inline void setPageBreakPolicy(PageBreakFlags flags)
    { setProperty(PageBreakPolicy, int(flags.toInt())); }
    inline PageBreakFlags pageBreakPolicy() const
    { return PageBreakFlags(intProperty(PageBreakPolicy)); }

protected:
    explicit BOBUIextFrameFormat(const BOBUIextFormat &fmt);
    friend class BOBUIextFormat;
    friend Q_GUI_EXPORT QDataStream &operator<<(QDataStream &, const BOBUIextFrameFormat &);
    friend Q_GUI_EXPORT QDataStream &operator>>(QDataStream &, BOBUIextFrameFormat &);
};

Q_DECLARE_SHARED(BOBUIextFrameFormat)

inline void BOBUIextFrameFormat::setBorder(qreal aborder)
{ setProperty(FrameBorder, aborder); }

inline void BOBUIextFrameFormat::setPadding(qreal apadding)
{ setProperty(FramePadding, apadding); }

inline void BOBUIextFrameFormat::setWidth(qreal awidth)
{ setProperty(FrameWidth, BOBUIextLength(BOBUIextLength::FixedLength, awidth)); }

inline void BOBUIextFrameFormat::setHeight(qreal aheight)
{ setProperty(FrameHeight, BOBUIextLength(BOBUIextLength::FixedLength, aheight)); }
inline void BOBUIextFrameFormat::setHeight(const BOBUIextLength &aheight)
{ setProperty(FrameHeight, aheight); }

inline void BOBUIextFrameFormat::setTopMargin(qreal amargin)
{ setProperty(FrameTopMargin, amargin); }

inline void BOBUIextFrameFormat::setBottomMargin(qreal amargin)
{ setProperty(FrameBottomMargin, amargin); }

inline void BOBUIextFrameFormat::setLeftMargin(qreal amargin)
{ setProperty(FrameLeftMargin, amargin); }

inline void BOBUIextFrameFormat::setRightMargin(qreal amargin)
{ setProperty(FrameRightMargin, amargin); }

class Q_GUI_EXPORT BOBUIextTableFormat : public BOBUIextFrameFormat
{
public:
    BOBUIextTableFormat();

    inline bool isValid() const { return isTableFormat(); }

    inline int columns() const
    { int cols = intProperty(TableColumns); if (cols == 0) cols = 1; return cols; }
    inline void setColumns(int columns);

    inline void setColumnWidthConstraints(const QList<BOBUIextLength> &constraints)
    { setProperty(TableColumnWidthConstraints, constraints); }

    inline QList<BOBUIextLength> columnWidthConstraints() const
    { return lengthVectorProperty(TableColumnWidthConstraints); }

    inline void clearColumnWidthConstraints()
    { clearProperty(TableColumnWidthConstraints); }

    inline qreal cellSpacing() const
    { return doubleProperty(TableCellSpacing); }
    inline void setCellSpacing(qreal spacing)
    { setProperty(TableCellSpacing, spacing); }

    inline qreal cellPadding() const
    { return doubleProperty(TableCellPadding); }
    inline void setCellPadding(qreal padding);

    inline void setAlignment(BobUI::Alignment alignment);
    inline BobUI::Alignment alignment() const
    { return QFlag(intProperty(BlockAlignment)); }

    inline void setHeaderRowCount(int count)
    { setProperty(TableHeaderRowCount, count); }
    inline int headerRowCount() const
    { return intProperty(TableHeaderRowCount); }

    inline void setBorderCollapse(bool borderCollapse)
    { setProperty(TableBorderCollapse, borderCollapse); }
    inline bool borderCollapse() const
    { return boolProperty(TableBorderCollapse); }

protected:
    explicit BOBUIextTableFormat(const BOBUIextFormat &fmt);
    friend class BOBUIextFormat;
};

Q_DECLARE_SHARED(BOBUIextTableFormat)

inline void BOBUIextTableFormat::setColumns(int acolumns)
{
    if (acolumns == 1)
        acolumns = 0;
    setProperty(TableColumns, acolumns);
}

inline void BOBUIextTableFormat::setCellPadding(qreal apadding)
{ setProperty(TableCellPadding, apadding); }

inline void BOBUIextTableFormat::setAlignment(BobUI::Alignment aalignment)
{ setProperty(BlockAlignment, int(aalignment.toInt())); }

class Q_GUI_EXPORT BOBUIextTableCellFormat : public BOBUIextCharFormat
{
public:
    BOBUIextTableCellFormat();

    inline bool isValid() const { return isTableCellFormat(); }

    inline void setTopPadding(qreal padding);
    inline qreal topPadding() const;

    inline void setBottomPadding(qreal padding);
    inline qreal bottomPadding() const;

    inline void setLeftPadding(qreal padding);
    inline qreal leftPadding() const;

    inline void setRightPadding(qreal padding);
    inline qreal rightPadding() const;

    inline void setPadding(qreal padding);

    inline void setTopBorder(qreal width)
    { setProperty(TableCellTopBorder, width); }
    inline qreal topBorder() const
    { return doubleProperty(TableCellTopBorder); }

    inline void setBottomBorder(qreal width)
    { setProperty(TableCellBottomBorder, width); }
    inline qreal bottomBorder() const
    { return doubleProperty(TableCellBottomBorder); }

    inline void setLeftBorder(qreal width)
    { setProperty(TableCellLeftBorder, width); }
    inline qreal leftBorder() const
    { return doubleProperty(TableCellLeftBorder); }

    inline void setRightBorder(qreal width)
    { setProperty(TableCellRightBorder, width); }
    inline qreal rightBorder() const
    { return doubleProperty(TableCellRightBorder); }

    inline void setBorder(qreal width);

    inline void setTopBorderStyle(BOBUIextFrameFormat::BorderStyle style)
    { setProperty(TableCellTopBorderStyle, style); }
    inline BOBUIextFrameFormat::BorderStyle topBorderStyle() const
    { return static_cast<BOBUIextFrameFormat::BorderStyle>(intProperty(TableCellTopBorderStyle)); }

    inline void setBottomBorderStyle(BOBUIextFrameFormat::BorderStyle style)
    { setProperty(TableCellBottomBorderStyle, style); }
    inline BOBUIextFrameFormat::BorderStyle bottomBorderStyle() const
    { return static_cast<BOBUIextFrameFormat::BorderStyle>(intProperty(TableCellBottomBorderStyle)); }

    inline void setLeftBorderStyle(BOBUIextFrameFormat::BorderStyle style)
    { setProperty(TableCellLeftBorderStyle, style); }
    inline BOBUIextFrameFormat::BorderStyle leftBorderStyle() const
    { return static_cast<BOBUIextFrameFormat::BorderStyle>(intProperty(TableCellLeftBorderStyle)); }

    inline void setRightBorderStyle(BOBUIextFrameFormat::BorderStyle style)
    { setProperty(TableCellRightBorderStyle, style); }
    inline BOBUIextFrameFormat::BorderStyle rightBorderStyle() const
    { return static_cast<BOBUIextFrameFormat::BorderStyle>(intProperty(TableCellRightBorderStyle)); }

    inline void setBorderStyle(BOBUIextFrameFormat::BorderStyle style);

    inline void setTopBorderBrush(const QBrush &brush)
    { setProperty(TableCellTopBorderBrush, brush); }
    inline QBrush topBorderBrush() const
    { return brushProperty(TableCellTopBorderBrush); }

    inline void setBottomBorderBrush(const QBrush &brush)
    { setProperty(TableCellBottomBorderBrush, brush); }
    inline QBrush bottomBorderBrush() const
    { return brushProperty(TableCellBottomBorderBrush); }

    inline void setLeftBorderBrush(const QBrush &brush)
    { setProperty(TableCellLeftBorderBrush, brush); }
    inline QBrush leftBorderBrush() const
    { return brushProperty(TableCellLeftBorderBrush); }

    inline void setRightBorderBrush(const QBrush &brush)
    { setProperty(TableCellRightBorderBrush, brush); }
    inline QBrush rightBorderBrush() const
    { return brushProperty(TableCellRightBorderBrush); }

    inline void setBorderBrush(const QBrush &brush);

protected:
    explicit BOBUIextTableCellFormat(const BOBUIextFormat &fmt);
    friend Q_GUI_EXPORT QDataStream &operator<<(QDataStream &, const BOBUIextTableCellFormat &);
    friend Q_GUI_EXPORT QDataStream &operator>>(QDataStream &, BOBUIextTableCellFormat &);
    friend class BOBUIextFormat;
};

Q_DECLARE_SHARED(BOBUIextTableCellFormat)

inline void BOBUIextTableCellFormat::setTopPadding(qreal padding)
{
    setProperty(TableCellTopPadding, padding);
}

inline qreal BOBUIextTableCellFormat::topPadding() const
{
    return doubleProperty(TableCellTopPadding);
}

inline void BOBUIextTableCellFormat::setBottomPadding(qreal padding)
{
    setProperty(TableCellBottomPadding, padding);
}

inline qreal BOBUIextTableCellFormat::bottomPadding() const
{
    return doubleProperty(TableCellBottomPadding);
}

inline void BOBUIextTableCellFormat::setLeftPadding(qreal padding)
{
    setProperty(TableCellLeftPadding, padding);
}

inline qreal BOBUIextTableCellFormat::leftPadding() const
{
    return doubleProperty(TableCellLeftPadding);
}

inline void BOBUIextTableCellFormat::setRightPadding(qreal padding)
{
    setProperty(TableCellRightPadding, padding);
}

inline qreal BOBUIextTableCellFormat::rightPadding() const
{
    return doubleProperty(TableCellRightPadding);
}

inline void BOBUIextTableCellFormat::setPadding(qreal padding)
{
    setTopPadding(padding);
    setBottomPadding(padding);
    setLeftPadding(padding);
    setRightPadding(padding);
}

inline void BOBUIextTableCellFormat::setBorder(qreal width)
{
    setTopBorder(width);
    setBottomBorder(width);
    setLeftBorder(width);
    setRightBorder(width);
}

inline void BOBUIextTableCellFormat::setBorderStyle(BOBUIextFrameFormat::BorderStyle style)
{
    setTopBorderStyle(style);
    setBottomBorderStyle(style);
    setLeftBorderStyle(style);
    setRightBorderStyle(style);
}

inline void BOBUIextTableCellFormat::setBorderBrush(const QBrush &brush)
{
    setTopBorderBrush(brush);
    setBottomBorderBrush(brush);
    setLeftBorderBrush(brush);
    setRightBorderBrush(brush);
}

BOBUI_END_NAMESPACE

#endif // BOBUIEXTFORMAT_H
