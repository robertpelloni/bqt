// Copyright (C) 2021 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "bobuiextformat.h"
#include "bobuiextformat_p.h"

#include <qvariant.h>
#include <qdatastream.h>
#include <qdebug.h>
#include <qmap.h>
#include <qhashfunctions.h>

BOBUI_BEGIN_NAMESPACE

/*!
    \class BOBUIextLength
    \reentrant

    \brief The BOBUIextLength class encapsulates the different types of length
    used in a BOBUIextDocument.
    \inmodule BobUIGui

    \ingroup richtext-processing

    When we specify a value for the length of an element in a text document,
    we often need to provide some other information so that the length is
    used in the way we expect. For example, when we specify a table width,
    the value can represent a fixed number of pixels, or it can be a percentage
    value. This information changes both the meaning of the value and the way
    it is used.

    Generally, this class is used to specify table widths. These can be
    specified either as a fixed amount of pixels, as a percentage of the
    containing frame's width, or by a variable width that allows it to take
    up just the space it requires.

    \sa BOBUIextTable
*/

/*!
    \fn explicit BOBUIextLength::BOBUIextLength()

    Constructs a new length object which represents a variable size.
*/

/*!
    \fn BOBUIextLength::BOBUIextLength(Type type, qreal value)

    Constructs a new length object of the given \a type and \a value.
*/

/*!
    \fn Type BOBUIextLength::type() const

    Returns the type of this length object.

    \sa BOBUIextLength::Type
*/

/*!
    \fn qreal BOBUIextLength::value(qreal maximumLength) const

    Returns the effective length, constrained by the type of the length object
    and the specified \a maximumLength.

    \sa type()
*/

/*!
    \fn qreal BOBUIextLength::rawValue() const

    Returns the constraint value that is specific for the type of the length.
    If the length is BOBUIextLength::PercentageLength then the raw value is in
    percent, in the range of 0 to 100. If the length is BOBUIextLength::FixedLength
    then that fixed amount is returned. For variable lengths, zero is returned.
*/

/*!
    \fn bool BOBUIextLength::operator==(const BOBUIextLength &other) const

    Returns \c true if this text length is the same as the \a other text
    length.
*/

/*!
    \fn bool BOBUIextLength::operator!=(const BOBUIextLength &other) const

    Returns \c true if this text length is different from the \a other text
    length.
*/

/*!
    \enum BOBUIextLength::Type

    This enum describes the different types a length object can
    have.

    \value VariableLength The width of the object is variable
    \value FixedLength The width of the object is fixed
    \value PercentageLength The width of the object is in
                            percentage of the maximum width

    \sa type()
*/

/*!
   Returns the text length as a QVariant
*/
BOBUIextLength::operator QVariant() const
{
    return QVariant::fromValue(*this);
}

#ifndef BOBUI_NO_DATASTREAM
QDataStream &operator<<(QDataStream &stream, const BOBUIextLength &length)
{
    return stream << qint32(length.lengthType) << double(length.fixedValueOrPercentage);
}

QDataStream &operator>>(QDataStream &stream, BOBUIextLength &length)
{
    qint32 type;
    double fixedValueOrPercentage;
    stream >> type >> fixedValueOrPercentage;
    length.fixedValueOrPercentage = fixedValueOrPercentage;
    length.lengthType = BOBUIextLength::Type(type);
    return stream;
}
#endif // BOBUI_NO_DATASTREAM

namespace {
struct Property
{
    inline Property(qint32 k, const QVariant &v) : key(k), value(v) {}
    inline Property() {}

    qint32 key = -1;
    QVariant value;

    inline bool operator==(const Property &other) const
    { return key == other.key && value == other.value; }
};
}
Q_DECLARE_TYPEINFO(Property, Q_RELOCATABLE_TYPE);

class BOBUIextFormatPrivate : public QSharedData
{
public:
    BOBUIextFormatPrivate() : hashDirty(true), fontDirty(true), hashValue(0) {}

    inline size_t hash() const
    {
        if (!hashDirty)
            return hashValue;
        return recalcHash();
    }

    inline bool operator==(const BOBUIextFormatPrivate &rhs) const {
        if (hash() != rhs.hash())
            return false;

        return props == rhs.props;
    }

    inline void insertProperty(qint32 key, const QVariant &value)
    {
        hashDirty = true;
        if (key >= BOBUIextFormat::FirstFontProperty && key <= BOBUIextFormat::LastFontProperty)
            fontDirty = true;

        for (int i = 0; i < props.size(); ++i)
            if (props.at(i).key == key) {
                props[i].value = value;
                return;
            }
        props.append(Property(key, value));
    }

    inline void clearProperty(qint32 key)
    {
        for (int i = 0; i < props.size(); ++i)
            if (props.at(i).key == key) {
                hashDirty = true;
                if (key >= BOBUIextFormat::FirstFontProperty && key <= BOBUIextFormat::LastFontProperty)
                    fontDirty = true;
                props.remove(i);
                return;
            }
    }

    inline int propertyIndex(qint32 key) const
    {
        for (int i = 0; i < props.size(); ++i)
            if (props.at(i).key == key)
                return i;
        return -1;
    }

    inline QVariant property(qint32 key) const
    {
        const int idx = propertyIndex(key);
        if (idx < 0)
            return QVariant();
        return props.at(idx).value;
    }

    inline bool hasProperty(qint32 key) const
    { return propertyIndex(key) != -1; }

    void resolveFont(const QFont &defaultFont);

    inline const QFont &font() const {
        if (fontDirty)
            recalcFont();
        return fnt;
    }

    QList<Property> props;
private:

    size_t recalcHash() const;
    void recalcFont() const;

    mutable bool hashDirty;
    mutable bool fontDirty;
    mutable size_t hashValue;
    mutable QFont fnt;

    friend QDataStream &operator<<(QDataStream &, const BOBUIextFormat &);
    friend QDataStream &operator>>(QDataStream &, BOBUIextFormat &);
};

static inline size_t hash(const QColor &color)
{
    return (color.isValid()) ? color.rgba() : 0x234109;
}

static inline size_t hash(const QPen &pen)
{
    return hash(pen.color()) + qHash(pen.widthF());
}

static inline size_t hash(const QBrush &brush)
{
    return hash(brush.color()) + (brush.style() << 3);
}

static inline size_t variantHash(const QVariant &variant)
{
    // simple and fast hash functions to differentiate between type and value
    switch (variant.userType()) { // sorted by occurrence frequency
    case QMetaType::QString: return qHash(variant.toString());
    case QMetaType::Double: return qHash(variant.toDouble());
    case QMetaType::Int: return 0x811890U + variant.toInt();
    case QMetaType::QBrush:
        return 0x01010101 + hash(qvariant_cast<QBrush>(variant));
    case QMetaType::Bool: return 0x371818 + variant.toBool();
    case QMetaType::QPen: return 0x02020202 + hash(qvariant_cast<QPen>(variant));
    case QMetaType::QVariantList:
        return 0x8377U + qvariant_cast<QVariantList>(variant).size();
    case QMetaType::QColor: return hash(qvariant_cast<QColor>(variant));
      case QMetaType::BOBUIextLength:
        return 0x377 + hash(qvariant_cast<BOBUIextLength>(variant).rawValue());
    case QMetaType::Float: return qHash(variant.toFloat());
    case QMetaType::UnknownType: return 0;
    default: break;
    }
    return qHash(variant.typeName());
}

static inline size_t getHash(const BOBUIextFormatPrivate *d, int format)
{
    return (d ? d->hash() : 0) + format;
}

size_t BOBUIextFormatPrivate::recalcHash() const
{
    hashValue = 0;
    const auto end = props.constEnd();
    for (auto it = props.constBegin(); it != end; ++it)
        hashValue += (static_cast<quint32>(it->key) << 16) + variantHash(it->value);

    hashDirty = false;

    return hashValue;
}

void BOBUIextFormatPrivate::resolveFont(const QFont &defaultFont)
{
    recalcFont();
    const uint oldMask = fnt.resolveMask();
    fnt = fnt.resolve(defaultFont);

    if (hasProperty(BOBUIextFormat::FontSizeAdjustment)) {
        const qreal scaleFactors[7] = {qreal(0.7), qreal(0.8), qreal(1.0), qreal(1.2), qreal(1.5), qreal(2), qreal(2.4)};

        const int htmlFontSize = qBound(0, property(BOBUIextFormat::FontSizeAdjustment).toInt() + 3 - 1, 6);


        if (defaultFont.pointSize() <= 0) {
            qreal pixelSize = scaleFactors[htmlFontSize] * defaultFont.pixelSize();
            fnt.setPixelSize(qRound(pixelSize));
        } else {
            qreal pointSize = scaleFactors[htmlFontSize] * defaultFont.pointSizeF();
            fnt.setPointSizeF(pointSize);
        }
    }

    fnt.setResolveMask(oldMask);
}

void BOBUIextFormatPrivate::recalcFont() const
{
    // update cached font as well
    QFont f;

    bool hasSpacingInformation = false;
    QFont::SpacingType spacingType = QFont::PercentageSpacing;
    qreal letterSpacing = 0.0;

    for (int i = 0; i < props.size(); ++i) {
        switch (props.at(i).key) {
            case BOBUIextFormat::FontFamilies:
                f.setFamilies(props.at(i).value.toStringList());
                break;
            case BOBUIextFormat::FontStyleName:
                f.setStyleName(props.at(i).value.toString());
                break;
            case BOBUIextFormat::FontPointSize:
                f.setPointSizeF(props.at(i).value.toReal());
                break;
            case  BOBUIextFormat::FontPixelSize:
                f.setPixelSize(props.at(i).value.toInt());
                break;
            case BOBUIextFormat::FontWeight: {
                const QVariant weightValue = props.at(i).value;
                int weight = weightValue.toInt();
                if (weight >= 0 && weightValue.isValid())
                    f.setWeight(QFont::Weight(weight));
                break; }
            case BOBUIextFormat::FontItalic:
                f.setItalic(props.at(i).value.toBool());
                break;
            case BOBUIextFormat::FontUnderline:
                if (! hasProperty(BOBUIextFormat::TextUnderlineStyle)) // don't use the old one if the new one is there.
                    f.setUnderline(props.at(i).value.toBool());
                break;
            case BOBUIextFormat::TextUnderlineStyle:
                f.setUnderline(static_cast<BOBUIextCharFormat::UnderlineStyle>(props.at(i).value.toInt()) == BOBUIextCharFormat::SingleUnderline);
                break;
            case BOBUIextFormat::FontOverline:
                f.setOverline(props.at(i).value.toBool());
                break;
            case BOBUIextFormat::FontStrikeOut:
                f.setStrikeOut(props.at(i).value.toBool());
                break;
            case BOBUIextFormat::FontLetterSpacingType:
                spacingType = static_cast<QFont::SpacingType>(props.at(i).value.toInt());
                hasSpacingInformation = true;
                break;
            case BOBUIextFormat::FontLetterSpacing:
                letterSpacing = props.at(i).value.toReal();
                hasSpacingInformation = true;
                break;
            case BOBUIextFormat::FontWordSpacing:
                f.setWordSpacing(props.at(i).value.toReal());
                break;
            case BOBUIextFormat::FontCapitalization:
                f.setCapitalization(static_cast<QFont::Capitalization> (props.at(i).value.toInt()));
                break;
            case BOBUIextFormat::FontFixedPitch: {
                const bool value = props.at(i).value.toBool();
                if (f.fixedPitch() != value)
                    f.setFixedPitch(value);
                break; }
            case BOBUIextFormat::FontStretch:
                f.setStretch(props.at(i).value.toInt());
                break;
            case BOBUIextFormat::FontStyleHint:
                f.setStyleHint(static_cast<QFont::StyleHint>(props.at(i).value.toInt()), f.styleStrategy());
                break;
            case BOBUIextFormat::FontHintingPreference:
                f.setHintingPreference(static_cast<QFont::HintingPreference>(props.at(i).value.toInt()));
                break;
            case BOBUIextFormat::FontStyleStrategy:
                f.setStyleStrategy(static_cast<QFont::StyleStrategy>(props.at(i).value.toInt()));
                break;
            case BOBUIextFormat::FontKerning:
                f.setKerning(props.at(i).value.toBool());
                break;
            case BOBUIextFormat::FontFeatures:
            {
                const auto fontFeatures = props.at(i).value.value<QHash<QFont::Tag, quint32>>();
                for (auto it = fontFeatures.constBegin(); it != fontFeatures.constEnd(); ++it)
                    f.setFeature(it.key(), it.value());
                break;
            }
            case BOBUIextFormat::FontVariableAxes:
            {
                const auto fontVariableAxes = props.at(i).value.value<QHash<QFont::Tag, float>>();
                for (auto it = fontVariableAxes.constBegin(); it != fontVariableAxes.constEnd(); ++it)
                    f.setVariableAxis(it.key(), it.value());
                break;
            }
            default:
                break;
            }
    }

    if (hasSpacingInformation)
        f.setLetterSpacing(spacingType, letterSpacing);

    fnt = f;
    fontDirty = false;
}

#ifndef BOBUI_NO_DATASTREAM
Q_GUI_EXPORT QDataStream &operator<<(QDataStream &stream, const BOBUIextFormat &fmt)
{
    QMap<int, QVariant> properties = fmt.properties();
    if (stream.version() < QDataStream::BobUI_6_11) {
        auto it = properties.constFind(BOBUIextFormat::FontFeatures);
        if (it != properties.cend())
            properties.erase(it);

        it = properties.constFind(BOBUIextFormat::FontVariableAxes);
        if (it != properties.cend())
            properties.erase(it);
    }

    if (stream.version() < QDataStream::BobUI_6_0) {
        auto it = properties.constFind(BOBUIextFormat::FontLetterSpacingType);
        if (it != properties.cend()) {
            properties[BOBUIextFormat::OldFontLetterSpacingType] = it.value();
            properties.erase(it);
        }

        it = properties.constFind(BOBUIextFormat::FontStretch);
        if (it != properties.cend()) {
            properties[BOBUIextFormat::OldFontStretch] = it.value();
            properties.erase(it);
        }

        it = properties.constFind(BOBUIextFormat::TextUnderlineColor);
        if (it != properties.cend()) {
            properties[BOBUIextFormat::OldTextUnderlineColor] = it.value();
            properties.erase(it);
        }

        it = properties.constFind(BOBUIextFormat::FontFamilies);
        if (it != properties.cend()) {
            properties[BOBUIextFormat::OldFontFamily] = QVariant(it.value().toStringList().constFirst());
            properties.erase(it);
        }
    }

    stream << fmt.format_type << properties;
    return stream;
}

