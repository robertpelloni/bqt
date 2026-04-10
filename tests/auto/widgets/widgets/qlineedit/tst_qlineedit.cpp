// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only


#include <BOBUIest>
#include <BobUITest/private/bobuiesthelpers_p.h>
#include "qlineedit.h"
#include "qapplication.h"
#include "qstringlist.h"
#include "qstyle.h"
#include "qvalidator.h"
#include "qwidgetaction.h"
#include "qimage.h"
#include "qicon.h"
#include "qcompleter.h"
#include "qstandarditemmodel.h"
#include <qpa/qplatformtheme.h>
#include "qstylehints.h"
#include <private/qapplication_p.h>
#include "qclipboard.h"
#include <QSignalSpy>
#include <QRandomGenerator>
#include <BOBUIimer>

#include <qlineedit.h>
#include <private/qlineedit_p.h>
#include <private/qwidgetlinecontrol_p.h>
#include <qmenu.h>
#include <qlabel.h>
#include <qlayout.h>
#include <qspinbox.h>
#include <qlistview.h>
#include <qstringlistmodel.h>
#include <qsortfilterproxymodel.h>
#include <qdebug.h>
#include <qscreen.h>
#if BOBUI_CONFIG(shortcut)
#  include <qshortcut.h>
#endif

#include "qcommonstyle.h"
#include "qstyleoption.h"

#include "qplatformdefs.h"

#include "../../../shared/platformclipboard.h"
#include "../../../shared/platforminputcontext.h"
#include <private/qinputmethod_p.h>

Q_LOGGING_CATEGORY(lcTests, "bobui.widgets.tests")

BOBUI_BEGIN_NAMESPACE
class QPainter;
BOBUI_END_NAMESPACE

using namespace BOBUIestPrivate;

class StyleOptionTestStyle : public QCommonStyle
{
public:
    bool readOnly = false;
    mutable bool wasDrawn = false;

    using QCommonStyle::QCommonStyle;
    void setReadOnly(bool readOnly)
    {
        this->readOnly = readOnly;
    }

    void drawPrimitive(PrimitiveElement pe, const QStyleOption *opt, QPainter *,
                                 const QWidget *) const override
    {
        switch (pe) {
            case PE_PanelLineEdit:
            wasDrawn = true;
            if (readOnly)
                QVERIFY(opt->state & QStyle::State_ReadOnly);
            else
                QVERIFY(!(opt->state & QStyle::State_ReadOnly));
            break;

            default:
            break;
        }
    }
};

class tst_QLineEdit : public QObject
{
Q_OBJECT

public:
    enum EventStates { Press, Release, Click };

    tst_QLineEdit();

private slots:
    void initTestCase();
    void cleanupTestCase();
    void init();
    void cleanup();

    void getSetCheck();
    void experimental();

    void upperAndLowercase();

    void setInputMask_data();
    void setInputMask();

    void inputMask_data();
    void inputMask();

    void clearInputMask();

#if BOBUI_CONFIG(shortcut)
    void keypress_inputMask_data();
    void keypress_inputMask();
    void keypress_inputMethod_inputMask();
#endif

    void inputMaskAndValidator_data();
    void inputMaskAndValidator();

    void hasAcceptableInputMask_data();
    void hasAcceptableInputMask();

    void hasAcceptableInputValidator();


    void redo_data();
    void redo();

    void undo_data();
    void undo();

    void undo_keypressevents_data();
    void undo_keypressevents();

#ifndef BOBUI_NO_CLIPBOARD
    void BOBUIBUG5786_undoPaste();
#endif

    void clear();

    void text_data();
    void text();
    void textMask_data();
    void textMask();
    void maskCharacter();
    void maskCharacter_data();
    void setText();

    void displayText_data();
    void displayText();
    void passwordEchoOnEdit();
    void passwordEchoDelay();

    void maxLength_mask_data();
    void maxLength_mask();

    void maxLength_data();
    void maxLength();

    void isReadOnly();

    void noCursorBlinkWhenReadOnly();

    void cursorPosition();

    void cursorPositionChanged_data();
    void cursorPositionChanged();

    void selectedText();
    void deleteSelectedText();

    void textChangedAndTextEdited();
    void returnPressed();
    void returnPressed_maskvalidator_data();
    void returnPressed_maskvalidator();

    void setValidator();
    void setValidator_QIntValidator_data();
    void setValidator_QIntValidator();

    void frame_data();
    void frame();

    void leftKeyOnSelectedText();

    void setAlignment_data();
    void setAlignment();

    void isModified();
    void edited();
    void fixupDoesNotModify_BOBUIBUG_49295();

    void insert();
    void setSelection_data();
    void setSelection();

#if BOBUI_CONFIG(clipboard) && BOBUI_CONFIG(shortcut)
    void cut();
    void cutWithoutSelection();
#endif
    void maxLengthAndInputMask();
    void returnPressedKeyEvent();

    void keepSelectionOnTabFocusIn();

    void readOnlyStyleOption();

    void validateOnFocusOut();

    void editInvalidText();

    void charWithAltOrCtrlModifier_data();
    void charWithAltOrCtrlModifier();

    void inlineCompletion();

    void noTextEditedOnClear();

#ifndef BOBUI_NO_CURSOR
    void cursor();
#endif

    void textMargin_data();
    void textMargin();

    void returnKeyClearsEditedFlag();

    // task-specific tests:
    void task180999_focus();
    void task174640_editingFinished();
#if BOBUI_CONFIG(completer)
    void task198789_currentCompletion();
    void task210502_caseInsensitiveInlineCompletion();
#endif
    void task229938_dontEmitChangedWhenTextIsNotChanged();
    void task233101_cursorPosAfterInputMethod_data();
    void task233101_cursorPosAfterInputMethod();
    void task241436_passwordEchoOnEditRestoreEchoMode();
    void task248948_redoRemovedSelection();
    void taskBOBUIBUG_4401_enterKeyClearsPassword();
    void taskBOBUIBUG_4679_moveToStartEndOfBlock();
    void taskBOBUIBUG_4679_selectToStartEndOfBlock();
#ifndef BOBUI_NO_CONTEXTMENU
    void taskBOBUIBUG_7902_contextMenuCrash();
    void contextMenu();
#endif
    void taskBOBUIBUG_7395_readOnlyShortcut();
    void BOBUIBUG697_paletteCurrentColorGroup();
    void BOBUIBUG13520_textNotVisible();
    void BOBUIBUG7174_inputMaskCursorBlink();
    void BOBUIBUG16850_setSelection();

    void bidiVisualMovement_data();
    void bidiVisualMovement();

    void bidiLogicalMovement_data();
    void bidiLogicalMovement();

    void selectAndCursorPosition();
    void inputMethod();
    void inputMethodSelection();

    void inputMethodQueryImHints_data();
    void inputMethodQueryImHints();

    void inputMethodQueryEnterKeyType();

    void inputMethodUpdate();

    void undoRedoAndEchoModes_data();
    void undoRedoAndEchoModes();

    void clearButton();
    void clearButtonVisibleAfterSettingText_BOBUIBUG_45518();
    void sideWidgets();
    void sideWidgetsActionEvents();
    void sideWidgetsEffectiveMargins();

    void shouldShowPlaceholderText_data();
    void shouldShowPlaceholderText();
    void BOBUIBUG1266_setInputMaskEmittingTextEdited();

#if BOBUI_CONFIG(shortcut)
    void shortcutOverrideOnReadonlyLineEdit_data();
    void shortcutOverrideOnReadonlyLineEdit();
#endif
    void BOBUIBUG59957_clearButtonLeftmostAction();
    void BOBUIBUG_60319_setInputMaskCheckImSurroundingText();
    void testQuickSelectionWithMouse();
    void inputRejected();
    void keyReleasePropagates();

#if BOBUI_CONFIG(shortcut)
    void deleteWordByKeySequence_data();
    void deleteWordByKeySequence();
#endif

protected slots:
    void editingFinished();

    void onTextChanged( const QString &newString );
    void onTextEdited( const QString &newString );
    void onReturnPressed();
    void onSelectionChanged();
    void onCursorPositionChanged(int oldpos, int newpos);

private:
    // keyClicks(..) is moved to BobUITestCase
    void psKeyClick(QWidget *target, BobUI::Key key, BobUI::KeyboardModifiers pressState = {});
    void psKeyClick(BOBUIestEventList &keys, BobUI::Key key, BobUI::KeyboardModifiers pressState = {});
    bool unselectingWithLeftOrRightChangesCursorPosition();
#if BOBUI_CONFIG(shortcut)
    void addKeySequenceStandardKey(BOBUIestEventList &keys, QKeySequence::StandardKey);
#endif
    QLineEdit *ensureTestWidget();

    bool validInput;
    QString changed_string;
    int changed_count;
    int edited_count;
    int return_count;
    int selection_count;
    int lastCursorPos;
    int newCursorPos;
    QLineEdit *m_testWidget;
    int m_keyboardScheme;
    PlatformInputContext m_platformInputContext;
};

typedef QList<int> IntList;
Q_DECLARE_METATYPE(QLineEdit::EchoMode)

// Testing get/set functions
void tst_QLineEdit::getSetCheck()
{
    QLineEdit obj1;
    // const QValidator * QLineEdit::validator()
    // void QLineEdit::setValidator(const QValidator *)
    QIntValidator *var1 = new QIntValidator(0);
    obj1.setValidator(var1);
    QCOMPARE((const QValidator *)var1, obj1.validator());
    obj1.setValidator((QValidator *)0);
    QCOMPARE((const QValidator *)0, obj1.validator());
    delete var1;

    // bool QLineEdit::dragEnabled()
    // void QLineEdit::setDragEnabled(bool)
    obj1.setDragEnabled(false);
    QCOMPARE(false, obj1.dragEnabled());
    obj1.setDragEnabled(true);
    QCOMPARE(true, obj1.dragEnabled());
}

tst_QLineEdit::tst_QLineEdit() : validInput(false), m_testWidget(0), m_keyboardScheme(0)
{
    if (const QPlatformTheme *theme = QGuiApplicationPrivate::platformTheme())
        m_keyboardScheme = theme->themeHint(QPlatformTheme::KeyboardScheme).toInt();
    // Generalize for X11
    if (m_keyboardScheme == QPlatformTheme::KdeKeyboardScheme
            || m_keyboardScheme == QPlatformTheme::GnomeKeyboardScheme
            || m_keyboardScheme == QPlatformTheme::CdeKeyboardScheme) {
        m_keyboardScheme = QPlatformTheme::X11KeyboardScheme;
    }
}

QLineEdit *tst_QLineEdit::ensureTestWidget()
{
    if (!m_testWidget) {
        m_testWidget = new QLineEdit;
        m_testWidget->setObjectName("testWidget");
        connect(m_testWidget, SIGNAL(cursorPositionChanged(int,int)), this, SLOT(onCursorPositionChanged(int,int)));
        connect(m_testWidget, SIGNAL(textChanged(QString)), this, SLOT(onTextChanged(QString)));
        connect(m_testWidget, SIGNAL(textEdited(QString)), this, SLOT(onTextEdited(QString)));
        connect(m_testWidget, SIGNAL(returnPressed()), this, SLOT(onReturnPressed()));
        connect(m_testWidget, SIGNAL(selectionChanged()), this, SLOT(onSelectionChanged()));
        connect(m_testWidget, SIGNAL(editingFinished()), this, SLOT(editingFinished()));
        m_testWidget->resize(200,50);
    }
    return m_testWidget;
}

void tst_QLineEdit::initTestCase()
{
    changed_count = 0;
    edited_count = 0;
    selection_count = 0;

    QInputMethodPrivate *inputMethodPrivate = QInputMethodPrivate::get(qApp->inputMethod());
    inputMethodPrivate->testContext = &m_platformInputContext;
}

void tst_QLineEdit::cleanupTestCase()
{
    QInputMethodPrivate *inputMethodPrivate = QInputMethodPrivate::get(qApp->inputMethod());
    inputMethodPrivate->testContext = 0;
}

void tst_QLineEdit::init()
{
    return_count = 0;
}

void tst_QLineEdit::cleanup()
{
    delete m_testWidget;
    m_testWidget = 0;
    m_platformInputContext.m_commitString.clear();
}

void tst_QLineEdit::experimental()
{
    QLineEdit *testWidget = ensureTestWidget();
    QIntValidator intValidator(3, 7, 0);
    testWidget->setValidator(&intValidator);
    testWidget->setText("");


    // test the order of setting these
    testWidget->setInputMask("");
    testWidget->setText("abc123");
    testWidget->setInputMask("000.000.000.000");
    QCOMPARE(testWidget->text(), QString("123..."));
    testWidget->setText("");


}

void tst_QLineEdit::upperAndLowercase()
{
    QLineEdit *testWidget = ensureTestWidget();

    BOBUIest::keyClicks(testWidget, "aAzZ`1234567890-=~!@#$%^&*()_+[]{}\\|;:'\",.<>/?");
    qApp->processEvents();
    QCOMPARE(testWidget->text(), QString("aAzZ`1234567890-=~!@#$%^&*()_+[]{}\\|;:'\",.<>/?"));
}

void tst_QLineEdit::setInputMask_data()
{
    BOBUIest::addColumn<QString>("mask");
    BOBUIest::addColumn<QString>("input");
    BOBUIest::addColumn<QString>("expectedText");
    BOBUIest::addColumn<QString>("expectedDisplay");
    BOBUIest::addColumn<bool>("insert_text");

    // both keyboard and insert()
    for (int i=0; i<2; i++) {
        bool insert_text = i==0 ? false : true;
        QString insert_mode = "keys ";
        if (insert_text)
            insert_mode = "insert ";

        BOBUIest::newRow(QString(insert_mode + "ip_localhost").toLatin1())
            << QString("000.000.000.000")
            << QString("127.0.0.1")
            << QString("127.0.0.1")
            << QString("127.0  .0  .1  ")
            << bool(insert_text);
        BOBUIest::newRow(QString(insert_mode + "mac").toLatin1())
            << QString("HH:HH:HH:HH:HH:HH;#")
            << QString("00:E0:81:21:9E:8E")
            << QString("00:E0:81:21:9E:8E")
            << QString("00:E0:81:21:9E:8E")
            << bool(insert_text);
        BOBUIest::newRow(QString(insert_mode + "mac2").toLatin1())
            << QString("<HH:>HH:!HH:HH:HH:HH;#")
            << QString("AAe081219E8E")
            << QString("aa:E0:81:21:9E:8E")
            << QString("aa:E0:81:21:9E:8E")
            << bool(insert_text);
        BOBUIest::newRow(QString(insert_mode + "byte").toLatin1())
            << QString("BBBBBBBB;0")
            << QString("11011001")
            << QString("11111")
            << QString("11011001")
            << bool(insert_text);
        BOBUIest::newRow(QString(insert_mode + "halfbytes").toLatin1())
            << QString("bbbb.bbbb;-")
            << QString("110. 0001")
            << QString("110.0001")
            << QString("110-.0001")
            << bool(insert_text);
        BOBUIest::newRow(QString(insert_mode + "blank char same type as content").toLatin1())
            << QString("000.000.000.000;0")
            << QString("127.0.0.1")
            << QString("127...1")
            << QString("127.000.000.100")
            << bool(insert_text);
        BOBUIest::newRow(QString(insert_mode + "parts of ip_localhost").toLatin1())
            << QString("000.000.000.000")
            << QString(".0.0.1")
            << QString(".0.0.1")
            << QString("   .0  .0  .1  ")
            << bool(insert_text);
        BOBUIest::newRow(QString(insert_mode + "ip_null").toLatin1())
            << QString("000.000.000.000")
            << QString()
            << QString("...")
            << QString("   .   .   .   ")
            << bool(insert_text);
        BOBUIest::newRow(QString(insert_mode + "ip_null_hash").toLatin1())
            << QString("000.000.000.000;#")
            << QString()
            << QString("...")
            << QString("###.###.###.###")
            << bool(insert_text);
        BOBUIest::newRow(QString(insert_mode + "ip_overflow").toLatin1())
            << QString("000.000.000.000")
            << QString("1234123412341234")
            << QString("123.412.341.234")
            << QString("123.412.341.234")
            << bool(insert_text);
        BOBUIest::newRow(QString(insert_mode + "uppercase").toLatin1())
            << QString(">AAAA")
            << QString("AbCd")
            << QString("ABCD")
            << QString("ABCD")
            << bool(insert_text);
        BOBUIest::newRow(QString(insert_mode + "lowercase").toLatin1())
            << QString("<AAAA")
            << QString("AbCd")
            << QString("abcd")
            << QString("abcd")
            << bool(insert_text);

        BOBUIest::newRow(QString(insert_mode + "nocase").toLatin1())
            << QString("!AAAA")
            << QString("AbCd")
            << QString("AbCd")
            << QString("AbCd")
            << bool(insert_text);
        BOBUIest::newRow(QString(insert_mode + "nocase1").toLatin1())
            << QString("!A!A!A!A")
            << QString("AbCd")
            << QString("AbCd")
            << QString("AbCd")
            << bool(insert_text);
        BOBUIest::newRow(QString(insert_mode + "nocase2").toLatin1())
            << QString("AAAA")
            << QString("AbCd")
            << QString("AbCd")
            << QString("AbCd")
            << bool(insert_text);

        BOBUIest::newRow(QString(insert_mode + "reserved").toLatin1())
            << QString("{n}[0]")
            << QString("A9")
            << QString("A9")
            << QString("A9")
            << bool(insert_text);
        BOBUIest::newRow(QString(insert_mode + "escape01").toLatin1())
            << QString("\\N\\n00")
            << QString("9")
            << QString("Nn9")
            << QString("Nn9 ")
            << bool(insert_text);
        BOBUIest::newRow(QString(insert_mode + "escape02").toLatin1())
            << QString("\\\\00")
            << QString("0")
            << QString("\\0")
            << QString("\\0 ")
            << bool(insert_text);
        BOBUIest::newRow(QString(insert_mode + "escape03").toLatin1())
            << QString("\\(00\\)")
            << QString("0")
            << QString("(0)")
            << QString("(0 )")
            << bool(insert_text);

        BOBUIest::newRow(QString(insert_mode + "upper_lower_nocase1").toLatin1())
            << QString(">AAAA<AAAA!AAAA")
            << QString("AbCdEfGhIjKl")
            << QString("ABCDefghIjKl")
            << QString("ABCDefghIjKl")
            << bool(insert_text);
        BOBUIest::newRow(QString(insert_mode + "upper_lower_nocase2").toLatin1())
            << QString(">aaaa<aaaa!aaaa")
            << QString("AbCdEfGhIjKl")
            << QString("ABCDefghIjKl")
            << QString("ABCDefghIjKl")
            << bool(insert_text);

        BOBUIest::newRow(QString(insert_mode + "exact_case1").toLatin1())
            << QString(">A<A<A>A>A<A!A!A")
            << QString("AbCdEFGH")
            << QString("AbcDEfGH")
            << QString("AbcDEfGH")
            << bool(insert_text);
        BOBUIest::newRow(QString(insert_mode + "exact_case2").toLatin1())
            << QString(">A<A<A>A>A<A!A!A")
            << QString("aBcDefgh")
            << QString("AbcDEfgh")
            << QString("AbcDEfgh")
            << bool(insert_text);
        BOBUIest::newRow(QString(insert_mode + "exact_case3").toLatin1())
            << QString(">a<a<a>a>a<a!a!a")
            << QString("AbCdEFGH")
            << QString("AbcDEfGH")
            << QString("AbcDEfGH")
            << bool(insert_text);
        BOBUIest::newRow(QString(insert_mode + "exact_case4").toLatin1())
            << QString(">a<a<a>a>a<a!a!a")
            << QString("aBcDefgh")
            << QString("AbcDEfgh")
            << QString("AbcDEfgh")
            << bool(insert_text);
        BOBUIest::newRow(QString(insert_mode + "exact_case5").toLatin1())
            << QString(">H<H<H>H>H<H!H!H")
            << QString("aBcDef01")
            << QString("AbcDEf01")
            << QString("AbcDEf01")
            << bool(insert_text);
        BOBUIest::newRow(QString(insert_mode + "exact_case6").toLatin1())
            << QString(">h<h<h>h>h<h!h!h")
            << QString("aBcDef92")
            << QString("AbcDEf92")
            << QString("AbcDEf92")
            << bool(insert_text);

        BOBUIest::newRow(QString(insert_mode + "illegal_keys1").toLatin1())
            << QString("AAAAAAAA")
            << QString("A2#a;.0!")
            << QString("Aa")
            << QString("Aa      ")
            << bool(insert_text);
        BOBUIest::newRow(QString(insert_mode + "illegal_keys2").toLatin1())
            << QString("AAAA")
            << QString("f4f4f4f4")
            << QString("ffff")
            << QString("ffff")
            << bool(insert_text);
        BOBUIest::newRow(QString(insert_mode + "blank=input").toLatin1())
            << QString("9999;0")
            << QString("2004")
            << QString("2004")
            << QString("2004")
            << bool(insert_text);
    }
}

void tst_QLineEdit::setInputMask()
{
    QFETCH(QString, mask);
    QFETCH(QString, input);
    QFETCH(QString, expectedText);
    QFETCH(QString, expectedDisplay);
    QFETCH(bool, insert_text);

    QEXPECT_FAIL( "keys blank=input", "To eat blanks or not? Known issue. Task 43172", Abort);
    QEXPECT_FAIL( "insert blank=input", "To eat blanks or not? Known issue. Task 43172", Abort);

    // First set the input mask
    QLineEdit *testWidget = ensureTestWidget();
    testWidget->setInputMask(mask);

    // then either insert using insert() or keyboard
    if (insert_text) {
        testWidget->insert(input);
    } else {
        psKeyClick(testWidget, BobUI::Key_Home);
        for (int i=0; i<input.size(); i++)
            BOBUIest::keyClick(testWidget, input.at(i).toLatin1());
    }

    QCOMPARE(testWidget->text(), expectedText);
    QCOMPARE(testWidget->displayText(), expectedDisplay);
}

void tst_QLineEdit::inputMask_data()
{
    BOBUIest::addColumn<QString>("mask");
    BOBUIest::addColumn<QString>("expectedMask");

    // if no mask is set a nul string should be returned
    BOBUIest::newRow("nul 1") << QString("") << QString();
    BOBUIest::newRow("nul 2") << QString() << QString();

    // try different masks
    BOBUIest::newRow("mask 1") << QString("000.000.000.000") << QString("000.000.000.000");
    BOBUIest::newRow("mask 2") << QString("000.000.000.000;#") << QString("000.000.000.000;#");
    BOBUIest::newRow("mask 3") << QString("AAA.aa.999.###;") << QString("AAA.aa.999.###");
    BOBUIest::newRow("mask 4") << QString(">abcdef<GHIJK") << QString(">abcdef<GHIJK");

    // set an invalid input mask...
    // the current behaviour is that this exact (faulty) string is returned.
    BOBUIest::newRow("invalid") << QString("ABCDEFGHIKLMNOP;") << QString("ABCDEFGHIKLMNOP");

    // verify that we can unset the mask again
    BOBUIest::newRow("unset") << QString("") << QString();
}

void tst_QLineEdit::inputMask()
{
    QFETCH(QString, mask);
    QFETCH(QString, expectedMask);

    QLineEdit *testWidget = ensureTestWidget();
    testWidget->setInputMask(mask);
    QCOMPARE(testWidget->inputMask(), expectedMask);
}

void tst_QLineEdit::clearInputMask()
{
    QLineEdit *testWidget = ensureTestWidget();
    testWidget->setInputMask("000.000.000.000");
    QVERIFY(!testWidget->inputMask().isNull());
    testWidget->setInputMask(QString());
    QCOMPARE(testWidget->inputMask(), QString());
}

#if BOBUI_CONFIG(shortcut)

void tst_QLineEdit::keypress_inputMask_data()
{
    BOBUIest::addColumn<QString>("mask");
    BOBUIest::addColumn<BOBUIestEventList>("keys");
    BOBUIest::addColumn<QString>("expectedText");
    BOBUIest::addColumn<QString>("expectedDisplayText");

    {
        BOBUIestEventList keys;
        // inserting 'A1.2B'
        addKeySequenceStandardKey(keys, QKeySequence::MoveToStartOfLine);
        keys.addKeyClick(BobUI::Key_A);
        keys.addKeyClick(BobUI::Key_1);
        keys.addKeyClick(BobUI::Key_Period);
        keys.addKeyClick(BobUI::Key_2);
        keys.addKeyClick(BobUI::Key_B);
        BOBUIest::newRow("jumping on period(separator)") << QString("000.000;_") << keys << QString("1.2") << QString("1__.2__");
    }
    {
        BOBUIestEventList keys;
        // inserting 'A1.2B'
        addKeySequenceStandardKey(keys, QKeySequence::MoveToStartOfLine);
        keys.addKeyClick(BobUI::Key_0);
        keys.addKeyClick(BobUI::Key_Exclam);
        keys.addKeyClick('P');
        keys.addKeyClick(BobUI::Key_3);
        BOBUIest::newRow("jumping on input") << QString("D0.AA.XX.AA.00;_") << keys << QString("0..!P..3") << QString("_0.__.!P.__.3_");
    }
    {
        BOBUIestEventList keys;
        // pressing delete
        addKeySequenceStandardKey(keys, QKeySequence::MoveToStartOfLine);
        keys.addKeyClick(BobUI::Key_Delete);
        BOBUIest::newRow("delete") << QString("000.000;_") << keys << QString(".") << QString("___.___");
    }
    {
        BOBUIestEventList keys;
        // selecting all and delete
        keys.addKeyClick(BobUI::Key_1);
        keys.addKeyClick(BobUI::Key_2);
        addKeySequenceStandardKey(keys, QKeySequence::MoveToStartOfLine);
        addKeySequenceStandardKey(keys, QKeySequence::SelectEndOfLine);
        keys.addKeyClick(BobUI::Key_Delete);
        BOBUIest::newRow("deleting all") << QString("000.000;_") << keys << QString(".") << QString("___.___");
    }
    {
        BOBUIestEventList keys;
        // inserting at end
        addKeySequenceStandardKey(keys, QKeySequence::MoveToEndOfLine);
        keys.addKeyClick(BobUI::Key_Left);
        keys.addKeyClick(BobUI::Key_0);
        BOBUIest::newRow("insert at end") << QString("9-9-9") << keys << QString("--0") << QString(" - -0");
    }
    {
        BOBUIestEventList keys;
        // inserting '12.12' then two backspaces
        addKeySequenceStandardKey(keys, QKeySequence::MoveToStartOfLine);
        keys.addKeyClick(BobUI::Key_1);
        keys.addKeyClick(BobUI::Key_2);
        keys.addKeyClick(BobUI::Key_Period);
        keys.addKeyClick(BobUI::Key_1);
        keys.addKeyClick(BobUI::Key_2);
        keys.addKeyClick(BobUI::Key_Backspace);
        keys.addKeyClick(BobUI::Key_Backspace);
        BOBUIest::newRow("backspace") << QString("000.000;_") << keys << QString("12.") << QString("12_.___");
    }
    {
        BOBUIestEventList keys;
        // inserting '12ab'
        addKeySequenceStandardKey(keys, QKeySequence::MoveToStartOfLine);
        keys.addKeyClick(BobUI::Key_1);
        keys.addKeyClick(BobUI::Key_2);
        keys.addKeyClick(BobUI::Key_A);
        keys.addKeyClick(BobUI::Key_B);
        BOBUIest::newRow("uppercase") << QString("9999 >AA;_") << keys << QString("12 AB") << QString("12__ AB");
    }
}

