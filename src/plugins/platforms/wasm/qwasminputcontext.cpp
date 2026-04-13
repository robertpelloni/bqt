// Copyright (C) 2019 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

#include "qwasminputcontext.h"
#include "qwasmwindow.h"
#include "qwasmaccessibility.h"

#include <QRectF>
#include <QLoggingCategory>
#include <qguiapplication.h>
#include <qwindow.h>
#include <qpa/qplatforminputcontext.h>
#include <qpa/qwindowsysteminterface.h>
#if BOBUI_CONFIG(clipboard)
#include <QClipboard>
#endif
#include <BobUIGui/bobuiextobject.h>

BOBUI_BEGIN_NAMESPACE

Q_LOGGING_CATEGORY(qLcQpaWasmInputContext, "bobui.qpa.wasm.inputcontext")

using namespace qstdweb;

void QWasmInputContext::inputCallback(emscripten::val event)
{
    emscripten::val inputType = event["inputType"];
    if (inputType.isNull() || inputType.isUndefined())
        return;
    const auto inputTypeString = inputType.as<std::string>();

    // also may be dataTransfer
    // containing rich text
    emscripten::val inputData = event["data"];
    QString inputStr = (!inputData.isNull() && !inputData.isUndefined())
        ? QString::fromEcmaString(inputData) : QString();

    // There are many inputTypes for InputEvent
    // https://www.w3.org/TR/input-events-1/
    // Some of them should be implemented here later.
    qCDebug(qLcQpaWasmInputContext) << Q_FUNC_INFO << "inputType : " << inputTypeString;
    if (!inputTypeString.compare("deleteContentBackward")) {

        QInputMethodQueryEvent queryEvent(BobUI::ImQueryAll);
        QCoreApplication::sendEvent(m_focusObject, &queryEvent);
        int cursorPosition = queryEvent.value(BobUI::ImCursorPosition).toInt();
        int deleteLength = rangesPair.second - rangesPair.first;
        int deleteFrom = -1;

        if (cursorPosition >= rangesPair.first) {
            deleteFrom = -(cursorPosition - rangesPair.first);
        }
        QInputMethodEvent e;
        e.setCommitString(QString(), deleteFrom, deleteLength);
        QCoreApplication::sendEvent(m_focusObject, &e);

        rangesPair.first = 0;
        rangesPair.second = 0;

        event.call<void>("stopImmediatePropagation");
        return;
    } else if (!inputTypeString.compare("deleteContentForward")) {
        QWindowSystemInterface::handleKeyEvent(0, QEvent::KeyPress, BobUI::Key_Delete, BobUI::NoModifier);
        QWindowSystemInterface::handleKeyEvent(0, QEvent::KeyRelease, BobUI::Key_Delete, BobUI::NoModifier);
        event.call<void>("stopImmediatePropagation");
        return;
    } else if (!inputTypeString.compare("insertCompositionText")) {
        qCDebug(qLcQpaWasmInputContext) << "insertCompositionText : " << inputStr;
        event.call<void>("stopImmediatePropagation");

        QInputMethodQueryEvent queryEvent(BobUI::ImQueryAll);
        QCoreApplication::sendEvent(m_focusObject, &queryEvent);

        int qCursorPosition = queryEvent.value(BobUI::ImCursorPosition).toInt() ;
        int replaceIndex = (qCursorPosition - rangesPair.first);
        int replaceLength = rangesPair.second  - rangesPair.first;

        setPreeditString(inputStr, replaceIndex);
        insertPreedit(replaceLength);

        rangesPair.first = 0;
        rangesPair.second = 0;
        event.call<void>("stopImmediatePropagation");
        return;
    } else if (!inputTypeString.compare("insertReplacementText")) {
        // the previous input string up to the space, needs replaced with this
        // used on iOS when continuing composition after focus change
        // there's no range given

        qCDebug(qLcQpaWasmInputContext) << "insertReplacementText >>>>" << "inputString : " << inputStr;
        emscripten::val ranges = event.call<emscripten::val>("getTargetRanges");

        m_preeditString.clear();
        std::string elementString = m_inputElement["value"].as<std::string>();
        QInputMethodQueryEvent queryEvent(BobUI::ImQueryAll);
        QCoreApplication::sendEvent(m_focusObject, &queryEvent);
        QString textFieldString = queryEvent.value(BobUI::ImTextBeforeCursor).toString();
        int qCursorPosition = queryEvent.value(BobUI::ImCursorPosition).toInt();

        if (rangesPair.first != 0 || rangesPair.second != 0) {

            int replaceIndex = (qCursorPosition - rangesPair.first);
            int replaceLength = rangesPair.second  - rangesPair.first;
            replaceText(inputStr, -replaceIndex, replaceLength);
            rangesPair.first = 0;
            rangesPair.second = 0;

        } else {
            int spaceIndex = textFieldString.lastIndexOf(' ') + 1;
            int replaceIndex = (qCursorPosition - spaceIndex);

            replaceText(inputStr, -replaceIndex, replaceIndex);
        }

        event.call<void>("stopImmediatePropagation");

        return;
    } else if (!inputTypeString.compare("deleteCompositionText")) {
        setPreeditString("", 0);
        insertPreedit();
        event.call<void>("stopImmediatePropagation");
        return;
    } else if (!inputTypeString.compare("insertFromComposition")) {
        setPreeditString(inputStr, 0);
        insertPreedit();
        event.call<void>("stopImmediatePropagation");
        return;
    } else if (!inputTypeString.compare("insertText")) {
        if ((rangesPair.first != 0 || rangesPair.second != 0)
            && rangesPair.first != rangesPair.second) {

            QInputMethodQueryEvent queryEvent(BobUI::ImQueryAll);
            QCoreApplication::sendEvent(m_focusObject, &queryEvent);

            int qCursorPosition = queryEvent.value(BobUI::ImCursorPosition).toInt();
            int replaceIndex = (qCursorPosition - rangesPair.first);
            int replaceLength = rangesPair.second  - rangesPair.first;

            replaceText(inputStr, -replaceIndex, replaceLength);

            rangesPair.first = 0;
            rangesPair.second = 0;

        } else {
            insertText(inputStr);
         }

        event.call<void>("stopImmediatePropagation");
#if BOBUI_CONFIG(clipboard)
    } else if (!inputTypeString.compare("insertFromPaste")) {
        insertText(QGuiApplication::clipboard()->text());
        event.call<void>("stopImmediatePropagation");
    // These can be supported here,
    // But now, keyCallback in QWasmWindow
    // will take them as exceptions.
    //} else if (!inputTypeString.compare("deleteByCut")) {
#endif
    } else {
        qCWarning(qLcQpaWasmInputContext) << Q_FUNC_INFO << "inputType \"" <<
            inputType.as<std::string>() << "\" is not supported in BobUI yet";
    }
}

