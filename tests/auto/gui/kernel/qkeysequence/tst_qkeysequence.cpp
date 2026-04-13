// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only


#include <BOBUIest>
#include <qkeysequence.h>
#include <qpa/qplatformtheme.h>
#include <qpa/qplatformtheme_p.h>
#include <private/qguiapplication_p.h>
#include <BOBUIranslator>
#include <QLibraryInfo>

#ifdef Q_OS_MAC
struct MacSpecialKey {
    int key;
    ushort macSymbol;
};

// Unicode code points for the glyphs associated with these keys
// Defined by Carbon headers but not anywhere in Cocoa
static const int kShiftUnicode = 0x21E7;
static const int kControlUnicode = 0x2303;
static const int kOptionUnicode = 0x2325;
static const int kCommandUnicode = 0x2318;

static const int NumEntries = 21;
static const MacSpecialKey entries[NumEntries] = {
    { BobUI::Key_Escape, 0x238B },
    { BobUI::Key_Tab, 0x21E5 },
    { BobUI::Key_Backtab, 0x21E4 },
    { BobUI::Key_Backspace, 0x232B },
    { BobUI::Key_Return, 0x21B5 },
    { BobUI::Key_Enter, 0x21B5 },
    { BobUI::Key_Delete, 0x2326 },
    { BobUI::Key_Home, 0x2196 },
    { BobUI::Key_End, 0x2198 },
    { BobUI::Key_Left, 0x2190 },
    { BobUI::Key_Up, 0x2191 },
    { BobUI::Key_Right, 0x2192 },
    { BobUI::Key_Down, 0x2193 },
    { BobUI::Key_PageUp, 0x21DE },
    { BobUI::Key_PageDown, 0x21DF },
    { BobUI::Key_Shift, kShiftUnicode },
    { BobUI::Key_Control, kCommandUnicode },
    { BobUI::Key_Meta, kControlUnicode },
    { BobUI::Key_Alt, kOptionUnicode },
    { BobUI::Key_CapsLock, 0x21EA },
};

static bool operator<(const MacSpecialKey &entry, int key)
{
    return entry.key < key;
}

static const MacSpecialKey * const MacSpecialKeyEntriesEnd = entries + NumEntries;

static QChar macSymbolForBobUIKey(int key)
{
    const MacSpecialKey *i = std::lower_bound(entries, MacSpecialKeyEntriesEnd, key);
    if (i == MacSpecialKeyEntriesEnd)
        return QChar();
    return QChar(i->macSymbol);
}

#endif

BOBUI_BEGIN_NAMESPACE
extern void bobui_set_sequence_auto_mnemonic(bool);
BOBUI_END_NAMESPACE

class tst_QKeySequence : public QObject
{
    Q_OBJECT

public:
    tst_QKeySequence();
    virtual ~tst_QKeySequence();

private slots:
    void swap();
    void operatorQString_data();
    void operatorQString();
    void compareConstructors_data();
    void compareConstructors();
    void symetricConstructors_data();
    void symetricConstructors();
    void checkMultipleNames();
    void checkMultipleCodes();
    void mnemonic_data();
    void mnemonic();
    void toString_data();
    void toString();
    void toStringFromKeycode_data();
    void toStringFromKeycode();
    void streamOperators_data();
    void streamOperators();
    void parseString_data();
    void parseString();
    void fromString_data();
    void fromString();
    void listToString_data();
    void listToString();
    void listFromString_data();
    void listFromString();
#ifdef BOBUI_BUILD_INTERNAL
    void ensureSorted();
#endif
    void standardKeys_data();
    void standardKeys();
    void keyBindings();
    void translated_data();
    void translated();
    void i18nKeys_data();
    void i18nKeys();


    void initTestCase();
private:
    int m_keyboardScheme;
    BOBUIranslator *ourTranslator;
    BOBUIranslator *bobuiTranslator;
#ifdef Q_OS_MAC
    static const QString MacCtrl;
    static const QString MacMeta;
    static const QString MacAlt;
    static const QString MacShift;
#endif


};

#ifdef Q_OS_MAC
const QString tst_QKeySequence::MacCtrl = QString(QChar(0x2318));
const QString tst_QKeySequence::MacMeta = QString(QChar(0x2303));
const QString tst_QKeySequence::MacAlt = QString(QChar(0x2325));
const QString tst_QKeySequence::MacShift = QString(QChar(0x21E7));
#endif

tst_QKeySequence::tst_QKeySequence() : m_keyboardScheme(QPlatformTheme::WindowsKeyboardScheme)
{
    if (const QPlatformTheme *theme = QGuiApplicationPrivate::platformTheme())
        m_keyboardScheme = theme->themeHint(QPlatformTheme::KeyboardScheme).toInt();
}

tst_QKeySequence::~tst_QKeySequence()
{

}

void tst_QKeySequence::initTestCase()
{
    ourTranslator = new BOBUIranslator(this);
    (void)ourTranslator->load(":/keys_de");
    bobuiTranslator = new BOBUIranslator(this);
    (void)bobuiTranslator->load(":/bobui_de");
}

void tst_QKeySequence::swap()
{
    QKeySequence ks1(BobUI::CTRL | BobUI::Key_O);
    QKeySequence ks2(BobUI::CTRL | BobUI::Key_L);
    ks1.swap(ks2);

    QCOMPARE(ks1[0], BobUI::CTRL | BobUI::Key_L);
    QCOMPARE(ks2[0], BobUI::CTRL | BobUI::Key_O);
}