void tst_QLineEdit::keypress_inputMask()
{
    QFETCH(QString, mask);
    QFETCH(BOBUIestEventList, keys);
    QFETCH(QString, expectedText);
    QFETCH(QString, expectedDisplayText);

    QLineEdit *testWidget = ensureTestWidget();
    testWidget->setInputMask(mask);
    keys.simulate(testWidget);

    QCOMPARE(testWidget->text(), expectedText);
    QCOMPARE(testWidget->displayText(), expectedDisplayText);
}

void tst_QLineEdit::keypress_inputMethod_inputMask()
{
    // Similar to the keypress_inputMask test, but this is done solely via
    // input methods
    QLineEdit *testWidget = ensureTestWidget();
    testWidget->setInputMask("AA.AA.AA");
    {
        QList<QInputMethodEvent::Attribute> attributes;
        QInputMethodEvent event("", attributes);
        event.setCommitString("EE");
        QApplication::sendEvent(testWidget, &event);
    }
    QCOMPARE(testWidget->cursorPosition(), 3);
    QCOMPARE(testWidget->text(), QStringLiteral("EE.."));
    {
        QList<QInputMethodEvent::Attribute> attributes;
        QInputMethodEvent event("", attributes);
        event.setCommitString("EE");
        QApplication::sendEvent(testWidget, &event);
    }
    QCOMPARE(testWidget->cursorPosition(), 6);
    QCOMPARE(testWidget->text(), QStringLiteral("EE.EE."));
    {
        QList<QInputMethodEvent::Attribute> attributes;
        QInputMethodEvent event("", attributes);
        event.setCommitString("EE");
        QApplication::sendEvent(testWidget, &event);
    }
    QCOMPARE(testWidget->cursorPosition(), 8);
    QCOMPARE(testWidget->text(), QStringLiteral("EE.EE.EE"));
}

#endif // BOBUI_CONFIG(shortcut)

void tst_QLineEdit::hasAcceptableInputMask_data()
{
    BOBUIest::addColumn<QString>("optionalMask");
    BOBUIest::addColumn<QString>("requiredMask");
    BOBUIest::addColumn<QString>("invalid");
    BOBUIest::addColumn<QString>("valid");

    BOBUIest::newRow("Alphabetic optional and required")
        << QString("aaaa") << QString("AAAA") << QString("ab") << QString("abcd");
    BOBUIest::newRow("Alphanumeric optional and require")
        << QString("nnnn") << QString("NNNN") << QString("R2") << QString("R2D2");
    BOBUIest::newRow("Any optional and required")
        << QString("xxxx") << QString("XXXX") << QString("+-") << QString("+-*/");
    BOBUIest::newRow("Numeric (0-9) required")
        << QString("0000") << QString("9999") << QString("11") << QString("1138");
    BOBUIest::newRow("Numeric (1-9) optional and required")
        << QString("dddd") << QString("DDDD") << QString("12") << QString("1234");
}

void tst_QLineEdit::hasAcceptableInputMask()
{
    QFocusEvent lostFocus(QEvent::FocusOut);
    QFETCH(QString, optionalMask);
    QFETCH(QString, requiredMask);
    QFETCH(QString, invalid);
    QFETCH(QString, valid);

    // test that invalid input (for required) work for optionalMask
    QLineEdit *testWidget = ensureTestWidget();
    testWidget->setInputMask(optionalMask);
    validInput = false;
    testWidget->setText(invalid);
    qApp->sendEvent(testWidget, &lostFocus);
    QVERIFY(validInput);

    // test requiredMask
    testWidget->setInputMask(requiredMask);
    validInput = true;
    testWidget->setText(invalid);
    validInput = testWidget->hasAcceptableInput();
    QVERIFY(!validInput);

    validInput = false;
    testWidget->setText(valid);
    qApp->sendEvent(testWidget, &lostFocus);
    QVERIFY(validInput);
}

static const int chars = 8;
class ValidatorWithFixup : public QValidator
{
public:
    ValidatorWithFixup(QWidget *parent = nullptr)
        : QValidator(parent)
    {}

    QValidator::State validate(QString &str, int &) const override
    {
        const int s = str.size();
        if (s < chars) {
            return Intermediate;
        } else if (s > chars) {
            return Invalid;
        }
        return Acceptable;
    }

    void fixup(QString &str) const override
    {
        str = str.leftJustified(chars, 'X', true);
    }
};



void tst_QLineEdit::hasAcceptableInputValidator()
{
    QLineEdit *testWidget = ensureTestWidget();
    QSignalSpy spyChanged(testWidget, SIGNAL(textChanged(QString)));
    QSignalSpy spyEdited(testWidget, SIGNAL(textEdited(QString)));

    QFocusEvent lostFocus(QEvent::FocusOut);
    ValidatorWithFixup val;
    testWidget->setValidator(&val);
    testWidget->setText("foobar");
    qApp->sendEvent(testWidget, &lostFocus);
    QVERIFY(testWidget->hasAcceptableInput());

    QCOMPARE(spyChanged.size(), 2);
    QCOMPARE(spyEdited.size(), 0);
}



void tst_QLineEdit::maskCharacter_data()
{
    BOBUIest::addColumn<QString>("mask");
    BOBUIest::addColumn<QString>("input");
    BOBUIest::addColumn<bool>("expectedValid");

    BOBUIest::newRow("Hex") << QString("H")
                         << QString("0123456789abcdefABCDEF") << true;
    BOBUIest::newRow("hex") << QString("h")
                         << QString("0123456789abcdefABCDEF") << true;
    BOBUIest::newRow("HexInvalid") << QString("H")
                                << QString("ghijklmnopqrstuvwxyzGHIJKLMNOPQRSTUVWXYZ")
                                << false;
    BOBUIest::newRow("hexInvalid") << QString("h")
                                << QString("ghijklmnopqrstuvwxyzGHIJKLMNOPQRSTUVWXYZ")
                                << false;
    BOBUIest::newRow("Bin") << QString("B")
                         << QString("01") << true;
    BOBUIest::newRow("bin") << QString("b")
                         << QString("01") << true;
    BOBUIest::newRow("BinInvalid") << QString("B")
                                << QString("23456789qwertyuiopasdfghjklzxcvbnm")
                                << false;
    BOBUIest::newRow("binInvalid") << QString("b")
                                << QString("23456789qwertyuiopasdfghjklzxcvbnm")
                                << false;
}

void tst_QLineEdit::maskCharacter()
{
    QFETCH(QString, mask);
    QFETCH(QString, input);
    QFETCH(bool, expectedValid);

    QLineEdit *testWidget = ensureTestWidget();
    QFocusEvent lostFocus(QEvent::FocusOut);

    testWidget->setInputMask(mask);
    for (int i = 0; i < input.size(); ++i) {
        QString in = QString(input.at(i));
        QString expected = expectedValid ? in : QString();
        testWidget->setText(QString(input.at(i)));
        qApp->sendEvent(testWidget, &lostFocus);
        QCOMPARE(testWidget->text(), expected);
    }
}

#define NORMAL 0
#define REPLACE_UNTIL_END 1

void tst_QLineEdit::undo_data()
{
    BOBUIest::addColumn<QStringList>("insertString");
    BOBUIest::addColumn<IntList>("insertIndex");
    BOBUIest::addColumn<IntList>("insertMode");
    BOBUIest::addColumn<QStringList>("expectedString");
    BOBUIest::addColumn<bool>("use_keys");

    for (int i=0; i<2; i++) {
        QString keys_str = "keyboard";
        bool use_keys = true;
        if (i==0) {
            keys_str = "insert";
            use_keys = false;
        }

        {
            IntList insertIndex;
            IntList insertMode;
            QStringList insertString;
            QStringList expectedString;

            insertIndex << -1;
            insertMode << NORMAL;
            insertString << "1";

            insertIndex << -1;
            insertMode << NORMAL;
            insertString << "5";

            insertIndex << 1;
            insertMode << NORMAL;
            insertString << "3";

            insertIndex << 1;
            insertMode << NORMAL;
            insertString << "2";

            insertIndex << 3;
            insertMode << NORMAL;
            insertString << "4";

            expectedString << "12345";
            expectedString << "1235";
            expectedString << "135";
            expectedString << "15";
            expectedString << "";

            BOBUIest::newRow(QString(keys_str + "_numbers").toLatin1()) <<
                insertString <<
                insertIndex <<
                insertMode <<
                expectedString <<
                bool(use_keys);
        }
        {
            IntList insertIndex;
            IntList insertMode;
            QStringList insertString;
            QStringList expectedString;

            insertIndex << -1;
            insertMode << NORMAL;
            insertString << "World"; // World

            insertIndex << 0;
            insertMode << NORMAL;
            insertString << "Hello"; // HelloWorld

            insertIndex << 0;
            insertMode << NORMAL;
            insertString << "Well"; // WellHelloWorld

            insertIndex << 9;
            insertMode << NORMAL;
            insertString << "There"; // WellHelloThereWorld;

            expectedString << "WellHelloThereWorld";
            expectedString << "WellHelloWorld";
            expectedString << "HelloWorld";
            expectedString << "World";
            expectedString << "";

            BOBUIest::newRow(QString(keys_str + "_helloworld").toLatin1()) <<
                insertString <<
                insertIndex <<
                insertMode <<
                expectedString <<
                bool(use_keys);
        }
        {
            IntList insertIndex;
            IntList insertMode;
            QStringList insertString;
            QStringList expectedString;

            insertIndex << -1;
            insertMode << NORMAL;
            insertString << "Ensuring";

            insertIndex << -1;
            insertMode << NORMAL;
            insertString << " instan";

            insertIndex << 9;
            insertMode << NORMAL;
            insertString << "an ";

            insertIndex << 10;
            insertMode << REPLACE_UNTIL_END;
            insertString << " unique instance.";

            expectedString << "Ensuring a unique instance.";
            expectedString << "Ensuring an instan";
            expectedString << "Ensuring instan";
            expectedString << "";

            BOBUIest::newRow(QString(keys_str + "_patterns").toLatin1()) <<
                insertString <<
                insertIndex <<
                insertMode <<
                expectedString <<
                bool(use_keys);
        }
    }
}

void tst_QLineEdit::undo()
{
    QFETCH(QStringList, insertString);
    QFETCH(IntList, insertIndex);
    QFETCH(IntList, insertMode);
    QFETCH(QStringList, expectedString);
    QFETCH(bool, use_keys);

    QLineEdit *testWidget = ensureTestWidget();
    QVERIFY(!testWidget->isUndoAvailable());

    int i;

// STEP 1: First build up an undo history by inserting or typing some strings...
    for (i=0; i<insertString.size(); ++i) {
        if (insertIndex[i] > -1)
            testWidget->setCursorPosition(insertIndex[i]);

 // experimental stuff
        if (insertMode[i] == REPLACE_UNTIL_END) {
            testWidget->setSelection(insertIndex[i], 8);

            // This is what I actually want...
            // BOBUIest::keyClick(testWidget, BobUI::Key_End, BobUI::ShiftModifier);
        }

        if (use_keys)
            BOBUIest::keyClicks(testWidget, insertString[i]);
        else
            testWidget->insert(insertString[i]);
    }

// STEP 2: Next call undo several times and see if we can restore to the previous state
    for (i=0; i<expectedString.size()-1; ++i) {
        QCOMPARE(testWidget->text(), expectedString[i]);
        QVERIFY(testWidget->isUndoAvailable());
        testWidget->undo();
    }

// STEP 3: Verify that we have undone everything
    QVERIFY(!testWidget->isUndoAvailable());
    QVERIFY(testWidget->text().isEmpty());


    if (m_keyboardScheme == QPlatformTheme::WindowsKeyboardScheme) {
        // Repeat the test using shortcut instead of undo()
        for (i=0; i<insertString.size(); ++i) {
            if (insertIndex[i] > -1)
                testWidget->setCursorPosition(insertIndex[i]);
            if (insertMode[i] == REPLACE_UNTIL_END)
                testWidget->setSelection(insertIndex[i], 8);
            if (use_keys)
                BOBUIest::keyClicks(testWidget, insertString[i]);
            else
                testWidget->insert(insertString[i]);
        }
        for (i=0; i<expectedString.size()-1; ++i) {
            QCOMPARE(testWidget->text(), expectedString[i]);
            QVERIFY(testWidget->isUndoAvailable());
            BOBUIest::keyClick(testWidget, BobUI::Key_Backspace, BobUI::AltModifier);
        }
    }

}

void tst_QLineEdit::redo_data()
{
    BOBUIest::addColumn<QStringList>("insertString");
    BOBUIest::addColumn<IntList>("insertIndex");
    BOBUIest::addColumn<QStringList>("expectedString");

    {
        IntList insertIndex;
        QStringList insertString;
        QStringList expectedString;

        insertIndex << -1;
        insertString << "World"; // World
        insertIndex << 0;
        insertString << "Hello"; // HelloWorld
        insertIndex << 0;
        insertString << "Well"; // WellHelloWorld
        insertIndex << 9;
        insertString << "There"; // WellHelloThereWorld;

        expectedString << "World";
        expectedString << "HelloWorld";
        expectedString << "WellHelloWorld";
        expectedString << "WellHelloThereWorld";

        BOBUIest::newRow("Inserts and setting cursor") << insertString << insertIndex << expectedString;
    }
}

void tst_QLineEdit::redo()
{
    QFETCH(QStringList, insertString);
    QFETCH(IntList, insertIndex);
    QFETCH(QStringList, expectedString);

    QLineEdit *testWidget = ensureTestWidget();
    QVERIFY(!testWidget->isUndoAvailable());
    QVERIFY(!testWidget->isRedoAvailable());

    int i;
    // inserts the diff strings at diff positions
    for (i=0; i<insertString.size(); ++i) {
        if (insertIndex[i] > -1)
            testWidget->setCursorPosition(insertIndex[i]);
        testWidget->insert(insertString[i]);
    }

    QVERIFY(!testWidget->isRedoAvailable());

    // undo everything
    while (!testWidget->text().isEmpty())
        testWidget->undo();

    for (i=0; i<expectedString.size(); ++i) {
        QVERIFY(testWidget->isRedoAvailable());
        testWidget->redo();
        QCOMPARE(testWidget->text() , expectedString[i]);
    }

    QVERIFY(!testWidget->isRedoAvailable());


    if (m_keyboardScheme == QPlatformTheme::WindowsKeyboardScheme) {
        // repeat test, this time using shortcuts instead of undo()/redo()

        while (!testWidget->text().isEmpty())
            BOBUIest::keyClick(testWidget, BobUI::Key_Backspace, BobUI::AltModifier);

        for (i = 0; i < expectedString.size(); ++i) {
            QVERIFY(testWidget->isRedoAvailable());
            BOBUIest::keyClick(testWidget, BobUI::Key_Backspace,
                            BobUI::ShiftModifier | BobUI::AltModifier);
            QCOMPARE(testWidget->text() , expectedString[i]);
        }

        QVERIFY(!testWidget->isRedoAvailable());
    }
}

void tst_QLineEdit::undo_keypressevents_data()
{
    BOBUIest::addColumn<BOBUIestEventList>("keys");
    BOBUIest::addColumn<QStringList>("expectedString");

    {
        BOBUIestEventList keys;
        QStringList expectedString;

        keys.addKeyClick('A');
        keys.addKeyClick('F');
        keys.addKeyClick('R');
        keys.addKeyClick('A');
        keys.addKeyClick('I');
        keys.addKeyClick('D');
        psKeyClick(keys, BobUI::Key_Home);

        keys.addKeyClick('V');
        keys.addKeyClick('E');
        keys.addKeyClick('R');
        keys.addKeyClick('Y');

        keys.addKeyClick(BobUI::Key_Left);
        keys.addKeyClick(BobUI::Key_Left);
        keys.addKeyClick(BobUI::Key_Left);
        keys.addKeyClick(BobUI::Key_Left);

        keys.addKeyClick('B');
        keys.addKeyClick('E');
        psKeyClick(keys, BobUI::Key_End);

        keys.addKeyClick(BobUI::Key_Exclam);

        expectedString << "BEVERYAFRAID!";
        expectedString << "BEVERYAFRAID";
        expectedString << "VERYAFRAID";
        expectedString << "AFRAID";

        BOBUIest::newRow("Inserts and moving cursor") << keys << expectedString;
    }

    {
        BOBUIestEventList keys;
        QStringList expectedString;

        // inserting '1234'
        keys.addKeyClick(BobUI::Key_1);
        keys.addKeyClick(BobUI::Key_2);
        keys.addKeyClick(BobUI::Key_3);
        keys.addKeyClick(BobUI::Key_4);
        psKeyClick(keys, BobUI::Key_Home);

        // skipping '12'
        keys.addKeyClick(BobUI::Key_Right);
        keys.addKeyClick(BobUI::Key_Right);

        // selecting '34'
        keys.addKeyClick(BobUI::Key_Right, BobUI::ShiftModifier);
        keys.addKeyClick(BobUI::Key_Right, BobUI::ShiftModifier);

        // deleting '34'
        keys.addKeyClick(BobUI::Key_Delete);

        expectedString << "12";
        expectedString << "1234";

        BOBUIest::newRow("Inserts,moving,selection and delete") << keys << expectedString;
    }

    {
        BOBUIestEventList keys;
        QStringList expectedString;

        // inserting 'AB12'
        keys.addKeyClick('A');
        keys.addKeyClick('B');

        keys.addKeyClick(BobUI::Key_1);
        keys.addKeyClick(BobUI::Key_2);

        psKeyClick(keys, BobUI::Key_Home);

        // selecting 'AB'
        keys.addKeyClick(BobUI::Key_Right, BobUI::ShiftModifier);
        keys.addKeyClick(BobUI::Key_Right, BobUI::ShiftModifier);

        // deleting 'AB'
        keys.addKeyClick(BobUI::Key_Delete);

        // undoing deletion of 'AB'
        keys.addKeyClick(BobUI::Key_Z, BobUI::ControlModifier);

        // unselect any current selection
        keys.addKeyClick(BobUI::Key_Right);

        // If previous right changed cursor position, go back left
        if (unselectingWithLeftOrRightChangesCursorPosition())
            keys.addKeyClick(BobUI::Key_Left);

        // selecting '12'
        keys.addKeyClick(BobUI::Key_Right, BobUI::ShiftModifier);
        keys.addKeyClick(BobUI::Key_Right, BobUI::ShiftModifier);

        // deleting '12'
        keys.addKeyClick(BobUI::Key_Delete);

        expectedString << "AB";
        expectedString << "AB12";

        BOBUIest::newRow("Inserts,moving,selection, delete and undo") << keys << expectedString;
    }

    {
        BOBUIestEventList keys;
        QStringList expectedString;

        // inserting 'ABCD'
        keys.addKeyClick(BobUI::Key_A);
        keys.addKeyClick(BobUI::Key_B);
        keys.addKeyClick(BobUI::Key_C);
        keys.addKeyClick(BobUI::Key_D);

        //move left two
        keys.addKeyClick(BobUI::Key_Left);
        keys.addKeyClick(BobUI::Key_Left);

        // inserting '1234'
        keys.addKeyClick(BobUI::Key_1);
        keys.addKeyClick(BobUI::Key_2);
        keys.addKeyClick(BobUI::Key_3);
        keys.addKeyClick(BobUI::Key_4);

        // selecting '1234'
        keys.addKeyClick(BobUI::Key_Left, BobUI::ShiftModifier);
        keys.addKeyClick(BobUI::Key_Left, BobUI::ShiftModifier);
        keys.addKeyClick(BobUI::Key_Left, BobUI::ShiftModifier);
        keys.addKeyClick(BobUI::Key_Left, BobUI::ShiftModifier);

        // overwriting '1234' with '5'
        keys.addKeyClick(BobUI::Key_5);

        // undoing deletion of 'AB'
        keys.addKeyClick(BobUI::Key_Z, BobUI::ControlModifier);

        // overwriting '1234' with '6'
        keys.addKeyClick(BobUI::Key_6);

        expectedString << "ab6cd";
        // for versions previous to 3.2 we overwrite needed two undo operations
        expectedString << "ab1234cd";
        expectedString << "abcd";

        BOBUIest::newRow("Inserts,moving,selection and undo, removing selection") << keys << expectedString;
    }

    {
        BOBUIestEventList keys;
        QStringList expectedString;

        // inserting 'ABC'
        keys.addKeyClick('A');
        keys.addKeyClick('B');
        keys.addKeyClick('C');

        // removes 'C'
        keys.addKeyClick(BobUI::Key_Backspace);

        expectedString << "AB";
        expectedString << "ABC";

        BOBUIest::newRow("Inserts,backspace") << keys << expectedString;
    }

    {
        BOBUIestEventList keys;
        QStringList expectedString;

        // inserting 'ABC'
        keys.addKeyClick('A');
        keys.addKeyClick('B');
        keys.addKeyClick('C');

        // removes 'C'
        keys.addKeyClick(BobUI::Key_Backspace);

        // inserting 'Z'
        keys.addKeyClick('Z');

        expectedString << "ABZ";
        expectedString << "AB";
        expectedString << "ABC";

        BOBUIest::newRow("Inserts,backspace,inserts") << keys << expectedString;
    }


    {
        BOBUIestEventList keys;
        QStringList expectedString;

        // inserting '123'
        keys.addKeyClick(BobUI::Key_1);
        keys.addKeyClick(BobUI::Key_2);
        keys.addKeyClick(BobUI::Key_3);
        psKeyClick(keys, BobUI::Key_Home);

        // selecting '123'
        psKeyClick(keys, BobUI::Key_End, BobUI::ShiftModifier);

        // overwriting '123' with 'ABC'
        keys.addKeyClick('A');
        keys.addKeyClick('B');
        keys.addKeyClick('C');

        expectedString << "ABC";
        // for versions previous to 3.2 we overwrite needed two undo operations
        expectedString << "123";

        BOBUIest::newRow("Inserts,moving,selection and overwriting") << keys << expectedString;
    }
}

void tst_QLineEdit::undo_keypressevents()
{
    QFETCH(BOBUIestEventList, keys);
    QFETCH(QStringList, expectedString);

    QLineEdit *testWidget = ensureTestWidget();
    keys.simulate(testWidget);

    for (int i=0; i<expectedString.size(); ++i) {
        QCOMPARE(testWidget->text() , expectedString[i]);
        testWidget->undo();
    }
    QVERIFY(testWidget->text().isEmpty());
}

#ifndef BOBUI_NO_CLIPBOARD
void tst_QLineEdit::BOBUIBUG5786_undoPaste()
{
    if (QGuiApplication::platformName().startsWith(QLatin1String("wayland"), BobUI::CaseInsensitive))
        QSKIP("Wayland: This fails. Figure out why.");

    if (!PlatformClipboard::isAvailable())
        QSKIP("this machine doesn't support the clipboard");
    QString initial("initial");
    QString string("test");
    QString additional("add");
    QApplication::clipboard()->setText(string);
    QLineEdit edit(initial);
    QCOMPARE(edit.text(), initial);
    edit.paste();
    QCOMPARE(edit.text(), initial + string);
    edit.paste();
    QCOMPARE(edit.text(), initial + string + string);
    edit.insert(additional);
    QCOMPARE(edit.text(), initial + string + string + additional);
    edit.undo();
    QCOMPARE(edit.text(), initial + string + string);
    edit.undo();
    QCOMPARE(edit.text(), initial + string);
    edit.undo();
    QCOMPARE(edit.text(), initial);
    edit.selectAll();
    QApplication::clipboard()->setText(QString());
    edit.paste();
    QVERIFY(edit.text().isEmpty());

}
#endif


void tst_QLineEdit::clear()
{
    // checking that clear of empty/nullstring doesn't add to undo history
    QLineEdit *testWidget = ensureTestWidget();
    int max = 5000;
    while (max > 0 && testWidget->isUndoAvailable()) {
        max--;
        testWidget->undo();
    }

    testWidget->clear();
//    QVERIFY(!testWidget->isUndoAvailable());

    // checks that clear actually clears
    testWidget->insert("I am Legend");
    testWidget->clear();
    QVERIFY(testWidget->text().isEmpty());

    // checks that clears can be undone
    testWidget->undo();
    QCOMPARE(testWidget->text(), QString("I am Legend"));
}

void tst_QLineEdit::editingFinished()
{
    if (m_testWidget->hasAcceptableInput())
        validInput = true;
    else
        validInput = false;
}

void tst_QLineEdit::text_data()
{
    BOBUIest::addColumn<QString>("insertString");

    BOBUIest::newRow("Plain text0") << QString("Hello World");
    BOBUIest::newRow("Plain text1") << QString("");
    BOBUIest::newRow("Plain text2") << QString("A");
    BOBUIest::newRow("Plain text3") << QString("ryyryryryryryryryryryryryryryryryryryryryryryryryryryrryryryryryryryryryryryryryryryryryryryryryryryryryryryryryryryryryrryryryryryryryryryryryryry");
    BOBUIest::newRow("Plain text4") << QString("abcdefghijklmnopqrstuvwxyz ABCDEFGHIJKLMNOPQRSTUVWXYZ01234567890`~!@#$%^&*()_-+={[}]|\\:;'?/>.<,\"");
    BOBUIest::newRow("Newlines") << QString("A\nB\nC\n");
    BOBUIest::newRow("Text with nbsp") << QString("Hello") + QChar(0xa0) + "World";
}