Q_GUI_EXPORT QDataStream &operator>>(QDataStream &stream, BOBUIextFormat &fmt)
{
    QMap<qint32, QVariant> properties;
    stream >> fmt.format_type >> properties;

    // BOBUIextFormat's default constructor doesn't allocate the private structure, so
    // we have to do this, in case fmt is a default constructed value.
    if (!fmt.d)
        fmt.d = new BOBUIextFormatPrivate();

    for (QMap<qint32, QVariant>::ConstIterator it = properties.constBegin();
         it != properties.constEnd(); ++it) {
        qint32 key = it.key();

        if (stream.version() < QDataStream::BobUI_6_0) {
            if (key == BOBUIextFormat::OldFontLetterSpacingType)
                key = BOBUIextFormat::FontLetterSpacingType;
            else if (key == BOBUIextFormat::OldFontStretch)
                key = BOBUIextFormat::FontStretch;
            else if (key == BOBUIextFormat::OldTextUnderlineColor)
                key = BOBUIextFormat::TextUnderlineColor;
            else if (key == BOBUIextFormat::OldFontFamily)
                key = BOBUIextFormat::FontFamilies;
        }
        fmt.d->insertProperty(key, it.value());
    }

    return stream;
}

Q_GUI_EXPORT QDataStream &operator<<(QDataStream &stream, const BOBUIextCharFormat &fmt)
{
    return stream << static_cast<const BOBUIextFormat &>(fmt);
}

Q_GUI_EXPORT QDataStream &operator>>(QDataStream &stream, BOBUIextCharFormat &fmt)
{
    return stream >> static_cast<BOBUIextFormat &>(fmt);
}

Q_GUI_EXPORT QDataStream &operator<<(QDataStream &stream, const BOBUIextBlockFormat &fmt)
{
    return stream << static_cast<const BOBUIextFormat &>(fmt);
}

Q_GUI_EXPORT QDataStream &operator>>(QDataStream &stream, BOBUIextBlockFormat &fmt)
{
    return stream >> static_cast<BOBUIextFormat &>(fmt);
}

Q_GUI_EXPORT QDataStream &operator<<(QDataStream &stream, const BOBUIextListFormat &fmt)
{
    return stream << static_cast<const BOBUIextFormat &>(fmt);
}

Q_GUI_EXPORT QDataStream &operator>>(QDataStream &stream, BOBUIextListFormat &fmt)
{
    return stream >> static_cast<BOBUIextFormat &>(fmt);
}

Q_GUI_EXPORT QDataStream &operator<<(QDataStream &stream, const BOBUIextFrameFormat &fmt)
{
    return stream << static_cast<const BOBUIextFormat &>(fmt);
}

Q_GUI_EXPORT QDataStream &operator>>(QDataStream &stream, BOBUIextFrameFormat &fmt)
{
    return stream >> static_cast<BOBUIextFormat &>(fmt);
}

Q_GUI_EXPORT QDataStream &operator<<(QDataStream &stream, const BOBUIextTableCellFormat &fmt)
{
    return stream << static_cast<const BOBUIextFormat &>(fmt);
}

Q_GUI_EXPORT QDataStream &operator>>(QDataStream &stream, BOBUIextTableCellFormat &fmt)
{
    return stream >> static_cast<BOBUIextFormat &>(fmt);
}
#endif // BOBUI_NO_DATASTREAM

/*!
    \class BOBUIextFormat
    \reentrant

    \brief The BOBUIextFormat class provides formatting information for a
    BOBUIextDocument.
    \inmodule BobUIGui

    \ingroup richtext-processing
    \ingroup shared

    A BOBUIextFormat is a generic class used for describing the format of
    parts of a BOBUIextDocument. The derived classes BOBUIextCharFormat,
    BOBUIextBlockFormat, BOBUIextListFormat, and BOBUIextTableFormat are usually
    more useful, and describe the formatting that is applied to
    specific parts of the document.

    A format has a \c FormatType which specifies the kinds of text item it
    can format; e.g. a block of text, a list, a table, etc. A format
    also has various properties (some specific to particular format
    types), as described by the Property enum. Every property has a
    corresponding Property.

    The format type is given by type(), and the format can be tested
    with isCharFormat(), isBlockFormat(), isListFormat(),
    isTableFormat(), isFrameFormat(), and isImageFormat(). If the
    type is determined, it can be retrieved with toCharFormat(),
    toBlockFormat(), toListFormat(), toTableFormat(), toFrameFormat(),
    and toImageFormat().

    A format's properties can be set with the setProperty() functions,
    and retrieved with boolProperty(), intProperty(), doubleProperty(),
    and stringProperty() as appropriate. All the property IDs used in
    the format can be retrieved with allPropertyIds(). One format can
    be merged into another using merge().

    A format's object index can be set with setObjectIndex(), and
    retrieved with objectIndex(). These methods can be used to
    associate the format with a BOBUIextObject. It is used to represent
    lists, frames, and tables inside the document.

    \sa {Rich Text Processing}
*/

/*!
    \enum BOBUIextFormat::FormatType

    This enum describes the text item a BOBUIextFormat object is formatting.

    \value InvalidFormat An invalid format as created by the default
                         constructor
    \value BlockFormat The object formats a text block
    \value CharFormat The object formats a single character
    \value ListFormat The object formats a list
    \value FrameFormat The object formats a frame

    \value UserFormat

    \sa BOBUIextCharFormat, BOBUIextBlockFormat, BOBUIextListFormat,
    BOBUIextTableFormat, type()
*/

/*!
    \enum BOBUIextFormat::Property

    This enum describes the different properties a format can have.

    \value ObjectIndex The index of the formatted object. See objectIndex().

    Paragraph and character properties

    \value CssFloat How a frame is located relative to the surrounding text
    \value LayoutDirection  The layout direction of the text in the document
                            (BobUI::LayoutDirection).

    \value OutlinePen
    \value ForegroundBrush
    \value BackgroundBrush
    \value BackgroundImageUrl

    Paragraph properties

    \value BlockAlignment
    \value BlockTopMargin
    \value BlockBottomMargin
    \value BlockLeftMargin
    \value BlockRightMargin
    \value TextIndent
    \value TabPositions     Specifies the tab positions.  The tab positions are structs of BOBUIextOption::Tab which are stored in
                            a QList (internally, in a QList<QVariant>).
    \value BlockIndent
    \value LineHeight
    \value LineHeightType
    \value BlockNonBreakableLines
    \value BlockTrailingHorizontalRulerWidth The width of a horizontal ruler element.
    \value HeadingLevel     The level of a heading, for example 1 corresponds to an HTML H1 tag; otherwise 0.
                            This enum value has been added in BobUI 5.12.
    \value BlockCodeFence   The character that was used in the "fences" around a Markdown code block.
                            If the code block was indented rather than fenced, the block should not have this property.
                            This enum value has been added in BobUI 5.14.

    \value BlockQuoteLevel  The depth of nested quoting on this block: 1 means the block is a top-level block quote.
                            Blocks that are not block quotes should not have this property.
                            This enum value has been added in BobUI 5.14.
    \value BlockCodeLanguage The programming language in a preformatted or code block.
                            Blocks that do not contain code should not have this property.
                            This enum value has been added in BobUI 5.14.
    \value BlockMarker      The \l{BOBUIextBlockFormat::MarkerType}{type of adornment} to be shown alongside the block.
                            This enum value has been added in BobUI 5.14.

    Character properties

    \value FontFamily e{This property has been deprecated.} Use BOBUIextFormat::FontFamilies instead.
    \omitvalue OldFontFamily
    \value FontFamilies
    \value FontStyleName
    \value FontPointSize
    \value FontPixelSize
    \value FontSizeAdjustment       Specifies an integer adjustment added to the base font size set using
                                    \c FontPointSize or \c FontPixelSize.
    \value FontFixedPitch
    \omitvalue FontSizeIncrement
    \value FontWeight
    \value FontItalic
    \value FontUnderline \e{This property has been deprecated.} Use BOBUIextFormat::TextUnderlineStyle instead.
    \value FontOverline
    \value FontStrikeOut
    \value FontCapitalization Specifies the capitalization type that is to be applied to the text.
    \value FontLetterSpacingType Specifies the meaning of the FontLetterSpacing property. The default
                                 is QFont::PercentageSpacing.
    \value FontLetterSpacing Changes the default spacing between individual letters in the font. The value is
                             specified as a percentage or absolute value, depending on FontLetterSpacingType.
                             The default value is 100%.
    \value FontWordSpacing  Changes the default spacing between individual words. A positive value increases the word spacing
                                                 by the corresponding pixels; a negative value decreases the spacing.
    \value FontStretch          Corresponds to the QFont::Stretch property
    \value FontStyleHint        Corresponds to the QFont::StyleHint property
    \value FontStyleStrategy    Corresponds to the QFont::StyleStrategy property
    \value FontKerning          Specifies whether the font has kerning turned on.
    \value FontHintingPreference Controls the use of hinting according to values
                                 of the QFont::HintingPreference enum.
    \value FontFeatures [since 6.11] Assigns integer numbers to typographical features. See
           \l{QFont::setFeature()} for additional information.
    \value FontVariableAxes [since 6.11] Assigns floating point numbers to variable axes in variable
           fonts. See \l{QFont::setVariableAxis()} for additional information.

    \omitvalue FirstFontProperty
    \omitvalue LastFontProperty

    \value TextUnderlineColor      Specifies the color to draw underlines, overlines and strikeouts.
    \value TextVerticalAlignment   Specifies the type of text vertical alignment according to
                                   the values of the BOBUIextCharFormat::VerticalAlignment enum.
    \value TextOutline             Specifies a \l QPen used to draw the text outline.
    \value TextUnderlineStyle      Specifies the style of text underline according to
                                   the values of the BOBUIextCharFormat::UnderlineStyle enum.
    \value TextToolTip Specifies the (optional) tool tip to be displayed for a fragment of text.
    \value TextSuperScriptBaseline Specifies the baseline (in % of height) of superscript texts.
    \value TextSubScriptBaseline   Specifies the baseline (in % of height) of subscript texts.
    \value TextBaselineOffset      Specifies the baseline (in % of height) of text. A positive value moves up the text,
                                   by the corresponding %; a negative value moves it down.

    \value IsAnchor
    \value AnchorHref
    \value AnchorName
    \omitvalue OldFontLetterSpacingType
    \omitvalue OldFontStretch
    \omitvalue OldTextUnderlineColor
    \value ObjectType

    List properties

    \value ListStyle        Specifies the style used for the items in a list,
                            described by values of the BOBUIextListFormat::Style enum.
    \value ListIndent       Specifies the amount of indentation used for a list.
    \value ListNumberPrefix Defines the text which is prepended to item numbers in
                            numeric lists.
    \value ListNumberSuffix Defines the text which is appended to item numbers in
                            numeric lists.
    \value [since 6.6] ListStart
                            Defines the first value of a list.

    Table and frame properties

    \value FrameBorder
    \value FrameBorderBrush
    \value FrameBorderStyle See the \l{BOBUIextFrameFormat::BorderStyle}{BorderStyle} enum.
    \value FrameBottomMargin
    \value FrameHeight
    \value FrameLeftMargin
    \value FrameMargin
    \value FramePadding
    \value FrameRightMargin
    \value FrameTopMargin
    \value FrameWidth
    \value TableCellSpacing
    \value TableCellPadding
    \value TableColumns
    \value TableColumnWidthConstraints
    \value TableHeaderRowCount
    \value TableBorderCollapse Specifies the \l BOBUIextTableFormat::borderCollapse property.

    Table cell properties

    \value TableCellRowSpan
    \value TableCellColumnSpan
    \value TableCellLeftPadding
    \value TableCellRightPadding
    \value TableCellTopPadding
    \value TableCellBottomPadding

    Table cell properties intended for use with \l BOBUIextTableFormat::borderCollapse enabled

    \value TableCellTopBorder
    \value TableCellBottomBorder
    \value TableCellLeftBorder
    \value TableCellRightBorder

    \value TableCellTopBorderStyle
    \value TableCellBottomBorderStyle
    \value TableCellLeftBorderStyle
    \value TableCellRightBorderStyle

    \value TableCellTopBorderBrush
    \value TableCellBottomBorderBrush
    \value TableCellLeftBorderBrush
    \value TableCellRightBorderBrush

    Image properties

    \value ImageName        The filename or source of the image.
    \value ImageTitle       The title attribute of an HTML image tag, or
                            the quoted string that comes after the URL in a Markdown image link.
                            This enum value has been added in BobUI 5.14.
    \value ImageAltText     The alt attribute of an HTML image tag, or
                            the image description in a Markdown image link.
                            This enum value has been added in BobUI 5.14.
    \value ImageWidth
    \value ImageHeight
    \value ImageQuality
    \value ImageMaxWidth    This enum value has been added in BobUI 6.8.

    Selection properties

    \value FullWidthSelection When set on the characterFormat of a selection,
                              the whole width of the text will be shown selected.

    Page break properties

    \value PageBreakPolicy Specifies how pages are broken. See the PageBreakFlag enum.

    \value UserProperty

    \sa property(), setProperty()
*/

/*!
    \enum BOBUIextFormat::ObjectTypes

    This enum describes what kind of BOBUIextObject this format is associated with.

    \value NoObject
    \value ImageObject
    \value TableObject
    \value TableCellObject
    \value UserObject The first object that can be used for application-specific purposes.

    \sa BOBUIextObject, BOBUIextTable, BOBUIextObject::format()
*/

/*!
    \enum BOBUIextFormat::PageBreakFlag
    \since 4.2

    This enum describes how page breaking is performed when printing. It maps to the
    corresponding css properties.

    \value PageBreak_Auto The page break is determined automatically depending on the
                          available space on the current page
    \value PageBreak_AlwaysBefore The page is always broken before the paragraph/table
    \value PageBreak_AlwaysAfter  A new page is always started after the paragraph/table

    \sa BOBUIextBlockFormat::pageBreakPolicy(), BOBUIextFrameFormat::pageBreakPolicy(),
    PageBreakPolicy
*/

/*!
    \fn bool BOBUIextFormat::isValid() const

    Returns \c true if the format is valid (i.e. is not
    InvalidFormat); otherwise returns \c false.
*/

/*!
    \fn bool BOBUIextFormat::isEmpty() const
    \since 5.3

    Returns true if the format does not store any properties; false otherwise.

    \sa propertyCount(), properties()
*/

/*!
    \fn bool BOBUIextFormat::isCharFormat() const

    Returns \c true if this text format is a \c CharFormat; otherwise
    returns \c false.
*/


/*!
    \fn bool BOBUIextFormat::isBlockFormat() const

    Returns \c true if this text format is a \c BlockFormat; otherwise
    returns \c false.
*/


/*!
    \fn bool BOBUIextFormat::isListFormat() const

    Returns \c true if this text format is a \c ListFormat; otherwise
    returns \c false.
*/


