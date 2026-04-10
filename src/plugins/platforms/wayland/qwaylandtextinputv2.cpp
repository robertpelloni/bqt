// Copyright (C) 2016 Klarälvdalens Datakonsult AB, a KDAB Group company, info@kdab.com
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include <qpa/qplatforminputcontext.h>

#include "qwaylandtextinputv2_p.h"

#include "qwaylandinputcontext_p.h"
#include "qwaylandwindow_p.h"
#include "qwaylandinputmethodeventbuilder_p.h"

#include <BobUICore/qloggingcategory.h>
#include <BobUIGui/QGuiApplication>
#include <BobUIGui/private/qguiapplication_p.h>
#include <BobUIGui/private/qhighdpiscaling_p.h>
#include <BobUIGui/qpa/qplatformintegration.h>
#include <BobUIGui/qevent.h>
#include <BobUIGui/qwindow.h>
#include <BOBUIextCharFormat>
#include <QList>
#include <QRectF>
#include <QLocale>

BOBUI_BEGIN_NAMESPACE

namespace BobUIWaylandClient {

namespace {

const BobUI::InputMethodQueries supportedQueries2 = BobUI::ImEnabled |
                                                BobUI::ImSurroundingText |
                                                BobUI::ImCursorPosition |
                                                BobUI::ImAnchorPosition |
                                                BobUI::ImHints |
                                                BobUI::ImCursorRectangle |
                                                BobUI::ImPreferredLanguage;
}

QWaylandTextInputv2::QWaylandTextInputv2(QWaylandDisplay *display, struct ::zwp_text_input_v2 *text_input)
    : BobUIWayland::zwp_text_input_v2(text_input)
    , m_display(display)
{
}

QWaylandTextInputv2::~QWaylandTextInputv2()
{
    if (m_resetCallback)
        wl_callback_destroy(m_resetCallback);
    destroy();
}

void QWaylandTextInputv2::showInputPanel()
{
    qCDebug(qLcQpaInputMethods()) << Q_FUNC_INFO;
    show_input_panel();
}
void QWaylandTextInputv2::hideInputPanel()
{
    qCDebug(qLcQpaInputMethods()) << Q_FUNC_INFO;
    hide_input_panel();
}
void QWaylandTextInputv2::enableSurface(::wl_surface *surface)
{
    qCDebug(qLcQpaInputMethods()) << Q_FUNC_INFO << surface;
    enable(surface);
}
void QWaylandTextInputv2::disableSurface(::wl_surface *surface)
{
    qCDebug(qLcQpaInputMethods()) << Q_FUNC_INFO << surface;
    disable(surface);
}

void QWaylandTextInputv2::reset()
{
    qCDebug(qLcQpaInputMethods()) << Q_FUNC_INFO;
    m_builder.reset();
    m_preeditCommit = QString();
    updateState(BobUI::ImQueryAll, BobUIWayland::zwp_text_input_v2::update_state_reset);
}

void QWaylandTextInputv2::commit()
{
    qCDebug(qLcQpaInputMethods()) << Q_FUNC_INFO;
    if (QObject *o = QGuiApplication::focusObject()) {
        if (!m_preeditCommit.isEmpty()) {

            QInputMethodEvent event;
            event.setCommitString(m_preeditCommit);
            m_preeditCommit = QString();

            QCoreApplication::sendEvent(o, &event);
        }
    }

    reset();
}

const wl_callback_listener QWaylandTextInputv2::callbackListener = {
    QWaylandTextInputv2::resetCallback
};

void QWaylandTextInputv2::resetCallback(void *data, wl_callback *, uint32_t)
{
    QWaylandTextInputv2 *self = static_cast<QWaylandTextInputv2*>(data);

    if (self->m_resetCallback) {
        wl_callback_destroy(self->m_resetCallback);
        self->m_resetCallback = nullptr;
    }
}

void QWaylandTextInputv2::updateState(BobUI::InputMethodQueries queries, uint32_t flags)
{
    qCDebug(qLcQpaInputMethods()) << Q_FUNC_INFO << queries << flags;
    if (!QGuiApplication::focusObject())
        return;

    if (!QGuiApplication::focusWindow() || !QGuiApplication::focusWindow()->handle())
        return;

    auto *window = static_cast<QWaylandWindow *>(QGuiApplication::focusWindow()->handle());
    auto *surface = window->wlSurface();
    if (!surface || (surface != m_surface))
        return;

    queries &= supportedQueries2;

    // Surrounding text, cursor and anchor positions are transferred together
    if ((queries & BobUI::ImSurroundingText) || (queries & BobUI::ImCursorPosition) || (queries & BobUI::ImAnchorPosition))
        queries |= BobUI::ImSurroundingText | BobUI::ImCursorPosition | BobUI::ImAnchorPosition;

    QInputMethodQueryEvent event(queries);
    QCoreApplication::sendEvent(QGuiApplication::focusObject(), &event);

    if ((queries & BobUI::ImSurroundingText) || (queries & BobUI::ImCursorPosition) || (queries & BobUI::ImAnchorPosition)) {
        QString text = event.value(BobUI::ImSurroundingText).toString();
        int cursor = event.value(BobUI::ImCursorPosition).toInt();
        int anchor = event.value(BobUI::ImAnchorPosition).toInt();

        // Make sure text is not too big
        if (text.toUtf8().size() > 2048) {
            int c = qAbs(cursor - anchor) <= 512 ? qMin(cursor, anchor) + qAbs(cursor - anchor) / 2: cursor;

            const int offset = c - qBound(0, c, 512 - qMin(text.size() - c, 256));
            text = text.mid(offset + c - 256, 512);
            cursor -= offset;
            anchor -= offset;
        }

        set_surrounding_text(text, QWaylandInputMethodEventBuilder::indexToWayland(text, cursor), QWaylandInputMethodEventBuilder::indexToWayland(text, anchor));
    }

    if (queries & BobUI::ImHints) {
        QWaylandInputMethodContentType contentType = QWaylandInputMethodContentType::convert(static_cast<BobUI::InputMethodHints>(event.value(BobUI::ImHints).toInt()));
        set_content_type(contentType.hint, contentType.purpose);
    }

    if (queries & BobUI::ImCursorRectangle) {
        const QRect &cRect = event.value(BobUI::ImCursorRectangle).toRect();
        const QRect &windowRect = QGuiApplication::inputMethod()->inputItemTransform().mapRect(cRect);
        const QRect &nativeRect = QHighDpi::toNativePixels(windowRect, QGuiApplication::focusWindow());
        const QMargins margins = window->clientSideMargins();
        const QRect &surfaceRect = nativeRect.translated(margins.left(), margins.top());
        set_cursor_rectangle(surfaceRect.x(), surfaceRect.y(), surfaceRect.width(), surfaceRect.height());
    }

    if (queries & BobUI::ImPreferredLanguage) {
        const QString &language = event.value(BobUI::ImPreferredLanguage).toString();
        set_preferred_language(language);
    }

    update_state(m_serial, flags);
    if (flags != BobUIWayland::zwp_text_input_v2::update_state_change) {
        if (m_resetCallback)
            wl_callback_destroy(m_resetCallback);
        m_resetCallback = wl_display_sync(m_display->wl_display());
        wl_callback_add_listener(m_resetCallback, &QWaylandTextInputv2::callbackListener, this);
    }
}

void QWaylandTextInputv2::setCursorInsidePreedit(int)
{
    // Not supported yet
}

bool QWaylandTextInputv2::isInputPanelVisible() const
{
    return m_inputPanelVisible;
}

QRectF QWaylandTextInputv2::keyboardRect() const
{
    return m_keyboardRectangle;
}

QLocale QWaylandTextInputv2::locale() const
{
    return m_locale;
}

BobUI::LayoutDirection QWaylandTextInputv2::inputDirection() const
{
    return m_inputDirection;
}

void QWaylandTextInputv2::zwp_text_input_v2_enter(uint32_t serial, ::wl_surface *surface)
{
    qCDebug(qLcQpaInputMethods()) << Q_FUNC_INFO << serial << surface;
    m_serial = serial;
    m_surface = surface;

    updateState(BobUI::ImQueryAll, BobUIWayland::zwp_text_input_v2::update_state_enter);
}

void QWaylandTextInputv2::zwp_text_input_v2_leave(uint32_t serial, ::wl_surface *surface)
{
    qCDebug(qLcQpaInputMethods()) << Q_FUNC_INFO << serial << surface;
    m_serial = serial;

    if (m_surface != surface) {
        qCDebug(qLcQpaInputMethods()) << Q_FUNC_INFO << "Got leave event for surface" << surface << "focused surface" << m_surface;
    }

    m_surface = nullptr;
}

void QWaylandTextInputv2::zwp_text_input_v2_modifiers_map(wl_array *map)
{
    const QList<QByteArray> modifiersMap = QByteArray::fromRawData(static_cast<const char*>(map->data), map->size).split('\0');

    m_modifiersMap.clear();

    for (const QByteArray &modifier : modifiersMap) {
        if (modifier == "Shift")
            m_modifiersMap.append(BobUI::ShiftModifier);
        else if (modifier == "Control")
            m_modifiersMap.append(BobUI::ControlModifier);
        else if (modifier == "Alt")
            m_modifiersMap.append(BobUI::AltModifier);
        else if (modifier == "Mod1")
            m_modifiersMap.append(BobUI::AltModifier);
        else if (modifier == "Mod4")
            m_modifiersMap.append(BobUI::MetaModifier);
        else
            m_modifiersMap.append(BobUI::NoModifier);
    }
}

void QWaylandTextInputv2::zwp_text_input_v2_input_panel_state(uint32_t visible, int32_t x, int32_t y, int32_t width, int32_t height)
{
    const bool inputPanelVisible = (visible == input_panel_visibility_visible);
    if (m_inputPanelVisible != inputPanelVisible) {
        m_inputPanelVisible = inputPanelVisible;
        QGuiApplicationPrivate::platformIntegration()->inputContext()->emitInputPanelVisibleChanged();
    }
    const QRectF keyboardRectangle(x, y, width, height);
    if (m_keyboardRectangle != keyboardRectangle) {
        m_keyboardRectangle = keyboardRectangle;
        QGuiApplicationPrivate::platformIntegration()->inputContext()->emitKeyboardRectChanged();
    }
}

void QWaylandTextInputv2::zwp_text_input_v2_preedit_string(const QString &text, const QString &commit)
{
    if (m_resetCallback) {
        qCDebug(qLcQpaInputMethods()) << "discard preedit_string: reset not confirmed";
        m_builder.reset();
        return;
    }

    if (!QGuiApplication::focusObject())
        return;

    QInputMethodEvent *event = m_builder.buildPreedit(text);

    m_builder.reset();
    m_preeditCommit = commit;

    QCoreApplication::sendEvent(QGuiApplication::focusObject(), event);
    delete event;
}

void QWaylandTextInputv2::zwp_text_input_v2_preedit_styling(uint32_t index, uint32_t length, uint32_t style)
{
    m_builder.addPreeditStyling(index, length, style);
}

void QWaylandTextInputv2::zwp_text_input_v2_preedit_cursor(int32_t index)
{
    m_builder.setPreeditCursor(index);
}

void QWaylandTextInputv2::zwp_text_input_v2_commit_string(const QString &text)
{
    if (m_resetCallback) {
        qCDebug(qLcQpaInputMethods()) << "discard commit_string: reset not confirmed";
        m_builder.reset();
        return;
    }

    if (!QGuiApplication::focusObject())
        return;

    QInputMethodEvent *event = m_builder.buildCommit(text);

    m_builder.reset();

    QCoreApplication::sendEvent(QGuiApplication::focusObject(), event);
    delete event;
}

void QWaylandTextInputv2::zwp_text_input_v2_cursor_position(int32_t index, int32_t anchor)
{
    m_builder.setCursorPosition(index, anchor);
}

void QWaylandTextInputv2::zwp_text_input_v2_delete_surrounding_text(uint32_t before_length, uint32_t after_length)
{
    m_builder.setDeleteSurroundingText(before_length, after_length);
}

void QWaylandTextInputv2::zwp_text_input_v2_keysym(uint32_t time, uint32_t sym, uint32_t state, uint32_t modifiers)
{
#if BOBUI_CONFIG(xkbcommon)
    if (m_resetCallback) {
        qCDebug(qLcQpaInputMethods()) << "discard keysym: reset not confirmed";
        return;
    }

    if (!QGuiApplication::focusWindow())
        return;

    BobUI::KeyboardModifiers bobuiModifiers = modifiersToBobUIModifiers(modifiers);

    QEvent::Type type = state == WL_KEYBOARD_KEY_STATE_PRESSED ? QEvent::KeyPress : QEvent::KeyRelease;
    QString text = QXkbCommon::lookupStringNoKeysymTransformations(sym);
    int bobuikey = QXkbCommon::keysymToBobUIKey(sym, bobuiModifiers);

    QWindowSystemInterface::handleKeyEvent(QGuiApplication::focusWindow(),
                                           time, type, bobuikey, bobuiModifiers, text);
#else
    Q_UNUSED(time);
    Q_UNUSED(sym);
    Q_UNUSED(state);
    Q_UNUSED(modifiers);
#endif
}

void QWaylandTextInputv2::zwp_text_input_v2_language(const QString &language)
{
    if (m_resetCallback) {
        qCDebug(qLcQpaInputMethods()) << "discard language: reset not confirmed";
        return;
    }

    const QLocale locale(language);
    if (m_locale != locale) {
        m_locale = locale;
        QGuiApplicationPrivate::platformIntegration()->inputContext()->emitLocaleChanged();
    }
}

void QWaylandTextInputv2::zwp_text_input_v2_text_direction(uint32_t direction)
{
    if (m_resetCallback) {
        qCDebug(qLcQpaInputMethods()) << "discard text_direction: reset not confirmed";
        return;
    }

    const BobUI::LayoutDirection inputDirection = (direction == text_direction_auto) ? BobUI::LayoutDirectionAuto :
                                               (direction == text_direction_ltr) ? BobUI::LeftToRight :
                                               (direction == text_direction_rtl) ? BobUI::RightToLeft : BobUI::LayoutDirectionAuto;
    if (m_inputDirection != inputDirection) {
        m_inputDirection = inputDirection;
        QGuiApplicationPrivate::platformIntegration()->inputContext()->emitInputDirectionChanged(m_inputDirection);
    }
}

void QWaylandTextInputv2::zwp_text_input_v2_input_method_changed(uint32_t serial, uint32_t flags)
{
    Q_UNUSED(flags);

    m_serial = serial;
    updateState(BobUI::ImQueryAll, BobUIWayland::zwp_text_input_v2::update_state_full);
}

BobUI::KeyboardModifiers QWaylandTextInputv2::modifiersToBobUIModifiers(uint32_t modifiers)
{
    BobUI::KeyboardModifiers ret = BobUI::NoModifier;
    for (int i = 0; i < m_modifiersMap.size(); ++i) {
        if (modifiers & (1 << i)) {
            ret |= m_modifiersMap[i];
        }
    }
    return ret;
}

}

BOBUI_END_NAMESPACE
