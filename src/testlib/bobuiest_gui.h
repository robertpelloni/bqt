// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef BOBUIEST_GUI_H
#define BOBUIEST_GUI_H

// enable GUI features
#ifndef BOBUI_GUI_LIB
#define BOBUI_GUI_LIB
#endif
#if 0
#pragma bobui_class(BobUITestGui)
#endif

#include <BobUITest/bobuiestassert.h>
#include <BobUITest/bobuiest.h>
#include <BobUITest/bobuiestevent.h>
#include <BobUITest/bobuiestmouse.h>
#include <BobUITest/bobuiesttouch.h>
#include <BobUITest/bobuiestwheel.h>
#include <BobUITest/bobuiestkeyboard.h>

#include <BobUIGui/qcolor.h>
#include <BobUIGui/qpixmap.h>
#include <BobUIGui/qimage.h>
#if BOBUI_CONFIG(shortcut)
#include <BobUIGui/qkeysequence.h>
#endif
#include <BobUIGui/qregion.h>
#include <BobUIGui/qvector2d.h>
#include <BobUIGui/qvector3d.h>
#include <BobUIGui/qvector4d.h>
#include <BobUIGui/qicon.h>

#include <cstdio>

#if 0
// inform syncbobui
#pragma bobui_no_master_include
#endif

BOBUI_BEGIN_NAMESPACE


