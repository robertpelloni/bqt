// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#ifndef QWINDOWSSTYLE_P_P_H
#define QWINDOWSSTYLE_P_P_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the BobUI API.  It exists for the convenience
// of qapplication_*.cpp, qwidget*.cpp and qfiledialog.cpp.  This header
// file may change from version to version without notice, or even be removed.
//
// We mean it.
//

#include <BobUIWidgets/private/bobuiwidgetsglobal_p.h>
#include "qwindowsstyle_p.h"
#include "qcommonstyle_p.h"

#if BOBUI_CONFIG(style_windows)
#include <qlist.h>

BOBUI_BEGIN_NAMESPACE

class Q_WIDGETS_EXPORT QWindowsStylePrivate : public QCommonStylePrivate
{
    Q_DECLARE_PUBLIC(QWindowsStyle)
public:
    enum { InvalidMetric = -23576 };

    QWindowsStylePrivate();
    static int pixelMetricFromSystemDp(QStyle::PixelMetric pm, const QStyleOption *option = nullptr, const QWidget *widget = nullptr);
    static int fixedPixelMetric(QStyle::PixelMetric pm);
    static qreal nativeMetricScaleFactor(const QWidget *widget = nullptr);

    bool hasSeenAlt(const QWidget *widget) const;
    bool altDown() const { return alt_down; }
    bool alt_down = false;
    QList<const QWidget *> seenAlt;

    QColor inactiveCaptionText;
    QColor activeCaptionColor;
    QColor activeGradientCaptionColor;
    QColor inactiveCaptionColor;
    QColor inactiveGradientCaptionColor;

    enum {
        windowsItemFrame        =  2, // menu item frame width
        windowsSepHeight        =  9, // separator item height
        windowsItemHMargin      =  3, // menu item hor text margin
        windowsItemVMargin      =  2, // menu item ver text margin
        windowsArrowHMargin     =  6, // arrow horizontal margin
        windowsRightBorder      = 15, // right border on windows
        windowsCheckMarkWidth   = 12  // checkmarks width on windows
    };
};

BOBUI_END_NAMESPACE

#endif // style_windows

#endif //QWINDOWSSTYLE_P_P_H
;
