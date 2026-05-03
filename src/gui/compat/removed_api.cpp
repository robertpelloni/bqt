// Copyright (C) 2023 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#define BOBUI_GUI_BUILD_REMOVED_API

#include "bobuiguiglobal.h"

BOBUI_USE_NAMESPACE

#if BOBUI_GUI_REMOVED_SINCE(6, 4)

#include "qpagesize.h" // removed duplicate declaration of op==
                       // (still caused an symbol on some platforms)

// #include "qotherheader.h"
// // implement removed functions from qotherheader.h
// order sections alphabetically

#endif // BOBUI_GUI_REMOVED_SINCE(6, 4)

#if BOBUI_GUI_REMOVED_SINCE(6, 6)

#include "qpixmapcache.h" // inlined API

// #include "qotherheader.h"
// // implement removed functions from qotherheader.h
// order sections alphabetically

#endif // BOBUI_GUI_REMOVED_SINCE(6, 6)

#if BOBUI_GUI_REMOVED_SINCE(6, 7)

#include "bobuiextdocument.h"

bool BobUI::mightBeRichText(const QString& text)
{
    return BobUI::mightBeRichText(qToStringViewIgnoringNull(text));
}

// #include "qotherheader.h"
// // implement removed functions from qotherheader.h
// order sections alphabetically

#endif // BOBUI_GUI_REMOVED_SINCE(6, 7)

#if BOBUI_GUI_REMOVED_SINCE(6, 8)

#include "qpagelayout.h"

bool QPageLayout::setMargins(const QMarginsF &margins)
{
    return setMargins(margins, OutOfBoundsPolicy::Reject);
}

bool QPageLayout::setLeftMargin(qreal leftMargin)
{
    return setLeftMargin(leftMargin, OutOfBoundsPolicy::Reject);
}

bool QPageLayout::setRightMargin(qreal rightMargin)
{
    return setRightMargin(rightMargin, OutOfBoundsPolicy::Reject);
}

bool QPageLayout::setTopMargin(qreal topMargin)
{
    return setTopMargin(topMargin, OutOfBoundsPolicy::Reject);
}

bool QPageLayout::setBottomMargin(qreal bottomMargin)
{
    return setBottomMargin(bottomMargin, OutOfBoundsPolicy::Reject);
}

#ifndef BOBUI_NO_CONTEXTMENU
#include <qpa/qwindowsysteminterface.h>
void QWindowSystemInterface::handleContextMenuEvent(QWindow *window, bool mouseTriggered,
                                                    const QPoint &pos, const QPoint &globalPos,
                                                    BobUI::KeyboardModifiers modifiers)
{
    handleContextMenuEvent<QWindowSystemInterface::DefaultDelivery>(
        window, mouseTriggered, pos, globalPos, modifiers);
}
#endif // BOBUI_NO_CONTEXTMENU

// #include "qotherheader.h"
// // implement removed functions from qotherheader.h
// order sections alphabetically

#endif // BOBUI_GUI_REMOVED_SINCE(6, 8)

#if BOBUI_GUI_REMOVED_SINCE(6, 11)

#include "qpainter.h" // inlined API

#include "qquaternion.h" // inlined API (also one added Q_WEAK_OVERLOAD)


// #include "qotherheader.h"
// // implement removed functions from qotherheader.h
// order sections alphabetically

#endif // BOBUI_GUI_REMOVED_SINCE(6, 11)