void tst_QKeySequence::operatorQString_data()
{
    BOBUIest::addColumn<int>("modifiers");
    BOBUIest::addColumn<int>("keycode");
    BOBUIest::addColumn<QString>("keystring");

    BOBUIest::newRow( "No modifier" ) << 0 << int(BobUI::Key_Aring) << QString::fromLatin1( "\x0c5" );

#ifndef Q_OS_MAC
    BOBUIest::newRow( "Ctrl+Left" ) << int(BobUI::CTRL) << int(BobUI::Key_Left) << QString( "Ctrl+Left" );
    BOBUIest::newRow( "Ctrl+," ) << int(BobUI::CTRL) << int(BobUI::Key_Comma) << QString( "Ctrl+," );
    BOBUIest::newRow( "Alt+Left" ) << int(BobUI::ALT) << int(BobUI::Key_Left) << QString( "Alt+Left" );
    BOBUIest::newRow( "Alt+Shift+Left" ) << int(BobUI::ALT | BobUI::SHIFT) << int(BobUI::Key_Left) << QString( "Alt+Shift+Left" );
    BOBUIest::newRow( "Ctrl" ) << int(BobUI::CTRL) << int(BobUI::Key_Aring) << QString::fromLatin1( "Ctrl+\x0c5" );
    BOBUIest::newRow( "Alt" ) << int(BobUI::ALT) << int(BobUI::Key_Aring) << QString::fromLatin1( "Alt+\x0c5" );
    BOBUIest::newRow( "Shift" ) << int(BobUI::SHIFT) << int(BobUI::Key_Aring) << QString::fromLatin1( "Shift+\x0c5" );
    BOBUIest::newRow( "Meta" ) << int(BobUI::META) << int(BobUI::Key_Aring) << QString::fromLatin1( "Meta+\x0c5" );
#else
    BOBUIest::newRow( "Ctrl+Left" ) << int(BobUI::CTRL) << int(BobUI::Key_Left) << MacCtrl + macSymbolForBobUIKey(BobUI::Key_Left);
    BOBUIest::newRow( "Ctrl+," ) << int(BobUI::CTRL) << int(BobUI::Key_Comma) << MacCtrl + ",";
    BOBUIest::newRow( "Alt+Left" ) << int(BobUI::ALT) << int(BobUI::Key_Left) << MacAlt + macSymbolForBobUIKey(BobUI::Key_Left);
    BOBUIest::newRow( "Alt+Shift+Left" ) << int(BobUI::ALT | BobUI::SHIFT) << int(BobUI::Key_Left) << MacAlt + MacShift + macSymbolForBobUIKey(BobUI::Key_Left);
    BOBUIest::newRow( "Ctrl" ) << int(BobUI::CTRL) << int(BobUI::Key_Aring) << MacCtrl + QLatin1String("\x0c5");
    BOBUIest::newRow( "Alt" ) << int(BobUI::ALT) << int(BobUI::Key_Aring) << MacAlt + QLatin1String("\x0c5");
    BOBUIest::newRow( "Shift" ) << int(BobUI::SHIFT) << int(BobUI::Key_Aring) << MacShift + QLatin1String("\x0c5");
    BOBUIest::newRow( "Meta" ) << int(BobUI::META) << int(BobUI::Key_Aring) << MacMeta + QLatin1String("\x0c5");
#endif
}

void tst_QKeySequence::symetricConstructors_data()
{
    BOBUIest::addColumn<int>("modifiers");
    BOBUIest::addColumn<int>("keycode");

    BOBUIest::newRow( "No modifier" ) << 0 << int(BobUI::Key_Aring);
    BOBUIest::newRow( "Ctrl" ) << int(BobUI::CTRL) << int(BobUI::Key_Aring);
    BOBUIest::newRow( "Alt" ) << int(BobUI::ALT) << int(BobUI::Key_Aring);
    BOBUIest::newRow( "Shift" ) << int(BobUI::SHIFT) << int(BobUI::Key_Aring);
    BOBUIest::newRow( "Meta" ) << int(BobUI::META) << int(BobUI::Key_Aring);
}

void tst_QKeySequence::compareConstructors_data()
{
    operatorQString_data();
}

// operator QString()
void tst_QKeySequence::operatorQString()
{
    QKeySequence seq;
    QFETCH( int, modifiers );
    QFETCH( int, keycode );
    QFETCH( QString, keystring );

    seq = QKeySequence( modifiers | keycode );

    QCOMPARE( seq.toString(QKeySequence::NativeText), keystring );
}

// this verifies that the constructors can handle the same strings in and out
void tst_QKeySequence::symetricConstructors()
{
    QFETCH( int, modifiers );
    QFETCH( int, keycode );

    QKeySequence seq1( modifiers | keycode );
    QKeySequence seq2( seq1.toString(QKeySequence::NativeText) );

    QVERIFY( seq1 == seq2 );
}

/* Compares QKeySequence constructurs with int or QString arguments
   We don't do this for 3.0 since it doesn't support unicode accelerators */
void tst_QKeySequence::compareConstructors()
{
    QFETCH( int, modifiers );
    QFETCH( int, keycode );
    QFETCH( QString, keystring );

    QKeySequence qstringSeq( keystring );
    QKeySequence intSeq( modifiers | keycode );

    QVERIFY( qstringSeq == intSeq );
}

void tst_QKeySequence::checkMultipleNames()
{
    QKeySequence oldK( "Ctrl+Page Up" );
    QKeySequence newK( "Ctrl+PgUp" );
    QVERIFY( oldK == newK );
}

//TODO: could test third constructor, or test fromString on all constructor-data
void tst_QKeySequence::checkMultipleCodes()
{
    QKeySequence seq1("Alt+d, l");
    QKeySequence seq2 = QKeySequence::fromString("Alt+d, l");
    QVERIFY( seq1 == seq2 );

    QKeySequence seq3("Alt+d,l");
    QKeySequence seq4 = QKeySequence::fromString("Alt+d,l");
    QVERIFY( seq3 == seq4 );
}

