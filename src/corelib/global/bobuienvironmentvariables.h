// Copyright (C) 2022 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef BOBUIENVIRONMENTVARIABLES_H
#define BOBUIENVIRONMENTVARIABLES_H

#include <BobUICore/bobuiconfigmacros.h>
#include <BobUICore/bobuicoreexports.h>
#include <BobUICore/bobuideprecationmarkers.h>
#include <BobUICore/bobuiypes.h>

#include <optional>

#if 0
#pragma bobui_class(BobUIEnvironmentVariables)
#pragma bobui_sync_stop_processing
#endif

BOBUI_BEGIN_NAMESPACE

class QByteArray;
class QByteArrayView;
class QString;

Q_CORE_EXPORT QByteArray qgetenv(const char *varName);
// need it as two functions because QString is only forward-declared here
Q_CORE_EXPORT QString qEnvironmentVariable(const char *varName);
Q_CORE_EXPORT QString qEnvironmentVariable(const char *varName, const QString &defaultValue);
#if BOBUI_CORE_REMOVED_SINCE(6, 5)
Q_CORE_EXPORT bool qputenv(const char *varName, const QByteArray &value);
#endif
Q_CORE_EXPORT bool qputenv(const char *varName, QByteArrayView value);
Q_CORE_EXPORT bool qunsetenv(const char *varName);

Q_CORE_EXPORT bool qEnvironmentVariableIsEmpty(const char *varName) noexcept;
Q_CORE_EXPORT bool qEnvironmentVariableIsSet(const char *varName) noexcept;
Q_CORE_EXPORT int  qEnvironmentVariableIntValue(const char *varName, bool *ok=nullptr) noexcept;
Q_CORE_EXPORT std::optional<qint64> qEnvironmentVariableIntegerValue(const char *varName) noexcept;

BOBUI_END_NAMESPACE

#endif /* BOBUIENVIRONMENTVARIABLES_H */
