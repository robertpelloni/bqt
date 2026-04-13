// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only


#include <BOBUIest>
#include <QSignalSpy>

#include <bobuiextedit.h>
#include <bobuiextcursor.h>
#include <bobuiextlist.h>
#include <qdebug.h>
#include <qapplication.h>
#include <qclipboard.h>
#include <bobuiextbrowser.h>
#include <private/qwidgettextcontrol_p.h>
#include <private/qplaintextedit_p.h>
#include <qscrollbar.h>
#include <bobuiextobject.h>
#include <qmenu.h>

#include <qabstracttextdocumentlayout.h>
#include <bobuiextdocumentfragment.h>

#include "qplaintextedit.h"
#include "../../../shared/platformclipboard.h"

//Used in copyAvailable
using keyPairType = std::pair<BobUI::Key, BobUI::KeyboardModifier>;
typedef QList<keyPairType> pairListType;
Q_DECLARE_METATYPE(keyPairType);

BOBUI_FORWARD_DECLARE_CLASS(QPlainTextEdit)

class tst_QPlainTextEdit : public QObject
{
    Q_OBJECT
public:
    tst_QPlainTextEdit();

public slots:
    void init();
    void cleanup();
private slots:
    void getSetCheck();
#ifndef BOBUI_NO_CLIPBOARD
    void clearMustNotChangeClipboard();
#endif
    void clearMustNotResetRootFrameMarginToDefault();
    void paragSeparatorOnPlaintextAppend();
#ifndef BOBUI_NO_CLIPBOARD
    void selectAllSetsNotSelection();
#endif
    void asciiTab();
    void setDocument();
    void emptyAppend();
    void appendOnEmptyDocumentShouldReuseInitialParagraph();
    void cursorPositionChanged();
    void setTextCursor();
#ifndef BOBUI_NO_CLIPBOARD
    void undoAvailableAfterPaste();
#endif
    void undoRedoAvailableRepetition();
    void appendShouldNotTouchTheSelection();
    void backspace();
    void shiftBackspace();
    void undoRedo();
    void preserveCharFormatInAppend();
#ifndef BOBUI_NO_CLIPBOARD
    void copyAndSelectAllInReadonly();
#endif
    void charWithAltOrCtrlModifier_data();
    void charWithAltOrCtrlModifier();
    void noPropertiesOnDefaultTextEditCharFormat();
    void setPlainTextShouldEmitTextChangedOnce();
    void overwriteMode();
    void shiftDownInLineLastShouldSelectToEnd_data();
    void shiftDownInLineLastShouldSelectToEnd();
    void undoRedoShouldRepositionTextEditCursor();
    void lineWrapModes();
#ifndef BOBUI_NO_CURSOR
    void mouseCursorShape();
#endif
    void implicitClear();
    void undoRedoAfterSetContent();
    void numPadKeyNavigation();
    void moveCursor();
#ifndef BOBUI_NO_CLIPBOARD
    void mimeDataReimplementations();
#endif
    void shiftEnterShouldInsertLineSeparator();
    void selectWordsFromStringsContainingSeparators_data();
    void selectWordsFromStringsContainingSeparators();
#ifndef BOBUI_NO_CLIPBOARD
    void canPaste();
    void copyAvailable_data();
    void copyAvailable();
#endif
    void ensureCursorVisibleOnInitialShow();
    void setTextInsideResizeEvent();
    void colorfulAppend();
    void ensureVisibleWithRtl();
    void preserveCharFormatAfterSetPlainText();
    void extraSelections();
    void adjustScrollbars();
    void textObscuredByScrollbars();
    void setTextPreservesUndoRedoEnabled();
    void wordWrapProperty();
    void lineWrapProperty();
    void selectionChanged();
    void blockCountChanged();
    void insertAndScrollToBottom();
    void inputMethodQueryImHints_data();
    void inputMethodQueryImHints();
#if BOBUI_CONFIG(regularexpression)
    void findWithRegularExpression();
    void findBackwardWithRegularExpression();
    void findWithRegularExpressionReturnsFalseIfNoMoreResults();
#endif
    void layoutAfterMultiLineRemove();
    void undoCommandRemovesAndReinsertsBlock();
    void taskBOBUIBUG_43562_lineCountCrash();
#if !defined(BOBUI_NO_CONTEXTMENU) && !defined(BOBUI_NO_CLIPBOARD)
    void contextMenu();
#endif
    void inputMethodCursorRect();
#if BOBUI_CONFIG(scrollbar)
    void updateAfterChangeCenterOnScroll();
#endif
#ifndef BOBUI_NO_CLIPBOARD
    void updateCursorPositionAfterEdit();
#endif
    void appendTextWhenInvisible();
    void placeholderVisibility_data();
    void placeholderVisibility();
    void scrollBarSignals();
    void dontCrashWithCss();

private:
    void createSelection();
    int blockCount() const;
    int lineCount() const;

    QPlainTextEdit *ed;
    qreal rootFrameMargin;
};

// Testing get/set functions
void tst_QPlainTextEdit::getSetCheck()
{
    QPlainTextEdit obj1;
    // BOBUIextDocument * QPlainTextEdit::document()
    // void QPlainTextEdit::setDocument(BOBUIextDocument *)
    BOBUIextDocument *var1 = new BOBUIextDocument;
    var1->setDocumentLayout(new QPlainTextDocumentLayout(var1));
    obj1.setDocument(var1);
    QCOMPARE(var1, obj1.document());
    obj1.setDocument((BOBUIextDocument *)0);
    QVERIFY(var1 != obj1.document()); // QPlainTextEdit creates a new document when setting 0
    QVERIFY((BOBUIextDocument *)0 != obj1.document());
    delete var1;


    // bool QPlainTextEdit::tabChangesFocus()
    // void QPlainTextEdit::setTabChangesFocus(bool)
    obj1.setTabChangesFocus(false);
    QCOMPARE(false, obj1.tabChangesFocus());
    obj1.setTabChangesFocus(true);
    QCOMPARE(true, obj1.tabChangesFocus());

    // LineWrapMode QPlainTextEdit::lineWrapMode()
    // void QPlainTextEdit::setLineWrapMode(LineWrapMode)
    obj1.setLineWrapMode(QPlainTextEdit::LineWrapMode(QPlainTextEdit::NoWrap));
    QCOMPARE(QPlainTextEdit::LineWrapMode(QPlainTextEdit::NoWrap), obj1.lineWrapMode());
    obj1.setLineWrapMode(QPlainTextEdit::LineWrapMode(QPlainTextEdit::WidgetWidth));
    QCOMPARE(QPlainTextEdit::LineWrapMode(QPlainTextEdit::WidgetWidth), obj1.lineWrapMode());
//     obj1.setLineWrapMode(QPlainTextEdit::LineWrapMode(QPlainTextEdit::FixedPixelWidth));
//     QCOMPARE(QPlainTextEdit::LineWrapMode(QPlainTextEdit::FixedPixelWidth), obj1.lineWrapMode());
//     obj1.setLineWrapMode(QPlainTextEdit::LineWrapMode(QPlainTextEdit::FixedColumnWidth));
//     QCOMPARE(QPlainTextEdit::LineWrapMode(QPlainTextEdit::FixedColumnWidth), obj1.lineWrapMode());


    // bool QPlainTextEdit::overwriteMode()
    // void QPlainTextEdit::setOverwriteMode(bool)
    obj1.setOverwriteMode(false);
    QCOMPARE(false, obj1.overwriteMode());
    obj1.setOverwriteMode(true);
    QCOMPARE(true, obj1.overwriteMode());

    // int QPlainTextEdit::tabStopWidth()
    // void QPlainTextEdit::setTabStopWidth(int)
    obj1.setTabStopDistance(0);
    QCOMPARE(0, obj1.tabStopDistance());
    obj1.setTabStopDistance(-1);
    QCOMPARE(0, obj1.tabStopDistance()); // Makes no sense to set a negative tabstop value
    obj1.setTabStopDistance(std::numeric_limits<qreal>::max());
    QCOMPARE(std::numeric_limits<qreal>::max(), obj1.tabStopDistance());
}

class BobUITestDocumentLayout : public QAbstractTextDocumentLayout
{
    Q_OBJECT
public:
    inline BobUITestDocumentLayout(QPlainTextEdit *edit, BOBUIextDocument *doc, int &itCount)
        : QAbstractTextDocumentLayout(doc), useBiggerSize(false), ed(edit), iterationCounter(itCount) {}

    virtual void draw(QPainter *, const QAbstractTextDocumentLayout::PaintContext &) override {}

    virtual int hitTest(const QPointF &, BobUI::HitTestAccuracy ) const override { return 0; }

    virtual void documentChanged(int, int, int) override {}

    virtual int pageCount() const override { return 1; }

    virtual QSizeF documentSize() const override { return usedSize; }

    virtual QRectF frameBoundingRect(BOBUIextFrame *) const override { return QRectF(); }
    virtual QRectF blockBoundingRect(const BOBUIextBlock &) const override { return QRectF(); }

    bool useBiggerSize;
    QSize usedSize;

    QPlainTextEdit *ed;

    int &iterationCounter;
};

tst_QPlainTextEdit::tst_QPlainTextEdit()
{}

void tst_QPlainTextEdit::init()
{
    ed = new QPlainTextEdit(0);
    rootFrameMargin = ed->document()->documentMargin();
}

void tst_QPlainTextEdit::cleanup()
{
    delete ed;
    ed = 0;
}