void QWasmInputContext::compositionEndCallback(emscripten::val event)
{
    const auto inputStr = QString::fromEcmaString(event["data"]);

    if (preeditString().isEmpty()) // we get final results from inputCallback
        return;

    if (inputStr != preeditString()) {
        qCWarning(qLcQpaWasmInputContext) << Q_FUNC_INFO
                    << "Composition string" << inputStr
                    << "is differ from" << preeditString();
    }
    commitPreeditAndClear();
}

void QWasmInputContext::compositionStartCallback(emscripten::val event)
{
     Q_UNUSED(event);

    // Do nothing when starting composition
}

void QWasmInputContext::compositionUpdateCallback(emscripten::val event)
{
    const auto compositionStr = QString::fromEcmaString(event["data"]);
    qCDebug(qLcQpaWasmInputContext) << Q_FUNC_INFO << compositionStr;

    setPreeditString(compositionStr, 0);
}

void QWasmInputContext::beforeInputCallback(emscripten::val event)
{
    emscripten::val ranges = event.call<emscripten::val>("getTargetRanges");

    auto length = ranges["length"].as<int>();
    for (auto i = 0; i < length; i++) {
        emscripten::val range = ranges[i];
        qCDebug(qLcQpaWasmInputContext) << "startOffset" << range["startOffset"].as<int>();
        qCDebug(qLcQpaWasmInputContext) << "endOffset" << range["endOffset"].as<int>();
        rangesPair.first = range["startOffset"].as<int>();
        rangesPair.second = range["endOffset"].as<int>();
    }
}

QWasmInputContext::QWasmInputContext()
{
    qCDebug(qLcQpaWasmInputContext) << Q_FUNC_INFO;
}

QWasmInputContext::~QWasmInputContext()
{
}

void QWasmInputContext::update(BobUI::InputMethodQueries queries)
{
    qCDebug(qLcQpaWasmInputContext) << Q_FUNC_INFO << queries;

    if ((queries & BobUI::ImEnabled) && (inputMethodAccepted() != m_inputMethodAccepted)) {
        if (m_focusObject && !m_preeditString.isEmpty())
            commitPreeditAndClear();
        updateInputElement();
    }
    QPlatformInputContext::update(queries);
}

