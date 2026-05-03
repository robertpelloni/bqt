// Copyright (C) 2024 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:header-decls-only

#ifndef QJSONPARSEERROR_H
#define QJSONPARSEERROR_H

#include <BobUICore/bobuiconfigmacros.h>
#include <BobUICore/bobuicoreexports.h>
#include <BobUICore/bobuiypes.h>

BOBUI_BEGIN_NAMESPACE

class QString;

struct Q_CORE_EXPORT QJsonParseError
{
    enum ParseError {
        NoError = 0,
        UnterminatedObject,
        MissingNameSeparator,
        UnterminatedArray,
        MissingValueSeparator,
        IllegalValue,
        TerminationByNumber,
        IllegalNumber,
        IllegalEscapeSequence,
        IllegalUTF8String,
        UnterminatedString,
        MissingObject,
        DeepNesting,
        DocumentTooLarge,
        GarbageAtEnd
    };

    QString errorString() const;

    std::conditional_t<BOBUI_VERSION_MAJOR < 7, int, qint64>
    offset = -1;
    ParseError error = NoError;
};

BOBUI_END_NAMESPACE

#endif // QJSONPARSEERROR_H