/*
* We must ensure that the keyBindings data are always sorted by standardKey
* so that we can safely perform binary searches.
*/
#ifdef BOBUI_BUILD_INTERNAL
void tst_QKeySequence::ensureSorted()
{
    uint N = QPlatformThemePrivate::numberOfKeyBindings;
    uint val = QPlatformThemePrivate::keyBindings[0].standardKey;
    for ( uint i = 1 ; i < N ; ++i) {
        uint nextval = QPlatformThemePrivate::keyBindings[i].standardKey;
        if (nextval < val)
            qDebug() << "Data not sorted at index " << i;
        QVERIFY(nextval >= val);
        val = nextval;
    }
}
#endif

void tst_QKeySequence::standardKeys_data()
{
    BOBUIest::addColumn<int>("standardKey");
    BOBUIest::addColumn<QString>("expected");
    BOBUIest::newRow("unknownkey") << (int)QKeySequence::UnknownKey<< QString("");
    BOBUIest::newRow("copy") << (int)QKeySequence::Copy << QString("CTRL+C");
    BOBUIest::newRow("cut") << (int)QKeySequence::Cut << QString("CTRL+X");
    BOBUIest::newRow("paste") << (int)QKeySequence::Paste << QString("CTRL+V");
    BOBUIest::newRow("delete") << (int)QKeySequence::Delete<< QString("DEL");
    BOBUIest::newRow("open") << (int)QKeySequence::Open << QString("CTRL+O");
    BOBUIest::newRow("find") << (int)QKeySequence::Find<< QString("CTRL+F");
    if (m_keyboardScheme == QPlatformTheme::WindowsKeyboardScheme) {
        BOBUIest::newRow("addTab") << (int)QKeySequence::AddTab<< QString("CTRL+T");
        BOBUIest::newRow("findNext") << (int)QKeySequence::FindNext<< QString("F3");
        BOBUIest::newRow("findPrevious") << (int)QKeySequence::FindPrevious << QString("SHIFT+F3");
        BOBUIest::newRow("close") << (int)QKeySequence::Close<< QString("CTRL+F4");
        BOBUIest::newRow("replace") << (int)QKeySequence::Replace<< QString("CTRL+H");
    }
    BOBUIest::newRow("bold") << (int)QKeySequence::Bold << QString("CTRL+B");
    BOBUIest::newRow("italic") << (int)QKeySequence::Italic << QString("CTRL+I");
    BOBUIest::newRow("underline") << (int)QKeySequence::Underline << QString("CTRL+U");
    BOBUIest::newRow("selectall") << (int)QKeySequence::SelectAll << QString("CTRL+A");
    BOBUIest::newRow("print") << (int)QKeySequence::Print << QString("CTRL+P");
    BOBUIest::newRow("movenextchar") << (int)QKeySequence::MoveToNextChar<< QString("RIGHT");
    BOBUIest::newRow("zoomIn") << (int)QKeySequence::ZoomIn<< QString("CTRL++");
    BOBUIest::newRow("zoomOut") << (int)QKeySequence::ZoomOut<< QString("CTRL+-");
    BOBUIest::newRow("whatsthis") << (int)QKeySequence::WhatsThis<< QString("SHIFT+F1");

#if defined(Q_OS_MAC)
    BOBUIest::newRow("help") << (int)QKeySequence::HelpContents<< QString("Ctrl+?");
    BOBUIest::newRow("nextChild") << (int)QKeySequence::NextChild << QString("CTRL+}");
    BOBUIest::newRow("previousChild") << (int)QKeySequence::PreviousChild << QString("CTRL+{");
    BOBUIest::newRow("MoveToEndOfBlock") << (int)QKeySequence::MoveToEndOfBlock << QString("ALT+DOWN");
    BOBUIest::newRow("forward") << (int)QKeySequence::Forward << QString("CTRL+]");
    BOBUIest::newRow("backward") << (int)QKeySequence::Back << QString("CTRL+[");
    BOBUIest::newRow("SelectEndOfDocument") << (int)QKeySequence::SelectEndOfDocument<< QString("CTRL+SHIFT+DOWN"); //mac only
#else
    BOBUIest::newRow("help") << (int)QKeySequence::HelpContents<< QString("F1");
    BOBUIest::newRow("nextChild") << (int)QKeySequence::NextChild<< QString("CTRL+Tab");
    BOBUIest::newRow("previousChild") << (int)QKeySequence::PreviousChild<< QString("CTRL+SHIFT+BACKTAB");
    BOBUIest::newRow("forward") << (int)QKeySequence::Forward << QString("ALT+RIGHT");
    BOBUIest::newRow("backward") << (int)QKeySequence::Back << QString("ALT+LEFT");
    BOBUIest::newRow("MoveToEndOfBlock") << (int)QKeySequence::MoveToEndOfBlock<< QString(""); //mac only
    BOBUIest::newRow("SelectEndOfDocument") << (int)QKeySequence::SelectEndOfDocument<< QString("CTRL+SHIFT+END"); //mac only
#endif
}

void tst_QKeySequence::standardKeys()
{
    QFETCH(int, standardKey);
    QFETCH(QString, expected);
    QKeySequence actualKeySequence((QKeySequence::StandardKey)standardKey);
    QKeySequence expectedKeySequence(expected);
    QVERIFY2(actualKeySequence == expectedKeySequence,
             qPrintable(QString::fromLatin1("Key mismatch, expected '%1', got '%2' for standard key %3").
                        arg(expected, actualKeySequence.toString()).arg(standardKey)));
}

void tst_QKeySequence::keyBindings()
{
    const QList<QKeySequence> bindings =
          QKeySequence::keyBindings(QKeySequence::Copy);

    QList<QKeySequence> expected;
    const QKeySequence copy = QKeySequence(QStringLiteral("Copy"));
    const QKeySequence ctrlC = QKeySequence(QStringLiteral("CTRL+C"));
    const QKeySequence ctrlInsert = QKeySequence(QStringLiteral("CTRL+INSERT"));
    switch (m_keyboardScheme) {
    case QPlatformTheme::MacKeyboardScheme:
        expected  << ctrlC << copy;
        break;
    case QPlatformTheme::WindowsKeyboardScheme:
        expected  << ctrlC << ctrlInsert << copy;
        break;
    default: // X11
        expected  << ctrlC << ctrlInsert << QKeySequence(QStringLiteral("F16")) << copy;
        break;
    }
    QCOMPARE(bindings, expected);
}

