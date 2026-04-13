// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

//
//  W A R N I N G
//  -------------
//
// This file is not part of the BobUI API.  It exists for the convenience
// of other BobUI classes.  This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//

#ifndef QMULTITOUCH_MAC_P_H
#define QMULTITOUCH_MAC_P_H

#include <BobUICore/qhash.h>
#include <BobUICore/private/qcore_mac_p.h>

#include <BobUIGui/qpointingdevice.h>

#include <qpa/qwindowsysteminterface.h>

Q_FORWARD_DECLARE_OBJC_CLASS(NSTouch);
BOBUI_FORWARD_DECLARE_OBJC_ENUM(NSTouchPhase, unsigned long);

BOBUI_BEGIN_NAMESPACE

class QCocoaTouch
{
    public:
        static QList<QWindowSystemInterface::TouchPoint> getCurrentTouchPointList(NSEvent *event, bool acceptSingleTouch);
        static void setMouseInDraggingState(bool inDraggingState);
        static QPointingDevice *getTouchDevice(QInputDevice::DeviceType type, quint64 id);

    private:
        static QHash<quint64, QPointingDevice*> _touchDevices;
        static QHash<qint64, QCocoaTouch*> _currentTouches;
        static QPointF _screenReferencePos;
        static QPointF _trackpadReferencePos;
        static int _idAssignmentCount;
        static int _touchCount;
        static bool _updateInternalStateOnly;

        QWindowSystemInterface::TouchPoint _touchPoint;
        qint64 _identity;

        QCocoaTouch(NSTouch *nstouch);
        ~QCocoaTouch();

        void updateTouchData(NSTouch *nstouch, NSTouchPhase phase);
        static QCocoaTouch *findQCocoaTouch(NSTouch *nstouch);
        static QEventPoint::State toTouchPointState(NSTouchPhase nsState);
};

BOBUI_END_NAMESPACE

#endif // QMULTITOUCH_MAC_P_H