/*!
    \fn bool BOBUIextFormat::isTableFormat() const

    Returns \c true if this text format is a \c TableFormat; otherwise
    returns \c false.
*/


/*!
    \fn bool BOBUIextFormat::isFrameFormat() const

    Returns \c true if this text format is a \c FrameFormat; otherwise
    returns \c false.
*/


/*!
    \fn bool BOBUIextFormat::isImageFormat() const

    Returns \c true if this text format is an image format; otherwise
    returns \c false.
*/


/*!
    \fn bool BOBUIextFormat::isTableCellFormat() const
    \since 4.4

    Returns \c true if this text format is a \c TableCellFormat; otherwise
    returns \c false.
*/


/*!
    Creates a new text format with an \c InvalidFormat.

    \sa FormatType
*/
BOBUIextFormat::BOBUIextFormat()
    : format_type(InvalidFormat)
{
}

/*!
    Creates a new text format of the given \a type.

    \sa FormatType
*/
BOBUIextFormat::BOBUIextFormat(int type)
    : format_type(type)
{
}


/*!
    \fn BOBUIextFormat::BOBUIextFormat(const BOBUIextFormat &other)

    Creates a new text format with the same attributes as the \a other
    text format.
*/
BOBUIextFormat::BOBUIextFormat(const BOBUIextFormat &rhs)
    : d(rhs.d), format_type(rhs.format_type)
{
}

/*!
    \fn BOBUIextFormat &BOBUIextFormat::operator=(const BOBUIextFormat &other)

    Assigns the \a other text format to this text format, and returns a
    reference to this text format.
*/
BOBUIextFormat &BOBUIextFormat::operator=(const BOBUIextFormat &rhs)
{
    d = rhs.d;
    format_type = rhs.format_type;
    return *this;
}

/*!
    \fn void BOBUIextFormat::swap(BOBUIextFormat &other)
    \since 5.0
    \memberswap{text format}
*/

/*!
    Destroys this text format.
*/
BOBUIextFormat::~BOBUIextFormat()
{
}


/*!
   Returns the text format as a QVariant
*/
BOBUIextFormat::operator QVariant() const
{
    return QVariant::fromValue(*this);
}

/*!
    Merges the \a other format with this format; where there are
    conflicts the \a other format takes precedence.
*/
void BOBUIextFormat::merge(const BOBUIextFormat &other)
{
    if (format_type != other.format_type)
        return;

    if (!d) {
        d = other.d;
        return;
    }

    if (!other.d)
        return;

    BOBUIextFormatPrivate *p = d.data();

    const QList<BOBUI_PREPEND_NAMESPACE(Property)> &otherProps = other.d.constData()->props;
    p->props.reserve(p->props.size() + otherProps.size());
    for (int i = 0; i < otherProps.size(); ++i) {
        const BOBUI_PREPEND_NAMESPACE(Property) &prop = otherProps.at(i);
        if (prop.value.isValid()) {
            p->insertProperty(prop.key, prop.value);
        } else {
            p->clearProperty(prop.key);
        }
    }
}

/*!
    Returns the type of this format.

    \sa FormatType
*/
int BOBUIextFormat::type() const
{
    return format_type;
}

/*!
    Returns this format as a block format.
*/
BOBUIextBlockFormat BOBUIextFormat::toBlockFormat() const
{
    return BOBUIextBlockFormat(*this);
}

/*!
    Returns this format as a character format.
*/
BOBUIextCharFormat BOBUIextFormat::toCharFormat() const
{
    return BOBUIextCharFormat(*this);
}

/*!
    Returns this format as a list format.
*/
BOBUIextListFormat BOBUIextFormat::toListFormat() const
{
    return BOBUIextListFormat(*this);
}

/*!
    Returns this format as a table format.
*/
BOBUIextTableFormat BOBUIextFormat::toTableFormat() const
{
    return BOBUIextTableFormat(*this);
}

/*!
    Returns this format as a frame format.
*/
BOBUIextFrameFormat BOBUIextFormat::toFrameFormat() const
{
    return BOBUIextFrameFormat(*this);
}

/*!
    Returns this format as an image format.
*/
BOBUIextImageFormat BOBUIextFormat::toImageFormat() const
{
    return BOBUIextImageFormat(*this);
}

/*!
    \since 4.4

    Returns this format as a table cell format.
*/
BOBUIextTableCellFormat BOBUIextFormat::toTableCellFormat() const
{
    return BOBUIextTableCellFormat(*this);
}

/*!
    Returns the value of the property specified by \a propertyId. If the
    property isn't of BOBUIextFormat::Bool type, false is returned instead.

    \sa setProperty(), intProperty(), doubleProperty(), stringProperty(), colorProperty(),
        lengthProperty(), lengthVectorProperty(), Property
*/
bool BOBUIextFormat::boolProperty(int propertyId) const
{
    if (!d)
        return false;
    const QVariant prop = d->property(propertyId);
    if (prop.userType() != QMetaType::Bool)
        return false;
    return prop.toBool();
}

/*!
    Returns the value of the property specified by \a propertyId. If the
    property is not of BOBUIextFormat::Integer type, 0 is returned instead.

    \sa setProperty(), boolProperty(), doubleProperty(), stringProperty(), colorProperty(),
        lengthProperty(), lengthVectorProperty(), Property
*/
int BOBUIextFormat::intProperty(int propertyId) const
{
    // required, since the default layout direction has to be LayoutDirectionAuto, which is not integer 0
    int def = (propertyId == BOBUIextFormat::LayoutDirection) ? int(BobUI::LayoutDirectionAuto) : 0;

    if (!d)
        return def;
    const QVariant prop = d->property(propertyId);
    if (prop.userType() != QMetaType::Int)
        return def;
    return prop.toInt();
}

/*!
    Returns the value of the property specified by \a propertyId. If the
    property isn't of QMetaType::Double or QMetaType::Float type, 0 is
    returned instead.

    \sa setProperty(), boolProperty(), intProperty(), stringProperty(), colorProperty(),
        lengthProperty(), lengthVectorProperty(), Property
*/
qreal BOBUIextFormat::doubleProperty(int propertyId) const
{
    if (!d)
        return 0.;
    const QVariant prop = d->property(propertyId);
    if (prop.userType() != QMetaType::Double && prop.userType() != QMetaType::Float)
        return 0.;
    return qvariant_cast<qreal>(prop);
}

/*!
    Returns the value of the property given by \a propertyId; if the
    property isn't of QMetaType::QString type, an empty string is
    returned instead.

    \sa setProperty(), boolProperty(), intProperty(), doubleProperty(), colorProperty(),
        lengthProperty(), lengthVectorProperty(), Property
*/
QString BOBUIextFormat::stringProperty(int propertyId) const
{
    if (!d)
        return QString();
    const QVariant prop = d->property(propertyId);
    if (prop.userType() != QMetaType::QString)
        return QString();
    return prop.toString();
}

/*!
    Returns the value of the property given by \a propertyId; if the
    property isn't of QMetaType::QColor type, an invalid color is
    returned instead.

    \sa setProperty(), boolProperty(), intProperty(), doubleProperty(),
        stringProperty(), lengthProperty(), lengthVectorProperty(), Property
*/
QColor BOBUIextFormat::colorProperty(int propertyId) const
{
    if (!d)
        return QColor();
    const QVariant prop = d->property(propertyId);
    if (prop.userType() != QMetaType::QColor)
        return QColor();
    return qvariant_cast<QColor>(prop);
}

/*!
    Returns the value of the property given by \a propertyId; if the
    property isn't of QMetaType::QPen type, BobUI::NoPen is
    returned instead.

    \sa setProperty(), boolProperty(), intProperty(), doubleProperty(), stringProperty(),
        lengthProperty(), lengthVectorProperty(), Property
*/
QPen BOBUIextFormat::penProperty(int propertyId) const
{
    if (!d)
        return QPen(BobUI::NoPen);
    const QVariant prop = d->property(propertyId);
    if (prop.userType() != QMetaType::QPen)
        return QPen(BobUI::NoPen);
    return qvariant_cast<QPen>(prop);
}

/*!
    Returns the value of the property given by \a propertyId; if the
    property isn't of QMetaType::QBrush type, BobUI::NoBrush is
    returned instead.

    \sa setProperty(), boolProperty(), intProperty(), doubleProperty(), stringProperty(),
        lengthProperty(), lengthVectorProperty(), Property
*/
QBrush BOBUIextFormat::brushProperty(int propertyId) const
{
    if (!d)
        return QBrush(BobUI::NoBrush);
    const QVariant prop = d->property(propertyId);
    if (prop.userType() != QMetaType::QBrush)
        return QBrush(BobUI::NoBrush);
    return qvariant_cast<QBrush>(prop);
}

/*!
    Returns the value of the property given by \a propertyId.

    \sa setProperty(), boolProperty(), intProperty(), doubleProperty(), stringProperty(),
        colorProperty(), lengthVectorProperty(), Property
*/
BOBUIextLength BOBUIextFormat::lengthProperty(int propertyId) const
{
    if (!d)
        return BOBUIextLength();
    return qvariant_cast<BOBUIextLength>(d->property(propertyId));
}

/*!
    Returns the value of the property given by \a propertyId. If the
    property isn't of BOBUIextFormat::LengthVector type, an empty
    list is returned instead.

    \sa setProperty(), boolProperty(), intProperty(), doubleProperty(), stringProperty(),
        colorProperty(), lengthProperty(), Property
*/
QList<BOBUIextLength> BOBUIextFormat::lengthVectorProperty(int propertyId) const
{
    QList<BOBUIextLength> list;
    if (!d)
        return list;
    const QVariant prop = d->property(propertyId);
    if (prop.userType() != QMetaType::QVariantList)
        return list;

    const QList<QVariant> propertyList = prop.toList();
    for (const auto &var : propertyList) {
        if (var.userType() == QMetaType::BOBUIextLength)
            list.append(qvariant_cast<BOBUIextLength>(var));
    }

    return list;
}

/*!
    Returns the property specified by the given \a propertyId.

    \sa Property
*/
QVariant BOBUIextFormat::property(int propertyId) const
{
    return d ? d->property(propertyId) : QVariant();
}

/*!
    Sets the property specified by the \a propertyId to the given \a value.

    \sa Property
*/
void BOBUIextFormat::setProperty(int propertyId, const QVariant &value)
{
    if (!d)
        d = new BOBUIextFormatPrivate;

    d->insertProperty(propertyId, value);
}

/*!
    Sets the value of the property given by \a propertyId to \a value.

    \sa lengthVectorProperty(), Property
*/
void BOBUIextFormat::setProperty(int propertyId, const QList<BOBUIextLength> &value)
{
    if (!d)
        d = new BOBUIextFormatPrivate;
    QVariantList list;
    const int numValues = value.size();
    list.reserve(numValues);
    for (int i = 0; i < numValues; ++i)
        list << value.at(i);
    d->insertProperty(propertyId, list);
}

/*!
    Clears the value of the property given by \a propertyId

    \sa Property
*/
void BOBUIextFormat::clearProperty(int propertyId)
{
    if (!d)
        return;
    d->clearProperty(propertyId);
}


/*!
    \fn void BOBUIextFormat::setObjectType(int type)

    Sets the text format's object type to \a type.

    \sa ObjectTypes, objectType()
*/


/*!
    \fn int BOBUIextFormat::objectType() const

    Returns the text format's object type.

    \sa ObjectTypes, setObjectType()
*/


/*!
    Returns the index of the format object, or -1 if the format object is invalid.

    \sa setObjectIndex()
*/
int BOBUIextFormat::objectIndex() const
{
    if (!d)
        return -1;
    const QVariant prop = d->property(ObjectIndex);
    if (prop.userType() != QMetaType::Int) // ####
        return -1;
    return prop.toInt();
}

/*!
    \fn void BOBUIextFormat::setObjectIndex(int index)

    Sets the format object's object \a index.

    \sa objectIndex()
*/
void BOBUIextFormat::setObjectIndex(int o)
{
    if (o == -1) {
        if (d.constData())
            d->clearProperty(ObjectIndex);
    } else {
        if (!d.constData())
            d = new BOBUIextFormatPrivate;
        // ### type
        d->insertProperty(ObjectIndex, o);
    }
}

/*!
    Returns \c true if the text format has a property with the given \a
    propertyId; otherwise returns \c false.

    \sa properties(), Property
*/
bool BOBUIextFormat::hasProperty(int propertyId) const
{
    return d ? d->hasProperty(propertyId) : false;
}

/*
    Returns the property type for the given \a propertyId.

    \sa hasProperty(), allPropertyIds(), Property
*/

/*!
    Returns a map with all properties of this text format.
*/
QMap<int, QVariant> BOBUIextFormat::properties() const
{
    QMap<int, QVariant> map;
    if (d) {
        for (int i = 0; i < d->props.size(); ++i)
            map.insert(d->props.at(i).key, d->props.at(i).value);
    }
    return map;
}

/*!
    \since 4.3
    Returns the number of properties stored in the format.
*/
int BOBUIextFormat::propertyCount() const
{
    return d ? d->props.size() : 0;
}

/*!
    \fn bool BOBUIextFormat::operator!=(const BOBUIextFormat &other) const

    Returns \c true if this text format is different from the \a other text
    format.
*/


/*!
    \fn bool BOBUIextFormat::operator==(const BOBUIextFormat &other) const

    Returns \c true if this text format is the same as the \a other text
    format.
*/
bool BOBUIextFormat::operator==(const BOBUIextFormat &rhs) const
{
    if (format_type != rhs.format_type)
        return false;

    if (d == rhs.d)
        return true;

    if (d && d->props.isEmpty() && !rhs.d)
        return true;

    if (!d && rhs.d && rhs.d->props.isEmpty())
        return true;

    if (!d || !rhs.d)
        return false;

    return *d == *rhs.d;
}

/*!
    \class BOBUIextCharFormat
    \reentrant

    \brief The BOBUIextCharFormat class provides formatting information for
    characters in a BOBUIextDocument.
    \inmodule BobUIGui

    \ingroup richtext-processing
    \ingroup shared

    The character format of text in a document specifies the visual properties
    of the text, as well as information about its role in a hypertext document.

    The font used can be set by supplying a font to the setFont() function, and
    each aspect of its appearance can be adjusted to give the desired effect.
    setFontFamilies() and setFontPointSize() define the font's family (e.g. Times)
    and printed size; setFontWeight() and setFontItalic() provide control over
    the style of the font. setFontUnderline(), setFontOverline(),
    setFontStrikeOut(), and setFontFixedPitch() provide additional effects for
    text.

    The color is set with setForeground(). If the text is intended to be used
    as an anchor (for hyperlinks), this can be enabled with setAnchor(). The
    setAnchorHref() and setAnchorNames() functions are used to specify the
    information about the hyperlink's destination and the anchor's name.

    \sa BOBUIextFormat, BOBUIextBlockFormat, BOBUIextTableFormat, BOBUIextListFormat
*/

