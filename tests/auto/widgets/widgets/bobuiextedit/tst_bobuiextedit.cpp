// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only


#include <BOBUIest>
#include <QSignalSpy>
#include <BOBUIimer>

#include <bobuiextedit.h>
#include <bobuiextcursor.h>
#include <bobuiextlist.h>
#include <qdebug.h>
#include <qapplication.h>
#include <qclipboard.h>
#include <bobuiextbrowser.h>
#include <private/qwidgettextcontrol_p.h>
#include <qscrollbar.h>
#include <bobuiextobject.h>
#include <bobuiexttable.h>
#include <qpainter.h>
#include <qimagereader.h>
#include <qimagewriter.h>
#include <qcommonstyle.h>
#include <qlayout.h>
#include <qdir.h>
#include <qpaintengine.h>

#include <qabstracttextdocumentlayout.h>
#include <bobuiextdocumentfragment.h>
#include <qsyntaxhighlighter.h>

#include "../../../shared/platformclipboard.h"
#include "../../../shared/platforminputcontext.h"
#include <private/qinputmethod_p.h>

#include <BobUIWidgets/private/qapplication_p.h>

Q_LOGGING_CATEGORY(lcTests, "bobui.widgets.tests")

//Used in copyAvailable
using keyPairType = std::pair<BobUI::Key, BobUI::KeyboardModifier>;
typedef QList<keyPairType> pairListType;
Q_DECLARE_METATYPE(keyPairType);

Q_DECLARE_METATYPE(QList<QInputMethodEvent::Attribute>);

BOBUI_FORWARD_DECLARE_CLASS(BOBUIextEdit)

class tst_BOBUIextEdit : public QObject
{
    Q_OBJECT
public:
    tst_BOBUIextEdit();

public slots:
    void initTestCase();
    void cleanupTestCase();
    void init();
    void cleanup();
private slots:
    void getSetCheck();
    void inlineAttributesOnInsert();
    void inlineAttributesOnSelection();
    void inlineAttributeSymmetry();
    void inlineAttributeSymmetryWithSelection();
    void autoBulletList1();
    void autoBulletList2();
    void preserveCharFormatAfterNewline();
#ifndef BOBUI_NO_CLIPBOARD
    void clearMustNotChangeClipboard();
#endif
    void clearMustNotResetRootFrameMarginToDefault();
    void clearShouldPreserveTheCurrentCharFormat();
    void clearShouldClearExtraSelections();
    void paragSeparatorOnPlaintextAppend();
    void layoutingLoop();
#ifndef BOBUI_NO_CLIPBOARD
    void selectAllSetsNotSelection();
#endif
    void asciiTab();
    void setDocument();
    void setDocument_shared();
    void mergeCurrentCharFormat();
    void mergeCurrentBlockCharFormat();
    void emptyAppend();
    void appendOnEmptyDocumentShouldReuseInitialParagraph();
    void cursorPositionChanged();
    void setTextCursor();
#ifndef BOBUI_NO_CLIPBOARD
    void undoAvailableAfterPaste();
#endif
    void undoRedoAvailableRepetition();
    void appendShouldUseCurrentFormat();
    void appendShouldNotTouchTheSelection();
    void backspace();
    void shiftBackspace();
    void undoRedo();
    void preserveCharFormatInAppend();
#ifndef BOBUI_NO_CLIPBOARD
    void copyAndSelectAllInReadonly();
#endif
    void noPropertiesOnDefaultTextEditCharFormat();
    void setPlainTextShouldUseCurrentCharFormat();
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
#ifndef BOBUI_NO_TEXTHTMLPARSER
    void mimeTypesAvailableFromRichText();
#endif
#if BOBUI_CONFIG(textmarkdownreader)
    void mimeTypesAvailableFromMarkdown();
#endif
#endif // BOBUI_NO_CLIPBOARD
    void ctrlEnterShouldInsertLineSeparator_NOT();
    void shiftEnterShouldInsertLineSeparator();
    void selectWordsFromStringsContainingSeparators_data();
    void selectWordsFromStringsContainingSeparators();
#ifndef BOBUI_NO_CLIPBOARD
    void canPaste();
    void copyAvailable_data();
    void copyAvailable();
#endif
    void ensureCursorVisibleOnInitialShow();
    void setHtmlInsideResizeEvent();
    void colorfulAppend();
    void ensureVisibleWithRtl();
    void preserveCharFormatAfterSetPlainText();
    void extraSelections();
    void adjustScrollbars();
    void currentCharFormatChanged();
    void textObscuredByScrollbars();
    void setTextPreservesUndoRedoEnabled();
    void wordWrapProperty();
    void lineWrapProperty();
    void selectionChanged();
#ifndef BOBUI_NO_CLIPBOARD
    void copyPasteBackgroundImage();
    void copyPasteForegroundImage();
#endif
    void setText();
    void cursorRect();
#ifdef BOBUI_BUILD_INTERNAL
    void fullWidthSelection_data();
    void fullWidthSelection();
    void fullWidthSelection2();
    void setDocumentPreservesPalette();
#endif
    void pasteFromBobUI3RichText();
    void pasteFromMarkdown();
    void noWrapBackgrounds();
    void preserveCharFormatAfterUnchangingSetPosition();
    void twoSameInputMethodEvents();
#ifndef BOBUI_NO_CONTEXTMENU
    void taskBOBUIBUG_7902_contextMenuCrash();
#endif
    void bidiVisualMovement_data();
    void bidiVisualMovement();

    void bidiLogicalMovement_data();
    void bidiLogicalMovement();

    void inputMethodEvent();
    void inputMethodSelection();
    void inputMethodQuery();
    void inputMethodQueryImHints_data();
    void inputMethodQueryImHints();
    void inputMethodCursorRect();

    void highlightLongLine();

    void countTextChangedOnRemove();

#if BOBUI_CONFIG(regularexpression)
    void findWithRegularExpression();
    void findBackwardWithRegularExpression();
    void findWithRegularExpressionReturnsFalseIfNoMoreResults();
#endif

#if BOBUI_CONFIG(wheelevent)
    void wheelEvent();
#endif

    void preeditCharFormat_data();
    void preeditCharFormat();

    void nextFormatAfterEnterPressed_data();
    void nextFormatAfterEnterPressed();

    void dontCrashWithCss();

private:
    void createSelection();
    int blockCount() const;
    void compareWidgetAndImage(BOBUIextEdit &widget, const QString &imageFileName);
    bool isMainFontFixed();

    BOBUIextEdit *ed;
    qreal rootFrameMargin;
    PlatformInputContext m_platformInputContext;
    const QString m_fullWidthSelectionImagesFolder;
};

// Testing get/set functions
void tst_BOBUIextEdit::getSetCheck()
{
    BOBUIextEdit obj1;
    // BOBUIextDocument * BOBUIextEdit::document()
    // void BOBUIextEdit::setDocument(BOBUIextDocument *)
    BOBUIextDocument *var1 = new BOBUIextDocument;
    obj1.setDocument(var1);
    QCOMPARE(var1, obj1.document());
    obj1.setDocument((BOBUIextDocument *)0);
    QVERIFY(var1 != obj1.document()); // BOBUIextEdit creates a new document when setting 0
    QVERIFY((BOBUIextDocument *)0 != obj1.document());
    delete var1;

    // AutoFormatting BOBUIextEdit::autoFormatting()
    // void BOBUIextEdit::setAutoFormatting(AutoFormatting)
    obj1.setAutoFormatting(BOBUIextEdit::AutoFormatting(BOBUIextEdit::AutoNone));
    QCOMPARE(BOBUIextEdit::AutoFormatting(BOBUIextEdit::AutoNone), obj1.autoFormatting());
    obj1.setAutoFormatting(BOBUIextEdit::AutoFormatting(BOBUIextEdit::AutoBulletList));
    QCOMPARE(BOBUIextEdit::AutoFormatting(BOBUIextEdit::AutoBulletList), obj1.autoFormatting());
    obj1.setAutoFormatting(BOBUIextEdit::AutoFormatting(BOBUIextEdit::AutoAll));
    QCOMPARE(BOBUIextEdit::AutoFormatting(BOBUIextEdit::AutoAll), obj1.autoFormatting());

    // bool BOBUIextEdit::tabChangesFocus()
    // void BOBUIextEdit::setTabChangesFocus(bool)
    obj1.setTabChangesFocus(false);
    QCOMPARE(false, obj1.tabChangesFocus());
    obj1.setTabChangesFocus(true);
    QCOMPARE(true, obj1.tabChangesFocus());

    // LineWrapMode BOBUIextEdit::lineWrapMode()
    // void BOBUIextEdit::setLineWrapMode(LineWrapMode)
    obj1.setLineWrapMode(BOBUIextEdit::LineWrapMode(BOBUIextEdit::NoWrap));
    QCOMPARE(BOBUIextEdit::LineWrapMode(BOBUIextEdit::NoWrap), obj1.lineWrapMode());
    obj1.setLineWrapMode(BOBUIextEdit::LineWrapMode(BOBUIextEdit::WidgetWidth));
    QCOMPARE(BOBUIextEdit::LineWrapMode(BOBUIextEdit::WidgetWidth), obj1.lineWrapMode());
    obj1.setLineWrapMode(BOBUIextEdit::LineWrapMode(BOBUIextEdit::FixedPixelWidth));
    QCOMPARE(BOBUIextEdit::LineWrapMode(BOBUIextEdit::FixedPixelWidth), obj1.lineWrapMode());
    obj1.setLineWrapMode(BOBUIextEdit::LineWrapMode(BOBUIextEdit::FixedColumnWidth));
    QCOMPARE(BOBUIextEdit::LineWrapMode(BOBUIextEdit::FixedColumnWidth), obj1.lineWrapMode());

    // int BOBUIextEdit::lineWrapColumnOrWidth()
    // void BOBUIextEdit::setLineWrapColumnOrWidth(int)
    obj1.setLineWrapColumnOrWidth(0);
    QCOMPARE(0, obj1.lineWrapColumnOrWidth());
    obj1.setLineWrapColumnOrWidth(INT_MIN);
    QCOMPARE(INT_MIN, obj1.lineWrapColumnOrWidth());
    obj1.setLineWrapColumnOrWidth(INT_MAX);
    QCOMPARE(INT_MAX, obj1.lineWrapColumnOrWidth());

    // bool BOBUIextEdit::overwriteMode()
    // void BOBUIextEdit::setOverwriteMode(bool)
    obj1.setOverwriteMode(false);
    QCOMPARE(false, obj1.overwriteMode());
    obj1.setOverwriteMode(true);
    QCOMPARE(true, obj1.overwriteMode());

    // int BOBUIextEdit::tabStopWidth()
    // void BOBUIextEdit::setTabStopWidth(int)
    obj1.setTabStopDistance(0);
    QCOMPARE(0, obj1.tabStopDistance());
    obj1.setTabStopDistance(-1);
    QCOMPARE(0, obj1.tabStopDistance()); // Makes no sense to set a negative tabstop value
    obj1.setTabStopDistance(std::numeric_limits<qreal>::max());
    QCOMPARE(std::numeric_limits<qreal>::max(), obj1.tabStopDistance());

    // bool BOBUIextEdit::acceptRichText()
    // void BOBUIextEdit::setAcceptRichText(bool)
    obj1.setAcceptRichText(false);
    QCOMPARE(false, obj1.acceptRichText());
    obj1.setAcceptRichText(true);
    QCOMPARE(true, obj1.acceptRichText());

    // qreal BOBUIextEdit::fontPointSize()
    // void BOBUIextEdit::setFontPointSize(qreal)
    obj1.setFontPointSize(qreal(1.1));
    QCOMPARE(qreal(1.1), obj1.fontPointSize());
    // we currently assert in QFont::setPointSizeF for that
    //obj1.setFontPointSize(0.0);
    //QCOMPARE(1.1, obj1.fontPointSize()); // Should not accept 0.0 => keep old

    // int BOBUIextEdit::fontWeight()
    // void BOBUIextEdit::setFontWeight(int)
    obj1.setFontWeight(1);
    QCOMPARE(1, obj1.fontWeight()); // Range<1, 99>
    obj1.setFontWeight(99);
    QCOMPARE(99, obj1.fontWeight()); // Range<1, 99>
    /* assertion in qfont.cpp
    obj1.setFontWeight(INT_MIN);
    QCOMPARE(1, obj1.fontWeight()); // Range<1, 99>
    obj1.setFontWeight(INT_MAX);
    QCOMPARE(99, obj1.fontWeight()); // Range<1, 99>
    */

    // bool BOBUIextEdit::fontUnderline()
    // void BOBUIextEdit::setFontUnderline(bool)
    obj1.setFontUnderline(false);
    QCOMPARE(false, obj1.fontUnderline());
    obj1.setFontUnderline(true);
    QCOMPARE(true, obj1.fontUnderline());

    // bool BOBUIextEdit::fontItalic()
    // void BOBUIextEdit::setFontItalic(bool)
    obj1.setFontItalic(false);
    QCOMPARE(false, obj1.fontItalic());
    obj1.setFontItalic(true);
    QCOMPARE(true, obj1.fontItalic());
}