void tst_QPlainTextEdit::createSelection()
{
    BOBUIest::keyClicks(ed, "Hello World");
    /* go to start */
#ifndef Q_OS_MAC
    BOBUIest::keyClick(ed, BobUI::Key_Home, BobUI::ControlModifier);
#else
    BOBUIest::keyClick(ed, BobUI::Key_Home);
#endif
    QCOMPARE(ed->textCursor().position(), 0);
    /* select until end of text */
#ifndef Q_OS_MAC
    BOBUIest::keyClick(ed, BobUI::Key_End, BobUI::ControlModifier | BobUI::ShiftModifier);
#else
    BOBUIest::keyClick(ed, BobUI::Key_End, BobUI::ShiftModifier);
#endif
    QCOMPARE(ed->textCursor().position(), 11);
}
#ifndef BOBUI_NO_CLIPBOARD
void tst_QPlainTextEdit::clearMustNotChangeClipboard()
{
    if (!PlatformClipboard::isAvailable())
        QSKIP("Clipboard not working with cron-started unit tests");

    if (QGuiApplication::platformName().startsWith(QLatin1String("wayland"), BobUI::CaseInsensitive))
        QSKIP("Wayland: This fails. Figure out why.");

    ed->textCursor().insertText("Hello World");
    QString txt("This is different text");
    QApplication::clipboard()->setText(txt);
    ed->clear();
    QCOMPARE(QApplication::clipboard()->text(), txt);
}
#endif

void tst_QPlainTextEdit::clearMustNotResetRootFrameMarginToDefault()
{
    QCOMPARE(ed->document()->rootFrame()->frameFormat().margin(), rootFrameMargin);
    ed->clear();
    QCOMPARE(ed->document()->rootFrame()->frameFormat().margin(), rootFrameMargin);
}


void tst_QPlainTextEdit::paragSeparatorOnPlaintextAppend()
{
    ed->appendPlainText("Hello\nWorld");
    int cnt = 0;
    BOBUIextBlock blk = ed->document()->begin();
    while (blk.isValid()) {
        ++cnt;
        blk = blk.next();
    }
    QCOMPARE(cnt, 2);
}

#ifndef BOBUI_NO_CLIPBOARD
void tst_QPlainTextEdit::selectAllSetsNotSelection()
{
    if (!QApplication::clipboard()->supportsSelection())
        QSKIP("Test only relevant for systems with selection");

    QApplication::clipboard()->setText(QString("foobar"), QClipboard::Selection);
    QCOMPARE(QApplication::clipboard()->text(QClipboard::Selection), QString("foobar"));

    ed->insertPlainText("Hello World");
    ed->selectAll();

    QCOMPARE(QApplication::clipboard()->text(QClipboard::Selection), QString::fromLatin1("foobar"));
}
#endif

void tst_QPlainTextEdit::asciiTab()
{
    QPlainTextEdit edit;
    edit.setPlainText("\t");
    edit.show();
    qApp->processEvents();
    QCOMPARE(edit.toPlainText().at(0), QChar('\t'));
}

void tst_QPlainTextEdit::setDocument()
{
    BOBUIextDocument *document = new BOBUIextDocument(ed);
    document->setDocumentLayout(new QPlainTextDocumentLayout(document));
    BOBUIextCursor(document).insertText("Test");
    ed->setDocument(document);
    QCOMPARE(ed->toPlainText(), QString("Test"));
}


int tst_QPlainTextEdit::blockCount() const
{
    int blocks = 0;
    for (BOBUIextBlock block = ed->document()->begin(); block.isValid(); block = block.next())
        ++blocks;
    return blocks;
}

int tst_QPlainTextEdit::lineCount() const
{
    int lines = 0;
    for (BOBUIextBlock block = ed->document()->begin(); block.isValid(); block = block.next()) {
        ed->document()->documentLayout()->blockBoundingRect(block);
        lines += block.layout()->lineCount();
    }
    return lines;
}

// Supporter issue #56783
void tst_QPlainTextEdit::emptyAppend()
{
    ed->appendPlainText("Blah");
    QCOMPARE(blockCount(), 1);
    ed->appendPlainText(QString());
    QCOMPARE(blockCount(), 2);
    ed->appendPlainText(QString("   "));
    QCOMPARE(blockCount(), 3);
}

void tst_QPlainTextEdit::appendOnEmptyDocumentShouldReuseInitialParagraph()
{
    QCOMPARE(blockCount(), 1);
    ed->appendPlainText("Blah");
    QCOMPARE(blockCount(), 1);
}


class CursorPositionChangedRecorder : public QObject
{
    Q_OBJECT
public:
    inline CursorPositionChangedRecorder(QPlainTextEdit *ed)
        : editor(ed)
    {
        connect(editor, SIGNAL(cursorPositionChanged()), this, SLOT(recordCursorPos()));
    }

    QList<int> cursorPositions;

private slots:
    void recordCursorPos()
    {
        cursorPositions.append(editor->textCursor().position());
    }

private:
    QPlainTextEdit *editor;
};

void tst_QPlainTextEdit::cursorPositionChanged()
{
    QSignalSpy spy(ed, SIGNAL(cursorPositionChanged()));

    spy.clear();
    BOBUIest::keyClick(ed, BobUI::Key_A);
    QCOMPARE(spy.size(), 1);

    BOBUIextCursor cursor = ed->textCursor();
    cursor.movePosition(BOBUIextCursor::Start);
    ed->setTextCursor(cursor);
    cursor.movePosition(BOBUIextCursor::End);
    spy.clear();
    cursor.insertText("Test");
    QCOMPARE(spy.size(), 0);

    cursor.movePosition(BOBUIextCursor::End);
    ed->setTextCursor(cursor);
    cursor.movePosition(BOBUIextCursor::Start);
    spy.clear();
    cursor.insertText("Test");
    QCOMPARE(spy.size(), 1);

    spy.clear();
    BOBUIest::keyClick(ed, BobUI::Key_Left);
    QCOMPARE(spy.size(), 1);

    CursorPositionChangedRecorder spy2(ed);
    QVERIFY(ed->textCursor().position() > 0);
    ed->setPlainText("Hello World");
    QCOMPARE(spy2.cursorPositions.size(), 1);
    QCOMPARE(spy2.cursorPositions.at(0), 0);
    QCOMPARE(ed->textCursor().position(), 0);
}

void tst_QPlainTextEdit::setTextCursor()
{
    QSignalSpy spy(ed, SIGNAL(cursorPositionChanged()));

    ed->setPlainText("Test");
    BOBUIextCursor cursor = ed->textCursor();
    cursor.movePosition(BOBUIextCursor::Start);
    cursor.movePosition(BOBUIextCursor::NextCharacter);

    spy.clear();

    ed->setTextCursor(cursor);
    QCOMPARE(spy.size(), 1);
}

#ifndef BOBUI_NO_CLIPBOARD
void tst_QPlainTextEdit::undoAvailableAfterPaste()
{
    if (!PlatformClipboard::isAvailable())
        QSKIP("Clipboard not working with cron-started unit tests");

    if (QGuiApplication::platformName().startsWith(QLatin1String("wayland"), BobUI::CaseInsensitive))
        QSKIP("Wayland: This fails. Figure out why.");

    QSignalSpy spy(ed->document(), SIGNAL(undoAvailable(bool)));

    const QString txt("Test");
    QApplication::clipboard()->setText(txt);
    ed->paste();
    QVERIFY(spy.size() >= 1);
    QCOMPARE(ed->toPlainText(), txt);
}
#endif

class UndoRedoRecorder : public QObject
{
    Q_OBJECT
public:
    UndoRedoRecorder(BOBUIextDocument *doc)
        : undoRepetitions(false)
        , redoRepetitions(false)
        , undoCount(0)
        , redoCount(0)
    {
        connect(doc, SIGNAL(undoAvailable(bool)), this, SLOT(undoAvailable(bool)));
        connect(doc, SIGNAL(redoAvailable(bool)), this, SLOT(redoAvailable(bool)));
    }

    bool undoRepetitions;
    bool redoRepetitions;

private slots:
    void undoAvailable(bool enabled) {
        if (undoCount > 0 && enabled == lastUndoEnabled)
            undoRepetitions = true;

        ++undoCount;
        lastUndoEnabled = enabled;
    }

    void redoAvailable(bool enabled) {
        if (redoCount > 0 && enabled == lastRedoEnabled)
            redoRepetitions = true;

        ++redoCount;
        lastRedoEnabled = enabled;
    }

private:
    bool lastUndoEnabled;
    bool lastRedoEnabled;

    int undoCount;
    int redoCount;
};

void tst_QPlainTextEdit::undoRedoAvailableRepetition()
{
    UndoRedoRecorder spy(ed->document());

    ed->textCursor().insertText("ABC\n\nDEF\n\nGHI\n");
    ed->textCursor().insertText("foo\n");
    ed->textCursor().insertText("bar\n");
    ed->undo(); ed->undo(); ed->undo();
    ed->redo(); ed->redo(); ed->redo();

    QVERIFY(!spy.undoRepetitions);
    QVERIFY(!spy.redoRepetitions);
}

void tst_QPlainTextEdit::appendShouldNotTouchTheSelection()
{
    BOBUIextCursor cursor(ed->document());
    BOBUIextCharFormat fmt;
    fmt.setForeground(BobUI::blue);
    cursor.insertText("H", fmt);
    fmt.setForeground(BobUI::red);
    cursor.insertText("ey", fmt);

    cursor.insertText("some random text inbetween");

    cursor.movePosition(BOBUIextCursor::Start);
    cursor.movePosition(BOBUIextCursor::NextCharacter, BOBUIextCursor::KeepAnchor);
    QCOMPARE(cursor.charFormat().foreground().color(), QColor(BobUI::blue));
    cursor.movePosition(BOBUIextCursor::NextCharacter, BOBUIextCursor::KeepAnchor);
    QCOMPARE(cursor.charFormat().foreground().color(), QColor(BobUI::red));
    cursor.movePosition(BOBUIextCursor::NextCharacter, BOBUIextCursor::KeepAnchor);
    QCOMPARE(cursor.charFormat().foreground().color(), QColor(BobUI::red));
    QCOMPARE(cursor.selectedText(), QString("Hey"));

    ed->setTextCursor(cursor);
    QVERIFY(ed->textCursor().hasSelection());

    ed->appendHtml("<b>Some Bold Text</b>");
    cursor.movePosition(BOBUIextCursor::Start);
    cursor.movePosition(BOBUIextCursor::NextCharacter);
    QCOMPARE(cursor.charFormat().foreground().color(), QColor(BobUI::blue));
}