/*!
    \enum BOBUIextCharFormat::VerticalAlignment

    This enum describes the ways that adjacent characters can be vertically
    aligned.

    \value AlignNormal  Adjacent characters are positioned in the standard
                        way for text in the writing system in use.
    \value AlignSuperScript Characters are placed above the base line for
                            normal text.
    \value AlignSubScript   Characters are placed below the base line for
                            normal text.
    \value AlignMiddle The center of the object is vertically aligned with the
                       base line. Currently, this is only implemented for
                       inline objects.
    \value AlignBottom The bottom edge of the object is vertically aligned with
                       the base line.
    \value AlignTop    The top edge of the object is vertically aligned with
                       the base line.
    \value AlignBaseline The base lines of the characters are aligned.
*/

/*!
    \enum BOBUIextCharFormat::UnderlineStyle

    This enum describes the different ways drawing underlined text.

    \value NoUnderline          Text is draw without any underlining decoration.
    \value SingleUnderline      A line is drawn using BobUI::SolidLine.
    \value DashUnderline        Dashes are drawn using BobUI::DashLine.
    \value DotLine              Dots are drawn using BobUI::DotLine;
    \value DashDotLine          Dashes and dots are drawn using BobUI::DashDotLine.
    \value DashDotDotLine       Underlines draw drawn using BobUI::DashDotDotLine.
    \value WaveUnderline        The text is underlined using a wave shaped line.
    \value SpellCheckUnderline  The underline is drawn depending on the SpellCheckUnderlineStyle
                                theme hint of QPlatformTheme. By default this is mapped to
                                WaveUnderline, on \macos it is mapped to DotLine.

    \sa BobUI::PenStyle
*/

/*!
    \fn BOBUIextCharFormat::BOBUIextCharFormat()

    Constructs a new character format object.
*/
BOBUIextCharFormat::BOBUIextCharFormat() : BOBUIextFormat(CharFormat) {}

/*!
    \internal
    \fn BOBUIextCharFormat::BOBUIextCharFormat(const BOBUIextFormat &other)

    Creates a new character format with the same attributes as the \a given
    text format.
*/
BOBUIextCharFormat::BOBUIextCharFormat(const BOBUIextFormat &fmt)
 : BOBUIextFormat(fmt)
{
}

/*!
    \fn bool BOBUIextCharFormat::isValid() const

    Returns \c true if this character format is valid; otherwise returns
    false.
*/


/*!
    \fn void BOBUIextCharFormat::setFontFamily(const QString &family)
    \deprecated [6.1] Use setFontFamilies() instead.

    Sets the text format's font \a family.

    \sa setFont()
*/


/*!
    \fn QString BOBUIextCharFormat::fontFamily() const
    \deprecated [6.1] Use fontFamilies() instead.

    Returns the text format's font family.

    \sa font()
*/

/*!
    \fn void BOBUIextCharFormat::setFontFamilies(const QStringList &families)
    \since 5.13

    Sets the text format's font \a families.

    \sa setFont()
*/

#if BOBUI_VERSION < BOBUI_VERSION_CHECK(7, 0, 0)
/*!
    \fn QVariant BOBUIextCharFormat::fontFamilies() const
    \since 5.13

    Returns the text format's font families.

    \note This function returns a QVariant for historical reasons. It will be
    corrected to return QStringList in BobUI 7. The variant contains a QStringList
    object, which can be extracted by calling \c{toStringList()} on it.

    \sa font()
*/
#else
/* // BobUI 7 documents this function
    \fn QStringList BOBUIextCharFormat::fontFamilies() const
    \since 5.13

    Returns the text format's font families.

    \sa font()
*/
#endif

/*!
    \fn void BOBUIextCharFormat::setFontStyleName(const QString &styleName)
    \since 5.13

    Sets the text format's font \a styleName.

    \sa setFont(), QFont::setStyleName()
*/

#if BOBUI_VERSION < BOBUI_VERSION_CHECK(7, 0, 0)
/*!
    \fn QVariant BOBUIextCharFormat::fontStyleName() const
    \since 5.13

    Returns the text format's font style name.

    \note This function returns a QVariant for historical reasons. It will be
    corrected to return QStringList in BobUI 7. The variant contains a QStringList
    object, which can be extracted by calling \c{toStringList()} on it.

    \sa font(), QFont::styleName()
*/
#else
/* // BobUI 7 documents this function
    \fn QStringList BOBUIextCharFormat::fontStyleName() const
    \since 5.13

    Returns the text format's font style name.

    \sa font(), QFont::styleName()
*/
#endif

/*!
    \fn void BOBUIextCharFormat::setFontPointSize(qreal size)

    Sets the text format's font \a size.

    \sa setFont()
*/


/*!
    \fn qreal BOBUIextCharFormat::fontPointSize() const

    Returns the font size used to display text in this format.

    \sa font()
*/


/*!
    \fn void BOBUIextCharFormat::setFontWeight(int weight)

    Sets the text format's font weight to \a weight.

    \sa setFont(), QFont::Weight
*/


/*!
    \fn int BOBUIextCharFormat::fontWeight() const

    Returns the text format's font weight.

    \sa font(), QFont::Weight
*/


/*!
    \fn void BOBUIextCharFormat::setFontItalic(bool italic)

    If \a italic is true, sets the text format's font to be italic; otherwise
    the font will be non-italic.

    \sa setFont()
*/


/*!
    \fn bool BOBUIextCharFormat::fontItalic() const

    Returns \c true if the text format's font is italic; otherwise
    returns \c false.

    \sa font()
*/


/*!
    \fn void BOBUIextCharFormat::setFontUnderline(bool underline)

    If \a underline is true, sets the text format's font to be underlined;
    otherwise it is displayed non-underlined.

    \sa setFont()
*/


/*!
    \fn bool BOBUIextCharFormat::fontUnderline() const

    Returns \c true if the text format's font is underlined; otherwise
    returns \c false.

    \sa font()
*/
bool BOBUIextCharFormat::fontUnderline() const
{
    if (hasProperty(TextUnderlineStyle))
        return underlineStyle() == SingleUnderline;
    return boolProperty(FontUnderline);
}

/*!
    \fn UnderlineStyle BOBUIextCharFormat::underlineStyle() const
    \since 4.2

    Returns the style of underlining the text.
*/

/*!
    \fn void BOBUIextCharFormat::setUnderlineStyle(UnderlineStyle style)
    \since 4.2

    Sets the style of underlining the text to \a style.
*/
void BOBUIextCharFormat::setUnderlineStyle(UnderlineStyle style)
{
    setProperty(TextUnderlineStyle, style);
    // for compatibility
    setProperty(FontUnderline, style == SingleUnderline);
}

/*!
    \fn void BOBUIextCharFormat::setFontOverline(bool overline)

    If \a overline is true, sets the text format's font to be overlined;
    otherwise the font is displayed non-overlined.

    \sa setFont()
*/


/*!
    \fn bool BOBUIextCharFormat::fontOverline() const

    Returns \c true if the text format's font is overlined; otherwise
    returns \c false.

    \sa font()
*/


/*!
    \fn void BOBUIextCharFormat::setFontStrikeOut(bool strikeOut)

    If \a strikeOut is true, sets the text format's font with strike-out
    enabled (with a horizontal line through it); otherwise it is displayed
    without strikeout.

    \sa setFont()
*/


/*!
    \fn bool BOBUIextCharFormat::fontStrikeOut() const

    Returns \c true if the text format's font is struck out (has a horizontal line
    drawn through it); otherwise returns \c false.

    \sa font()
*/


/*!
    \since 4.5
    \fn void BOBUIextCharFormat::setFontStyleHint(QFont::StyleHint hint, QFont::StyleStrategy strategy)

    Sets the font style \a hint and \a strategy.

    BobUI does not support style hints on X11 since this information is not provided by the window system.

    \sa setFont()
    \sa QFont::setStyleHint()
*/


/*!
    \since 4.5
    \fn void BOBUIextCharFormat::setFontStyleStrategy(QFont::StyleStrategy strategy)

    Sets the font style \a strategy.

    \sa setFont()
    \sa QFont::setStyleStrategy()
*/


/*!
    \since 4.5
    \fn void BOBUIextCharFormat::setFontKerning(bool enable)
    Enables kerning for this font if \a enable is true; otherwise disables it.

    When kerning is enabled, glyph metrics do not add up anymore, even for
    Latin text. In other words, the assumption that width('a') + width('b')
    is equal to width("ab") is not neccesairly true.

    \sa setFont()
*/


/*!
    \fn BOBUIextCharFormat::StyleHint BOBUIextCharFormat::fontStyleHint() const
    \since 4.5

    Returns the font style hint.

    \sa setFontStyleHint(), font()
*/


/*!
    \since 4.5
    \fn BOBUIextCharFormat::StyleStrategy BOBUIextCharFormat::fontStyleStrategy() const

    Returns the current font style strategy.

    \sa setFontStyleStrategy()
    \sa font()
*/


/*!
    \since 4.5
    \fn  bool BOBUIextCharFormat::fontKerning() const
    Returns \c true if the font kerning is enabled.

    \sa setFontKerning()
    \sa font()
*/


/*!
    \fn void BOBUIextCharFormat::setFontFixedPitch(bool fixedPitch)

    If \a fixedPitch is true, sets the text format's font to be fixed pitch;
    otherwise a non-fixed pitch font is used.

    \sa setFont()
*/


/*!
    \fn bool BOBUIextCharFormat::fontFixedPitch() const

    Returns \c true if the text format's font is fixed pitch; otherwise
    returns \c false.

    \sa font()
*/

/*!
    \since 4.8

    \fn void BOBUIextCharFormat::setFontHintingPreference(QFont::HintingPreference hintingPreference)

    Sets the hinting preference of the text format's font to be \a hintingPreference.

    \sa setFont(), QFont::setHintingPreference()
*/

/*!
    \since 4.8

    \fn QFont::HintingPreference BOBUIextCharFormat::fontHintingPreference() const

    Returns the hinting preference set for this text format.

    \sa font(), QFont::hintingPreference()
*/

/*!
    \since 6.11

    Sets the typographical features of the text format's font to be \a fontFeatures.

    \sa QFont::setFeature()
*/
void BOBUIextCharFormat::setFontFeatures(const QHash<QFont::Tag, quint32> &fontFeatures)
{
    setProperty(FontFeatures, QVariant::fromValue(fontFeatures));
}

/*!
    \since 6.11

    Gets the typographical features of the text format's font.

    \sa setFontFeatures()
*/
QHash<QFont::Tag, quint32> BOBUIextCharFormat::fontFeatures() const
{
    return property(FontFeatures).value<QHash<QFont::Tag, quint32>>();
}

/*!
    \since 6.11

    Sets the variable axes of the text format's font to be \a fontVariableAxes.

    \sa QFont::setVariableAxis()
*/
void BOBUIextCharFormat::setFontVariableAxes(const QHash<QFont::Tag, float> &fontVariableAxes)
{
    setProperty(FontVariableAxes, QVariant::fromValue(fontVariableAxes));
}

/*!
    \since 6.11

    Gets the variable axes of the text format's font.

    \sa setFontVariableAxes()
*/
QHash<QFont::Tag, float> BOBUIextCharFormat::fontVariableAxes() const
{
    return property(FontVariableAxes).value<QHash<QFont::Tag, float>>();
}


/*!
    \fn QPen BOBUIextCharFormat::textOutline() const

    Returns the pen used to draw the outlines of characters in this format.
*/


/*!
    \fn void BOBUIextCharFormat::setTextOutline(const QPen &pen)

    Sets the pen used to draw the outlines of characters to the given \a pen.
*/

/*!
    \fn void BOBUIextCharFormat::setToolTip(const QString &text)
    \since 4.3

    Sets the tool tip for a fragment of text to the given \a text.
*/

/*!
    \fn QString BOBUIextCharFormat::toolTip() const
    \since 4.3

    Returns the tool tip that is displayed for a fragment of text.
*/

/*!
    \fn void BOBUIextFormat::setForeground(const QBrush &brush)

    Sets the foreground brush to the specified \a brush. The foreground
    brush is mostly used to render text.

    \sa foreground(), clearForeground(), setBackground()
*/


/*!
    \fn QBrush BOBUIextFormat::foreground() const

    Returns the brush used to render foreground details, such as text,
    frame outlines, and table borders.

    \sa setForeground(), clearForeground(), background()
*/

/*!
    \fn void BOBUIextFormat::clearForeground()

    Clears the brush used to paint the document's foreground. The default
    brush will be used.

    \sa foreground(), setForeground(), clearBackground()
*/

/*!
    \fn void BOBUIextCharFormat::setSuperScriptBaseline(qreal baseline)
    \since 6.0

    Sets the superscript's base line as a % of font height to \a baseline.
    The default value is 50% (1/2 of height).

    \sa superScriptBaseline(), setSubScriptBaseline(), subScriptBaseline(), setBaselineOffset(), baselineOffset()
*/

/*!
    \fn qreal BOBUIextCharFormat::superScriptBaseline() const
    \since 6.0

    Returns the superscript's base line as a % of font height.

    \sa setSuperScriptBaseline(), setSubScriptBaseline(), subScriptBaseline(), setBaselineOffset(), baselineOffset()
*/

/*!
    \fn void BOBUIextCharFormat::setSubScriptBaseline(qreal baseline)
    \since 6.0

    Sets the subscript's base line as a % of font height to \a baseline.
    The default value is 16.67% (1/6 of height)

    \sa subScriptBaseline(), setSuperScriptBaseline(), superScriptBaseline(), setBaselineOffset(), baselineOffset()
*/

/*!
    \fn qreal BOBUIextCharFormat::subScriptBaseline() const
    \since 6.0

    Returns the subscript's base line as a % of font height.

    \sa setSubScriptBaseline(), setSuperScriptBaseline(), superScriptBaseline(), setBaselineOffset(), baselineOffset()
*/

/*!
    \fn void BOBUIextCharFormat::setBaselineOffset(qreal baseline)
    \since 6.0

    Sets the base line (in % of height) of text to \a baseline. A positive value moves the text
    up, by the corresponding %; a negative value moves it down. The default value is 0.

    \sa baselineOffset(), setSubScriptBaseline(), subScriptBaseline(), setSuperScriptBaseline(), superScriptBaseline()
*/

/*!
    \fn qreal BOBUIextCharFormat::baselineOffset() const
    \since 6.0

    Returns the the baseline offset in %.

    \sa setBaselineOffset(), setSubScriptBaseline(), subScriptBaseline(), setSuperScriptBaseline(), superScriptBaseline()
*/

