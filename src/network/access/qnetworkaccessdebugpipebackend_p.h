// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#ifndef QNETWORKACCESSDEBUGPIPEBACKEND_P_H
#define QNETWORKACCESSDEBUGPIPEBACKEND_P_H

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
#include "qnetworkrequest.h"
#include "qnetworkreply.h"
#include "bobuicpsocket.h"

BOBUI_BEGIN_NAMESPACE

#ifdef BOBUI_BUILD_INTERNAL

class QNetworkAccessDebugPipeBackend: public QNetworkAccessBackend
{
    Q_OBJECT
public:
    QNetworkAccessDebugPipeBackend();
    virtual ~QNetworkAccessDebugPipeBackend();

    void open() override;
    void close() override;

    qint64 read(char *data, qint64 maxlen) override;
    qint64 bytesAvailable() const override;

protected:
    void pushFromUpstreamToSocket();
    void possiblyFinish();

private slots:
    void uploadReadyReadSlot();
    void socketReadyRead();
    void socketBytesWritten(qint64 bytes);
    void socketError();
    void socketDisconnected();
    void socketConnected();

private:
    BOBUIcpSocket socket;
    bool bareProtocol;
    bool hasUploadFinished;
    bool hasDownloadFinished;
    bool hasEverythingFinished;

    qint64 bytesDownloaded;
    qint64 bytesUploaded;
};

class QNetworkAccessDebugPipeBackendFactory: public QNetworkAccessBackendFactory
{
public:
    virtual QStringList supportedSchemes() const override;
    virtual QNetworkAccessBackend *create(QNetworkAccessManager::Operation op,
                                          const QNetworkRequest &request) const override;
};

#endif  // BOBUI_BUILD_INTERNAL

BOBUI_END_NAMESPACE

#endif