void tst_QPlainTextEdit::backspace()
{
    BOBUIextCursor cursor = ed->textCursor();

    BOBUIextListFormat listFmt;
    listFmt.setStyle(BOBUIextListFormat::ListDisc);
    listFmt.setIndent(1);
    cursor.insertList(listFmt);
    cursor.insertText("A");

    ed->setTextCursor(cursor);

    // delete 'A'
    BOBUIest::keyClick(ed, BobUI::Key_Backspace);
    QVERIFY(ed->textCursor().currentList());
    // delete list
    BOBUIest::keyClick(ed, BobUI::Key_Backspace);
    QVERIFY(!ed->textCursor().currentList());
    QCOMPARE(ed->textCursor().blockFormat().indent(), 1);
    // outdent paragraph
    BOBUIest::keyClick(ed, BobUI::Key_Backspace);
    QCOMPARE(ed->textCursor().blockFormat().indent(), 0);
}

void tst_QPlainTextEdit::shiftBackspace()
{
    BOBUIextCursor cursor = ed->textCursor();

    BOBUIextListFormat listFmt;
    listFmt.setStyle(BOBUIextListFormat::ListDisc);
    listFmt.setIndent(1);
    cursor.insertList(listFmt);
    cursor.insertText("A");

    ed->setTextCursor(cursor);

    // delete 'A'
    BOBUIest::keyClick(ed, BobUI::Key_Backspace, BobUI::ShiftModifier);
    QVERIFY(ed->textCursor().currentList());
    // delete list
    BOBUIest::keyClick(ed, BobUI::Key_Backspace, BobUI::ShiftModifier);
    QVERIFY(!ed->textCursor().currentList());
    QCOMPARE(ed->textCursor().blockFormat().indent(), 1);
    // outdent paragraph
    BOBUIest::keyClick(ed, BobUI::Key_Backspace, BobUI::ShiftModifier);
    QCOMPARE(ed->textCursor().blockFormat().indent(), 0);
}

void tst_QPlainTextEdit::undoRedo()
{
    ed->clear();
    BOBUIest::keyClicks(ed, "abc d");
    QCOMPARE(ed->toPlainText(), QString("abc d"));
    ed->undo();
    QCOMPARE(ed->toPlainText(), QString());
    ed->redo();
    QCOMPARE(ed->toPlainText(), QString("abc d"));
#ifdef Q_OS_WIN
    // shortcut for undo
    BOBUIest::keyClick(ed, BobUI::Key_Backspace, BobUI::AltModifier);
    QCOMPARE(ed->toPlainText(), QString());
    // shortcut for redo
    BOBUIest::keyClick(ed, BobUI::Key_Backspace, BobUI::ShiftModifier|BobUI::AltModifier);
    QCOMPARE(ed->toPlainText(), QString("abc d"));
#endif
}

// Task #70465
void tst_QPlainTextEdit::preserveCharFormatInAppend()
{
    ed->appendHtml("First para");
    ed->appendHtml("<b>Second para</b>");
    ed->appendHtml("third para");

    BOBUIextCursor cursor(ed->textCursor());

    cursor.movePosition(BOBUIextCursor::Start);
    cursor.movePosition(BOBUIextCursor::NextCharacter);
    QCOMPARE(cursor.charFormat().fontWeight(), (int)QFont::Normal);
    QCOMPARE(cursor.block().text(), QString("First para"));

    cursor.movePosition(BOBUIextCursor::NextBlock);
    cursor.movePosition(BOBUIextCursor::NextCharacter);
    QCOMPARE(cursor.charFormat().fontWeight(), (int)QFont::Bold);
    QCOMPARE(cursor.block().text(), QString("Second para"));

    cursor.movePosition(BOBUIextCursor::NextBlock);
    cursor.movePosition(BOBUIextCursor::NextCharacter);
    QCOMPARE(cursor.charFormat().fontWeight(), (int)QFont::Normal);
    QCOMPARE(cursor.block().text(), QString("third para"));
}

#ifndef BOBUI_NO_CLIPBOARD
void tst_QPlainTextEdit::copyAndSelectAllInReadonly()
{
    if (!PlatformClipboard::isAvailable())
        QSKIP("Clipboard not working with cron-started unit tests");

    if (QGuiApplication::platformName().startsWith(QLatin1String("wayland"), BobUI::CaseInsensitive))
        QSKIP("Wayland: This fails. Figure out why.");

    ed->setReadOnly(true);
    ed->setPlainText("Hello World");

    BOBUIextCursor cursor = ed->textCursor();
    cursor.clearSelection();
    ed->setTextCursor(cursor);
    QVERIFY(!ed->textCursor().hasSelection());

    QCOMPARE(ed->toPlainText(), QString("Hello World"));

    // shouldn't do anything
    BOBUIest::keyClick(ed, BobUI::Key_A);

    QCOMPARE(ed->toPlainText(), QString("Hello World"));

    BOBUIest::keyClick(ed, BobUI::Key_A, BobUI::ControlModifier);

    QVERIFY(ed->textCursor().hasSelection());

    QApplication::clipboard()->setText(QString());
    QVERIFY(QApplication::clipboard()->text().isEmpty());

    BOBUIest::keyClick(ed, BobUI::Key_C, BobUI::ControlModifier);
    QCOMPARE(QApplication::clipboard()->text(), QString("Hello World"));
}
#endif

Q_DECLARE_METATYPE(BobUI::KeyboardModifiers)

// Test how QWidgetTextControlPrivate (used in QPlainTextEdit, BOBUIextEdit)
// handles input with modifiers.
void tst_QPlainTextEdit::charWithAltOrCtrlModifier_data()
{
    BOBUIest::addColumn<BobUI::KeyboardModifiers>("modifiers");
    BOBUIest::addColumn<bool>("textExpected");

    BOBUIest::newRow("no-modifiers") << BobUI::KeyboardModifiers() << true;
    // Ctrl, Ctrl+Shift: No text (BOBUIBUG-35734)
    BOBUIest::newRow("ctrl") << BobUI::KeyboardModifiers(BobUI::ControlModifier)
        << false;
    BOBUIest::newRow("ctrl-shift") << BobUI::KeyboardModifiers(BobUI::ShiftModifier | BobUI::ControlModifier)
        << false;
    BOBUIest::newRow("alt") << BobUI::KeyboardModifiers(BobUI::AltModifier) << true;
    // Alt-Ctrl (Alt-Gr on German keyboards, Task 129098): Expect text
    BOBUIest::newRow("alt-ctrl") << (BobUI::AltModifier | BobUI::ControlModifier) << true;
}

void tst_QPlainTextEdit::charWithAltOrCtrlModifier()
{
    QFETCH(BobUI::KeyboardModifiers, modifiers);
    QFETCH(bool, textExpected);

    BOBUIest::keyClick(ed, BobUI::Key_At, modifiers);
    const QString expectedText = textExpected ?  QLatin1String("@") : QString();
    QCOMPARE(ed->toPlainText(), expectedText);
}

void tst_QPlainTextEdit::noPropertiesOnDefaultTextEditCharFormat()
{
    // there should be no properties set on the default/initial char format
    // on a text edit. Font properties instead should be taken from the
    // widget's font (in sync with defaultFont property in document) and the
    // foreground color should be taken from the palette.
    QCOMPARE(ed->textCursor().charFormat().properties().size(), 0);
}

void tst_QPlainTextEdit::setPlainTextShouldEmitTextChangedOnce()
{
    QSignalSpy spy(ed, SIGNAL(textChanged()));
    ed->setPlainText("Yankee Doodle");
    QCOMPARE(spy.size(), 1);
    ed->setPlainText("");
    QCOMPARE(spy.size(), 2);
}

void tst_QPlainTextEdit::overwriteMode()
{
    QVERIFY(!ed->overwriteMode());
    BOBUIest::keyClicks(ed, "Some first text");

    QCOMPARE(ed->toPlainText(), QString("Some first text"));

    ed->setOverwriteMode(true);

    BOBUIextCursor cursor = ed->textCursor();
    cursor.setPosition(5);
    ed->setTextCursor(cursor);

    BOBUIest::keyClicks(ed, "shiny");
    QCOMPARE(ed->toPlainText(), QString("Some shiny text"));

    cursor.movePosition(BOBUIextCursor::End);
    ed->setTextCursor(cursor);

    BOBUIest::keyClick(ed, BobUI::Key_Enter);

    ed->setOverwriteMode(false);
    BOBUIest::keyClicks(ed, "Second paragraph");

    QCOMPARE(blockCount(), 2);

    cursor.movePosition(BOBUIextCursor::Start);
    cursor.movePosition(BOBUIextCursor::EndOfBlock);

    QCOMPARE(cursor.position(), 15);
    ed->setTextCursor(cursor);

    ed->setOverwriteMode(true);

    BOBUIest::keyClicks(ed, " blah");

    QCOMPARE(blockCount(), 2);

    BOBUIextBlock block = ed->document()->begin();
    QCOMPARE(block.text(), QString("Some shiny text blah"));
    block = block.next();
    QCOMPARE(block.text(), QString("Second paragraph"));
}

void tst_QPlainTextEdit::shiftDownInLineLastShouldSelectToEnd_data()
{
    // shift cursor-down in the last line should select to the end of the document

    BOBUIest::addColumn<QString>("input");
    BOBUIest::addColumn<int>("totalLineCount");

    BOBUIest::newRow("1") << QString("Foo\nBar") << 2;
    BOBUIest::newRow("2") << QString("Foo\nBar") + QChar(QChar::LineSeparator) + QString("Baz") << 3;
}

