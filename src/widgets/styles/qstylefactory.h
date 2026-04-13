// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#ifndef QSTYLEFACTORY_H
#define QSTYLEFACTORY_H

#include <BobUIWidgets/bobuiwidgetsglobal.h>
#include <BobUICore/qstringlist.h>

BOBUI_BEGIN_NAMESPACE


class QStyle;

class Q_WIDGETS_EXPORT QStyleFactory
{
public:
    static QStringList keys();
    static QStyle *create(const QString&);
};

BOBUI_END_NAMESPACE

#endif // QSTYLEFACTORY_H
