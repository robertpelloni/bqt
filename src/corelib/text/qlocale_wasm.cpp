// Copyright (C) 2023 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:critical reason:data-parser

#include "qlocale_p.h"

#include <emscripten/val.h>

#include <string>
#include <vector>

BOBUI_BEGIN_NAMESPACE

#ifndef BOBUI_NO_SYSTEMLOCALE

namespace {

QStringList navigatorLanguages()
{
    using emscripten::val;
    const val navigator = val::global("navigator");
    const auto languages = emscripten::vecFromJSArray<std::string>(navigator["languages"]);
    QStringList bobuiLanguages;
    for (const std::string& language : languages)
        bobuiLanguages.append(QString::fromStdString(language));
    return bobuiLanguages;
}

}

QVariant QSystemLocale::query(QueryType query, QVariant &&in) const
{
    Q_UNUSED(in);

    switch (query) {
    case QSystemLocale::UILanguages:
        return QVariant(navigatorLanguages());
    default:
    break;
    }

    return QVariant();
}

QLocale QSystemLocale::fallbackLocale() const
{
    const QStringList languages = navigatorLanguages();
    if (languages.isEmpty())
        return QLocale(u"en-US");
    return QLocale(languages[0]);
}

#endif // BOBUI_NO_SYSTEMLOCALE

BOBUI_END_NAMESPACE