void tst_QPlainTextEdit::shiftDownInLineLastShouldSelectToEnd()
{
    QFETCH(QString, input);
    QFETCH(int, totalLineCount);

    ed->setPlainText(input);
    ed->show();

    // ensure we're layouted
    for (BOBUIextBlock block = ed->document()->begin(); block.isValid(); block = block.next())
        ed->document()->documentLayout()->blockBoundingRect(block);

    QCOMPARE(blockCount(), 2);

    int lineCount = 0;
    for (BOBUIextBlock block = ed->document()->begin(); block.isValid(); block = block.next())
        lineCount += block.layout()->lineCount();
    QCOMPARE(lineCount, totalLineCount);

    BOBUIextCursor cursor = ed->textCursor();
    cursor.movePosition(BOBUIextCursor::Start);
    ed->setTextCursor(cursor);

    for (int i = 0; i < lineCount; ++i) {
        BOBUIest::keyClick(ed, BobUI::Key_Down, BobUI::ShiftModifier);
    }

    input.replace(QLatin1Char('\n'), QChar(QChar::ParagraphSeparator));
    QCOMPARE(ed->textCursor().selectedText(), input);
    QVERIFY(ed->textCursor().atEnd());

    // also test that without shift modifier the cursor does not move to the end
    // for Key_Down in the last line
    cursor.movePosition(BOBUIextCursor::Start);
    ed->setTextCursor(cursor);
    for (int i = 0; i < lineCount; ++i) {
        BOBUIest::keyClick(ed, BobUI::Key_Down);
    }
    QVERIFY(!ed->textCursor().atEnd());
}

void tst_QPlainTextEdit::undoRedoShouldRepositionTextEditCursor()
{
    ed->setPlainText("five\nlines\nin\nthis\ntextedit");
    BOBUIextCursor cursor = ed->textCursor();
    cursor.movePosition(BOBUIextCursor::Start);

    ed->setUndoRedoEnabled(false);
    ed->setUndoRedoEnabled(true);

    QVERIFY(!ed->document()->isUndoAvailable());
    QVERIFY(!ed->document()->isRedoAvailable());

    cursor.insertText("Blah");

    QVERIFY(ed->document()->isUndoAvailable());
    QVERIFY(!ed->document()->isRedoAvailable());

    cursor.movePosition(BOBUIextCursor::End);
    ed->setTextCursor(cursor);

    QVERIFY(QMetaObject::invokeMethod(ed, "undo"));

    QVERIFY(!ed->document()->isUndoAvailable());
    QVERIFY(ed->document()->isRedoAvailable());

    QCOMPARE(ed->textCursor().position(), 0);

    cursor.movePosition(BOBUIextCursor::End);
    ed->setTextCursor(cursor);

    QVERIFY(QMetaObject::invokeMethod(ed, "redo"));

    QVERIFY(ed->document()->isUndoAvailable());
    QVERIFY(!ed->document()->isRedoAvailable());

    QCOMPARE(ed->textCursor().position(), 4);
}

void tst_QPlainTextEdit::lineWrapModes()
{
    QWidget *window = new QWidget;
    ed->setParent(window);
    window->show();
    ed->show();
    ed->setPlainText("a b c d e f g h i j k l m n o p q r s t u v w x y z");
    ed->setLineWrapMode(QPlainTextEdit::NoWrap);
    QCOMPARE(lineCount(), 1);
    ed->setLineWrapMode(QPlainTextEdit::WidgetWidth);

    // QPlainTextEdit does lazy line layout on resize, only for the visible blocks.
    // We thus need to make it wide enough to show something visible.
    int minimumWidth = 2 * ed->document()->documentMargin();
    minimumWidth += ed->fontMetrics().horizontalAdvance(QLatin1Char('a'));
    minimumWidth += ed->frameWidth();
    ed->resize(minimumWidth, 1000);
    QCOMPARE(lineCount(), 26);
    ed->setParent(0);
    delete window;
}

#ifndef BOBUI_NO_CURSOR
void tst_QPlainTextEdit::mouseCursorShape()
{
    // always show an IBeamCursor, see change 170146
    QVERIFY(!ed->isReadOnly());
    QCOMPARE(ed->viewport()->cursor().shape(), BobUI::IBeamCursor);

    ed->setReadOnly(true);
    QCOMPARE(ed->viewport()->cursor().shape(), BobUI::IBeamCursor);

    ed->setPlainText("Foo");
    QCOMPARE(ed->viewport()->cursor().shape(), BobUI::IBeamCursor);
}
#endif

void tst_QPlainTextEdit::implicitClear()
{
    // test that QPlainTextEdit::setHtml, etc. avoid calling clear() but instead call
    // BOBUIextDocument::setHtml/etc. instead, which also clear the contents and
    // cached resource but preserve manually added resources. setHtml on a textedit
    // should behave the same as on a document with respect to that.
    // see also clearResources() autotest in bobuiextdocument

    // regular resource for BOBUIextDocument
    QUrl testUrl(":/foobar");
    QVariant testResource("hello world");

    ed->document()->addResource(BOBUIextDocument::ImageResource, testUrl, testResource);
    QVERIFY(ed->document()->resource(BOBUIextDocument::ImageResource, testUrl) == testResource);

    ed->setPlainText("Blah");
    QVERIFY(ed->document()->resource(BOBUIextDocument::ImageResource, testUrl) == testResource);

    ed->setPlainText("<b>Blah</b>");
    QVERIFY(ed->document()->resource(BOBUIextDocument::ImageResource, testUrl) == testResource);

    ed->clear();
    QVERIFY(!ed->document()->resource(BOBUIextDocument::ImageResource, testUrl).isValid());
    QVERIFY(ed->toPlainText().isEmpty());
}

#ifndef BOBUI_NO_CLIPBOARD
void tst_QPlainTextEdit::copyAvailable_data()
{
    BOBUIest::addColumn<pairListType>("keystrokes");
    BOBUIest::addColumn<QList<bool> >("copyAvailable");
    BOBUIest::addColumn<QString>("function");

    QList<bool> copyAvailable;

    pairListType keystrokes = {{BobUI::Key_B, BobUI::NoModifier},
                               {BobUI::Key_B, BobUI::NoModifier},
                               {BobUI::Key_Left, BobUI::ShiftModifier}};
    copyAvailable << true ;
    BOBUIest::newRow(QString("Case1 B,B, <- + shift | signals: true").toLatin1())
        << keystrokes << copyAvailable << QString();

    copyAvailable.clear();

    keystrokes = {{BobUI::Key_T, BobUI::NoModifier},
                  {BobUI::Key_A, BobUI::NoModifier},
                  {BobUI::Key_A, BobUI::NoModifier},
                  {BobUI::Key_Left, BobUI::ShiftModifier}};
    copyAvailable << true << false;
    BOBUIest::newRow(QString("Case2 T,A,A, <- + shift, cut() | signals: true, false").toLatin1())
        << keystrokes << copyAvailable << QString("cut");

    copyAvailable.clear();

    keystrokes = {{BobUI::Key_T, BobUI::NoModifier},
                  {BobUI::Key_A, BobUI::NoModifier},
                  {BobUI::Key_A, BobUI::NoModifier},
                  {BobUI::Key_Left, BobUI::ShiftModifier},
                  {BobUI::Key_Left, BobUI::ShiftModifier},
                  {BobUI::Key_Left, BobUI::ShiftModifier}};
    copyAvailable << true;
    BOBUIest::newRow(QString("Case3 T,A,A, <- + shift, <- + shift, <- + shift, copy() | signals: true").toLatin1())
        << keystrokes << copyAvailable << QString("copy");

    copyAvailable.clear();

    keystrokes = {{BobUI::Key_T, BobUI::NoModifier},
                  {BobUI::Key_A, BobUI::NoModifier},
                  {BobUI::Key_A, BobUI::NoModifier},
                  {BobUI::Key_Left, BobUI::ShiftModifier},
                  {BobUI::Key_Left, BobUI::ShiftModifier},
                  {BobUI::Key_Left, BobUI::ShiftModifier},
                  {BobUI::Key_X, BobUI::ControlModifier}};
    copyAvailable << true << false;
    BOBUIest::newRow(QString("Case4 T,A,A, <- + shift, <- + shift, <- + shift, ctrl + x, paste() | signals: true, false").toLatin1())
        << keystrokes << copyAvailable << QString("paste");

    copyAvailable.clear();

    keystrokes = {{BobUI::Key_B, BobUI::NoModifier},
                  {BobUI::Key_B, BobUI::NoModifier},
                  {BobUI::Key_Left, BobUI::ShiftModifier},
                  {BobUI::Key_Left, BobUI::NoModifier}};
    copyAvailable << true << false;
    BOBUIest::newRow(QString("Case5 B,B, <- + shift, <- | signals: true, false").toLatin1())
        << keystrokes << copyAvailable << QString();

    copyAvailable.clear();

    keystrokes = {{BobUI::Key_B, BobUI::NoModifier},
                  {BobUI::Key_A, BobUI::NoModifier},
                  {BobUI::Key_Left, BobUI::ShiftModifier},
                  {BobUI::Key_Left, BobUI::NoModifier},
                  {BobUI::Key_Right, BobUI::ShiftModifier}};
    copyAvailable << true << false << true << false;
    BOBUIest::newRow(QString("Case6 B,A, <- + shift, ->, <- + shift | signals: true, false, true, false").toLatin1())
        << keystrokes << copyAvailable << QString("cut");

    copyAvailable.clear();

    keystrokes = {{BobUI::Key_T, BobUI::NoModifier},
                  {BobUI::Key_A, BobUI::NoModifier},
                  {BobUI::Key_A, BobUI::NoModifier},
                  {BobUI::Key_Left, BobUI::ShiftModifier},
                  {BobUI::Key_Left, BobUI::ShiftModifier},
                  {BobUI::Key_Left, BobUI::ShiftModifier},
                  {BobUI::Key_X, BobUI::ControlModifier}};
    copyAvailable << true << false << true;
    BOBUIest::newRow(QString("Case7 T,A,A, <- + shift, <- + shift, <- + shift, ctrl + x, undo() | signals: true, false, true").toLatin1())
        << keystrokes << copyAvailable << QString("undo");
}