class BobUITestDocumentLayout : public QAbstractTextDocumentLayout
{
    Q_OBJECT
public:
    inline BobUITestDocumentLayout(BOBUIextEdit *edit, BOBUIextDocument *doc, int &itCount)
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

    BOBUIextEdit *ed;

    int &iterationCounter;
};

tst_BOBUIextEdit::tst_BOBUIextEdit() :
    m_fullWidthSelectionImagesFolder(QFINDTESTDATA("fullWidthSelection"))
{

}

void tst_BOBUIextEdit::initTestCase()
{
    QInputMethodPrivate *inputMethodPrivate = QInputMethodPrivate::get(qApp->inputMethod());
    inputMethodPrivate->testContext = &m_platformInputContext;
    QVERIFY2(!m_fullWidthSelectionImagesFolder.isEmpty(), qPrintable(QString::fromLatin1("Cannot locate 'fullWidthSelection' starting from %1").arg(QDir::currentPath())));
}

void tst_BOBUIextEdit::cleanupTestCase()
{
    QInputMethodPrivate *inputMethodPrivate = QInputMethodPrivate::get(qApp->inputMethod());
    inputMethodPrivate->testContext = 0;
}

void tst_BOBUIextEdit::init()
{
    ed = new BOBUIextEdit(0);
    rootFrameMargin = ed->document()->documentMargin();
}

void tst_BOBUIextEdit::cleanup()
{
    delete ed;
    ed = 0;
}

void tst_BOBUIextEdit::inlineAttributesOnInsert()
{
    const QColor blue(BobUI::blue);
    QVERIFY(ed->textCursor().charFormat().foreground().color() != blue);

    ed->setTextColor(blue);
    BOBUIest::keyClick(ed, BobUI::Key_A);

    QCOMPARE(ed->textCursor().charFormat().foreground().color(), blue);
}

void tst_BOBUIextEdit::inlineAttributesOnSelection()
{
    createSelection();

    ed->setFontItalic(true);

    QVERIFY(ed->textCursor().charFormat().fontItalic());
}

void tst_BOBUIextEdit::inlineAttributeSymmetry()
{
    ed->setFontPointSize(42.0);
    QCOMPARE(double(ed->fontPointSize()), 42.0);

    ed->setFontFamily("Test");
    QCOMPARE(ed->fontFamily(), QString("Test"));

    ed->setFontWeight(QFont::Bold);
    QCOMPARE((int)ed->fontWeight(), (int)QFont::Bold);

    ed->setFontUnderline(true);
    QCOMPARE(ed->fontUnderline(), true);

    ed->setFontItalic(true);
    QCOMPARE(ed->fontItalic(), true);

    ed->setTextColor(BobUI::blue);
    QCOMPARE(ed->textColor(), QColor(BobUI::blue));

    ed->setTextBackgroundColor(BobUI::red);
    QCOMPARE(ed->textBackgroundColor(), QColor(BobUI::red));

    ed->setAlignment(BobUI::AlignRight);
    QCOMPARE((int)ed->alignment(), (int)BobUI::AlignRight);
}

void tst_BOBUIextEdit::inlineAttributeSymmetryWithSelection()
{
    createSelection();

    inlineAttributeSymmetry();
}

void tst_BOBUIextEdit::autoBulletList1()
{
    ed->setAutoFormatting(BOBUIextEdit::AutoBulletList);

    BOBUIest::keyClick(ed, BobUI::Key_Return);
    BOBUIest::keyClicks(ed, "*This should become a list");

    QVERIFY(ed->textCursor().currentList());
    QCOMPARE(ed->textCursor().currentList()->format().style(), BOBUIextListFormat::ListDisc);
}

void tst_BOBUIextEdit::autoBulletList2()
{
    ed->setAutoFormatting(BOBUIextEdit::AutoNone);
    BOBUIest::keyClick(ed, BobUI::Key_Return);
    BOBUIest::keyClicks(ed, "*This should NOT become a list");

    QVERIFY(!ed->textCursor().currentList());
}

void tst_BOBUIextEdit::preserveCharFormatAfterNewline()
{
    ed->setTextColor(BobUI::blue);
    BOBUIest::keyClicks(ed, "Hello");

    BOBUIest::keyClick(ed, BobUI::Key_Return);

    QCOMPARE(ed->textColor(), QColor(BobUI::blue));
}

void tst_BOBUIextEdit::createSelection()
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
void tst_BOBUIextEdit::clearMustNotChangeClipboard()
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

void tst_BOBUIextEdit::clearMustNotResetRootFrameMarginToDefault()
{
    QCOMPARE(ed->document()->rootFrame()->frameFormat().margin(), rootFrameMargin);
    ed->clear();
    QCOMPARE(ed->document()->rootFrame()->frameFormat().margin(), rootFrameMargin);
}

void tst_BOBUIextEdit::clearShouldPreserveTheCurrentCharFormat()
{
    ed->setFontUnderline(true);
    QVERIFY(ed->fontUnderline());
    ed->clear();
    QVERIFY(ed->fontUnderline());
}

void tst_BOBUIextEdit::clearShouldClearExtraSelections()
{
    BOBUIextEdit::ExtraSelection sel;
    sel.cursor = ed->textCursor();
    sel.format.setProperty(BOBUIextFormat::FullWidthSelection, true);
    ed->setExtraSelections(QList<BOBUIextEdit::ExtraSelection>() << sel);
    QCOMPARE(ed->extraSelections().size(), 1);
    ed->clear();
    QCOMPARE(ed->extraSelections().size(), 0);
}

void tst_BOBUIextEdit::paragSeparatorOnPlaintextAppend()
{
    ed->append("Hello\nWorld");
    int cnt = 0;
    BOBUIextBlock blk = ed->document()->begin();
    while (blk.isValid()) {
        ++cnt;
        blk = blk.next();
    }
    QCOMPARE(cnt, 2);
}

void tst_BOBUIextEdit::layoutingLoop()
{
    QPointer<BOBUIextEdit> ed = new BOBUIextEdit(0);
    // this is a testcase for an ugly layouting problem, causing an infinite loop.
    // BOBUIextEdit's resizeEvent has a long comment about what and why it can happen.

    int callsToSetPageSize = 0;

    BOBUIextDocument *doc = new BOBUIextDocument;
    BobUITestDocumentLayout *lout = new BobUITestDocumentLayout(ed, doc, callsToSetPageSize);
    doc->setDocumentLayout(lout);
    ed->setDocument(doc);

    ed->show();
    ed->resize(100, 100);

    qApp->processEvents();
    delete doc;
    delete ed;

    // ###### should need less!
    QVERIFY(callsToSetPageSize < 10);
}

#ifndef BOBUI_NO_CLIPBOARD
void tst_BOBUIextEdit::selectAllSetsNotSelection()
{
    if (!QApplication::clipboard()->supportsSelection()) {
        QSKIP("Test only relevant for systems with selection");
        return;
    }

    QApplication::clipboard()->setText(QString("foobar"), QClipboard::Selection);
    QCOMPARE(QApplication::clipboard()->text(QClipboard::Selection), QString("foobar"));

    ed->insertPlainText("Hello World");
    ed->selectAll();

    QCOMPARE(QApplication::clipboard()->text(QClipboard::Selection), QString::fromLatin1("foobar"));
}
#endif
void tst_BOBUIextEdit::asciiTab()
{
    BOBUIextEdit edit;
    edit.setPlainText("\t");
    edit.show();
    qApp->processEvents();
    QCOMPARE(edit.toPlainText().at(0), QChar('\t'));
}


void tst_BOBUIextEdit::setDocument()
{
    BOBUIextDocument *document = new BOBUIextDocument(ed);
    QCOMPARE(document->isModified(), false);
    QCOMPARE(document->isUndoAvailable(), false);
    BOBUIextCursor(document).insertText("Test");
    QCOMPARE(document->isUndoAvailable(), true);
    QCOMPARE(document->isModified(), true);
    ed->setDocument(document);
    QCOMPARE(ed->toPlainText(), QString("Test"));
    QCOMPARE(document->isUndoAvailable(), true);
    QCOMPARE(document->isModified(), true);
}

void tst_BOBUIextEdit::setDocument_shared()
{
    BOBUIextDocument *document = new BOBUIextDocument(ed);
    QCOMPARE(document->isModified(), false);
    QCOMPARE(document->isUndoAvailable(), false);
    BOBUIextCursor(document).insertText("Test");
    QCOMPARE(document->isUndoAvailable(), true);
    document->setModified(false);
    ed->setDocument(document);
    QCOMPARE(ed->toPlainText(), QString("Test"));
    QCOMPARE(document->isUndoAvailable(), true);
    QCOMPARE(document->isModified(), false);
    BOBUIextCursor(document).insertText("Test2");
    QCOMPARE(document->isModified(), true);
    BOBUIextEdit editor2;
    editor2.setDocument(document);
    QCOMPARE(document->isUndoAvailable(), true);
    QCOMPARE(document->isModified(), true);
}

void tst_BOBUIextEdit::mergeCurrentCharFormat()
{
    ed->setPlainText("Hello Test World");
    BOBUIextCursor cursor = ed->textCursor();
    cursor.setPosition(7);
    ed->setTextCursor(cursor);

    BOBUIextCharFormat mod;
    mod.setFontItalic(true);
    mod.setForeground(BobUI::red);
    ed->mergeCurrentCharFormat(mod);

    cursor.movePosition(BOBUIextCursor::Right);
    cursor.movePosition(BOBUIextCursor::Right);
    // do NOT select the current word under the cursor, /JUST/
    // call mergeCharFormat on the cursor
    QVERIFY(!cursor.charFormat().fontItalic());
    QVERIFY(cursor.charFormat().foreground().color() != BobUI::red);
}