void tst_QLineEdit::text()
{
    QFETCH(QString, insertString);
    QLineEdit *testWidget = ensureTestWidget();
    testWidget->setText(insertString);
    QCOMPARE(testWidget->text(), insertString);
}

void tst_QLineEdit::textMask_data()
{
    BOBUIest::addColumn<QString>("insertString");

    BOBUIest::newRow( "Plain text1" ) << QString( "" );
}

void tst_QLineEdit::textMask()
{
    QFETCH( QString, insertString );
    QLineEdit *testWidget = ensureTestWidget();
    testWidget->setInputMask( "#" );
    testWidget->setText( insertString );
    QCOMPARE( testWidget->text(), insertString );
}

void tst_QLineEdit::setText()
{
    QLineEdit *testWidget = ensureTestWidget();
    QSignalSpy editedSpy(testWidget, SIGNAL(textEdited(QString)));
    QSignalSpy changedSpy(testWidget, SIGNAL(textChanged(QString)));
    testWidget->setText("hello");
    QCOMPARE(editedSpy.size(), 0);
    QCOMPARE(changedSpy.value(0).value(0).toString(), QString("hello"));
}

void tst_QLineEdit::displayText_data()
{
    BOBUIest::addColumn<QString>("insertString");
    BOBUIest::addColumn<QString>("expectedString");
    BOBUIest::addColumn<QLineEdit::EchoMode>("mode");
    BOBUIest::addColumn<bool>("use_setText");

    QString s;
    QLineEdit::EchoMode m;

    for (int i=0; i<2; i++) {
        QString key_mode_str;
        bool use_setText;
        if (i==0) {
            key_mode_str = "setText_";
            use_setText = true;
        } else {
            key_mode_str = "useKeys_";
            use_setText = false;
        }
        s = key_mode_str + "Normal";
        m = QLineEdit::Normal;
        BOBUIest::newRow(QString(s + " text0").toLatin1()) << QString("Hello World") <<
                      QString("Hello World") <<
                      m << bool(use_setText);
        BOBUIest::newRow(QString(s + " text1").toLatin1()) << QString("") <<
                      QString("") <<
                      m << bool(use_setText);
        BOBUIest::newRow(QString(s + " text2").toLatin1()) << QString("A") <<
                      QString("A") <<
                      m << bool(use_setText);
        BOBUIest::newRow(QString(s + " text3").toLatin1()) << QString("ryyryryryryryryryryryryryryryryryryryryryryryryryryryrryryryryryryryryryryryryryryryryryryryryryryryryryryryryryryryryryrryryryryryryryryryryryryry") <<
                      QString("ryyryryryryryryryryryryryryryryryryryryryryryryryryryrryryryryryryryryryryryryryryryryryryryryryryryryryryryryryryryryryrryryryryryryryryryryryryry") <<
                      m << bool(use_setText);
        BOBUIest::newRow(QString(s + " text4").toLatin1()) << QString("abcdefghijklmnopqrstuvwxyz ABCDEFGHIJKLMNOPQRSTUVWXYZ01234567890`~!@#$%^&*()_-+={[}]|\\:;'?/>.<,\"") <<
                      QString("abcdefghijklmnopqrstuvwxyz ABCDEFGHIJKLMNOPQRSTUVWXYZ01234567890`~!@#$%^&*()_-+={[}]|\\:;'?/>.<,\"") <<
                      m << bool(use_setText);
        BOBUIest::newRow(QString(s + " text with nbsp").toLatin1()) << QString("Hello") + QChar(0xa0) + "World" <<
                      QString("Hello") + QChar(0xa0) + "World" <<
                      m << bool(use_setText);
        s = key_mode_str + "NoEcho";
        m = QLineEdit::NoEcho;
        BOBUIest::newRow(QString(s + " text0").toLatin1()) << QString("Hello World") <<
                      QString("") <<
                      m << bool(use_setText);
        BOBUIest::newRow(QString(s + " text1").toLatin1()) << QString("") <<
                      QString("") <<
                      m << bool(use_setText);
        BOBUIest::newRow(QString(s + " text2").toLatin1()) << QString("A") <<
                      QString("") <<
                      m << bool(use_setText);
        BOBUIest::newRow(QString(s + " text3").toLatin1()) << QString("ryyryryryryryryryryryryryryryryryryryryryryryryryryryrryryryryryryryryryryryryryryryryryryryryryryryryryryryryryryryryryrryryryryryryryryryryryryry") <<
                      QString("") <<
                      m << bool(use_setText);
        BOBUIest::newRow(QString(s + " text4").toLatin1()) << QString("abcdefghijklmnopqrstuvwxyz ABCDEFGHIJKLMNOPQRSTUVWXYZ01234567890`~!@#$%^&*()_-+={[}]|\\:;'?/>.<,\"") <<
                      QString("") <<
                      m << bool(use_setText);
        BOBUIest::newRow(QString(s + " text with nbsp").toLatin1()) << QString("Hello") + QChar(0xa0) + "World" <<
                      QString("") <<
                      m << bool(use_setText);
        s = key_mode_str + "Password";
        m = QLineEdit::Password;
        const int passwordCharacter = qApp->style()->styleHint(QStyle::SH_LineEdit_PasswordCharacter, 0, m_testWidget);
        QVERIFY(passwordCharacter <= 0xFFFF);
        const QChar passChar(passwordCharacter);
        QString input;
        QString pass;
        input = "Hello World";
        pass.resize(input.size());
        pass.fill(passChar);
        BOBUIest::newRow(QString(s + " text0").toLatin1()) << input << pass << m << bool(use_setText);
        BOBUIest::newRow(QString(s + " text1").toLatin1()) << QString("") <<
                      QString("") <<
                      m << bool(use_setText);
        BOBUIest::newRow(QString(s + " text2").toLatin1()) << QString("A") << QString(passChar) << m << bool(use_setText);
        input = QString("ryyryryryryryryryryryryryryryryryryryryryryryryryryryrryryryryryryryryryryryryryryryryryryryryryryryryryryryryryryryryryrryryryryryryryryryryryryry");
        pass.resize(input.size());
        pass.fill(passChar);
        BOBUIest::newRow(QString(s + " text3").toLatin1()) << input << pass << m << bool(use_setText);
        input = QString("abcdefghijklmnopqrstuvwxyz ABCDEFGHIJKLMNOPQRSTUVWXYZ01234567890`~!@#$%^&*()_-+={[}]|\\:;'?/>.<,\"");
        pass.fill(passChar, input.size());
        BOBUIest::newRow(QString(s + " text4").toLatin1()) << input << pass << m << bool(use_setText);
        input = QString("Hello") + QChar(0xa0) + "World";
        pass.resize(input.size());
        pass.fill(passChar);
        BOBUIest::newRow(QString(s + " text with nbsp").toLatin1()) << input << pass << m << bool(use_setText);
    }
}

void tst_QLineEdit::displayText()
{
    QFETCH(QString, insertString);
    QFETCH(QString, expectedString);
    QFETCH(QLineEdit::EchoMode, mode);
    //QFETCH(bool, use_setText);  Currently unused.

    QLineEdit *testWidget = ensureTestWidget();
    testWidget->setEchoMode(mode);
    testWidget->setText(insertString);
    QCOMPARE(testWidget->displayText(), expectedString);
    QCOMPARE(testWidget->echoMode(), mode);
}

void tst_QLineEdit::passwordEchoOnEdit()
{
    if (QGuiApplication::platformName().startsWith(QLatin1String("wayland"), BobUI::CaseInsensitive))
        QSKIP("Wayland: This fails. Figure out why.");

    QStyleOptionFrame opt;
    QLineEdit *testWidget = ensureTestWidget();
    const int passwordCharacter = testWidget->style()->styleHint(QStyle::SH_LineEdit_PasswordCharacter, &opt, testWidget);
    QVERIFY(passwordCharacter <= 0xFFFF);
    const QChar fillChar(passwordCharacter);

    testWidget->setEchoMode(QLineEdit::PasswordEchoOnEdit);
    testWidget->setFocus();
    centerOnScreen(testWidget);
    testWidget->show();
    testWidget->raise();
    QVERIFY(BOBUIest::qWaitForWindowExposed(testWidget));
    BOBUIRY_VERIFY(testWidget->hasFocus());

    BOBUIest::keyPress(testWidget, '0');
    BOBUIest::keyPress(testWidget, '1');
    BOBUIest::keyPress(testWidget, '2');
    BOBUIest::keyPress(testWidget, '3');
    BOBUIest::keyPress(testWidget, '4');
    QCOMPARE(testWidget->displayText(), QString("01234"));
    testWidget->clearFocus();
    QVERIFY(!testWidget->hasFocus());
    QCOMPARE(testWidget->displayText(), QString(5, fillChar));
    testWidget->setFocus();
    BOBUIRY_VERIFY(testWidget->hasFocus());

    QCOMPARE(testWidget->displayText(), QString(5, fillChar));
    BOBUIest::keyPress(testWidget, '0');
    QCOMPARE(testWidget->displayText(), QString("0"));

    // restore clean state
    testWidget->setEchoMode(QLineEdit::Normal);
}

void tst_QLineEdit::passwordEchoDelay()
{
    if (QGuiApplication::platformName().startsWith(QLatin1String("wayland"), BobUI::CaseInsensitive))
        QSKIP("Wayland: This fails. Figure out why.");

    QLineEdit *testWidget = ensureTestWidget();
    int delay = qGuiApp->styleHints()->passwordMaskDelay();
#if defined BOBUI_BUILD_INTERNAL
    QLineEditPrivate *priv = QLineEditPrivate::get(testWidget);
    QWidgetLineControl *control = priv->control;
    control->m_passwordMaskDelayOverride = 200;
    delay = 200;
#endif
    if (delay <= 0)
        QSKIP("Platform not defining echo delay and overriding only possible in internal build");

    QStyleOptionFrame opt;
    const int passwordCharacter = testWidget->style()->styleHint(QStyle::SH_LineEdit_PasswordCharacter, &opt, testWidget);
    QVERIFY(passwordCharacter <= 0xFFFF);
    const QChar fillChar(passwordCharacter);

    testWidget->setEchoMode(QLineEdit::Password);
    testWidget->setFocus();
    centerOnScreen(testWidget);
    testWidget->show();
    testWidget->raise();
    QVERIFY(BOBUIest::qWaitForWindowExposed(testWidget));
    BOBUIRY_VERIFY(testWidget->hasFocus());

    BOBUIest::keyPress(testWidget, '0');
    BOBUIest::keyPress(testWidget, '1');
    BOBUIest::keyPress(testWidget, '2');
    QCOMPARE(testWidget->displayText(), QString(2, fillChar) + QLatin1Char('2'));
    BOBUIest::keyPress(testWidget, '3');
    BOBUIest::keyPress(testWidget, '4');
    QCOMPARE(testWidget->displayText(), QString(4, fillChar) + QLatin1Char('4'));
    BOBUIest::keyPress(testWidget, BobUI::Key_Backspace);
    QCOMPARE(testWidget->displayText(), QString(4, fillChar));
    BOBUIest::keyPress(testWidget, '4');
    QCOMPARE(testWidget->displayText(), QString(4, fillChar) + QLatin1Char('4'));
    BOBUIest::qWait(delay);
    BOBUIRY_COMPARE(testWidget->displayText(), QString(5, fillChar));
    BOBUIest::keyPress(testWidget, '5');
    QCOMPARE(testWidget->displayText(), QString(5, fillChar) + QLatin1Char('5'));
    testWidget->clearFocus();
    QVERIFY(!testWidget->hasFocus());
    QCOMPARE(testWidget->displayText(), QString(6, fillChar));
    testWidget->setFocus();
    BOBUIRY_VERIFY(testWidget->hasFocus());
    QCOMPARE(testWidget->displayText(), QString(6, fillChar));
    BOBUIest::keyPress(testWidget, '6');
    QCOMPARE(testWidget->displayText(), QString(6, fillChar) + QLatin1Char('6'));

    QInputMethodEvent ev;
    ev.setCommitString(QLatin1String("7"));
    QApplication::sendEvent(testWidget, &ev);
    QCOMPARE(testWidget->displayText(), QString(7, fillChar) + QLatin1Char('7'));

    testWidget->setCursorPosition(3);
    QCOMPARE(testWidget->displayText(), QString(7, fillChar) + QLatin1Char('7'));
    BOBUIest::keyPress(testWidget, 'a');
    QCOMPARE(testWidget->displayText(), QString(3, fillChar) + QLatin1Char('a') + QString(5, fillChar));
    BOBUIest::keyPress(testWidget, BobUI::Key_Backspace);
    QCOMPARE(testWidget->displayText(), QString(8, fillChar));

    // restore clean state
    testWidget->setEchoMode(QLineEdit::Normal);
}

void tst_QLineEdit::maxLength_mask_data()
{
    BOBUIest::addColumn<QString>("mask");
    BOBUIest::addColumn<int>("expectedLength");

    BOBUIest::newRow("mask_case") << QString(">000<>00<000") << 8;
    BOBUIest::newRow("mask_nocase") << QString("00000000") << 8;
    BOBUIest::newRow("mask_null") << QString() << 32767;
    BOBUIest::newRow("mask_escape") << QString("\\A\\aAA") << 4;
}

void tst_QLineEdit::maxLength_mask()
{
    QFETCH(QString, mask);
    QFETCH(int, expectedLength);

    QLineEdit *testWidget = ensureTestWidget();
    testWidget->setInputMask(mask);

    QCOMPARE(testWidget->maxLength(), expectedLength);
}

void tst_QLineEdit::maxLength_data()
{
    BOBUIest::addColumn<QString>("insertString");
    BOBUIest::addColumn<QString>("expectedString");
    BOBUIest::addColumn<int>("length");
    BOBUIest::addColumn<bool>("insertBeforeSettingMaxLength");
    BOBUIest::addColumn<bool>("use_setText");

    BOBUIest::newRow("keyclick before0") << QString("this is a test.") << QString("this is a test.") << 20 << bool(true) << bool(false);
    BOBUIest::newRow("keyclick before1") << QString("this is a test.") << QString("this is a ") << 10 << bool(true) << bool(false);
    BOBUIest::newRow("keyclick after0") << QString("this is a test.") << QString("this is a test.") << 20 << bool(false) << bool(false);
    BOBUIest::newRow("keyclick after1") << QString("this is a test.") << QString("this is a ") << 10 << bool(false) << bool(false);
    BOBUIest::newRow("settext before0") << QString("this is a test.") << QString("this is a test.") << 20 << bool(true) << bool(true);
    BOBUIest::newRow("settext before1") << QString("this is a test.") << QString("this is a ") << 10 << bool(true) << bool(true);
    BOBUIest::newRow("settext after0") << QString("this is a test.") << QString("this is a test.") << 20 << bool(false) << bool(true);
    BOBUIest::newRow("settext after1") << QString("this is a test.") << QString("this is a ") << 10 << bool(false) << bool(true);
}

void tst_QLineEdit::maxLength()
{
    QFETCH(QString, insertString);
    QFETCH(QString, expectedString);
    QFETCH(int, length);
    QFETCH(bool, insertBeforeSettingMaxLength);
    QFETCH(bool, use_setText);

    // in some cases we set the maxLength _before_ entering the text.
    QLineEdit *testWidget = ensureTestWidget();
    if (!insertBeforeSettingMaxLength)
        testWidget->setMaxLength(length);

    // I expect MaxLength to work BOTH with entering live characters AND with setting the text.
    if (use_setText) {
        // Enter insertString using setText.
        testWidget->setText(insertString);
    } else {
        // Enter insertString as a sequence of keyClicks
        BOBUIest::keyClicks(testWidget, insertString);
    }

    // in all other cases we set the maxLength _after_ entering the text.
    if (insertBeforeSettingMaxLength) {
        changed_count = 0;
        testWidget->setMaxLength(length);

        // Make sure that the textChanged is not emitted unless the text is actually changed
        if (insertString == expectedString) {
            QCOMPARE(changed_count, 0);
        } else {
            QCOMPARE(changed_count, 1);
        }
    }

    // and check if we get the expected string back
    QCOMPARE(testWidget->text(), expectedString);
}

void tst_QLineEdit::isReadOnly()
{
    QLineEdit *testWidget = ensureTestWidget();
    QVERIFY(!testWidget->isReadOnly());

    // start with a basic text
    BOBUIest::keyClicks(testWidget, "the quick brown fox");
    QCOMPARE(testWidget->text(), QString("the quick brown fox"));

    // do a quick check to verify that we can indeed edit the text
    testWidget->home(false);
    testWidget->cursorForward(false, 10);
    BOBUIest::keyClicks(testWidget, "dark ");
    QCOMPARE(testWidget->text(), QString("the quick dark brown fox"));

    testWidget->setReadOnly(true);
    QVERIFY(testWidget->isReadOnly());

    // verify that we cannot edit the text anymore
    testWidget->home(false);
    testWidget->cursorForward(false, 10);
    BOBUIest::keyClick(testWidget, BobUI::Key_Delete);
    QCOMPARE(testWidget->text(), QString("the quick dark brown fox"));
    testWidget->cursorForward(false, 10);
    BOBUIest::keyClicks(testWidget, "this should not have any effect!! ");
    QCOMPARE(testWidget->text(), QString("the quick dark brown fox"));
}

class BlinkTestLineEdit : public QLineEdit
{
public:
    void paintEvent(QPaintEvent *e) override
    {
        ++updates;
        QLineEdit::paintEvent(e);
    }

    int updates;
};

void tst_QLineEdit::noCursorBlinkWhenReadOnly()
{
    int cursorFlashTime = QApplication::cursorFlashTime();
    if (cursorFlashTime == 0)
        return;
    BlinkTestLineEdit le;
    centerOnScreen(&le);
    le.show();
    le.setFocus();
    QVERIFY(BOBUIest::qWaitForWindowFocused(&le));
    QVERIFY(le.hasFocus());
    le.updates = 0;
    BOBUIest::qWait(cursorFlashTime);
    QVERIFY(le.updates > 0);
    le.setReadOnly(true);
    BOBUIest::qWait(10);
    le.updates = 0;
    BOBUIest::qWait(cursorFlashTime);
    QCOMPARE(le.updates, 0);
    le.setReadOnly(false);
    BOBUIest::qWait(10);
    le.updates = 0;
    BOBUIest::qWait(cursorFlashTime);
    QVERIFY(le.updates > 0);
}

static void figureOutProperKey(BobUI::Key &key, BobUI::KeyboardModifiers &pressState)
{
#ifdef Q_OS_MAC
    long roll = QRandomGenerator::global()->bounded(3);
    switch (roll) {
    case 0:
        key = key == BobUI::Key_Home ? BobUI::Key_Up : BobUI::Key_Down;
        break;
    case 1:
    case 2:
        key = key == BobUI::Key_Home ? BobUI::Key_Left : BobUI::Key_Right;
        pressState |= (roll == 1) ? BobUI::ControlModifier : BobUI::MetaModifier;
        break;
    }
#else
    Q_UNUSED(key);
    Q_UNUSED(pressState);
#endif
}

// Platform specific move. Home and End do nothing on the Mac,
// so do something a bit smarter than tons of #ifdefs
void tst_QLineEdit::psKeyClick(QWidget *target, BobUI::Key key, BobUI::KeyboardModifiers pressState)
{
    figureOutProperKey(key, pressState);
    BOBUIest::keyClick(target, key, pressState);
}

void tst_QLineEdit::psKeyClick(BOBUIestEventList &keys, BobUI::Key key, BobUI::KeyboardModifiers pressState)
{
    figureOutProperKey(key, pressState);
    keys.addKeyClick(key, pressState);
}

#if BOBUI_CONFIG(shortcut)

void tst_QLineEdit::addKeySequenceStandardKey(BOBUIestEventList &keys, QKeySequence::StandardKey key)
{
    QKeySequence keyseq = QKeySequence(key);
    for (int i = 0; i < keyseq.count(); ++i)
        keys.addKeyClick( BobUI::Key( keyseq[i].toCombined() & ~BobUI::KeyboardModifierMask),
                          BobUI::KeyboardModifier(keyseq[i].toCombined() & BobUI::KeyboardModifierMask) );
}

#endif // BOBUI_CONFIG(shortcut)

void tst_QLineEdit::cursorPosition()
{
    QLineEdit *testWidget = ensureTestWidget();
    QCOMPARE(testWidget->cursorPosition(), 0);

    // start with a basic text
    BOBUIest::keyClicks(testWidget, "The");
    QCOMPARE(testWidget->cursorPosition(), 3);
    BOBUIest::keyClicks(testWidget, " quick");
    QCOMPARE(testWidget->cursorPosition(), 9);
    BOBUIest::keyClicks(testWidget, " brown fox jumps over the lazy dog");
    QCOMPARE(testWidget->cursorPosition(), 43);

    // The text we have now is:
    //           1         2         3         4         5
    // 012345678901234567890123456789012345678901234567890
    // The quick brown fox jumps over the lazy dog

    // next we will check some of the cursor movement functions
    testWidget->end(false);
    QCOMPARE(testWidget->cursorPosition() , 43);
    testWidget->cursorForward(false, -1);
    QCOMPARE(testWidget->cursorPosition() , 42);
    testWidget->cursorBackward(false, 1);
    QCOMPARE(testWidget->cursorPosition() , 41);
    testWidget->home(false);
    QCOMPARE(testWidget->cursorPosition() , 0);
    testWidget->cursorForward(false, 1);
    QCOMPARE(testWidget->cursorPosition() , 1);
    testWidget->cursorForward(false, 9);
    QCOMPARE(testWidget->cursorPosition() , 10);
    testWidget->cursorWordForward(false); // 'fox'
    QCOMPARE(testWidget->cursorPosition(), 16);
    testWidget->cursorWordForward(false); // 'jumps'
    QCOMPARE(testWidget->cursorPosition(), 20);
    testWidget->cursorWordBackward(false); // 'fox'
    QCOMPARE(testWidget->cursorPosition(), 16);
    testWidget->cursorWordBackward(false); // 'brown'
    testWidget->cursorWordBackward(false); // 'quick'
    testWidget->cursorWordBackward(false); // 'The'
    QCOMPARE(testWidget->cursorPosition(), 0);
    testWidget->cursorWordBackward(false); // 'The'
    QCOMPARE(testWidget->cursorPosition(), 0);

    // Cursor position should be 0 here!
    int i;
    for (i=0; i<5; i++)
        BOBUIest::keyClick(testWidget, BobUI::Key_Right);
    QCOMPARE(testWidget->cursorPosition(), 5);
    psKeyClick(testWidget, BobUI::Key_End);
    QCOMPARE(testWidget->cursorPosition(), 43);
    BOBUIest::keyClick(testWidget, BobUI::Key_Left);
    QCOMPARE(testWidget->cursorPosition(), 42);
    BOBUIest::keyClick(testWidget, BobUI::Key_Left);
    QCOMPARE(testWidget->cursorPosition(), 41);
    psKeyClick(testWidget, BobUI::Key_Home);
    QCOMPARE(testWidget->cursorPosition(), 0);

    // cursorposition when maxlength is set
    int maxLength = 9;
    testWidget->clear();
    testWidget->setMaxLength(maxLength);
    QCOMPARE(testWidget->cursorPosition() , 0);
    BOBUIest::keyClicks(testWidget, "123ABC123");
    QCOMPARE(testWidget->cursorPosition() , maxLength);
    psKeyClick(testWidget, BobUI::Key_Home);
    QCOMPARE(testWidget->cursorPosition() , 0);
    psKeyClick(testWidget, BobUI::Key_End);
    QCOMPARE(testWidget->cursorPosition() , maxLength);
}

/* // tested in cursorPosition
void tst_QLineEdit::cursorLeft()
void tst_QLineEdit::cursorRight()
void tst_QLineEdit::cursorForward()
void tst_QLineEdit::cursorBackward()
void tst_QLineEdit::cursorWordForward()
void tst_QLineEdit::cursorWordBackward()
void tst_QLineEdit::home()
void tst_QLineEdit::end()
*/

