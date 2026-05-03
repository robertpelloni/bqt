// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#ifndef QNETWORKREPLYFILEIMPL_P_H
#define QNETWORKREPLYFILEIMPL_P_H

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
#include "qnetworkreply.h"
#include "qnetworkreply_p.h"
#include "qnetworkaccessmanager.h"

#include <QFile>
#include <BobUICore/qpointer.h>
#include <private/qabstractfileengine_p.h>

BOBUI_BEGIN_NAMESPACE

class QNetworkReplyFileImplPrivate;
class QNetworkReplyFileImpl: public QNetworkReply
{
    Q_OBJECT
public:
    QNetworkReplyFileImpl(QNetworkAccessManager *manager, const QNetworkRequest &req, const QNetworkAccessManager::Operation op);
    ~QNetworkReplyFileImpl();
    virtual void abort() override;

    // reimplemented from QNetworkReply
    virtual void close() override;
    virtual qint64 bytesAvailable() const override;
    virtual bool isSequential () const override;
    qint64 size() const override;

    virtual qint64 readData(char *data, qint64 maxlen) override;

private Q_SLOTS:
    void fileOpenFinished(bool isOpen);

private:
    Q_DECLARE_PRIVATE(QNetworkReplyFileImpl)
};

class QNetworkReplyFileImplPrivate: public QNetworkReplyPrivate
{
public:
    QNetworkReplyFileImplPrivate();

    QNetworkAccessManagerPrivate *managerPrivate;
    QPointer<QFile> realFile;

    Q_DECLARE_PUBLIC(QNetworkReplyFileImpl)
};

BOBUI_END_NAMESPACE

// ### move to qnetworkrequest.h
BOBUI_DECL_METATYPE_EXTERN_TAGGED(QNetworkRequest::KnownHeaders,
                               QNetworkRequest__KnownHeaders, Q_NETWORK_EXPORT)

#endif // QNETWORKREPLYFILEIMPL_P_H
