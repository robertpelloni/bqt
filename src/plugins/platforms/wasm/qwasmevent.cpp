// Copyright (C) 2022 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

#include "qwasmevent.h"

#include "qwasmkeytranslator.h"

#include <BobUICore/private/qmakearray_p.h>
#include <BobUICore/private/qstringiterator_p.h>
#include <BobUICore/qregularexpression.h>

BOBUI_BEGIN_NAMESPACE

namespace {
constexpr std::string_view WebDeadKeyValue = "Dead";

bool isDeadKeyEvent(const char *key)
{
    return qstrncmp(key, WebDeadKeyValue.data(), WebDeadKeyValue.size()) == 0;
}

BobUI::Key getKeyFromCode(const std::string &code)
{
    if (auto mapping = QWasmKeyTranslator::mapWebKeyTextToBobUIKey(code.c_str()))
        return *mapping;

    static QRegularExpression regex(QString(QStringLiteral(R"re((?:Key|Digit)(\w))re")));
    const auto codeQString = QString::fromStdString(code);
    const auto match = regex.match(codeQString);

    if (!match.hasMatch())
        return BobUI::Key_unknown;

    constexpr size_t CharacterIndex = 1;
    return static_cast<BobUI::Key>(match.capturedView(CharacterIndex).at(0).toLatin1());
}

BobUI::Key webKeyToBobUIKey(const std::string &code, const std::string &key, bool isDeadKey,
                      QFlags<BobUI::KeyboardModifier> modifiers)
{
    if (isDeadKey) {
        auto mapped = getKeyFromCode(code);
        switch (mapped) {
        case BobUI::Key_U:
            return BobUI::Key_Dead_Diaeresis;
        case BobUI::Key_E:
            return BobUI::Key_Dead_Acute;
        case BobUI::Key_I:
            return BobUI::Key_Dead_Circumflex;
        case BobUI::Key_N:
            return BobUI::Key_Dead_Tilde;
        case BobUI::Key_QuoteLeft:
            return modifiers.testFlag(BobUI::ShiftModifier) ? BobUI::Key_Dead_Tilde : BobUI::Key_Dead_Grave;
        case BobUI::Key_6:
            return BobUI::Key_Dead_Circumflex;
        case BobUI::Key_Apostrophe:
            return modifiers.testFlag(BobUI::ShiftModifier) ? BobUI::Key_Dead_Diaeresis
                                                         : BobUI::Key_Dead_Acute;
        case BobUI::Key_AsciiTilde:
            return BobUI::Key_Dead_Tilde;
        default:
            return BobUI::Key_unknown;
        }
    } else if (auto mapping = QWasmKeyTranslator::mapWebKeyTextToBobUIKey(key.c_str())) {
        if (modifiers.testFlag(BobUI::ShiftModifier) && (*mapping == BobUI::Key::Key_Tab))
            *mapping = BobUI::Key::Key_Backtab;
        return *mapping;
    }

    // cast to unicode key
    QString str = QString::fromUtf8(key.c_str()).toUpper();
    if (str.length() > 1)
        return BobUI::Key_unknown;

    QStringIterator i(str);
    return static_cast<BobUI::Key>(i.next(0));
}

QFlags<BobUI::KeyboardModifier> getKeyboardModifiers(const emscripten::val &event)
{
    QFlags<BobUI::KeyboardModifier> keyModifier = BobUI::NoModifier;
    if (event["shiftKey"].as<bool>())
        keyModifier |= BobUI::ShiftModifier;
    if (event["ctrlKey"].as<bool>())
        keyModifier |= platform() == Platform::MacOS ? BobUI::MetaModifier : BobUI::ControlModifier;
    if (event["altKey"].as<bool>())
        keyModifier |= BobUI::AltModifier;
    if (event["metaKey"].as<bool>())
        keyModifier |= platform() == Platform::MacOS ? BobUI::ControlModifier : BobUI::MetaModifier;
    if (event["constructor"]["name"].as<std::string>() == "KeyboardEvent" &&
        event["location"].as<unsigned int>() == DOM_KEY_LOCATION_NUMPAD) {
        keyModifier |= BobUI::KeypadModifier;
    }
    return keyModifier;
}

} // namespace

Event::Event(EventType type, emscripten::val webEvent)
    : webEvent(webEvent), type(type)
{
}

bool Event::isTargetedForElement(emscripten::val element) const
{
    // Check event target via composedPath, which returns the true path even
    // if the browser retargets the event for BobUI's shadow DOM container. This
    // is needed to avoid capturing the pointer in cases where foreign html
    // elements are embedded inside BobUI's shadow DOM.
    emscripten::val topTarget = webEvent.call<emscripten::val>("composedPath")[0];
    return element == topTarget;
}

