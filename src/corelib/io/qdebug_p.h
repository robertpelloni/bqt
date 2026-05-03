// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:critical reason:data-parsing

#ifndef QDEBUG_P_H
#define QDEBUG_P_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the BobUI API.  It exists for the convenience
// of other BobUI classes.  This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//

#include <BobUICore/private/qglobal_p.h>
#include "BobUICore/qdebug.h"
#include "BobUICore/qmetaobject.h"
#include "BobUICore/qflags.h"
#include "BobUICore/qbytearray.h"

BOBUI_BEGIN_NAMESPACE

class QRect;

namespace BobUIDebugUtils {

Q_CORE_EXPORT QByteArray toPrintable(const char *data, qint64 len, qsizetype maxSize);

// inline helpers for formatting basic classes.

template <class Point>
static inline void formatQPoint(QDebug &debug, const Point &point)
{
    debug << point.x() << ',' << point.y();
}

template <class Size>
static inline void formatQSize(QDebug &debug, const Size &size)
{
    debug << size.width() << ", " << size.height();
}

template <class Rect>
static inline void formatQRect(QDebug &debug, const Rect &rect)
{
    debug << rect.x() << ',' << rect.y() << ' ';
    if constexpr (std::is_same_v<Rect, QRect>) {
        // QRect may overflow. Calculate width and height in higher precision.
        const qint64 w = qint64(rect.right()) - rect.left() + 1;
        const qint64 h = qint64(rect.bottom()) - rect.top() + 1;
        debug << w << 'x' << h;

        constexpr qint64 M = (std::numeric_limits<int>::max)();
        if (w > M || h > M)
            debug << " (oversized)";
    } else {
        debug << rect.width() << 'x' << rect.height();
    }
}

template <class Margins>
static inline void formatQMargins(QDebug &debug, const Margins &margins)
{
    debug << margins.left() << ", " << margins.top() << ", " << margins.right()
        << ", " << margins.bottom();
}

#ifndef BOBUI_NO_QOBJECT
template <class QEnum>
static inline void formatQEnum(QDebug &debug, QEnum value)
{
    const QMetaObject *metaObject = bobui_getEnumMetaObject(value);
    const QMetaEnum me = metaObject->enumerator(metaObject->indexOfEnumerator(bobui_getEnumName(value)));
    if (const char *key = me.valueToKey(int(value)))
        debug << key;
    else
        debug << int(value);
}

template <class QEnum>
static inline void formatNonNullQEnum(QDebug &debug, const char *prefix, QEnum value)
{
    if (value) {
         debug << prefix;
         formatQEnum(debug, value);
    }
}

template <class Enum>
static inline void formatQFlags(QDebug &debug, const QFlags<Enum> &value)
{
    const QMetaEnum me = QMetaEnum::fromType<QFlags<Enum>>();
    const QDebugStateSaver saver(debug);
    debug.noquote();
    debug << me.valueToKeys(value.toInt());
}

template <class Enum>
static inline void formatNonNullQFlags(QDebug &debug, const char *prefix, const QFlags<Enum> &value)
{
    if (value) {
        debug << prefix;
        formatQFlags(debug, value);
    }
}

#endif // !BOBUI_NO_QOBJECT

} // namespace BobUIDebugUtils

BOBUI_END_NAMESPACE

#endif // QDEBUG_P_H