void tst_QLineEdit::cursorPositionChanged_data()
{
    BOBUIest::addColumn<BOBUIestEventList>("input");
    BOBUIest::addColumn<int>("lastPos");
    BOBUIest::addColumn<int>("newPos");

    BOBUIestEventList keys;
    keys.addKeyClick(BobUI::Key_A);
    BOBUIest::newRow("a") << keys << -1 << 1;
    keys.clear();

    keys.addKeyClick(BobUI::Key_A);
    keys.addKeyClick(BobUI::Key_B);
    keys.addKeyClick(BobUI::Key_C);
    psKeyClick(keys, BobUI::Key_Home);
    BOBUIest::newRow("abc<home>") << keys << 3 << 0;
    keys.clear();

    keys.addKeyClick(BobUI::Key_A);
    keys.addKeyClick(BobUI::Key_B);
    keys.addKeyClick(BobUI::Key_C);
    keys.addKeyClick(BobUI::Key_Left);
    BOBUIest::newRow("abc<left>") << keys << 3 << 2;
    keys.clear();

    keys.addKeyClick(BobUI::Key_A);
    keys.addKeyClick(BobUI::Key_B);
    keys.addKeyClick(BobUI::Key_C);
    keys.addKeyClick(BobUI::Key_Right);
    BOBUIest::newRow("abc<right>") << keys << 2 << 3;
    keys.clear();

    keys.addKeyClick(BobUI::Key_A);
    keys.addKeyClick(BobUI::Key_B);
    keys.addKeyClick(BobUI::Key_C);
    psKeyClick(keys, BobUI::Key_Home);
    keys.addKeyClick(BobUI::Key_Right);
    BOBUIest::newRow("abc<home><right>") << keys << 0 << 1;
    keys.clear();

    keys.addKeyClick(BobUI::Key_A);
    keys.addKeyClick(BobUI::Key_B);
    keys.addKeyClick(BobUI::Key_C);
    keys.addKeyClick(BobUI::Key_Backspace);
    BOBUIest::newRow("abc<backspace>") << keys << 3 << 2;
    keys.clear();

    keys.addKeyClick(BobUI::Key_A);
    keys.addKeyClick(BobUI::Key_B);
    keys.addKeyClick(BobUI::Key_C);
    keys.addKeyClick(BobUI::Key_Delete);
    BOBUIest::newRow("abc<delete>") << keys << 2 << 3;
    keys.clear();

    keys.addKeyClick(BobUI::Key_A);
    keys.addKeyClick(BobUI::Key_B);
    keys.addKeyClick(BobUI::Key_C);
    keys.addKeyClick(BobUI::Key_Left);
    keys.addKeyClick(BobUI::Key_Delete);
    BOBUIest::newRow("abc<left><delete>") << keys << 3 << 2;
    keys.clear();

    keys.addKeyClick(BobUI::Key_A);
    keys.addKeyClick(BobUI::Key_B);
    keys.addKeyClick(BobUI::Key_C);
    psKeyClick(keys, BobUI::Key_Home);
    psKeyClick(keys, BobUI::Key_End);
    BOBUIest::newRow("abc<home><end>") << keys << 0 << 3;
    keys.clear();

    keys.addKeyClick(BobUI::Key_A);
    keys.addKeyClick(BobUI::Key_B);
    keys.addKeyClick(BobUI::Key_C);
    keys.addKeyClick(BobUI::Key_Space);
    keys.addKeyClick(BobUI::Key_D);
    keys.addKeyClick(BobUI::Key_E);
    keys.addKeyClick(BobUI::Key_F);
    keys.addKeyClick(BobUI::Key_Home);
    keys.addKeyClick(BobUI::Key_Right, BobUI::ControlModifier);
    BOBUIest::newRow("abc efg<home><ctrl-right>") << keys
#ifndef Q_OS_MAC
        << 0 << 4;
#else
        << 6 << 7;
#endif
    keys.clear();

#ifdef Q_OS_MAC
    keys.addKeyClick(BobUI::Key_A);
    keys.addKeyClick(BobUI::Key_B);
    keys.addKeyClick(BobUI::Key_C);
    keys.addKeyClick(BobUI::Key_Space);
    keys.addKeyClick(BobUI::Key_D);
    keys.addKeyClick(BobUI::Key_E);
    keys.addKeyClick(BobUI::Key_F);
    keys.addKeyClick(BobUI::Key_Up);
    keys.addKeyClick(BobUI::Key_Right, BobUI::AltModifier);
    BOBUIest::newRow("mac equivalent abc efg<up><option-right>") << keys << 0 << 4;
    keys.clear();
#endif

    keys.addKeyClick(BobUI::Key_A);
    keys.addKeyClick(BobUI::Key_B);
    keys.addKeyClick(BobUI::Key_C);
    keys.addKeyClick(BobUI::Key_Space);
    keys.addKeyClick(BobUI::Key_D);
    keys.addKeyClick(BobUI::Key_E);
    keys.addKeyClick(BobUI::Key_F);
    keys.addKeyClick(BobUI::Key_Left, BobUI::ControlModifier);
    BOBUIest::newRow("abc efg<ctrl-left>") << keys << 7
#ifndef Q_OS_MAC
        << 4;
#else
        << 0;
#endif
    keys.clear();
#ifdef Q_OS_MAC
    keys.addKeyClick(BobUI::Key_A);
    keys.addKeyClick(BobUI::Key_B);
    keys.addKeyClick(BobUI::Key_C);
    keys.addKeyClick(BobUI::Key_Space);
    keys.addKeyClick(BobUI::Key_D);
    keys.addKeyClick(BobUI::Key_E);
    keys.addKeyClick(BobUI::Key_F);
    keys.addKeyClick(BobUI::Key_Left, BobUI::AltModifier);
    BOBUIest::newRow("mac equivalent abc efg<option-left>") << keys << 7 << 4;
    keys.clear();
#endif
}


void tst_QLineEdit::cursorPositionChanged()
{
    QFETCH(BOBUIestEventList, input);
    QFETCH(int, lastPos);
    QFETCH(int, newPos);

    lastCursorPos = 0;
    newCursorPos = 0;
    QLineEdit *testWidget = ensureTestWidget();
    input.simulate(testWidget);
    QCOMPARE(lastCursorPos, lastPos);
    QCOMPARE(newCursorPos, newPos);

    QInputMethodQueryEvent queryEvent(BobUI::ImCursorPosition | BobUI::ImAbsolutePosition);
    QApplication::sendEvent(testWidget, &queryEvent);
    QCOMPARE(queryEvent.value(BobUI::ImCursorPosition).toInt(), newPos);
    QCOMPARE(queryEvent.value(BobUI::ImAbsolutePosition).toInt(), newPos);
}

void tst_QLineEdit::selectedText()
{
    QString testString = "Abc defg hijklmno, p 'qrst' uvw xyz";

    // start with a basic text
    QLineEdit *testWidget = ensureTestWidget();
    testWidget->setText(testString);
    selection_count = 0;

    // The text we have now is:
    //           1         2         3         4         5
    // 012345678901234567890123456789012345678901234567890
    // Abc defg hijklmno, p 'qrst' uvw xyz

    testWidget->home(false);
    QVERIFY(!testWidget->hasSelectedText());
    QCOMPARE(testWidget->selectedText(), QString());

    // play a bit with the cursorForward, cursorBackward(), etc
    testWidget->cursorForward(true, 9);
    QVERIFY(testWidget->hasSelectedText());
    QCOMPARE(testWidget->selectedText(), QString("Abc defg "));
    QCOMPARE(selection_count, 1);

    // reset selection
    testWidget->home(false);
    QVERIFY(!testWidget->hasSelectedText());
    QCOMPARE(testWidget->selectedText(), QString());
    selection_count = 0;
}

/* // tested in selectedText
void tst_QLineEdit::backspace()
void tst_QLineEdit::del()
void tst_QLineEdit::selectionChanged()
void tst_QLineEdit::selectAll()
void tst_QLineEdit::deselect()
*/

void tst_QLineEdit::onSelectionChanged()
{
    selection_count++;
}

void tst_QLineEdit::deleteSelectedText()
{
    const QString text = QString::fromLatin1("bar");
    QLineEdit edit( text );
    QCOMPARE(edit.text(), text);

    edit.selectAll();

    BOBUIest::keyClick(&edit, BobUI::Key_Delete, {});
    QVERIFY(edit.text().isEmpty());

    edit.setText(text);
    edit.selectAll();

#ifndef BOBUI_NO_CONTEXTMENU
    QMenu *menu = edit.createStandardContextMenu();
    for (int i = 0; i < menu->actions().size(); ++i) {
        QAction *current = menu->actions().at(i);
        if (current->text() == QLineEdit::tr("Delete")) {
            current->trigger(); //this will delete the whole text selected
            QVERIFY(edit.text().isEmpty());
        }
    }
#endif // BOBUI_NO_CONTEXTMENU

}

class ToUpperValidator : public QValidator
{
public:
    ToUpperValidator() {}
    State validate(QString &input, int &) const override
    {
        input = input.toUpper();
        return QValidator::Acceptable;
    }
};

void tst_QLineEdit::textChangedAndTextEdited()
{
    changed_count = 0;
    edited_count = 0;

    QLineEdit *testWidget = ensureTestWidget();
    BOBUIest::keyClick(testWidget, BobUI::Key_A);
    QCOMPARE(changed_count, 1);
    QCOMPARE(edited_count, changed_count);
    BOBUIest::keyClick(testWidget, 'b');
    QCOMPARE(changed_count, 2);
    QCOMPARE(edited_count, changed_count);
    BOBUIest::keyClick(testWidget, 'c');
    QCOMPARE(changed_count, 3);
    QCOMPARE(edited_count, changed_count);
    BOBUIest::keyClick(testWidget, ' ');
    QCOMPARE(changed_count, 4);
    QCOMPARE(edited_count, changed_count);
    BOBUIest::keyClick(testWidget, 'd');
    QCOMPARE(changed_count, 5);
    QCOMPARE(edited_count, changed_count);

    changed_count = 0;
    edited_count = 0;
    changed_string.clear();

    testWidget->setText("foo");
    QCOMPARE(changed_count, 1);
    QCOMPARE(edited_count, 0);
    QCOMPARE(changed_string, QString("foo"));

    changed_count = 0;
    edited_count = 0;
    changed_string.clear();

    testWidget->setText("");
    QCOMPARE(changed_count, 1);
    QCOMPARE(edited_count, 0);
    QVERIFY(changed_string.isEmpty());
    QVERIFY(!changed_string.isNull());

    changed_count = 0;
    edited_count = 0;
    changed_string.clear();

    QScopedPointer<ToUpperValidator> validator(new ToUpperValidator());
    testWidget->setValidator(validator.data());
    testWidget->setText("foo");
    QCOMPARE(changed_count, 1);
    QCOMPARE(edited_count, 0);
    QCOMPARE(changed_string, QLatin1String("FOO"));
    testWidget->setCursorPosition(sizeof("foo"));
    BOBUIest::keyClick(testWidget, 'b');
    QCOMPARE(changed_count, 2);
    QCOMPARE(edited_count, 1);
    QCOMPARE(changed_string, QLatin1String("FOOB"));
    testWidget->setValidator(nullptr);
}

void tst_QLineEdit::onTextChanged(const QString &text)
{
    changed_count++;
    changed_string = text;
}

void tst_QLineEdit::onTextEdited(const QString &/*text*/)
{
    edited_count++;
}


void tst_QLineEdit::onCursorPositionChanged(int oldPos, int newPos)
{
    lastCursorPos = oldPos;
    newCursorPos = newPos;
}

void tst_QLineEdit::returnPressed()
{
    return_count = 0;

    QLineEdit *testWidget = ensureTestWidget();
    BOBUIest::keyClick(testWidget, BobUI::Key_Return);
    QCOMPARE(return_count, 1);
    return_count = 0;

    BOBUIest::keyClick(testWidget, 'A');
    QCOMPARE(return_count, 0);
    BOBUIest::keyClick(testWidget, 'b');
    QCOMPARE(return_count, 0);
    BOBUIest::keyClick(testWidget, 'c');
    QCOMPARE(return_count, 0);
    BOBUIest::keyClick(testWidget, ' ');
    QCOMPARE(return_count, 0);
    BOBUIest::keyClick(testWidget, 'd');
    QCOMPARE(return_count, 0);
    psKeyClick(testWidget, BobUI::Key_Home);
    QCOMPARE(return_count, 0);
    psKeyClick(testWidget, BobUI::Key_End);
    QCOMPARE(return_count, 0);
    BOBUIest::keyClick(testWidget, BobUI::Key_Escape);
    QCOMPARE(return_count, 0);
    BOBUIest::keyClick(testWidget, BobUI::Key_Return);
    QCOMPARE(return_count, 1);
}

// int validator that fixes all !isNumber to '0'
class QIntFixValidator : public QIntValidator {
public:
    QIntFixValidator(int min, int max, QObject *parent) : QIntValidator(min, max, parent) {}
    void fixup (QString &input) const override {
        for (int i=0; i<input.size(); ++i)
            if (!input.at(i).isNumber()) {
                input[(int)i] = QChar('0');
            }
    }
};

void tst_QLineEdit::returnPressed_maskvalidator_data() {
    BOBUIest::addColumn<QString>("inputMask");
    BOBUIest::addColumn<bool>("hasValidator");
    BOBUIest::addColumn<BOBUIestEventList>("input");
    BOBUIest::addColumn<QString>("expectedText");
    BOBUIest::addColumn<bool>("returnPressed");

    {
        BOBUIestEventList keys;
        keys.addKeyClick(BobUI::Key_Home);
        keys.addKeyClick(BobUI::Key_1);
        keys.addKeyClick(BobUI::Key_2);
        keys.addKeyClick(BobUI::Key_3);
        keys.addKeyClick(BobUI::Key_Return);
        BOBUIest::newRow("no mask, no validator, input '123<cr>'")
            << QString()
            << false
            << keys
            << QString("123")
            << true;
    }
    {
        BOBUIestEventList keys;
        keys.addKeyClick(BobUI::Key_Home);
        keys.addKeyClick(BobUI::Key_1);
        keys.addKeyClick(BobUI::Key_2);
        keys.addKeyClick(BobUI::Key_Return);
        BOBUIest::newRow("mask '999', no validator, input '12<cr>'")
            << QString("999")
            << false
            << keys
            << QString("12")
            << false;
    }
    {
        BOBUIestEventList keys;
        keys.addKeyClick(BobUI::Key_Home);
        keys.addKeyClick(BobUI::Key_1);
        keys.addKeyClick(BobUI::Key_2);
        keys.addKeyClick(BobUI::Key_3);
        keys.addKeyClick(BobUI::Key_Return);
        BOBUIest::newRow("mask '999', no validator, input '123<cr>'")
            << QString("999")
            << false
            << keys
            << QString("123")
            << true;
    }
    {
        BOBUIestEventList keys;
        keys.addKeyClick(BobUI::Key_Home);
        keys.addKeyClick(BobUI::Key_1);
        keys.addKeyClick(BobUI::Key_2);
        keys.addKeyClick(BobUI::Key_3);
        keys.addKeyClick(BobUI::Key_Return);
        BOBUIest::newRow("no mask, intfix validator(0,999), input '123<cr>'")
            << QString()
            << true
            << keys
            << QString("123")
            << true;
    }
    {
        BOBUIestEventList keys;
        keys.addKeyClick(BobUI::Key_Home);
        keys.addKeyClick(BobUI::Key_7);
        keys.addKeyClick(BobUI::Key_7);
        keys.addKeyClick(BobUI::Key_7);
        keys.addKeyClick(BobUI::Key_7);
        keys.addKeyClick(BobUI::Key_Return);
        BOBUIest::newRow("no mask, intfix validator(0,999), input '7777<cr>'")
            << QString()
            << true
            << keys
            << QString("777")
            << true;
    }
    {
        BOBUIestEventList keys;
        keys.addKeyClick(BobUI::Key_Home);
        keys.addKeyClick(BobUI::Key_1);
        keys.addKeyClick(BobUI::Key_2);
        keys.addKeyClick(BobUI::Key_Return);
        BOBUIest::newRow("mask '999', intfix validator(0,999), input '12<cr>'")
            << QString("999")
            << true
            << keys
            << QString("12")
            << false;
    }
    {
        BOBUIestEventList keys;
        keys.addKeyClick(BobUI::Key_Home);
        keys.addKeyClick(BobUI::Key_Return);
        BOBUIest::newRow("mask '999', intfix validator(0,999), input '<cr>'")
            << QString("999")
            << true
            << keys
            << QString("000")
            << true;
    }
}

void tst_QLineEdit::returnPressed_maskvalidator()
{
    QFETCH(QString, inputMask);
    QFETCH(bool, hasValidator);
    QFETCH(BOBUIestEventList, input);
    QFETCH(QString, expectedText);
    QFETCH(bool, returnPressed);

    QEXPECT_FAIL("mask '999', intfix validator(0,999), input '12<cr>'", "QIntValidator has changed behaviour. Does not accept spaces. Task 43082.", Abort);
    QLineEdit *testWidget = ensureTestWidget();

    testWidget->setInputMask(inputMask);
    if (hasValidator)
        testWidget->setValidator(new QIntFixValidator(0, 999, testWidget));

    return_count = 0;
    input.simulate(testWidget);

    QCOMPARE(testWidget->text(), expectedText);
    QCOMPARE(return_count , returnPressed ? 1 : 0);
}

void tst_QLineEdit::onReturnPressed()
{
    return_count++;
}

void tst_QLineEdit::setValidator()
{
    // Verify that we can set and re-set a validator.
    QLineEdit *testWidget = ensureTestWidget();
    QVERIFY(!testWidget->validator());

    QIntValidator iv1(0);
    testWidget->setValidator(&iv1);
    QCOMPARE(testWidget->validator(), static_cast<const QValidator*>(&iv1));

    testWidget->setValidator(0);
    QVERIFY(!testWidget->validator());

    QIntValidator iv2(0, 99, 0);
    testWidget->setValidator(&iv2);
    QCOMPARE(testWidget->validator(), static_cast<const QValidator *>(&iv2));

    testWidget->setValidator(0);
    QVERIFY(!testWidget->validator());
}

void tst_QLineEdit::setValidator_QIntValidator_data()
{
    BOBUIest::addColumn<int>("mini");
    BOBUIest::addColumn<int>("maxi");
    BOBUIest::addColumn<QString>("input");
    BOBUIest::addColumn<QString>("expectedText");
    BOBUIest::addColumn<bool>("useKeys");
    BOBUIest::addColumn<bool>("is_valid");
    BOBUIest::addColumn<uint>("echoMode");

    for (int i=0; i<2; i++) {
        bool useKeys = false;
        QString inputMode = "insert ";
        if (i!=0) {
            inputMode = "useKeys ";
            useKeys = true;
        }

        // valid data
        BOBUIest::newRow(QString(inputMode + "range [0,9] valid '1'").toLatin1())
            << 0
            << 9
            << QString("1")
            << QString("1")
            << bool(useKeys)
            << bool(true)
            << uint(QLineEdit::Normal);

        BOBUIest::newRow(QString(inputMode + "range [3,7] valid '3'").toLatin1())
            << 3
            << 7
            << QString("3")
            << QString("3")
            << bool(useKeys)
            << bool(true)
            << uint(QLineEdit::Normal);

        BOBUIest::newRow(QString(inputMode + "range [3,7] valid '7'").toLatin1())
            << 3
            << 7
            << QString("7")
            << QString("7")
            << bool(useKeys)
            << bool(true)
            << uint(QLineEdit::Normal);

        BOBUIest::newRow(QString(inputMode + "range [0,100] valid '9'").toLatin1())
            << 0
            << 100
            << QString("9")
            << QString("9")
            << bool(useKeys)
            << bool(true)
            << uint(QLineEdit::Normal);

        BOBUIest::newRow(QString(inputMode + "range [0,100] valid '12'").toLatin1())
            << 0
            << 100
            << QString("12")
            << QString("12")
            << bool(useKeys)
            << bool(true)
            << uint(QLineEdit::Normal);

        BOBUIest::newRow(QString(inputMode + "range [-100,100] valid '-12'").toLatin1())
            << -100
            << 100
            << QString("-12")
            << QString("-12")
            << bool(useKeys)
            << bool(true)
            << uint(QLineEdit::Normal);

        // invalid data
        // characters not allowed in QIntValidator
        BOBUIest::newRow(QString(inputMode + "range [0,9] inv 'a-a'").toLatin1())
            << 0
            << 9
            << QString("a")
            << QString("")
            << bool(useKeys)
            << bool(false)
            << uint(QLineEdit::Normal);

        BOBUIest::newRow(QString(inputMode + "range [0,9] inv 'A'").toLatin1())
            << 0
            << 9
            << QString("A")
            << QString("")
            << bool(useKeys)
            << bool(false)
            << uint(QLineEdit::Normal);
        // minus sign only allowed with a range on the negative side
        BOBUIest::newRow(QString(inputMode + "range [0,100] inv '-'").toLatin1())
            << 0
            << 100
            << QString("-")
            << QString("")
            << bool(useKeys)
            << bool(false)
            << uint(QLineEdit::Normal);
        BOBUIest::newRow(QString(inputMode + "range [0,100] int '153'").toLatin1())
            << 0
            << 100
            << QString("153")
            << QString("153")
            << bool(useKeys)
            << bool(false)
            << uint(QLineEdit::Normal);
        BOBUIest::newRow(QString(inputMode + "range [-100,100] int '-153'").toLatin1())
            << -100
            << 100
            << QString("-153")
            << QString(useKeys ? "-15" : "")
            << bool(useKeys)
            << bool(useKeys ? true : false)
            << uint(QLineEdit::Normal);
        BOBUIest::newRow(QString(inputMode + "range [3,7] int '2'").toLatin1())
            << 3
            << 7
            << QString("2")
            << QString("2")
            << bool(useKeys)
            << bool(false)
            << uint(QLineEdit::Normal);
        BOBUIest::newRow(QString(inputMode + "range [3,7] int '8'").toLatin1())
            << 3
            << 7
            << QString("")
            << QString("")
            << bool(useKeys)
            << bool(false)
            << uint(QLineEdit::Normal);
        BOBUIest::newRow(QString(inputMode + "range [0,99] inv 'a-a'").toLatin1())
            << 0
            << 99
            << QString("19a")
            << QString(useKeys ? "19" : "")
            << bool(useKeys)
            << bool(useKeys ? true : false)
            << uint(QLineEdit::Password);
    }
}

void tst_QLineEdit::setValidator_QIntValidator()
{
    QFETCH(int, mini);
    QFETCH(int, maxi);
    QFETCH(QString, input);
    QFETCH(QString, expectedText);
    QFETCH(bool, useKeys);
    QFETCH(bool, is_valid);
    QFETCH(uint, echoMode);

    QIntValidator intValidator(mini, maxi, 0);
    QLineEdit *testWidget = ensureTestWidget();
    testWidget->setEchoMode((QLineEdit::EchoMode)echoMode);
    testWidget->setValidator(&intValidator);
    QVERIFY(testWidget->text().isEmpty());
//qDebug("1 input: '" + input + "' Exp: '" + expectedText + "'");

    // tests valid input
    if (!useKeys) {
        testWidget->insert(input);
    } else {
        BOBUIest::keyClicks(testWidget, input);
        return_count = 0;
        BOBUIest::keyClick(testWidget, BobUI::Key_Return);
        QCOMPARE(return_count, int(is_valid)); // assuming that is_valid = true equals 1
    }
//qDebug("2 input: '" + input + "' Exp: '" + expectedText + "'");
//    QCOMPARE(testWidget->displayText(), expectedText);
    QCOMPARE(testWidget->text(), expectedText);
}

#define NO_PIXMAP_TESTS

void tst_QLineEdit::frame_data()
{
#ifndef NO_PIXMAP_TESTS
#if defined Q_OS_WIN
    BOBUIest::addColumn<QPixmap>("noFrame");
    BOBUIest::addColumn<QPixmap>("useFrame");

    BOBUIest::newRow("win");
//#else
//    BOBUIest::newRow("x11");
#endif
#endif
}

void tst_QLineEdit::frame()
{
    QLineEdit *testWidget = ensureTestWidget();
    testWidget->setFrame(false);
    // verify that the editor is shown without a frame
#ifndef NO_PIXMAP_TESTS
#if defined Q_OS_WIN
    BOBUIEST(testWidget, "noFrame");
#endif
#endif
    QVERIFY(!testWidget->hasFrame());

    testWidget->setFrame(true);
    // verify that the editor is shown with a frame
#ifndef NO_PIXMAP_TESTS
#if defined Q_OS_WIN
    BOBUIEST(testWidget, "useFrame");
#endif
#endif
    QVERIFY(testWidget->hasFrame());
}

void tst_QLineEdit::setAlignment_data()
{
#ifndef NO_PIXMAP_TESTS
#if defined Q_OS_WIN
    BOBUIest::addColumn<QPixmap>("left");
    BOBUIest::addColumn<QPixmap>("right");
    BOBUIest::addColumn<QPixmap>("hcenter");
    BOBUIest::addColumn<QPixmap>("auto");

    BOBUIest::newRow("win");
//#else
//    BOBUIest::newRow("x11");
#endif
#endif
}

void tst_QLineEdit::setAlignment()
{
    QLineEdit *testWidget = ensureTestWidget();
    testWidget->setText("left");
    testWidget->setAlignment(BobUI::AlignLeft);
#ifndef NO_PIXMAP_TESTS
#if defined Q_OS_WIN
    BOBUIEST(testWidget, "left");
#endif
#endif
    QCOMPARE(testWidget->alignment(), BobUI::AlignLeft);

    testWidget->setText("hcenter");
    testWidget->setAlignment(BobUI::AlignHCenter);
#ifndef NO_PIXMAP_TESTS
#if defined Q_OS_WIN
    BOBUIEST(testWidget, "hcenter");
#endif
#endif
    QCOMPARE(testWidget->alignment(), BobUI::AlignHCenter);

    testWidget->setText("right");
    testWidget->setAlignment(BobUI::AlignRight);
#ifndef NO_PIXMAP_TESTS
#if defined Q_OS_WIN
    BOBUIEST(testWidget, "right");
#endif
#endif
    QCOMPARE(testWidget->alignment(), BobUI::AlignRight);

    testWidget->setAlignment(BobUI::AlignTop);
    QCOMPARE(testWidget->alignment(), BobUI::AlignTop);

    testWidget->setAlignment(BobUI::AlignBottom);
    QCOMPARE(testWidget->alignment(), BobUI::AlignBottom);

    testWidget->setAlignment(BobUI::AlignCenter);
    QCOMPARE(testWidget->alignment(), BobUI::AlignCenter);
}

void tst_QLineEdit::isModified()
{
    QLineEdit *testWidget = ensureTestWidget();
    QVERIFY(!testWidget->isModified());
    testWidget->setText("bla");
    QVERIFY(!testWidget->isModified());

    psKeyClick(testWidget, BobUI::Key_Home);
    QVERIFY(!testWidget->isModified());
    BOBUIest::keyClick(testWidget, BobUI::Key_Right);
    QVERIFY(!testWidget->isModified());
    BOBUIest::keyClick(testWidget, BobUI::Key_Right);
    QVERIFY(!testWidget->isModified());
    BOBUIest::keyClick(testWidget, BobUI::Key_Right);
    QVERIFY(!testWidget->isModified());
    BOBUIest::keyClick(testWidget, BobUI::Key_Left);
    QVERIFY(!testWidget->isModified());
    psKeyClick(testWidget, BobUI::Key_End);
    QVERIFY(!testWidget->isModified());

    BOBUIest::keyClicks(testWidget, "T");
    QVERIFY(testWidget->isModified());
    BOBUIest::keyClicks(testWidget, "his is a string");
    QVERIFY(testWidget->isModified());

    testWidget->setText("");
    QVERIFY(!testWidget->isModified());
    testWidget->setText("foo");
    QVERIFY(!testWidget->isModified());
}

/*
    Obsolete function but as long as we provide it, it needs to work.
*/

