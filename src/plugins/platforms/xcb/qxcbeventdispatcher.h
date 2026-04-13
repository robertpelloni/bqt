// Copyright (C) 2018 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#pragma once

#include <BobUICore/QObject>
#include <BobUICore/QEventLoop>

#include <BobUICore/private/qeventdispatcher_unix_p.h>
#if BOBUI_CONFIG(glib)
#include <BobUICore/private/qeventdispatcher_glib_p.h>
#include <glib.h>
#endif

BOBUI_BEGIN_NAMESPACE

class QXcbConnection;

class QXcbUnixEventDispatcher : public QEventDispatcherUNIX
{
    Q_OBJECT
public:
    explicit QXcbUnixEventDispatcher(QXcbConnection *connection, QObject *parent = nullptr);
    ~QXcbUnixEventDispatcher();
    bool processEvents(QEventLoop::ProcessEventsFlags flags) override;

private:
    QXcbConnection *m_connection;
};

#if BOBUI_CONFIG(glib)

struct XcbEventSource;
class QXcbGlibEventDispatcherPrivate;

class QXcbGlibEventDispatcher : public QEventDispatcherGlib
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(QXcbGlibEventDispatcher)

public:
    explicit QXcbGlibEventDispatcher(QXcbConnection *connection, QObject *parent = nullptr);
    ~QXcbGlibEventDispatcher();

    bool processEvents(QEventLoop::ProcessEventsFlags flags) override;
    QEventLoop::ProcessEventsFlags flags() const { return m_flags; }

private:
    XcbEventSource *m_xcbEventSource;
    GSourceFuncs m_xcbEventSourceFuncs;
    QEventLoop::ProcessEventsFlags m_flags;
};

class QXcbGlibEventDispatcherPrivate : public QEventDispatcherGlibPrivate
{
    Q_DECLARE_PUBLIC(QXcbGlibEventDispatcher)

public:
    QXcbGlibEventDispatcherPrivate();
};

#endif // BOBUI_CONFIG(glib)

class QXcbEventDispatcher
{
public:
    static QAbstractEventDispatcher *createEventDispatcher(QXcbConnection *connection);
};

BOBUI_END_NAMESPACE
