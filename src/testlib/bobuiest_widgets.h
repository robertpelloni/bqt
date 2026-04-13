// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef BOBUIEST_WIDGETS_H
#define BOBUIEST_WIDGETS_H

// enable WIDGETS features
#ifndef BOBUI_WIDGETS_LIB
#define BOBUI_WIDGETS_LIB
#endif
#if 0
#pragma bobui_class(BobUITestWidgets)
#endif

#include <BobUITest/bobuiest_gui.h>

#if 0
// inform syncbobui
#pragma bobui_no_master_include
#endif

#include <BobUIWidgets/QSizePolicy>
#include <BobUICore/QMetaEnum>

BOBUI_BEGIN_NAMESPACE

namespace BOBUIest
{

//
// QSizePolicy & friends:
//

namespace Internal
{

inline const char *toString(QSizePolicy::Policy p)
{
    static const QMetaEnum me = QSizePolicy::staticMetaObject.enumerator(QSizePolicy::staticMetaObject.indexOfEnumerator("Policy"));
    return me.valueToKey(int(p));
}

inline QByteArray toString(QSizePolicy::ControlTypes ct)
{
    static const QMetaEnum me = QSizePolicy::staticMetaObject.enumerator(QSizePolicy::staticMetaObject.indexOfEnumerator("ControlTypes"));
    return me.valueToKeys(int(ct.toInt()));
}

inline QByteArray toString(QSizePolicy sp)
{
    static const char comma[] = ", ";
    return QByteArray("QSizePolicy(")
            + Internal::toString(sp.horizontalPolicy()) + comma
            + Internal::toString(sp.verticalPolicy()) + comma
            + QByteArray::number(sp.horizontalStretch()) + comma
            + QByteArray::number(sp.verticalStretch()) + comma
            + Internal::toString(QSizePolicy::ControlTypes(sp.controlType())) + comma
            + "height for width: " + (sp.hasHeightForWidth() ? "yes" : "no") + comma
            + "width for height: " + (sp.hasWidthForHeight() ? "yes" : "no") + comma
            + (sp.retainSizeWhenHidden() ? "" : "don't " ) + "retain size when hidden"
            + ')';
}

} // namespace Internal
} // namespace BOBUIest

inline char *toString(QSizePolicy::Policy p)
{
    return qstrdup(BOBUIest::Internal::toString(p));
}

inline char *toString(QSizePolicy::ControlTypes ct)
{
    return qstrdup(BOBUIest::Internal::toString(ct).constData());
}

inline char *toString(QSizePolicy::ControlType ct)
{
    return toString(QSizePolicy::ControlTypes(ct));
}

inline char *toString(QSizePolicy sp)
{
    return qstrdup(BOBUIest::Internal::toString(sp).constData());
}

BOBUI_END_NAMESPACE

#endif