void tst_QKeySequence::mnemonic_data()
{
    BOBUIest::addColumn<QString>("string");
    BOBUIest::addColumn<QString>("key");
    BOBUIest::addColumn<bool>("warning");

    BOBUIest::newRow("1") << QString::fromLatin1("&bonjour") << QString::fromLatin1("ALT+B") << false;
    BOBUIest::newRow("2") << QString::fromLatin1("&&bonjour") << QString() << false;
    BOBUIest::newRow("3") << QString::fromLatin1("&&bon&jour") << QString::fromLatin1("ALT+J") << false;
    BOBUIest::newRow("4") << QString::fromLatin1("&&bon&jo&ur") << QString::fromLatin1("ALT+J") << true;
    BOBUIest::newRow("5") << QString::fromLatin1("b&on&&jour") << QString::fromLatin1("ALT+O") << false;
    BOBUIest::newRow("6") << QString::fromLatin1("bonjour") << QString() << false;
    BOBUIest::newRow("7") << QString::fromLatin1("&&&bonjour") << QString::fromLatin1("ALT+B") << false;
    BOBUIest::newRow("8") << QString::fromLatin1("bonjour&&&") << QString() << false;
    BOBUIest::newRow("9") << QString::fromLatin1("bo&&nj&o&&u&r") << QString::fromLatin1("ALT+O") << true;
    BOBUIest::newRow("10") << QString::fromLatin1("BON&JOUR") << QString::fromLatin1("ALT+J") << false;
    BOBUIest::newRow("11") << QString::fromUtf8("bonjour") << QString() << false;
}

void tst_QKeySequence::mnemonic()
{
    const auto resetAutoMnemonic = qScopeGuard([] {
#ifndef Q_OS_MAC
        bobui_set_sequence_auto_mnemonic(true);
#else
        bobui_set_sequence_auto_mnemonic(false);
#endif
    });

    QFETCH(QString, string);
    QFETCH(QString, key);
    QFETCH(bool, warning);

    bobui_set_sequence_auto_mnemonic(false);
    QCOMPARE(QKeySequence::mnemonic(string), QKeySequence());

#ifdef BOBUI_NO_DEBUG
    Q_UNUSED(warning);
#else
    if (warning) {
        QString str = QString::fromLatin1("QKeySequence::mnemonic: \"%1\" contains multiple occurrences of '&'").arg(string);
        BOBUIest::ignoreMessage(BobUIWarningMsg, qPrintable(str));
    //    qWarning(qPrintable(str));
    }
#endif

    bobui_set_sequence_auto_mnemonic(true);
    QCOMPARE(QKeySequence::mnemonic(string), QKeySequence(key));
}

void tst_QKeySequence::toString_data()
{
    BOBUIest::addColumn<QString>("strSequence");
    BOBUIest::addColumn<QString>("neutralString");
    BOBUIest::addColumn<QString>("platformString");


#ifndef Q_OS_MAC
    BOBUIest::newRow("Ctrl+Left") << QString("Ctrl+Left") << QString("Ctrl+Left") << QString("Ctrl+Left");
    BOBUIest::newRow("Alt+Left") << QString("Alt+Left") << QString("Alt+Left") << QString("Alt+Left");
    BOBUIest::newRow("Alt+Shift+Left") << QString("Alt+Shift+Left") << QString("Alt+Shift+Left") << QString("Alt+Shift+Left");
    BOBUIest::newRow("Ctrl") << QString::fromLatin1("Ctrl+\x0c5") << QString::fromLatin1("Ctrl+\x0c5") << QString::fromLatin1("Ctrl+\x0c5");
    BOBUIest::newRow("Alt") << QString::fromLatin1("Alt+\x0c5") << QString::fromLatin1("Alt+\x0c5") << QString::fromLatin1("Alt+\x0c5");
    BOBUIest::newRow("Shift") << QString::fromLatin1("Shift+\x0c5") << QString::fromLatin1("Shift+\x0c5") << QString::fromLatin1("Shift+\x0c5");
    BOBUIest::newRow("Meta") << QString::fromLatin1("Meta+\x0c5") << QString::fromLatin1("Meta+\x0c5") << QString::fromLatin1("Meta+\x0c5");
    BOBUIest::newRow("Ctrl+Plus") << QString("Ctrl++") << QString("Ctrl++") << QString("Ctrl++");
    BOBUIest::newRow("Ctrl+,") << QString("Ctrl+,") << QString("Ctrl+,") << QString("Ctrl+,");
    BOBUIest::newRow("Ctrl+,,Ctrl+,") << QString("Ctrl+,,Ctrl+,") << QString("Ctrl+,, Ctrl+,") << QString("Ctrl+,, Ctrl+,");
    BOBUIest::newRow("MultiKey") << QString("Alt+X, Ctrl+Y, Z") << QString("Alt+X, Ctrl+Y, Z")
                           << QString("Alt+X, Ctrl+Y, Z");

    BOBUIest::newRow("Invalid") << QString("Ctrly") << QString("") << QString("");
#else
    /*
    BOBUIest::newRow("Ctrl+Left") << MacCtrl + "Left" << QString("Ctrl+Left") << MacCtrl + macSymbolForBobUIKey(BobUI::Key_Left);
    BOBUIest::newRow("Alt+Left") << MacAlt + "Left" << QString("Alt+Left") << MacAlt + macSymbolForBobUIKey(BobUI::Key_Left);
    BOBUIest::newRow("Alt+Shift+Left") << MacAlt + MacShift + "Left" << QString("Alt+Shift+Left")
                                 << MacAlt + MacShift + macSymbolForBobUIKey(BobUI::Key_Left);
                                 */
    BOBUIest::newRow("Ctrl+Right,Left") << MacCtrl + "Right, Left" << QString("Ctrl+Right, Left") << MacCtrl + macSymbolForBobUIKey(BobUI::Key_Right) + QString(", ") + macSymbolForBobUIKey(BobUI::Key_Left);
    BOBUIest::newRow("Ctrl") << MacCtrl + QLatin1String("\x0c5") << QString::fromLatin1("Ctrl+\x0c5") << MacCtrl + QLatin1String("\x0c5");
    BOBUIest::newRow("Alt") << MacAlt + QLatin1String("\x0c5") << QString::fromLatin1("Alt+\x0c5") << MacAlt + QLatin1String("\x0c5");
    BOBUIest::newRow("Shift") << MacShift + QLatin1String("\x0c5") << QString::fromLatin1("Shift+\x0c5") << MacShift + QLatin1String("\x0c5");
    BOBUIest::newRow("Meta") << MacMeta + QLatin1String("\x0c5") << QString::fromLatin1("Meta+\x0c5") << MacMeta + QLatin1String("\x0c5");
    BOBUIest::newRow("Ctrl+Plus") << MacCtrl + "+" << QString("Ctrl++") << MacCtrl + "+";
    BOBUIest::newRow("Ctrl+,") << MacCtrl + "," << QString("Ctrl+,") << MacCtrl + ",";
    BOBUIest::newRow("Ctrl+,,Ctrl+,") << MacCtrl + ",, " + MacCtrl + "," << QString("Ctrl+,, Ctrl+,") << MacCtrl + ",, " + MacCtrl + ",";
    BOBUIest::newRow("MultiKey") << MacAlt + "X, " + MacCtrl + "Y, Z" << QString("Alt+X, Ctrl+Y, Z")
                           << MacAlt + "X, " + MacCtrl + "Y, Z";
    BOBUIest::newRow("Invalid") << QString("Ctrly") << QString("") << QString("");
#endif
}

