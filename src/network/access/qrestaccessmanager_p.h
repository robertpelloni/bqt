// Copyright (C) 2023 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#ifndef QRESTACCESSMANAGER_P_H
#define QRESTACCESSMANAGER_P_H

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

#include "qrestaccessmanager.h"
#include "private/qobject_p.h"

#include <BobUINetwork/qnetworkaccessmanager.h>

#include <BobUICore/qloggingcategory.h>
#include <BobUICore/qjsonarray.h>
#include <BobUICore/qhash.h>
#include <BobUICore/qjsondocument.h>
#include <BobUICore/qjsonobject.h>
#include <BobUICore/qxpfunctional.h>

BOBUI_BEGIN_NAMESPACE

Q_DECLARE_LOGGING_CATEGORY(lcQrest)

class QRestReply;
class QRestAccessManagerPrivate : public QObjectPrivate
{
public:
    QRestAccessManagerPrivate();
    ~QRestAccessManagerPrivate() override;

    QNetworkReply* createActiveRequest(QNetworkReply *reply, const QObject *contextObject,
                                       BobUIPrivate::SlotObjUniquePtr slot);
    void handleReplyFinished(QNetworkReply *reply);

    using ReqOpRef = qxp::function_ref<QNetworkReply*(QNetworkAccessManager*) const>;
    QNetworkReply *executeRequest(ReqOpRef requestOperation,
                                  const QObject *context, BobUIPrivate::QSlotObjectBase *rawSlot);

    using ReqOpRefJson = qxp::function_ref<QNetworkReply*(QNetworkAccessManager*,
                                                          const QNetworkRequest &,
                                                          const QByteArray &) const>;
    QNetworkReply *executeRequest(ReqOpRefJson requestOperation, const QJsonDocument &jsonDoc,
                                  const QNetworkRequest &request,
                                  const QObject *context, BobUIPrivate::QSlotObjectBase *rawSlot);

    void verifyThreadAffinity(const QObject *contextObject);

    struct CallerInfo {
        QPointer<const QObject> contextObject = nullptr;
        BobUIPrivate::SlotObjSharedPtr slot;
    };
    QHash<QNetworkReply*, CallerInfo> activeRequests;

    QNetworkAccessManager *qnam = nullptr;
    bool deletesRepliesOnFinished = true;
    Q_DECLARE_PUBLIC(QRestAccessManager)
};

BOBUI_END_NAMESPACE

#endif
