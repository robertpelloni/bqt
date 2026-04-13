// Copyright (C) 2018 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#ifndef BOBUIESTSUPPORT_WIDGETS_H
#define BOBUIESTSUPPORT_WIDGETS_H

#include <BobUIWidgets/bobuiwidgetsglobal.h>
#include <BobUIGui/bobuiestsupport_gui.h>

BOBUI_BEGIN_NAMESPACE

class QPointingDevice;
class QWidget;

namespace BOBUIest {

[[nodiscard]] Q_WIDGETS_EXPORT bool qWaitForWindowActive(QWidget *widget, int timeout);
[[nodiscard]] Q_WIDGETS_EXPORT bool qWaitForWindowActive(QWidget *widget, QDeadlineTimer timeout);
[[nodiscard]] Q_WIDGETS_EXPORT bool qWaitForWindowActive(QWidget *widget);

[[nodiscard]] Q_WIDGETS_EXPORT bool qWaitForWindowFocused(QWidget *widget, QDeadlineTimer timeout);
[[nodiscard]] Q_WIDGETS_EXPORT bool qWaitForWindowFocused(QWidget *widget);

[[nodiscard]] Q_WIDGETS_EXPORT bool qWaitForWindowExposed(QWidget *widget, int timeout);
[[nodiscard]] Q_WIDGETS_EXPORT bool qWaitForWindowExposed(QWidget *widget, QDeadlineTimer timeout);
[[nodiscard]] Q_WIDGETS_EXPORT bool qWaitForWindowExposed(QWidget *widget);

class Q_WIDGETS_EXPORT BOBUIouchEventWidgetSequence : public BOBUIouchEventSequence
{
public:
    ~BOBUIouchEventWidgetSequence() override;
    BOBUIouchEventWidgetSequence& press(int touchId, const QPoint &pt, QWidget *widget = nullptr);
    BOBUIouchEventWidgetSequence& move(int touchId, const QPoint &pt, QWidget *widget = nullptr);
    BOBUIouchEventWidgetSequence& release(int touchId, const QPoint &pt, QWidget *widget = nullptr);
    BOBUIouchEventWidgetSequence& stationary(int touchId) override;

    bool commit(bool processEvents = true) override;

private:
    BOBUIouchEventWidgetSequence(QWidget *widget, QPointingDevice *aDevice, bool autoCommit);

    QPoint mapToScreen(QWidget *widget, const QPoint &pt);

    QWidget *targetWidget = nullptr;

    friend BOBUIouchEventWidgetSequence touchEvent(QWidget *widget, QPointingDevice *device, bool autoCommit);
};

} // namespace BOBUIest

BOBUI_END_NAMESPACE

#endif
