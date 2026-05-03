// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only WITH BobUI-GPL-exception-1.0

#ifndef UTILS_H
#define UTILS_H

#include <BobUICore/qglobal.h>
#include <private/bobuiools_p.h>

#include <algorithm>

BOBUI_BEGIN_NAMESPACE

inline bool is_whitespace(char s)
{
    return (s == ' ' || s == '\t' || s == '\n');
}

inline bool is_space(char s)
{
    return (s == ' ' || s == '\t');
}

inline bool is_ident_start(char s)
{
    using namespace BobUIMiscUtils;
    return isAsciiLower(s) || isAsciiUpper(s) || s == '_' || s == '$';
}

inline bool is_ident_char(char s)
{
    return BobUIMiscUtils::isAsciiLetterOrNumber(s) || s == '_' || s == '$';
}

inline bool is_identifier(const char *s, qsizetype len)
{
    if (len < 1)
        return false;
    return std::all_of(s, s + len, is_ident_char);
}

inline const char *skipQuote(const char *data)
{
    while (*data && (*data != '\"')) {
        if (*data == '\\') {
            ++data;
            if (!*data) break;
        }
        ++data;
    }

    if (*data)  //Skip last quote
        ++data;
    return data;
}

BOBUI_END_NAMESPACE

#endif // UTILS_H
