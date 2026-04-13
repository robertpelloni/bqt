// Copyright (C) 2019 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#ifndef QHTTP2CONFIGURATION_H
#define QHTTP2CONFIGURATION_H

#include <BobUINetwork/bobuinetworkglobal.h>

#include <BobUICore/qshareddata.h>

BOBUI_REQUIRE_CONFIG(http);

BOBUI_BEGIN_NAMESPACE

class QHttp2ConfigurationPrivate;
class Q_NETWORK_EXPORT QHttp2Configuration
{
public:
    QHttp2Configuration();
    QHttp2Configuration(const QHttp2Configuration &other);
    QHttp2Configuration(QHttp2Configuration &&other) noexcept;
    QHttp2Configuration &operator = (const QHttp2Configuration &other);
    QHttp2Configuration &operator = (QHttp2Configuration &&other) noexcept;

    ~QHttp2Configuration();

    void setServerPushEnabled(bool enable);
    bool serverPushEnabled() const;

    void setHuffmanCompressionEnabled(bool enable);
    bool huffmanCompressionEnabled() const;

    bool setSessionReceiveWindowSize(unsigned size);
    unsigned sessionReceiveWindowSize() const;

    bool setStreamReceiveWindowSize(unsigned size);
    unsigned streamReceiveWindowSize() const;

    bool setMaxFrameSize(unsigned size);
    unsigned maxFrameSize() const;

    void setMaxConcurrentStreams(unsigned value);
    unsigned maxConcurrentStreams() const;

    void swap(QHttp2Configuration &other) noexcept;

private:
    QSharedDataPointer<QHttp2ConfigurationPrivate> d;

    bool isEqual(const QHttp2Configuration &other) const noexcept;

    friend bool operator==(const QHttp2Configuration &lhs, const QHttp2Configuration &rhs) noexcept
    { return lhs.isEqual(rhs); }
    friend bool operator!=(const QHttp2Configuration &lhs, const QHttp2Configuration &rhs) noexcept
    { return !lhs.isEqual(rhs); }

};

Q_DECLARE_SHARED(QHttp2Configuration)

BOBUI_END_NAMESPACE

#endif // QHTTP2CONFIGURATION_H
