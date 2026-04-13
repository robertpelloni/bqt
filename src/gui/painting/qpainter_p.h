// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QPAINTER_P_H
#define QPAINTER_P_H

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

#include <BobUICore/qspan.h>
#include <BobUICore/qvarlengtharray.h>
#include <BobUIGui/private/bobuiguiglobal_p.h>
#include "BobUIGui/qbrush.h"
#include "BobUIGui/qcolorspace.h"
#include "BobUIGui/qcolortransform.h"
#include "BobUIGui/qfont.h"
#include "BobUIGui/qpen.h"
#include "BobUIGui/qregion.h"
#include "BobUIGui/qpainter.h"
#include "BobUIGui/qpainterpath.h"
#include "BobUIGui/qpaintengine.h"

#include <private/qpen_p.h>

#include <memory>
#include <stack>

BOBUI_BEGIN_NAMESPACE

class QPaintEngine;
class QEmulationPaintEngine;
class QPaintEngineEx;
struct QFixedPoint;

struct BOBUILWExtra;

struct DataPtrContainer {
    void *ptr;
};

inline const void *data_ptr(const BOBUIransform &t) { return (const DataPtrContainer *) &t; }
inline bool bobuiransform_fast_equals(const BOBUIransform &a, const BOBUIransform &b) { return data_ptr(a) == data_ptr(b); }

// QPen inline functions...
inline QPen::DataPtr &data_ptr(const QPen &p) { return const_cast<QPen &>(p).data_ptr(); }
inline bool qpen_fast_equals(const QPen &a, const QPen &b) { return data_ptr(a) == data_ptr(b); }
inline QBrush qpen_brush(const QPen &p) { return data_ptr(p)->brush; }
inline qreal qpen_widthf(const QPen &p) { return data_ptr(p)->width; }
inline BobUI::PenStyle qpen_style(const QPen &p) { return data_ptr(p)->style; }
inline BobUI::PenCapStyle qpen_capStyle(const QPen &p) { return data_ptr(p)->capStyle; }
inline BobUI::PenJoinStyle qpen_joinStyle(const QPen &p) { return data_ptr(p)->joinStyle; }

// QBrush inline functions...
inline QBrush::DataPtr &data_ptr(const QBrush &p) { return const_cast<QBrush &>(p).data_ptr(); }
inline bool qbrush_fast_equals(const QBrush &a, const QBrush &b) { return data_ptr(a) == data_ptr(b); }
inline BobUI::BrushStyle qbrush_style(const QBrush &b) { return data_ptr(b)->style; }
inline const QColor &qbrush_color(const QBrush &b) { return data_ptr(b)->color; }
inline bool qbrush_has_transform(const QBrush &b) { return data_ptr(b)->transform.type() > BOBUIransform::TxNone; }

class QPainterClipInfo
{
public:
    QPainterClipInfo() { } // for QList, don't use
    enum ClipType { RegionClip, PathClip, RectClip, RectFClip };

    QPainterClipInfo(const QPainterPath &p, BobUI::ClipOperation op, const BOBUIransform &m) :
        clipType(PathClip), matrix(m), operation(op), path(p) { }

    QPainterClipInfo(const QRegion &r, BobUI::ClipOperation op, const BOBUIransform &m) :
        clipType(RegionClip), matrix(m), operation(op), region(r) { }

    QPainterClipInfo(const QRect &r, BobUI::ClipOperation op, const BOBUIransform &m) :
        clipType(RectClip), matrix(m), operation(op), rect(r) { }

    QPainterClipInfo(const QRectF &r, BobUI::ClipOperation op, const BOBUIransform &m) :
        clipType(RectFClip), matrix(m), operation(op), rectf(r) { }

    ClipType clipType;
    BOBUIransform matrix;
    BobUI::ClipOperation operation;
    QPainterPath path;
    QRegion region;
    QRect rect;
    QRectF rectf;

    // ###
//     union {
//         QRegionData *d;
//         QPainterPathPrivate *pathData;

//         struct {
//             int x, y, w, h;
//         } rectData;
//         struct {
//             qreal x, y, w, h;
//         } rectFData;
//     };

};

Q_DECLARE_TYPEINFO(QPainterClipInfo, Q_RELOCATABLE_TYPE);

class Q_GUI_EXPORT QPainterState : public QPaintEngineState
{
public:
    QPainterState();
    QPainterState(const QPainterState *s);
    virtual ~QPainterState();
    void init(QPainter *p);

    QPointF brushOrigin;
    QFont font;
    QFont deviceFont;
    QPen pen;
    QBrush brush;
    QBrush bgBrush = BobUI::white; // background brush
    QRegion clipRegion;
    QPainterPath clipPath;
    BobUI::ClipOperation clipOperation = BobUI::NoClip;
    QPainter::RenderHints renderHints;
    QList<QPainterClipInfo> clipInfo; // ### Make me smaller and faster to copy around...
    BOBUIransform worldMatrix;       // World transformation matrix, not window and viewport
    BOBUIransform matrix;            // Complete transformation matrix,
    BOBUIransform redirectionMatrix;
    int wx = 0, wy = 0, ww = 0, wh = 0; // window rectangle
    int vx = 0, vy = 0, vw = 0, vh = 0; // viewport rectangle
    qreal opacity = 1;

