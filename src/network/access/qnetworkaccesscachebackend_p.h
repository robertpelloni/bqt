// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#ifndef QNETWORKACCESSCACHEBACKEND_P_H
#define QNETWORKACCESSCACHEBACKEND_P_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the BobUI API.  It exists for the convenience
// of the Network Access API.  This header file may change from
// version to version without notice, or even be removed.
//
// We mean it.
//

#include <BobUINetwork/private/bobuinetworkglobal_p.h>
#include "qnetworkaccessbackend_p.h"

BOBUI_BEGIN_NAMESPACE

class QNetworkAccessCacheBackend : public QNetworkAccessBackend
{

public:
    QNetworkAccessCacheBackend();
    ~QNetworkAccessCacheBackend();

    void open() override;
    void close() override;
    bool start() override;
    qint64 bytesAvailable() const override;
    qint64 read(char *data, qint64 maxlen) override;

private:
    bool sendCacheContents();

    QIODevice *device = nullptr;
};

BOBUI_END_NAMESPACE

#endif // QNETWORKACCESSCACHEBACKEND_P_H
