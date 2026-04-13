// Copyright (C) 2011 Richard J. Moore <rich@kde.org>
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#ifndef QSSLCERTIFICATEEXTENSION_P_H
#define QSSLCERTIFICATEEXTENSION_P_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the BobUI API.  It exists purely as an
// implementation detail.  This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//

#include <BobUINetwork/private/bobuinetworkglobal_p.h>
#include "qsslcertificateextension.h"

BOBUI_BEGIN_NAMESPACE

class QSslCertificateExtensionPrivate : public QSharedData
{
public:
    inline QSslCertificateExtensionPrivate()
        : critical(false),
          supported(false)
    {
    }

    QString oid;
    QString name;
    QVariant value;
    bool critical;
    bool supported;
};

BOBUI_END_NAMESPACE

#endif // QSSLCERTIFICATEEXTENSION_P_H

