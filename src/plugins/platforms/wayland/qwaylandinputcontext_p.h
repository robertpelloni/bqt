// Copyright (C) 2016 Klarälvdalens Datakonsult AB, a KDAB Group company, info@kdab.com
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only


#ifndef QWAYLANDINPUTCONTEXT_H
#define QWAYLANDINPUTCONTEXT_H

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

#include <qpa/qplatforminputcontext.h>

#include <QLoggingCategory>
#include <QPointer>

#include "qwaylandtextinputinterface_p.h"
#include <bobuiwaylandclientglobal_p.h>
#if BOBUI_CONFIG(xkbcommon)
#include <xkbcommon/xkbcommon-compose.h>
#endif

struct wl_callback;
struct wl_callback_listener;

BOBUI_BEGIN_NAMESPACE

Q_DECLARE_LOGGING_CATEGORY(qLcQpaInputMethods)

namespace BobUIWaylandClient {

class QWaylandDisplay;

class QWaylandInputContext : public QPlatformInputContext
{
    Q_OBJECT
public:
    explicit QWaylandInputContext(QWaylandDisplay *display);
    ~QWaylandInputContext() override;

    bool isValid() const override;

    void reset() override;
    void commit() override;
    void update(BobUI::InputMethodQueries) override;

    void invokeAction(QInputMethod::Action, int cursorPosition) override;

    void showInputPanel() override;
    void hideInputPanel() override;
    bool isInputPanelVisible() const override;
    QRectF keyboardRect() const override;

    QLocale locale() const override;
    BobUI::LayoutDirection inputDirection() const override;

    void setFocusObject(QObject *object) override;

#if BOBUI_CONFIG(xkbcommon)
    bool filterEvent(const QEvent *event) override;

    // This invokable is called from QXkbCommon::setXkbContext().
    Q_INVOKABLE void setXkbContext(struct xkb_context *context) { m_XkbContext = context; }
#endif

private:
    QWaylandTextInputInterface *textInput() const;

    QWaylandDisplay *mDisplay = nullptr;
    QPointer<QWindow> mCurrentWindow;

#if BOBUI_CONFIG(xkbcommon)
    void ensureInitialized();

    bool m_initialized = false;
    QObject *m_focusObject = nullptr;
    xkb_compose_table *m_composeTable = nullptr;
    xkb_compose_state *m_composeState = nullptr;
    struct xkb_context *m_XkbContext = nullptr;
#endif
};

}

BOBUI_END_NAMESPACE

#endif // QWAYLANDINPUTCONTEXT_H
