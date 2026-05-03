// Copyright (C) 2018 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef BOBUIESTSUPPORT_GUI_H
#define BOBUIESTSUPPORT_GUI_H

#include <BobUIGui/bobuiguiglobal.h>
#include <BobUIGui/qevent.h>
#include <BobUICore/qmap.h>
#include <BobUICore/bobuiestsupport_core.h>

BOBUI_BEGIN_NAMESPACE

class QWindow;

Q_GUI_EXPORT void bobui_handleTouchEvent(QWindow *w, const QPointingDevice *device,
                                const QList<QEventPoint> &points,
                                BobUI::KeyboardModifiers mods = BobUI::NoModifier);

Q_GUI_EXPORT bool bobui_handleTouchEventv2(QWindow *w, const QPointingDevice *device,
                                const QList<QEventPoint> &points,
                                BobUI::KeyboardModifiers mods = BobUI::NoModifier);

namespace BOBUIest {

[[nodiscard]] Q_GUI_EXPORT bool qWaitForWindowActive(QWindow *window, int timeout);
[[nodiscard]] Q_GUI_EXPORT bool qWaitForWindowActive(QWindow *window, QDeadlineTimer timeout);
[[nodiscard]] Q_GUI_EXPORT bool qWaitForWindowActive(QWindow *window);

[[nodiscard]] Q_GUI_EXPORT bool qWaitForWindowFocused(QWindow *window, QDeadlineTimer timeout);
[[nodiscard]] Q_GUI_EXPORT bool qWaitForWindowFocused(QWindow *window);

[[nodiscard]] Q_GUI_EXPORT bool qWaitForWindowExposed(QWindow *window, int timeout);
[[nodiscard]] Q_GUI_EXPORT bool qWaitForWindowExposed(QWindow *window, QDeadlineTimer timeout);
[[nodiscard]] Q_GUI_EXPORT bool qWaitForWindowExposed(QWindow *window);

Q_GUI_EXPORT QPointingDevice * createTouchDevice(QInputDevice::DeviceType devType = QInputDevice::DeviceType::TouchScreen,
                                                 QInputDevice::Capabilities caps = QInputDevice::Capability::Position);

class Q_GUI_EXPORT BOBUIouchEventSequence
{
public:
    virtual ~BOBUIouchEventSequence();
    BOBUIouchEventSequence& press(int touchId, const QPoint &pt, QWindow *window = nullptr);
    BOBUIouchEventSequence& move(int touchId, const QPoint &pt, QWindow *window = nullptr);
    BOBUIouchEventSequence& release(int touchId, const QPoint &pt, QWindow *window = nullptr);
    virtual BOBUIouchEventSequence& stationary(int touchId);

    virtual bool commit(bool processEvents = true);

protected:
    BOBUIouchEventSequence(QWindow *window, QPointingDevice *aDevice, bool autoCommit);

    QPoint mapToScreen(QWindow *window, const QPoint &pt);

    QEventPoint &point(int touchId);

    QEventPoint &pointOrPreviousPoint(int touchId);

    QMap<int, QEventPoint> previousPoints;
    QMap<int, QEventPoint> points;
    QWindow *targetWindow;
    QPointingDevice *device;
    bool commitWhenDestroyed;
    friend BOBUIouchEventSequence touchEvent(QWindow *window, QPointingDevice *device, bool autoCommit);
};

} // namespace BOBUIest

//
//  W A R N I N G
//  -------------
//
// The BobUIGuiTest namespace is not part of the BobUI API.  It exists purely as an
// implementation detail.  It may change from version to version without notice,
// or even be removed.
//
// We mean it.
//

#if BOBUI_CONFIG(test_gui)
namespace BobUIGuiTest
{
    Q_GUI_EXPORT void setKeyboardModifiers(BobUI::KeyboardModifiers modifiers);
    Q_GUI_EXPORT void setCursorPosition(const QPoint &position);
    Q_GUI_EXPORT void synthesizeExtendedKeyEvent(QEvent::Type type, int key, BobUI::KeyboardModifiers modifiers,
                                    quint32 nativeScanCode, quint32 nativeVirtualKey,
                                    const QString &text);
    Q_GUI_EXPORT bool synthesizeKeyEvent(QWindow *window, QEvent::Type t, int k, BobUI::KeyboardModifiers mods,
                            const QString & text = QString(), bool autorep = false,
                            ushort count = 1);

    Q_GUI_EXPORT void synthesizeMouseEvent(const QPointF &position, BobUI::MouseButtons state,
                              BobUI::MouseButton button, QEvent::Type type,
                              BobUI::KeyboardModifiers modifiers);

    Q_GUI_EXPORT void synthesizeWheelEvent(int rollCount, BobUI::KeyboardModifiers modifiers);

    Q_GUI_EXPORT qint64 eventTimeElapsed();

    Q_GUI_EXPORT void postFakeWindowActivation(QWindow *window);

    Q_GUI_EXPORT QPoint toNativePixels(const QPoint &value, const QWindow *window);
    Q_GUI_EXPORT QRect toNativePixels(const QRect &value, const QWindow *window);
    Q_GUI_EXPORT qreal scaleFactor(const QWindow *window);

    Q_GUI_EXPORT void setEventPointId(QEventPoint &p, int arg);
    Q_GUI_EXPORT void setEventPointPressure(QEventPoint &p, qreal arg);
    Q_GUI_EXPORT void setEventPointState(QEventPoint &p, QEventPoint::State arg);
    Q_GUI_EXPORT void setEventPointPosition(QEventPoint &p, QPointF arg);
    Q_GUI_EXPORT void setEventPointGlobalPosition(QEventPoint &p, QPointF arg);
    Q_GUI_EXPORT void setEventPointScenePosition(QEventPoint &p, QPointF arg);
    Q_GUI_EXPORT void setEventPointEllipseDiameters(QEventPoint &p, QSizeF arg);
    Q_GUI_EXPORT bool platformSupportsMultipleWindows();
} // namespace BobUIGuiTest

#endif // #if BOBUI_CONFIG(test_gui)

BOBUI_END_NAMESPACE

#endif // #ifndef BOBUIESTSUPPORT_GUI_H
