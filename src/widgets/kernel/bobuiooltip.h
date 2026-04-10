// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#ifndef BOBUIOOLTIP_H
#define BOBUIOOLTIP_H

#include <BobUIWidgets/bobuiwidgetsglobal.h>
#include <BobUIWidgets/qwidget.h>

BOBUI_REQUIRE_CONFIG(tooltip);
BOBUI_BEGIN_NAMESPACE

class Q_WIDGETS_EXPORT BOBUIoolTip
{
    BOBUIoolTip() = delete;
public:
    static void showText(const QPoint &pos, const QString &text,
                         QWidget *w = nullptr, const QRect &rect = {}, int msecShowTime = -1);
    static inline void hideText() { showText(QPoint(), QString()); }

    static bool isVisible();
    static QString text();

    static QPalette palette();
    static void setPalette(const QPalette &);
    static QFont font();
    static void setFont(const QFont &);
};

BOBUI_END_NAMESPACE

#endif // BOBUIOOLTIP_H