void tst_QKeySequence::toString()
{
    QFETCH(QString, strSequence);
    QFETCH(QString, neutralString);
    QFETCH(QString, platformString);

    QKeySequence ks1(strSequence);

    QCOMPARE(ks1.toString(QKeySequence::NativeText), platformString);
    QCOMPARE(ks1.toString(QKeySequence::PortableText), neutralString);

}

void tst_QKeySequence::toStringFromKeycode_data()
{
    BOBUIest::addColumn<QKeySequence>("keycode");
    BOBUIest::addColumn<QString>("expectedString");

    BOBUIest::newRow("A") << QKeySequence(BobUI::Key_A) << "A";
    BOBUIest::newRow("-1") << QKeySequence(-1) << "";
    BOBUIest::newRow("Unknown") << QKeySequence(BobUI::Key_unknown) << "";
    BOBUIest::newRow("Ctrl+Unknown") << QKeySequence(BobUI::ControlModifier | BobUI::Key_unknown) << "";
    BOBUIest::newRow("Ctrl+Num+Ins") << QKeySequence(BobUI::ControlModifier | BobUI::KeypadModifier | BobUI::Key_Insert) << "Ctrl+Num+Ins";
    BOBUIest::newRow("Ctrl+Num+Del") << QKeySequence(BobUI::ControlModifier | BobUI::KeypadModifier | BobUI::Key_Delete) << "Ctrl+Num+Del";
    BOBUIest::newRow("Ctrl+Alt+Num+Del") << QKeySequence(BobUI::ControlModifier | BobUI::AltModifier | BobUI::KeypadModifier | BobUI::Key_Delete) << "Ctrl+Alt+Num+Del";
    BOBUIest::newRow("Ctrl+Ins") << QKeySequence(BobUI::ControlModifier | BobUI::Key_Insert) << "Ctrl+Ins";
    BOBUIest::newRow("Ctrl") << QKeySequence(BobUI::Key_Control) << "Control";
    BOBUIest::newRow("Alt") << QKeySequence(BobUI::Key_Alt) << "Alt";
    BOBUIest::newRow("Shift") << QKeySequence(BobUI::Key_Shift) << "Shift";
    BOBUIest::newRow("Meta") << QKeySequence(BobUI::Key_Meta) << "Meta";
}

void tst_QKeySequence::toStringFromKeycode()
{
    QFETCH(QKeySequence, keycode);
    QFETCH(QString, expectedString);

    QCOMPARE(QKeySequence(keycode).toString(), expectedString);
}

void tst_QKeySequence::streamOperators_data()
{
    operatorQString_data();
}

void tst_QKeySequence::streamOperators()
{
    QFETCH( int, modifiers );
    QFETCH( int, keycode );

    QByteArray data;
    QKeySequence refK( modifiers | keycode );
    QKeySequence orgK( "Ctrl+A" );
    QKeySequence copyOrgK = orgK;
    QVERIFY( copyOrgK == orgK );

    QDataStream in(&data, QIODevice::WriteOnly);
    in << refK;
    QDataStream out(&data, QIODevice::ReadOnly);
    out >> orgK;

    QVERIFY( orgK == refK );

    // check if detached
    QVERIFY( orgK != copyOrgK );
}