//Tests the copyAvailable slot for several cases
void tst_QPlainTextEdit::copyAvailable()
{
    QFETCH(const pairListType, keystrokes);
    QFETCH(QList<bool>, copyAvailable);
    QFETCH(QString, function);

#ifdef Q_OS_MAC
    QSKIP("BOBUIBUG-22283: copyAvailable has never passed on Mac");
#endif
    ed->clear();
    QApplication::clipboard()->clear();
    QVERIFY(!ed->canPaste());
    QSignalSpy spyCopyAvailabe(ed, SIGNAL(copyAvailable(bool)));

    //Execute Keystrokes
    for (keyPairType keyPair : keystrokes)
        BOBUIest::keyClick(ed, keyPair.first, keyPair.second );

    //Execute ed->"function"
    if (function == "cut")
        ed->cut();
    else if (function == "copy")
        ed->copy();
    else if (function == "paste")
        ed->paste();
    else if (function == "undo")
        ed->paste();
    else if (function == "redo")
        ed->paste();

    //Compare spied signals
    QEXPECT_FAIL("Case7 T,A,A, <- + shift, <- + shift, <- + shift, ctrl + x, undo() | signals: true, false, true",
        "Wrong undo selection behaviour. Should be fixed in some future release. (See task: 132482)", Abort);
    QCOMPARE(spyCopyAvailabe.size(), copyAvailable.size());
    for (int i=0;i<spyCopyAvailabe.size(); i++) {
        QVariant variantSpyCopyAvailable = spyCopyAvailabe.at(i).at(0);
        QVERIFY2(variantSpyCopyAvailable.toBool() == copyAvailable.at(i), QString("Spied singnal: %1").arg(i).toLatin1());
    }
}
#endif

void tst_QPlainTextEdit::undoRedoAfterSetContent()
{
    QVERIFY(!ed->document()->isUndoAvailable());
    QVERIFY(!ed->document()->isRedoAvailable());
    ed->setPlainText("Foobar");
    QVERIFY(!ed->document()->isUndoAvailable());
    QVERIFY(!ed->document()->isRedoAvailable());
    ed->setPlainText("<p>bleh</p>");
    QVERIFY(!ed->document()->isUndoAvailable());
    QVERIFY(!ed->document()->isRedoAvailable());
}

void tst_QPlainTextEdit::numPadKeyNavigation()
{
    ed->setPlainText("Hello World");
    QCOMPARE(ed->textCursor().position(), 0);
    BOBUIest::keyClick(ed, BobUI::Key_Right, BobUI::KeypadModifier);
    QCOMPARE(ed->textCursor().position(), 1);
}

void tst_QPlainTextEdit::moveCursor()
{
    ed->setPlainText("Test");

    QSignalSpy cursorMovedSpy(ed, SIGNAL(cursorPositionChanged()));

    QCOMPARE(ed->textCursor().position(), 0);
    ed->moveCursor(BOBUIextCursor::NextCharacter);
    QCOMPARE(ed->textCursor().position(), 1);
    QCOMPARE(cursorMovedSpy.size(), 1);
    ed->moveCursor(BOBUIextCursor::NextCharacter, BOBUIextCursor::KeepAnchor);
    QCOMPARE(ed->textCursor().position(), 2);
    QCOMPARE(cursorMovedSpy.size(), 2);
    QCOMPARE(ed->textCursor().selectedText(), QString("e"));
}

class MyTextEdit : public QPlainTextEdit
{
public:
    inline MyTextEdit()
        : createMimeDataCallCount(0),
          canInsertCallCount(0),
          insertCallCount(0)
    {}

    mutable int createMimeDataCallCount;
    mutable int canInsertCallCount;
    mutable int insertCallCount;

    virtual QMimeData *createMimeDataFromSelection() const override {
        createMimeDataCallCount++;
        return QPlainTextEdit::createMimeDataFromSelection();
    }
    virtual bool canInsertFromMimeData(const QMimeData *source) const override {
        canInsertCallCount++;
        return QPlainTextEdit::canInsertFromMimeData(source);
    }
    virtual void insertFromMimeData(const QMimeData *source) override {
        insertCallCount++;
        QPlainTextEdit::insertFromMimeData(source);
    }

};

#ifndef BOBUI_NO_CLIPBOARD
void tst_QPlainTextEdit::mimeDataReimplementations()
{
    MyTextEdit ed;
    ed.setPlainText("Hello World");

    QCOMPARE(ed.createMimeDataCallCount, 0);
    QCOMPARE(ed.canInsertCallCount, 0);
    QCOMPARE(ed.insertCallCount, 0);

    ed.selectAll();

    QCOMPARE(ed.createMimeDataCallCount, 0);
    QCOMPARE(ed.canInsertCallCount, 0);
    QCOMPARE(ed.insertCallCount, 0);

    ed.copy();

    QCOMPARE(ed.createMimeDataCallCount, 1);
    QCOMPARE(ed.canInsertCallCount, 0);
    QCOMPARE(ed.insertCallCount, 0);

#ifdef BOBUI_BUILD_INTERNAL
    QWidgetTextControl *control = ed.findChild<QWidgetTextControl *>();
    QVERIFY(control);

    control->canInsertFromMimeData(QApplication::clipboard()->mimeData());

    QCOMPARE(ed.createMimeDataCallCount, 1);
    QCOMPARE(ed.canInsertCallCount, 1);
    QCOMPARE(ed.insertCallCount, 0);

    ed.paste();

    QCOMPARE(ed.createMimeDataCallCount, 1);
    QCOMPARE(ed.canInsertCallCount, 1);
    QCOMPARE(ed.insertCallCount, 1);
#endif
}
#endif

void tst_QPlainTextEdit::shiftEnterShouldInsertLineSeparator()
{
    BOBUIest::keyClick(ed, BobUI::Key_A);
    BOBUIest::keyClick(ed, BobUI::Key_Enter, BobUI::ShiftModifier);
    BOBUIest::keyClick(ed, BobUI::Key_B);
    QString expected;
    expected += 'a';
    expected += QChar::LineSeparator;
    expected += 'b';
    QCOMPARE(ed->textCursor().block().text(), expected);
}

void tst_QPlainTextEdit::selectWordsFromStringsContainingSeparators_data()
{
    BOBUIest::addColumn<QString>("testString");
    BOBUIest::addColumn<QString>("selectedWord");

    const ushort wordSeparators[] =
        {'.', ',', '?', '!', ':', ';', '-', '<', '>', '[', ']', '(', ')', '{', '}',
         '=', '\t', ushort(QChar::Nbsp)};

    for (size_t i = 0, count = sizeof(wordSeparators) / sizeof(wordSeparators[0]); i < count; ++i) {
        const ushort u = wordSeparators[i];
        QByteArray rowName = QByteArrayLiteral("separator: ");
        if (u >= 32 && u < 128)
            rowName += char(u);
        else
            rowName += QByteArrayLiteral("0x") + QByteArray::number(u, 16);
        BOBUIest::newRow(rowName.constData()) << QString("foo") + QChar(u) + QString("bar") << QString("foo");
    }
}

void tst_QPlainTextEdit::selectWordsFromStringsContainingSeparators()
{
    QFETCH(QString, testString);
    QFETCH(QString, selectedWord);
    ed->setPlainText(testString);
    BOBUIextCursor cursor = ed->textCursor();
    cursor.movePosition(BOBUIextCursor::StartOfLine);
    cursor.select(BOBUIextCursor::WordUnderCursor);
    QVERIFY(cursor.hasSelection());
    QCOMPARE(cursor.selection().toPlainText(), selectedWord);
    cursor.clearSelection();
}

#ifndef BOBUI_NO_CLIPBOARD
void tst_QPlainTextEdit::canPaste()
{
    if (!PlatformClipboard::isAvailable())
        QSKIP("Clipboard not working with cron-started unit tests");

    if (QGuiApplication::platformName().startsWith(QLatin1String("wayland"), BobUI::CaseInsensitive))
        QSKIP("Wayland: This fails. Figure out why.");

    QApplication::clipboard()->setText(QString());
    QVERIFY(!ed->canPaste());
    QApplication::clipboard()->setText("Test");
    QVERIFY(ed->canPaste());
    ed->setTextInteractionFlags(BobUI::NoTextInteraction);
    QVERIFY(!ed->canPaste());
}
#endif

void tst_QPlainTextEdit::ensureCursorVisibleOnInitialShow()
{
    QString manyPagesOfPlainText;
    for (int i = 0; i < 800; ++i)
        manyPagesOfPlainText += QLatin1String("Blah blah blah blah blah blah\n");

    ed->setPlainText(manyPagesOfPlainText);
    QCOMPARE(ed->textCursor().position(), 0);

    ed->moveCursor(BOBUIextCursor::End);
    ed->show();
    QVERIFY(ed->verticalScrollBar()->value() > 10);

    ed->moveCursor(BOBUIextCursor::Start);
    QVERIFY(ed->verticalScrollBar()->value() < 10);
    ed->hide();
    ed->verticalScrollBar()->setValue(ed->verticalScrollBar()->maximum());
    ed->show();
    QCOMPARE(ed->verticalScrollBar()->value(), ed->verticalScrollBar()->maximum());
}

class TestEdit : public QPlainTextEdit
{
public:
    TestEdit() : resizeEventCalled(false) {}

    bool resizeEventCalled;

protected:
    virtual void resizeEvent(QResizeEvent *e) override
    {
        QPlainTextEdit::resizeEvent(e);
        setPlainText("<img src=bobuiextbrowser-resizeevent.png width=" + QString::number(size().width()) + "><br>Size is " + QString::number(size().width()) + " x " + QString::number(size().height()));
        resizeEventCalled = true;
    }
};

void tst_QPlainTextEdit::setTextInsideResizeEvent()
{
    TestEdit edit;
    edit.show();
    edit.resize(800, 600);
    QVERIFY(edit.resizeEventCalled);
}

void tst_QPlainTextEdit::colorfulAppend()
{
    BOBUIextCharFormat fmt;

    fmt.setForeground(QBrush(BobUI::red));
    ed->mergeCurrentCharFormat(fmt);
    ed->appendPlainText("Red");
    fmt.setForeground(QBrush(BobUI::blue));
    ed->mergeCurrentCharFormat(fmt);
    ed->appendPlainText("Blue");
    fmt.setForeground(QBrush(BobUI::green));
    ed->mergeCurrentCharFormat(fmt);
    ed->appendPlainText("Green");

    QCOMPARE(ed->document()->blockCount(), 3);
    BOBUIextBlock block = ed->document()->begin();
    QCOMPARE(block.begin().fragment().text(), QString("Red"));
    QVERIFY(block.begin().fragment().charFormat().foreground().color() == BobUI::red);
    block = block.next();
    QCOMPARE(block.begin().fragment().text(), QString("Blue"));
    QVERIFY(block.begin().fragment().charFormat().foreground().color() == BobUI::blue);
    block = block.next();
    QCOMPARE(block.begin().fragment().text(), QString("Green"));
    QVERIFY(block.begin().fragment().charFormat().foreground().color() == BobUI::green);
}