    uint WxF:1;                 // World transformation
    uint VxF:1;                 // View transformation
    uint clipEnabled:1;

    BobUI::BGMode bgMode = BobUI::TransparentMode;
    QPainter *painter = nullptr;
    BobUI::LayoutDirection layoutDirection;
    QPainter::CompositionMode composition_mode = QPainter::CompositionMode_SourceOver;
    uint emulationSpecifier = 0;
    uint changeFlags = 0;
};

struct QPainterDummyState
{
    QFont font;
    QPen pen;
    QBrush brush;
    BOBUIransform transform;
};

class QRawFont;
class QPainterPrivate
{
    Q_DECLARE_PUBLIC(QPainter)
public:
    explicit QPainterPrivate(QPainter *painter);
    ~QPainterPrivate();

    QPainter *q_ptr;
    // Allocate space for 4 d-pointers (enough for up to 4 sub-sequent
    // redirections within the same paintEvent(), which should be enough
    // in 99% of all cases). E.g: A renders B which renders C which renders D.
    static constexpr qsizetype NDPtrs = 4;
    QVarLengthArray<std::unique_ptr<QPainterPrivate>, NDPtrs> d_ptrs;

    std::unique_ptr<QPainterState> state;
    template <typename T, std::size_t N = 8>
    struct SmallStack : std::stack<T, QVarLengthArray<T, N>> {
        void clear() { this->c.clear(); }
    };
    SmallStack<std::unique_ptr<QPainterState>> savedStates;

    mutable std::unique_ptr<QPainterDummyState> dummyState;

    BOBUIransform invMatrix;
    uint txinv:1;
    uint inDestructor : 1;
    uint refcount = 1;

    enum DrawOperation { StrokeDraw        = 0x1,
                         FillDraw          = 0x2,
                         StrokeAndFillDraw = 0x3
    };

    QPainterDummyState *fakeState() const {
        if (!dummyState)
            dummyState = std::make_unique<QPainterDummyState>();
        return dummyState.get();
    }

    void updateEmulationSpecifier(QPainterState *s);
    void updateStateImpl(QPainterState *state);
    void updateState(QPainterState *state);
    void updateState(std::unique_ptr<QPainterState> &state) { updateState(state.get()); }

    void draw_helper(const QPainterPath &path, DrawOperation operation = StrokeAndFillDraw);
    void drawStretchedGradient(const QPainterPath &path, DrawOperation operation);
    void drawOpaqueBackground(const QPainterPath &path, DrawOperation operation);
    void drawTextItem(const QPointF &p, const BOBUIextItem &_ti, BOBUIextEngine *textEngine);

#if !defined(BOBUI_NO_RAWFONT)
    void drawGlyphs(const QPointF &decorationPosition, const quint32 *glyphArray, QFixedPoint *positionArray, int glyphCount,
                    QFontEngine *fontEngine, bool overline = false, bool underline = false,
                    bool strikeOut = false);
#endif

    void updateMatrix();
    void updateInvMatrix();

    void checkEmulation();

    static QPainterPrivate *get(QPainter *painter)
    {
        return painter->d_ptr.get();
    }

    BOBUIransform viewTransform() const;
    qreal effectiveDevicePixelRatio() const;
    BOBUIransform hidpiScaleTransform() const;
    static bool attachPainterPrivate(QPainter *q, QPaintDevice *pdev);
    void detachPainterPrivate(QPainter *q);
    void initFrom(const QPaintDevice *device);

    QPaintDevice *device = nullptr;
    QPaintDevice *original_device = nullptr;
    QPaintDevice *helper_device = nullptr;

    struct QPaintEngineDestructor {
        void operator()(QPaintEngine *pe) const noexcept
        {
            if (pe && pe->autoDestruct())
                delete pe;
        }
    };
    std::unique_ptr<QPaintEngine, QPaintEngineDestructor> engine;

    std::unique_ptr<QEmulationPaintEngine> emulationEngine;
    QPaintEngineEx *extended = nullptr;
    QBrush colorBrush;          // for fill with solid color

    Q_GUI_EXPORT void setEngineDirtyFlags(QSpan<const QPaintEngine::DirtyFlags>);
};

Q_GUI_EXPORT void bobui_draw_helper(QPainterPrivate *p, const QPainterPath &path, QPainterPrivate::DrawOperation operation);

QString bobui_generate_brush_key(const QBrush &brush);


BOBUI_END_NAMESPACE

#endif // QPAINTER_P_H
