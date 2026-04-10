// Copyright (C) 2024 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QVXTOUCHMANAGER_P_H
#define QVXTOUCHMANAGER_P_H

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

#include <BobUIInputSupport/private/devicehandlerlist_p.h>

#include <QObject>
#include <QHash>
#include <QSocketNotifier>

BOBUI_BEGIN_NAMESPACE

class QVxTouchScreenHandlerThread;

class QVxTouchManager : public QObject
{
public:
    QVxTouchManager(const QString &key, const QString &spec, QObject *parent = nullptr);
    ~QVxTouchManager();

    void addDevice(const QString &deviceNode);
    void removeDevice(const QString &deviceNode);

    void updateInputDeviceCount();

private:
    QString m_spec;
    BobUIInputSupport::DeviceHandlerList<QVxTouchScreenHandlerThread> m_activeDevices;
};

BOBUI_END_NAMESPACE

#endif // QVXTOUCHMANAGER_P_H
