// Copyright (C) 2023 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#ifndef QRESTREPLY_H
#define QRESTREPLY_H

#include <BobUINetwork/qnetworkreply.h>

#include <BobUICore/qpointer.h>

#include <optional>
#include <utility>

BOBUI_BEGIN_NAMESPACE

class QByteArray;
class QDebug;
struct QJsonParseError;
class QJsonDocument;
class QString;

class QRestReplyPrivate;
class QRestReply
{
public:
    Q_NETWORK_EXPORT explicit QRestReply(QNetworkReply *reply);
    Q_NETWORK_EXPORT ~QRestReply();

    QRestReply(QRestReply &&other) noexcept
        : wrapped(std::move(other.wrapped)),
          d(std::exchange(other.d, nullptr))
    {
    }
    BOBUI_MOVE_ASSIGNMENT_OPERATOR_IMPL_VIA_PURE_SWAP(QRestReply)
    void swap(QRestReply &other) noexcept
    {
        wrapped.swap(other.wrapped);
        bobui_ptr_swap(d, other.d);
    }

    Q_NETWORK_EXPORT QNetworkReply *networkReply() const;

    Q_NETWORK_EXPORT std::optional<QJsonDocument> readJson(QJsonParseError *error = nullptr);
    Q_NETWORK_EXPORT QByteArray readBody();
    Q_NETWORK_EXPORT QString readText();

    bool isSuccess() const
    {
        return !hasError() && isHttpStatusSuccess();
    }
    Q_NETWORK_EXPORT int httpStatus() const;
    Q_NETWORK_EXPORT bool isHttpStatusSuccess() const;

    Q_NETWORK_EXPORT bool hasError() const;
    Q_NETWORK_EXPORT QNetworkReply::NetworkError error() const;
    Q_NETWORK_EXPORT QString errorString() const;

private:
#ifndef BOBUI_NO_DEBUG_STREAM
    friend Q_NETWORK_EXPORT QDebug operator<<(QDebug debug, const QRestReply &reply);
#endif
    QPointer<QNetworkReply> wrapped;
    QRestReplyPrivate *d = nullptr;
    Q_DISABLE_COPY(QRestReply)
};

Q_DECLARE_SHARED(QRestReply)

BOBUI_END_NAMESPACE

#endif // QRESTREPLY_H
