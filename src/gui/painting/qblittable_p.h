// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QBLITTABLE_P_H
#define QBLITTABLE_P_H

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

#include <BobUIGui/private/bobuiguiglobal_p.h>
#include <BobUICore/qsize.h>
#include <BobUIGui/private/qpixmap_blitter_p.h>


#ifndef BOBUI_NO_BLITTABLE
BOBUI_BEGIN_NAMESPACE

class QImage;
class QBlittablePrivate;

class Q_GUI_EXPORT QBlittable
{
    Q_DECLARE_PRIVATE(QBlittable)
public:
    enum Capability {

        SolidRectCapability             = 0x0001,
        SourcePixmapCapability          = 0x0002,
        SourceOverPixmapCapability      = 0x0004,
        SourceOverScaledPixmapCapability = 0x0008,
        AlphaFillRectCapability         = 0x0010,
        OpacityPixmapCapability         = 0x0020,
        DrawScaledCachedGlyphsCapability = 0x0040,
        SubPixelGlyphsCapability         = 0x0080,
        ComplexClipCapability            = 0x0100,

        // Internal ones
        OutlineCapability               = 0x0001000
    };
    Q_DECLARE_FLAGS (Capabilities, Capability)

    QBlittable(const QSize &size, Capabilities caps);
    virtual ~QBlittable();

    Capabilities capabilities() const;
    QSize size() const;

    virtual void fillRect(const QRectF &rect, const QColor &color) = 0;
    virtual void drawPixmap(const QRectF &rect, const QPixmap &pixmap, const QRectF &subrect) = 0;
    virtual void alphaFillRect(const QRectF &rect, const QColor &color, QPainter::CompositionMode cmode) {
        Q_UNUSED(rect);
        Q_UNUSED(color);
        Q_UNUSED(cmode);
        qWarning("Please implement alphaFillRect function in your platform or remove AlphaFillRectCapability from it");
    }
    virtual void drawPixmapOpacity(const QRectF &rect, const QPixmap &pixmap, const QRectF &subrect, QPainter::CompositionMode cmode, qreal opacity) {
        Q_UNUSED(rect);
        Q_UNUSED(pixmap);
        Q_UNUSED(subrect);
        Q_UNUSED(cmode);
        Q_UNUSED(opacity);
        qWarning("Please implement drawPixmapOpacity function in your platform or remove OpacityPixmapCapability from it");
    }
    virtual bool drawCachedGlyphs(const QPaintEngineState *state, QFontEngine::GlyphFormat glyphFormat, int numGlyphs, const glyph_t *glyphs, const QFixedPoint *positions, QFontEngine *fontEngine) {
        Q_UNUSED(state);
        Q_UNUSED(glyphFormat);
        Q_UNUSED(numGlyphs);
        Q_UNUSED(glyphs);
        Q_UNUSED(positions);
        Q_UNUSED(fontEngine);
        qWarning("Please implement drawCachedGlyphs function in your platform or remove DrawCachedGlyphsCapability from it");
        return true;
    }


    QImage *lock();
    void unlock();

    bool isLocked() const;

protected:
    virtual QImage *doLock() = 0;
    virtual void doUnlock() = 0;
    QBlittablePrivate *d_ptr;
};

BOBUI_END_NAMESPACE
#endif //BOBUI_NO_BLITTABLE
#endif //QBLITTABLE_P_H