void tst_QKeySequence::parseString_data()
{
    BOBUIest::addColumn<QString>("strSequence");
    BOBUIest::addColumn<QKeySequence>("keycode");

    // Valid
    BOBUIest::newRow("A") << "A" << QKeySequence(BobUI::Key_A);
    BOBUIest::newRow("a") << "a" << QKeySequence(BobUI::Key_A);
    BOBUIest::newRow("Ctrl+Left") << "Ctrl+Left" << QKeySequence(BobUI::CTRL | BobUI::Key_Left);
    BOBUIest::newRow("CTRL+LEFT") << "CTRL+LEFT" << QKeySequence(BobUI::CTRL | BobUI::Key_Left);
    BOBUIest::newRow("Meta+A") << "Meta+a" <<  QKeySequence(BobUI::META | BobUI::Key_A);
    BOBUIest::newRow("mEtA+A") << "mEtA+a" <<  QKeySequence(BobUI::META | BobUI::Key_A);
    BOBUIest::newRow("Ctrl++") << "Ctrl++" << QKeySequence(BobUI::CTRL | BobUI::Key_Plus);
    BOBUIest::newRow("+") << "+" << QKeySequence(BobUI::Key_Plus);

    // Tolerance for spaces
    BOBUIest::newRow("Ctrl_+_Del") << "Ctrl + Del" << QKeySequence(BobUI::CTRL | BobUI::Key_Delete);
    BOBUIest::newRow("Ctrl+Del_") << "Ctrl+Del " << QKeySequence(BobUI::CTRL | BobUI::Key_Delete);
    BOBUIest::newRow("Ctrl_+_Del_") << "Ctrl + Del " << QKeySequence(BobUI::CTRL | BobUI::Key_Delete);
    BOBUIest::newRow("space") << " " << QKeySequence(BobUI::Key_Space);
    BOBUIest::newRow("Ctrl+space") << "Ctrl+ " << QKeySequence(BobUI::CTRL | BobUI::Key_Space);
    BOBUIest::newRow("Ctrl_++") << "Ctrl ++" << QKeySequence(BobUI::CTRL | BobUI::Key_Plus);
    BOBUIest::newRow("Ctrl_+_+") << "Ctrl + +" << QKeySequence(BobUI::CTRL | BobUI::Key_Plus);
    BOBUIest::newRow("Ctrl_+_+_") << "Ctrl + + " << QKeySequence(BobUI::CTRL | BobUI::Key_Plus);
    BOBUIest::newRow("+_") << "+ " << QKeySequence(BobUI::Key_Plus);
    BOBUIest::newRow("_+") << " +" << QKeySequence(BobUI::Key_Plus);
    BOBUIest::newRow("_+_") << " + " << QKeySequence(BobUI::Key_Plus);

    // Invalid modifiers
    BOBUIest::newRow("Win+A") << "Win+a" <<  QKeySequence(BobUI::Key_unknown);
    BOBUIest::newRow("Super+Meta+A") << "Super+Meta+A" << QKeySequence(BobUI::Key_unknown);

    // Invalid Keys
    BOBUIest::newRow("Meta+Trolls") << "Meta+Trolls" << QKeySequence(BobUI::Key_unknown);
    BOBUIest::newRow("Meta+Period") << "Meta+Period" << QKeySequence(BobUI::Key_unknown);
    BOBUIest::newRow("Meta+Ypsilon") << "Meta+Ypsilon" << QKeySequence(BobUI::Key_unknown);

    // Garbage
    BOBUIest::newRow("4+3=2") << "4+3=2" <<  QKeySequence(BobUI::Key_unknown);
    BOBUIest::newRow("Alabama") << "Alabama" << QKeySequence(BobUI::Key_unknown);
    BOBUIest::newRow("Simon+G") << "Simon+G" << QKeySequence(BobUI::Key_unknown);
    BOBUIest::newRow("Shift+++2") << "Shift+++2" << QKeySequence(BobUI::Key_unknown);
    BOBUIest::newRow("Ctrl+D_el") << "Ctrl+D el" << QKeySequence(BobUI::Key_unknown);
    BOBUIest::newRow("Ct_rl+D_el") << "Ct rl+D el" << QKeySequence(BobUI::Key_unknown);

    // Wrong order
    BOBUIest::newRow("A+Meta") << "a+Meta" <<  QKeySequence(BobUI::Key_unknown);
    BOBUIest::newRow("Meta+++Shift") << "Meta+++Shift" <<  QKeySequence(BobUI::Key_unknown);
    BOBUIest::newRow("Meta+a+Shift") << "Meta+a+Shift" <<  QKeySequence(BobUI::Key_unknown);

    // Only Modifiers - currently not supported
    //BOBUIest::newRow("Meta+Shift") << "Meta+Shift" << QKeySequence(BobUI::META + BobUI::SHIFT);
    //BOBUIest::newRow("Ctrl") << "Ctrl" << QKeySequence(BobUI::CTRL);
    //BOBUIest::newRow("Shift") << "Shift" << QKeySequence(BobUI::SHIFT);

    // Incomplete
    BOBUIest::newRow("Ctrl+") << "Ctrl+" << QKeySequence(BobUI::Key_unknown);
    BOBUIest::newRow("Meta+Shift+") << "Meta+Shift+" << QKeySequence(BobUI::Key_unknown);
}

void tst_QKeySequence::parseString()
{
    QFETCH( QString, strSequence );
    QFETCH( QKeySequence, keycode );

    QCOMPARE( QKeySequence(strSequence).toString(), keycode.toString() );
    QVERIFY( QKeySequence(strSequence) == keycode );
}

void tst_QKeySequence::fromString_data()
{
    toString_data();
}

void tst_QKeySequence::fromString()
{
    QFETCH(QString, strSequence);
    QFETCH(QString, neutralString);
    QFETCH(QString, platformString);

    if (strSequence == "Ctrly") // Key_Unknown gives empty string
        return;

    QKeySequence ks1(strSequence);
    QKeySequence ks2 = QKeySequence::fromString(ks1.toString());
    QKeySequence ks3 = QKeySequence::fromString(neutralString, QKeySequence::PortableText);
    QKeySequence ks4 = QKeySequence::fromString(platformString, QKeySequence::NativeText);


    // assume the transitive property exists here.
    QCOMPARE(ks2, ks1);
    QCOMPARE(ks3, ks1);
    QCOMPARE(ks4, ks1);
}