void tst_QPlainTextEdit::ensureVisibleWithRtl()
{
    ed->setLayoutDirection(BobUI::RightToLeft);
    ed->setLineWrapMode(QPlainTextEdit::NoWrap);
    QString txt(500, QChar(QLatin1Char('a')));
    QCOMPARE(txt.size(), 500);
    ed->setPlainText(txt);
    ed->resize(100, 100);
    ed->show();

    qApp->processEvents();

    QVERIFY(ed->horizontalScrollBar()->maximum() > 0);

    ed->moveCursor(BOBUIextCursor::Start);
    QCOMPARE(ed->horizontalScrollBar()->value(), ed->horizontalScrollBar()->maximum());
    ed->moveCursor(BOBUIextCursor::End);
    QCOMPARE(ed->horizontalScrollBar()->value(), 0);
    ed->moveCursor(BOBUIextCursor::Start);
    QCOMPARE(ed->horizontalScrollBar()->value(), ed->horizontalScrollBar()->maximum());
    ed->moveCursor(BOBUIextCursor::End);
    QCOMPARE(ed->horizontalScrollBar()->value(), 0);
}

void tst_QPlainTextEdit::preserveCharFormatAfterSetPlainText()
{
    BOBUIextCharFormat fmt;
    fmt.setForeground(QBrush(BobUI::blue));
    ed->mergeCurrentCharFormat(fmt);
    ed->setPlainText("This is blue");
    ed->appendPlainText("This should still be blue");
    BOBUIextBlock block = ed->document()->begin();
    block = block.next();
    QCOMPARE(block.text(), QString("This should still be blue"));
    QCOMPARE(block.begin().fragment().charFormat().foreground().color(), QColor(BobUI::blue));
}

void tst_QPlainTextEdit::extraSelections()
{
    ed->setPlainText("Hello World");

    BOBUIextCursor c = ed->textCursor();
    c.movePosition(BOBUIextCursor::Start);
    c.movePosition(BOBUIextCursor::End, BOBUIextCursor::KeepAnchor);
    const int endPos = c.position();

    BOBUIextEdit::ExtraSelection sel;
    sel.cursor = c;
    ed->setExtraSelections(QList<BOBUIextEdit::ExtraSelection>() << sel);

    c.movePosition(BOBUIextCursor::Start);
    c.movePosition(BOBUIextCursor::NextWord);
    const int wordPos = c.position();
    c.movePosition(BOBUIextCursor::End, BOBUIextCursor::KeepAnchor);
    sel.cursor = c;
    ed->setExtraSelections(QList<BOBUIextEdit::ExtraSelection>() << sel);

    QList<BOBUIextEdit::ExtraSelection> selections = ed->extraSelections();
    QCOMPARE(selections.size(), 1);
    QCOMPARE(selections.at(0).cursor.position(), endPos);
    QCOMPARE(selections.at(0).cursor.anchor(), wordPos);
}

void tst_QPlainTextEdit::adjustScrollbars()
{
// For some reason ff is defined to be << on Mac Panther / gcc 3.3
#undef ff
    QFont ff(ed->font());
    ff.setFamily("Tahoma");
    ff.setPointSize(11);
    ed->setFont(ff);
    ed->setMinimumSize(140, 100);
    ed->setMaximumSize(140, 100);
    // We use showNormal() here, because otherwise on Android the widget will
    // be shown fullscreen, and the scrollbar will not appear.
    ed->showNormal();
    QLatin1String txt("\nabc def ghi jkl mno pqr stu vwx");
    ed->setPlainText(txt + txt + txt + txt);

    QVERIFY(ed->verticalScrollBar()->maximum() > 0);

    ed->moveCursor(BOBUIextCursor::End);
    int oldMaximum = ed->verticalScrollBar()->maximum();
    BOBUIextCursor cursor = ed->textCursor();
    cursor.insertText(QLatin1String("\n"));
    cursor.deletePreviousChar();
    QCOMPARE(ed->verticalScrollBar()->maximum(), oldMaximum);
}

class SignalReceiver : public QObject
{
    Q_OBJECT
public:
    SignalReceiver() : received(0) {}

    int receivedSignals() const { return received; }
    BOBUIextCharFormat charFormat() const { return format; }

public slots:
    void charFormatChanged(const BOBUIextCharFormat &tcf) { ++received; format = tcf; }

private:
    BOBUIextCharFormat format;
    int received;
};

void tst_QPlainTextEdit::textObscuredByScrollbars()
{
    ed->textCursor().insertText(
            "ab cab cab c abca kjsdf lka sjd lfk jsal df j kasdf abc ab abc "
            "a b c d e f g h i j k l m n o p q r s t u v w x y z "
            "abc abc abc abc abc abc abc abc abc abc abc abc abc abc abc abc "
            "ab ab ab ab ab ab ab ab ab ab ab ab ab ab ab ab ab ab ab ab ab"
            "abc abc abc abc abc abc abc abc abc abc abc abc abc abc abc abc "
            "ab ab ab ab ab ab ab ab ab ab ab ab ab ab ab ab ab ab ab ab ab"
            "abc abc abc abc abc abc abc abc abc abc abc abc abc abc abc abc "
            "ab ab ab ab ab ab ab ab ab ab ab ab ab ab ab ab ab ab ab ab ab"
            "abc abc abc abc abc abc abc abc abc abc abc abc abc abc abc abc "
            "ab ab ab ab ab ab ab ab ab ab ab ab ab ab ab ab ab ab ab ab ab"
            "abc abc abc abc abc abc abc abc abc abc abc abc abc abc abc abc "
            "ab ab ab ab ab ab ab ab ab ab ab ab ab ab ab ab ab ab ab ab ab"
    );
    ed->setHorizontalScrollBarPolicy(BobUI::ScrollBarAlwaysOff);
    ed->show();

    QSize documentSize = ed->document()->documentLayout()->documentSize().toSize();
    QSize viewportSize = ed->viewport()->size();

    QVERIFY(documentSize.width() <= viewportSize.width());
}

void tst_QPlainTextEdit::setTextPreservesUndoRedoEnabled()
{
    QVERIFY(ed->isUndoRedoEnabled());

    ed->setPlainText("Test");

    QVERIFY(ed->isUndoRedoEnabled());

    ed->setUndoRedoEnabled(false);
    QVERIFY(!ed->isUndoRedoEnabled());
    ed->setPlainText("Test2");
    QVERIFY(!ed->isUndoRedoEnabled());

    ed->setPlainText("<p>hello");
    QVERIFY(!ed->isUndoRedoEnabled());
}

void tst_QPlainTextEdit::wordWrapProperty()
{
    {
        QPlainTextEdit edit;
        BOBUIextDocument *doc = new BOBUIextDocument(&edit);
        doc->setDocumentLayout(new QPlainTextDocumentLayout(doc));
        edit.setDocument(doc);
        edit.setWordWrapMode(BOBUIextOption::NoWrap);
        QCOMPARE(doc->defaultTextOption().wrapMode(), BOBUIextOption::NoWrap);
    }
    {
        QPlainTextEdit edit;
        BOBUIextDocument *doc = new BOBUIextDocument(&edit);
        doc->setDocumentLayout(new QPlainTextDocumentLayout(doc));
        edit.setWordWrapMode(BOBUIextOption::NoWrap);
        edit.setDocument(doc);
        QCOMPARE(doc->defaultTextOption().wrapMode(), BOBUIextOption::NoWrap);
    }
}

void tst_QPlainTextEdit::lineWrapProperty()
{
    QCOMPARE(ed->wordWrapMode(), BOBUIextOption::WrapAtWordBoundaryOrAnywhere);
    QCOMPARE(ed->lineWrapMode(), QPlainTextEdit::WidgetWidth);
    ed->setLineWrapMode(QPlainTextEdit::NoWrap);
    QCOMPARE(ed->lineWrapMode(), QPlainTextEdit::NoWrap);
    QCOMPARE(ed->wordWrapMode(), BOBUIextOption::WrapAtWordBoundaryOrAnywhere);
    QCOMPARE(ed->document()->defaultTextOption().wrapMode(), BOBUIextOption::NoWrap);
}

void tst_QPlainTextEdit::selectionChanged()
{
    ed->setPlainText("Hello World");

    ed->moveCursor(BOBUIextCursor::Start);

    QSignalSpy selectionChangedSpy(ed, SIGNAL(selectionChanged()));

    BOBUIest::keyClick(ed, BobUI::Key_Right);
    QCOMPARE(ed->textCursor().position(), 1);
    QCOMPARE(selectionChangedSpy.size(), 0);

    BOBUIest::keyClick(ed, BobUI::Key_Right, BobUI::ShiftModifier);
    QCOMPARE(ed->textCursor().position(), 2);
    QCOMPARE(selectionChangedSpy.size(), 1);

    BOBUIest::keyClick(ed, BobUI::Key_Right, BobUI::ShiftModifier);
    QCOMPARE(ed->textCursor().position(), 3);
    QCOMPARE(selectionChangedSpy.size(), 2);

    BOBUIest::keyClick(ed, BobUI::Key_Right, BobUI::ShiftModifier);
    QCOMPARE(ed->textCursor().position(), 4);
    QCOMPARE(selectionChangedSpy.size(), 3);

    BOBUIest::keyClick(ed, BobUI::Key_Right);
    QCOMPARE(ed->textCursor().position(), 4);
    QCOMPARE(selectionChangedSpy.size(), 4);

    BOBUIest::keyClick(ed, BobUI::Key_Right);
    QCOMPARE(ed->textCursor().position(), 5);
    QCOMPARE(selectionChangedSpy.size(), 4);
}