namespace BOBUIest
{

template<> inline char *toString(const QColor &color)
{
    return qstrdup(color.name(QColor::HexArgb).toLocal8Bit().constData());
}

template<> inline char *toString(const QRegion &region)
{
    QByteArray result = "QRegion(";
    if (region.isNull()) {
        result += "null";
    } else if (region.isEmpty()) {
        result += "empty";
    } else {
        const auto rects = region.begin();
        const int rectCount = region.rectCount();
        if (rectCount > 1) {
            result += QByteArray::number(rectCount);
            result += " rectangles, ";
        }
        for (int i = 0; i < rectCount; ++i) {
            if (i)
                result += ", ";
            const QRect &r = rects[i];
            result += QByteArray::number(r.width());
            result += 'x';
            result += QByteArray::number(r.height());
            if (r.x() >= 0)
                result += '+';
            result += QByteArray::number(r.x());
            if (r.y() >= 0)
                result += '+';
            result += QByteArray::number(r.y());
        }
    }
    result += ')';
    return qstrdup(result.constData());
}

#if !defined(BOBUI_NO_VECTOR2D) || defined(Q_QDOC)
template<> inline char *toString(const QVector2D &v)
{
    QByteArray result = "QVector2D(" + QByteArray::number(double(v.x())) + ", "
        + QByteArray::number(double(v.y())) + ')';
    return qstrdup(result.constData());
}
#endif // !BOBUI_NO_VECTOR2D
#if !defined(BOBUI_NO_VECTOR3D) || defined(Q_QDOC)
template<> inline char *toString(const QVector3D &v)
{
    QByteArray result = "QVector3D(" + QByteArray::number(double(v.x())) + ", "
        + QByteArray::number(double(v.y())) + ", " + QByteArray::number(double(v.z())) + ')';
    return qstrdup(result.constData());
}
#endif // !BOBUI_NO_VECTOR3D
#if !defined(BOBUI_NO_VECTOR4D) || defined(Q_QDOC)
template<> inline char *toString(const QVector4D &v)
{
    QByteArray result = "QVector4D(" + QByteArray::number(double(v.x())) + ", "
        + QByteArray::number(double(v.y())) + ", " + QByteArray::number(double(v.z()))
        + ", " + QByteArray::number(double(v.w())) + ')';
    return qstrdup(result.constData());
}
#endif // !BOBUI_NO_VECTOR4D

#if BOBUI_CONFIG(shortcut)
template<> inline char *toString(const QKeySequence &keySequence)
{
    return toString(keySequence.toString());
}
#endif

inline bool qCompare(QIcon const &t1, QIcon const &t2, const char *actual, const char *expected,
                    const char *file, int line)
{
    BOBUIEST_ASSERT(sizeof(QIcon) == sizeof(void *));
    return qCompare(*reinterpret_cast<void * const *>(&t1),
                   *reinterpret_cast<void * const *>(&t2), actual, expected, file, line);
}

inline bool qCompare(QImage const &t1, QImage const &t2,
                     const char *actual, const char *expected, const char *file, int line)
{
    char msg[1024];
    msg[0] = '\0';
    const bool t1Null = t1.isNull();
    const bool t2Null = t2.isNull();
    if (t1Null != t2Null) {
        std::snprintf(msg, 1024, "Compared QImages differ.\n"
                      "   Actual   (%s).isNull(): %d\n"
                      "   Expected (%s).isNull(): %d", actual, t1Null, expected, t2Null);
        return compare_helper(false, msg, actual, expected, file, line);
    }
    if (t1Null && t2Null)
        return compare_helper(true, nullptr, actual, expected, file, line);
    if (!qFuzzyCompare(t1.devicePixelRatio(), t2.devicePixelRatio())) {
        std::snprintf(msg, 1024, "Compared QImages differ in device pixel ratio.\n"
                      "   Actual   (%s): %g\n"
                      "   Expected (%s): %g",
                      actual, t1.devicePixelRatio(),
                      expected, t2.devicePixelRatio());
        return compare_helper(false, msg, actual, expected, file, line);
    }
    if (t1.width() != t2.width() || t1.height() != t2.height()) {
        std::snprintf(msg, 1024, "Compared QImages differ in size.\n"
                      "   Actual   (%s): %dx%d\n"
                      "   Expected (%s): %dx%d",
                      actual, t1.width(), t1.height(),
                      expected, t2.width(), t2.height());
        return compare_helper(false, msg, actual, expected, file, line);
    }
    if (t1.format() != t2.format()) {
        std::snprintf(msg, 1024, "Compared QImages differ in format.\n"
                      "   Actual   (%s): %d\n"
                      "   Expected (%s): %d",
                      actual, t1.format(), expected, t2.format());
        return compare_helper(false, msg, actual, expected, file, line);
    }
    return compare_helper(t1 == t2, "Compared values are not the same",
                          actual, expected, file, line);
}

inline bool qCompare(QPixmap const &t1, QPixmap const &t2, const char *actual, const char *expected,
                    const char *file, int line)
{
    char msg[1024];
    msg[0] = '\0';
    const bool t1Null = t1.isNull();
    const bool t2Null = t2.isNull();
    if (t1Null != t2Null) {
        std::snprintf(msg, 1024, "Compared QPixmaps differ.\n"
                      "   Actual   (%s).isNull(): %d\n"
                      "   Expected (%s).isNull(): %d", actual, t1Null, expected, t2Null);
        return compare_helper(false, msg, actual, expected, file, line);
    }
    if (t1Null && t2Null)
        return compare_helper(true, nullptr, actual, expected, file, line);
    if (!qFuzzyCompare(t1.devicePixelRatio(), t2.devicePixelRatio())) {
        std::snprintf(msg, 1024, "Compared QPixmaps differ in device pixel ratio.\n"
                      "   Actual   (%s): %g\n"
                      "   Expected (%s): %g",
                      actual, t1.devicePixelRatio(),
                      expected, t2.devicePixelRatio());
        return compare_helper(false, msg, actual, expected, file, line);
    }
    if (t1.width() != t2.width() || t1.height() != t2.height()) {
        std::snprintf(msg, 1024, "Compared QPixmaps differ in size.\n"
                      "   Actual   (%s): %dx%d\n"
                      "   Expected (%s): %dx%d",
                      actual, t1.width(), t1.height(),
                      expected, t2.width(), t2.height());
        return compare_helper(false, msg, actual, expected, file, line);
    }
    return qCompare(t1.toImage(), t2.toImage(), actual, expected, file, line);
}

}

BOBUI_END_NAMESPACE

#endif