void tst_BOBUIextEdit::mergeCurrentBlockCharFormat()
{
    ed->setPlainText("FirstLine\n\nThirdLine");
    BOBUIextCursor cursor = ed->textCursor();
    cursor.movePosition(BOBUIextCursor::Start);
    cursor.movePosition(BOBUIextCursor::Down);

    // make sure we're in the empty second line
    QVERIFY(cursor.atBlockStart());
    QVERIFY(cursor.atBlockEnd());

    ed->setTextCursor(cursor);

    BOBUIextCharFormat mod;
    mod.setForeground(BobUI::red);
    ed->mergeCurrentCharFormat(mod);

    QVERIFY(cursor.blockCharFormat().foreground().color() != BobUI::red);
    cursor.movePosition(BOBUIextCursor::Up);
    QVERIFY(cursor.blockCharFormat().foreground().color() != BobUI::red);
    cursor.movePosition(BOBUIextCursor::Down);
    cursor.movePosition(BOBUIextCursor::Down);
    QVERIFY(cursor.blockCharFormat().foreground().color() != BobUI::red);
}

int tst_BOBUIextEdit::blockCount() const
{
    int blocks = 0;
    for (BOBUIextBlock block = ed->document()->begin(); block.isValid(); block = block.next())
        ++blocks;
    return blocks;
}

// Supporter issue #56783
void tst_BOBUIextEdit::emptyAppend()
{
    ed->append("Blah");
    QCOMPARE(blockCount(), 1);
    ed->append(QString());
    QCOMPARE(blockCount(), 2);
    ed->append(QString("   "));
    QCOMPARE(blockCount(), 3);
}

void tst_BOBUIextEdit::appendOnEmptyDocumentShouldReuseInitialParagraph()
{
    QCOMPARE(blockCount(), 1);
    ed->append("Blah");
    QCOMPARE(blockCount(), 1);
}

class CursorPositionChangedRecorder : public QObject
{
    Q_OBJECT
public:
    inline CursorPositionChangedRecorder(BOBUIextEdit *ed)
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
    BOBUIextEdit *editor;
};

void tst_BOBUIextEdit::cursorPositionChanged()
{
    QSignalSpy spy(ed, SIGNAL(cursorPositionChanged()));
    ed->show();

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

    cursor.movePosition(BOBUIextCursor::Start);
    ed->setTextCursor(cursor);
    spy.clear();
    QVERIFY(!ed->textCursor().hasSelection());
    BOBUIest::mouseDClick(ed->viewport(), BobUI::LeftButton, {}, ed->cursorRect().center());
    QVERIFY(ed->textCursor().hasSelection());

    QCOMPARE(spy.size(), 1);

    CursorPositionChangedRecorder spy2(ed);
    QVERIFY(ed->textCursor().position() > 0);
    ed->setPlainText("Hello World");
    QCOMPARE(spy2.cursorPositions.size(), 1);
    QCOMPARE(spy2.cursorPositions.at(0), 0);
    QCOMPARE(ed->textCursor().position(), 0);

    ed->selectAll();
    QCOMPARE(spy2.cursorPositions.size(), 2);
    QCOMPARE(spy2.cursorPositions.at(1), 11);
    QCOMPARE(ed->textCursor().position(), 11);
}

void tst_BOBUIextEdit::setTextCursor()
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
void tst_BOBUIextEdit::undoAvailableAfterPaste()
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

void tst_BOBUIextEdit::undoRedoAvailableRepetition()
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

void tst_BOBUIextEdit::appendShouldUseCurrentFormat()
{
    ed->textCursor().insertText("A");
    BOBUIextCharFormat fmt;
    fmt.setForeground(BobUI::blue);
    fmt.setFontItalic(true);
    ed->setCurrentCharFormat(fmt);
    ed->append("Hello");
    const QColor blue(BobUI::blue);

    BOBUIextCursor cursor(ed->document());

    QVERIFY(cursor.movePosition(BOBUIextCursor::NextCharacter));
    QVERIFY(cursor.charFormat().foreground().color() != blue);
    QVERIFY(!cursor.charFormat().fontItalic());

    QVERIFY(cursor.movePosition(BOBUIextCursor::NextBlock));

    {
        BOBUIextCursor tmp = cursor;
        tmp.movePosition(BOBUIextCursor::End, BOBUIextCursor::KeepAnchor);
        QCOMPARE(tmp.selectedText(), QString::fromLatin1("Hello"));
    }

    QVERIFY(cursor.movePosition(BOBUIextCursor::NextCharacter));
    QCOMPARE(cursor.charFormat().foreground().color(), blue);
    QVERIFY(cursor.charFormat().fontItalic());
}

void tst_BOBUIextEdit::appendShouldNotTouchTheSelection()
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

    ed->append("<b>Some Bold Text</b>");
    cursor.movePosition(BOBUIextCursor::Start);
    cursor.movePosition(BOBUIextCursor::NextCharacter);
    QCOMPARE(cursor.charFormat().foreground().color(), QColor(BobUI::blue));
}

void tst_BOBUIextEdit::backspace()
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

void tst_BOBUIextEdit::shiftBackspace()
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

