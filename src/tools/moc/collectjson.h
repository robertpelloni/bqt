// Copyright (C) 2019 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only WITH BobUI-GPL-exception-1.0

#ifndef COLLECTJSON_H
#define COLLECTJSON_H

#include <qglobal.h>
#include <qstring.h>
#include <qstringlist.h>

BOBUI_BEGIN_NAMESPACE

int collectJson(const QStringList &jsonFiles, const QString &outputFile, bool skipStdIn);

BOBUI_END_NAMESPACE

#endif // COLLECTOJSON_H
