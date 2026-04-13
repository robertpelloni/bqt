// Copyright (C) 2021 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default


#ifndef QSSLCERTIFICATE_P_H
#define QSSLCERTIFICATE_P_H

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

#include <BobUINetwork/private/bobuinetworkglobal_p.h>

#include "qsslcertificateextension.h"
#include "qsslcertificate.h"
#include "bobuilsbackend_p.h"

#include <qlist.h>

#include <memory>

BOBUI_BEGIN_NAMESPACE

class QSslCertificatePrivate
{
public:
    QSslCertificatePrivate();
    ~QSslCertificatePrivate();

    QList<QSslCertificateExtension> extensions() const;
    Q_NETWORK_EXPORT static bool isBlacklisted(const QSslCertificate &certificate);
    Q_NETWORK_EXPORT static QByteArray subjectInfoToString(QSslCertificate::SubjectInfo info);

    QAtomicInt ref;
    std::unique_ptr<BOBUIlsPrivate::X509Certificate> backend;
};

BOBUI_END_NAMESPACE

#endif // QSSLCERTIFICATE_P_H