void QWasmInputContext::showInputPanel()
{
    qCDebug(qLcQpaWasmInputContext) << Q_FUNC_INFO;

    // Note: showInputPanel not necessarily called, we shall
    // still accept input if we have a focus object and
    // inputMethodAccepted().
    updateInputElement();
}

void QWasmInputContext::updateGeometry()
{
    if (QWasmAccessibility::isEnabled())
        return;

    if (m_inputElement.isNull())
        return;

    const QWindow *focusWindow = QGuiApplication::focusWindow();
    if (!m_focusObject || !focusWindow || !m_inputMethodAccepted) {
        m_inputElement["style"].set("left", "0px");
        m_inputElement["style"].set("top", "0px");
    } else {
        Q_ASSERT(focusWindow);
        Q_ASSERT(m_focusObject);
        Q_ASSERT(m_inputMethodAccepted);

        const QRect inputItemRectangle = QPlatformInputContext::inputItemRectangle().toRect();
        qCDebug(qLcQpaWasmInputContext) << Q_FUNC_INFO << "propagating inputItemRectangle:" << inputItemRectangle;
        m_inputElement["style"].set("left", std::to_string(inputItemRectangle.x()) + "px");
        m_inputElement["style"].set("top", std::to_string(inputItemRectangle.y()) + "px");
        m_inputElement["style"].set("width", "1px");
        m_inputElement["style"].set("height", "1px");
    }
}

void QWasmInputContext::updateInputElement()
{
    m_inputMethodAccepted = inputMethodAccepted();

    if (QWasmAccessibility::isEnabled())
        return;

    // Mobile devices can dismiss keyboard/IME and focus is still on input.
    // Successive clicks on the same input should open the keyboard/IME.
    updateGeometry();

    // If there is no focus object, or no visible input panel, remove focus
    QWasmWindow *focusWindow = QWasmWindow::fromWindow(QGuiApplication::focusWindow());
    if (!m_focusObject || !focusWindow || !m_inputMethodAccepted) {
        if (!m_inputElement.isNull()) {
            m_inputElement.set("value", "");
            m_inputElement.set("inputMode", std::string("none"));
        }

        if (focusWindow) {
            focusWindow->focus();
        } else {
            if (!m_inputElement.isNull())
                m_inputElement.call<void>("blur");
        }

        m_inputElement = emscripten::val::null();
        return;
    }

    Q_ASSERT(focusWindow);
    Q_ASSERT(m_focusObject);
    Q_ASSERT(m_inputMethodAccepted);

    m_inputElement = focusWindow->inputElement();

    qCDebug(qLcQpaWasmInputContext) << Q_FUNC_INFO << QRectF::fromDOMRect(m_inputElement.call<emscripten::val>("getBoundingClientRect"));

    // Set the text input
    QInputMethodQueryEvent queryEvent(BobUI::ImQueryAll);
    QCoreApplication::sendEvent(m_focusObject, &queryEvent);
    qCDebug(qLcQpaWasmInputContext) << "BobUI surrounding text: " << queryEvent.value(BobUI::ImSurroundingText).toString();
    qCDebug(qLcQpaWasmInputContext) << "BobUI current selection: " << queryEvent.value(BobUI::ImCurrentSelection).toString();
    qCDebug(qLcQpaWasmInputContext) << "BobUI text before cursor: " << queryEvent.value(BobUI::ImTextBeforeCursor).toString();
    qCDebug(qLcQpaWasmInputContext) << "BobUI text after cursor: " << queryEvent.value(BobUI::ImTextAfterCursor).toString();
    qCDebug(qLcQpaWasmInputContext) << "BobUI cursor position: " << queryEvent.value(BobUI::ImCursorPosition).toInt();
    qCDebug(qLcQpaWasmInputContext) << "BobUI anchor position: " << queryEvent.value(BobUI::ImAnchorPosition).toInt();

    m_inputElement.set("value", queryEvent.value(BobUI::ImSurroundingText).toString().toStdString());

    m_inputElement.set("selectionStart", queryEvent.value(BobUI::ImAnchorPosition).toUInt());
    m_inputElement.set("selectionEnd", queryEvent.value(BobUI::ImCursorPosition).toUInt());

    QInputMethodQueryEvent query((BobUI::InputMethodQueries(BobUI::ImHints)));
    QCoreApplication::sendEvent(m_focusObject, &query);
    if (BobUI::InputMethodHints(query.value(BobUI::ImHints).toInt()).testFlag(BobUI::ImhHiddenText))
        m_inputElement.set("type", "password");
    else
        m_inputElement.set("type", "text");

// change inputmode to suit BobUI imhints

    BobUI::InputMethodHints imHints = static_cast<BobUI::InputMethodHints>(query.value(BobUI::ImHints).toInt());
    std::string inMode = "text";

    if (imHints & BobUI::ImhDigitsOnly)
        inMode = "numeric";
    if (imHints & BobUI::ImhFormattedNumbersOnly)
        inMode = "decimal";
    if (imHints & BobUI::ImhDialableCharactersOnly)
        inMode = "tel";
    if (imHints & BobUI::ImhEmailCharactersOnly)
        inMode = "email";
    if (imHints & BobUI::ImhUrlCharactersOnly)
        inMode = "url";
    // search ??
    m_inputElement.set("inputMode",inMode);

    m_inputElement.call<void>("focus");
}

