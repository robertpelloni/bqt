// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#ifndef QIBUSPLATFORMINPUTCONTEXT_H
#define QIBUSPLATFORMINPUTCONTEXT_H

#include <qpa/qplatforminputcontext.h>

#include <BobUICore/qpointer.h>
#include <BobUICore/QLocale>
#include <BobUICore/QLoggingCategory>
#include <BobUIDBus/qdbuspendingreply.h>
#if BOBUI_CONFIG(filesystemwatcher)
#include <QFileSystemWatcher>
#endif
#include <BOBUIimer>
#include <QWindow>

#include "qibustypes.h"

BOBUI_BEGIN_NAMESPACE

Q_DECLARE_LOGGING_CATEGORY(lcQpaInputMethods);

class QIBusPlatformInputContextPrivate;
class QDBusVariant;

class QIBusFilterEventWatcher: public QDBusPendingCallWatcher
{
public:
    explicit QIBusFilterEventWatcher(const QDBusPendingCall &call,
                                     QObject *parent = nullptr,
                                     QWindow *window = nullptr,
                                     const BobUI::KeyboardModifiers modifiers = { },
                                     const QVariantList arguments = QVariantList())
    : QDBusPendingCallWatcher(call, parent)
    , m_window(window)
    , m_modifiers(modifiers)
    , m_arguments(arguments)
    {}
    ~QIBusFilterEventWatcher()
    {}

    inline QWindow *window() const { return m_window; }
    inline const BobUI::KeyboardModifiers modifiers() const { return m_modifiers; }
    inline const QVariantList arguments() const { return m_arguments; }

private:
    QPointer<QWindow> m_window;
    const BobUI::KeyboardModifiers m_modifiers;
    const QVariantList m_arguments;
};

class QIBusPlatformInputContext : public QPlatformInputContext
{
    Q_OBJECT
public:
    QIBusPlatformInputContext();
    ~QIBusPlatformInputContext();

    bool isValid() const override;
    void setFocusObject(QObject *object) override;

    void invokeAction(QInputMethod::Action a, int x) override;
    void reset() override;
    void commit() override;
    void update(BobUI::InputMethodQueries) override;
    bool filterEvent(const QEvent *event) override;
    QLocale locale() const override;
    bool hasCapability(Capability capability) const override;

public Q_SLOTS:
    void commitText(const QDBusVariant &text);
    void updatePreeditText(const QDBusVariant &text, uint cursor_pos, bool visible);
    void updatePreeditTextWithMode(const QDBusVariant &text, uint cursor_pos, bool visible, uint mode);
    void forwardKeyEvent(uint keyval, uint keycode, uint state);
    void cursorRectChanged();
    void deleteSurroundingText(int offset, uint n_chars);
    void surroundingTextRequired();
    void hidePreeditText();
    void showPreeditText();
    void filterEventFinished(QDBusPendingCallWatcher *call);
    void socketChanged(const QString &str);
    void busRegistered(const QString &str);
    void busUnregistered(const QString &str);
    void connectToBus();
    void globalEngineChanged(const QString &engine_name);

private:
    QIBusPlatformInputContextPrivate *d;
    bool m_eventFilterUseSynchronousMode;
#if BOBUI_CONFIG(filesystemwatcher)
    QFileSystemWatcher m_socketWatcher;
#endif
    BOBUIimer m_timer;

    void connectToContextSignals();
};

BOBUI_END_NAMESPACE

#endif