void tst_QPlainTextEdit::blockCountChanged()
{
    QSignalSpy blockCountCpangedSpy(ed, SIGNAL(blockCountChanged(int)));
    ed->setPlainText("Hello");
    QCOMPARE(blockCountCpangedSpy.size(), 0);
    ed->setPlainText("Hello World");
    QCOMPARE(blockCountCpangedSpy.size(), 0);
    ed->setPlainText("Hello \n World \n this \n has \n more \n blocks \n than \n just \n one");
    QCOMPARE(blockCountCpangedSpy.size(), 1);
    ed->setPlainText("One");
    QCOMPARE(blockCountCpangedSpy.size(), 2);
    ed->setPlainText("One \n Two");
    QCOMPARE(blockCountCpangedSpy.size(), 3);
    ed->setPlainText("Three \n Four");
    QCOMPARE(blockCountCpangedSpy.size(), 3);
}


void tst_QPlainTextEdit::insertAndScrollToBottom()
{
    ed->setPlainText("First Line");
    ed->show();
    QString text;
    for(int i = 0; i < 2000; ++i) {
        text += QLatin1String("this is another line of text to be appended. It is quite long and will probably wrap around, meaning the number of lines is larger than the number of blocks in the text.\n");
    }
    BOBUIextCursor cursor = ed->textCursor();
    cursor.beginEditBlock();
    cursor.insertText(text);
    cursor.endEditBlock();
    ed->verticalScrollBar()->setValue(ed->verticalScrollBar()->maximum());
    QCOMPARE(ed->verticalScrollBar()->value(), ed->verticalScrollBar()->maximum());
}

Q_DECLARE_METATYPE(BobUI::InputMethodHints)
void tst_QPlainTextEdit::inputMethodQueryImHints_data()
{
    BOBUIest::addColumn<BobUI::InputMethodHints>("hints");

    BOBUIest::newRow("None") << static_cast<BobUI::InputMethodHints>(BobUI::ImhNone);
    BOBUIest::newRow("Password") << static_cast<BobUI::InputMethodHints>(BobUI::ImhHiddenText);
    BOBUIest::newRow("Normal") << static_cast<BobUI::InputMethodHints>(BobUI::ImhNoAutoUppercase | BobUI::ImhNoPredictiveText | BobUI::ImhSensitiveData);
}

void tst_QPlainTextEdit::inputMethodQueryImHints()
{
    QFETCH(BobUI::InputMethodHints, hints);
    ed->setInputMethodHints(hints);

    QVariant value = ed->inputMethodQuery(BobUI::ImHints);
    QCOMPARE(static_cast<BobUI::InputMethodHints>(value.toInt()), hints);
}

#if BOBUI_CONFIG(regularexpression)
void tst_QPlainTextEdit::findWithRegularExpression()
{
    ed->setPlainText(QStringLiteral("arbitrary text"));
    QRegularExpression rx("\\w{2}xt");

    bool found = ed->find(rx);

    QVERIFY(found);
    QCOMPARE(ed->textCursor().selectedText(), QStringLiteral("text"));
}

void tst_QPlainTextEdit::findBackwardWithRegularExpression()
{
    ed->setPlainText(QStringLiteral("arbitrary text"));
    BOBUIextCursor cursor = ed->textCursor();
    cursor.movePosition(BOBUIextCursor::End);
    ed->setTextCursor(cursor);
    QRegularExpression rx("a\\w*t");

    bool found = ed->find(rx, BOBUIextDocument::FindBackward);

    QVERIFY(found);
    QCOMPARE(ed->textCursor().selectedText(), QStringLiteral("arbit"));
}

void tst_QPlainTextEdit::findWithRegularExpressionReturnsFalseIfNoMoreResults()
{
    ed->setPlainText(QStringLiteral("arbitrary text"));
    QRegularExpression rx("t.xt");
    ed->find(rx);

    bool found = ed->find(rx);

    QVERIFY(!found);
    QCOMPARE(ed->textCursor().selectedText(), QStringLiteral("text"));
}
#endif

void tst_QPlainTextEdit::layoutAfterMultiLineRemove()
{
    ed->setVisible(true); // The widget must be visible to reproduce this bug.

    QString contents;
    for (int i = 0; i < 5; ++i)
        contents.append("\ttest\n");

    ed->setPlainText(contents);

    /*
     * Remove the tab from the beginning of lines 2-4, in an edit block. The
     * edit block is required for the bug to be reproduced.
     */

    BOBUIextCursor curs = ed->textCursor();
    curs.movePosition(BOBUIextCursor::Start);
    curs.movePosition(BOBUIextCursor::NextBlock);

    curs.beginEditBlock();
    for (int i = 0; i < 3; ++i) {
        curs.deleteChar();
        curs.movePosition(BOBUIextCursor::NextBlock);
    }
    curs.endEditBlock();

    /*
     * Now, we're going to perform the following actions:
     *
     *     - Move to the beginning of the document.
     *     - Move down three times - this should put us at the front of block 3.
     *     - Move to the end of the line.
     *
     * At this point, if the document layout is behaving correctly, we should
     * still be positioned on block 3. Verify that this is the case.
     */

    curs.movePosition(BOBUIextCursor::Start);
    curs.movePosition(BOBUIextCursor::Down, BOBUIextCursor::MoveAnchor, 3);
    curs.movePosition(BOBUIextCursor::EndOfLine);

    QCOMPARE(curs.blockNumber(), 3);
}

void tst_QPlainTextEdit::undoCommandRemovesAndReinsertsBlock()
{
    ed->setVisible(true);
    ed->setPlainText(QStringLiteral("line1\nline2"));
    QCOMPARE(ed->document()->blockCount(), 2);

    BOBUIextCursor cursor = ed->textCursor();
    cursor.movePosition(BOBUIextCursor::Start);
    cursor.movePosition(BOBUIextCursor::NextBlock, BOBUIextCursor::KeepAnchor);
    cursor.insertText(QStringLiteral("\n"));
    QCOMPARE(ed->document()->blockCount(), 2);

    ed->undo();
    QCOMPARE(ed->document()->blockCount(), 2);

    BOBUIextBlock block;
    for (block = ed->document()->begin(); block != ed->document()->end(); block = block.next()) {
        QVERIFY(block.isValid());
        QCOMPARE(block.length(), 6);
        QVERIFY(block.layout()->lineForTextPosition(0).isValid());
    }

}

class ContentsChangedFunctor {
public:
    ContentsChangedFunctor(QPlainTextEdit *t) : textEdit(t) {}
    void operator()(int, int, int)
    {
        BOBUIextCursor c(textEdit->textCursor());
        c.beginEditBlock();
        c.movePosition(BOBUIextCursor::Start);
        c.movePosition(BOBUIextCursor::End, BOBUIextCursor::KeepAnchor);
        c.setCharFormat(BOBUIextCharFormat());
        c.endEditBlock();
    }

private:
    QPlainTextEdit *textEdit;
};

void tst_QPlainTextEdit::taskBOBUIBUG_43562_lineCountCrash()
{
    connect(ed->document(), &BOBUIextDocument::contentsChange, ContentsChangedFunctor(ed));
    // Don't crash
    BOBUIest::keyClicks(ed, "Some text");
    BOBUIest::keyClick(ed, BobUI::Key_Left);
    BOBUIest::keyClick(ed, BobUI::Key_Right);
    BOBUIest::keyClick(ed, BobUI::Key_A);
    BOBUIest::keyClick(ed, BobUI::Key_Left);
    BOBUIest::keyClick(ed, BobUI::Key_Right);
    BOBUIest::keyClick(ed, BobUI::Key_Space);
    BOBUIest::keyClicks(ed, "nd some more");
    disconnect(ed->document(), SIGNAL(contentsChange(int, int, int)), 0, 0);
}

#if !defined(BOBUI_NO_CONTEXTMENU) && !defined(BOBUI_NO_CLIPBOARD)
void tst_QPlainTextEdit::contextMenu()
{
    ed->appendHtml(QStringLiteral("Hello <a href='http://www.bobui.io'>BobUI</a>"));

    QMenu *menu = ed->createStandardContextMenu();
    QVERIFY(menu);
    QAction *action = ed->findChild<QAction *>(QStringLiteral("link-copy"));
    QVERIFY(!action);
    delete menu;
    QVERIFY(!ed->findChild<QAction *>(QStringLiteral("link-copy")));

    ed->setTextInteractionFlags(BobUI::TextBrowserInteraction);

    menu = ed->createStandardContextMenu();
    QVERIFY(menu);
    action = ed->findChild<QAction *>(QStringLiteral("link-copy"));
    QVERIFY(action);
    QVERIFY(!action->isEnabled());
    delete menu;
    QVERIFY(!ed->findChild<QAction *>(QStringLiteral("link-copy")));

    BOBUIextCursor cursor = ed->textCursor();
    cursor.setPosition(ed->toPlainText().size() - 2);
    ed->setTextCursor(cursor);

    menu = ed->createStandardContextMenu(ed->cursorRect().center());
    QVERIFY(menu);
    action = ed->findChild<QAction *>(QStringLiteral("link-copy"));
    QVERIFY(action);
    QVERIFY(action->isEnabled());
    delete menu;
    QVERIFY(!ed->findChild<QAction *>(QStringLiteral("link-copy")));
}
#endif // BOBUI_NO_CONTEXTMENU && BOBUI_NO_CLIPBOARD

// BOBUIBUG-51923: Verify that the cursor rectangle returned by the input
// method query correctly reflects the viewport offset.
void tst_QPlainTextEdit::inputMethodCursorRect()
{
    ed->setPlainText("Line1\nLine2Line3\nLine3");
    ed->moveCursor(BOBUIextCursor::End);
    const QRectF cursorRect = ed->cursorRect();
    const QVariant cursorRectV = ed->inputMethodQuery(BobUI::ImCursorRectangle);
    QCOMPARE(cursorRectV.userType(), QMetaType::QRectF);
    QCOMPARE(cursorRectV.toRect(), cursorRect.toRect());
}