/*!
    \fn void BOBUIextCharFormat::setAnchor(bool anchor)

    If \a anchor is true, text with this format represents an anchor, and is
    formatted in the appropriate way; otherwise the text is formatted normally.
    (Anchors are hyperlinks which are often shown underlined and in a different
    color from plain text.)

    The way the text is rendered is independent of whether or not the format
    has a valid anchor defined. Use setAnchorHref(), and optionally
    setAnchorNames() to create a hypertext link.

    \sa isAnchor()
*/


/*!
    \fn bool BOBUIextCharFormat::isAnchor() const

    Returns \c true if the text is formatted as an anchor; otherwise
    returns \c false.

    \sa setAnchor(), setAnchorHref(), setAnchorNames()
*/


/*!
    \fn void BOBUIextCharFormat::setAnchorHref(const QString &value)

    Sets the hypertext link for the text format to the given \a value.
    This is typically a URL like "http://example.com/index.html".

    The anchor will be displayed with the \a value as its display text;
    if you want to display different text call setAnchorNames().

    To format the text as a hypertext link use setAnchor().
*/


/*!
    \fn QString BOBUIextCharFormat::anchorHref() const

    Returns the text format's hypertext link, or an empty string if
    none has been set.
*/

/*!
    \fn void BOBUIextCharFormat::setAnchorNames(const QStringList &names)
    \since 4.3

    Sets the text format's anchor \a names. For the anchor to work as a
    hyperlink, the destination must be set with setAnchorHref() and
    the anchor must be enabled with setAnchor().
*/

/*!
    \fn QStringList BOBUIextCharFormat::anchorNames() const
    \since 4.3

    Returns the anchor names associated with this text format, or an empty
    string list if none has been set. If the anchor names are set, text with this
    format can be the destination of a hypertext link.
*/
QStringList BOBUIextCharFormat::anchorNames() const
{
    QVariant prop = property(AnchorName);
    if (prop.userType() == QMetaType::QStringList)
        return prop.toStringList();
    else if (prop.userType() != QMetaType::QString)
        return QStringList();
    return QStringList(prop.toString());
}


/*!
    \fn void BOBUIextCharFormat::setTableCellRowSpan(int tableCellRowSpan)
    \internal

    If this character format is applied to characters in a table cell,
    the cell will span \a tableCellRowSpan rows.
*/


/*!
    \fn int BOBUIextCharFormat::tableCellRowSpan() const
    \internal

    If this character format is applied to characters in a table cell,
    this function returns the number of rows spanned by the text (this may
    be 1); otherwise it returns 1.
*/

/*!
    \fn void BOBUIextCharFormat::setTableCellColumnSpan(int tableCellColumnSpan)
    \internal

    If this character format is applied to characters in a table cell,
    the cell will span \a tableCellColumnSpan columns.
*/


/*!
    \fn int BOBUIextCharFormat::tableCellColumnSpan() const
    \internal

    If this character format is applied to characters in a table cell,
    this function returns the number of columns spanned by the text (this
    may be 1); otherwise it returns 1.
*/

/*!
    \fn void BOBUIextCharFormat::setUnderlineColor(const QColor &color)

    Sets the color used to draw underlines, overlines and strikeouts on the
    characters with this format to the \a color specified.

    \sa underlineColor()
*/

/*!
    \fn QColor BOBUIextCharFormat::underlineColor() const

    Returns the color used to draw underlines, overlines and strikeouts
    on the characters with this format.

    \sa setUnderlineColor()
*/

/*!
    \fn void BOBUIextCharFormat::setVerticalAlignment(VerticalAlignment alignment)

    Sets the vertical alignment used for the characters with this format to
    the \a alignment specified.

    \sa verticalAlignment()
*/

/*!
    \fn VerticalAlignment BOBUIextCharFormat::verticalAlignment() const

    Returns the vertical alignment used for characters with this format.

    \sa setVerticalAlignment()
*/

/*!
    \enum BOBUIextCharFormat::FontPropertiesInheritanceBehavior
    \since 5.3

    This enum specifies how the setFont() function should behave with
    respect to unset font properties.

    \value FontPropertiesSpecifiedOnly  If a property is not explicitly set, do not
                                        change the text format's property value.
    \value FontPropertiesAll  If a property is not explicitly set, override the
                              text format's property with a default value.

    \sa setFont()
*/

/*!
    \since 5.3

    Sets the text format's \a font.

    If \a behavior is BOBUIextCharFormat::FontPropertiesAll, the font property that
    has not been explicitly set is treated like as it were set with default value;
    If \a behavior is BOBUIextCharFormat::FontPropertiesSpecifiedOnly, the font property that
    has not been explicitly set is ignored and the respective property value
    remains unchanged.

    \sa font()
*/
void BOBUIextCharFormat::setFont(const QFont &font, FontPropertiesInheritanceBehavior behavior)
{
    const uint mask = behavior == FontPropertiesAll ? uint(QFont::AllPropertiesResolved)
                                                    : font.resolveMask();

    if (mask & QFont::FamiliesResolved)
        setFontFamilies(font.families());
    if (mask & QFont::StyleNameResolved)
        setFontStyleName(font.styleName());

    if (mask & QFont::SizeResolved) {
        const qreal pointSize = font.pointSizeF();
        if (pointSize > 0) {
            setFontPointSize(pointSize);
        } else {
            const int pixelSize = font.pixelSize();
            if (pixelSize > 0)
                setProperty(BOBUIextFormat::FontPixelSize, pixelSize);
        }
    }

    if (mask & QFont::WeightResolved)
        setFontWeight(font.weight());
    if (mask & QFont::StyleResolved)
        setFontItalic(font.style() != QFont::StyleNormal);
    if (mask & QFont::UnderlineResolved)
        setUnderlineStyle(font.underline() ? SingleUnderline : NoUnderline);
    if (mask & QFont::OverlineResolved)
        setFontOverline(font.overline());
    if (mask & QFont::StrikeOutResolved)
        setFontStrikeOut(font.strikeOut());
    if (mask & QFont::FixedPitchResolved)
        setFontFixedPitch(font.fixedPitch());
    if (mask & QFont::CapitalizationResolved)
        setFontCapitalization(font.capitalization());
    if (mask & QFont::WordSpacingResolved)
        setFontWordSpacing(font.wordSpacing());
    if (mask & QFont::LetterSpacingResolved) {
        setFontLetterSpacingType(font.letterSpacingType());
        setFontLetterSpacing(font.letterSpacing());
    }
    if (mask & QFont::StretchResolved)
        setFontStretch(font.stretch());
    if (mask & QFont::StyleHintResolved)
        setFontStyleHint(font.styleHint());
    if (mask & QFont::StyleStrategyResolved)
        setFontStyleStrategy(font.styleStrategy());
    if (mask & QFont::HintingPreferenceResolved)
        setFontHintingPreference(font.hintingPreference());
    if (mask & QFont::KerningResolved)
        setFontKerning(font.kerning());
    if (mask & QFont::FeaturesResolved) {
        const auto tags = font.featureTags();

        QHash<QFont::Tag, quint32> fontFeatures;
        for (QFont::Tag tag : tags)
            fontFeatures.insert(tag, font.featureValue(tag));
        setFontFeatures(fontFeatures);
    }
    if (mask & QFont::VariableAxesResolved) {
        const auto tags = font.variableAxisTags();

        QHash<QFont::Tag, float> fontVariableAxes;
        for (QFont::Tag tag : tags)
            fontVariableAxes.insert(tag, font.variableAxisValue(tag));
        setFontVariableAxes(fontVariableAxes);
    }
}

/*!
    Returns the font for this character format.

    This function takes into account the format's font attributes (such as fontWeight()
    and fontPointSize()) and resolves them on top of the default font, defined as follows.
    If the format is part of a document, that is the document's default font.
    Otherwise the properties are resolved on top of a default constructed QFont.

    For example, if this format's font size hasn't been changed from the default font,
    fontPointSize() returns 0, while \c {font().pointSize()} returns the actual
    size used for drawing.

    \sa BOBUIextDocument::defaultFont()
*/
QFont BOBUIextCharFormat::font() const
{
    return d ? d->font() : QFont();
}

/*!
    \class BOBUIextBlockFormat
    \reentrant

    \brief The BOBUIextBlockFormat class provides formatting information for
    blocks of text in a BOBUIextDocument.
    \inmodule BobUIGui

    \ingroup richtext-processing
    \ingroup shared

    A document is composed of a list of blocks, represented by BOBUIextBlock
    objects. Each block can contain an item of some kind, such as a
    paragraph of text, a table, a list, or an image. Every block has an
    associated BOBUIextBlockFormat that specifies its characteristics.

    To cater for left-to-right and right-to-left languages you can set
    a block's direction with setLayoutDirection(). Paragraph alignment is
    set with setAlignment(). Margins are controlled by setTopMargin(),
    setBottomMargin(), setLeftMargin(), setRightMargin(). Overall
    indentation is set with setIndent(), the indentation of the first
    line with setTextIndent().

    Line spacing is set with setLineHeight() and retrieved via lineHeight()
    and lineHeightType(). The types of line spacing available are in the
    LineHeightTypes enum.

    Line breaking can be enabled and disabled with setNonBreakableLines().

    The brush used to paint the paragraph's background
    is set with \l{BOBUIextFormat::setBackground()}{setBackground()}, and other
    aspects of the text's appearance can be customized by using the
    \l{BOBUIextFormat::setProperty()}{setProperty()} function with the
    \c OutlinePen, \c ForegroundBrush, and \c BackgroundBrush
    \l{BOBUIextFormat::Property} values.

    If a text block is part of a list, it can also have a list format that
    is accessible with the listFormat() function.

    \sa BOBUIextBlock, BOBUIextCharFormat
*/

/*!
    \since 4.8
    \enum BOBUIextBlockFormat::LineHeightTypes

    This enum describes the various types of line spacing support paragraphs can have.

    \value SingleHeight This is the default line height: single spacing.
    \value ProportionalHeight This sets the spacing proportional to the line (in percentage).
                              For example, set to 200 for double spacing.
    \value FixedHeight This sets the line height to a fixed line height (in pixels).
    \value MinimumHeight This sets the minimum line height (in pixels).
    \value LineDistanceHeight This adds the specified height between lines (in pixels).

    \sa lineHeight(), lineHeightType(), setLineHeight()
*/

/*!
    \fn BOBUIextBlockFormat::BOBUIextBlockFormat()

    Constructs a new BOBUIextBlockFormat.
*/
BOBUIextBlockFormat::BOBUIextBlockFormat() : BOBUIextFormat(BlockFormat) {}

/*!
    \internal
    \fn BOBUIextBlockFormat::BOBUIextBlockFormat(const BOBUIextFormat &other)

    Creates a new block format with the same attributes as the \a given
    text format.
*/
BOBUIextBlockFormat::BOBUIextBlockFormat(const BOBUIextFormat &fmt)
 : BOBUIextFormat(fmt)
{
}

/*!
    \since 4.4
    Sets the tab positions for the text block to those specified by
    \a tabs.

    \sa tabPositions()
*/
void BOBUIextBlockFormat::setTabPositions(const QList<BOBUIextOption::Tab> &tabs)
{
    QList<QVariant> list;
    list.reserve(tabs.size());
    for (const auto &e : tabs)
        list.append(QVariant::fromValue(e));
    setProperty(TabPositions, list);
}

/*!
    \since 4.4
    Returns a list of tab positions defined for the text block.

    \sa setTabPositions()
*/
QList<BOBUIextOption::Tab> BOBUIextBlockFormat::tabPositions() const
{
    QVariant variant = property(TabPositions);
    if (variant.isNull())
        return QList<BOBUIextOption::Tab>();
    QList<BOBUIextOption::Tab> answer;
    const QList<QVariant> variantsList = qvariant_cast<QList<QVariant> >(variant);
    answer.reserve(variantsList.size());
    for (const auto &e: variantsList)
        answer.append(qvariant_cast<BOBUIextOption::Tab>(e));
    return answer;
}

/*!
    \fn BOBUIextBlockFormat::isValid() const

    Returns \c true if this block format is valid; otherwise returns
    false.
*/

/*!
    \fn void BOBUIextFormat::setLayoutDirection(BobUI::LayoutDirection direction)

    Sets the document's layout direction to the specified \a direction.

    \sa layoutDirection()
*/


/*!
    \fn BobUI::LayoutDirection BOBUIextFormat::layoutDirection() const

    Returns the document's layout direction.

    \sa setLayoutDirection()
*/


/*!
    \fn void BOBUIextBlockFormat::setAlignment(BobUI::Alignment alignment)

    Sets the paragraph's \a alignment.

    \sa alignment()
*/


/*!
    \fn BobUI::Alignment BOBUIextBlockFormat::alignment() const

    Returns the paragraph's alignment.

    \sa setAlignment()
*/


/*!
    \fn void BOBUIextBlockFormat::setTopMargin(qreal margin)

    Sets the paragraph's top \a margin.

    \sa topMargin(), setBottomMargin(), setLeftMargin(), setRightMargin()
*/


/*!
    \fn qreal BOBUIextBlockFormat::topMargin() const

    Returns the paragraph's top margin.

    \sa setTopMargin(), bottomMargin()
*/


/*!
    \fn void BOBUIextBlockFormat::setBottomMargin(qreal margin)

    Sets the paragraph's bottom \a margin.

    \sa bottomMargin(), setTopMargin(), setLeftMargin(), setRightMargin()
*/


/*!
    \fn qreal BOBUIextBlockFormat::bottomMargin() const

    Returns the paragraph's bottom margin.

    \sa setBottomMargin(), topMargin()
*/


/*!
    \fn void BOBUIextBlockFormat::setLeftMargin(qreal margin)

    Sets the paragraph's left \a margin. Indentation can be applied separately
    with setIndent().

    \sa leftMargin(), setRightMargin(), setTopMargin(), setBottomMargin()
*/


/*!
    \fn qreal BOBUIextBlockFormat::leftMargin() const

    Returns the paragraph's left margin.

    \sa setLeftMargin(), rightMargin(), indent()
*/


/*!
    \fn void BOBUIextBlockFormat::setRightMargin(qreal margin)

    Sets the paragraph's right \a margin.

    \sa rightMargin(), setLeftMargin(), setTopMargin(), setBottomMargin()
*/


/*!
    \fn qreal BOBUIextBlockFormat::rightMargin() const

    Returns the paragraph's right margin.

    \sa setRightMargin(), leftMargin()
*/


/*!
    \fn void BOBUIextBlockFormat::setTextIndent(qreal indent)

    Sets the \a indent for the first line in the block. This allows the first
    line of a paragraph to be indented differently to the other lines,
    enhancing the readability of the text.

    \sa textIndent(), setLeftMargin(), setRightMargin(), setTopMargin(), setBottomMargin()
*/


