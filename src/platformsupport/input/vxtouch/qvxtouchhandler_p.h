// Copyright (C) 2024 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QVXTOUCHHANDLER_P_H
#define QVXTOUCHHANDLER_P_H

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

#include <BobUIGui/private/bobuiguiglobal_p.h>
#include <QObject>
#include <QString>
#include <QList>
#include <QHash>
#include <BOBUIhread>
#include <QLoggingCategory>
#include <BobUICore/private/bobuihread_p.h>
#include <qpa/qwindowsysteminterface.h>
#include <BobUIInputSupport/private/bobuiouchfilter_p.h>

BOBUI_BEGIN_NAMESPACE

Q_DECLARE_LOGGING_CATEGORY(qLcVxTouch)

class QSocketNotifier;
class QPointingDevice;
class QVxTouchScreenData;
class QVxTouchScreenHandlerThread;

class QVxTouchScreenHandler : public QObject
{
    Q_OBJECT

public:
    explicit QVxTouchScreenHandler(const QString &device, const QString &spec = QString(), QObject *parent = nullptr);
    ~QVxTouchScreenHandler();

    QPointingDevice *touchDevice() const;

    bool isFiltered() const;

    void readData();

signals:
    void touchPointsUpdated();

private:
    friend class QVxTouchScreenData;
    friend class QVxTouchScreenHandlerThread;

    void registerPointingDevice();
    void unregisterPointingDevice();

    QSocketNotifier *m_notify;
    int m_fd;
    QVxTouchScreenData *d;
    QPointingDevice *m_device;
};

class QVxTouchScreenHandlerThread : public QDaemonThread
{
    Q_OBJECT
public:
    explicit QVxTouchScreenHandlerThread(const QString &device, const QString &spec, QObject *parent = nullptr);
    ~QVxTouchScreenHandlerThread();
    void run() override;

    bool isPointingDeviceRegistered() const;

    bool eventFilter(QObject *object, QEvent *event) override;

    void scheduleTouchPointUpdate();

signals:
    void touchDeviceRegistered();

private:
    Q_INVOKABLE void notifyTouchDeviceRegistered();

    void filterAndSendTouchPoints();
    QRect targetScreenGeometry() const;

    QString m_device;
    QString m_spec;
    QVxTouchScreenHandler *m_handler;
    bool m_touchDeviceRegistered;

    bool m_touchUpdatePending;
    QWindow *m_filterWindow;

    struct FilteredTouchPoint {
        BOBUIouchFilter x;
        BOBUIouchFilter y;
        QWindowSystemInterface::TouchPoint touchPoint;
    };
    QHash<int, FilteredTouchPoint> m_filteredPoints;

    float m_touchRate;
};

BOBUI_END_NAMESPACE

#endif // QVXTOUCHHANDLER_P_H
