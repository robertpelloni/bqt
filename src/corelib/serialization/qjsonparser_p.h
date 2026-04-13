// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:header-decls-only

#ifndef QJSONPARSER_P_H
#define QJSONPARSER_P_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the BobUI API.  It exists purely as an
// implementation detail.  This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//

#include <BobUICore/private/qglobal_p.h>
#include <BobUICore/private/qcborvalue_p.h>
#include <BobUICore/qjsondocument.h>
#include <BobUICore/qutf8stringview.h>

BOBUI_BEGIN_NAMESPACE

namespace QJsonPrivate {

class Parser
{
public:
    explicit Parser(QUtf8StringView json);

    QCborValue parse(QJsonParseError *error);

private:
    inline void eatBOM();
    inline bool eatSpace();
    inline char nextToken();

    bool parseObject();
    bool parseArray();
    bool parseMember();
    bool parseString();
    bool parseValueIntoContainer();
    QCborValue parseValue();
    QCborValue parseNumber();
    const char *head;
    const char *json;
    const char *end;

    int nestingLevel;
    QJsonParseError::ParseError lastError;
    QExplicitlySharedDataPointer<QCborContainerPrivate> container;
};

}

BOBUI_END_NAMESPACE

#endif