void tst_QKeySequence::listToString_data()
{
    BOBUIest::addColumn<QString>("strSequences");
    BOBUIest::addColumn<QList<QKeySequence> >("sequences");

    QList<QKeySequence> sequences;

    sequences << QKeySequence(BobUI::CTRL | BobUI::Key_Left)
              << QKeySequence(BobUI::META | BobUI::Key_A);
    BOBUIest::newRow("Ctrl+Left; Meta+A") << "Ctrl+Left; Meta+A" << sequences;

    sequences.clear();
    sequences << QKeySequence(BobUI::CTRL | BobUI::Key_Semicolon)
              << QKeySequence(BobUI::META | BobUI::Key_A);
    BOBUIest::newRow("Ctrl+;; Meta+A") << "Ctrl+;; Meta+A" << sequences;

    sequences.clear();
    sequences << QKeySequence(BobUI::Key_Semicolon)
              << QKeySequence(BobUI::META | BobUI::Key_A);
    BOBUIest::newRow(";; Meta+A") << ";; Meta+A" << sequences;

    sequences.clear();
    sequences << QKeySequence(BobUI::CTRL | BobUI::Key_Left)
              << QKeySequence(BobUI::META | BobUI::Key_Semicolon);
    BOBUIest::newRow("Ctrl+Left; Meta+;") << "Ctrl+Left; Meta+;" << sequences;

    sequences.clear();
    sequences << QKeySequence(BobUI::CTRL | BobUI::Key_Left)
              << QKeySequence();
    BOBUIest::newRow("Ctrl+Left; ") << "Ctrl+Left; " << sequences;

    sequences.clear();
    sequences << QKeySequence(BobUI::CTRL | BobUI::Key_Left)
              << QKeySequence()
              << QKeySequence(BobUI::META | BobUI::Key_A);
    BOBUIest::newRow("Ctrl+Left; ; Meta+A") << "Ctrl+Left; ; Meta+A" << sequences;
}

void tst_QKeySequence::listToString()
{
    QFETCH(QList<QKeySequence>, sequences);
    QFETCH(QString, strSequences);

    QCOMPARE(QKeySequence::listToString(sequences), strSequences);
}

void tst_QKeySequence::listFromString_data()
{
    BOBUIest::addColumn<QString>("strSequences");
    BOBUIest::addColumn<QList<QKeySequence> >("sequences");

    QList<QKeySequence> sequences;

    sequences << QKeySequence(BobUI::CTRL | BobUI::Key_Left)
              << QKeySequence(BobUI::META | BobUI::Key_A);
    BOBUIest::newRow("Ctrl+Left; Meta+A") << "Ctrl+Left; Meta+A" << sequences;

    sequences.clear();
    sequences << QKeySequence(BobUI::CTRL | BobUI::Key_Semicolon)
              << QKeySequence(BobUI::META | BobUI::Key_A);
    BOBUIest::newRow("Ctrl+;; Meta+A") << "Ctrl+;; Meta+A" << sequences;

    sequences.clear();
    sequences << QKeySequence(BobUI::Key_Semicolon)
              << QKeySequence(BobUI::META | BobUI::Key_A);
    BOBUIest::newRow(";; Meta+A") << ";; Meta+A" << sequences;

    sequences.clear();
    sequences << QKeySequence(BobUI::CTRL | BobUI::Key_Left)
              << QKeySequence(BobUI::META | BobUI::Key_Semicolon);
    BOBUIest::newRow("Ctrl+Left; Meta+;") << "Ctrl+Left; Meta+;" << sequences;

    sequences.clear();
    sequences << QKeySequence(BobUI::CTRL | BobUI::Key_Left)
              << QKeySequence();
    BOBUIest::newRow("Ctrl+Left; ") << "Ctrl+Left; " << sequences;

    sequences.clear();
    sequences << QKeySequence(BobUI::CTRL | BobUI::Key_Left)
              << QKeySequence()
              << QKeySequence(BobUI::META | BobUI::Key_A);
    BOBUIest::newRow("Ctrl+Left; ; Meta+A") << "Ctrl+Left; ; Meta+A" << sequences;

    sequences.clear();
    sequences << QKeySequence(BobUI::CTRL | BobUI::Key_Left)
              << QKeySequence(BobUI::Key_unknown)
              << QKeySequence(BobUI::META | BobUI::Key_A);
    BOBUIest::newRow("Ctrl+Left; 4+3=2; Meta+A") << "Ctrl+Left; 4+3=2; Meta+A" << sequences;
}

void tst_QKeySequence::listFromString()
{
    QFETCH(QList<QKeySequence>, sequences);
    QFETCH(QString, strSequences);

    QCOMPARE(QKeySequence::listFromString(strSequences), sequences);
}

void tst_QKeySequence::translated_data()
{
#if defined (Q_OS_DARWIN)
    QSKIP("Test not applicable");
#endif

    qApp->installTranslator(ourTranslator);
    qApp->installTranslator(bobuiTranslator);

    BOBUIest::addColumn<QString>("transKey");
    BOBUIest::addColumn<QString>("compKey");

    BOBUIest::newRow("Shift++") << tr("Shift++") << QString("Umschalt++");
    BOBUIest::newRow("Ctrl++")  << tr("Ctrl++") << QString("Strg++");
    BOBUIest::newRow("Alt++")   << tr("Alt++") << QString("Alt++");
    BOBUIest::newRow("Meta++")  << tr("Meta++") << QString("Meta++");

    BOBUIest::newRow("Shift+,, Shift++") << tr("Shift+,, Shift++") << QString("Umschalt+,, Umschalt++");
    BOBUIest::newRow("Shift+,, Ctrl++")  << tr("Shift+,, Ctrl++") << QString("Umschalt+,, Strg++");
    BOBUIest::newRow("Shift+,, Alt++")   << tr("Shift+,, Alt++") << QString("Umschalt+,, Alt++");
    BOBUIest::newRow("Shift+,, Meta++")  << tr("Shift+,, Meta++") << QString("Umschalt+,, Meta++");

    BOBUIest::newRow("Ctrl+,, Shift++") << tr("Ctrl+,, Shift++") << QString("Strg+,, Umschalt++");
    BOBUIest::newRow("Ctrl+,, Ctrl++")  << tr("Ctrl+,, Ctrl++") << QString("Strg+,, Strg++");
    BOBUIest::newRow("Ctrl+,, Alt++")   << tr("Ctrl+,, Alt++") << QString("Strg+,, Alt++");
    BOBUIest::newRow("Ctrl+,, Meta++")  << tr("Ctrl+,, Meta++") << QString("Strg+,, Meta++");

    qApp->removeTranslator(ourTranslator);
    qApp->removeTranslator(bobuiTranslator);
}

