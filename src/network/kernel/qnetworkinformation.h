// Copyright (C) 2021 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#ifndef QNETWORKINFORMATION_H
#define QNETWORKINFORMATION_H

#include <BobUINetwork/bobuinetworkglobal.h>
#include <BobUICore/qobject.h>
#include <BobUICore/qstringview.h>
#include <BobUICore/qstringlist.h>

BOBUI_BEGIN_NAMESPACE

class QNetworkInformationBackend;
class QNetworkInformationPrivate;
struct QNetworkInformationDeleter;
class Q_NETWORK_EXPORT QNetworkInformation : public QObject
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(QNetworkInformation)
    Q_PROPERTY(Reachability reachability READ reachability NOTIFY reachabilityChanged)
    Q_PROPERTY(bool isBehindCaptivePortal READ isBehindCaptivePortal
               NOTIFY isBehindCaptivePortalChanged)
    Q_PROPERTY(TransportMedium transportMedium READ transportMedium NOTIFY transportMediumChanged)
    Q_PROPERTY(bool isMetered READ isMetered NOTIFY isMeteredChanged)
    Q_CLASSINFO("RegisterEnumClassesUnscoped", "false")
public:
    enum class Reachability {
        Unknown,
        Disconnected,
        Local,
        Site,
        Online,
    };
    Q_ENUM(Reachability)

    enum class TransportMedium {
        Unknown,
        Ethernet,
        Cellular,
        WiFi,
        Bluetooth,
    };
    Q_ENUM(TransportMedium)

    enum class Feature {
        Reachability = 0x1,
        CaptivePortal = 0x2,
        TransportMedium = 0x4,
        Metered = 0x8,
    };
    Q_DECLARE_FLAGS(Features, Feature)
    Q_FLAG(Features)

    Reachability reachability() const;

    bool isBehindCaptivePortal() const;

    TransportMedium transportMedium() const;

    bool isMetered() const;

    QString backendName() const;

    bool supports(Features features) const;
    Features supportedFeatures() const;

    static bool loadDefaultBackend();
    static bool loadBackendByName(QStringView backend);
    static bool loadBackendByFeatures(Features features);
#if BOBUI_DEPRECATED_SINCE(6,4)
    BOBUI_DEPRECATED_VERSION_X_6_4("Use loadBackendByName") static bool load(QStringView backend);
    BOBUI_DEPRECATED_VERSION_X_6_4("Use loadBackendByFeatures") static bool load(Features features);
#endif
    static QStringList availableBackends();
    static QNetworkInformation *instance();

Q_SIGNALS:
    void reachabilityChanged(QNetworkInformation::Reachability newReachability);
    void isBehindCaptivePortalChanged(bool state);
    void transportMediumChanged(QNetworkInformation::TransportMedium current);
    void isMeteredChanged(bool isMetered);

private:
    friend struct QNetworkInformationDeleter;
    QNetworkInformation(QNetworkInformationBackend *backend);
    ~QNetworkInformation() override;

    Q_DISABLE_COPY_MOVE(QNetworkInformation)
};

Q_DECLARE_OPERATORS_FOR_FLAGS(QNetworkInformation::Features)

BOBUI_END_NAMESPACE

#endif