/*!
    \fn qreal BOBUIextBlockFormat::textIndent() const

    Returns the paragraph's text indent.

    \sa setTextIndent()
*/


/*!
    \fn void BOBUIextBlockFormat::setIndent(int indentation)

    Sets the paragraph's \a indentation. Margins are set independently of
    indentation with setLeftMargin() and setTextIndent().
    The \a indentation is an integer that is multiplied with the document-wide
    standard indent, resulting in the actual indent of the paragraph.

    \sa indent(), BOBUIextDocument::indentWidth()
*/


/*!
    \fn int BOBUIextBlockFormat::indent() const

    Returns the paragraph's indent.

    \sa setIndent()
*/


/*!
    \fn void BOBUIextBlockFormat::setHeadingLevel(int level)
    \since 5.12

    Sets the paragraph's heading \a level, where 1 is the highest-level heading
    type (usually with the largest possible heading font size), and increasing
    values are progressively deeper into the document (and usually with smaller
    font sizes). For example when reading an HTML H1 tag, the heading level is
    set to 1. Setting the heading level does not automatically change the font
    size; however BOBUIextDocumentFragment::fromHtml() sets both the heading level
    and the font size simultaneously.

    If the paragraph is not a heading, the level should be set to 0 (the default).

    \sa headingLevel()
*/


/*!
    \fn int BOBUIextBlockFormat::headingLevel() const
    \since 5.12

    Returns the paragraph's heading level if it is a heading, or 0 if not.

    \sa setHeadingLevel()
*/


/*!
    \fn void BOBUIextBlockFormat::setMarker(MarkerType marker)
    \since 5.14

    Sets the type of adornment that should be rendered alongside the paragraph to \a marker.
    For example, a list item can be adorned with a checkbox, either checked
    or unchecked, as a replacement for its bullet. The default is \c NoMarker.

    \sa marker()
*/


/*!
    \fn MarkerType BOBUIextBlockFormat::marker() const
    \since 5.14

    Returns the paragraph's marker if one has been set, or \c NoMarker if not.

    \sa setMarker()
*/


/*!
    \since 5.14
    \enum BOBUIextBlockFormat::MarkerType

    This enum describes the types of markers a list item can have.
    If a list item (a paragraph for which \l BOBUIextBlock::textList() returns the list)
    has a marker, it is rendered instead of the normal bullet.
    In this way, checkable list items can be mixed with plain list items in the
    same list, overriding the type of bullet specified by the
    \l BOBUIextListFormat::style() for the entire list.

    \value NoMarker This is the default: the list item's bullet will be shown.
    \value Unchecked Instead of the list item's bullet, an unchecked checkbox will be shown.
    \value Checked Instead of the list item's bullet, a checked checkbox will be shown.

    In the future, this may be extended to specify other types of paragraph
    decorations.

    \sa BOBUIextListFormat::style()
*/


/*!
    \fn void BOBUIextBlockFormat::setLineHeight(qreal height, int heightType)
    \since 4.8

    Sets the line height for the paragraph to the value given by \a height
    which is dependent on \a heightType in the way described by the
    LineHeightTypes enum.

    \sa LineHeightTypes, lineHeight(), lineHeightType()
*/


/*!
    \fn qreal BOBUIextBlockFormat::lineHeight(qreal scriptLineHeight, qreal scaling) const
    \since 4.8

    Returns the height of the lines in the paragraph based on the height of the
    script line given by \a scriptLineHeight and the specified \a scaling
    factor.

    The value that is returned is also dependent on the given LineHeightType of
    the paragraph as well as the LineHeight setting that has been set for the
    paragraph.

    The scaling is needed for heights that include a fixed number of pixels, to
    scale them appropriately for printing.

    \sa LineHeightTypes, setLineHeight(), lineHeightType()
*/


/*!
    \fn qreal BOBUIextBlockFormat::lineHeight() const
    \since 4.8

    This returns the LineHeight property for the paragraph.

    \sa LineHeightTypes, setLineHeight(), lineHeightType()
*/


/*!
    \fn qreal BOBUIextBlockFormat::lineHeightType() const
    \since 4.8

    This returns the LineHeightType property of the paragraph.

    \sa LineHeightTypes, setLineHeight(), lineHeight()
*/


/*!
    \fn void BOBUIextBlockFormat::setNonBreakableLines(bool b)

    If \a b is true, the lines in the paragraph are treated as
    non-breakable; otherwise they are breakable.

    \sa nonBreakableLines()
*/


/*!
    \fn bool BOBUIextBlockFormat::nonBreakableLines() const

    Returns \c true if the lines in the paragraph are non-breakable;
    otherwise returns \c false.

    \sa setNonBreakableLines()
*/

/*!
    \fn BOBUIextFormat::PageBreakFlags BOBUIextBlockFormat::pageBreakPolicy() const
    \since 4.2

    Returns the currently set page break policy for the paragraph. The default is
    BOBUIextFormat::PageBreak_Auto.

    \sa setPageBreakPolicy()
*/

/*!
    \fn void BOBUIextBlockFormat::setPageBreakPolicy(PageBreakFlags policy)
    \since 4.2

    Sets the page break policy for the paragraph to \a policy.

    \sa pageBreakPolicy()
*/

/*!
    \class BOBUIextListFormat
    \reentrant

    \brief The BOBUIextListFormat class provides formatting information for
    lists in a BOBUIextDocument.
    \inmodule BobUIGui

    \ingroup richtext-processing
    \ingroup shared

    A list is composed of one or more items, represented as text blocks.
    The list's format specifies the appearance of items in the list.
    In particular, it determines the indentation and the style of each item.

    The indentation of the items is an integer value that causes each item to
    be offset from the left margin by a certain amount. This value is read with
    indent() and set with setIndent().

    The style used to decorate each item is set with setStyle() and can be read
    with the style() function. The style controls the type of bullet points and
    numbering scheme used for items in the list. Note that lists that use the
    decimal numbering scheme begin counting at 1 rather than 0, unless it has
    been overridden via setStart().

    Style properties can be set to further configure the appearance of list
    items; for example, the ListNumberPrefix and ListNumberSuffix properties
    can be used to customize the numbers used in an ordered list so that they
    appear as (1), (2), (3), etc.:

    \snippet textdocument-listitemstyles/mainwindow.cpp add a styled, ordered list

    \sa BOBUIextList
*/

/*!
    \enum BOBUIextListFormat::Style

    This enum describes the symbols used to decorate list items:

    \value ListDisc        a filled circle
    \value ListCircle      an empty circle
    \value ListSquare      a filled square
    \value ListDecimal     decimal values in ascending order
    \value ListLowerAlpha  lower case Latin characters in alphabetical order
    \value ListUpperAlpha  upper case Latin characters in alphabetical order
    \value ListLowerRoman  lower case roman numerals (supports up to 4999 items only)
    \value ListUpperRoman  upper case roman numerals (supports up to 4999 items only)
    \omitvalue ListStyleUndefined
*/

/*!
    \fn BOBUIextListFormat::BOBUIextListFormat()

    Constructs a new list format object.
*/
BOBUIextListFormat::BOBUIextListFormat()
    : BOBUIextFormat(ListFormat)
{
    setIndent(1);
    setStart(1);
}

/*!
    \internal
    \fn BOBUIextListFormat::BOBUIextListFormat(const BOBUIextFormat &other)

    Creates a new list format with the same attributes as the \a given
    text format.
*/
BOBUIextListFormat::BOBUIextListFormat(const BOBUIextFormat &fmt)
 : BOBUIextFormat(fmt)
{
}

/*!
    \fn bool BOBUIextListFormat::isValid() const

    Returns \c true if this list format is valid; otherwise
    returns \c false.
*/

/*!
    \fn void BOBUIextListFormat::setStyle(Style style)

    Sets the list format's \a style.

    \sa style(), Style
*/

/*!
    \fn Style BOBUIextListFormat::style() const

    Returns the list format's style.

    \sa setStyle(), Style
*/


/*!
    \fn void BOBUIextListFormat::setIndent(int indentation)

    Sets the list format's \a indentation.
    The indentation is multiplied by the BOBUIextDocument::indentWidth
    property to get the effective indent in pixels.

    \sa indent()
*/


/*!
    \fn int BOBUIextListFormat::indent() const

    Returns the list format's indentation.
    The indentation is multiplied by the BOBUIextDocument::indentWidth
    property to get the effective indent in pixels.

    \sa setIndent()
*/

/*!
    \fn void BOBUIextListFormat::setNumberPrefix(const QString &numberPrefix)
    \since 4.8

    Sets the list format's number prefix to the string specified by
    \a numberPrefix. This can be used with all sorted list types. It does not
    have any effect on unsorted list types.

    The default prefix is an empty string.

    \sa numberPrefix()
*/

/*!
    \fn int BOBUIextListFormat::numberPrefix() const
    \since 4.8

    Returns the list format's number prefix.

    \sa setNumberPrefix()
*/

/*!
    \fn void BOBUIextListFormat::setNumberSuffix(const QString &numberSuffix)
    \since 4.8

    Sets the list format's number suffix to the string specified by
    \a numberSuffix. This can be used with all sorted list types. It does not
    have any effect on unsorted list types.

    The default suffix is ".".

    \sa numberSuffix()
*/

/*!
    \fn int BOBUIextListFormat::numberSuffix() const
    \since 4.8

    Returns the list format's number suffix.

    \sa setNumberSuffix()
*/

/*!
    \fn void BOBUIextListFormat::setStart(int start)
    \since 6.6

    Sets the list format's \a start index.

    This allows you to start a list with an index other than 1. This can be
    used with all sorted list types: for example if the style() is
    BOBUIextListFormat::ListLowerAlpha and start() is \c 4, the first list item
    begins with "d". It does not have any effect on unsorted list types.

    The default start is \c 1.

    \sa start()
*/

/*!
    \fn int BOBUIextListFormat::start() const
    \since 6.6

    Returns the number to be shown by the first list item, if the style() is
    BOBUIextListFormat::ListDecimal, or to offset other sorted list types.

    \sa setStart()
*/

/*!
    \class BOBUIextFrameFormat
    \reentrant

    \brief The BOBUIextFrameFormat class provides formatting information for
    frames in a BOBUIextDocument.
    \inmodule BobUIGui

    \ingroup richtext-processing
    \ingroup shared

    A text frame groups together one or more blocks of text, providing a layer
    of structure larger than the paragraph. The format of a frame specifies
    how it is rendered and positioned on the screen. It does not directly
    specify the behavior of the text formatting within, but provides
    constraints on the layout of its children.

    The frame format defines the width() and height() of the frame on the
    screen. Each frame can have a border() that surrounds its contents with
    a rectangular box. The border is surrounded by a margin() around the frame,
    and the contents of the frame are kept separate from the border by the
    frame's padding(). This scheme is similar to the box model used by Cascading
    Style Sheets for HTML pages.

    \image bobuiextframe-style.png {Diagram showing how padding, border, and margin
           are laid around the text content rectangle.}

    The position() of a frame is set using setPosition() and determines how it
    is located relative to the surrounding text.

    The validity of a BOBUIextFrameFormat object can be determined with the
    isValid() function.

    \sa BOBUIextFrame, BOBUIextBlockFormat
*/

/*!
    \enum BOBUIextFrameFormat::Position

    This enum describes how a frame is located relative to the surrounding text.

    \value InFlow
    \value FloatLeft
    \value FloatRight

    \sa position(), CssFloat
*/

/*!
    \enum BOBUIextFrameFormat::BorderStyle
    \since 4.3

    This enum describes different border styles for the text frame.

    \value BorderStyle_None
    \value BorderStyle_Dotted
    \value BorderStyle_Dashed
    \value BorderStyle_Solid
    \value BorderStyle_Double
    \value BorderStyle_DotDash
    \value BorderStyle_DotDotDash
    \value BorderStyle_Groove
    \value BorderStyle_Ridge
    \value BorderStyle_Inset
    \value BorderStyle_Outset

    \sa borderStyle(), FrameBorderStyle
*/

/*!
    \fn BOBUIextFrameFormat::BOBUIextFrameFormat()

    Constructs a text frame format object with the default properties.
*/
BOBUIextFrameFormat::BOBUIextFrameFormat() : BOBUIextFormat(FrameFormat)
{
    setBorderStyle(BorderStyle_Outset);
    setBorderBrush(BobUI::darkGray);
}

/*!
    \internal
    \fn BOBUIextFrameFormat::BOBUIextFrameFormat(const BOBUIextFormat &other)

    Creates a new frame format with the same attributes as the \a given
    text format.
*/
BOBUIextFrameFormat::BOBUIextFrameFormat(const BOBUIextFormat &fmt)
 : BOBUIextFormat(fmt)
{
}

/*!
    \fn bool BOBUIextFrameFormat::isValid() const

    Returns \c true if the format description is valid; otherwise returns \c false.
*/

/*!
    \fn void BOBUIextFrameFormat::setPosition(Position policy)

    Sets the \a policy for positioning frames with this frame format.

*/

/*!
    \fn Position BOBUIextFrameFormat::position() const

    Returns the positioning policy for frames with this frame format.
*/

/*!
    \fn void BOBUIextFrameFormat::setBorder(qreal width)

    Sets the \a width (in pixels) of the frame's border.
*/

/*!
    \fn qreal BOBUIextFrameFormat::border() const

    Returns the width of the border in pixels.
*/

/*!
    \fn void BOBUIextFrameFormat::setBorderBrush(const QBrush &brush)
    \since 4.3

    Sets the \a brush used for the frame's border.
*/

/*!
    \fn QBrush BOBUIextFrameFormat::borderBrush() const
    \since 4.3

    Returns the brush used for the frame's border.
*/

/*!
    \fn void BOBUIextFrameFormat::setBorderStyle(BorderStyle style)
    \since 4.3

    Sets the \a style of the frame's border.
*/

/*!
    \fn BorderStyle BOBUIextFrameFormat::borderStyle() const
    \since 4.3

    Returns the style of the frame's border.
*/

/*!
    \fn void BOBUIextFrameFormat::setMargin(qreal margin)

    Sets the frame's \a margin in pixels.
    This method also sets the left, right, top and bottom margins
    of the frame to the same value. The individual margins override
    the general margin.
*/
void BOBUIextFrameFormat::setMargin(qreal amargin)
{
    setProperty(FrameMargin, amargin);
    setProperty(FrameTopMargin, amargin);
    setProperty(FrameBottomMargin, amargin);
    setProperty(FrameLeftMargin, amargin);
    setProperty(FrameRightMargin, amargin);
}


/*!
    \fn qreal BOBUIextFrameFormat::margin() const

    Returns the width of the frame's external margin in pixels.
*/

/*!
    \fn void BOBUIextFrameFormat::setTopMargin(qreal margin)
    \since 4.3

    Sets the frame's top \a margin in pixels.
*/

