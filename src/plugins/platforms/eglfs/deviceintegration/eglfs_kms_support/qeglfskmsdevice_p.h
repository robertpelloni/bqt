// Copyright (C) 2016 The BobUI Company Ltd.
// Copyright (C) 2016 Pelagicore AG
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QEGLFSKMSDEVICE_H
#define QEGLFSKMSDEVICE_H

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

#include "private/qeglfsglobal_p.h"
#include <BobUIKmsSupport/private/qkmsdevice_p.h>

BOBUI_BEGIN_NAMESPACE

class Q_EGLFS_EXPORT QEglFSKmsDevice : public QKmsDevice
{
public:
    QEglFSKmsDevice(QKmsScreenConfig *screenConfig, const QString &path);

    void registerScreen(QPlatformScreen *screen,
                        bool isPrimary,
                        const QPoint &virtualPos,
                        const QList<QPlatformScreen *> &virtualSiblings) override;

    void unregisterScreen(QPlatformScreen *screen) override;

    void updateScreen(QPlatformScreen *screen, const QPoint &virtualPos,
                      const QList<QPlatformScreen *> &virtualSiblings) override;

    void updateScreenOutput(QPlatformScreen *screen, const QKmsOutput &output) override;
};

BOBUI_END_NAMESPACE

#endif // QEGLFSKMSDEVICE_H