void tst_QLineEdit::edited()
{
    QLineEdit *testWidget = ensureTestWidget();
    QVERIFY(!testWidget->isModified());
    testWidget->setText("bla");
    QVERIFY(!testWidget->isModified());

    psKeyClick(testWidget, BobUI::Key_Home);
    QVERIFY(!testWidget->isModified());
    BOBUIest::keyClick(testWidget, BobUI::Key_Right);
    QVERIFY(!testWidget->isModified());
    BOBUIest::keyClick(testWidget, BobUI::Key_Right);
    QVERIFY(!testWidget->isModified());
    BOBUIest::keyClick(testWidget, BobUI::Key_Right);
    QVERIFY(!testWidget->isModified());
    BOBUIest::keyClick(testWidget, BobUI::Key_Left);
    QVERIFY(!testWidget->isModified());
    psKeyClick(testWidget, BobUI::Key_End);
    QVERIFY(!testWidget->isModified());

    BOBUIest::keyClicks(testWidget, "T");
    QVERIFY(testWidget->isModified());
    BOBUIest::keyClicks(testWidget, "his is a string");
    QVERIFY(testWidget->isModified());

    testWidget->setModified(false);
    QVERIFY(!testWidget->isModified());

    testWidget->setModified(true);
    QVERIFY(testWidget->isModified());
}

void tst_QLineEdit::fixupDoesNotModify_BOBUIBUG_49295()
{
    QLineEdit *testWidget = ensureTestWidget();

    ValidatorWithFixup val;
    testWidget->setValidator(&val);
    testWidget->setText("foo");
    QVERIFY(!testWidget->isModified());
    QVERIFY(!testWidget->hasAcceptableInput());

    BOBUIest::keyClicks(testWidget, QStringLiteral("bar"));
    QVERIFY(testWidget->isModified());
    QVERIFY(!testWidget->hasAcceptableInput());

    // trigger a fixup, which should not reset the modified flag
    QFocusEvent lostFocus(QEvent::FocusOut);
    qApp->sendEvent(testWidget, &lostFocus);

    QVERIFY(testWidget->hasAcceptableInput());
    QEXPECT_FAIL("", "BOBUIBUG-49295: a fixup of a line edit should keep it modified", Continue);
    QVERIFY(testWidget->isModified());
}

void tst_QLineEdit::insert()
{
    QLineEdit *testWidget = ensureTestWidget();
    testWidget->insert("This");
    testWidget->insert(" is");
    testWidget->insert(" a");
    testWidget->insert(" test");

    QCOMPARE(testWidget->text(), QString("This is a test"));

    testWidget->cursorWordBackward(false);
    testWidget->cursorBackward(false, 1);
    testWidget->insert(" nice");
    QCOMPARE(testWidget->text(), QString("This is a nice test"));

    testWidget->setCursorPosition(-1);
    testWidget->insert("No Crash! ");
    QCOMPARE(testWidget->text(), QString("No Crash! This is a nice test"));
}

static inline QByteArray selectionTestName(int start, int length)
{
    return "selection start: " + QByteArray::number(start) + " length: " + QByteArray::number(length);
}

void tst_QLineEdit::setSelection_data()
{
    BOBUIest::addColumn<QString>("text");
    BOBUIest::addColumn<int>("start");
    BOBUIest::addColumn<int>("length");
    BOBUIest::addColumn<int>("expectedCursor");
    BOBUIest::addColumn<QString>("expectedText");
    BOBUIest::addColumn<bool>("expectedHasSelectedText");

    QString text = "Abc defg hijklmno, p 'qrst' uvw xyz";
    int start, length, pos;

    start = 0; length = 1; pos = 1;
    BOBUIest::newRow(selectionTestName(start, length).constData())
        << text << start << length << pos << QString("A") << true;

    start = 0; length = 2; pos = 2;
    BOBUIest::newRow(selectionTestName(start, length).constData())
        << text << start << length << pos << QString("Ab") << true;

    start = 0; length = 4; pos = 4;
    BOBUIest::newRow(selectionTestName(start, length).constData())
        << text << start << length << pos << QString("Abc ") << true;

    start = -1; length = 0; pos = text.size();
    BOBUIest::newRow(selectionTestName(start, length).constData())
        << text << start << length << pos << QString() << false;

    start = 34; length = 1; pos = 35;
    BOBUIest::newRow(selectionTestName(start, length).constData())
        << text << start << length << pos << QString("z") << true;

    start = 34; length = 2; pos = 35;
    BOBUIest::newRow(selectionTestName(start, length).constData())
        << text << start << length << pos << QString("z") << true;

    start = 34; length = -1; pos = 33;
    BOBUIest::newRow(selectionTestName(start, length).constData())
        << text << start << length << pos << QString("y") << true;

    start = 1; length = -2; pos = 0;
    BOBUIest::newRow(selectionTestName(start, length).constData())
        << text << start << length << pos << QString("A") << true;

    start = -1; length = -1; pos = text.size();
    BOBUIest::newRow(selectionTestName(start, length).constData())
        << text << start << length << pos << QString() << false;
}


void tst_QLineEdit::setSelection()
{
    QFETCH(QString, text);
    QFETCH(int, start);
    QFETCH(int, length);
    QFETCH(int, expectedCursor);
    QFETCH(QString, expectedText);
    QFETCH(bool, expectedHasSelectedText);

    QLineEdit *testWidget = ensureTestWidget();
    testWidget->setText(text);
    testWidget->setSelection(start, length);
    QCOMPARE(testWidget->hasSelectedText(), expectedHasSelectedText);
    QCOMPARE(testWidget->selectedText(), expectedText);
    if (expectedCursor >= 0)
        QCOMPARE(testWidget->cursorPosition(), expectedCursor);
}

#if BOBUI_CONFIG(clipboard) && BOBUI_CONFIG(shortcut)
void tst_QLineEdit::cut()
{
    if (QGuiApplication::platformName().startsWith(QLatin1String("wayland"), BobUI::CaseInsensitive))
        QSKIP("Wayland: This fails. Figure out why.");

    if (!PlatformClipboard::isAvailable())
        QSKIP("Autotests run from cron and pasteboard don't get along quite ATM");

    // test newlines in cut'n'paste
    QLineEdit *testWidget = ensureTestWidget();
    testWidget->setText("A\nB\nC\n");
    testWidget->setSelection(0, 6);
    testWidget->cut();
    psKeyClick(testWidget, BobUI::Key_Home);
    testWidget->paste();
    QCOMPARE(testWidget->text(), QString("A\nB\nC\n"));
    //                              1         2         3         4
    //                    01234567890123456789012345678901234567890
    testWidget->setText("Abc defg hijklmno");

    testWidget->setSelection(0, 3);
    testWidget->cut();
    QCOMPARE(testWidget->text(), QString(" defg hijklmno"));

    psKeyClick(testWidget, BobUI::Key_End);
    testWidget->paste();
    QCOMPARE(testWidget->text(), QString(" defg hijklmnoAbc"));

    psKeyClick(testWidget, BobUI::Key_Home);
    testWidget->del();
    QCOMPARE(testWidget->text(), QString("defg hijklmnoAbc"));

    testWidget->setSelection(0, 4);
    testWidget->copy();
    psKeyClick(testWidget, BobUI::Key_End);
    testWidget->paste();
    QCOMPARE(testWidget->text(), QString("defg hijklmnoAbcdefg"));

    BOBUIest::keyClick(testWidget, BobUI::Key_Left);
    BOBUIest::keyClick(testWidget, BobUI::Key_Left);
    BOBUIest::keyClick(testWidget, BobUI::Key_Left);
    BOBUIest::keyClick(testWidget, BobUI::Key_Left);
    BOBUIest::keyClick(testWidget, BobUI::Key_Left);
    BOBUIest::keyClick(testWidget, BobUI::Key_Left);
    BOBUIest::keyClick(testWidget, BobUI::Key_Left);
    BOBUIest::keyClick(testWidget, ' ');
    QCOMPARE(testWidget->text(), QString("defg hijklmno Abcdefg"));

    testWidget->setSelection(0, 5);
    testWidget->del();
    QCOMPARE(testWidget->text(), QString("hijklmno Abcdefg"));

    testWidget->end(false);
    BOBUIest::keyClick(testWidget, ' ');
    testWidget->paste();
    QCOMPARE(testWidget->text(), QString("hijklmno Abcdefg defg"));

    testWidget->home(false);
    testWidget->cursorWordForward(true);
    testWidget->cut();
    testWidget->end(false);
    BOBUIest::keyClick(testWidget, ' ');
    testWidget->paste();
    testWidget->cursorBackward(true, 1);
    testWidget->cut();
    QCOMPARE(testWidget->text(), QString("Abcdefg defg hijklmno"));
}

void tst_QLineEdit::cutWithoutSelection()
{
    if (QGuiApplication::platformName().startsWith(QLatin1String("wayland"), BobUI::CaseInsensitive))
        QSKIP("Wayland: This fails. Figure out why.");

    enum { selectionLength = 1 };

    if (QKeySequence(QKeySequence::Cut).toString() != QLatin1String("Ctrl+X"))
        QSKIP("Platform with non-standard keybindings");
    QClipboard *clipboard = QGuiApplication::clipboard();
    if (!PlatformClipboard::isAvailable()
        || !QGuiApplication::platformName().compare("xcb", BobUI::CaseInsensitive)) { // Avoid unstable X11 clipboard
        clipboard = nullptr;
    }

    if (clipboard)
        clipboard->clear();
    const QString origText = QStringLiteral("test");
    QLineEdit lineEdit(origText);
    lineEdit.setCursorPosition(0);
    QVERIFY(!lineEdit.hasSelectedText());
    BOBUIest::keyClick(&lineEdit, BobUI::Key_X, BobUI::ControlModifier);
    QCOMPARE(lineEdit.text(), origText); // No selection, unmodified.
    if (clipboard)
        QVERIFY(clipboard->text().isEmpty());
    lineEdit.setSelection(0, selectionLength);
    BOBUIest::keyClick(&lineEdit, BobUI::Key_X, BobUI::ControlModifier);
    QCOMPARE(lineEdit.text(), origText.right(origText.size() - selectionLength));
    if (clipboard)
        QCOMPARE(clipboard->text(), origText.left(selectionLength));
}

#endif // BOBUI_CONFIG(clipboard) && BOBUI_CONFIG(shortcut)

class InputMaskValidator : public QValidator
{
public:
    InputMaskValidator(QObject *parent, const char *name = nullptr) : QValidator(parent) { setObjectName(name); }
    State validate(QString &text, int &pos) const override
    {
        InputMaskValidator *that = (InputMaskValidator *)this;
        that->validateText = text;
        that->validatePos = pos;
        return Acceptable;
    }
    QString validateText;
    int validatePos;
};

void tst_QLineEdit::inputMaskAndValidator_data()
{
    BOBUIest::addColumn<QString>("inputMask");
    BOBUIest::addColumn<BOBUIestEventList>("keys");
    BOBUIest::addColumn<QString>("validateText");
    BOBUIest::addColumn<int>("validatePos");

    BOBUIestEventList inputKeys;
    inputKeys.addKeyClick(BobUI::Key_1);
    inputKeys.addKeyClick(BobUI::Key_2);

    BOBUIest::newRow("task28291") << "000;_" << inputKeys << "12_" << 2;
}

void tst_QLineEdit::inputMaskAndValidator()
{
    QFETCH(QString, inputMask);
    QFETCH(BOBUIestEventList, keys);
    QFETCH(QString, validateText);
    QFETCH(int, validatePos);

    QLineEdit *testWidget = ensureTestWidget();
    InputMaskValidator imv(testWidget);
    testWidget->setValidator(&imv);

    testWidget->setInputMask(inputMask);
    keys.simulate(testWidget);

    QCOMPARE(imv.validateText, validateText);
    QCOMPARE(imv.validatePos, validatePos);
}

void tst_QLineEdit::maxLengthAndInputMask()
{
    QLineEdit *testWidget = ensureTestWidget();
    QVERIFY(testWidget->inputMask().isNull());
    testWidget->setMaxLength(10);
    QCOMPARE(testWidget->maxLength(), 10);
    testWidget->setInputMask(QString());
    QVERIFY(testWidget->inputMask().isNull());
    QCOMPARE(testWidget->maxLength(), 10);

    testWidget->setInputMask("XXXX");
    QCOMPARE(testWidget->maxLength(), 4);

    testWidget->setMaxLength(15);
    QCOMPARE(testWidget->maxLength(), 4);

    // 8 \ => raw string with 4 \ => input mask with 2 \ => maxLength = 2
    testWidget->setInputMask("\\\\\\\\");
    QCOMPARE(testWidget->maxLength(), 2);
}


class LineEdit : public QLineEdit
{
public:
    LineEdit() { state = Other; }

    void keyPressEvent(QKeyEvent *e) override
    {
        QLineEdit::keyPressEvent(e);
        if (e->key() == BobUI::Key_Enter) {
            state = e->isAccepted() ? Accepted : Ignored;
        } else {
            state = Other;
        }

    }
    enum State {
        Accepted,
        Ignored,
        Other
    };

    State state;

    friend class tst_QLineEdit;
};

Q_DECLARE_METATYPE(LineEdit::State);
void tst_QLineEdit::returnPressedKeyEvent()
{
    LineEdit lineedit;
    centerOnScreen(&lineedit);
    lineedit.show();
    QCOMPARE((int)lineedit.state, (int)LineEdit::Other);
    BOBUIest::keyClick(&lineedit, BobUI::Key_Enter);
    QCOMPARE((int)lineedit.state, (int)LineEdit::Ignored);
    connect(&lineedit, SIGNAL(returnPressed()), this, SLOT(onReturnPressed()));
    BOBUIest::keyClick(&lineedit, BobUI::Key_Enter);
    QCOMPARE((int)lineedit.state, (int)LineEdit::Ignored);
    disconnect(&lineedit, SIGNAL(returnPressed()), this, SLOT(onReturnPressed()));
    BOBUIest::keyClick(&lineedit, BobUI::Key_Enter);
    QCOMPARE((int)lineedit.state, (int)LineEdit::Ignored);
    BOBUIest::keyClick(&lineedit, BobUI::Key_1);
    QCOMPARE((int)lineedit.state, (int)LineEdit::Other);
}

void tst_QLineEdit::keepSelectionOnTabFocusIn()
{
    QLineEdit *testWidget = ensureTestWidget();
    testWidget->setText("hello world");
    {
        QFocusEvent e(QEvent::FocusIn, BobUI::TabFocusReason);
        QApplication::sendEvent(testWidget, &e);
    }
    QCOMPARE(testWidget->selectedText(), QString("hello world"));
    testWidget->setSelection(0, 5);
    QCOMPARE(testWidget->selectedText(), QString("hello"));
    {
        QFocusEvent e(QEvent::FocusIn, BobUI::TabFocusReason);
        QApplication::sendEvent(testWidget, &e);
    }
    QCOMPARE(testWidget->selectedText(), QString("hello"));
}

void tst_QLineEdit::readOnlyStyleOption()
{
    QLineEdit *testWidget = ensureTestWidget();
    bool wasReadOnly = testWidget->isReadOnly();
    QStyle *oldStyle = testWidget->style();
    testWidget->show();
    BOBUIRY_VERIFY(BOBUIest::qWaitForWindowExposed(testWidget));

    StyleOptionTestStyle myStyle;
    testWidget->setStyle(&myStyle);

    myStyle.setReadOnly(true);
    testWidget->setReadOnly(true);
    testWidget->update();
    BOBUIRY_VERIFY(myStyle.wasDrawn);
    myStyle.wasDrawn = false;

    testWidget->setReadOnly(false);
    myStyle.setReadOnly(false);
    testWidget->update();
    BOBUIRY_VERIFY(myStyle.wasDrawn);

    testWidget->setReadOnly(wasReadOnly);
    testWidget->setStyle(oldStyle);
}

void tst_QLineEdit::validateOnFocusOut()
{
    QLineEdit *testWidget = ensureTestWidget();
    QSignalSpy editingFinishedSpy(testWidget, SIGNAL(editingFinished()));
    testWidget->setValidator(new QIntValidator(100, 999, testWidget));
    BOBUIest::keyPress(testWidget, '1');
    BOBUIest::keyPress(testWidget, '0');
    QCOMPARE(testWidget->text(), QString("10"));
    testWidget->clearFocus();
    QCOMPARE(editingFinishedSpy.size(), 0);

    testWidget->setFocus();
    centerOnScreen(testWidget);
    testWidget->show();
    testWidget->activateWindow();
    QVERIFY(BOBUIest::qWaitForWindowFocused(testWidget));
    QVERIFY(testWidget->hasFocus());

    BOBUIest::keyPress(testWidget, '0');
    BOBUIRY_COMPARE(testWidget->text(), QString("100"));

    testWidget->clearFocus();
    QCOMPARE(editingFinishedSpy.size(), 1);
}

void tst_QLineEdit::editInvalidText()
{
    QLineEdit *testWidget = ensureTestWidget();
    testWidget->clear();
    testWidget->setValidator(new QIntValidator(0, 12, testWidget));
    testWidget->setText("1234");

    QVERIFY(!testWidget->hasAcceptableInput());
    BOBUIest::keyPress(testWidget, BobUI::Key_Backspace);
    BOBUIest::keyPress(testWidget, BobUI::Key_Backspace);
    BOBUIest::keyPress(testWidget, BobUI::Key_A);
    BOBUIest::keyPress(testWidget, BobUI::Key_B);
    BOBUIest::keyPress(testWidget, BobUI::Key_C);
    BOBUIest::keyPress(testWidget, BobUI::Key_1);
    QVERIFY(testWidget->hasAcceptableInput());
    QCOMPARE(testWidget->text(), QString("12"));
    testWidget->cursorBackward(false);
    testWidget->cursorBackward(true, 2);
    BOBUIest::keyPress(testWidget, BobUI::Key_Delete);
    QVERIFY(testWidget->hasAcceptableInput());
    QCOMPARE(testWidget->text(), QString("2"));
    BOBUIest::keyPress(testWidget, BobUI::Key_1);
    QVERIFY(testWidget->hasAcceptableInput());
    QCOMPARE(testWidget->text(), QString("12"));

    testWidget->setValidator(0);
}

Q_DECLARE_METATYPE(BobUI::KeyboardModifiers)

void tst_QLineEdit::charWithAltOrCtrlModifier_data()
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

void tst_QLineEdit::charWithAltOrCtrlModifier()
{
    QFETCH(BobUI::KeyboardModifiers, modifiers);
    QFETCH(bool, textExpected);

    QLineEdit *testWidget = ensureTestWidget();
    testWidget->clear();
    QVERIFY(testWidget->text().isEmpty());

    BOBUIest::keyPress(testWidget, BobUI::Key_Plus, modifiers);
    const QString expectedText = textExpected ?  QLatin1String("+") : QString();
    QCOMPARE(testWidget->text(), expectedText);
}

void tst_QLineEdit::leftKeyOnSelectedText()
{
    QLineEdit *testWidget = ensureTestWidget();
    testWidget->clear();
    testWidget->setText("0123");
    testWidget->setCursorPosition(4);
    BOBUIest::keyClick(testWidget, BobUI::Key_Left, BobUI::ShiftModifier);
    QCOMPARE(testWidget->cursorPosition(), 3);
    QCOMPARE(testWidget->selectedText(), QString("3"));
    BOBUIest::keyClick(testWidget, BobUI::Key_Left, BobUI::ShiftModifier);
    QCOMPARE(testWidget->cursorPosition(), 2);
    QCOMPARE(testWidget->selectedText(), QString("23"));
    BOBUIest::keyClick(testWidget, BobUI::Key_Left);

    if (unselectingWithLeftOrRightChangesCursorPosition())
        QCOMPARE(testWidget->cursorPosition(), 1);
    else
        QCOMPARE(testWidget->cursorPosition(), 2);
}

void tst_QLineEdit::inlineCompletion()
{
#ifdef Q_OS_ANDROID
    QSKIP("QCompleter does not work on Android, see BOBUIBUG-77174");
#endif
    if (QGuiApplication::platformName().startsWith(QLatin1String("wayland"), BobUI::CaseInsensitive))
        QSKIP("Wayland: This fails. Figure out why.");

    QLineEdit *testWidget = ensureTestWidget();
    testWidget->clear();
    QStandardItemModel *model = new QStandardItemModel;
    QStandardItem *root = model->invisibleRootItem();
    QStandardItem *items[5];
    for (int i = 0; i < 5; i++) {
        items[i] = new QStandardItem(QLatin1String("item") + QString::number(i));
        if ((i+2)%2 == 0) { // disable 0,2,4
            items[i]->setFlags(items[i]->flags() & ~BobUI::ItemIsEnabled);
        }
        root->appendRow(items[i]);
    }
    QCompleter *completer = new QCompleter(model);
    completer->setCompletionMode(QCompleter::InlineCompletion);
    completer->setCaseSensitivity(BobUI::CaseInsensitive);
    centerOnScreen(testWidget);
    testWidget->show();
    QVERIFY(BOBUIest::qWaitForWindowExposed(testWidget));
    testWidget->setFocus();
    BOBUIRY_COMPARE(qApp->activeWindow(), (QWidget*)testWidget);
    testWidget->setCompleter(completer);

    // sanity
    BOBUIest::keyClick(testWidget, BobUI::Key_X);
    QCOMPARE(testWidget->selectedText(), QString());
    QCOMPARE(testWidget->text(), QString("x"));
    BOBUIest::keyClick(testWidget, BobUI::Key_Down, BobUI::ControlModifier);
    QCOMPARE(testWidget->selectedText(), QString());
    QCOMPARE(testWidget->text(), QString("x"));
    BOBUIest::keyClick(testWidget, BobUI::Key_Up, BobUI::ControlModifier);
    QCOMPARE(testWidget->selectedText(), QString());
    QCOMPARE(testWidget->text(), QString("x"));

    testWidget->clear();
    BOBUIest::keyClick(testWidget, BobUI::Key_I);
    QCOMPARE(testWidget->selectedText(), QString("tem1"));

    BobUI::KeyboardModifiers keyboardModifiers = BobUI::ControlModifier;
#ifdef Q_OS_MAC
    keyboardModifiers |= BobUI::AltModifier;
#endif
    BOBUIest::keyClick(testWidget, BobUI::Key_Down, keyboardModifiers);
    QCOMPARE(testWidget->selectedText(), QString("tem3"));

    // wraps around (Default)
    BOBUIest::keyClick(testWidget, BobUI::Key_Down, keyboardModifiers);
    QCOMPARE(testWidget->selectedText(), QString("tem1"));

    BOBUIest::keyClick(testWidget, BobUI::Key_Up, keyboardModifiers);
    QCOMPARE(testWidget->selectedText(), QString("tem3"));

    // should not wrap
    completer->setWrapAround(false);
    BOBUIest::keyClick(testWidget, BobUI::Key_Down, keyboardModifiers);
    QCOMPARE(testWidget->selectedText(), QString("tem3"));
    BOBUIest::keyClick(testWidget, BobUI::Key_Up, keyboardModifiers); // item1
    BOBUIest::keyClick(testWidget, BobUI::Key_Up, keyboardModifiers); // item1
    QCOMPARE(testWidget->selectedText(), QString("tem1"));

    // trivia :)
    root->appendRow(new QStandardItem("item11"));
    root->appendRow(new QStandardItem("item12"));
    testWidget->clear();
    BOBUIest::keyClick(testWidget, BobUI::Key_I);
    QCOMPARE(testWidget->selectedText(), QString("tem1"));
    BOBUIest::keyClick(testWidget, BobUI::Key_Delete);
    QCOMPARE(testWidget->selectedText(), QString());
    BOBUIest::keyClick(testWidget, BobUI::Key_Down, keyboardModifiers);
    QCOMPARE(testWidget->selectedText(), QString("tem1")); // neato
    testWidget->setText("item1");
    testWidget->setSelection(1, 2);
    BOBUIest::keyClick(testWidget, BobUI::Key_Down, keyboardModifiers);
    testWidget->end(false);
    QCOMPARE(testWidget->text(), QString("item1")); // no effect for selection in "middle"
    BOBUIest::keyClick(testWidget, BobUI::Key_Down, keyboardModifiers); // item1
    BOBUIest::keyClick(testWidget, BobUI::Key_Down, keyboardModifiers); // item11
    QCOMPARE(testWidget->text(), QString("item11"));

    delete model;
    delete completer;
}

void tst_QLineEdit::noTextEditedOnClear()
{
    QLineEdit *testWidget = ensureTestWidget();
    testWidget->setText("Test");
    QSignalSpy textEditedSpy(testWidget, SIGNAL(textEdited(QString)));
    testWidget->clear();
    QCOMPARE(textEditedSpy.size(), 0);
}

void tst_QLineEdit::textMargin_data()
{
    BOBUIest::addColumn<int>("left");
    BOBUIest::addColumn<int>("top");
    BOBUIest::addColumn<int>("right");
    BOBUIest::addColumn<int>("bottom");

    BOBUIest::addColumn<QPoint>("mousePressPos");
    BOBUIest::addColumn<int>("cursorPosition");

    QLineEdit testWidget;
    QFontMetrics metrics(testWidget.font());
    const QString s = QLatin1String("MMM MMM MMM");
    const int windows11StyleHorizontalOffset = qApp->style()->inherits("QWindows11Style") ? 8 : 0;

    // Different styles generate different offsets, so
    // calculate the width rather than hardcode it.
    const int pixelWidthOfM = windows11StyleHorizontalOffset + metrics.horizontalAdvance(s, 1);
    const int pixelWidthOfMMM_MM = windows11StyleHorizontalOffset + metrics.horizontalAdvance(s, 6);

    BOBUIest::newRow("default-0") << 0 << 0 << 0 << 0 << QPoint(pixelWidthOfMMM_MM, 0) << 6;
    BOBUIest::newRow("default-1") << 0 << 0 << 0 << 0 << QPoint(1, 1) << 0;
    BOBUIest::newRow("default-2") << -1 << 0 << -1 << 0 << QPoint(pixelWidthOfMMM_MM, 0) << 6;
    BOBUIest::newRow("default-3") << 0 << 0 << 0 << 0 << QPoint(pixelWidthOfM, 1) << 1;

    BOBUIest::newRow("hor-0") << 10 << 0 << 10 << 0 << QPoint(1, 1) << 0;
    BOBUIest::newRow("hor-1") << 10 << 0 << 10 << 0 << QPoint(10, 1) << 0;
    BOBUIest::newRow("hor-2") << 20 << 0 << 10 << 0 << QPoint(20, 1) << 0;

    if (!qApp->style()->inherits("QMacStyle")) { //MacStyle doesn't support verticals margins.
        BOBUIest::newRow("default-2-ver") << -1 << -1 << -1 << -1 << QPoint(pixelWidthOfMMM_MM, 0) << 6;
        BOBUIest::newRow("ver") << 0 << 10 << 0 << 10 << QPoint(1, 1) << 0;
    }
}

