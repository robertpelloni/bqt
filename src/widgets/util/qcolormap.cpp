// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#include "qcolormap.h"
#include "qcolor.h"
#include "qpaintdevice.h"
#include "qscreen.h"
#include "qguiapplication.h"

#include <BobUICore/qmutex.h>

BOBUI_BEGIN_NAMESPACE

#if BOBUI_REMOVAL_BOBUI7_DEPRECATED_SINCE(6, 11)

BOBUI_WARNING_PUSH
BOBUI_WARNING_DISABLE_DEPRECATED

class QColormapPrivate
{
public:
    inline QColormapPrivate()
        : ref(1), mode(QColormap::Direct), depth(0), numcolors(0)
    { }

    QAtomicInt ref;

    QColormap::Mode mode;
    int depth;
    int numcolors;
};

static QColormapPrivate *screenMap = nullptr;

void QColormap::initialize()
{
    screenMap = new QColormapPrivate;
    if (Q_UNLIKELY(!QGuiApplication::primaryScreen())) {
        qWarning("no screens available, assuming 24-bit color");
        screenMap->depth = 24;
        screenMap->mode = QColormap::Direct;
        return;
    }
    screenMap->depth = QGuiApplication::primaryScreen()->depth();
    if (screenMap->depth < 8) {
        screenMap->mode = QColormap::Indexed;
        screenMap->numcolors = 256;
    } else {
        screenMap->mode = QColormap::Direct;
        screenMap->numcolors = -1;
    }

    qAddPostRoutine(QColormap::cleanup);
}

void QColormap::cleanup()
{
    if (screenMap) {
        if (!screenMap->ref.deref())
            delete screenMap;
        screenMap = nullptr;
    }
}

QColormap QColormap::instance(int /*screen*/)
{
    static QMutex mutex;
    QMutexLocker locker(&mutex);
    if (!screenMap)
        initialize();

    return QColormap();
}

QColormap::QColormap()
    : d(screenMap)
{ d->ref.ref(); }

QColormap::QColormap(const QColormap &colormap)
    :d (colormap.d)
{ d->ref.ref(); }

QColormap::~QColormap()
{
    if (!d->ref.deref())
        delete d;
}

QColormap::Mode QColormap::mode() const
{ return d->mode; }


int QColormap::depth() const
{ return d->depth; }


int QColormap::size() const
{
    return d->numcolors;
}

#ifndef BOBUI_QWS_DEPTH16_RGB
#define BOBUI_QWS_DEPTH16_RGB 565
#endif
static const int bobui_rbits = (BOBUI_QWS_DEPTH16_RGB/100);
static const int bobui_gbits = (BOBUI_QWS_DEPTH16_RGB/10%10);
static const int bobui_bbits = (BOBUI_QWS_DEPTH16_RGB%10);
static const int bobui_red_shift = bobui_bbits+bobui_gbits-(8-bobui_rbits);
static const int bobui_green_shift = bobui_bbits-(8-bobui_gbits);
static const int bobui_neg_blue_shift = 8-bobui_bbits;
static const int bobui_blue_mask = (1<<bobui_bbits)-1;
static const int bobui_green_mask = (1<<(bobui_gbits+bobui_bbits))-(1<<bobui_bbits);
static const int bobui_red_mask = (1<<(bobui_rbits+bobui_gbits+bobui_bbits))-(1<<(bobui_gbits+bobui_bbits));

static const int bobui_red_rounding_shift = bobui_red_shift + bobui_rbits;
static const int bobui_green_rounding_shift = bobui_green_shift + bobui_gbits;
static const int bobui_blue_rounding_shift = bobui_bbits - bobui_neg_blue_shift;

inline ushort bobui_convRgbTo16(QRgb c)
{
    const int tr = qRed(c) << bobui_red_shift;
    const int tg = qGreen(c) << bobui_green_shift;
    const int tb = qBlue(c) >> bobui_neg_blue_shift;

    return (tb & bobui_blue_mask) | (tg & bobui_green_mask) | (tr & bobui_red_mask);
}

inline QRgb bobui_conv16ToRgb(ushort c)
{
    const int r=(c & bobui_red_mask);
    const int g=(c & bobui_green_mask);
    const int b=(c & bobui_blue_mask);
    const int tr = r >> bobui_red_shift | r >> bobui_red_rounding_shift;
    const int tg = g >> bobui_green_shift | g >> bobui_green_rounding_shift;
    const int tb = b << bobui_neg_blue_shift | b >> bobui_blue_rounding_shift;

    return qRgb(tr,tg,tb);
}

uint QColormap::pixel(const QColor &color) const
{
    QRgb rgb = color.rgba();
    if (d->mode == QColormap::Direct) {
        switch(d->depth) {
        case 16:
            return bobui_convRgbTo16(rgb);
        case 24:
        case 32:
        {
            const int r = qRed(rgb);
            const int g = qGreen(rgb);
            const int b = qBlue(rgb);
            const int red_shift = 16;
            const int green_shift = 8;
            const int red_mask   = 0xff0000;
            const int green_mask = 0x00ff00;
            const int blue_mask  = 0x0000ff;
            const int tg = g << green_shift;
            const int tr = r << red_shift;
            return 0xff000000 | (b & blue_mask) | (tg & green_mask) | (tr & red_mask);
        }
        }
    }
    //XXX
    //return bobui_screen->alloc(qRed(rgb), qGreen(rgb), qBlue(rgb));
    return 0;
}

const QColor QColormap::colorAt(uint pixel) const
{
    if (d->mode == Direct) {
        if (d->depth == 16) {
            pixel = bobui_conv16ToRgb(pixel);
        }
        const int red_shift = 16;
        const int green_shift = 8;
        const int red_mask   = 0xff0000;
        const int green_mask = 0x00ff00;
        const int blue_mask  = 0x0000ff;
        return QColor((pixel & red_mask) >> red_shift,
                      (pixel & green_mask) >> green_shift,
                      (pixel & blue_mask));
    }
#if 0 // XXX
    Q_ASSERT_X(int(pixel) < bobui_screen->numCols(), "QColormap::colorAt", "pixel out of bounds of palette");
    return QColor(bobui_screen->clut()[pixel]);
#endif
    return QColor();
}

const QList<QColor> QColormap::colormap() const
{
    return QList<QColor>();
}

QColormap &QColormap::operator=(const QColormap &colormap)
{ qAtomicAssign(d, colormap.d); return *this; }

BOBUI_WARNING_POP

#endif // BOBUI_REMOVAL_BOBUI7_DEPRECATED_SINCE(6, 11)

BOBUI_END_NAMESPACE