/*!
    \fn qreal BOBUIextFrameFormat::topMargin() const
    \since 4.3

    Returns the width of the frame's top margin in pixels.
*/
qreal BOBUIextFrameFormat::topMargin() const
{
    if (!hasProperty(FrameTopMargin))
        return margin();
    return doubleProperty(FrameTopMargin);
}

/*!
    \fn void BOBUIextFrameFormat::setBottomMargin(qreal margin)
    \since 4.3

    Sets the frame's bottom \a margin in pixels.
*/

/*!
    \fn qreal BOBUIextFrameFormat::bottomMargin() const
    \since 4.3

    Returns the width of the frame's bottom margin in pixels.
*/
qreal BOBUIextFrameFormat::bottomMargin() const
{
    if (!hasProperty(FrameBottomMargin))
        return margin();
    return doubleProperty(FrameBottomMargin);
}

/*!
    \fn void BOBUIextFrameFormat::setLeftMargin(qreal margin)
    \since 4.3

    Sets the frame's left \a margin in pixels.
*/

/*!
    \fn qreal BOBUIextFrameFormat::leftMargin() const
    \since 4.3

    Returns the width of the frame's left margin in pixels.
*/
qreal BOBUIextFrameFormat::leftMargin() const
{
    if (!hasProperty(FrameLeftMargin))
        return margin();
    return doubleProperty(FrameLeftMargin);
}

/*!
    \fn void BOBUIextFrameFormat::setRightMargin(qreal margin)
    \since 4.3

    Sets the frame's right \a margin in pixels.
*/

/*!
    \fn qreal BOBUIextFrameFormat::rightMargin() const
    \since 4.3

    Returns the width of the frame's right margin in pixels.
*/
qreal BOBUIextFrameFormat::rightMargin() const
{
    if (!hasProperty(FrameRightMargin))
        return margin();
    return doubleProperty(FrameRightMargin);
}

/*!
    \fn void BOBUIextFrameFormat::setPadding(qreal width)

    Sets the \a width of the frame's internal padding in pixels.
*/

/*!
    \fn qreal BOBUIextFrameFormat::padding() const

    Returns the width of the frame's internal padding in pixels.
*/

/*!
    \fn void BOBUIextFrameFormat::setWidth(const BOBUIextLength &width)

    Sets the frame's border rectangle's \a width.

    \sa BOBUIextLength
*/

/*!
    \fn void BOBUIextFrameFormat::setWidth(qreal width)
    \overload

    Convenience method that sets the width of the frame's border
    rectangle's width to the specified fixed \a width.
*/

/*!
    \fn BOBUIextFormat::PageBreakFlags BOBUIextFrameFormat::pageBreakPolicy() const
    \since 4.2

    Returns the currently set page break policy for the frame/table. The default is
    BOBUIextFormat::PageBreak_Auto.

    \sa setPageBreakPolicy()
*/

/*!
    \fn void BOBUIextFrameFormat::setPageBreakPolicy(PageBreakFlags policy)
    \since 4.2

    Sets the page break policy for the frame/table to \a policy.

    \sa pageBreakPolicy()
*/

/*!
    \fn BOBUIextLength BOBUIextFrameFormat::width() const

    Returns the width of the frame's border rectangle.

    \sa BOBUIextLength
*/

/*!
    \fn void BOBUIextFrameFormat::setHeight(const BOBUIextLength &height)

    Sets the frame's \a height.
*/

/*!
    \fn void BOBUIextFrameFormat::setHeight(qreal height)
    \overload

    Sets the frame's \a height.
*/

/*!
    \fn qreal BOBUIextFrameFormat::height() const

    Returns the height of the frame's border rectangle.
*/

/*!
    \class BOBUIextTableFormat
    \reentrant

    \brief The BOBUIextTableFormat class provides formatting information for
    tables in a BOBUIextDocument.
    \inmodule BobUIGui

    \ingroup richtext-processing
    \ingroup shared

    A table is a group of cells ordered into rows and columns. Each table
    contains at least one row and one column. Each cell contains a block.
    Tables in rich text documents are formatted using the properties
    defined in this class.

    Tables are horizontally justified within their parent frame according to the
    table's alignment. This can be read with the alignment() function and set
    with setAlignment().

    Cells within the table are separated by cell spacing. The number of pixels
    between cells is set with setCellSpacing() and read with cellSpacing().
    The contents of each cell is surrounded by cell padding. The number of pixels
    between each cell edge and its contents is set with setCellPadding() and read
    with cellPadding().

    \image bobuiexttableformat-cell.png {Diagram showing how cell spacing and padding
           are laid around the cell content rectangle.}

    The table's background color can be read with the background() function,
    and can be specified with setBackground(). The background color of each
    cell can be set independently, and will control the color of the cell within
    the padded area.

    The table format also provides a way to constrain the widths of the columns
    in the table. Columns can be assigned a fixed width, a variable width, or
    a percentage of the available width (see BOBUIextLength). The columns() function
    returns the number of columns with constraints, and the
    columnWidthConstraints() function returns the constraints defined for the
    table. These quantities can also be set by calling setColumnWidthConstraints()
    with a list containing new constraints. If no constraints are
    required, clearColumnWidthConstraints() can be used to remove them.

    \sa BOBUIextTable, BOBUIextTableCell, BOBUIextLength
*/

/*!
    \fn BOBUIextTableFormat::BOBUIextTableFormat()

    Constructs a new table format object.
*/
BOBUIextTableFormat::BOBUIextTableFormat()
 : BOBUIextFrameFormat()
{
    setObjectType(TableObject);
    setCellPadding(4);
    setBorderCollapse(true);
    setBorder(1);
}

/*!
    \internal
    \fn BOBUIextTableFormat::BOBUIextTableFormat(const BOBUIextFormat &other)

    Creates a new table format with the same attributes as the \a given
    text format.
*/
BOBUIextTableFormat::BOBUIextTableFormat(const BOBUIextFormat &fmt)
 : BOBUIextFrameFormat(fmt)
{
}

/*!
    \fn bool BOBUIextTableFormat::isValid() const

    Returns \c true if this table format is valid; otherwise
    returns \c false.
*/


/*!
    \fn int BOBUIextTableFormat::columns() const

    Returns the number of columns specified by the table format.
*/


/*!
    \internal
    \fn void BOBUIextTableFormat::setColumns(int columns)

    Sets the number of \a columns required by the table format.

    \sa columns()
*/

/*!
    \fn void BOBUIextTableFormat::clearColumnWidthConstraints()

    Clears the column width constraints for the table.

    \sa columnWidthConstraints(), setColumnWidthConstraints()
*/

/*!
    \fn void BOBUIextTableFormat::setColumnWidthConstraints(const QList<BOBUIextLength> &constraints)

    Sets the column width \a constraints for the table.

    \sa columnWidthConstraints(), clearColumnWidthConstraints()
*/

/*!
    \fn QList<BOBUIextLength> BOBUIextTableFormat::columnWidthConstraints() const

    Returns a list of constraints used by this table format to control the
    appearance of columns in a table.

    \sa setColumnWidthConstraints()
*/

/*!
    \fn qreal BOBUIextTableFormat::cellSpacing() const

    Returns the table's cell spacing. This describes the distance between
    adjacent cells.
*/

/*!
    \fn void BOBUIextTableFormat::setCellSpacing(qreal spacing)

    Sets the cell \a spacing for the table. This determines the distance
    between adjacent cells.

    This property will be ignored if \l borderCollapse is enabled.
*/

/*!
    \fn qreal BOBUIextTableFormat::cellPadding() const

    Returns the table's cell padding. This describes the distance between
    the border of a cell and its contents.
*/

/*!
    \fn void BOBUIextTableFormat::setCellPadding(qreal padding)

    Sets the cell \a padding for the table. This determines the distance
    between the border of a cell and its contents.
*/

/*!
    \fn void BOBUIextTableFormat::setAlignment(BobUI::Alignment alignment)

    Sets the table's \a alignment.

    \sa alignment()
*/

/*!
    \fn BobUI::Alignment BOBUIextTableFormat::alignment() const

    Returns the table's alignment.

    \sa setAlignment()
*/

/*!
    \fn void BOBUIextTableFormat::setHeaderRowCount(int count)
    \since 4.2

    Declares the first \a count rows of the table as table header.
    The table header rows get repeated when a table is broken
    across a page boundary.
*/

/*!
    \fn int BOBUIextTableFormat::headerRowCount() const
    \since 4.2

    Returns the number of rows in the table that define the header.

    \sa setHeaderRowCount()
*/

/*!
    \fn void BOBUIextTableFormat::setBorderCollapse(bool borderCollapse)
    \since 5.14

    By default, \l borderCollapse() is \c true, which has the following implications:
    \list
    \li The borders and grid of the table will be rendered following the
        CSS table \c border-collapse: \c collapse rules
    \li Setting the \c border property to a minimum value of \c 1 will render a
        one pixel solid inner table grid using the \l borderBrush property and an
        outer border as specified
    \li The various border style properties of \l BOBUIextTableCellFormat can be used to
        customize the grid and have precedence over the border and grid of the table
    \li The \l cellSpacing property will be ignored
    \li For print pagination:
        \list
        \li Columns continued on a page will not have their top cell border rendered
        \li Repeated header rows will always have their bottom cell border rendered
        \endlist
    \endlist

    With \a borderCollapse set to \c false, cell borders can still be styled
    using BOBUIextTableCellFormat but styling will be applied only within
    the cell's frame, which is probably not very useful in practice.

    \note In BobUI versions prior to 6.8, the default value was \c false.

    \sa setBorder(), setBorderBrush(), setBorderStyle()
    \sa BOBUIextTableCellFormat
*/

/*!
    \fn bool BOBUIextTableFormat::borderCollapse() const
    \since 5.14

    Returns \c true if table borders are to be collapsed. The default is \c true.

    \sa setBorderCollapse()
*/

/*!
    \fn void BOBUIextFormat::setBackground(const QBrush &brush)

    Sets the brush use to paint the document's background to the
    \a brush specified.

    \sa background(), clearBackground(), setForeground()
*/

/*!
    \fn QColor BOBUIextFormat::background() const

    Returns the brush used to paint the document's background.

    \sa setBackground(), clearBackground(), foreground()
*/

/*!
    \fn void BOBUIextFormat::clearBackground()

    Clears the brush used to paint the document's background. The default
    brush will be used.

    \sa background(), setBackground(), clearForeground()
*/


/*!
    \class BOBUIextImageFormat
    \reentrant

    \brief The BOBUIextImageFormat class provides formatting information for
    images in a BOBUIextDocument.
    \inmodule BobUIGui

    \ingroup richtext-processing
    \ingroup shared

    Inline images are represented by a Unicode value U+FFFC (OBJECT
    REPLACEMENT CHARACTER) which has an associated BOBUIextImageFormat. The
    image format specifies a name with setName() that is used to
    locate the image. The size of the rectangle that the image will
    occupy is specified in pixels using setWidth() and setHeight().
    The desired image quality may be set with setQuality().

    Images can be supplied in any format for which BobUI has an image
    reader, so SVG drawings can be included alongside PNG, TIFF and
    other bitmap formats.

    \sa QImage, QImageReader
*/

/*!
    \fn BOBUIextImageFormat::BOBUIextImageFormat()

    Creates a new image format object.
*/
BOBUIextImageFormat::BOBUIextImageFormat() : BOBUIextCharFormat() { setObjectType(ImageObject); }

/*!
    \internal
    \fn BOBUIextImageFormat::BOBUIextImageFormat(const BOBUIextFormat &other)

    Creates a new image format with the same attributes as the \a given
    text format.
*/
BOBUIextImageFormat::BOBUIextImageFormat(const BOBUIextFormat &fmt)
 : BOBUIextCharFormat(fmt)
{
}

/*!
    \fn bool BOBUIextImageFormat::isValid() const

    Returns \c true if this image format is valid; otherwise returns \c false.
*/


/*!
    \fn void BOBUIextImageFormat::setName(const QString &name)

    Sets the \a name of the image. The \a name is used to locate the image
    in the application's resources.

    \sa name()
*/


/*!
    \fn QString BOBUIextImageFormat::name() const

    Returns the name of the image. The name refers to an entry in the
    application's resources file.

    \sa setName()
*/

/*!
    \fn void BOBUIextImageFormat::setWidth(qreal width)

    Sets the \a width of the rectangle occupied by the image.

    \sa width(), setHeight(), maximumWidth()
*/


/*!
    \fn qreal BOBUIextImageFormat::width() const

    Returns the width of the rectangle occupied by the image.

    \sa height(), setWidth()
*/

/*!
    \fn void BOBUIextImageFormat::setMaximumWidth(BOBUIextLength maximumWidth)

    Sets the \a maximumWidth of the rectangle occupied by the image. This
    can be an absolute number or a percentage of the available document size.

    \sa width(), setHeight()
*/


/*!
    \fn BOBUIextLength BOBUIextImageFormat::maximumWidth() const

    Returns the maximum width of the rectangle occupied by the image.

    \sa width(), setMaximumWidth()
*/


/*!
    \fn void BOBUIextImageFormat::setHeight(qreal height)

    Sets the \a height of the rectangle occupied by the image.

    \sa height(), setWidth()
*/


/*!
    \fn qreal BOBUIextImageFormat::height() const

    Returns the height of the rectangle occupied by the image.

    \sa width(), setHeight()
*/

/*!
    \fn void BOBUIextImageFormat::setQuality(int quality = 100)
    \since 5.12

    Sets the quality that should be used by exporters when exporting the image. BOBUIextDocumentWriter
    will export jpg images with the \a quality set here when exporting to ODF files if \a quality is
    set to a value between 0 and 100. Or it will export png images if \a quality is set to 100
    (default) or greater.

    \sa quality()
*/


/*!
    \fn qreal BOBUIextImageFormat::quality() const
    \since 5.12

    Returns the value set by setQuality().

    \sa setQuality()
*/

/*!
    \fn void BOBUIextCharFormat::setFontCapitalization(QFont::Capitalization capitalization)
    \since 4.4

    Sets the capitalization of the text that appears in this font to \a capitalization.

    A font's capitalization makes the text appear in the selected capitalization mode.

    \sa fontCapitalization()
*/

/*!
    \fn Capitalization BOBUIextCharFormat::fontCapitalization() const
    \since 4.4

    Returns the current capitalization type of the font.
*/

/*!
    \fn void BOBUIextCharFormat::setFontLetterSpacingType(QFont::SpacingType letterSpacingType)
    \since 5.0

    Sets the letter spacing type of this format to \a letterSpacingType.

    \sa fontLetterSpacingType()
    \sa setFontLetterSpacing()
    \sa fontLetterSpacing()
*/

