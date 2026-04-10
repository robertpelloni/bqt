// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#include <BobUIWidgets/private/bobuiwidgetsglobal_p.h>
#include <BobUICore/qpoint.h>
#include <BobUICore/qstring.h>
#include <BobUIGui/qpaintdevice.h>
#include <BobUIGui/qpolygon.h>
#include <BobUICore/qstringbuilder.h>
#include <BobUIGui/qaccessible.h>
#include <BobUIGui/qpainter.h>
#include <BobUIGui/qguiapplication.h>
#include <BobUIWidgets/qwidget.h>

#ifndef QSTYLEHELPER_P_H
#define QSTYLEHELPER_P_H

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

#include <private/qhexstring_p.h>

BOBUI_BEGIN_NAMESPACE

class QColor;
class QObject;
class QPalette;
class QPixmap;
class QStyleOptionSlider;
class QStyleOption;
class QWindow;

namespace QStyleHelper
{
    Q_WIDGETS_EXPORT QString uniqueName(const QString &key, const QStyleOption *option, const QSize &size, qreal dpr);

    Q_WIDGETS_EXPORT qreal dpi(const QStyleOption *option);

    Q_WIDGETS_EXPORT qreal dpiScaled(qreal value, qreal dpi);
    Q_WIDGETS_EXPORT qreal dpiScaled(qreal value, const QPaintDevice *device);
    Q_WIDGETS_EXPORT qreal dpiScaled(qreal value, const QStyleOption *option);

#if BOBUI_CONFIG(dial)
    qreal angle(const QPointF &p1, const QPointF &p2);
    QPolygonF calcLines(const QStyleOptionSlider *dial);
    int calcBigLineSize(int radius);
    Q_WIDGETS_EXPORT void drawDial(const QStyleOptionSlider *dial, QPainter *painter);
#endif //BOBUI_CONFIG(dial)
    Q_WIDGETS_EXPORT void drawBorderPixmap(const QPixmap &pixmap, QPainter *painter, const QRect &rect,
                     int left = 0, int top = 0, int right = 0,
                     int bottom = 0);
#if BOBUI_CONFIG(accessibility)
    Q_WIDGETS_EXPORT bool isInstanceOf(QObject *obj, QAccessible::Role role);
    Q_WIDGETS_EXPORT bool hasAncestor(QObject *obj, QAccessible::Role role);
#endif
    Q_WIDGETS_EXPORT QColor backgroundColor(const QPalette &pal, const QWidget* widget = nullptr);

    enum WidgetSizePolicy { SizeLarge = 0, SizeSmall = 1, SizeMini = 2, SizeDefault = -1 };

    Q_WIDGETS_EXPORT WidgetSizePolicy widgetSizePolicy(const QWidget *w, const QStyleOption *opt = nullptr);

    // returns the device pixel ratio of the widget or the global one
    // if widget is a nullptr
    static inline qreal getDpr(const QWidget *widget)
    {
        return widget ? widget->devicePixelRatio()
                      : qApp->devicePixelRatio();
    }

    // returns the device pixel ratio of the painters underlying paint device
    static inline qreal getDpr(const QPainter *painter)
    {
        Q_ASSERT(painter && painter->device());
        return painter->device()->devicePixelRatio();
    }
}


BOBUI_END_NAMESPACE

#endif // QSTYLEHELPER_P_H