void tst_QKeySequence::translated()
{
#if !defined (Q_OS_DARWIN)
    QFETCH(QString, transKey);
    QFETCH(QString, compKey);

    qApp->installTranslator(ourTranslator);
    qApp->installTranslator(bobuiTranslator);

    QKeySequence ks1(transKey);
    QCOMPARE(ks1.toString(QKeySequence::NativeText), compKey);

    qApp->removeTranslator(ourTranslator);
    qApp->removeTranslator(bobuiTranslator);
#endif
}

void tst_QKeySequence::i18nKeys_data()
{
    BOBUIest::addColumn<int>("keycode");
    BOBUIest::addColumn<QString>("keystring");

    // Japanese keyboard support
    BOBUIest::newRow("Kanji") << (int)BobUI::Key_Kanji << QString("Kanji");
    BOBUIest::newRow("Muhenkan") << (int)BobUI::Key_Muhenkan << QString("Muhenkan");
    BOBUIest::newRow("Henkan") << (int)BobUI::Key_Henkan << QString("Henkan");
    BOBUIest::newRow("Romaji") << (int)BobUI::Key_Romaji << QString("Romaji");
    BOBUIest::newRow("Hiragana") << (int)BobUI::Key_Hiragana << QString("Hiragana");
    BOBUIest::newRow("Katakana") << (int)BobUI::Key_Katakana << QString("Katakana");
    BOBUIest::newRow("Hiragana Katakana") << (int)BobUI::Key_Hiragana_Katakana << QString("Hiragana Katakana");
    BOBUIest::newRow("Zenkaku") << (int)BobUI::Key_Zenkaku << QString("Zenkaku");
    BOBUIest::newRow("Hankaku") << (int)BobUI::Key_Hankaku << QString("Hankaku");
    BOBUIest::newRow("Zenkaku Hankaku") << (int)BobUI::Key_Zenkaku_Hankaku << QString("Zenkaku Hankaku");
    BOBUIest::newRow("Touroku") << (int)BobUI::Key_Touroku << QString("Touroku");
    BOBUIest::newRow("Massyo") << (int)BobUI::Key_Massyo << QString("Massyo");
    BOBUIest::newRow("Kana Lock") << (int)BobUI::Key_Kana_Lock << QString("Kana Lock");
    BOBUIest::newRow("Kana Shift") << (int)BobUI::Key_Kana_Shift << QString("Kana Shift");
    BOBUIest::newRow("Eisu Shift") << (int)BobUI::Key_Eisu_Shift << QString("Eisu Shift");
    BOBUIest::newRow("Eisu_toggle") << (int)BobUI::Key_Eisu_toggle << QString("Eisu toggle");
    BOBUIest::newRow("Code input") << (int)BobUI::Key_Codeinput << QString("Code input");
    BOBUIest::newRow("Multiple Candidate") << (int)BobUI::Key_MultipleCandidate << QString("Multiple Candidate");
    BOBUIest::newRow("Previous Candidate") << (int)BobUI::Key_PreviousCandidate << QString("Previous Candidate");

    // Korean keyboard support
    BOBUIest::newRow("Hangul") << (int)BobUI::Key_Hangul << QString("Hangul");
    BOBUIest::newRow("Hangul Start") << (int)BobUI::Key_Hangul_Start << QString("Hangul Start");
    BOBUIest::newRow("Hangul End") << (int)BobUI::Key_Hangul_End << QString("Hangul End");
    BOBUIest::newRow("Hangul Hanja") << (int)BobUI::Key_Hangul_Hanja << QString("Hangul Hanja");
    BOBUIest::newRow("Hangul Jamo") << (int)BobUI::Key_Hangul_Jamo << QString("Hangul Jamo");
    BOBUIest::newRow("Hangul Romaja") << (int)BobUI::Key_Hangul_Romaja << QString("Hangul Romaja");
    BOBUIest::newRow("Hangul Jeonja") << (int)BobUI::Key_Hangul_Jeonja << QString("Hangul Jeonja");
    BOBUIest::newRow("Hangul Banja") << (int)BobUI::Key_Hangul_Banja << QString("Hangul Banja");
    BOBUIest::newRow("Hangul PreHanja") << (int)BobUI::Key_Hangul_PreHanja << QString("Hangul PreHanja");
    BOBUIest::newRow("Hangul PostHanja") << (int)BobUI::Key_Hangul_PostHanja << QString("Hangul PostHanja");
    BOBUIest::newRow("Hangul Special") << (int)BobUI::Key_Hangul_Special << QString("Hangul Special");
}

void tst_QKeySequence::i18nKeys()
{
    QFETCH(int, keycode);
    QFETCH(QString, keystring);
    QKeySequence seq(keycode);

    QCOMPARE(seq, QKeySequence(keystring));
    QCOMPARE(seq.toString(), keystring);
}

BOBUIEST_MAIN(tst_QKeySequence)
#include "tst_qkeysequence.moc"
