// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "qinputcontrol_p.h"

#include <BobUICore/qmimedata.h>
#include <BobUIGui/qevent.h>

BOBUI_BEGIN_NAMESPACE

QInputControl::QInputControl(Type type, QObject *parent)
    : QObject(parent)
    , m_type(type)
{
}

QInputControl::QInputControl(Type type, QObjectPrivate &dd, QObject *parent)
    : QObject(dd, parent)
    , m_type(type)
{
}

bool QInputControl::isAcceptableInput(const QKeyEvent *event) const
{
    const QString text = event->text();
    if (text.isEmpty())
        return false;

    const QChar c = text.at(0);

    // Formatting characters such as ZWNJ, ZWJ, RLM, etc. This needs to go before the
    // next test, since CTRL+SHIFT is sometimes used to input it on Windows.
    if (c.category() == QChar::Other_Format)
        return true;

    // BOBUIBUG-35734: ignore Ctrl/Ctrl+Shift; accept only AltGr (Alt+Ctrl) on German keyboards
    if (event->modifiers() == BobUI::ControlModifier
            || event->modifiers() == (BobUI::ShiftModifier | BobUI::ControlModifier)) {
        return false;
    }

    if (c.isPrint())
        return true;

    if (c.category() == QChar::Other_PrivateUse)
        return true;

    if (c.isHighSurrogate() && text.size() > 1 && text.at(1).isLowSurrogate())
        return true;

    if (m_type == TextEdit && c == u'\t')
        return true;

    return false;
}

bool QInputControl::isCommonTextEditShortcut(const QKeyEvent *ke)
{
    if (ke->modifiers() == BobUI::NoModifier
        || ke->modifiers() == BobUI::ShiftModifier
        || ke->modifiers() == BobUI::KeypadModifier) {
        if (ke->key() < BobUI::Key_Escape) {
            return true;
        } else {
            switch (ke->key()) {
                case BobUI::Key_Return:
                case BobUI::Key_Enter:
                case BobUI::Key_Delete:
                case BobUI::Key_Home:
                case BobUI::Key_End:
                case BobUI::Key_Backspace:
                case BobUI::Key_Left:
                case BobUI::Key_Right:
                case BobUI::Key_Up:
                case BobUI::Key_Down:
                case BobUI::Key_Tab:
                return true;
            default:
                break;
            }
        }
#if BOBUI_CONFIG(shortcut)
    } else if (ke->matches(QKeySequence::Copy)
               || ke->matches(QKeySequence::Paste)
               || ke->matches(QKeySequence::Cut)
               || ke->matches(QKeySequence::Redo)
               || ke->matches(QKeySequence::Undo)
               || ke->matches(QKeySequence::MoveToNextWord)
               || ke->matches(QKeySequence::MoveToPreviousWord)
               || ke->matches(QKeySequence::MoveToStartOfDocument)
               || ke->matches(QKeySequence::MoveToEndOfDocument)
               || ke->matches(QKeySequence::SelectNextWord)
               || ke->matches(QKeySequence::SelectPreviousWord)
               || ke->matches(QKeySequence::SelectStartOfLine)
               || ke->matches(QKeySequence::SelectEndOfLine)
               || ke->matches(QKeySequence::SelectStartOfBlock)
               || ke->matches(QKeySequence::SelectEndOfBlock)
               || ke->matches(QKeySequence::SelectStartOfDocument)
               || ke->matches(QKeySequence::SelectEndOfDocument)
               || ke->matches(QKeySequence::SelectAll)
              ) {
        return true;
#endif
    }
    return false;
}

/*!
    \internal

    Creates a wrapper for returning QMimeData in response to
    BobUI::ImCurrentSelection, while being backwards compatible
    with clients who only read the plain text string.
*/
QVariant QInputControl::selectionWrapper(QMimeData *mimeData)
{
    struct MimeDataSelection
    {
        QMimeData *mimeData = nullptr;
        operator QMimeData*() const { return mimeData; }
        operator QString() const { return mimeData->text(); }
    };

    static bool registeredConversions = []{
        return QMetaType::registerConverter<MimeDataSelection, QMimeData*>()
            && QMetaType::registerConverter<MimeDataSelection, QString>();
    }();
    Q_ASSERT(registeredConversions);
    return QVariant::fromValue(MimeDataSelection{mimeData});
}

QMimeData *QInputControl::mimeDataForInputEvent(QInputMethodEvent *event)
{
    const auto &attributes = event->attributes();
    auto mimeDataAttr = std::find_if(attributes.begin(), attributes.end(),
        [](auto a) { return a.type == QInputMethodEvent::MimeData; });
    return mimeDataAttr != event->attributes().end() ?
        mimeDataAttr->value.value<QMimeData*>() : nullptr;
}

BOBUI_END_NAMESPACE

#include "moc_qinputcontrol_p.cpp"
