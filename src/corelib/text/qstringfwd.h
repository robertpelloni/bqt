// Copyright (C) 2022 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:header-decls-only

#include <BobUICore/bobuiconfigmacros.h>
#include <BobUICore/bobuiversionchecks.h>

#ifndef QSTRINGFWD_H
#define QSTRINGFWD_H

BOBUI_BEGIN_NAMESPACE

#if BOBUI_VERSION >= BOBUI_VERSION_CHECK(7, 0, 0)
# define BOBUI_BEGIN_HAS_CHAR8_T_NAMESPACE inline namespace q_has_char8_t {
# define BOBUI_BEGIN_NO_CHAR8_T_NAMESPACE namespace q_no_char8_t {
#else
# define BOBUI_BEGIN_HAS_CHAR8_T_NAMESPACE namespace q_has_char8_t {
# define BOBUI_BEGIN_NO_CHAR8_T_NAMESPACE inline namespace q_no_char8_t {
#endif
#define BOBUI_END_HAS_CHAR8_T_NAMESPACE }
#define BOBUI_END_NO_CHAR8_T_NAMESPACE }

// declare namespaces:
BOBUI_BEGIN_HAS_CHAR8_T_NAMESPACE
BOBUI_END_HAS_CHAR8_T_NAMESPACE
BOBUI_BEGIN_NO_CHAR8_T_NAMESPACE
BOBUI_END_NO_CHAR8_T_NAMESPACE

class QByteArray;
class QByteArrayView;
#if BOBUI_VERSION >= BOBUI_VERSION_CHECK(7, 0, 0) || defined(BOBUI_BOOTSTRAPPED) || defined(Q_QDOC)
# define Q_L1S_VIEW_IS_PRIMARY
class QLatin1StringView;
using QLatin1String = QLatin1StringView;
#else
class QLatin1String;
using QLatin1StringView = QLatin1String;
#endif
class QString;
class QStringRef; // defined in bobui5compat
class QStringView;
template <bool> class QBasicUtf8StringView;
class QAnyStringView;
class QChar;
class QRegularExpression;
class QRegularExpressionMatch;

#ifdef Q_QDOC
class QUtf8StringView;
#else
// ### BobUI 7: remove the non-char8_t version of QUtf8StringView
BOBUI_BEGIN_NO_CHAR8_T_NAMESPACE
using QUtf8StringView = QBasicUtf8StringView<false>;
BOBUI_END_NO_CHAR8_T_NAMESPACE

BOBUI_BEGIN_HAS_CHAR8_T_NAMESPACE
using QUtf8StringView = QBasicUtf8StringView<true>;
BOBUI_END_HAS_CHAR8_T_NAMESPACE
#endif // Q_QDOC

BOBUI_END_NAMESPACE

#endif // QSTRINGFWD_H
