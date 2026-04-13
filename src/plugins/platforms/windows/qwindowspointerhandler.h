// Copyright (C) 2019 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QWINDOWSPOINTERHANDLER_H
#define QWINDOWSPOINTERHANDLER_H

#include "bobuiwindowsglobal.h"
#include <BobUICore/bobui_windows.h>

#include <BobUICore/qpointer.h>
#include <BobUICore/qscopedpointer.h>
#include <BobUICore/qsharedpointer.h>
#include <BobUICore/qhash.h>
#include <BobUIGui/qevent.h>
#include <qpa/qwindowsysteminterface.h>

BOBUI_BEGIN_NAMESPACE

class QWindow;
class QPointingDevice;

class QWindowsPointerHandler
{
    Q_DISABLE_COPY_MOVE(QWindowsPointerHandler)
public:
    using QPointingDevicePtr = QSharedPointer<QPointingDevice>;

    QWindowsPointerHandler() = default;
    ~QWindowsPointerHandler();
    bool translatePointerEvent(QWindow *window, HWND hwnd, BobUIWindows::WindowsEventType et, MSG msg, LRESULT *result);
    bool translateMouseEvent(QWindow *window, HWND hwnd, BobUIWindows::WindowsEventType et, MSG msg, LRESULT *result);

    const QPointingDevicePtr &touchDevice() const { return m_touchDevice; }
    void setTouchDevice(const QPointingDevicePtr &d) { m_touchDevice = d; }
    static QPointingDevicePtr createTouchDevice(bool mouseEmulation);

    QWindow *windowUnderMouse() const { return m_windowUnderPointer.data(); }
    void clearWindowUnderMouse() { m_windowUnderPointer = nullptr; }
    void clearEvents();

    static BobUI::MouseButtons queryMouseButtons();

private:
    bool translateTouchEvent(QWindow *window, HWND hwnd, BobUIWindows::WindowsEventType et, MSG msg, PVOID vTouchInfo, unsigned int count);
    bool translatePenEvent(QWindow *window, HWND hwnd, BobUIWindows::WindowsEventType et, MSG msg, PVOID vPenInfo);
    bool translateMouseWheelEvent(QWindow *window, QWindow *currentWindowUnderPointer, MSG msg, QPoint globalPos, BobUI::KeyboardModifiers keyModifiers);
    void handleCaptureRelease(QWindow *window, QWindow *currentWindowUnderPointer, HWND hwnd, QEvent::Type eventType, BobUI::MouseButtons mouseButtons);
    void handleEnterLeave(QWindow *window, QWindow *currentWindowUnderPointer, QPoint globalPos);

#if BOBUI_CONFIG(tabletevent)
    QPointingDevicePtr findTabletDevice(QPointingDevice::PointerType pointerType) const;
#endif

    QPointingDevicePtr m_touchDevice;
#if BOBUI_CONFIG(tabletevent)
    QList<QPointingDevicePtr> m_tabletDevices;
    QPointingDevicePtr m_activeTabletDevice;
#endif
    QHash<int, QWindowSystemInterface::TouchPoint> m_lastTouchPoints;
    QHash<DWORD, int> m_touchInputIDToTouchPointID;
    QPointer<QWindow> m_windowUnderPointer;
    QPointer<QWindow> m_currentWindow;
    QWindow *m_previousCaptureWindow = nullptr;
    bool m_needsEnterOnPointerUpdate = false;
    QEvent::Type m_lastEventType = QEvent::None;
    BobUI::MouseButton m_lastEventButton = BobUI::NoButton;
    DWORD m_pointerType = 0;
    static qint64 m_nextInputDeviceId; // workaround until we know how to get system device IDs
};

BOBUI_END_NAMESPACE

#endif // QWINDOWSPOINTERHANDLER_H