void tst_BOBUIextEdit::undoRedo()
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
void tst_BOBUIextEdit::preserveCharFormatInAppend()
{
    ed->append("First para");
    ed->append("<b>Second para</b>");
    ed->append("third para");

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
void tst_BOBUIextEdit::copyAndSelectAllInReadonly()
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

void tst_BOBUIextEdit::noPropertiesOnDefaultTextEditCharFormat()
{
    // there should be no properties set on the default/initial char format
    // on a text edit. Font properties instead should be taken from the
    // widget's font (in sync with defaultFont property in document) and the
    // foreground color should be taken from the palette.
    QCOMPARE(ed->currentCharFormat().properties().size(), 0);
}

void tst_BOBUIextEdit::setPlainTextShouldUseCurrentCharFormat()
{
    ed->setFontUnderline(true);
    ed->setPlainText("Hello World");
    BOBUIextCursor cursor(ed->document());
    cursor.movePosition(BOBUIextCursor::NextCharacter);
    QCOMPARE(cursor.charFormat().fontUnderline(), true);

    ed->setHtml("<p style=\"color:blue\">Foo</p>");
    ed->setPlainText("Hello World");
    cursor = BOBUIextCursor(ed->document());
    cursor.movePosition(BOBUIextCursor::NextCharacter);
    QCOMPARE(cursor.charFormat().fontUnderline(), true);
}

void tst_BOBUIextEdit::setPlainTextShouldEmitTextChangedOnce()
{
    QSignalSpy spy(ed, SIGNAL(textChanged()));
    ed->setPlainText("Yankee Doodle");
    QCOMPARE(spy.size(), 1);
    ed->setPlainText("");
    QCOMPARE(spy.size(), 2);
}

void tst_BOBUIextEdit::overwriteMode()
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

void tst_BOBUIextEdit::shiftDownInLineLastShouldSelectToEnd_data()
{
    // shift cursor-down in the last line should select to the end of the document

    BOBUIest::addColumn<QString>("input");
    BOBUIest::addColumn<int>("totalLineCount");

    BOBUIest::newRow("1") << QString("Foo\nBar") << 2;
    BOBUIest::newRow("2") << QString("Foo\nBar") + QChar(QChar::LineSeparator) + QString("Baz") << 3;
}

void tst_BOBUIextEdit::shiftDownInLineLastShouldSelectToEnd()
{
    QFETCH(QString, input);
    QFETCH(int, totalLineCount);

    ed->setPlainText(input);
    ed->show();

    // ensure we're layouted
    ed->document()->documentLayout()->documentSize();

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

void tst_BOBUIextEdit::undoRedoShouldRepositionTextEditCursor()
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

void tst_BOBUIextEdit::lineWrapModes()
{
    ed->setLineWrapMode(BOBUIextEdit::NoWrap);
    // NoWrap at the same time as having all lines that are all left aligned means we optimize to only layout once. The effect is that the width is always 0
    QCOMPARE(ed->document()->pageSize().width(), qreal(0));

    BOBUIextCursor cursor = BOBUIextCursor(ed->document());
    cursor.insertText(QString("A simple line"));
    cursor.insertBlock();
    BOBUIextBlockFormat fmt;
    fmt.setAlignment(BobUI::AlignRight);
    cursor.mergeBlockFormat(fmt);
    cursor.insertText(QString("Another line"));
    ed->show(); // relayout;
    QVERIFY(ed->document()->pageSize().width() > qreal(0));

    ed->setLineWrapColumnOrWidth(10);
    ed->setLineWrapMode(BOBUIextEdit::FixedColumnWidth);
    QVERIFY(!qIsNull(ed->document()->pageSize().width()));

    ed->setLineWrapColumnOrWidth(1000);
    ed->setLineWrapMode(BOBUIextEdit::FixedPixelWidth);
    QCOMPARE(ed->document()->pageSize().width(), qreal(1000));
}

#ifndef BOBUI_NO_CURSOR
void tst_BOBUIextEdit::mouseCursorShape()
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

void tst_BOBUIextEdit::implicitClear()
{
    // test that BOBUIextEdit::setHtml, etc. avoid calling clear() but instead call
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
void tst_BOBUIextEdit::copyAvailable_data()
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
void tst_BOBUIextEdit::copyAvailable()
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

void tst_BOBUIextEdit::undoRedoAfterSetContent()
{
    QVERIFY(!ed->document()->isUndoAvailable());
    QVERIFY(!ed->document()->isRedoAvailable());
    ed->setPlainText("Foobar");
    QVERIFY(!ed->document()->isUndoAvailable());
    QVERIFY(!ed->document()->isRedoAvailable());
    ed->setHtml("<p>bleh</p>");
    QVERIFY(!ed->document()->isUndoAvailable());
    QVERIFY(!ed->document()->isRedoAvailable());
}

void tst_BOBUIextEdit::numPadKeyNavigation()
{
    ed->setText("Hello World");
    QCOMPARE(ed->textCursor().position(), 0);
    BOBUIest::keyClick(ed, BobUI::Key_Right, BobUI::KeypadModifier);
    QCOMPARE(ed->textCursor().position(), 1);
}

void tst_BOBUIextEdit::moveCursor()
{
    ed->setText("Test");

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

class MyTextEdit : public BOBUIextEdit
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
        return BOBUIextEdit::createMimeDataFromSelection();
    }
    virtual bool canInsertFromMimeData(const QMimeData *source) const override {
        canInsertCallCount++;
        return BOBUIextEdit::canInsertFromMimeData(source);
    }
    virtual void insertFromMimeData(const QMimeData *source) override {
        insertCallCount++;
        BOBUIextEdit::insertFromMimeData(source);
    }

};

#ifndef BOBUI_NO_CLIPBOARD
void tst_BOBUIextEdit::mimeDataReimplementations()
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

#ifndef BOBUI_NO_TEXTHTMLPARSER
void tst_BOBUIextEdit::mimeTypesAvailableFromRichText()
{
    MyTextEdit ed;
    ed.setHtml("<i>Hello <b>World</b></i>");
    ed.selectAll();
    ed.copy();
    const auto *mimeData = QApplication::clipboard()->mimeData();
    qCDebug(lcTests) << "available mime types" << mimeData->formats();
    QVERIFY(mimeData->formats().contains("text/plain"));
#if BOBUI_CONFIG(textmarkdownwriter)
    QVERIFY(mimeData->formats().contains("text/markdown"));
    const QByteArray expectedMarkdown = "*Hello **World***\n\n";
    if (mimeData->data("text/markdown") != expectedMarkdown && isMainFontFixed())
        QEXPECT_FAIL("", "fixed-pitch main font (BOBUIBUG-103484)", Continue);
    QCOMPARE(mimeData->data("text/markdown"), expectedMarkdown);
#endif
#ifndef BOBUI_NO_TEXTHTMLPARSER
    QVERIFY(mimeData->formats().contains("text/html"));
    QVERIFY(mimeData->hasHtml());
#endif
#ifndef BOBUI_NO_TEXTODFWRITER
    QVERIFY(mimeData->formats().contains("application/vnd.oasis.opendocument.text"));
#endif
}
#endif // BOBUI_NO_TEXTHTMLPARSER

#if BOBUI_CONFIG(textmarkdownreader)
void tst_BOBUIextEdit::mimeTypesAvailableFromMarkdown()
{
    MyTextEdit ed;
    const QString md("# TODO\n\n- [x] Fix bugs\n- [ ] Have a beer\n");
    ed.setMarkdown(md);
    ed.selectAll();
    ed.copy();
    const auto *mimeData = QApplication::clipboard()->mimeData();
    qCDebug(lcTests) << "available mime types" << mimeData->formats();
    QVERIFY(mimeData->formats().contains("text/plain"));
#if BOBUI_CONFIG(textmarkdownwriter)
    QVERIFY(mimeData->formats().contains("text/markdown"));
    if (mimeData->data("text/markdown") != md && isMainFontFixed())
        QEXPECT_FAIL("", "fixed-pitch main font (BOBUIBUG-103484)", Continue);
    QCOMPARE(mimeData->data("text/markdown"), md);
#endif
#ifndef BOBUI_NO_TEXTHTMLPARSER
    QVERIFY(mimeData->formats().contains("text/html"));
    QVERIFY(mimeData->hasHtml());
    QVERIFY(mimeData->html().contains("checked")); // <li class=\"checked\" ...
#endif
#ifndef BOBUI_NO_TEXTODFWRITER
    QVERIFY(mimeData->formats().contains("application/vnd.oasis.opendocument.text"));
#endif
}
#endif // textmarkdownreader

#endif // BOBUI_NO_CLIPBOARD

void tst_BOBUIextEdit::ctrlEnterShouldInsertLineSeparator_NOT()
{
    BOBUIest::keyClick(ed, BobUI::Key_A);
    BOBUIest::keyClick(ed, BobUI::Key_Enter, BobUI::ControlModifier);
    BOBUIest::keyClick(ed, BobUI::Key_B);
    QString expected;
    expected += 'a';
//    expected += QChar::LineSeparator; // do NOT insert
    expected += 'b';
    QCOMPARE(ed->textCursor().block().text(), expected);
}

void tst_BOBUIextEdit::shiftEnterShouldInsertLineSeparator()
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

void tst_BOBUIextEdit::selectWordsFromStringsContainingSeparators_data()
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

void tst_BOBUIextEdit::selectWordsFromStringsContainingSeparators()
{
    QFETCH(QString, testString);
    QFETCH(QString, selectedWord);
    ed->setText(testString);
    BOBUIextCursor cursor = ed->textCursor();
    cursor.movePosition(BOBUIextCursor::StartOfLine);
    cursor.select(BOBUIextCursor::WordUnderCursor);
    QVERIFY(cursor.hasSelection());
    QCOMPARE(cursor.selection().toPlainText(), selectedWord);
    cursor.clearSelection();
}

#ifndef BOBUI_NO_CLIPBOARD
void tst_BOBUIextEdit::canPaste()
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

void tst_BOBUIextEdit::ensureCursorVisibleOnInitialShow()
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

class TestEdit : public BOBUIextEdit
{
public:
    TestEdit() : resizeEventCalled(false) {}

    bool resizeEventCalled;

protected:
    virtual void resizeEvent(QResizeEvent *e) override
    {
        BOBUIextEdit::resizeEvent(e);
        setHtml("<img src=bobuiextbrowser-resizeevent.png width=" + QString::number(size().width()) + "><br>Size is " + QString::number(size().width()) + " x " + QString::number(size().height()));
        resizeEventCalled = true;
    }
};

void tst_BOBUIextEdit::setHtmlInsideResizeEvent()
{
    TestEdit edit;
    edit.show();
    edit.resize(800, 600);
    QVERIFY(edit.resizeEventCalled);
}

void tst_BOBUIextEdit::colorfulAppend()
{
    ed->setTextColor(BobUI::red);
    ed->append("Red");
    ed->setTextColor(BobUI::blue);
    ed->append("Blue");
    ed->setTextColor(BobUI::green);
    ed->append("Green");

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

void tst_BOBUIextEdit::ensureVisibleWithRtl()
{
    ed->setLayoutDirection(BobUI::RightToLeft);
    ed->setLineWrapMode(BOBUIextEdit::NoWrap);
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

void tst_BOBUIextEdit::preserveCharFormatAfterSetPlainText()
{
    ed->setTextColor(BobUI::blue);
    ed->setPlainText("This is blue");
    ed->append("This should still be blue");
    BOBUIextBlock block = ed->document()->begin();
    block = block.next();
    QCOMPARE(block.text(), QString("This should still be blue"));
    QCOMPARE(block.begin().fragment().charFormat().foreground().color(), QColor(BobUI::blue));
}

void tst_BOBUIextEdit::extraSelections()
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

void tst_BOBUIextEdit::adjustScrollbars()
{
// For some reason ff is defined to be << on Mac Panther / gcc 3.3
#undef ff
    QFont ff(ed->font());
    ff.setFamily("Tahoma");
    ff.setPointSize(11);
    ed->setFont(ff);
    ed->setMinimumSize(140, 100);
    ed->setMaximumSize(140, 100);
    ed->show();
    QLatin1String txt("\nabc def ghi jkl mno pqr stu vwx");
    ed->setText(txt + txt + txt + txt);

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

void tst_BOBUIextEdit::currentCharFormatChanged()
{
    QFont ff(ed->font());
    ff.setFamily("Tahoma");
    ff.setPointSize(11);

    SignalReceiver receiver;
    QObject::connect(ed, SIGNAL(currentCharFormatChanged(BOBUIextCharFormat)) , &receiver, SLOT(charFormatChanged(BOBUIextCharFormat)));

    ed->show();
    ed->setCurrentFont(ff);

    QVERIFY(receiver.receivedSignals() > 0);
    QCOMPARE(receiver.charFormat().font(), ff);
}

void tst_BOBUIextEdit::textObscuredByScrollbars()
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

void tst_BOBUIextEdit::setTextPreservesUndoRedoEnabled()
{
    QVERIFY(ed->isUndoRedoEnabled());

    ed->setPlainText("Test");

    QVERIFY(ed->isUndoRedoEnabled());

    ed->setUndoRedoEnabled(false);
    QVERIFY(!ed->isUndoRedoEnabled());
    ed->setPlainText("Test2");
    QVERIFY(!ed->isUndoRedoEnabled());

    ed->setHtml("<p>hello");
    QVERIFY(!ed->isUndoRedoEnabled());
}

void tst_BOBUIextEdit::wordWrapProperty()
{
    {
        BOBUIextEdit edit;
        BOBUIextDocument *doc = new BOBUIextDocument(&edit);
        edit.setDocument(doc);
        edit.setWordWrapMode(BOBUIextOption::NoWrap);
        QCOMPARE(doc->defaultTextOption().wrapMode(), BOBUIextOption::NoWrap);
    }
    {
        BOBUIextEdit edit;
        BOBUIextDocument *doc = new BOBUIextDocument(&edit);
        edit.setWordWrapMode(BOBUIextOption::NoWrap);
        edit.setDocument(doc);
        QCOMPARE(doc->defaultTextOption().wrapMode(), BOBUIextOption::NoWrap);
    }
}

void tst_BOBUIextEdit::lineWrapProperty()
{
    QCOMPARE(ed->wordWrapMode(), BOBUIextOption::WrapAtWordBoundaryOrAnywhere);
    QCOMPARE(ed->lineWrapMode(), BOBUIextEdit::WidgetWidth);
    ed->setLineWrapMode(BOBUIextEdit::NoWrap);
    QCOMPARE(ed->lineWrapMode(), BOBUIextEdit::NoWrap);
    QCOMPARE(ed->wordWrapMode(), BOBUIextOption::WrapAtWordBoundaryOrAnywhere);
    QCOMPARE(ed->document()->defaultTextOption().wrapMode(), BOBUIextOption::NoWrap);
}

void tst_BOBUIextEdit::selectionChanged()
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

#ifndef BOBUI_NO_CLIPBOARD
void tst_BOBUIextEdit::copyPasteBackgroundImage()
{
    if (!PlatformClipboard::isAvailable())
        QSKIP("Native clipboard not working in this setup");

    if (QGuiApplication::platformName().startsWith(QLatin1String("wayland"), BobUI::CaseInsensitive))
        QSKIP("Wayland: This fails. Figure out why.");

    QImage foo(16, 16, QImage::Format_ARGB32_Premultiplied);
    foo.save("foo.png");
    ed->setHtml("<body><table><tr><td background=\"foo.png\">Foo</td></tr></table></body>");

    ed->moveCursor(BOBUIextCursor::Start);
    ed->moveCursor(BOBUIextCursor::End, BOBUIextCursor::KeepAnchor);

    ed->copy();

    ed->moveCursor(BOBUIextCursor::End, BOBUIextCursor::MoveAnchor);
    ed->paste();

    ed->moveCursor(BOBUIextCursor::Start);

    ed->moveCursor(BOBUIextCursor::NextBlock);
    BOBUIextTable *a = ed->textCursor().currentTable();

    ed->moveCursor(BOBUIextCursor::End);
    ed->moveCursor(BOBUIextCursor::PreviousBlock);
    BOBUIextTable *b = ed->textCursor().currentTable();

    QVERIFY(a);
    QVERIFY(b);
    QVERIFY(a != b);

    QBrush ba = a->cellAt(0, 0).format().background();
    QBrush bb = b->cellAt(0, 0).format().background();

    QCOMPARE(ba.style(), BobUI::TexturePattern);
    QCOMPARE(ba.style(), bb.style());

    // we don't want a copy/paste of the background image to cause
    // a new image/pixmap to be created, it should use the cached resource
    // already in the document
    QVERIFY(ba.textureImage().cacheKey() == bb.textureImage().cacheKey() ||
            ba.texture().cacheKey() == bb.texture().cacheKey());
    QFile::remove(QLatin1String("foo.png"));
}

void tst_BOBUIextEdit::copyPasteForegroundImage()
{
    ed->clear();

    QPixmap pix(20, 20);
    pix.fill(BobUI::blue);

    BOBUIextCharFormat fmt;
    {
        QBrush textureBrush;
        {
            textureBrush.setTexture(pix);
        }
        textureBrush.setStyle(BobUI::TexturePattern);
        fmt.setForeground(textureBrush);
    }
    ed->textCursor().insertText("Foobar", fmt);

    ed->moveCursor(BOBUIextCursor::Start);
    ed->moveCursor(BOBUIextCursor::End, BOBUIextCursor::KeepAnchor);

    ed->copy();
    ed->clear();
    ed->paste();

    QBrush brush = ed->textCursor().charFormat().foreground();
    QCOMPARE(brush.style(), BobUI::TexturePattern);
    QCOMPARE(brush.texture().cacheKey(), pix.cacheKey());
}
#endif

void tst_BOBUIextEdit::setText()
{
    BOBUIextEdit browser;
    browser.setText(QLatin1String("hello"));
    QCOMPARE(browser.toPlainText(), QLatin1String("hello"));
    browser.setText(QLatin1String("<b>bold   bold</b>"));
    QCOMPARE(browser.toPlainText(), QLatin1String("bold bold"));
    browser.setText(QLatin1String("with   space"));
    QCOMPARE(browser.toPlainText(), QLatin1String("with   space"));
}

#ifdef BOBUI_BUILD_INTERNAL
BOBUI_BEGIN_NAMESPACE
// qfontdatabase.cpp
Q_AUTOTEST_EXPORT void bobui_setBobUIEnableTestFont(bool value);
BOBUI_END_NAMESPACE
#endif

#ifdef BOBUI_BUILD_INTERNAL
void tst_BOBUIextEdit::fullWidthSelection_data()
{
    BOBUIest::addColumn<int>("cursorFrom");
    BOBUIest::addColumn<int>("cursorTo");
    BOBUIest::addColumn<QString>("imageFileName");

    BOBUIest::newRow("centered fully selected")
        << 0 << 15 << (m_fullWidthSelectionImagesFolder + QStringLiteral("/centered-fully-selected.png"));
    BOBUIest::newRow("centered partly selected")
       << 2 << 15 << (m_fullWidthSelectionImagesFolder + QStringLiteral("/centered-partly-selected.png"));
    BOBUIest::newRow("last char on line")
       << 42 << 44 << (m_fullWidthSelectionImagesFolder + QStringLiteral("/last-char-on-line.png"));
    BOBUIest::newRow("last char on parag")
       << 545 << 548 << (m_fullWidthSelectionImagesFolder + QStringLiteral("/last-char-on-parag.png"));
    BOBUIest::newRow("multiple full width lines")
       << 20 << 60 << (m_fullWidthSelectionImagesFolder + QStringLiteral("/multiple-full-width-lines.png"));
    BOBUIest::newRow("single full width line")
       << 20 << 30 << (m_fullWidthSelectionImagesFolder + QStringLiteral("/single-full-width-line.png"));
}
#endif

#ifdef BOBUI_BUILD_INTERNAL

// With the fix for BOBUIBUG-78318 scaling of documentMargin is added. The testing framework
// forces bobui_defaultDpi() to always return 96 DPI. For systems where the actual DPI differs
// (typically 72 DPI) this would now cause scaling of the documentMargin when
// drawing BOBUIextEdit into QImage. In order to avoid the need of multiple reference PNGs
// for comparison we disable the BobUI::AA_Use96Dpi attribute for these tests.

struct ForceSystemDpiHelper {
  ForceSystemDpiHelper() { QCoreApplication::setAttribute(BobUI::AA_Use96Dpi, false); }
  ~ForceSystemDpiHelper() { QCoreApplication::setAttribute(BobUI::AA_Use96Dpi, old); }
  bool old = QCoreApplication::testAttribute(BobUI::AA_Use96Dpi);
};

void tst_BOBUIextEdit::fullWidthSelection()
{
    ForceSystemDpiHelper useSystemDpi;

    QFETCH(int, cursorFrom);
    QFETCH(int, cursorTo);
    QFETCH(QString, imageFileName);

    // enable full-width-selection for our test widget.
    class FullWidthStyle : public QCommonStyle {
        int styleHint(StyleHint stylehint, const QStyleOption *opt, const QWidget *widget, QStyleHintReturn *returnData) const override
        {
            if (stylehint == QStyle::SH_RichText_FullWidthSelection)
                return 1;
            return QCommonStyle::styleHint(stylehint, opt, widget, returnData);
        };
    };
    FullWidthStyle myStyle;

    QPalette myPalette = myStyle.standardPalette();
    myPalette.setColor(QPalette::All, QPalette::HighlightedText, QColor(0,0,0,0));
    myPalette.setColor(QPalette::All, QPalette::Highlight, QColor(239,221,85));

    BOBUIextEdit widget;
    widget.document()->setDocumentMargin(2);
    widget.setPalette(myPalette);
    widget.setStyle(&myStyle);
    BOBUIextCursor cursor = widget.textCursor();
    BOBUIextBlockFormat bf1;
    bf1.setAlignment(BobUI::AlignCenter);
    cursor.setBlockFormat(bf1);

    // use the test font so we always know where stuff will end up.
    bobui_setBobUIEnableTestFont(true);
    QFont testFont;
    testFont.setFamily("__BobUI__Box__Engine__");
    testFont.setPixelSize(11);
    testFont.setWeight(QFont::Normal);
    BOBUIextCharFormat cf;
    cf.setFont(testFont);
    cf.setForeground(QColor(0,0,0,0)); // tricky bit, this :)
    cursor.setCharFormat(cf);

    // populate with some demo text.
    cursor.insertText("centered");
    BOBUIextBlockFormat bf;
    cursor.insertBlock(bf, cf);
    cursor.insertText("Duis autem vel eum iriure dolor in hendrerit in vulputate velit esse molestie consequat, vel illum dolore eu feugiat nulla facilisis at vero eros et accumsan et iusto odio dignissim qui blandit praesent luptatum zzril delenit augue duis dolore te feugait nulla facilisi. Lorem ipsum dolor sit amet, consectetuer adipiscing elit, sed diam nonummy nibh euismod tincidunt ut laoreet dolore magna aliquam erat volutpat. Ut wisi enim ad minim veniam, quis nostrud exerci tation ullamcorper suscipit lobortis nisl ut aliquip ex ea commodo cons.\nfoo\n");

    // Usecase 1 for full-width-selections; the 'show-cursor-position' one.
    QList<BOBUIextEdit::ExtraSelection> selections;
    BOBUIextCursor cursor2(widget.document());
    BOBUIextEdit::ExtraSelection es;
    cursor2.setPosition(300);
    es.cursor = cursor2;
    es.format.setProperty( BOBUIextFormat::FullWidthSelection, true );
    es.format.setBackground(QColor(255, 0, 0));
    selections.append(es);
    widget.setExtraSelections(selections);

    // Usecase 2; show it based on the style.
    // Select part of the centered text and part of the next; which means we should show the part right of the centered text.
    cursor.setPosition(cursorFrom);
    cursor.setPosition(cursorTo, BOBUIextCursor::KeepAnchor);
    widget.setTextCursor(cursor);

    compareWidgetAndImage(widget, imageFileName);
}
#endif

#ifdef BOBUI_BUILD_INTERNAL
void tst_BOBUIextEdit::fullWidthSelection2()
{
    ForceSystemDpiHelper useSystemDpi;

    QPalette myPalette;
    myPalette.setColor(QPalette::All, QPalette::HighlightedText, QColor(0,0,0,0));
    myPalette.setColor(QPalette::All, QPalette::Highlight, QColor(239,221,85));
    myPalette.setColor(QPalette::All, QPalette::Base, QColor(255,255,255));

    BOBUIextEdit widget;
    widget.setPalette(myPalette);
    widget.setLineWrapMode(BOBUIextEdit::NoWrap);
    BOBUIextCursor cursor = widget.textCursor();

    // use the test font so we always know where stuff will end up.
    bobui_setBobUIEnableTestFont(true);
    QFont testFont;
    testFont.setFamily("__BobUI__Box__Engine__");
    testFont.setPixelSize(11);
    testFont.setWeight(QFont::Normal);
    BOBUIextCharFormat cf;
    cf.setFont(testFont);
    cf.setForeground(QColor(0,0,0,0)); // tricky bit, this :)
    cursor.setCharFormat(cf);
    cursor.insertText("foo");

    QList<BOBUIextEdit::ExtraSelection> selections;
    BOBUIextCursor cursor2(widget.document());
    BOBUIextEdit::ExtraSelection es;
    cursor2.setPosition(1);
    es.cursor = cursor2;
    es.format.setProperty( BOBUIextFormat::FullWidthSelection, true );
    es.format.setBackground(QColor(255, 0, 0));
    selections.append(es);
    widget.setExtraSelections(selections);

    compareWidgetAndImage(widget, m_fullWidthSelectionImagesFolder + "/nowrap_long.png");
}
#endif

void tst_BOBUIextEdit::compareWidgetAndImage(BOBUIextEdit &widget, const QString &imageFileName)
{
    widget.setGeometry(0, 0, 300, 390);
    widget.setFrameShape(QFrame::NoFrame);

    QImage image(300, 390, QImage::Format_RGB32);
    QPainter painter(&image);
    widget.viewport()->render(&painter);
    painter.end();
    QImageReader reader(imageFileName, "PNG");

    QImage original = reader.read();

    QVERIFY2(!original.isNull(),
             qPrintable(QString::fromLatin1("Unable to read image %1: %2").arg(imageFileName, reader.errorString())));
    QCOMPARE(original.size(), image.size());
    QCOMPARE(image.depth(), 32);
    QCOMPARE(original.depth(), image.depth());

    const qsizetype bytesPerLine = image.bytesPerLine();
    const int width = image.width();
    const int height = image.height();

    for (int y = 0; y < height; ++y) {
        const QRgb * const firstLine = reinterpret_cast<const QRgb *>(image.scanLine(y));
        const QRgb * const secondLine = reinterpret_cast<const QRgb *>(original.scanLine(y));

        if (memcmp(firstLine, secondLine, bytesPerLine) != 0) {
            for (int x = 0; x < width; ++x) {
                const QRgb a = firstLine[x];
                const QRgb b = secondLine[x];
                const bool same = qAbs(qRed(a) - qRed(b)) <= 20
                    && qAbs(qGreen(a) - qGreen(b)) <= 20
                    && qAbs(qBlue(a) - qBlue(b)) <= 20;
                if (!same) {
                    QString fileName = imageFileName;
                    QImageWriter writer("failed_"+ fileName.replace('/','_'), "PNG");
                    writer.write(image);
                }
                QVERIFY(same);
            }
        }
    }
}

bool tst_BOBUIextEdit::isMainFontFixed()
{
    bool ret = QFontInfo(QGuiApplication::font()).fixedPitch();
    if (ret) {
        qCWarning(lcTests) << "QFontDatabase::GeneralFont is monospaced: markdown writing is likely to use too many backticks";
        qCWarning(lcTests) << "system fonts: fixed" << QFontDatabase::systemFont(QFontDatabase::FixedFont)
                           << "fixed?" << QFontInfo(QFontDatabase::systemFont(QFontDatabase::FixedFont)).fixedPitch()
                           << "general" << QFontDatabase::systemFont(QFontDatabase::GeneralFont);
    }
    return ret;
}

void tst_BOBUIextEdit::cursorRect()
{
    ed->show();
    ed->setPlainText("Hello Test World");
    ed->setCursorWidth(1);
    QCOMPARE(ed->cursorRect().width(), 1);
    ed->setCursorWidth(2);
    QCOMPARE(ed->cursorRect().width(), 2);
    ed->setCursorWidth(4);
    QCOMPARE(ed->cursorRect().width(), 4);
    ed->setCursorWidth(10);
    QCOMPARE(ed->cursorRect().width(), 10);
}

#ifdef BOBUI_BUILD_INTERNAL
void tst_BOBUIextEdit::setDocumentPreservesPalette()
{
    QWidgetTextControl *control = ed->findChild<QWidgetTextControl *>();
    QVERIFY(control);

    QPalette whitePal = ed->palette();
    whitePal.setColor(QPalette::Active, QPalette::Text, BobUI::white);


    QVERIFY(whitePal != ed->palette());
    ed->setPalette(whitePal);
    QVERIFY(whitePal.color(QPalette::Active, QPalette::Text)
            == ed->palette().color(QPalette::Active, QPalette::Text));
    QVERIFY(whitePal.color(QPalette::Active, QPalette::Text)
            == control->palette().color(QPalette::Active, QPalette::Text));

    BOBUIextDocument *newDoc = new BOBUIextDocument(ed);
    ed->setDocument(newDoc);
    QCOMPARE(control->document(), newDoc);
    QVERIFY(whitePal.color(QPalette::Active, QPalette::Text)
            == control->palette().color(QPalette::Active, QPalette::Text));
}
#endif

void tst_BOBUIextEdit::pasteFromBobUI3RichText()
{
    QByteArray richtext("<!--StartFragment--><p>  BOBUIextEdit is an  ");

    QMimeData mimeData;
    mimeData.setData("application/x-qrichtext", richtext);

    ed->insertFromMimeData(&mimeData);

    QCOMPARE(ed->toPlainText(), QString::fromLatin1("  BOBUIextEdit is an  "));
    ed->clear();

    richtext = "<!--StartFragment-->  BOBUIextEdit is an  ";
    mimeData.setData("application/x-qrichtext", richtext);

    ed->insertFromMimeData(&mimeData);

    QCOMPARE(ed->toPlainText(), QString::fromLatin1("  BOBUIextEdit is an  "));
}

void tst_BOBUIextEdit::pasteFromMarkdown()
{
    QByteArray richtext("*This* text is **rich**");

    QMimeData mimeData;
    mimeData.setData("text/markdown", richtext);

    ed->insertFromMimeData(&mimeData);

    QCOMPARE(ed->toPlainText(), "This text is rich");
#if BOBUI_CONFIG(textmarkdownwriter)
    const auto expectedMarkdown = QString::fromLatin1(richtext + "\n\n");
    if (ed->toMarkdown() != expectedMarkdown && isMainFontFixed())
        QEXPECT_FAIL("", "fixed-pitch main font (BOBUIBUG-103484)", Continue);
    QCOMPARE(ed->toMarkdown(), expectedMarkdown);
#endif
}

void tst_BOBUIextEdit::noWrapBackgrounds()
{
    QWidget topLevel;
    QVBoxLayout *layout = new QVBoxLayout(&topLevel);

    BOBUIextEdit edit;
    edit.setLineWrapMode(BOBUIextEdit::NoWrap);

    // hide the cursor in order to make the image comparison below reliable
    edit.setCursorWidth(0);

    BOBUIextFrame *root = edit.document()->rootFrame();
    BOBUIextFrameFormat frameFormat = root->frameFormat();
    frameFormat.setLeftMargin(2);
    frameFormat.setRightMargin(2);
    root->setFrameFormat(frameFormat);

    BOBUIextBlockFormat format;
    format.setBackground(BobUI::red);
    edit.textCursor().setBlockFormat(format);
    edit.insertPlainText(QLatin1String(" \n  \n   \n    \n"));
    edit.setFixedSize(100, 200);

    layout->addWidget(&edit);
    topLevel.show();

    const QImage img = edit.viewport()->grab().toImage();
    QCOMPARE(img, img.flipped(BobUI::Horizontal));
}

void tst_BOBUIextEdit::preserveCharFormatAfterUnchangingSetPosition()
{
    QColor color(BobUI::yellow);
    BOBUIextEdit edit;
    edit.setTextColor(color);

    BOBUIextCursor c = edit.textCursor();
    c.setPosition(c.position());
    edit.setTextCursor(c);

    QCOMPARE(edit.textColor(), color);
}

// Regression test for BOBUIBUG-4696
void tst_BOBUIextEdit::twoSameInputMethodEvents()
{
    ed->setText("testLine");
    ed->show();
    QList<QInputMethodEvent::Attribute> attributes;
    attributes.append(QInputMethodEvent::Attribute(QInputMethodEvent::Cursor,
                                                   ed->textCursor().position(),
                                                   0,
                                                   QVariant()));

    QInputMethodEvent event("PreEditText", attributes);
    QApplication::sendEvent(ed, &event);
    BOBUIRY_COMPARE(ed->document()->firstBlock().layout()->lineCount(), 1);
    QApplication::sendEvent(ed, &event);
    QCOMPARE(ed->document()->firstBlock().layout()->lineCount(), 1);
}

#ifndef BOBUI_NO_CONTEXTMENU
void tst_BOBUIextEdit::taskBOBUIBUG_7902_contextMenuCrash()
{
    BOBUIextEdit *w = new BOBUIextEdit;
    w->show();
    QVERIFY(BOBUIest::qWaitForWindowExposed(w));

    BOBUIimer ti;
    w->connect(&ti, SIGNAL(timeout()), w, SLOT(deleteLater()));
    ti.start(200);

    QContextMenuEvent *cme = new QContextMenuEvent(QContextMenuEvent::Mouse, w->rect().center(),
                                                   w->viewport()->mapToGlobal(w->rect().center()));
    qApp->postEvent(w->viewport(), cme);

    BOBUIest::qWait(300);
    // No crash, it's allright.
}
#endif

void tst_BOBUIextEdit::bidiVisualMovement_data()
{
    BOBUIest::addColumn<QString>("logical");
    BOBUIest::addColumn<int>("basicDir");
    BOBUIest::addColumn<QList<int> >("positionList");

    BOBUIest::newRow("Latin text")
        << QString::fromUtf8("abc")
        << (int) QChar::DirL
        << (QList<int>() << 0 << 1 << 2 << 3);
    BOBUIest::newRow("Hebrew text, one item")
        << QString::fromUtf8("\327\220\327\221\327\222")
        << (int) QChar::DirR
        << (QList<int>() << 0 << 1 << 2 << 3);
    BOBUIest::newRow("Hebrew text after Latin text")
        << QString::fromUtf8("abc\327\220\327\221\327\222")
        << (int) QChar::DirL
        << (QList<int>() << 0 << 1 << 2 << 6 << 5 << 4 << 3);
    BOBUIest::newRow("Latin text after Hebrew text")
        << QString::fromUtf8("\327\220\327\221\327\222abc")
        << (int) QChar::DirR
        << (QList<int>() << 0 << 1 << 2 << 6 << 5 << 4 << 3);
    BOBUIest::newRow("LTR, 3 items")
        << QString::fromUtf8("abc\327\220\327\221\327\222abc")
        << (int) QChar::DirL
        << (QList<int>() << 0 << 1 << 2 << 5 << 4 << 3 << 6 << 7 << 8 << 9);
    BOBUIest::newRow("RTL, 3 items")
        << QString::fromUtf8("\327\220\327\221\327\222abc\327\220\327\221\327\222")
        << (int) QChar::DirR
        << (QList<int>() << 0 << 1 << 2 << 5 << 4 << 3 << 6 << 7 << 8 << 9);
    BOBUIest::newRow("LTR, 4 items")
        << QString::fromUtf8("abc\327\220\327\221\327\222abc\327\220\327\221\327\222")
        << (int) QChar::DirL
        << (QList<int>() << 0 << 1 << 2 << 5 << 4 << 3 << 6 << 7 << 8 << 12 << 11 << 10 << 9);
    BOBUIest::newRow("RTL, 4 items")
        << QString::fromUtf8("\327\220\327\221\327\222abc\327\220\327\221\327\222abc")
        << (int) QChar::DirR
        << (QList<int>() << 0 << 1 << 2 << 5 << 4 << 3 << 6 << 7 << 8 << 12 << 11 << 10 << 9);
}

void tst_BOBUIextEdit::bidiVisualMovement()
{
    QFETCH(QString,      logical);
    QFETCH(int,          basicDir);
    QFETCH(QList<int>,   positionList);

    ed->setText(logical);

    BOBUIextOption option = ed->document()->defaultTextOption();
    option.setTextDirection(basicDir == QChar::DirL ? BobUI::LeftToRight : BobUI::RightToLeft);
    ed->document()->setDefaultTextOption(option);

    ed->document()->setDefaultCursorMoveStyle(BobUI::VisualMoveStyle);
    ed->moveCursor(BOBUIextCursor::Start);
    ed->show();

    bool moved;
    int i = 0, oldPos, newPos = 0;

    do {
        oldPos = newPos;
        QCOMPARE(oldPos, positionList[i]);
        if (basicDir == QChar::DirL) {
            ed->moveCursor(BOBUIextCursor::Right);
        } else
        {
            ed->moveCursor(BOBUIextCursor::Left);
        }
        newPos = ed->textCursor().position();
        moved = (oldPos != newPos);
        i++;
    } while (moved);

    QCOMPARE(i, positionList.size());

    do {
        i--;
        oldPos = newPos;
        QCOMPARE(oldPos, positionList[i]);
        if (basicDir == QChar::DirL) {
            ed->moveCursor(BOBUIextCursor::Left);
        } else
        {
            ed->moveCursor(BOBUIextCursor::Right);
        }
        newPos = ed->textCursor().position();
        moved = (oldPos != newPos);
    } while (moved && i >= 0);
}

void tst_BOBUIextEdit::bidiLogicalMovement_data()
{
    bidiVisualMovement_data();
}

void tst_BOBUIextEdit::bidiLogicalMovement()
{
    QFETCH(QString,      logical);
    QFETCH(int,          basicDir);
    QFETCH(QList<int>,   positionList);

    ed->setText(logical);

    BOBUIextOption option = ed->document()->defaultTextOption();
    option.setTextDirection(basicDir == QChar::DirL ? BobUI::LeftToRight : BobUI::RightToLeft);
    ed->document()->setDefaultTextOption(option);

    ed->document()->setDefaultCursorMoveStyle(BobUI::LogicalMoveStyle);
    ed->moveCursor(BOBUIextCursor::Start);
    ed->show();

    bool moved;
    int i = 0, oldPos, newPos = 0;

    do {
        oldPos = newPos;
        QCOMPARE(oldPos, i);
        if (basicDir == QChar::DirL) {
            ed->moveCursor(BOBUIextCursor::Right);
        } else
        {
            ed->moveCursor(BOBUIextCursor::Left);
        }
        newPos = ed->textCursor().position();
        moved = (oldPos != newPos);
        i++;
    } while (moved);

    QCOMPARE(i, positionList.size());

    do {
        i--;
        oldPos = newPos;
        QCOMPARE(oldPos, i);
        if (basicDir == QChar::DirL) {
            ed->moveCursor(BOBUIextCursor::Left);
        } else
        {
            ed->moveCursor(BOBUIextCursor::Right);
        }
        newPos = ed->textCursor().position();
        moved = (oldPos != newPos);
    } while (moved && i >= 0);
}

void tst_BOBUIextEdit::inputMethodEvent()
{
    if (QGuiApplication::platformName().startsWith(QLatin1String("wayland"), BobUI::CaseInsensitive))
        QSKIP("Wayland: This fails. Figure out why.");

    ed->show();

    // test that text change with an input method event triggers change signal
    QSignalSpy spy(ed, SIGNAL(textChanged()));

    QInputMethodEvent event;
    event.setCommitString("text");
    QApplication::sendEvent(ed, &event);
    QCOMPARE(spy.size(), 1);
    QCOMPARE(ed->toPlainText(), QString("text"));

    // test that input method gets chance to commit preedit when removing focus
    ed->setText("");
    BOBUIRY_VERIFY(QApplication::focusWindow());
    QCOMPARE(qApp->focusObject(), ed);

    m_platformInputContext.setCommitString("text");
    m_platformInputContext.m_commitCallCount = 0;
    QList<QInputMethodEvent::Attribute> attributes;
    QInputMethodEvent preeditEvent("preedit text", attributes);
    QApplication::sendEvent(ed, &preeditEvent);

    ed->clearFocus();
    QCOMPARE(m_platformInputContext.m_commitCallCount, 1);
    QCOMPARE(ed->toPlainText(), QString("text"));
}

void tst_BOBUIextEdit::inputMethodSelection()
{
    ed->setText("Lorem ipsum dolor sit amet, consectetur adipiscing elit.");

    QSignalSpy selectionSpy(ed, SIGNAL(selectionChanged()));
    BOBUIextCursor cursor = ed->textCursor();
    cursor.setPosition(0);
    cursor.setPosition(5, BOBUIextCursor::KeepAnchor);
    ed->setTextCursor(cursor);

    QCOMPARE(selectionSpy.size(), 1);
    QCOMPARE(ed->textCursor().selectionStart(), 0);
    QCOMPARE(ed->textCursor().selectionEnd(), 5);

    QList<QInputMethodEvent::Attribute> attributes;
    attributes << QInputMethodEvent::Attribute(QInputMethodEvent::Selection, 12, 5, QVariant());
    QInputMethodEvent event("", attributes);
    QApplication::sendEvent(ed, &event);

    QCOMPARE(selectionSpy.size(), 2);
    QCOMPARE(ed->textCursor().selectionStart(), 12);
    QCOMPARE(ed->textCursor().selectionEnd(), 17);
}

void tst_BOBUIextEdit::inputMethodQuery()
{
    QString text("first line of text\nsecond line of text");
    ed->setText(text);
    ed->selectAll();

    QInputMethodQueryEvent event(BobUI::ImQueryInput | BobUI::ImEnabled);
    QGuiApplication::sendEvent(ed, &event);
    int anchor = event.value(BobUI::ImAnchorPosition).toInt();
    int position = event.value(BobUI::ImCursorPosition).toInt();
    QCOMPARE(qAbs(position - anchor), text.size());
    QCOMPARE(event.value(BobUI::ImEnabled).toBool(), true);

    ed->setEnabled(false);
    QGuiApplication::sendEvent(ed, &event);
    QCOMPARE(event.value(BobUI::ImEnabled).toBool(), false);
}

Q_DECLARE_METATYPE(BobUI::InputMethodHints)
void tst_BOBUIextEdit::inputMethodQueryImHints_data()
{
    BOBUIest::addColumn<BobUI::InputMethodHints>("hints");

    BOBUIest::newRow("None") << static_cast<BobUI::InputMethodHints>(BobUI::ImhNone);
    BOBUIest::newRow("Password") << static_cast<BobUI::InputMethodHints>(BobUI::ImhHiddenText);
    BOBUIest::newRow("Normal") << static_cast<BobUI::InputMethodHints>(BobUI::ImhNoAutoUppercase | BobUI::ImhNoPredictiveText | BobUI::ImhSensitiveData);
}

void tst_BOBUIextEdit::inputMethodQueryImHints()
{
    QFETCH(BobUI::InputMethodHints, hints);
    ed->setInputMethodHints(hints);

    QVariant value = ed->inputMethodQuery(BobUI::ImHints);
    QCOMPARE(static_cast<BobUI::InputMethodHints>(value.toInt()), hints);
}

// BOBUIBUG-51923: Verify that the cursor rectangle returned by the input
// method query correctly reflects the viewport offset.
void tst_BOBUIextEdit::inputMethodCursorRect()
{
    ed->setPlainText("Line1\nLine2Line3\nLine3");
    ed->moveCursor(BOBUIextCursor::End);
    const QRectF cursorRect = ed->cursorRect();
    const QVariant cursorRectV = ed->inputMethodQuery(BobUI::ImCursorRectangle);
    QCOMPARE(cursorRectV.userType(), QMetaType::QRectF);
    QCOMPARE(cursorRectV.toRect(), cursorRect.toRect());
}

void tst_BOBUIextEdit::highlightLongLine()
{
    if (QGuiApplication::platformName().startsWith(QLatin1String("wayland"), BobUI::CaseInsensitive))
        QSKIP("Wayland: This fails. Figure out why.");

    BOBUIextEdit edit;
    edit.setAcceptRichText(false);
    edit.setWordWrapMode(BOBUIextOption::NoWrap);

    QString singeLongLine;
    for (int i = 0; i < 10000; ++i)
        singeLongLine += "0123456789";
    edit.setPlainText(singeLongLine);

    class NumHighlighter : public QSyntaxHighlighter {
    public:
        explicit NumHighlighter(BOBUIextDocument*doc) : QSyntaxHighlighter(doc) {};
        virtual void highlightBlock(const QString& text) override {
            // odd number in bold
            BOBUIextCharFormat format;
            format.setFontWeight(QFont::Bold);
            for (int i = 0; i < text.size(); ++i) {
                if (text.at(i).unicode() % 2)
                    setFormat(i, 1, format);
            }
        }
    };
    NumHighlighter nh(edit.document());
    edit.show();
    QVERIFY(BOBUIest::qWaitForWindowActive(edit.windowHandle()));
    QCoreApplication::processEvents();
    //If there is a quadratic behaviour, this would take forever.
    QVERIFY(true);
}

//check for bug 15003, are there multiple textChanged() signals on remove?
void tst_BOBUIextEdit::countTextChangedOnRemove()
{
    BOBUIextEdit edit;
    edit.insertPlainText("Hello");

    QSignalSpy spy(&edit, SIGNAL(textChanged()));

    QKeyEvent event(QEvent::KeyPress, BobUI::Key_Backspace, BobUI::NoModifier);
    QCoreApplication::instance()->notify(&edit, &event);

    QCOMPARE(spy.size(), 1);
}

#if BOBUI_CONFIG(regularexpression)
void tst_BOBUIextEdit::findWithRegularExpression()
{
    ed->setHtml(QStringLiteral("arbitrary te<span style=\"color:#ff0000\">xt</span>"));
    QRegularExpression rx("\\w{2}xt");

    bool found = ed->find(rx);

    QVERIFY(found);
    QCOMPARE(ed->textCursor().selectedText(), QStringLiteral("text"));
}

void tst_BOBUIextEdit::findBackwardWithRegularExpression()
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

void tst_BOBUIextEdit::findWithRegularExpressionReturnsFalseIfNoMoreResults()
{
    ed->setPlainText(QStringLiteral("arbitrary text"));
    QRegularExpression rx("t.xt");
    ed->find(rx);

    bool found = ed->find(rx);

    QVERIFY(!found);
    QCOMPARE(ed->textCursor().selectedText(), QStringLiteral("text"));
}
#endif

#if BOBUI_CONFIG(wheelevent)

class TextEdit : public BOBUIextEdit
{
public:
    TextEdit(QWidget *parent = nullptr)
        : BOBUIextEdit(parent)
    {}
    void wheelEvent(QWheelEvent *event) override
    {
        BOBUIextEdit::wheelEvent(event);
    }
};

void tst_BOBUIextEdit::wheelEvent()
{
    TextEdit ed(0);
    ed.setPlainText(QStringLiteral("Line\nLine\nLine\n"));
    ed.setReadOnly(true);

    float defaultFontSize = ed.font().pointSizeF();
    QWheelEvent wheelUp(QPointF(), QPointF(), QPoint(), QPoint(0, 120),
                        BobUI::NoButton, BobUI::ControlModifier, BobUI::NoScrollPhase, BobUI::MouseEventNotSynthesized);
    ed.wheelEvent(&wheelUp);

    QCOMPARE(defaultFontSize + 1, ed.font().pointSizeF());

    QWheelEvent wheelHalfDown(QPointF(), QPointF(), QPoint(), QPoint(0, -60),
                              BobUI::NoButton, BobUI::ControlModifier, BobUI::NoScrollPhase, BobUI::MouseEventNotSynthesized);
    ed.wheelEvent(&wheelHalfDown);

    QCOMPARE(defaultFontSize + 0.5, ed.font().pointSizeF());
}

#endif

namespace {
    class MyPaintEngine : public QPaintEngine
    {
    public:
        bool begin(QPaintDevice *) override { return true; }

        bool end() override { return true; }

        void updateState(const QPaintEngineState &) override  { }

        void drawPixmap(const QRectF &, const QPixmap &, const QRectF &) override { }

        void drawTextItem(const QPointF &, const BOBUIextItem &textItem) override
        {
            itemFonts.append({textItem.text(), textItem.font()});
        }

        Type type() const override { return User; }


        QList<std::pair<QString, QFont> > itemFonts;
    };

    class MyPaintDevice : public QPaintDevice
    {
    public:
        MyPaintDevice() : m_paintEngine(std::make_unique<MyPaintEngine>())
        {
        }


        QPaintEngine *paintEngine () const override
        {
            return m_paintEngine.get();
        }

        int metric (QPaintDevice::PaintDeviceMetric metric) const override {
            switch (metric) {
            case QPaintDevice::PdmWidth:
            case QPaintDevice::PdmHeight:
            case QPaintDevice::PdmWidthMM:
            case QPaintDevice::PdmHeightMM:
            case QPaintDevice::PdmNumColors:
                return INT_MAX;
            case QPaintDevice::PdmDepth:
                return 32;
            case QPaintDevice::PdmDpiX:
            case QPaintDevice::PdmDpiY:
            case QPaintDevice::PdmPhysicalDpiX:
            case QPaintDevice::PdmPhysicalDpiY:
                return 72;
            case QPaintDevice::PdmDevicePixelRatio:
            case QPaintDevice::PdmDevicePixelRatioScaled:
            case QPaintDevice::PdmDevicePixelRatioF_EncodedA:
            case QPaintDevice::PdmDevicePixelRatioF_EncodedB:
                ; // fall through
            }
            return 0;
        }

        std::unique_ptr<MyPaintEngine> m_paintEngine;
    };
}

void tst_BOBUIextEdit::preeditCharFormat_data()
{
    BOBUIest::addColumn<QList<QInputMethodEvent::Attribute> >("imeAttributes");
    BOBUIest::addColumn<QStringList>("substrings");
    BOBUIest::addColumn<QList<bool> >("boldnessList");
    BOBUIest::addColumn<QList<bool> >("italicnessList");
    BOBUIest::addColumn<QList<int> >("pointSizeList");

    {
        QList<QInputMethodEvent::Attribute> attributes;
        {
            BOBUIextCharFormat tcf;
            tcf.setFontPointSize(13);
            tcf.setFontItalic(true);
            attributes.append(QInputMethodEvent::Attribute(QInputMethodEvent::TextFormat, 1, 1, tcf));
        }

        {
            BOBUIextCharFormat tcf;
            tcf.setFontPointSize(8);
            tcf.setFontWeight(QFont::Normal);
            attributes.append(QInputMethodEvent::Attribute(QInputMethodEvent::TextFormat, 4, 2, tcf));
        }

        BOBUIest::newRow("Two formats, middle, in order")
            << attributes
            << (QStringList() << "P" << "r" << "eE" << "di" << "tText")
            << (QList<bool>() << true << true << true << false << true)
            << (QList<bool>() << false << true << false << false << false)
            << (QList<int>() << 20 << 13 << 20 << 8 << 20);
    }

    {
        QList<QInputMethodEvent::Attribute> attributes;
        {
            BOBUIextCharFormat tcf;
            tcf.setFontPointSize(8);
            tcf.setFontWeight(QFont::Normal);
            attributes.append(QInputMethodEvent::Attribute(QInputMethodEvent::TextFormat, 4, 2, tcf));
        }

        {
            BOBUIextCharFormat tcf;
            tcf.setFontPointSize(13);
            tcf.setFontItalic(true);
            attributes.append(QInputMethodEvent::Attribute(QInputMethodEvent::TextFormat, 1, 1, tcf));
        }

        BOBUIest::newRow("Two formats, middle, out of order")
            << attributes
            << (QStringList() << "P" << "r" << "eE" << "di" << "tText")
            << (QList<bool>() << true << true << true << false << true)
            << (QList<bool>() << false << true << false << false << false)
            << (QList<int>() << 20 << 13 << 20 << 8 << 20);
    }

    {
        QList<QInputMethodEvent::Attribute> attributes;
        {
            BOBUIextCharFormat tcf;
            tcf.setFontPointSize(13);
            tcf.setFontItalic(true);
            attributes.append(QInputMethodEvent::Attribute(QInputMethodEvent::TextFormat, 0, 1, tcf));
        }

        {
            BOBUIextCharFormat tcf;
            tcf.setFontPointSize(8);
            tcf.setFontWeight(QFont::Normal);
            attributes.append(QInputMethodEvent::Attribute(QInputMethodEvent::TextFormat, 4, 2, tcf));
        }

        BOBUIest::newRow("Two formats, front, in order")
            << attributes
            << (QStringList() << "P" << "reE" << "di" << "tText")
            << (QList<bool>() << true << true << false << true)
            << (QList<bool>() << true << false << false << false)
            << (QList<int>() << 13 << 20 << 8 << 20);
    }

    {
        QList<QInputMethodEvent::Attribute> attributes;
        {
            BOBUIextCharFormat tcf;
            tcf.setFontPointSize(8);
            tcf.setFontWeight(QFont::Normal);
            attributes.append(QInputMethodEvent::Attribute(QInputMethodEvent::TextFormat, 4, 2, tcf));
        }

        {
            BOBUIextCharFormat tcf;
            tcf.setFontPointSize(13);
            tcf.setFontItalic(true);
            attributes.append(QInputMethodEvent::Attribute(QInputMethodEvent::TextFormat, 0, 1, tcf));
        }

        BOBUIest::newRow("Two formats, front, out of order")
            << attributes
            << (QStringList() << "P" << "reE" << "di" << "tText")
            << (QList<bool>() << true << true << false << true)
            << (QList<bool>() << true << false << false << false)
            << (QList<int>() << 13 << 20 << 8 << 20);
    }
}

void tst_BOBUIextEdit::preeditCharFormat()
{
    QFETCH(QList<QInputMethodEvent::Attribute>, imeAttributes);
    QFETCH(QStringList, substrings);
    QFETCH(QList<bool>, boldnessList);
    QFETCH(QList<bool>, italicnessList);
    QFETCH(QList<int>, pointSizeList);

    BOBUIextEdit *w = new BOBUIextEdit;
    w->show();
    QVERIFY(BOBUIest::qWaitForWindowExposed(w));

    // Set main char format
    {
        BOBUIextCharFormat tcf;
        tcf.setFontPointSize(20);
        tcf.setFontWeight(QFont::Bold);
        w->mergeCurrentCharFormat(tcf);
    }

    QList<QInputMethodEvent::Attribute> attributes;
    attributes.prepend(QInputMethodEvent::Attribute(QInputMethodEvent::Cursor,
                                                   w->textCursor().position(),
                                                   0,
                                                   QVariant()));

    attributes += imeAttributes;

    QInputMethodEvent event("PreEditText", attributes);
    QApplication::sendEvent(w, &event);

    MyPaintDevice device;
    {
        QPainter p(&device);
        w->document()->drawContents(&p);
    }

    QCOMPARE(device.m_paintEngine->itemFonts.size(), substrings.size());
    for (int i = 0; i < substrings.size(); ++i)
        QCOMPARE(device.m_paintEngine->itemFonts.at(i).first, substrings.at(i));

    for (int i = 0; i < substrings.size(); ++i)
        QCOMPARE(device.m_paintEngine->itemFonts.at(i).second.bold(), boldnessList.at(i));

    for (int i = 0; i < substrings.size(); ++i)
        QCOMPARE(device.m_paintEngine->itemFonts.at(i).second.italic(), italicnessList.at(i));

    for (int i = 0; i < substrings.size(); ++i)
        QCOMPARE(device.m_paintEngine->itemFonts.at(i).second.pointSize(), pointSizeList.at(i));

    delete w;
}

void tst_BOBUIextEdit::nextFormatAfterEnterPressed_data()
{
    typedef QMap<int, QVariant> pmap;
    BOBUIest::addColumn<QString>("html");
    BOBUIest::addColumn<int>("enterKeyCount");
    BOBUIest::addColumn<pmap>("expectedPrevBlockProps");
    BOBUIest::addColumn<pmap>("expectedPrevCharProps");
    BOBUIest::addColumn<pmap>("expectedNewBlockProps");
    BOBUIest::addColumn<pmap>("expectedNewCharProps");

    // the BlockBottomMargin on "two" will be removed: property() returns invalid QVariant
    BOBUIest::newRow("bottom margin after ordered list") << "<ol><li>one</li><li>two</li></ol>" << 1
        << pmap{{BOBUIextFormat::BlockBottomMargin, {}}} << pmap{}
        << pmap{{BOBUIextFormat::BlockBottomMargin, 12}} << pmap{};
    BOBUIest::newRow("double enter after list: default format") << "<ol><li>one</li><li>two</li></ol>" << 2
        << pmap{{BOBUIextFormat::BlockBottomMargin, {}}} << pmap{}
        << pmap{} << pmap{};
    BOBUIest::newRow("continue block quote") << "<blockquote>I'll be back</blockquote>" << 1
        << pmap{{BOBUIextFormat::BlockLeftMargin, 40}} << pmap{}
        << pmap{{BOBUIextFormat::BlockLeftMargin, 40}} << pmap{};
    BOBUIest::newRow("double enter after block quote") << "<blockquote>I'll be back</blockquote>" << 2
        << pmap{{BOBUIextFormat::BlockLeftMargin, 40}} << pmap{}
        << pmap{{BOBUIextFormat::BlockLeftMargin, {}}} << pmap{};
    BOBUIest::newRow("bottom margin after bullet list") << "<ul><li>one</li><li>two</li></ul>" << 1
        << pmap{{BOBUIextFormat::BlockBottomMargin, {}}} << pmap{}
        << pmap{{BOBUIextFormat::BlockBottomMargin, 12}} << pmap{};
    BOBUIest::newRow("paragraph after heading") << "<h1>so big!</h1>" << 1
        << pmap{{BOBUIextFormat::HeadingLevel, 1}} << pmap{}
        << pmap{{BOBUIextFormat::HeadingLevel, {}}} << pmap{};
    BOBUIest::newRow("paragraph after hrule") << "<p style='font-size:18px;'>blah blah<hr/></p>" << 1
        << pmap{} << pmap{}
        << pmap{{BOBUIextFormat::BlockTrailingHorizontalRulerWidth, {}}} << pmap{};
}

void tst_BOBUIextEdit::nextFormatAfterEnterPressed()
{
    typedef QMap<int, QVariant> pmap;
    QFETCH(QString, html);
    QFETCH(int, enterKeyCount);
    QFETCH(pmap, expectedPrevBlockProps);
    QFETCH(pmap, expectedPrevCharProps);
    QFETCH(pmap, expectedNewBlockProps);
    QFETCH(pmap, expectedNewCharProps);

    ed->setHtml(html);
    BOBUIextCursor cursor = ed->textCursor();
    cursor.movePosition(BOBUIextCursor::End);
    ed->setTextCursor(cursor);

    if (lcTests().isDebugEnabled()) {
        ed->show();
        BOBUIest::qWait(500);
    }

    for (int i = 0; i < enterKeyCount; ++i)
        BOBUIest::keyClick(ed, BobUI::Key_Enter);
    BOBUIest::keyClicks(ed, "foo");

    if (lcTests().isDebugEnabled()) {
        // visually see what happened when debug is enabled
        BOBUIest::qWait(500);
        qCDebug(lcTests) << "new block" << BobUI::hex << ed->textCursor().blockFormat().properties();
        qCDebug(lcTests) << "new char" << BobUI::hex << ed->textCursor().charFormat().properties();
    }

    // if expectedNewBlockProps is empty, we expect the current block format to be the default format
    if (expectedNewBlockProps.isEmpty())
        QCOMPARE(ed->textCursor().blockFormat(), BOBUIextBlockFormat());
    // otherwise we expect to find certain property values in the current block format
    else for (auto it = expectedNewBlockProps.constBegin(); it != expectedNewBlockProps.constEnd(); ++it)
        QCOMPARE(ed->textCursor().blockFormat().property(it.key()), it.value());

    // if expectedNewCharProps is empty, we expect the current char format to be the default format
    if (expectedNewCharProps.isEmpty())
        QCOMPARE(ed->textCursor().charFormat(), BOBUIextCharFormat());
    // otherwise we expect to find certain property values in the current char format
    else for (auto it = expectedNewCharProps.constBegin(); it != expectedNewCharProps.constEnd(); ++it)
        QCOMPARE(ed->textCursor().charFormat().property(it.key()), it.value());

    // check the cases where QWidgetTextControlPrivate::insertParagraphSeparator() should modify
    // the previous block's block format and/or char format
    auto prevBlockCursor = ed->textCursor();
    prevBlockCursor.movePosition(BOBUIextCursor::PreviousBlock);
    for (auto it = expectedPrevBlockProps.constBegin(); it != expectedPrevBlockProps.constEnd(); ++it)
        QCOMPARE(prevBlockCursor.blockFormat().property(it.key()), it.value());
    for (auto it = expectedPrevCharProps.constBegin(); it != expectedPrevCharProps.constEnd(); ++it)
        QCOMPARE(prevBlockCursor.charFormat().property(it.key()), it.value());
}

void tst_BOBUIextEdit::dontCrashWithCss()
{
    qApp->setStyleSheet("QWidget { font: 10pt; }");
    BOBUIextEdit edit;
    edit.show();
    qApp->setStyleSheet(QString());
}


BOBUIEST_MAIN(tst_BOBUIextEdit)
#include "tst_bobuiextedit.moc"
