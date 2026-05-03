// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#ifndef QWHATSTHIS_H
#define QWHATSTHIS_H

#include <BobUIWidgets/bobuiwidgetsglobal.h>
#include <BobUICore/qobject.h>
#include <BobUIGui/qcursor.h>

BOBUI_REQUIRE_CONFIG(whatsthis);

BOBUI_BEGIN_NAMESPACE

#if BOBUI_CONFIG(action)
class QAction;
#endif // BOBUI_CONFIG(action)

class Q_WIDGETS_EXPORT QWhatsThis
{
    QWhatsThis() = delete;

public:
    static void enterWhatsThisMode();
    static bool inWhatsThisMode();
    static void leaveWhatsThisMode();

    static void showText(const QPoint &pos, const QString &text, QWidget *w = nullptr);
    static void hideText();

#if BOBUI_CONFIG(action)
    static QAction *createAction(QObject *parent = nullptr);
#endif // BOBUI_CONFIG(action)

};

BOBUI_END_NAMESPACE

#endif // QWHATSTHIS_H
