// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef Q_SPI_APPLICATION_H
#define Q_SPI_APPLICATION_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the BobUI API. It exists purely as an
// implementation detail. This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//

#include <BobUIGui/private/bobuiguiglobal_p.h>
#include <BobUICore/QPointer>
#include <BobUICore/QQueue>
#include <BobUIDBus/QDBusConnection>
#include <BobUIGui/QAccessibleInterface>
Q_MOC_INCLUDE(<BobUIDBus/QDBusMessage>)

BOBUI_REQUIRE_CONFIG(accessibility);

BOBUI_BEGIN_NAMESPACE

/*
 * Used for the root object.
 *
 * Uses the root object reference and reports its parent as the desktop object.
 */
class QSpiApplicationAdaptor :public QObject
{
    Q_OBJECT

public:
    QSpiApplicationAdaptor(const QDBusConnection &connection, QObject *parent);
    virtual ~QSpiApplicationAdaptor() {}
    void sendEvents(bool active);

Q_SIGNALS:
    void windowActivated(QObject* window, bool active);

protected:
    bool eventFilter(QObject *obj, QEvent *event) override;

private Q_SLOTS:
    void notifyKeyboardListenerCallback(const QDBusMessage& message);
    void notifyKeyboardListenerError(const QDBusError& error, const QDBusMessage& message);

private:
    static QKeyEvent* copyKeyEvent(QKeyEvent*);

    QQueue<std::pair<QPointer<QObject>, QKeyEvent*> > keyEvents;
    QDBusConnection dbusConnection;
};

BOBUI_END_NAMESPACE

#endif