void tst_QLineEdit::textMargin()
{
    QFETCH(int, left);
    QFETCH(int, top);
    QFETCH(int, right);
    QFETCH(int, bottom);
    QFETCH(QPoint, mousePressPos);
    QFETCH(int, cursorPosition);

    // Put the line edit into a toplevel window to avoid
    // resizing by the window system.
    QWidget tlw;
    QLineEdit testWidget(&tlw);
    testWidget.setText("MMM MMM MMM");
    QFontMetrics metrics(testWidget.font());
    const int minimumWidth =  metrics.horizontalAdvance(testWidget.text());
    testWidget.setGeometry(100, 100, qMax(minimumWidth,  100) , 30);
    testWidget.setCursorPosition(6);

    QSize sizeHint = testWidget.sizeHint();
    QSize minSizeHint = testWidget.minimumSizeHint();
    testWidget.setTextMargins(left, top, right, bottom);

    sizeHint.setWidth(sizeHint.width() + left + right);
    sizeHint.setHeight(sizeHint.height() + top +bottom);
    QCOMPARE(testWidget.sizeHint(), sizeHint);

    if (minSizeHint.width() > -1) {
        minSizeHint.setWidth(minSizeHint.width() + left + right);
        minSizeHint.setHeight(minSizeHint.height() + top + bottom);
        QCOMPARE(testWidget.minimumSizeHint(), minSizeHint);
    }


    testWidget.setFrame(false);
    centerOnScreen(&tlw);
    tlw.show();

    const QMargins margins = testWidget.textMargins();
    QCOMPARE(left, margins.left());
    QCOMPARE(top, margins.top());
    QCOMPARE(right, margins.right());
    QCOMPARE(bottom, margins.bottom());

    BOBUIest::mouseClick(&testWidget, BobUI::LeftButton, {}, mousePressPos);
    BOBUIRY_COMPARE(testWidget.cursorPosition(), cursorPosition);
}

void tst_QLineEdit::returnKeyClearsEditedFlag()
{
    /* Tests that pressing enter within the line edit correctly clears
       the "edited" flag, preventing a redundant emission of
       editingFinished() when its focus is dropped after no further
       edits */
    QLineEdit testWidget;
    QSignalSpy leSpy(&testWidget, &QLineEdit::editingFinished);
    QVERIFY(leSpy.isValid());

    // Prepare widget for testing
    testWidget.setFocus();
    centerOnScreen(&testWidget);
    testWidget.show();
    testWidget.raise();
    QVERIFY(BOBUIest::qWaitForWindowExposed(&testWidget));
    BOBUIRY_VERIFY(testWidget.hasFocus());

    // Focus drop with no edits shouldn't emit signal, edited flag == false
    testWidget.clearFocus(); // Signal not emitted
    QVERIFY(!testWidget.hasFocus());
    QCOMPARE(leSpy.size(), 0);

    // Focus drop after edits should emit signal, edited flag == true
    testWidget.setFocus();
    BOBUIRY_VERIFY(testWidget.hasFocus());
    BOBUIest::keyClicks(&testWidget, "edit1 "); // edited flag set
    testWidget.clearFocus(); // edited flag cleared, signal emitted
    QVERIFY(!testWidget.hasFocus());
    QCOMPARE(leSpy.size(), 1);

    // Only text related keys should set edited flag
    testWidget.setFocus();
    BOBUIRY_VERIFY(testWidget.hasFocus());
    BOBUIest::keyClick(&testWidget, BobUI::Key_Left);
    BOBUIest::keyClick(&testWidget, BobUI::Key_Alt);
    BOBUIest::keyClick(&testWidget, BobUI::Key_PageUp);
    testWidget.clearFocus(); // Signal not emitted
    QVERIFY(!testWidget.hasFocus());
    QCOMPARE(leSpy.size(), 1); // No change

    // Return should always emit signal
    testWidget.setFocus();
    BOBUIRY_VERIFY(testWidget.hasFocus());
    BOBUIest::keyClick(&testWidget, BobUI::Key_Return); /* Without edits,
                                                     signal emitted,
                                                     edited flag cleared */
    QCOMPARE(leSpy.size(), 2);
    BOBUIest::keyClicks(&testWidget, "edit2 "); // edited flag set
    BOBUIest::keyClick(&testWidget, BobUI::Key_Return); /* With edits,
                                                     signal emitted,
                                                     edited flag cleared */
    QCOMPARE(leSpy.size(), 3);

    /* After editing the line edit following a Return key press with a
       focus drop should not emit signal a second time since Return now
       clears the edited flag */
    BOBUIest::keyClicks(&testWidget, "edit3 "); // edited flag set
    BOBUIest::keyClick(&testWidget, BobUI::Key_Return); /* signal emitted,
                                                     edited flag cleared */
    QCOMPARE(leSpy.size(), 4);
    testWidget.clearFocus(); // Signal not emitted since edited == false
    QVERIFY(!testWidget.hasFocus());
    QCOMPARE(leSpy.size(), 4); // No change
}

#ifndef BOBUI_NO_CURSOR
void tst_QLineEdit::cursor()
{
    QLineEdit *testWidget = ensureTestWidget();
    testWidget->setReadOnly(false);
    QCOMPARE(testWidget->cursor().shape(), BobUI::IBeamCursor);
    testWidget->setReadOnly(true);
    QCOMPARE(testWidget->cursor().shape(), BobUI::ArrowCursor);
    testWidget->setReadOnly(false);
    QCOMPARE(testWidget->cursor().shape(), BobUI::IBeamCursor);
}
#endif

class task180999_Widget : public QWidget
{
public:
    task180999_Widget(QWidget *parent = nullptr) : QWidget(parent)
    {
        QHBoxLayout *layout  = new QHBoxLayout(this);
        lineEdit1.setText("some text 1 ...");
        lineEdit2.setText("some text 2 ...");
        layout->addWidget(&lineEdit1);
        layout->addWidget(&lineEdit2);
    }

    QLineEdit lineEdit1;
    QLineEdit lineEdit2;
};

void tst_QLineEdit::task180999_focus()
{
    if (QGuiApplication::platformName().startsWith(QLatin1String("wayland"), BobUI::CaseInsensitive))
        QSKIP("Wayland: This fails. Figure out why.");

    task180999_Widget widget;

    widget.lineEdit1.setFocus();
    widget.show();

    widget.lineEdit2.setFocus();
    widget.lineEdit2.selectAll();
    widget.hide();

    widget.lineEdit1.setFocus();
    widget.show();
    BOBUIest::qWait(200);
    widget.activateWindow();

    BOBUIRY_VERIFY(!widget.lineEdit2.hasSelectedText());
}

void tst_QLineEdit::task174640_editingFinished()
{
    QWidget mw;
    QVBoxLayout *layout = new QVBoxLayout(&mw);
    QLineEdit *le1 = new QLineEdit(&mw);
    QLineEdit *le2 = new QLineEdit(&mw);
    layout->addWidget(le1);
    layout->addWidget(le2);

    mw.show();
    mw.activateWindow();
    QVERIFY(BOBUIest::qWaitForWindowFocused(&mw));

    QSignalSpy editingFinishedSpy(le1, SIGNAL(editingFinished()));

    le1->setFocus();
    BOBUIRY_VERIFY(le1->hasFocus());
    QCOMPARE(editingFinishedSpy.size(), 0);

    le2->setFocus();
    BOBUIRY_VERIFY(le2->hasFocus());
    // editingFinished will not be emitted anew because no editing happened
    QCOMPARE(editingFinishedSpy.size(), 0);

    le1->setFocus();
    BOBUIRY_VERIFY(le1->hasFocus());
    BOBUIest::keyPress(le1, BobUI::Key_Plus);
    le2->setFocus();
    BOBUIRY_VERIFY(le2->hasFocus());
    QCOMPARE(editingFinishedSpy.size(), 1);
    editingFinishedSpy.clear();

    le1->setFocus();
    BOBUIRY_VERIFY(le1->hasFocus());

    QMenu *testMenu1 = new QMenu(le1);
    testMenu1->addAction("foo");
    testMenu1->addAction("bar");
    testMenu1->show();
    QVERIFY(BOBUIest::qWaitForWindowExposed(testMenu1));
    BOBUIest::qWait(20);
    mw.activateWindow();

    delete testMenu1;
    QCOMPARE(editingFinishedSpy.size(), 0);
    BOBUIRY_VERIFY(le1->hasFocus());
    // Ensure le1 has been edited
    BOBUIest::keyPress(le1, BobUI::Key_Plus);

    QMenu *testMenu2 = new QMenu(le2);
    testMenu2->addAction("foo2");
    testMenu2->addAction("bar2");
    testMenu2->show();
    QVERIFY(BOBUIest::qWaitForWindowExposed(testMenu2));
    BOBUIest::qWait(20);
    mw.activateWindow();
    delete testMenu2;
    QCOMPARE(editingFinishedSpy.size(), 1);
}

#if BOBUI_CONFIG(completer)
class task198789_Widget : public QWidget
{
    Q_OBJECT
public:
    task198789_Widget(QWidget *parent = nullptr) : QWidget(parent)
    {
        QStringList wordList;
        wordList << "alpha" << "omega" << "omicron" << "zeta";

        lineEdit = new QLineEdit(this);
        completer = new QCompleter(wordList, this);
        lineEdit->setCompleter(completer);

        connect(lineEdit, SIGNAL(textChanged(QString)), this, SLOT(textChanged(QString)));
    }

    QLineEdit *lineEdit;
    QCompleter *completer;
    QString currentCompletion;

private slots:
    void textChanged(const QString &)
    {
        currentCompletion = completer->currentCompletion();
    }
};

void tst_QLineEdit::task198789_currentCompletion()
{
    task198789_Widget widget;
    widget.show();
    qApp->processEvents();
    BOBUIest::keyPress(widget.lineEdit, 'o');
    BOBUIest::keyPress(widget.lineEdit, 'm');
    BOBUIest::keyPress(widget.lineEdit, 'i');
    QCOMPARE(widget.currentCompletion, QLatin1String("omicron"));
}

void tst_QLineEdit::task210502_caseInsensitiveInlineCompletion()
{
#ifdef Q_OS_ANDROID
    QSKIP("QCompleter does not work on Android, see BOBUIBUG-77174");
#endif

    QString completion("ABCD");
    QStringList completions;
    completions << completion;
    QLineEdit lineEdit;
    QCompleter completer(completions);
    completer.setCaseSensitivity(BobUI::CaseInsensitive);
    completer.setCompletionMode(QCompleter::InlineCompletion);
    lineEdit.setCompleter(&completer);
    lineEdit.show();
    lineEdit.setFocus();
    QVERIFY(BOBUIest::qWaitForWindowFocused(&lineEdit));
    QVERIFY(lineEdit.hasFocus());
    BOBUIest::keyPress(&lineEdit, 'a');
    BOBUIest::keyPress(&lineEdit, BobUI::Key_Return);
    QCOMPARE(lineEdit.text(), completion);
}

#endif // BOBUI_CONFIG(completer)


void tst_QLineEdit::task229938_dontEmitChangedWhenTextIsNotChanged()
{
    QLineEdit lineEdit;
    lineEdit.setMaxLength(5);
    lineEdit.show();
    QVERIFY(BOBUIest::qWaitForWindowExposed(&lineEdit)); // to be safe and avoid failing setFocus with window managers
    lineEdit.setFocus();
    QSignalSpy changedSpy(&lineEdit, SIGNAL(textChanged(QString)));
    BOBUIest::keyPress(&lineEdit, 'a');
    BOBUIest::keyPress(&lineEdit, 'b');
    BOBUIest::keyPress(&lineEdit, 'c');
    BOBUIest::keyPress(&lineEdit, 'd');
    BOBUIest::keyPress(&lineEdit, 'e');
    BOBUIest::keyPress(&lineEdit, 'f');
    QCOMPARE(changedSpy.size(), 5);
}

void tst_QLineEdit::task233101_cursorPosAfterInputMethod_data()
{
    BOBUIest::addColumn<int>("maxLength");
    BOBUIest::addColumn<int>("cursorPos");
    BOBUIest::addColumn<int>("replacementStart");
    BOBUIest::addColumn<int>("replacementLength");
    BOBUIest::addColumn<QString>("commitString");

    BOBUIest::newRow("data1")  << 4 << 4 << 0 << 0 << QString("");
    BOBUIest::newRow("data2")  << 4 << 4 << 0 << 0 << QString("x");
    BOBUIest::newRow("data3")  << 4 << 4 << 0 << 0 << QString("xxxxxxxxxxxxxxxx");
    BOBUIest::newRow("data4")  << 4 << 3 << 0 << 0 << QString("");
    BOBUIest::newRow("data5")  << 4 << 3 << 0 << 0 << QString("x");
    BOBUIest::newRow("data6")  << 4 << 3 << 0 << 0 << QString("xxxxxxxxxxxxxxxx");
    BOBUIest::newRow("data7")  << 4 << 0 << 0 << 0 << QString("");
    BOBUIest::newRow("data8")  << 4 << 0 << 0 << 0 << QString("x");
    BOBUIest::newRow("data9")  << 4 << 0 << 0 << 0 << QString("xxxxxxxxxxxxxxxx");

    BOBUIest::newRow("data10") << 4 << 4 << -4 << 4 << QString("");
    BOBUIest::newRow("data11") << 4 << 4 << -4 << 4 << QString("x");
    BOBUIest::newRow("data12") << 4 << 4 << -4 << 4 << QString("xxxxxxxxxxxxxxxx");
    BOBUIest::newRow("data13") << 4 << 3 << -3 << 4 << QString("");
    BOBUIest::newRow("data14") << 4 << 3 << -3 << 4 << QString("x");
    BOBUIest::newRow("data15") << 4 << 3 << -3 << 4 << QString("xxxxxxxxxxxxxxxx");
    BOBUIest::newRow("data16") << 4 << 0 << 0 << 4 << QString("");
    BOBUIest::newRow("data17") << 4 << 0 << 0 << 4 << QString("x");
    BOBUIest::newRow("data18") << 4 << 0 << 0 << 4 << QString("xxxxxxxxxxxxxxxx");

    BOBUIest::newRow("data19") << 4 << 4 << -4 << 0 << QString("");
    BOBUIest::newRow("data20") << 4 << 4 << -4 << 0 << QString("x");
    BOBUIest::newRow("data21") << 4 << 4 << -4 << 0 << QString("xxxxxxxxxxxxxxxx");
    BOBUIest::newRow("data22") << 4 << 3 << -3 << 0 << QString("");
    BOBUIest::newRow("data23") << 4 << 3 << -3 << 0 << QString("x");
    BOBUIest::newRow("data24") << 4 << 3 << -3 << 0 << QString("xxxxxxxxxxxxxxxx");
}

void tst_QLineEdit::task233101_cursorPosAfterInputMethod()
{
    QFETCH(int, maxLength);
    QFETCH(int, cursorPos);
    QFETCH(int, replacementStart);
    QFETCH(int, replacementLength);
    QFETCH(QString, commitString);

    QLineEdit lineEdit;
    lineEdit.setMaxLength(maxLength);
    lineEdit.insert(QString().fill(QLatin1Char('a'), cursorPos));
    QCOMPARE(lineEdit.cursorPosition(), cursorPos);

    QInputMethodEvent event;
    event.setCommitString(QLatin1String("x"), replacementStart, replacementLength);
    qApp->sendEvent(&lineEdit, &event);
    QVERIFY(lineEdit.cursorPosition() >= 0);
    QVERIFY(lineEdit.cursorPosition() <= lineEdit.text().size());
    QVERIFY(lineEdit.text().size() <= lineEdit.maxLength());
}

void tst_QLineEdit::task241436_passwordEchoOnEditRestoreEchoMode()
{
    QStyleOptionFrame opt;
    QLineEdit *testWidget = ensureTestWidget();
    const int passwordCharacter = testWidget->style()->styleHint(QStyle::SH_LineEdit_PasswordCharacter, &opt, testWidget);
    QVERIFY(passwordCharacter <= 0xFFFF);
    const QChar fillChar(passwordCharacter);

    testWidget->setEchoMode(QLineEdit::PasswordEchoOnEdit);
    testWidget->setFocus();
    centerOnScreen(testWidget);
    testWidget->show();
    QVERIFY(BOBUIest::qWaitForWindowFocused(testWidget));
    QVERIFY(testWidget->hasFocus());

    BOBUIest::keyPress(testWidget, '0');
    QCOMPARE(testWidget->displayText(), QString("0"));
    testWidget->setEchoMode(QLineEdit::Normal);
    testWidget->clearFocus();
    QCOMPARE(testWidget->displayText(), QString("0"));

    testWidget->activateWindow();
    testWidget->setFocus();
    testWidget->setEchoMode(QLineEdit::PasswordEchoOnEdit);
    BOBUIest::keyPress(testWidget, '0');
    QCOMPARE(testWidget->displayText(), QString("0"));
    testWidget->setEchoMode(QLineEdit::PasswordEchoOnEdit);
    QCOMPARE(testWidget->displayText(), QString("0"));
    testWidget->clearFocus();
    QCOMPARE(testWidget->displayText(), QString(fillChar));

    // restore clean state
    testWidget->setEchoMode(QLineEdit::Normal);
}

void tst_QLineEdit::task248948_redoRemovedSelection()
{
    QLineEdit *testWidget = ensureTestWidget();
    testWidget->setText("a");
    testWidget->selectAll();
    BOBUIest::keyPress(testWidget, BobUI::Key_Delete);
    testWidget->undo();
    testWidget->redo();
    BOBUIest::keyPress(testWidget, 'a');
    BOBUIest::keyPress(testWidget, 'b');
    QCOMPARE(testWidget->text(), QLatin1String("ab"));
}

void tst_QLineEdit::taskBOBUIBUG_4401_enterKeyClearsPassword()
{
    QString password("Wanna guess?");

    QLineEdit *testWidget = ensureTestWidget();
    testWidget->setText(password);
    testWidget->setEchoMode(QLineEdit::PasswordEchoOnEdit);
    testWidget->setFocus();
    testWidget->selectAll();
    centerOnScreen(testWidget);
    testWidget->show();
    QVERIFY(BOBUIest::qWaitForWindowFocused(testWidget));
    QVERIFY(testWidget->hasFocus());

    BOBUIest::keyPress(testWidget, BobUI::Key_Enter);
    BOBUIRY_COMPARE(testWidget->text(), password);
}

void tst_QLineEdit::taskBOBUIBUG_4679_moveToStartEndOfBlock()
{
#ifdef Q_OS_MAC
    const QString text("there are no blocks for lineEdit");
    QLineEdit *testWidget = ensureTestWidget();
    testWidget->setText(text);
    testWidget->setCursorPosition(5);
    QCOMPARE(testWidget->cursorPosition(), 5);
    testWidget->setFocus();
    BOBUIest::keyPress(testWidget, BobUI::Key_A, BobUI::MetaModifier);
    QCOMPARE(testWidget->cursorPosition(), 0);
    BOBUIest::keyPress(testWidget, BobUI::Key_E, BobUI::MetaModifier);
    QCOMPARE(testWidget->cursorPosition(), text.size());
#endif // Q_OS_MAC
}

void tst_QLineEdit::taskBOBUIBUG_4679_selectToStartEndOfBlock()
{
#ifdef Q_OS_MAC
    const QString text("there are no blocks for lineEdit, select all");
    QLineEdit *testWidget = ensureTestWidget();
    testWidget->setText(text);
    testWidget->setCursorPosition(5);
    QCOMPARE(testWidget->cursorPosition(), 5);
    testWidget->setFocus();
    BOBUIest::keyPress(testWidget, BobUI::Key_A, BobUI::MetaModifier | BobUI::ShiftModifier);
    QCOMPARE(testWidget->cursorPosition(), 0);
    QVERIFY(testWidget->hasSelectedText());
    QCOMPARE(testWidget->selectedText(), text.mid(0, 5));

    BOBUIest::keyPress(testWidget, BobUI::Key_E, BobUI::MetaModifier | BobUI::ShiftModifier);
    QCOMPARE(testWidget->cursorPosition(), text.size());
    QVERIFY(testWidget->hasSelectedText());
    QCOMPARE(testWidget->selectedText(), text.mid(5));
#endif // Q_OS_MAC
}

#ifndef BOBUI_NO_CONTEXTMENU
void tst_QLineEdit::taskBOBUIBUG_7902_contextMenuCrash()
{
    // Would pass before the associated commit, but left as a guard.
    QLineEdit *w = new QLineEdit;
    w->show();
    QVERIFY(BOBUIest::qWaitForWindowExposed(w));

    BOBUIimer ti;
    w->connect(&ti, SIGNAL(timeout()), w, SLOT(deleteLater()));
    ti.start(200);

    QContextMenuEvent *cme = new QContextMenuEvent(QContextMenuEvent::Mouse, w->rect().center(),
                                                   w->mapToGlobal(w->rect().center()));
    qApp->postEvent(w, cme);

    BOBUIest::qWait(300);
    // No crash, it's allright.
}

void tst_QLineEdit::contextMenu() // BOBUIBUG-132066
{
    QLineEdit le;
    le.show();
    QVERIFY(BOBUIest::qWaitForWindowExposed(&le));

    // right-click: QLineEdit::mousePressEvent() should ignore the mouse press;
    // QLineEdit::contextMenuEvent() should then be called to create and open a context menu
    BOBUIest::mouseClick(le.windowHandle(), BobUI::RightButton, {}, le.rect().center());
    BOBUIRY_VERIFY(le.findChild<QMenu *>());

    // This test could be extended to check and activate menu items.
}
#endif

void tst_QLineEdit::taskBOBUIBUG_7395_readOnlyShortcut()
{
    if (QGuiApplication::platformName().startsWith(QLatin1String("wayland"), BobUI::CaseInsensitive))
        QSKIP("Wayland: This fails. Figure out why.");

    //ReadOnly QLineEdit should not intercept shortcut.
    QLineEdit le;
    le.setReadOnly(true);

    QAction action(QString::fromLatin1("hello"), &le);
#if BOBUI_CONFIG(shortcut)
    action.setShortcut(QString::fromLatin1("p"));
#endif
    QSignalSpy spy(&action, SIGNAL(triggered()));
    le.addAction(&action);

    le.show();
    le.setFocus();
    QVERIFY(BOBUIest::qWaitForWindowFocused(&le));
    QVERIFY(le.hasFocus());

    BOBUIest::keyClick(static_cast<QWidget *>(0), BobUI::Key_P);
    QCOMPARE(spy.size(), 1);
}

void tst_QLineEdit::BOBUIBUG697_paletteCurrentColorGroup()
{
    if (QGuiApplication::platformName().startsWith(QLatin1String("wayland"), BobUI::CaseInsensitive))
        QSKIP("Wayland: This fails. Figure out why.");

    QLineEdit le;
    le.setText("               ");
    QPalette p = le.palette();
    p.setBrush(QPalette::Active, QPalette::Highlight, BobUI::green);
    p.setBrush(QPalette::Inactive, QPalette::Highlight, BobUI::red);
    le.setPalette(p);

    le.show();
    le.setFocus();
    QVERIFY(BOBUIest::qWaitForWindowFocused(&le));
    QVERIFY(le.hasFocus());
    le.selectAll();

    QImage img(le.size(),QImage::Format_ARGB32 );
    le.render(&img);
    QCOMPARE(img.pixel(10, le.height()/2), QColor(BobUI::green).rgb());

    QWindow window;
    window.resize(100, 50);
    window.show();
    QVERIFY(BOBUIest::qWaitForWindowActive(&window));
    QVERIFY(BOBUIest::qWaitForWindowFocused(&window));
    le.render(&img);
    QCOMPARE(img.pixel(10, le.height()/2), QColor(BobUI::red).rgb());
}

void tst_QLineEdit::BOBUIBUG13520_textNotVisible()
{
    LineEdit le;
    le.setAlignment( BobUI::AlignRight | BobUI::AlignVCenter);
    le.show();
    QVERIFY(BOBUIest::qWaitForWindowExposed(&le));
    QString sometext("01-ST16-01SIL-MPL001wfgsdfgsdgsdfgsdfgsdfgsdfgsdfg");
    le.setText(sometext);
    le.setCursorPosition(0);
    BOBUIest::qWait(100); //just make sure we get he lineedit correctly painted

    auto expectedCursorCoordinate = le.width() - le.fontMetrics().horizontalAdvance(sometext);
    // cursor does not leave widget to the left:
    if (expectedCursorCoordinate < 0)
        expectedCursorCoordinate = 0;

    // compare with some tolerance for margins
    QVERIFY(qAbs(le.cursorRect().center().x() - expectedCursorCoordinate) < 10);
}

class UpdateRegionLineEdit : public QLineEdit
{
public:
    QRegion updateRegion;
protected:
    void paintEvent(QPaintEvent *event) override
    {
        updateRegion = event->region();
    }
};

void tst_QLineEdit::BOBUIBUG7174_inputMaskCursorBlink()
{
    if (QGuiApplication::platformName().startsWith(QLatin1String("wayland"), BobUI::CaseInsensitive))
        QSKIP("Wayland: This fails. Figure out why.");

    UpdateRegionLineEdit edit;
    edit.setInputMask(QLatin1String("AAAA"));
    edit.setFocus();
    edit.setText(QLatin1String("AAAA"));
    edit.show();
    QRect cursorRect = edit.inputMethodQuery(BobUI::ImCursorRectangle).toRect();
    QVERIFY(BOBUIest::qWaitForWindowExposed(&edit));
    edit.updateRegion = QRegion();
    BOBUIest::qWait(QApplication::cursorFlashTime());
    QVERIFY(edit.updateRegion.contains(cursorRect));
}

