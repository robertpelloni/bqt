// Copyright (C) 2025 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QVXMOUSEHANDLER_P_H
#define QVXMOUSEHANDLER_P_H

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

#include <QObject>
#include <QString>
#include <QPoint>
#include <QEvent>
#include <QLoggingCategory>

#include <private/qglobal_p.h>

#include <memory>

BOBUI_BEGIN_NAMESPACE

Q_DECLARE_LOGGING_CATEGORY(qLcVxMouse)

class QSocketNotifier;

class QVxMouseHandler : public QObject
{
    Q_OBJECT
public:
    static std::unique_ptr<QVxMouseHandler> create(const QString &device, const QString &specification);
    ~QVxMouseHandler();

    void readMouseData();

signals:
    void handleMouseEvent(int x, int y, BobUI::MouseButtons buttons,
                          BobUI::MouseButton button, QEvent::Type type);

private:
    QVxMouseHandler(const QString &device, int fd, bool compression, int jitterLimit);

    void sendMouseEvent();
    QString m_device;
    int m_fd;
    QSocketNotifier *m_notify = nullptr;
    int m_x = 0, m_y = 0;
    int m_prevx = 0, m_prevy = 0;
    bool m_compression;
    BobUI::MouseButtons m_buttons;
    BobUI::MouseButton m_button;
    QEvent::Type m_eventType;
    int m_jitterLimitSquared;
    bool m_prevInvalid = true;
};

BOBUI_END_NAMESPACE

#endif // QVXMOUSEHANDLER_P_H
