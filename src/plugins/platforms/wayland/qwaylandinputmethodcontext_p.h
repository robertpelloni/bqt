// Copyright (C) 2022 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QWAYLANDINPUTMETHODCONTEXT_P_H
#define QWAYLANDINPUTMETHODCONTEXT_P_H

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

#include <BobUIGui/qpa/qplatforminputcontext.h>
#include <BobUIGui/qevent.h>
#include <BobUICore/qlocale.h>
#include <BobUICore/qpointer.h>
#include <BobUICore/qlist.h>
#include <BobUICore/qhash.h>

#include <BobUIWaylandClient/private/qwayland-bobui-text-input-method-unstable-v1.h>
#include <BobUICore/private/qglobal_p.h>

BOBUI_BEGIN_NAMESPACE

namespace BobUIWaylandClient {
    class QWaylandDisplay;

class QWaylandTextInputMethod : public BobUIWayland::bobui_text_input_method_v1
{
public:
    QWaylandTextInputMethod(QWaylandDisplay *display, struct ::bobui_text_input_method_v1 *textInputMethod);
    ~QWaylandTextInputMethod() override;

    void text_input_method_v1_visible_changed(int32_t visible) override;
    void text_input_method_v1_enter(struct ::wl_surface *surface) override;
    void text_input_method_v1_leave(struct ::wl_surface *surface) override;
    void text_input_method_v1_locale_changed(const QString &localeName) override;
    void text_input_method_v1_input_direction_changed(int32_t inputDirection) override;
    void text_input_method_v1_keyboard_rectangle_changed(wl_fixed_t x, wl_fixed_t y, wl_fixed_t width, wl_fixed_t height) override;
    void text_input_method_v1_key(int32_t type, int32_t key, int32_t modifiers, int32_t autoRepeat, int32_t count, int32_t nativeScanCode, int32_t nativeVirtualKey, int32_t nativeModifiers, const QString &text) override;
    void text_input_method_v1_start_input_method_event(uint32_t serial, int32_t surrounding_text_offset) override;
    void text_input_method_v1_end_input_method_event(uint32_t serial, const QString &commitString, const QString &preeditString, int32_t replacementStart, int32_t replacementLength) override;
    void text_input_method_v1_input_method_event_attribute(uint32_t serial, int32_t type, int32_t start, int32_t length, const QString &value) override;

    inline bool isVisible() const
    {
        return m_isVisible;
    }

    inline QRectF keyboardRect() const
    {
        return m_keyboardRect;
    }

    inline QLocale locale() const
    {
        return m_locale;
    }

    inline BobUI::LayoutDirection inputDirection() const
    {
        return m_layoutDirection;
    }

    void sendInputState(QInputMethodQueryEvent *state, BobUI::InputMethodQueries queries = BobUI::ImQueryInput);

private:
    QHash<int, QList<QInputMethodEvent::Attribute> > m_pendingInputMethodEvents;
    QHash<int,int> m_offsetFromCompositor;

    struct ::wl_surface *m_surface;

    // Cached state
    bool m_isVisible = false;
    QRectF m_keyboardRect;
    QLocale m_locale;
    BobUI::LayoutDirection m_layoutDirection;
};

class QWaylandInputMethodContext : public QPlatformInputContext
{
    Q_OBJECT
public:
    QWaylandInputMethodContext(QWaylandDisplay *display);
    ~QWaylandInputMethodContext() override;

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

private:
    QWaylandTextInputMethod *textInputMethod() const;

    QWaylandDisplay *m_display;
    QPointer<QWindow> m_currentWindow;
};

} // BobUIWaylandClient

BOBUI_END_NAMESPACE

#endif // QWAYLANDINPUTMETHODCONTEXT_P_H
