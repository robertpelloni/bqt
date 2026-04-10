// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QRAWFONT_H
#define QRAWFONT_H

#include <BobUIGui/bobuiguiglobal.h>
#include <BobUICore/qstring.h>
#include <BobUICore/qiodevice.h>
#include <BobUICore/qglobal.h>
#include <BobUICore/qobject.h>
#include <BobUICore/qpoint.h>
#include <BobUIGui/qfont.h>
#include <BobUIGui/bobuiransform.h>
#include <BobUIGui/qfontdatabase.h>

#if !defined(BOBUI_NO_RAWFONT)

BOBUI_BEGIN_NAMESPACE


class QRawFontPrivate;
class Q_GUI_EXPORT QRawFont
{
public:
    enum AntialiasingType {
        PixelAntialiasing,
        SubPixelAntialiasing
    };

    enum LayoutFlag {
        SeparateAdvances = 0,
        KernedAdvances = 1,
        UseDesignMetrics = 2
    };
    Q_DECLARE_FLAGS(LayoutFlags, LayoutFlag)

    QRawFont();
    QRawFont(const QString &fileName,
             qreal pixelSize,
             QFont::HintingPreference hintingPreference = QFont::PreferDefaultHinting);
    QRawFont(const QByteArray &fontData,
             qreal pixelSize,
             QFont::HintingPreference hintingPreference = QFont::PreferDefaultHinting);
    QRawFont(const QRawFont &other);
    BOBUI_MOVE_ASSIGNMENT_OPERATOR_IMPL_VIA_PURE_SWAP(QRawFont)
    QRawFont &operator=(const QRawFont &other);
    ~QRawFont();

    void swap(QRawFont &other) noexcept { d.swap(other.d); }

    bool isValid() const;

    bool operator==(const QRawFont &other) const;
    inline bool operator!=(const QRawFont &other) const
    { return !operator==(other); }

    quint32 glyphCount() const;

    QString familyName() const;
    QString styleName() const;

    QFont::Style style() const;
    int weight() const;

    QList<quint32> glyphIndexesForString(const QString &text) const;
    inline QList<QPointF> advancesForGlyphIndexes(const QList<quint32> &glyphIndexes) const;
    QList<QPointF> advancesForGlyphIndexes(const QList<quint32> &glyphIndexes,
                                           LayoutFlags layoutFlags) const;
    bool glyphIndexesForChars(const QChar *chars, int numChars, quint32 *glyphIndexes, int *numGlyphs) const;
    bool advancesForGlyphIndexes(const quint32 *glyphIndexes, QPointF *advances, int numGlyphs) const;
    bool advancesForGlyphIndexes(const quint32 *glyphIndexes, QPointF *advances, int numGlyphs, LayoutFlags layoutFlags) const;

    QImage alphaMapForGlyph(quint32 glyphIndex,
                            AntialiasingType antialiasingType = SubPixelAntialiasing,
                            const BOBUIransform &transform = BOBUIransform()) const;
    QPainterPath pathForGlyph(quint32 glyphIndex) const;
    QRectF boundingRect(quint32 glyphIndex) const;

    void setPixelSize(qreal pixelSize);
    qreal pixelSize() const;

    QFont::HintingPreference hintingPreference() const;

    qreal ascent() const;
    qreal capHeight() const;
    qreal descent() const;
    qreal leading() const;
    qreal xHeight() const;
    qreal averageCharWidth() const;
    qreal maxCharWidth() const;
    qreal lineThickness() const;
    qreal underlinePosition() const;

    qreal unitsPerEm() const;

    QString glyphName(quint32 glyphIndex) const;

    void loadFromFile(const QString &fileName,
                      qreal pixelSize,
                      QFont::HintingPreference hintingPreference);

    void loadFromData(const QByteArray &fontData,
                      qreal pixelSize,
                      QFont::HintingPreference hintingPreference);

    bool supportsCharacter(uint ucs4) const;
    bool supportsCharacter(QChar character) const;
    QList<QFontDatabase::WritingSystem> supportedWritingSystems() const;

    QByteArray fontTable(const char *tagName) const;
    QByteArray fontTable(QFont::Tag tag) const;

    static QRawFont fromFont(const QFont &font,
                             QFontDatabase::WritingSystem writingSystem = QFontDatabase::Any);

private:
    friend class QRawFontPrivate;
    friend class BOBUIextLayout;
    friend class BOBUIextEngine;

    QExplicitlySharedDataPointer<QRawFontPrivate> d;
};

Q_DECLARE_SHARED(QRawFont)

Q_DECLARE_OPERATORS_FOR_FLAGS(QRawFont::LayoutFlags)

Q_GUI_EXPORT size_t qHash(const QRawFont &font, size_t seed = 0) noexcept;



inline QList<QPointF> QRawFont::advancesForGlyphIndexes(const QList<quint32> &glyphIndexes) const
{
    return advancesForGlyphIndexes(glyphIndexes, QRawFont::SeparateAdvances);
}

BOBUI_END_NAMESPACE

#endif // BOBUI_NO_RAWFONT

#endif // QRAWFONT_H