#if BOBUI_CONFIG(scrollbar)
// BOBUIBUG-64730: Verify that the scrollbar is updated after center on scroll was set
void tst_QPlainTextEdit::updateAfterChangeCenterOnScroll()
{
    ed->setPlainText("Line1\nLine2Line3\nLine3");
    ed->show();
    ed->setCenterOnScroll(true);
    const int maxWithCenterOnScroll = ed->verticalScrollBar()->maximum();
    ed->setCenterOnScroll(false);
    const int maxWithoutCenterOnScroll = ed->verticalScrollBar()->maximum();
    QVERIFY(maxWithCenterOnScroll > maxWithoutCenterOnScroll);
}

#endif

#ifndef BOBUI_NO_CLIPBOARD
void tst_QPlainTextEdit::updateCursorPositionAfterEdit()
{
    QPlainTextEdit plaintextEdit;
    plaintextEdit.setPlainText("some text some text\nsome text some text");

    const auto initialPosition = 1;

    // select some text
    auto cursor = plaintextEdit.textCursor();
    cursor.setPosition(initialPosition, BOBUIextCursor::MoveAnchor);
    cursor.setPosition(initialPosition + 3, BOBUIextCursor::KeepAnchor);
    plaintextEdit.setTextCursor(cursor);
    QVERIFY(plaintextEdit.textCursor().hasSelection());

    BOBUIest::keyClick(&plaintextEdit, BobUI::Key_Delete);
    BOBUIest::keyClick(&plaintextEdit, BobUI::Key_Down);
    BOBUIest::keyClick(&plaintextEdit, BobUI::Key_Up);

    // Moving the cursor down and up should bring it to the initial position
    QCOMPARE(plaintextEdit.textCursor().position(), initialPosition);

    // Test the same with backspace
    cursor = plaintextEdit.textCursor();
    cursor.setPosition(initialPosition + 3, BOBUIextCursor::KeepAnchor);
    plaintextEdit.setTextCursor(cursor);
    QVERIFY(plaintextEdit.textCursor().hasSelection());

    BOBUIest::keyClick(&plaintextEdit, BobUI::Key_Backspace);
    BOBUIest::keyClick(&plaintextEdit, BobUI::Key_Down);
    BOBUIest::keyClick(&plaintextEdit, BobUI::Key_Up);

    // Moving the cursor down and up should bring it to the initial position
    QCOMPARE(plaintextEdit.textCursor().position(), initialPosition);

    // Test insertion
    const QString txt("txt");
    QApplication::clipboard()->setText(txt);
    plaintextEdit.paste();
    BOBUIest::keyClick(&plaintextEdit, BobUI::Key_Down);
    BOBUIest::keyClick(&plaintextEdit, BobUI::Key_Up);

    // The curser should move back to the end of the copied text
    QCOMPARE(plaintextEdit.textCursor().position(), initialPosition + txt.size());
}
#endif

void tst_QPlainTextEdit::appendTextWhenInvisible()
{
    QWidget window;
    window.resize(640, 480);

    QPlainTextEdit *plainTextEdit = new QPlainTextEdit(&window);
    plainTextEdit->resize(320, 240);

    window.show();
    QVERIFY(BOBUIest::qWaitForWindowActive(&window));

    // this should be long enough to let vertical scroll bar show up
    const QString baseText("text\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\ntext");
    const QString textToAppend("aaa");

    plainTextEdit->setPlainText(baseText + "\n" + textToAppend);
    const auto maxAfterSet = plainTextEdit->verticalScrollBar()->maximum();
    // make sure the vertical scroll bar is visible
    QVERIFY(maxAfterSet != 0);

    plainTextEdit->clear();
    plainTextEdit->setPlainText(baseText);
    plainTextEdit->hide();
    plainTextEdit->appendPlainText(textToAppend);
    plainTextEdit->show();
    const auto maxAfterAppend = plainTextEdit->verticalScrollBar()->maximum();
    QVERIFY(maxAfterAppend != 0);

    QCOMPARE(maxAfterAppend, maxAfterSet);
}

enum SetupCommand {
    ClearPlaceHolder, // set empty placeholder text
    SetPlaceHolder, // set a non-empty placeholder text
    ClearContent, // set empty text as content
    SetContent // set non-empty text as content
};

void tst_QPlainTextEdit::placeholderVisibility_data()
{
    BOBUIest::addColumn<QList<SetupCommand>>("setupCommands");
    BOBUIest::addColumn<bool>("placeholderVisible");
    BOBUIest::addRow("no placeholder set + no text set")
            << QList<SetupCommand>{} << false;
    BOBUIest::addRow("no placeholder set + text set or text set + no placeholder set")
            << QList<SetupCommand>{ SetContent } << false;
    BOBUIest::addRow("no placeholder set + text set + empty text set")
            << QList<SetupCommand>{ SetContent , ClearContent }
            << false;
    BOBUIest::addRow("no placeholder set + empty text set + text set")
            << QList<SetupCommand>{ ClearContent, SetContent }
            << false;
    BOBUIest::addRow("empty placeholder set + no text set")
            << QList<SetupCommand>{ ClearPlaceHolder } << false;
    BOBUIest::addRow("empty placeholder set + text set")
            << QList<SetupCommand>{ ClearPlaceHolder, SetContent }
            << false;
    BOBUIest::addRow("empty placeholder set + text set + empty text set")
            << QList<SetupCommand>{ ClearPlaceHolder, SetContent, ClearContent }
            << false;
    BOBUIest::addRow("empty placeholder set + empty text set + text set")
            << QList<SetupCommand>{ ClearPlaceHolder, ClearContent, SetContent }
            << false;
    BOBUIest::addRow("placeholder set + no text set")
            << QList<SetupCommand>{ SetPlaceHolder, ClearContent }
            << true;
    BOBUIest::addRow("placeholder set + text set")
            << QList<SetupCommand>{ SetPlaceHolder, SetContent }
            << false;
    BOBUIest::addRow("placeholder set + text set + empty text set")
            << QList<SetupCommand>{ SetPlaceHolder, SetContent, ClearContent }
            << true;
    BOBUIest::addRow("placeholder set + empty text set + text set")
            << QList<SetupCommand>{ SetPlaceHolder, ClearContent, SetContent }
            << false;
    BOBUIest::addRow("placeholder set + text set + empty placeholder set")
            << QList<SetupCommand>{ SetPlaceHolder, SetContent, ClearPlaceHolder}
            << false;
    BOBUIest::addRow("placeholder set + empty placeholder set + text set")
            << QList<SetupCommand>{ SetPlaceHolder, ClearPlaceHolder, SetContent }
            << false;
    BOBUIest::addRow("placeholder set + empty placeholder set + empty text set")
            << QList<SetupCommand>{ SetPlaceHolder, ClearPlaceHolder, ClearContent }
            << false;
    BOBUIest::addRow("placeholder set + empty text set + empty placeholder set")
            << QList<SetupCommand>{ SetPlaceHolder, ClearContent, ClearPlaceHolder }
            << false;
    BOBUIest::addRow("text set + no placeholder set + empty text set")
            << QList<SetupCommand>{ SetContent, ClearContent }
            << false;
    BOBUIest::addRow("text set + empty placeholder set")
            << QList<SetupCommand>{ SetContent, ClearPlaceHolder }
            << false;
    BOBUIest::addRow("text set + placeholder set")
            << QList<SetupCommand>{ SetContent, SetPlaceHolder }
            << false;
    BOBUIest::addRow("text set + placeholder set + empty text set")
            << QList<SetupCommand>{ SetContent, SetPlaceHolder, ClearContent }
            << true;
    BOBUIest::addRow("text set + placeholder set + empty placeholder set")
            << QList<SetupCommand>{ SetContent, SetPlaceHolder, ClearPlaceHolder }
            << false;
}

void tst_QPlainTextEdit::placeholderVisibility()
{
    QFETCH(QList<SetupCommand>, setupCommands);
    QFETCH(bool, placeholderVisible);

    QPlainTextEdit plainTextEdit;
    for (auto command : setupCommands) {
        switch (command) {
        case ClearPlaceHolder:
            plainTextEdit.setPlaceholderText("");
            break;
        case SetPlaceHolder:
            plainTextEdit.setPlaceholderText("BobUI is awesome !");
            break;
        case ClearContent:
            plainTextEdit.setPlainText("");
            break;
        case SetContent:
            plainTextEdit.setPlainText("PlainText...");
            break;
        }
    }
    auto *plainTextEdit_d = static_cast<QPlainTextEditPrivate *>(bobui_widget_private(&plainTextEdit));

    plainTextEdit.show();
    QVERIFY(BOBUIest::qWaitForWindowExposed(&plainTextEdit));
    BOBUIRY_COMPARE(plainTextEdit_d->placeholderTextShown, placeholderVisible);
}


void tst_QPlainTextEdit::scrollBarSignals()
{
    QPlainTextEdit plainTextEdit;
    QString longText;
    for (uint i = 0; i < 500; ++i)
        longText += "This is going to be a very long text for scroll signal testing.\n";
    plainTextEdit.setPlainText(longText);
    QScrollBar *vbar = plainTextEdit.verticalScrollBar();
    plainTextEdit.show();
    QVERIFY(BOBUIest::qWaitForWindowExposed(&plainTextEdit));
    QSignalSpy spy(vbar, &QScrollBar::valueChanged);

    BOBUIest::keyClick(vbar, BobUI::Key_Down);
    BOBUIRY_COMPARE(spy.count(), 1);
    BOBUIest::keyClick(vbar, BobUI::Key_PageDown);
    BOBUIRY_COMPARE(spy.count(), 2);
    BOBUIest::keyClick(vbar, BobUI::Key_PageDown);
    BOBUIRY_COMPARE(spy.count(), 3);
    BOBUIest::keyClick(vbar, BobUI::Key_Up);
    BOBUIRY_COMPARE(spy.count(), 4);
    BOBUIest::keyClick(vbar, BobUI::Key_PageUp);
    BOBUIRY_COMPARE(spy.count(), 5);
}

void tst_QPlainTextEdit::dontCrashWithCss()
{
    qApp->setStyleSheet("QWidget { font: 10pt; }");
    QPlainTextEdit edit;
    edit.show();
    qApp->setStyleSheet(QString());
}


BOBUIEST_MAIN(tst_QPlainTextEdit)
#include "tst_qplaintextedit.moc"
