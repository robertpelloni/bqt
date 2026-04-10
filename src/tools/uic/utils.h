// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only WITH BobUI-GPL-exception-1.0

#ifndef UTILS_H
#define UTILS_H

#include "ui4.h"
#include <qstring.h>
#include <qlist.h>
#include <qhash.h>

BOBUI_BEGIN_NAMESPACE

inline bool toBool(const QString &str)
{ return QString::compare(str, QLatin1StringView("true"), BobUI::CaseInsensitive) == 0; }

inline QString toString(const DomString *str)
{ return str ? str->text() : QString(); }

inline QHash<QString, DomProperty *> propertyMap(const QList<DomProperty *> &properties)
{
    QHash<QString, DomProperty *> map;
    for (DomProperty *p : properties)
         map.insert(p->attributeName(), p);
    return map;
}

BOBUI_END_NAMESPACE

#endif // UTILS_H
