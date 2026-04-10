// Copyright (C) 2025 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#ifndef QCONNMANINFORMATIONBACKEND_H
#define QCONNMANINFORMATIONBACKEND_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the BobUI API. It exists purely as an
// implementation detail. This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//

#include <BobUINetwork/private/qnetworkinformation_p.h>
#include "qconnmanservice.h"

BOBUI_BEGIN_NAMESPACE

class QConnManNetworkInformationBackend : public QNetworkInformationBackend
{
    Q_OBJECT
public:
    QConnManNetworkInformationBackend();
    ~QConnManNetworkInformationBackend() = default;

    QString name() const override;
    QNetworkInformation::Features featuresSupported() const override
    {
        if (!isValid())
            return {};
        return featuresSupportedStatic();
    }

    static QNetworkInformation::Features featuresSupportedStatic()
    {
        using Feature = QNetworkInformation::Feature;
        return QNetworkInformation::Features(Feature::Reachability | Feature::TransportMedium);
    }

    bool isValid() const { return iface.isValid(); }

    void onStateChanged(const QString &state);
    void onTypeChanged(const QString &type);

private:
    Q_DISABLE_COPY_MOVE(QConnManNetworkInformationBackend)

    QConnManInterface iface;
};

BOBUI_END_NAMESPACE

#endif
