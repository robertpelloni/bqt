// Copyright (C) 2014 Robin Burchell <robin.burchell@viroteck.net>
// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef BOBUIUIOHANDLER_P_H
#define BOBUIUIOHANDLER_P_H

#include <QList>
#include <QObject>
#include <QMap>
#include <QUdpSocket>
#include <BOBUIransform>

#include <qpa/qwindowsysteminterface.h>

BOBUI_BEGIN_NAMESPACE

class QPointingDevice;
class QOscMessage;
class BOBUIuioCursor;
class BOBUIuioToken;

class BOBUIuioHandler : public QObject
{
    Q_OBJECT
    Q_MOC_INCLUDE("qoscmessage_p.h")

public:
    explicit BOBUIuioHandler(const QString &specification);
    virtual ~BOBUIuioHandler();

private slots:
    void processPackets();
    void process2DCurSource(const QOscMessage &message);
    void process2DCurAlive(const QOscMessage &message);
    void process2DCurSet(const QOscMessage &message);
    void process2DCurFseq(const QOscMessage &message);
    void process2DObjSource(const QOscMessage &message);
    void process2DObjAlive(const QOscMessage &message);
    void process2DObjSet(const QOscMessage &message);
    void process2DObjFseq(const QOscMessage &message);

private:
    QWindowSystemInterface::TouchPoint cursorToTouchPoint(const BOBUIuioCursor &tc, QWindow *win);
    QWindowSystemInterface::TouchPoint tokenToTouchPoint(const BOBUIuioToken &tc, QWindow *win);

    QPointingDevice *m_device = nullptr;
    QUdpSocket m_socket;
    QMap<int, BOBUIuioCursor> m_activeCursors;
    QList<BOBUIuioCursor> m_deadCursors;
    QMap<int, BOBUIuioToken> m_activeTokens;
    QList<BOBUIuioToken> m_deadTokens;
    BOBUIransform m_transform;
};

BOBUI_END_NAMESPACE

#endif // BOBUIUIOHANDLER_P_H