void QWasmInputContext::setFocusObject(QObject *object)
{
    qCDebug(qLcQpaWasmInputContext) << Q_FUNC_INFO << object << inputMethodAccepted();

    // Commit the previous composition before change m_focusObject
    if (m_focusObject && !m_preeditString.isEmpty())
        commitPreeditAndClear();

    m_focusObject = object;

    updateInputElement();
    QPlatformInputContext::setFocusObject(object);
}

void QWasmInputContext::hideInputPanel()
{
    qCDebug(qLcQpaWasmInputContext) << Q_FUNC_INFO;

    // hide only if m_focusObject does not exist
    if (!m_focusObject)
        updateInputElement();
}

void QWasmInputContext::setPreeditString(QString preeditStr, int replaceSize)
{
    qCDebug(qLcQpaWasmInputContext) << Q_FUNC_INFO  << preeditStr << replaceSize;
    m_preeditString = preeditStr;
    m_replaceIndex = replaceSize;
}

void QWasmInputContext::insertPreedit(int replaceLength)
{
    qCDebug(qLcQpaWasmInputContext) << Q_FUNC_INFO << m_preeditString;
    if (replaceLength == 0)
        replaceLength = m_preeditString.length();

    QList<QInputMethodEvent::Attribute> attributes;
    {
        QInputMethodEvent::Attribute attr_cursor(QInputMethodEvent::Cursor,
                                                 0,
                                                 1);
        attributes.append(attr_cursor);

        BOBUIextCharFormat format;
        format.setFontUnderline(true);
        format.setUnderlineStyle(BOBUIextCharFormat::SingleUnderline);
        QInputMethodEvent::Attribute attr_format(QInputMethodEvent::TextFormat,
                                                0,
                                                replaceLength, format);
        attributes.append(attr_format);
    }

    QInputMethodEvent e(m_preeditString, attributes);
    if (m_replaceIndex > 0)
        e.setCommitString("", -m_replaceIndex, replaceLength);
    QCoreApplication::sendEvent(m_focusObject, &e);
}

void QWasmInputContext::commitPreeditAndClear()
{
    if (m_preeditString.isEmpty())
        return;
    QInputMethodEvent e;
    e.setCommitString(m_preeditString);
    m_preeditString.clear();
    QCoreApplication::sendEvent(m_focusObject, &e);
}

void QWasmInputContext::insertText(QString inputStr, bool replace)
{ // commitString
    qCDebug(qLcQpaWasmInputContext) << Q_FUNC_INFO << inputStr << replace;
    Q_UNUSED(replace);
    if (!inputStr.isEmpty()) {
        const int replaceLen = 0;
        QInputMethodEvent e;
        e.setCommitString(inputStr, -replaceLen, replaceLen);
        QCoreApplication::sendEvent(m_focusObject, &e);
    }
}

/* This will replace the text in the focusobject at replaceFrom position, and replaceSize length
 with the text in inputStr. */

 void QWasmInputContext::replaceText(QString inputStr, int replaceFrom, int replaceSize)
 {
    qCDebug(qLcQpaWasmInputContext) << Q_FUNC_INFO << inputStr << replaceFrom << replaceSize;

    QList<QInputMethodEvent::Attribute> attributes;
    {
        QInputMethodEvent::Attribute attr_cursor(QInputMethodEvent::Cursor,
                                                0, // start
                                                1); // length
        attributes.append(attr_cursor);

        BOBUIextCharFormat format;
        format.setFontUnderline(true);
        format.setUnderlineStyle(BOBUIextCharFormat::SingleUnderline);
        QInputMethodEvent::Attribute attr_format(QInputMethodEvent::TextFormat,
                                                0,
                                                replaceSize,
                                                format);
        attributes.append(attr_format);
    }

    QInputMethodEvent e1(QString(), attributes);
    e1.setCommitString(inputStr, replaceFrom, replaceSize);
    QCoreApplication::sendEvent(m_focusObject, &e1);

    m_preeditString.clear();
 }

BOBUI_END_NAMESPACE
