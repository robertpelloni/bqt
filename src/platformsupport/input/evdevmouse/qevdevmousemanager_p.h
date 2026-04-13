// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QEVDEVMOUSEMANAGER_P_H
#define QEVDEVMOUSEMANAGER_P_H

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

#include "qevdevmousehandler_p.h"

#include <BobUIInputSupport/private/devicehandlerlist_p.h>

#include <QObject>
#include <QHash>
#include <QSocketNotifier>
#include <QPoint>

BOBUI_BEGIN_NAMESPACE

class QDeviceDiscovery;

class QEvdevMouseManager : public QObject
{
public:
    QEvdevMouseManager(const QString &key, const QString &specification, QObject *parent = nullptr);
    ~QEvdevMouseManager();

    void handleMouseEvent(int x, int y, bool abs, BobUI::MouseButtons buttons,
                          BobUI::MouseButton button, QEvent::Type type);
    void handleWheelEvent(QPoint delta);

    void addMouse(const QString &deviceNode = QString());
    void removeMouse(const QString &deviceNode);

private:
    void clampPosition();
    void updateDeviceCount();

    QString m_spec;
    BobUIInputSupport::DeviceHandlerList<QEvdevMouseHandler> m_mice;
    int m_x;
    int m_y;
    int m_xoffset;
    int m_yoffset;
};

BOBUI_END_NAMESPACE

#endif // QEVDEVMOUSEMANAGER_P_H
