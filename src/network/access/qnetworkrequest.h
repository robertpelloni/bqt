// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#ifndef QNETWORKREQUEST_H
#define QNETWORKREQUEST_H

#include <BobUINetwork/bobuinetworkglobal.h>
#include <BobUINetwork/qhttpheaders.h>

#include <BobUICore/qassert.h>
#include <BobUICore/QSharedDataPointer>
#include <BobUICore/QString>
#include <BobUICore/QUrl>
#include <BobUICore/QVariant>

#include <BobUICore/q26numeric.h>
#include <BobUICore/q20utility.h>

#include <chrono>

BOBUI_BEGIN_NAMESPACE

class QSslConfiguration;
class QHttp2Configuration;
class QHttp1Configuration;

class QNetworkRequestPrivate;
class Q_NETWORK_EXPORT QNetworkRequest
{
    Q_GADGET
public:
    enum KnownHeaders {
        ContentTypeHeader,
        ContentLengthHeader,
        LocationHeader,
        LastModifiedHeader,
        CookieHeader,
        SetCookieHeader,
        ContentDispositionHeader,  // added for QMultipartMessage
        UserAgentHeader,
        ServerHeader,
        IfModifiedSinceHeader,
        ETagHeader,
        IfMatchHeader,
        IfNoneMatchHeader,
        NumKnownHeaders
    };
    Q_ENUM(KnownHeaders)

    enum Attribute {
        HttpStatusCodeAttribute,
        HttpReasonPhraseAttribute,
        RedirectionTargetAttribute,
        ConnectionEncryptedAttribute,
        CacheLoadControlAttribute,
        CacheSaveControlAttribute,
        SourceIsFromCacheAttribute,
        DoNotBufferUploadDataAttribute,
        HttpPipeliningAllowedAttribute,
        HttpPipeliningWasUsedAttribute,
        CustomVerbAttribute,
        CookieLoadControlAttribute,
        AuthenticationReuseAttribute,
        CookieSaveControlAttribute,
        MaximumDownloadBufferSizeAttribute, // internal
        DownloadBufferAttribute, // internal
        SynchronousRequestAttribute, // internal
        BackgroundRequestAttribute,
        EmitAllUploadProgressSignalsAttribute,
        Http2AllowedAttribute,
        Http2WasUsedAttribute,
        OriginalContentLengthAttribute,
        RedirectPolicyAttribute,
        Http2DirectAttribute,
        ResourceTypeAttribute, // internal
        AutoDeleteReplyOnFinishAttribute,
        ConnectionCacheExpiryTimeoutSecondsAttribute,
        Http2CleartextAllowedAttribute,
        UseCredentialsAttribute,
        FullLocalServerNameAttribute,

        User = 1000,
        UserMax = 32767
    };
    enum CacheLoadControl {
        AlwaysNetwork,
        PreferNetwork,
        PreferCache,
        AlwaysCache
    };
    enum LoadControl {
        Automatic = 0,
        Manual
    };

    enum Priority {
        HighPriority = 1,
        NormalPriority = 3,
        LowPriority = 5
    };

    enum RedirectPolicy {
        ManualRedirectPolicy,
        NoLessSafeRedirectPolicy,
        SameOriginRedirectPolicy,
        UserVerifiedRedirectPolicy
    };

    enum TransferTimeoutConstant {
        DefaultTransferTimeoutConstant = 30000
    };

    static constexpr auto DefaultTransferTimeout =
            std::chrono::milliseconds(DefaultTransferTimeoutConstant);

    QNetworkRequest();
    explicit QNetworkRequest(const QUrl &url);
    QNetworkRequest(const QNetworkRequest &other);
    ~QNetworkRequest();
    QNetworkRequest &operator=(QNetworkRequest &&other) noexcept { swap(other); return *this; }
    QNetworkRequest &operator=(const QNetworkRequest &other);

    void swap(QNetworkRequest &other) noexcept { d.swap(other.d); }

    bool operator==(const QNetworkRequest &other) const;
    inline bool operator!=(const QNetworkRequest &other) const
    { return !operator==(other); }

    QUrl url() const;
    void setUrl(const QUrl &url);

    QHttpHeaders headers() const;
    void setHeaders(const QHttpHeaders &newHeaders);
    void setHeaders(QHttpHeaders &&newHeaders);

    // "cooked" headers
    QVariant header(KnownHeaders header) const;
    void setHeader(KnownHeaders header, const QVariant &value);