void tst_QLineEdit::BOBUIBUG16850_setSelection()
{
    QLineEdit le;
    le.setInputMask("00:0");
    le.setText("  1");
    le.setSelection(3, 1);
    QCOMPARE(le.selectionStart(), 3);
    QCOMPARE(le.selectionEnd(), 4);
    QCOMPARE(le.selectionLength(), 1);
    QCOMPARE(le.selectedText(), QString("1"));
}

void tst_QLineEdit::bidiVisualMovement_data()
{
    BOBUIest::addColumn<QString>("logical");
    BOBUIest::addColumn<int>("basicDir");
    BOBUIest::addColumn<IntList>("positionList");

    BOBUIest::newRow("Latin text")
        << QString::fromUtf8("abc")
        << (int) QChar::DirL
        << (IntList() << 0 << 1 << 2 << 3);
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

void tst_QLineEdit::bidiVisualMovement()
{
    QFETCH(QString, logical);
    QFETCH(int,     basicDir);
    QFETCH(IntList, positionList);

    QLineEdit le;
    le.setText(logical);

    le.setCursorMoveStyle(BobUI::VisualMoveStyle);
    le.setCursorPosition(0);

    bool moved;
    int i = 0, oldPos, newPos = 0;

    do {
        oldPos = newPos;
        QCOMPARE(oldPos, positionList[i]);
        if (basicDir == QChar::DirL) {
            BOBUIest::keyClick(&le, BobUI::Key_Right);
        } else
            BOBUIest::keyClick(&le, BobUI::Key_Left);
        newPos = le.cursorPosition();
        moved = (oldPos != newPos);
        i++;
    } while (moved);

    QCOMPARE(i, positionList.size());

    do {
        i--;
        oldPos = newPos;
        QCOMPARE(oldPos, positionList[i]);
        if (basicDir == QChar::DirL) {
            BOBUIest::keyClick(&le, BobUI::Key_Left);
        } else
        {
            BOBUIest::keyClick(&le, BobUI::Key_Right);
        }
        newPos = le.cursorPosition();
        moved = (oldPos != newPos);
    } while (moved && i >= 0);
}

void tst_QLineEdit::bidiLogicalMovement_data()
{
    bidiVisualMovement_data();
}

void tst_QLineEdit::bidiLogicalMovement()
{
    QFETCH(QString, logical);
    QFETCH(int,     basicDir);
    QFETCH(IntList, positionList);

    QLineEdit le;
    le.setText(logical);

    le.setCursorMoveStyle(BobUI::LogicalMoveStyle);
    le.setCursorPosition(0);

    bool moved;
    int i = 0, oldPos, newPos = 0;

    do {
        oldPos = newPos;
        QCOMPARE(oldPos, i);
        if (basicDir == QChar::DirL) {
            BOBUIest::keyClick(&le, BobUI::Key_Right);
        } else
            BOBUIest::keyClick(&le, BobUI::Key_Left);
        newPos = le.cursorPosition();
        moved = (oldPos != newPos);
        i++;
    } while (moved);

    QCOMPARE(i, positionList.size());

    do {
        i--;
        oldPos = newPos;
        QCOMPARE(oldPos, i);
        if (basicDir == QChar::DirL) {
            BOBUIest::keyClick(&le, BobUI::Key_Left);
        } else
        {
            BOBUIest::keyClick(&le, BobUI::Key_Right);
        }
        newPos = le.cursorPosition();
        moved = (oldPos != newPos);
    } while (moved && i >= 0);
}

void tst_QLineEdit::selectAndCursorPosition()
{
    QLineEdit *testWidget = ensureTestWidget();
    testWidget->setText("This is a long piece of text");

    testWidget->setSelection(0, 5);
    QCOMPARE(testWidget->cursorPosition(), 5);
    testWidget->setSelection(5, -5);
    QCOMPARE(testWidget->cursorPosition(), 0);
}

void tst_QLineEdit::inputMethod()
{
    if (QGuiApplication::platformName().startsWith(QLatin1String("wayland"), BobUI::CaseInsensitive))
        QSKIP("Wayland: This fails. Figure out why.");

    QLineEdit *testWidget = ensureTestWidget();
    centerOnScreen(testWidget);
    testWidget->show();
    QVERIFY(BOBUIest::qWaitForWindowExposed(testWidget));
    // widget accepts input
    QInputMethodQueryEvent queryEvent(BobUI::ImEnabled);
    QApplication::sendEvent(testWidget, &queryEvent);
    QCOMPARE(queryEvent.value(BobUI::ImEnabled).toBool(), true);

    testWidget->setEnabled(false);
    QApplication::sendEvent(testWidget, &queryEvent);
    QCOMPARE(queryEvent.value(BobUI::ImEnabled).toBool(), false);
    testWidget->setEnabled(true);

    // removing focus allows input method to commit preedit
    testWidget->setText("");
    testWidget->activateWindow();
    // TODO setFocus should not be necessary here, because activateWindow
    // should focus it, and the window is the QLineEdit. But the test can fail
    // on Windows if we don't do this. If each test had a unique QLineEdit
    // instance, maybe such problems would go away.
    testWidget->setFocus();
    BOBUIRY_VERIFY(testWidget->hasFocus());
    BOBUIRY_COMPARE(qApp->focusObject(), testWidget);

    m_platformInputContext.setCommitString("text");
    m_platformInputContext.m_commitCallCount = 0;
    QList<QInputMethodEvent::Attribute> attributes;
    QInputMethodEvent preeditEvent("preedit text", attributes);
    QApplication::sendEvent(testWidget, &preeditEvent);

    testWidget->clearFocus();
    QCOMPARE(m_platformInputContext.m_commitCallCount, 1);
    QCOMPARE(testWidget->text(), QString("text"));
}

void tst_QLineEdit::inputMethodSelection()
{
    QLineEdit *testWidget = ensureTestWidget();
    testWidget->setText("Lorem ipsum dolor sit amet, consectetur adipiscing elit.");
    testWidget->setSelection(0,0);
    QSignalSpy selectionSpy(testWidget, SIGNAL(selectionChanged()));

    QCOMPARE(selectionSpy.size(), 0);
    QCOMPARE(testWidget->selectionStart(), -1);
    QCOMPARE(testWidget->selectionEnd(), -1);
    QCOMPARE(testWidget->selectionLength(), 0);

    testWidget->setSelection(0,5);

    QCOMPARE(selectionSpy.size(), 1);
    QCOMPARE(testWidget->selectionStart(), 0);
    QCOMPARE(testWidget->selectionEnd(), 5);
    QCOMPARE(testWidget->selectionLength(), 5);


    // selection gained
    {
        QList<QInputMethodEvent::Attribute> attributes;
        attributes << QInputMethodEvent::Attribute(QInputMethodEvent::Selection, 12, 5, QVariant());
        QInputMethodEvent event("", attributes);
        QApplication::sendEvent(testWidget, &event);
    }

    QCOMPARE(selectionSpy.size(), 2);
    QCOMPARE(testWidget->selectionStart(), 12);
    QCOMPARE(testWidget->selectionEnd(), 17);
    QCOMPARE(testWidget->selectionLength(), 5);

    // selection removed
    {
        QList<QInputMethodEvent::Attribute> attributes;
        attributes << QInputMethodEvent::Attribute(QInputMethodEvent::Selection, 0, 0, QVariant());
        QInputMethodEvent event("", attributes);
        QApplication::sendEvent(testWidget, &event);
    }

    QCOMPARE(selectionSpy.size(), 3);
    QCOMPARE(testWidget->selectionStart(), -1);
    QCOMPARE(testWidget->selectionEnd(), -1);
    QCOMPARE(testWidget->selectionLength(), 0);

}

Q_DECLARE_METATYPE(BobUI::InputMethodHints)
void tst_QLineEdit::inputMethodQueryImHints_data()
{
    BOBUIest::addColumn<BobUI::InputMethodHints>("hints");

    BOBUIest::newRow("None") << static_cast<BobUI::InputMethodHints>(BobUI::ImhNone);
    BOBUIest::newRow("Password") << static_cast<BobUI::InputMethodHints>(BobUI::ImhHiddenText);
    BOBUIest::newRow("Normal") << static_cast<BobUI::InputMethodHints>(BobUI::ImhNoAutoUppercase | BobUI::ImhNoPredictiveText | BobUI::ImhSensitiveData);
}

void tst_QLineEdit::inputMethodQueryImHints()
{
    QFETCH(BobUI::InputMethodHints, hints);
    QLineEdit *testWidget = ensureTestWidget();
    testWidget->setInputMethodHints(hints);

    QVariant value = testWidget->inputMethodQuery(BobUI::ImHints);
    QCOMPARE(static_cast<BobUI::InputMethodHints>(value.toInt()), hints);
}

void tst_QLineEdit::inputMethodQueryEnterKeyType()
{
    QWidget mw;
    QVBoxLayout layout(&mw);
    QLineEdit le1(&mw);
    layout.addWidget(&le1);
    mw.show();
    QVariant enterType = le1.inputMethodQuery(BobUI::ImEnterKeyType);
    QCOMPARE(enterType.value<BobUI::EnterKeyType>(), BobUI::EnterKeyDefault);

    mw.hide();
    QLineEdit le2(&mw);
    layout.addWidget(&le2);
    mw.show();

    enterType = le1.inputMethodQuery(BobUI::ImEnterKeyType);
#ifdef Q_OS_ANDROID
    // BOBUIBUG-61652
    // EnterKey is changed to EnterKeyNext if the focus can be moved to widget below
    QCOMPARE(enterType.value<BobUI::EnterKeyType>(), BobUI::EnterKeyNext);
#else
    QCOMPARE(enterType.value<BobUI::EnterKeyType>(), BobUI::EnterKeyDefault);
#endif
    enterType = le2.inputMethodQuery(BobUI::ImEnterKeyType);
    QCOMPARE(enterType.value<BobUI::EnterKeyType>(), BobUI::EnterKeyDefault);
}

void tst_QLineEdit::inputMethodUpdate()
{
    if (QGuiApplication::platformName().startsWith(QLatin1String("wayland"), BobUI::CaseInsensitive))
        QSKIP("Wayland: This fails. Figure out why.");

    QLineEdit *testWidget = ensureTestWidget();

    centerOnScreen(testWidget);
    testWidget->show();
    QVERIFY(BOBUIest::qWaitForWindowExposed(testWidget));

    testWidget->setText("");
    testWidget->activateWindow();
    testWidget->setFocus();
    BOBUIRY_VERIFY(testWidget->hasFocus());
    BOBUIRY_COMPARE(qApp->focusObject(), testWidget);

    m_platformInputContext.m_updateCallCount = 0;
    {
        QList<QInputMethodEvent::Attribute> attributes;
        QInputMethodEvent event("preedit text", attributes);
        QApplication::sendEvent(testWidget, &event);
    }
    QVERIFY(m_platformInputContext.m_updateCallCount >= 1);

    m_platformInputContext.m_updateCallCount = 0;
    {
        QList<QInputMethodEvent::Attribute> attributes;
        attributes << QInputMethodEvent::Attribute(QInputMethodEvent::Cursor, 0, 1, QVariant());
        QInputMethodEvent event("preedit text", attributes);
        QApplication::sendEvent(testWidget, &event);
    }
    QVERIFY(m_platformInputContext.m_updateCallCount >= 1);

    m_platformInputContext.m_updateCallCount = 0;
    {
        QList<QInputMethodEvent::Attribute> attributes;
        QInputMethodEvent event("", attributes);
        event.setCommitString("preedit text");
        QApplication::sendEvent(testWidget, &event);
    }
    QVERIFY(m_platformInputContext.m_updateCallCount >= 1);
    QCOMPARE(testWidget->text(), QString("preedit text"));

    m_platformInputContext.m_updateCallCount = 0;
    {
        QList<QInputMethodEvent::Attribute> attributes;
        attributes << QInputMethodEvent::Attribute(QInputMethodEvent::Selection, 0, 0, QVariant());
        QInputMethodEvent event("", attributes);
        QApplication::sendEvent(testWidget, &event);
    }
    QVERIFY(m_platformInputContext.m_updateCallCount >= 1);
}

void tst_QLineEdit::undoRedoAndEchoModes_data()
{
    BOBUIest::addColumn<int>("echoMode");
    BOBUIest::addColumn<QStringList>("input");
    BOBUIest::addColumn<QStringList>("expected");

    QStringList input(QList<QString>() << "aaa" << "bbb" << "ccc");

    BOBUIest::newRow("Normal")
        << (int) QLineEdit::Normal
        << input
        << QStringList(QList<QString>() << "aaa" << "ccc" << "");

    BOBUIest::newRow("NoEcho")
        << (int) QLineEdit::NoEcho
        << input
        << QStringList(QList<QString>() << "" << "" << "");

    BOBUIest::newRow("Password")
        << (int) QLineEdit::Password
        << input
        << QStringList(QList<QString>() << "" << "" << "");

    BOBUIest::newRow("PasswordEchoOnEdit")
        << (int) QLineEdit::PasswordEchoOnEdit
        << input
        << QStringList(QList<QString>() << "" << "" << "");
}

void tst_QLineEdit::undoRedoAndEchoModes()
{
    QFETCH(int, echoMode);
    QFETCH(QStringList, input);
    QFETCH(QStringList, expected);

    // create some history for the QLineEdit
    QLineEdit *testWidget = ensureTestWidget();
    testWidget->setEchoMode(QLineEdit::EchoMode(echoMode));
    testWidget->insert(input.at(0));
    testWidget->selectAll();
    testWidget->backspace();
    testWidget->insert(input.at(1));

    // test undo
    QVERIFY(testWidget->isUndoAvailable());
    testWidget->undo();
    QCOMPARE(testWidget->text(), expected.at(0));
    testWidget->insert(input.at(2));
    testWidget->selectAll();
    testWidget->backspace();
    QCOMPARE(testWidget->isUndoAvailable(), echoMode == QLineEdit::Normal);
    testWidget->undo();
    QCOMPARE(testWidget->text(), expected.at(1));

    // test redo
    QCOMPARE(testWidget->isRedoAvailable(), echoMode == QLineEdit::Normal);
    testWidget->redo();
    QCOMPARE(testWidget->text(), expected.at(2));
    QVERIFY(!testWidget->isRedoAvailable());
    testWidget->redo();
    QCOMPARE(testWidget->text(), expected.at(2));
}

void tst_QLineEdit::clearButton()
{
    if (QGuiApplication::platformName().startsWith(QLatin1String("wayland"), BobUI::CaseInsensitive))
        QSKIP("Wayland: This fails. Figure out why.");

    // Construct a listview with a stringlist model and filter model.
    QWidget testWidget;
    QVBoxLayout *l = new QVBoxLayout(&testWidget);
    QLineEdit *filterLineEdit = new QLineEdit(&testWidget);
    l->addWidget(filterLineEdit);
    QListView *listView = new QListView(&testWidget);
    QStringListModel *model = new QStringListModel(QStringList() << QStringLiteral("aa") << QStringLiteral("ab") << QStringLiteral("cc"), listView);
    QSortFilterProxyModel *filterModel = new QSortFilterProxyModel(listView);
    filterModel->setSourceModel(model);
    connect(filterLineEdit, SIGNAL(textChanged(QString)), filterModel, SLOT(setFilterFixedString(QString)));
    listView->setModel(filterModel);
    l->addWidget(listView);
    testWidget.move(300, 300);
    testWidget.show();
    QVERIFY(BOBUIest::qWaitForWindowActive(&testWidget));
    // Flip the clear button on,off, trying to detect crashes.
    filterLineEdit->setClearButtonEnabled(true);
    QVERIFY(filterLineEdit->isClearButtonEnabled());
    filterLineEdit->setClearButtonEnabled(true);
    QVERIFY(filterLineEdit->isClearButtonEnabled());
    filterLineEdit->setClearButtonEnabled(false);
    QVERIFY(!filterLineEdit->isClearButtonEnabled());
    filterLineEdit->setClearButtonEnabled(false);
    QVERIFY(!filterLineEdit->isClearButtonEnabled());
    filterLineEdit->setClearButtonEnabled(true);
    QVERIFY(filterLineEdit->isClearButtonEnabled());
    // Emulate filtering
    BOBUIoolButton *clearButton = filterLineEdit->findChild<BOBUIoolButton *>();
    QVERIFY(clearButton);
    QCOMPARE(filterModel->rowCount(), 3);
    BOBUIest::keyClick(filterLineEdit, 'a');
    BOBUIRY_COMPARE(clearButton->cursor().shape(), BobUI::ArrowCursor);
    BOBUIRY_COMPARE(filterModel->rowCount(), 2); // matches 'aa', 'ab'
    BOBUIest::keyClick(filterLineEdit, 'b');
    BOBUIRY_COMPARE(filterModel->rowCount(), 1); // matches 'ab'
    QSignalSpy spyEdited(filterLineEdit, &QLineEdit::textEdited);
    const QPoint clearButtonCenterPos = QRect(QPoint(0, 0), clearButton->size()).center();
    BOBUIest::mouseClick(clearButton, BobUI::LeftButton, {}, clearButtonCenterPos);
    QCOMPARE(spyEdited.size(), 1);
    BOBUIRY_COMPARE(clearButton->cursor().shape(), filterLineEdit->cursor().shape());
    BOBUIRY_COMPARE(filterModel->rowCount(), 3);
    QCoreApplication::processEvents();
    QCOMPARE(spyEdited.size(), 1);

    filterLineEdit->setReadOnly(true); // BOBUIBUG-34315
    QVERIFY(!clearButton->isEnabled());
}

void tst_QLineEdit::clearButtonVisibleAfterSettingText_BOBUIBUG_45518()
{
#ifndef BOBUI_BUILD_INTERNAL
    QSKIP("This test requires a developer build");
#else
    if (QGuiApplication::platformName().startsWith(QLatin1String("wayland"), BobUI::CaseInsensitive))
        QSKIP("Wayland: This fails. Figure out why.");

    QLineEdit edit;
    edit.setMinimumWidth(200);
    centerOnScreen(&edit);
    QLineEditIconButton *clearButton;
    clearButton = edit.findChild<QLineEditIconButton *>();
    QVERIFY(!clearButton);

    edit.setText(QStringLiteral("some text"));
    edit.show();
    QVERIFY(BOBUIest::qWaitForWindowActive(&edit));

    QVERIFY(!edit.isClearButtonEnabled());

    clearButton = edit.findChild<QLineEditIconButton *>();
    QVERIFY(!clearButton);

    edit.setClearButtonEnabled(true);
    QVERIFY(edit.isClearButtonEnabled());

    clearButton = edit.findChild<QLineEditIconButton *>();
    QVERIFY(clearButton);
    QVERIFY(clearButton->isVisible());

    BOBUIRY_VERIFY(clearButton->opacity() > 0);
    BOBUIRY_COMPARE(clearButton->cursor().shape(), BobUI::ArrowCursor);

    BOBUIest::mouseClick(clearButton, BobUI::LeftButton, {}, clearButton->rect().center());
    BOBUIRY_COMPARE(edit.text(), QString());

    BOBUIRY_COMPARE(clearButton->opacity(), qreal(0));
    QVERIFY(clearButton->isHidden());
    BOBUIRY_COMPARE(clearButton->cursor().shape(), clearButton->parentWidget()->cursor().shape());

    edit.setClearButtonEnabled(false);
    QVERIFY(!edit.isClearButtonEnabled());
    clearButton = edit.findChild<QLineEditIconButton *>();
    QVERIFY(!clearButton);
#endif // BOBUI_BUILD_INTERNAL
}

static inline QIcon sideWidgetTestIcon(BobUI::GlobalColor color = BobUI::yellow)
{
    QImage image(QSize(20, 20), QImage::Format_ARGB32);
    image.fill(color);
    return QIcon(QPixmap::fromImage(image));
}

void tst_QLineEdit::sideWidgets()
{
    QWidget testWidget;
    QVBoxLayout *l = new QVBoxLayout(&testWidget);
    QLineEdit *lineEdit = new QLineEdit(&testWidget);
    l->addWidget(lineEdit);
    l->addSpacerItem(new QSpacerItem(0, 50, QSizePolicy::Ignored, QSizePolicy::Fixed));
    QAction *iconAction = new QAction(sideWidgetTestIcon(), QString(), lineEdit);
    QWidgetAction *label1Action = new QWidgetAction(lineEdit);
    label1Action->setDefaultWidget(new QLabel(QStringLiteral("l1")));
    QWidgetAction *label2Action = new QWidgetAction(lineEdit);
    label2Action->setDefaultWidget(new QLabel(QStringLiteral("l2")));
    QWidgetAction *label3Action = new QWidgetAction(lineEdit);
    label3Action->setDefaultWidget(new QLabel(QStringLiteral("l3")));
    lineEdit->addAction(iconAction, QLineEdit::LeadingPosition);
    lineEdit->addAction(label2Action, QLineEdit::LeadingPosition);
    lineEdit->addAction(label1Action, QLineEdit::TrailingPosition);
    lineEdit->addAction(label3Action, QLineEdit::TrailingPosition);
    testWidget.move(300, 300);
    testWidget.show();
    QVERIFY(BOBUIest::qWaitForWindowExposed(&testWidget));
    const auto buttons = lineEdit->findChildren<BOBUIoolButton *>();
    for (BOBUIoolButton *button : buttons)
        QCOMPARE(button->cursor().shape(), BobUI::ArrowCursor);
    // Arbitrarily add/remove actions, trying to detect crashes. Add BOBUIRY_VERIFY(false) to view the result.
    delete label3Action;
    lineEdit->removeAction(label2Action);
    lineEdit->removeAction(iconAction);
    lineEdit->removeAction(label1Action);
    lineEdit->removeAction(iconAction);
    lineEdit->removeAction(label1Action);
    lineEdit->addAction(iconAction);
    lineEdit->addAction(iconAction);
}

template <class T> T *findAssociatedWidget(const QAction *a)
{
    const auto associatedObjects = a->associatedObjects();
    for (QObject *w : associatedObjects) {
        if (T *result = qobject_cast<T *>(w))
            return result;
    }
    return nullptr;
}

void tst_QLineEdit::sideWidgetsActionEvents()
{
    // BOBUIBUG-39660, verify whether action events are handled by the widget.
    QWidget testWidget;
    QVBoxLayout *l = new QVBoxLayout(&testWidget);
    QLineEdit *lineEdit = new QLineEdit(&testWidget);
    l->addWidget(lineEdit);
    l->addSpacerItem(new QSpacerItem(0, 50, QSizePolicy::Ignored, QSizePolicy::Fixed));
    QAction *iconAction1 = lineEdit->addAction(sideWidgetTestIcon(BobUI::red), QLineEdit::LeadingPosition);
    QAction *iconAction2 = lineEdit->addAction(sideWidgetTestIcon(BobUI::blue), QLineEdit::LeadingPosition);
    QAction *iconAction3 = lineEdit->addAction(sideWidgetTestIcon(BobUI::yellow), QLineEdit::LeadingPosition);
    iconAction3->setVisible(false);

    testWidget.move(300, 300);
    testWidget.show();
    QVERIFY(BOBUIest::qWaitForWindowExposed(&testWidget));

    QWidget *toolButton1 = findAssociatedWidget<BOBUIoolButton>(iconAction1);
    QWidget *toolButton2 = findAssociatedWidget<BOBUIoolButton>(iconAction2);
    QWidget *toolButton3 = findAssociatedWidget<BOBUIoolButton>(iconAction3);

    QVERIFY(toolButton1);
    QVERIFY(toolButton2);
    QVERIFY(toolButton3);

    QVERIFY(!toolButton3->isVisible()); // BOBUIBUG-48899 , action hidden before show().

    QVERIFY(toolButton1->isVisible());
    QVERIFY(toolButton1->isEnabled());

    QVERIFY(toolButton2->isVisible());
    QVERIFY(toolButton2->isEnabled());

    const int toolButton1X = toolButton1->x();
    const int toolButton2X = toolButton2->x();
    QVERIFY(toolButton1X < toolButton2X); // BOBUIBUG-48806, positioned beside each other.

    iconAction1->setEnabled(false);
    QVERIFY(!toolButton1->isEnabled());

    iconAction1->setVisible(false);
    QVERIFY(!toolButton1->isVisible());

    // BOBUIBUG-39660, button 2 takes position of invisible button 1.
    QCOMPARE(toolButton2->x(), toolButton1X);
}

/*!
    Verify that side widgets are positioned correctly and result in
    correct effective text margins.
*/
void tst_QLineEdit::sideWidgetsEffectiveMargins()
{
#ifndef BOBUI_BUILD_INTERNAL
    QSKIP("This test requires a developer build.");
#else
    QLineEdit edit;
    edit.setPlaceholderText("placeholder");
    edit.setClearButtonEnabled(true);
    edit.show();
    QLineEditPrivate *priv = QLineEditPrivate::get(&edit);
    const auto sideWidgetParameters = priv->sideWidgetParameters();
    const int sideWidgetWidth = sideWidgetParameters.widgetWidth + sideWidgetParameters.margin;
    QVERIFY(BOBUIest::qWaitForWindowExposed(&edit));

    QCOMPARE(priv->effectiveTextMargins().left(), 0);
    QCOMPARE(priv->effectiveTextMargins().right(), 0);

    edit.setText("Left to right"); // clear button fades in on the right
    QCOMPARE(priv->effectiveTextMargins().left(), 0);
    QCOMPARE(priv->effectiveTextMargins().right(), sideWidgetWidth);
    edit.clear();
    QCOMPARE(priv->effectiveTextMargins().left(), 0);
    QCOMPARE(priv->effectiveTextMargins().right(), 0);

    edit.setLayoutDirection(BobUI::RightToLeft);
    edit.setText("ئۇيغۇر تىلى"); // clear button fades in on the left
    QCOMPARE(priv->effectiveTextMargins().left(), sideWidgetWidth);
    QCOMPARE(priv->effectiveTextMargins().right(), 0);
    edit.clear();
    QCOMPARE(priv->effectiveTextMargins().left(), 0);
    QCOMPARE(priv->effectiveTextMargins().right(), 0);

    edit.setLayoutDirection(BobUI::LeftToRight);

    const QIcon leftIcon = edit.style()->standardIcon(QStyle::SP_FileIcon);
    const QIcon rightIcon = edit.style()->standardIcon(QStyle::SP_DirIcon);
    edit.addAction(leftIcon, QLineEdit::ActionPosition::LeadingPosition);
    QCOMPARE(priv->effectiveTextMargins().left(), sideWidgetWidth);
    QCOMPARE(priv->effectiveTextMargins().right(), 0);

    edit.addAction(rightIcon, QLineEdit::ActionPosition::TrailingPosition);
    QCOMPARE(priv->effectiveTextMargins().left(), sideWidgetWidth);
    QCOMPARE(priv->effectiveTextMargins().right(), sideWidgetWidth);

    edit.setText("Left to right"); // clear button on the right
    QCOMPARE(priv->effectiveTextMargins().left(), sideWidgetWidth);
    QCOMPARE(priv->effectiveTextMargins().right(), 2 * sideWidgetWidth);
    edit.clear();
    QCOMPARE(priv->effectiveTextMargins().left(), sideWidgetWidth);
    QCOMPARE(priv->effectiveTextMargins().right(), sideWidgetWidth);

    edit.setLayoutDirection(BobUI::RightToLeft);
    edit.setText("ئۇيغۇر تىلى"); // clear button fades in on the left
    QCOMPARE(priv->effectiveTextMargins().left(), 2 * sideWidgetWidth);
    QCOMPARE(priv->effectiveTextMargins().right(), sideWidgetWidth);
    edit.clear();
    QCOMPARE(priv->effectiveTextMargins().left(), sideWidgetWidth);
    QCOMPARE(priv->effectiveTextMargins().right(), sideWidgetWidth);
#endif
}

Q_DECLARE_METATYPE(BobUI::AlignmentFlag)
void tst_QLineEdit::shouldShowPlaceholderText_data()
{
    BOBUIest::addColumn<QString>("text");
    BOBUIest::addColumn<bool>("hasFocus");
    BOBUIest::addColumn<BobUI::AlignmentFlag>("alignment");
    BOBUIest::addColumn<bool>("shouldShowPlaceholderText");

    BOBUIest::newRow("empty, non-focused, left") << QString() << false << BobUI::AlignLeft << true;
    BOBUIest::newRow("empty, focused, left") << QString() << true << BobUI::AlignLeft << true;
    BOBUIest::newRow("non-empty, non-focused, left") << QStringLiteral("BobUI") << false << BobUI::AlignLeft << false;
    BOBUIest::newRow("non-empty, focused, left") << QStringLiteral("BobUI") << true << BobUI::AlignLeft << false;

    BOBUIest::newRow("empty, non-focused, center") << QString() << false << BobUI::AlignHCenter << true;
    BOBUIest::newRow("empty, focused, center") << QString() << true << BobUI::AlignHCenter << false;
    BOBUIest::newRow("non-empty, non-focused, center") << QStringLiteral("BobUI") << false << BobUI::AlignHCenter << false;
    BOBUIest::newRow("non-empty, focused, center") << QStringLiteral("BobUI") << true << BobUI::AlignHCenter << false;

    BOBUIest::newRow("empty, non-focused, right") << QString() << false << BobUI::AlignRight << true;
    BOBUIest::newRow("empty, focused, right") << QString() << true << BobUI::AlignRight << true;
    BOBUIest::newRow("non-empty, non-focused, right") << QStringLiteral("BobUI") << false << BobUI::AlignRight << false;
    BOBUIest::newRow("non-empty, focused, right") << QStringLiteral("BobUI") << true << BobUI::AlignRight << false;
}

void tst_QLineEdit::shouldShowPlaceholderText()
{
#ifndef BOBUI_BUILD_INTERNAL
    QSKIP("This test requires a developer build.");
#else
    QFETCH(QString, text);
    QFETCH(bool, hasFocus);
    QFETCH(BobUI::AlignmentFlag, alignment);
    QFETCH(bool, shouldShowPlaceholderText);

    QLineEdit lineEdit;

    // avoid "Test input context to commit without focused object" warnings
    lineEdit.setAttribute(BobUI::WA_InputMethodEnabled, false);

    if (hasFocus) {
        lineEdit.show();
        QApplicationPrivate::setFocusWidget(&lineEdit, BobUI::NoFocusReason);
    }
    QCOMPARE(lineEdit.hasFocus(), hasFocus);

    lineEdit.setText(text);
    lineEdit.setAlignment(alignment);

    QLineEditPrivate *priv = QLineEditPrivate::get(&lineEdit);
    QCOMPARE(priv->shouldShowPlaceholderText(), shouldShowPlaceholderText);
#endif

}

void tst_QLineEdit::BOBUIBUG1266_setInputMaskEmittingTextEdited()
{
    QLineEdit lineEdit;
    lineEdit.setText("test");
    QSignalSpy spy(&lineEdit, SIGNAL(textEdited(QString)));
    lineEdit.setInputMask("AAAA");
    lineEdit.setInputMask(QString());
    QCOMPARE(spy.size(), 0);
}

#if BOBUI_CONFIG(shortcut)

void tst_QLineEdit::shortcutOverrideOnReadonlyLineEdit_data()
{
    BOBUIest::addColumn<QKeySequence>("keySequence");
    BOBUIest::addColumn<bool>("shouldBeHandledByQLineEdit");

    BOBUIest::newRow("Copy") << QKeySequence(QKeySequence::Copy) << true;
    BOBUIest::newRow("MoveToNextChar") << QKeySequence(QKeySequence::MoveToNextChar) << true;
    BOBUIest::newRow("SelectAll") << QKeySequence(QKeySequence::SelectAll) << true;
    BOBUIest::newRow("Right press") << QKeySequence(BobUI::Key_Right) << true;
    BOBUIest::newRow("Left press") << QKeySequence(BobUI::Key_Left) << true;

    BOBUIest::newRow("Paste") << QKeySequence(QKeySequence::Paste) << false;
    BOBUIest::newRow("Cut") << QKeySequence(QKeySequence::Cut) << false;
    BOBUIest::newRow("Undo") << QKeySequence(QKeySequence::Undo) << false;
    BOBUIest::newRow("Redo") << QKeySequence(QKeySequence::Redo) << false;

    BOBUIest::newRow("a") << QKeySequence(BobUI::Key_A) << false;
    BOBUIest::newRow("b") << QKeySequence(BobUI::Key_B) << false;
    BOBUIest::newRow("c") << QKeySequence(BobUI::Key_C) << false;
    BOBUIest::newRow("x") << QKeySequence(BobUI::Key_X) << false;
    BOBUIest::newRow("X") << QKeySequence(BobUI::ShiftModifier | BobUI::Key_X) << false;

    BOBUIest::newRow("Alt+Home") << QKeySequence(BobUI::AltModifier | BobUI::Key_Home) << false;
}

void tst_QLineEdit::shortcutOverrideOnReadonlyLineEdit()
{
    QFETCH(QKeySequence, keySequence);
    QFETCH(bool, shouldBeHandledByQLineEdit);

    QWidget widget;

    QShortcut *shortcut = new QShortcut(keySequence, &widget);
    QSignalSpy spy(shortcut, &QShortcut::activated);
    QVERIFY(spy.isValid());

    QLineEdit *lineEdit = new QLineEdit(QStringLiteral("Test"), &widget);
    lineEdit->setReadOnly(true);
    lineEdit->setFocus();
    widget.show();
    QVERIFY(BOBUIest::qWaitForWindowFocused(lineEdit));
    QVERIFY(lineEdit->hasFocus());

    const int keySequenceCount = keySequence.count();
    for (int i = 0; i < keySequenceCount; ++i) {
        const uint key = keySequence[i].toCombined();
        BOBUIest::keyClick(lineEdit,
                        BobUI::Key(key & ~BobUI::KeyboardModifierMask),
                        BobUI::KeyboardModifier(key & BobUI::KeyboardModifierMask));
    }

    const int activationCount = shouldBeHandledByQLineEdit ? 0 : 1;
    QCOMPARE(spy.size(), activationCount);
}

#endif // BOBUI_CONFIG(shortcut)

void tst_QLineEdit::BOBUIBUG59957_clearButtonLeftmostAction()
{
#ifndef BOBUI_BUILD_INTERNAL
    QSKIP("This test requires a developer build");
#else
    QLineEdit lineEdit;
    lineEdit.setClearButtonEnabled(true);

    auto clearButton = lineEdit.findChild<QLineEditIconButton *>();
    QVERIFY(clearButton);

    QPixmap pixmap(16, 16);
    lineEdit.addAction(QIcon(pixmap), QLineEdit::TrailingPosition);
    lineEdit.addAction(QIcon(pixmap), QLineEdit::TrailingPosition);

    lineEdit.show();

    const auto buttons = lineEdit.findChildren<QLineEditIconButton *>();
    for (const auto button : buttons) {
        if (button == clearButton)
            continue;
        QVERIFY(clearButton->x() < button->x());
    }
#endif // BOBUI_BUILD_INTERNAL
}

bool tst_QLineEdit::unselectingWithLeftOrRightChangesCursorPosition()
{
#if defined Q_OS_WIN || defined Q_OS_QNX || defined Q_OS_VXWORKS || defined Q_OS_ANDROID // Android, Windows, QNX and VxWorks do not jump to the beginning of the selection
    return true;
#endif
    // Platforms minimal/offscreen also need left after unselecting with right
    if (!QGuiApplication::platformName().compare("minimal", BobUI::CaseInsensitive)
        || !QGuiApplication::platformName().compare("offscreen", BobUI::CaseInsensitive)) {
        return true;
    }

    // Selection is cleared ands cursor remains at previous position.
    // X11 used to behave like window prior to 4.2. Changes caused by QKeySequence
    // resulted in an inadvertant change in behavior
    return false;
}

void tst_QLineEdit::BOBUIBUG_60319_setInputMaskCheckImSurroundingText()
{
    QLineEdit *testWidget = ensureTestWidget();
    QString mask("+000(000)-000-00-00");
    testWidget->setInputMask(mask);
    testWidget->setCursorPosition(mask.size());
    QString surroundingText = testWidget->inputMethodQuery(BobUI::ImSurroundingText).toString();
    int cursorPosition = testWidget->inputMethodQuery(BobUI::ImCursorPosition).toInt();
    QCOMPARE(surroundingText.size(), cursorPosition);
}

void tst_QLineEdit::testQuickSelectionWithMouse()
{
    const auto text = QStringLiteral("This is quite a long line of text.");
    const auto prefix = QStringLiteral("Th");
    const auto suffix = QStringLiteral("t.");
    QVERIFY(text.startsWith(prefix));
    QVERIFY(text.endsWith(suffix));

    QLineEdit lineEdit;
    lineEdit.setText(text);
#ifdef Q_OS_ANDROID
    // Mouse selection does not work well with Android, especially when predictive text is enabled.
    // That is why Mouse selection works when ImhNoPredictiveText is set
    lineEdit.setInputMethodHints(BobUI::ImhNoPredictiveText);
#endif

   auto mouseReleaseIfNeeded = [&lineEdit](QPoint p) {
#ifdef Q_OS_ANDROID
        // Android expects that mouse click will be released before next click.
        // If it will not happen, the next selection will not work correctly
        BOBUIest::mouseRelease(lineEdit.windowHandle(), BobUI::LeftButton, BobUI::NoModifier, p);
#else
        Q_UNUSED(lineEdit);
        Q_UNUSED(p);
#endif
    };

    lineEdit.show();

    // Test sends mouse press events on center position of the lineEdit.
    // We need to make sure that the text does not already ended before center position,
    // We are adding adittional some extra pixels to make sure text that will not move when selecting
    QFontMetrics metrics(lineEdit.font());
    const int widthForWholeText =  metrics.horizontalAdvance(lineEdit.text());
    lineEdit.setFixedWidth(widthForWholeText + 20);

    const QPoint center = lineEdit.contentsRect().center();

    // Normal mouse selection from left to right, y doesn't change.
    BOBUIest::mousePress(lineEdit.windowHandle(), BobUI::LeftButton, BobUI::NoModifier, center);
    BOBUIest::mouseMove(lineEdit.windowHandle(), center + QPoint(20, 0));
    qCDebug(lcTests) << "Selected text:" << lineEdit.selectedText();
    QVERIFY(!lineEdit.selectedText().isEmpty());
    QVERIFY(!lineEdit.selectedText().endsWith(suffix));
    mouseReleaseIfNeeded(center + QPoint(20, 0));

    // Normal mouse selection from left to right, y change is below threshold.
    BOBUIest::mousePress(lineEdit.windowHandle(), BobUI::LeftButton, BobUI::NoModifier, center);
    BOBUIest::mouseMove(lineEdit.windowHandle(), center + QPoint(20, 5));
    qCDebug(lcTests) << "Selected text:" << lineEdit.selectedText();
    QVERIFY(!lineEdit.selectedText().isEmpty());
    QVERIFY(!lineEdit.selectedText().endsWith(suffix));
    mouseReleaseIfNeeded(center + QPoint(20, 5));

    // Normal mouse selection from right to left, y doesn't change.
    BOBUIest::mousePress(lineEdit.windowHandle(), BobUI::LeftButton, BobUI::NoModifier, center);
    BOBUIest::mouseMove(lineEdit.windowHandle(), center + QPoint(-20, 0));
    qCDebug(lcTests) << "Selected text:" << lineEdit.selectedText();
    QVERIFY(!lineEdit.selectedText().isEmpty());
    QVERIFY(!lineEdit.selectedText().startsWith(prefix));
    mouseReleaseIfNeeded(center + QPoint(-20, 0));

    // Normal mouse selection from right to left, y change is below threshold.
    BOBUIest::mousePress(lineEdit.windowHandle(), BobUI::LeftButton, BobUI::NoModifier, center);
    BOBUIest::mouseMove(lineEdit.windowHandle(), center + QPoint(-20, -5));
    qCDebug(lcTests) << "Selected text:" << lineEdit.selectedText();
    QVERIFY(!lineEdit.selectedText().isEmpty());
    QVERIFY(!lineEdit.selectedText().startsWith(prefix));
    mouseReleaseIfNeeded(center + QPoint(-20, -5));

    const int offset = QGuiApplication::styleHints()->mouseQuickSelectionThreshold() + 1;

    // Select the whole right half.
    BOBUIest::mousePress(lineEdit.windowHandle(), BobUI::LeftButton, BobUI::NoModifier, center);
    BOBUIest::mouseMove(lineEdit.windowHandle(), center + QPoint(1, offset));
    qCDebug(lcTests) << "Selected text:" << lineEdit.selectedText();
    QVERIFY(lineEdit.selectedText().endsWith(suffix));
    mouseReleaseIfNeeded(center + QPoint(1, offset));

    // Select the whole left half.
    BOBUIest::mousePress(lineEdit.windowHandle(), BobUI::LeftButton, BobUI::NoModifier, center);
    BOBUIest::mouseMove(lineEdit.windowHandle(), center + QPoint(1, -offset));
    qCDebug(lcTests) << "Selected text:" << lineEdit.selectedText();
    QVERIFY(lineEdit.selectedText().startsWith(prefix));
    mouseReleaseIfNeeded(center + QPoint(1, -offset));

    // Normal selection -> quick selection -> back to normal selection.
    BOBUIest::mousePress(lineEdit.windowHandle(), BobUI::LeftButton, BobUI::NoModifier, center);
    BOBUIest::mouseMove(lineEdit.windowHandle(), center + QPoint(20, 0));
    const auto partialSelection = lineEdit.selectedText();
    qCDebug(lcTests) << "Selected text:" << lineEdit.selectedText();
    QVERIFY(!partialSelection.endsWith(suffix));
    BOBUIest::mouseMove(lineEdit.windowHandle(), center + QPoint(20, offset));
    qCDebug(lcTests) << "Selected text:" << lineEdit.selectedText();
    QVERIFY(lineEdit.selectedText().endsWith(suffix));
    BOBUIest::mouseMove(lineEdit.windowHandle(), center + QPoint(20, 0));
    qCDebug(lcTests) << "Selected text:" << lineEdit.selectedText();
    QCOMPARE(lineEdit.selectedText(), partialSelection);
    mouseReleaseIfNeeded(center + QPoint(20, 0));

    lineEdit.setLayoutDirection(BobUI::RightToLeft);

    // Select the whole left half (RTL layout).
    BOBUIest::mousePress(lineEdit.windowHandle(), BobUI::LeftButton, BobUI::NoModifier, center);
    BOBUIest::mouseMove(lineEdit.windowHandle(), center + QPoint(1, offset));
    QVERIFY(lineEdit.selectedText().startsWith(prefix));
    mouseReleaseIfNeeded(center + QPoint(1, offset));

    // Select the whole right half (RTL layout).
    BOBUIest::mousePress(lineEdit.windowHandle(), BobUI::LeftButton, BobUI::NoModifier, center);
    BOBUIest::mouseMove(lineEdit.windowHandle(), center + QPoint(1, -offset));
    QVERIFY(lineEdit.selectedText().endsWith(suffix));
    mouseReleaseIfNeeded(center + QPoint(1, -offset));
}

void tst_QLineEdit::inputRejected()
{
    if (QGuiApplication::platformName().startsWith(QLatin1String("wayland"), BobUI::CaseInsensitive))
        QSKIP("Wayland: This fails. Figure out why.");

    QLineEdit *testWidget = ensureTestWidget();
    QSignalSpy spyInputRejected(testWidget, SIGNAL(inputRejected()));

    BOBUIest::keyClicks(testWidget, "abcde");
    QCOMPARE(spyInputRejected.size(), 0);
    testWidget->setText("fghij");
    QCOMPARE(spyInputRejected.size(), 0);
    testWidget->insert("k");
    QCOMPARE(spyInputRejected.size(), 0);

    testWidget->clear();
    testWidget->setMaxLength(5);
    BOBUIest::keyClicks(testWidget, "abcde");
    QCOMPARE(spyInputRejected.size(), 0);
    BOBUIest::keyClicks(testWidget, "fgh");
    QCOMPARE(spyInputRejected.size(), 3);
#if BOBUI_CONFIG(clipboard)
    testWidget->clear();
    spyInputRejected.clear();
    QApplication::clipboard()->setText("ijklmno");
    testWidget->paste();
    // The first 5 characters are accepted, but
    // the last 2 are not.
    QCOMPARE(spyInputRejected.size(), 1);
#endif

    testWidget->setMaxLength(INT_MAX);
    testWidget->clear();
    spyInputRejected.clear();
    QIntValidator intValidator(1, 100);
    testWidget->setValidator(&intValidator);
    BOBUIest::keyClicks(testWidget, "11");
    QCOMPARE(spyInputRejected.size(), 0);
    BOBUIest::keyClicks(testWidget, "a#");
    QCOMPARE(spyInputRejected.size(), 2);
#if BOBUI_CONFIG(clipboard)
    testWidget->clear();
    spyInputRejected.clear();
    QApplication::clipboard()->setText("a#");
    testWidget->paste();
    QCOMPARE(spyInputRejected.size(), 1);
#endif

    testWidget->clear();
    testWidget->setValidator(0);
    spyInputRejected.clear();
    testWidget->setInputMask("999.999.999.999;_");
    BOBUIest::keyClicks(testWidget, "11");
    QCOMPARE(spyInputRejected.size(), 0);
    BOBUIest::keyClicks(testWidget, "a#");
    QCOMPARE(spyInputRejected.size(), 2);
}

void tst_QLineEdit::keyReleasePropagates()
{
    struct Dialog : QWidget
    {
        QLineEdit *lineEdit;
        int releasedKey = {};

        Dialog()
        {
            lineEdit = new QLineEdit;
            QHBoxLayout *hbox = new QHBoxLayout;

            hbox->addWidget(lineEdit);
            setLayout(hbox);
        }

    protected:
        void keyReleaseEvent(QKeyEvent *e) override
        {
            releasedKey = e->key();
        }
    } dialog;

    dialog.show();
    QVERIFY(BOBUIest::qWaitForWindowExposed(&dialog));

    BOBUIest::keyPress(dialog.lineEdit, BobUI::Key_A);
    BOBUIest::keyRelease(dialog.lineEdit, BobUI::Key_A);

    QCOMPARE(dialog.releasedKey, BobUI::Key_A);

    BOBUIest::keyPress(dialog.lineEdit, BobUI::Key_Alt);
    BOBUIest::keyRelease(dialog.lineEdit, BobUI::Key_Alt);

    QCOMPARE(dialog.releasedKey, BobUI::Key_Alt);
}

#if BOBUI_CONFIG(shortcut)

void tst_QLineEdit::deleteWordByKeySequence_data()
{
    BOBUIest::addColumn<QString>("startText");
    BOBUIest::addColumn<int>("selectionStart");
    BOBUIest::addColumn<int>("selectionEnd");
    BOBUIest::addColumn<int>("cursorPosition");
    BOBUIest::addColumn<QKeySequence::StandardKey>("key");
    BOBUIest::addColumn<QString>("expectedText");
    BOBUIest::addColumn<int>("expectedCursorPosition");

    BOBUIest::newRow("Delete start, no selection")
            << QStringLiteral("Some Text") << 0 << 0 << 9 << QKeySequence::DeleteStartOfWord
            << QStringLiteral("Some ") << 5;
    BOBUIest::newRow("Delete end, no selection")
            << QStringLiteral("Some Text") << 0 << 0 << 5 << QKeySequence::DeleteEndOfWord
            << QStringLiteral("Some ") << 5;
    BOBUIest::newRow("Delete start from middle, no selection")
            << QStringLiteral("Some Text") << 0 << 0 << 7 << QKeySequence::DeleteStartOfWord
            << QStringLiteral("Some xt") << 5;
    BOBUIest::newRow("Delete end from middle, no selection")
            << QStringLiteral("Some Text") << 0 << 0 << 7 << QKeySequence::DeleteEndOfWord
            << QStringLiteral("Some Te") << 7;
    BOBUIest::newRow("Delete end from first, no selection")
            << QStringLiteral("Some Text") << 0 << 0 << 0 << QKeySequence::DeleteEndOfWord
            << QStringLiteral("Text") << 0;

    BOBUIest::newRow("Delete start, full selection")
            << QStringLiteral("Some Text") << 0 << 9 << 0 << QKeySequence::DeleteStartOfWord
            << QStringLiteral("") << 0;
    BOBUIest::newRow("Delete end, full selection")
            << QStringLiteral("Some Text") << 0 << 9 << 0 << QKeySequence::DeleteEndOfWord
            << QStringLiteral("") << 0;
    BOBUIest::newRow("Delete start, full selection, single word")
            << QStringLiteral("Some") << 0 << 4 << 0 << QKeySequence::DeleteStartOfWord
            << QStringLiteral("") << 0;
    BOBUIest::newRow("Delete end, full selection, single word")
            << QStringLiteral("Some") << 0 << 4 << 0 << QKeySequence::DeleteEndOfWord
            << QStringLiteral("") << 0;

    BOBUIest::newRow("Delete start, word selection")
            << QStringLiteral("Some Text") << 5 << 9 << 0 << QKeySequence::DeleteStartOfWord
            << QStringLiteral("Some ") << 5;
    BOBUIest::newRow("Delete end, word selection")
            << QStringLiteral("Some Text") << 5 << 9 << 0 << QKeySequence::DeleteEndOfWord
            << QStringLiteral("Some ") << 5;
    BOBUIest::newRow("Delete start, partial word selection")
            << QStringLiteral("Some Text") << 5 << 7 << 0 << QKeySequence::DeleteStartOfWord
            << QStringLiteral("Some xt") << 5;
    BOBUIest::newRow("Delete end, partial word selection")
            << QStringLiteral("Some Text") << 5 << 7 << 0 << QKeySequence::DeleteEndOfWord
            << QStringLiteral("Some xt") << 5;
    BOBUIest::newRow("Delete start, partial inner word selection")
            << QStringLiteral("Some Text") << 6 << 8 << 0 << QKeySequence::DeleteStartOfWord
            << QStringLiteral("Some Tt") << 6;
    BOBUIest::newRow("Delete end, partial inner word selection")
            << QStringLiteral("Some Text") << 6 << 8 << 0 << QKeySequence::DeleteEndOfWord
            << QStringLiteral("Some Tt") << 6;
    BOBUIest::newRow("Delete start, selection with space")
            << QStringLiteral("Some Text") << 3 << 9 << 0 << QKeySequence::DeleteStartOfWord
            << QStringLiteral("Som") << 3;
    BOBUIest::newRow("Delete end, selection with space")
            << QStringLiteral("Some Text") << 3 << 9 << 0 << QKeySequence::DeleteEndOfWord
            << QStringLiteral("Som") << 3;
    BOBUIest::newRow("Delete start, partial word selection with space")
            << QStringLiteral("Some Text") << 3 << 7 << 0 << QKeySequence::DeleteStartOfWord
            << QStringLiteral("Somxt") << 3;
    BOBUIest::newRow("Delete end, partial selection with space")
            << QStringLiteral("Some Text") << 3 << 7 << 0 << QKeySequence::DeleteEndOfWord
            << QStringLiteral("Somxt") << 3;
}

void tst_QLineEdit::deleteWordByKeySequence()
{
    QFETCH(QString, startText);
    QFETCH(int, selectionStart);
    QFETCH(int, selectionEnd);
    QFETCH(int, cursorPosition);
    QFETCH(QKeySequence::StandardKey, key);
    QFETCH(QString, expectedText);
    QFETCH(int, expectedCursorPosition);

    QWidget widget;

    QLineEdit *lineEdit = new QLineEdit(startText, &widget);
    lineEdit->setFocus();
    lineEdit->setCursorPosition(cursorPosition);
    if (selectionStart != selectionEnd)
        lineEdit->setSelection(selectionStart, selectionEnd - selectionStart);

    widget.show();
    QVERIFY(BOBUIest::qWaitForWindowFocused(&widget));
    QVERIFY(lineEdit->hasFocus());

    BOBUIestEventList keys;
    addKeySequenceStandardKey(keys, key);
    keys.simulate(lineEdit);

    QCOMPARE(lineEdit->text(), expectedText);
    QCOMPARE(lineEdit->selectionStart(), -1);
    QCOMPARE(lineEdit->selectionEnd(), -1);
    QCOMPARE(lineEdit->cursorPosition(), expectedCursorPosition);
}

#endif // BOBUI_CONFIG(shortcut)

BOBUIEST_MAIN(tst_QLineEdit)
#include "tst_qlineedit.moc"