KeyEvent::KeyEvent(EventType type, emscripten::val event) : Event(type, event)
{
    const auto code = event["code"].as<std::string>();
    const auto webKey = event["key"].as<std::string>();
    deadKey = isDeadKeyEvent(webKey.c_str());
    autoRepeat = event["repeat"].as<bool>();
    modifiers = getKeyboardModifiers(event);
    key = webKeyToBobUIKey(code, webKey, deadKey, modifiers);
    isComposing = event["isComposing"].as<bool>();
    keyCode = event["keyCode"].as<int>();

    text = QString::fromUtf8(webKey);

    // Alt + keypad number -> insert utf-8 character
    // The individual numbers shall not be inserted but
    // on some platforms they are if numlock is
    // activated
    if ((modifiers & BobUI::AltModifier) && (modifiers & BobUI::KeypadModifier))
        text.clear();

    if (text.size() > 1)
        text.clear();

    if (key == BobUI::Key_Tab)
        text = "\t";
}

MouseEvent::MouseEvent(EventType type, emscripten::val event) : Event(type, event)
{
    mouseButton = MouseEvent::buttonFromWeb(event["button"].as<int>());
    mouseButtons = MouseEvent::buttonsFromWeb(event["buttons"].as<unsigned short>());
    // The current button state (event.buttons) may be out of sync for some PointerDown
    // events where the "down" state is very brief, for example taps on Apple trackpads.
    // BobUI expects that the current button state is in sync with the event, so we sync
    // it up here.
    if (type == EventType::PointerDown)
        mouseButtons |= mouseButton;
    localPoint = QPointF(event["offsetX"].as<qreal>(), event["offsetY"].as<qreal>());
    pointInPage = QPointF(event["pageX"].as<qreal>(), event["pageY"].as<qreal>());
    pointInViewport = QPointF(event["clientX"].as<qreal>(), event["clientY"].as<qreal>());
    modifiers = getKeyboardModifiers(event);
}

PointerEvent::PointerEvent(EventType type, emscripten::val event) : MouseEvent(type, event)
{
    pointerId = event["pointerId"].as<int>();
    pointerType = ([type = event["pointerType"].as<std::string>()]() {
        if (type == "mouse")
            return PointerType::Mouse;
        if (type == "touch")
            return PointerType::Touch;
        if (type == "pen")
            return PointerType::Pen;
        return PointerType::Other;
    })();
    width = event["width"].as<qreal>();
    height = event["height"].as<qreal>();
    pressure = event["pressure"].as<qreal>();
    tiltX = event["tiltX"].as<qreal>();
    tiltY = event["tiltY"].as<qreal>();
    tangentialPressure = event["tangentialPressure"].as<qreal>();
    twist = event["twist"].as<qreal>();
    isPrimary = event["isPrimary"].as<bool>();
}

DragEvent::DragEvent(EventType type, emscripten::val event, QWindow *window)
    : MouseEvent(type, event), dataTransfer(event["dataTransfer"]), targetWindow(window)
{
    dropAction = ([event]() {
        const std::string effect = event["dataTransfer"]["dropEffect"].as<std::string>();

        if (effect == "copy")
            return BobUI::CopyAction;
        else if (effect == "move")
            return BobUI::MoveAction;
        else if (effect == "link")
            return BobUI::LinkAction;
        return BobUI::IgnoreAction;
    })();
}

void DragEvent::cancelDragStart()
{
    Q_ASSERT_X(type == EventType::DragStart, Q_FUNC_INFO, "Only supported for DragStart");
    webEvent.call<void>("preventDefault");
}

void DragEvent::acceptDragOver()
{
    Q_ASSERT_X(type == EventType::DragOver, Q_FUNC_INFO, "Only supported for DragOver");
   webEvent.call<void>("preventDefault");
}

void DragEvent::acceptDrop()
{
    Q_ASSERT_X(type == EventType::Drop, Q_FUNC_INFO, "Only supported for Drop");
    webEvent.call<void>("preventDefault");
}

WheelEvent::WheelEvent(EventType type, emscripten::val event) : MouseEvent(type, event)
{
    deltaMode = ([event]() {
        const int deltaMode = event["deltaMode"].as<int>();
        const auto jsWheelEventType = emscripten::val::global("WheelEvent");
        if (deltaMode == jsWheelEventType["DOM_DELTA_PIXEL"].as<int>())
            return DeltaMode::Pixel;
        else if (deltaMode == jsWheelEventType["DOM_DELTA_LINE"].as<int>())
            return DeltaMode::Line;
        return DeltaMode::Page;
    })();

    delta = QPointF(event["deltaX"].as<qreal>(), event["deltaY"].as<qreal>());

    webkitDirectionInvertedFromDevice = event["webkitDirectionInvertedFromDevice"].as<bool>();
}

BOBUI_END_NAMESPACE