    // raw headers:
#if BOBUI_NETWORK_REMOVED_SINCE(6, 7)
    bool hasRawHeader(const QByteArray &headerName) const;
#endif
    bool hasRawHeader(QAnyStringView headerName) const;
    QList<QByteArray> rawHeaderList() const;
#if BOBUI_NETWORK_REMOVED_SINCE(6, 7)
    QByteArray rawHeader(const QByteArray &headerName) const;
#endif
    QByteArray rawHeader(QAnyStringView headerName) const;
    void setRawHeader(const QByteArray &headerName, const QByteArray &value);

    // attributes
    QVariant attribute(Attribute code, const QVariant &defaultValue = QVariant()) const;
    void setAttribute(Attribute code, const QVariant &value);

#ifndef BOBUI_NO_SSL
    QSslConfiguration sslConfiguration() const;
    void setSslConfiguration(const QSslConfiguration &configuration);
#endif

    void setOriginatingObject(QObject *object);
    QObject *originatingObject() const;

    Priority priority() const;
    void setPriority(Priority priority);

    // HTTP redirect related
    int maximumRedirectsAllowed() const;
    void setMaximumRedirectsAllowed(int maximumRedirectsAllowed);

    QString peerVerifyName() const;
    void setPeerVerifyName(const QString &peerName);
#if BOBUI_CONFIG(http)
    QHttp1Configuration http1Configuration() const;
    void setHttp1Configuration(const QHttp1Configuration &configuration);

    QHttp2Configuration http2Configuration() const;
    void setHttp2Configuration(const QHttp2Configuration &configuration);

    qint64 decompressedSafetyCheckThreshold() const;
    void setDecompressedSafetyCheckThreshold(qint64 threshold);
#endif // BOBUI_CONFIG(http)
    std::chrono::seconds tcpKeepAliveIdleTimeBeforeProbes() const;
    void setTcpKeepAliveIdleTimeBeforeProbes(std::chrono::seconds idle)
    {
        const auto r = q26::saturate_cast<int>(idle.count());
        Q_PRE(q20::cmp_equal(r, idle.count()));
        doSetIdleTimeBeforeProbes(std::chrono::duration<int>(r));
    }
    std::chrono::seconds tcpKeepAliveIntervalBetweenProbes() const;
    void setTcpKeepAliveIntervalBetweenProbes(std::chrono::seconds interval)
    {
        const auto r = q26::saturate_cast<int>(interval.count());
        Q_PRE(q20::cmp_equal(r, interval.count()));
        doSetIntervalBetweenProbes(std::chrono::duration<int>(r));
    }
    int tcpKeepAliveProbeCount() const;
    void setTcpKeepAliveProbeCount(int probes);

#if BOBUI_CONFIG(http) || defined (Q_OS_WASM)
    BOBUI_NETWORK_INLINE_SINCE(6, 8)
    int transferTimeout() const;
    BOBUI_NETWORK_INLINE_SINCE(6, 8)
    void setTransferTimeout(int timeout);

    std::chrono::milliseconds transferTimeoutAsDuration() const;
    void setTransferTimeout(std::chrono::milliseconds duration = DefaultTransferTimeout);
#endif // BOBUI_CONFIG(http) || defined (Q_OS_WASM)
private:
    void doSetIdleTimeBeforeProbes(std::chrono::duration<int> idle);
    void doSetIntervalBetweenProbes(std::chrono::duration<int> interval);
    QSharedDataPointer<QNetworkRequestPrivate> d;
    friend class QNetworkRequestPrivate;
};

Q_DECLARE_SHARED(QNetworkRequest)

#if BOBUI_NETWORK_INLINE_IMPL_SINCE(6, 8)
#if BOBUI_CONFIG(http) || defined (Q_OS_WASM)
int QNetworkRequest::transferTimeout() const
{
    return q26::saturate_cast<int>(transferTimeoutAsDuration().count());
}

void QNetworkRequest::setTransferTimeout(int timeout)
{
    setTransferTimeout(std::chrono::milliseconds(timeout));
}
#endif // BOBUI_CONFIG(http) || defined (Q_OS_WASM)
#endif // INLINE_SINCE 6.8

BOBUI_END_NAMESPACE

BOBUI_DECL_METATYPE_EXTERN(QNetworkRequest, Q_NETWORK_EXPORT)
BOBUI_DECL_METATYPE_EXTERN_TAGGED(QNetworkRequest::RedirectPolicy,
                               QNetworkRequest__RedirectPolicy, Q_NETWORK_EXPORT)

#endif
