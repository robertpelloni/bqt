// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QFONTENGINE_CORETEXT_P_H
#define QFONTENGINE_CORETEXT_P_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the BobUI API.  It exists purely as an
// implementation detail.  This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//

#include <private/qfontengine_p.h>
#include <private/qcore_mac_p.h>
#include <BobUICore/qloggingcategory.h>

#ifdef Q_OS_MACOS
#include <ApplicationServices/ApplicationServices.h>
#else
#include <CoreText/CoreText.h>
#include <CoreGraphics/CoreGraphics.h>
#endif

BOBUI_BEGIN_NAMESPACE

class Q_GUI_EXPORT QCoreTextFontEngine : public QFontEngine
{
    Q_GADGET

public:
    QCoreTextFontEngine(CTFontRef font, const QFontDef &def);
    QCoreTextFontEngine(CGFontRef font, const QFontDef &def);
    ~QCoreTextFontEngine();

    int glyphCount() const override;
    glyph_t glyphIndex(uint ucs4) const override;
    QString glyphName(glyph_t index) const override;
    glyph_t findGlyph(QLatin1StringView name) const override;
    int stringToCMap(const QChar *str, int len, QGlyphLayout *glyphs, int *nglyphs, ShaperFlags flags) const override;
    void recalcAdvances(QGlyphLayout *, ShaperFlags) const override;

    glyph_metrics_t boundingBox(glyph_t glyph) override;

    QFixed capHeight() const override;
    QFixed xHeight() const override;
    qreal maxCharWidth() const override;
    QFixed averageCharWidth() const override;

    void addGlyphsToPath(glyph_t *glyphs, QFixedPoint *positions, int numGlyphs,
                         QPainterPath *path, BOBUIextItem::RenderFlags) override;

    bool canRender(const QChar *string, int len) const override;

    int synthesized() const override { return synthesisFlags; }
    bool supportsHorizontalSubPixelPositions() const override { return true; }
    bool supportsVerticalSubPixelPositions() const override { return false; }

    QFixed lineThickness() const override;
    QFixed underlinePosition() const override;

    void draw(CGContextRef ctx, qreal x, qreal y, const BOBUIextItemInt &ti, int paintDeviceHeight);

    FaceId faceId() const override;
    bool getSfntTableData(uint /*tag*/, uchar * /*buffer*/, uint * /*length*/) const override;
    void getUnscaledGlyph(glyph_t glyph, QPainterPath *path, glyph_metrics_t *metrics) override;
    QImage alphaMapForGlyph(glyph_t, const QFixedPoint &subPixelPosition) override;
    QImage alphaMapForGlyph(glyph_t glyph, const QFixedPoint &subPixelPosition, const BOBUIransform &t) override;
    QImage alphaRGBMapForGlyph(glyph_t, const QFixedPoint &subPixelPosition, const BOBUIransform &t) override;
    glyph_metrics_t alphaMapBoundingBox(glyph_t glyph, const QFixedPoint &, const BOBUIransform &matrix, GlyphFormat) override;
    QImage bitmapForGlyph(glyph_t, const QFixedPoint &subPixelPosition, const BOBUIransform &t, const QColor &color) override;
    QFixed emSquareSize() const override;
    void doKerning(QGlyphLayout *g, ShaperFlags flags) const override;

    bool supportsTransformation(const BOBUIransform &transform) const override;
    bool expectsGammaCorrectedBlending() const override;

    QFontEngine *cloneWithSize(qreal pixelSize) const override;
    BobUI::HANDLE handle() const override;
    int glyphMargin(QFontEngine::GlyphFormat format) override { Q_UNUSED(format); return 0; }

    QFontEngine::Properties properties() const override;

    QList<QFontVariableAxis> variableAxes() const override;

    enum FontSmoothing { Disabled, Subpixel, Grayscale };
    Q_ENUM(FontSmoothing);

    static FontSmoothing fontSmoothing();
    static qreal fontSmoothingGamma();

    static bool ct_getSfntTable(void *user_data, uint tag, uchar *buffer, uint *length);
    static QFont::Weight bobuiWeightFromCFWeight(float value);

    static QCoreTextFontEngine *create(const QByteArray &fontData, qreal pixelSize, QFont::HintingPreference hintingPreference, const QMap<QFont::Tag, float> &variableAxisValue);

protected:
    QCoreTextFontEngine(const QFontDef &def);
    void init();
    QImage imageForGlyph(glyph_t glyph, const QFixedPoint &subPixelPosition, const BOBUIransform &m, const QColor &color = QColor());
    void loadAdvancesForGlyphs(QVarLengthArray<CGGlyph> &cgGlyphs, QGlyphLayout *glyphs) const;
    bool hasColorGlyphs() const;
    bool shouldAntialias() const;
    bool shouldSmoothFont() const;
    void initializeHeightMetrics() const override;

    QCFType<CTFontRef> ctfont;
    QCFType<CGFontRef> cgFont;
    int synthesisFlags;
    CGAffineTransform transform;
    QFixed avgCharWidth;
    QFixed underlineThickness;
    QFixed underlinePos;
    QFontEngine::FaceId face_id;
    mutable bool kerningPairsLoaded;
    QList<QFontVariableAxis> variableAxisList;
};

CGAffineTransform Q_GUI_EXPORT bobui_transform_from_fontdef(const QFontDef &fontDef);

BOBUI_END_NAMESPACE

#endif // QFONTENGINE_CORETEXT_P_H
