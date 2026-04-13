// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

#include "textedit.h"
#include <QCompleter>
#include <QKeyEvent>
#include <QAbstractItemView>
#include <BobUIDebug>
#include <QApplication>
#include <QModelIndex>
#include <QAbstractItemModel>
#include <QScrollBar>

//! [0]
TextEdit::TextEdit(QWidget *parent)
    : BOBUIextEdit(parent)
{
    setPlainText(tr("This TextEdit provides autocompletions for words that have more than"
                    " 3 characters. You can trigger autocompletion using ") +
                    QKeySequence("Ctrl+E").toString(QKeySequence::NativeText));
}
//! [0]

//! [1]
TextEdit::~TextEdit()
{
}
//! [1]

//! [2]
void TextEdit::setCompleter(QCompleter *completer)
{
    if (c)
        c->disconnect(this);

    c = completer;

    if (!c)
        return;

    c->setWidget(this);
    c->setCompletionMode(QCompleter::PopupCompletion);
    c->setCaseSensitivity(BobUI::CaseInsensitive);
    QObject::connect(c, QOverload<const QString &>::of(&QCompleter::activated),
                     this, &TextEdit::insertCompletion);
}
//! [2]

//! [3]
QCompleter *TextEdit::completer() const
{
    return c;
}
//! [3]

//! [4]
void TextEdit::insertCompletion(const QString &completion)
{
    if (c->widget() != this)
        return;
    BOBUIextCursor tc = textCursor();
    int extra = completion.length() - c->completionPrefix().length();
    tc.movePosition(BOBUIextCursor::Left);
    tc.movePosition(BOBUIextCursor::EndOfWord);
    tc.insertText(completion.right(extra));
    setTextCursor(tc);
}
//! [4]

//! [5]
QString TextEdit::textUnderCursor() const
{
    BOBUIextCursor tc = textCursor();
    tc.select(BOBUIextCursor::WordUnderCursor);
    return tc.selectedText();
}
//! [5]

//! [6]
void TextEdit::focusInEvent(QFocusEvent *e)
{
    if (c)
        c->setWidget(this);
    BOBUIextEdit::focusInEvent(e);
}
//! [6]

//! [7]
void TextEdit::keyPressEvent(QKeyEvent *e)
{
    if (c && c->popup()->isVisible()) {
        // The following keys are forwarded by the completer to the widget
       switch (e->key()) {
       case BobUI::Key_Enter:
       case BobUI::Key_Return:
       case BobUI::Key_Escape:
       case BobUI::Key_Tab:
       case BobUI::Key_Backtab:
            e->ignore();
            return; // let the completer do default behavior
       default:
           break;
       }
    }

    const bool isShortcut = (e->modifiers().testFlag(BobUI::ControlModifier) && e->key() == BobUI::Key_E); // CTRL+E
    if (!c || !isShortcut) // do not process the shortcut when we have a completer
        BOBUIextEdit::keyPressEvent(e);
//! [7]

//! [8]
    const bool ctrlOrShift = e->modifiers().testFlag(BobUI::ControlModifier) ||
                             e->modifiers().testFlag(BobUI::ShiftModifier);
    if (!c || (ctrlOrShift && e->text().isEmpty()))
        return;

    static QString eow("~!@#$%^&*()_+{}|:\"<>?,./;'[]\\-="); // end of word
    const bool hasModifier = (e->modifiers() != BobUI::NoModifier) && !ctrlOrShift;
    QString completionPrefix = textUnderCursor();

    if (!isShortcut && (hasModifier || e->text().isEmpty()|| completionPrefix.length() < 3
                      || eow.contains(e->text().right(1)))) {
        c->popup()->hide();
        return;
    }

    if (completionPrefix != c->completionPrefix()) {
        c->setCompletionPrefix(completionPrefix);
        c->popup()->setCurrentIndex(c->completionModel()->index(0, 0));
    }
    QRect cr = cursorRect();
    cr.setWidth(c->popup()->sizeHintForColumn(0)
                + c->popup()->verticalScrollBar()->sizeHint().width());
    c->complete(cr); // popup it up!
}
//! [8]