/*!
    \fn QFont::SpacingType BOBUIextCharFormat::fontLetterSpacingType() const
    \since 5.0

    Returns the letter spacing type of this format..

    \sa setFontLetterSpacingType()
    \sa setFontLetterSpacing()
    \sa fontLetterSpacing()
*/

/*!
    \fn void BOBUIextCharFormat::setFontLetterSpacing(qreal spacing)
    \since 4.4

    Sets the letter spacing of this format to the given \a spacing. The meaning of the value
    depends on the font letter spacing type.

    For percentage spacing a value of 100 indicates default spacing; a value of 200 doubles the
    amount of space a letter takes.

    \sa fontLetterSpacing()
    \sa setFontLetterSpacingType()
    \sa fontLetterSpacingType()
*/

/*!
    \fn qreal BOBUIextCharFormat::fontLetterSpacing() const
    \since 4.4

    Returns the current letter spacing.

    \sa setFontLetterSpacing()
    \sa setFontLetterSpacingType()
    \sa fontLetterSpacingType()
*/

/*!
    \fn void BOBUIextCharFormat::setFontWordSpacing(qreal spacing)
    \since 4.4

    Sets the word spacing of this format to the given \a spacing, in pixels.

    \sa fontWordSpacing()
*/

/*!
    \fn qreal BOBUIextCharFormat::fontWordSpacing() const
    \since 4.4

    Returns the current word spacing value.
*/

/*!
    \fn void BOBUIextCharFormat::setFontStretch(int factor)
    \since 5.0

    Sets the stretch factor for the font to \a factor.

    The stretch factor changes the width of all characters in the font by factor percent. For example, setting \a factor to 150 results in all characters in the font being 1.5 times (ie. 150%) wider. The default stretch factor is 100. The minimum stretch factor is 1, and the maximum stretch factor is 4000.

    The stretch factor is only applied to outline fonts. The stretch factor is ignored for bitmap fonts.

    \sa fontStretch()
*/

/*!
    \fn int BOBUIextCharFormat::fontStretch() const
    \since 5.0

    Returns the current font stretching.
    \sa setFontStretch()
*/

/*!
   \fn qreal BOBUIextTableCellFormat::topPadding() const
    \since 4.4

   Gets the top padding of the table cell.

   \sa setTopPadding(), leftPadding(), rightPadding(), bottomPadding()
*/

/*!
   \fn qreal BOBUIextTableCellFormat::bottomPadding() const
    \since 4.4

   Gets the bottom padding of the table cell.

   \sa setBottomPadding(), leftPadding(), rightPadding(), topPadding()
*/

/*!
   \fn qreal BOBUIextTableCellFormat::leftPadding() const
    \since 4.4

   Gets the left padding of the table cell.

   \sa setLeftPadding(), rightPadding(), topPadding(), bottomPadding()
*/

/*!
   \fn qreal BOBUIextTableCellFormat::rightPadding() const
    \since 4.4

   Gets the right padding of the table cell.

   \sa setRightPadding(), leftPadding(), topPadding(), bottomPadding()
*/

/*!
   \fn void BOBUIextTableCellFormat::setTopPadding(qreal padding)
    \since 4.4

   Sets the top \a padding of the table cell.

   \sa topPadding(), setLeftPadding(), setRightPadding(), setBottomPadding()
*/

/*!
   \fn void BOBUIextTableCellFormat::setBottomPadding(qreal padding)
    \since 4.4

   Sets the bottom \a padding of the table cell.

   \sa bottomPadding(), setLeftPadding(), setRightPadding(), setTopPadding()
*/

/*!
   \fn void BOBUIextTableCellFormat::setLeftPadding(qreal padding)
    \since 4.4

   Sets the left \a padding of the table cell.

   \sa leftPadding(), setRightPadding(), setTopPadding(), setBottomPadding()
*/

/*!
   \fn void BOBUIextTableCellFormat::setRightPadding(qreal padding)
    \since 4.4

   Sets the right \a padding of the table cell.

   \sa rightPadding(), setLeftPadding(), setTopPadding(), setBottomPadding()
*/

/*!
   \fn void BOBUIextTableCellFormat::setPadding(qreal padding)
    \since 4.4

   Sets the left, right, top, and bottom \a padding of the table cell.

   \sa setLeftPadding(), setRightPadding(), setTopPadding(), setBottomPadding()
*/

/*!
   \fn void BOBUIextTableCellFormat::setTopBorder(qreal width)
   \since 5.14

   Sets the top border \a width of the table cell.

   \sa BOBUIextTableFormat::setBorderCollapse
*/

/*!
   \fn qreal BOBUIextTableCellFormat::topBorder() const
   \since 5.14

   Returns the top border width of the table cell.
*/

/*!
   \fn void BOBUIextTableCellFormat::setBottomBorder(qreal width)
   \since 5.14

   Sets the bottom border \a width of the table cell.

   \sa BOBUIextTableFormat::setBorderCollapse
*/

/*!
   \fn qreal BOBUIextTableCellFormat::bottomBorder() const
   \since 5.14

   Returns the bottom border width of the table cell.
*/

/*!
   \fn void BOBUIextTableCellFormat::setLeftBorder(qreal width)
   \since 5.14

   Sets the left border \a width of the table cell.

   \sa BOBUIextTableFormat::setBorderCollapse
*/

/*!
   \fn qreal BOBUIextTableCellFormat::leftBorder() const
   \since 5.14

   Returns the left border width of the table cell.
*/

/*!
   \fn void BOBUIextTableCellFormat::setRightBorder(qreal width)
   \since 5.14

   Sets the right border \a width of the table cell.

   \sa BOBUIextTableFormat::setBorderCollapse
*/

/*!
   \fn qreal BOBUIextTableCellFormat::rightBorder() const
   \since 5.14

   Returns the right border width of the table cell.
*/

/*!
   \fn void BOBUIextTableCellFormat::setBorder(qreal width)
   \since 5.14

   Sets the left, right, top, and bottom border \a width of the table cell.

   \sa setLeftBorder(), setRightBorder(), setTopBorder(), setBottomBorder()
   \sa BOBUIextTableFormat::setBorderCollapse
*/

/*!
   \fn void BOBUIextTableCellFormat::setTopBorderStyle(BOBUIextFrameFormat::BorderStyle style)
   \since 5.14

   Sets the top border \a style of the table cell.

   \sa BOBUIextTableFormat::setBorderCollapse
*/

/*!
   \fn BOBUIextFrameFormat::BorderStyle BOBUIextTableCellFormat::topBorderStyle() const
   \since 5.14

   Returns the top border style of the table cell.
*/

/*!
   \fn void BOBUIextTableCellFormat::setBottomBorderStyle(BOBUIextFrameFormat::BorderStyle style)
   \since 5.14

   Sets the bottom border \a style of the table cell.

   \sa BOBUIextTableFormat::setBorderCollapse
*/

/*!
   \fn BOBUIextFrameFormat::BorderStyle BOBUIextTableCellFormat::bottomBorderStyle() const
   \since 5.14

   Returns the bottom border style of the table cell.
*/

/*!
   \fn void BOBUIextTableCellFormat::setLeftBorderStyle(BOBUIextFrameFormat::BorderStyle style)
   \since 5.14

   Sets the left border \a style of the table cell.

   \sa BOBUIextTableFormat::setBorderCollapse
*/

/*!
   \fn BOBUIextFrameFormat::BorderStyle BOBUIextTableCellFormat::leftBorderStyle() const
   \since 5.14

   Returns the left border style of the table cell.
*/

/*!
   \fn void BOBUIextTableCellFormat::setRightBorderStyle(BOBUIextFrameFormat::BorderStyle style)
   \since 5.14

   Sets the right border \a style of the table cell.

   \sa BOBUIextTableFormat::setBorderCollapse
*/

/*!
   \fn BOBUIextFrameFormat::BorderStyle BOBUIextTableCellFormat::rightBorderStyle() const
   \since 5.14

   Returns the right border style of the table cell.
*/

/*!
   \fn void BOBUIextTableCellFormat::setBorderStyle(BOBUIextFrameFormat::BorderStyle style)
   \since 5.14

   Sets the left, right, top, and bottom border \a style of the table cell.

   \sa setLeftBorderStyle(), setRightBorderStyle(), setTopBorderStyle(), setBottomBorderStyle()
   \sa BOBUIextTableFormat::setBorderCollapse
*/

/*!
   \fn void BOBUIextTableCellFormat::setTopBorderBrush(const QBrush &brush)
   \since 5.14

   Sets the top border \a brush of the table cell.

   \sa BOBUIextTableFormat::setBorderCollapse
*/

/*!
   \fn QBrush BOBUIextTableCellFormat::topBorderBrush() const
   \since 5.14

   Returns the top border brush of the table cell.
*/

/*!
   \fn void BOBUIextTableCellFormat::setBottomBorderBrush(const QBrush &brush)
   \since 5.14

   Sets the bottom border \a brush of the table cell.

   \sa BOBUIextTableFormat::setBorderCollapse
*/

/*!
   \fn QBrush BOBUIextTableCellFormat::bottomBorderBrush() const
   \since 5.14

   Returns the bottom border brush of the table cell.
*/

/*!
   \fn void BOBUIextTableCellFormat::setLeftBorderBrush(const QBrush &brush)
   \since 5.14

   Sets the left border \a brush of the table cell.

   \sa BOBUIextTableFormat::setBorderCollapse
*/

/*!
   \fn QBrush BOBUIextTableCellFormat::leftBorderBrush() const
   \since 5.14

   Returns the left border brush of the table cell.
*/

/*!
   \fn void BOBUIextTableCellFormat::setRightBorderBrush(const QBrush &brush)
   \since 5.14

   Sets the right border \a brush of the table cell.

   \sa BOBUIextTableFormat::setBorderCollapse
*/

/*!
   \fn QBrush BOBUIextTableCellFormat::rightBorderBrush() const
   \since 5.14

   Returns the right border brush of the table cell.
*/

/*!
   \fn void BOBUIextTableCellFormat::setBorderBrush(const QBrush &brush)
   \since 5.14

   Sets the left, right, top, and bottom border \a brush of the table cell.

   \sa setLeftBorderBrush(), setRightBorderBrush(), setTopBorderBrush(), setBottomBorderBrush()
   \sa BOBUIextTableFormat::setBorderCollapse
*/

/*!
    \fn bool BOBUIextTableCellFormat::isValid() const
    \since 4.4

    Returns \c true if this table cell format is valid; otherwise returns \c false.
*/

/*!
    \fn BOBUIextTableCellFormat::BOBUIextTableCellFormat()
    \since 4.4

    Constructs a new table cell format object.
*/
BOBUIextTableCellFormat::BOBUIextTableCellFormat()
    : BOBUIextCharFormat()
{
    setObjectType(TableCellObject);
}

/*!
    \internal
    \fn BOBUIextTableCellFormat::BOBUIextTableCellFormat(const BOBUIextFormat &other)

    Creates a new table cell format with the same attributes as the \a given
    text format.
*/
BOBUIextTableCellFormat::BOBUIextTableCellFormat(const BOBUIextFormat &fmt)
    : BOBUIextCharFormat(fmt)
{
}

/*!
    \class BOBUIextTableCellFormat
    \reentrant
    \since 4.4

    \brief The BOBUIextTableCellFormat class provides formatting information for
    table cells in a BOBUIextDocument.
    \inmodule BobUIGui

    \ingroup richtext-processing
    \ingroup shared

    The table cell format of a table cell in a document specifies the visual
    properties of the table cell.

    The padding properties of a table cell are controlled by setLeftPadding(),
    setRightPadding(), setTopPadding(), and setBottomPadding(). All the paddings
    can be set at once using setPadding().

    \sa BOBUIextFormat, BOBUIextBlockFormat, BOBUIextTableFormat, BOBUIextCharFormat
*/

// ------------------------------------------------------

BOBUIextFormatCollection::~BOBUIextFormatCollection()
{
}

void BOBUIextFormatCollection::clear()
{
    formats.clear();
    objFormats.clear();
    hashes.clear();
}

int BOBUIextFormatCollection::indexForFormat(const BOBUIextFormat &format)
{
    size_t hash = getHash(format.d, format.format_type);
    auto i = hashes.constFind(hash);
    while (i != hashes.constEnd() && i.key() == hash) {
        if (formats.value(i.value()) == format) {
            return i.value();
        }
        ++i;
    }

    int idx = formats.size();
    formats.append(format);

    BOBUI_TRY{
        BOBUIextFormat &f = formats.last();
        if (!f.d)
            f.d = new BOBUIextFormatPrivate;
        f.d->resolveFont(defaultFnt);

        hashes.insert(hash, idx);

    } BOBUI_CATCH(...) {
        formats.pop_back();
        BOBUI_RETHROW;
    }
    return idx;
}

bool BOBUIextFormatCollection::hasFormatCached(const BOBUIextFormat &format) const
{
    size_t hash = getHash(format.d, format.format_type);
    auto i = hashes.constFind(hash);
    while (i != hashes.constEnd() && i.key() == hash) {
        if (formats.value(i.value()) == format) {
            return true;
        }
        ++i;
    }
    return false;
}

int BOBUIextFormatCollection::objectFormatIndex(int objectIndex) const
{
    if (objectIndex == -1 || objectIndex >= objFormats.size())
        return -1;
    return objFormats.at(objectIndex);
}

void BOBUIextFormatCollection::setObjectFormatIndex(int objectIndex, int formatIndex)
{
    objFormats[objectIndex] = formatIndex;
}

int BOBUIextFormatCollection::createObjectIndex(const BOBUIextFormat &f)
{
    const int objectIndex = objFormats.size();
    objFormats.append(indexForFormat(f));
    return objectIndex;
}

BOBUIextFormat BOBUIextFormatCollection::format(int idx) const
{
    if (idx < 0 || idx >= formats.size())
        return BOBUIextFormat();

    return formats.at(idx);
}

void BOBUIextFormatCollection::setDefaultFont(const QFont &f)
{
    defaultFnt = f;
    for (int i = 0; i < formats.size(); ++i)
        if (formats.at(i).d)
            formats[i].d->resolveFont(defaultFnt);
}

#ifndef BOBUI_NO_DEBUG_STREAM
QDebug operator<<(QDebug dbg, const BOBUIextLength &l)
{
    QDebugStateSaver saver(dbg);
    dbg.nospace() << "BOBUIextLength(BOBUIextLength::Type(" << l.type() << "))";
    return dbg;
}

QDebug operator<<(QDebug dbg, const BOBUIextFormat &f)
{
    QDebugStateSaver saver(dbg);
    dbg.nospace() << "BOBUIextFormat(BOBUIextFormat::FormatType(" << f.type() << "))";
    return dbg;
}

#endif

BOBUI_END_NAMESPACE

#include "moc_bobuiextformat.cpp"
