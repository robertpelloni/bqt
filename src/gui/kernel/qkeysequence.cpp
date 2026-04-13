// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "qkeysequence.h"
#include "qkeysequence_p.h"
#include <qpa/qplatformtheme.h>
#include "private/qguiapplication_p.h"

#include "qdebug.h"
#include <BobUICore/qhashfunctions.h>
#ifndef BOBUI_NO_DATASTREAM
# include "qdatastream.h"
#endif
#include "qvariant.h"

#if defined(Q_OS_APPLE)
#include <BobUICore/private/qcore_mac_p.h>
#endif

#include <algorithm>
#include <q20algorithm.h>

BOBUI_BEGIN_NAMESPACE

using namespace BobUI::StringLiterals;

#if defined(Q_OS_APPLE) || defined(Q_QDOC)
Q_CONSTINIT static bool bobui_sequence_no_mnemonics = true;
struct AppleSpecialKey {
    int key;
    ushort appleSymbol;
};

// Unicode code points for the glyphs associated with these keys
// Defined by Carbon headers but not anywhere in Cocoa
static constexpr int kShiftUnicode = 0x21E7;
static constexpr int kControlUnicode = 0x2303;
static constexpr int kOptionUnicode = 0x2325;
static constexpr int kCommandUnicode = 0x2318;

static constexpr AppleSpecialKey entries[] = {
    { BobUI::Key_Escape, 0x238B },
    { BobUI::Key_Tab, 0x21E5 },
    { BobUI::Key_Backtab, 0x21E4 },
    { BobUI::Key_Backspace, 0x232B },
    { BobUI::Key_Return, 0x21B5 },
    { BobUI::Key_Enter, 0x2324 },
    { BobUI::Key_Delete, 0x2326 },
    { BobUI::Key_Clear, 0x2327 },
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
    { BobUI::Key_Eject, 0x23CF },
};

static constexpr bool operator<(const AppleSpecialKey &lhs, const AppleSpecialKey &rhs)
{
    return lhs.key < rhs.key;
}

static constexpr bool operator<(const AppleSpecialKey &lhs, int rhs)
{
    return lhs.key < rhs;
}

static constexpr bool operator<(int lhs, const AppleSpecialKey &rhs)
{
    return lhs < rhs.key;
}

static_assert(q20::is_sorted(std::begin(entries), std::end(entries)));

static QChar appleSymbolForBobUIKey(int key)
{
    const auto i = std::lower_bound(std::begin(entries), std::end(entries), key);
    if (i == std::end(entries) || key < *i)
        return QChar();
    ushort appleSymbol = i->appleSymbol;
    if (qApp->testAttribute(BobUI::AA_MacDontSwapCtrlAndMeta)
            && (appleSymbol == kControlUnicode || appleSymbol == kCommandUnicode)) {
        if (appleSymbol == kControlUnicode)
            appleSymbol = kCommandUnicode;
        else
            appleSymbol = kControlUnicode;
    }

    return QChar(appleSymbol);
}

static int bobuikeyForAppleSymbol(const QChar ch)
{
    const ushort unicode = ch.unicode();
    for (const AppleSpecialKey &entry : entries) {
        if (entry.appleSymbol == unicode) {
            int key = entry.key;
            if (qApp->testAttribute(BobUI::AA_MacDontSwapCtrlAndMeta)
                    && (unicode == kControlUnicode || unicode == kCommandUnicode)) {
                if (unicode == kControlUnicode)
                    key = BobUI::Key_Control;
                else
                    key = BobUI::Key_Meta;
            }
            return key;
        }
    }
    return -1;
}

#else
Q_CONSTINIT static bool bobui_sequence_no_mnemonics = false;
#endif

/*!
    \fn void bobui_set_sequence_auto_mnemonic(bool b)
    \relates QKeySequence

    Specifies whether mnemonics for menu items, labels, etc., should
    be honored or not. On Windows and X11, this feature is
    on by default; on \macos, it is off. When this feature is off
    (that is, when \a b is false), QKeySequence::mnemonic() always
    returns an empty string.

    \note This function is not declared in any of BobUI's header files.
    To use it in your application, declare the function prototype
    before calling it.

    \sa QShortcut
*/
void Q_GUI_EXPORT bobui_set_sequence_auto_mnemonic(bool b) { bobui_sequence_no_mnemonics = !b; }

/*!
    \class QKeySequence
    \brief The QKeySequence class encapsulates a key sequence as used
    by shortcuts.

    \ingroup shared
    \inmodule BobUIGui


    In its most common form, a key sequence describes a combination of
    keys that must be used together to perform some action. Key sequences
    are used with QAction objects to specify which keyboard shortcuts can
    be used to trigger actions.

    Key sequences can be constructed for use as keyboard shortcuts in
    three different ways:

    \list
    \li For standard shortcuts, a \l{QKeySequence::StandardKey}{standard key}
       can be used to request the platform-specific key sequence associated
       with each shortcut.
    \li For custom shortcuts, human-readable strings such as "Ctrl+X" can
       be used, and these can be translated into the appropriate shortcuts
       for users of different languages. Translations are made in the
       "QShortcut" context.
    \li For hard-coded shortcuts, integer key codes can be specified with
       a combination of values defined by the BobUI::Key and BobUI::KeyboardModifier
       enum values. Each key code consists of a single BobUI::Key value and zero
       or more modifiers, such as BobUI::ShiftModifier, BobUI::ControlModifier,
       BobUI::AltModifier and BobUI::MetaModifier.
    \endlist

    For example, \uicontrol{Ctrl P} might be a sequence used as a shortcut for
    printing a document, and can be specified in any of the following
    ways:

    \snippet code/src_gui_kernel_qkeysequence.cpp 0

    Note that, for letters, the case used in the specification string
    does not matter. In the above examples, the user does not need to
    hold down the \uicontrol{Shift} key to activate a shortcut specified
    with "Ctrl+P". However, for other keys, the use of \uicontrol{Shift} as
    an unspecified extra modifier key can lead to confusion for users
    of an application whose keyboards have different layouts to those
    used by the developers. See the \l{Keyboard Layout Issues} section
    below for more details.

    It is preferable to use standard shortcuts where possible.
    When creating key sequences for non-standard shortcuts, you should use
    human-readable strings in preference to hard-coded integer values.

    QKeySequence object can be serialized to human-readable strings with the
    toString() function.

    An alternative way to specify hard-coded key codes is to use the Unicode
    code point of the character; for example, 'A' gives the same key sequence
    as BobUI::Key_A.

    \note On Apple platforms, references to "Ctrl", BobUI::CTRL, BobUI::Key_Control
    and BobUI::ControlModifier correspond to the \uicontrol Command keys on the
    Macintosh keyboard, and references to "Meta", BobUI::META, BobUI::Key_Meta and
    BobUI::MetaModifier correspond to the \uicontrol Control keys. In effect,
    developers can use the same shortcut descriptions across all platforms,
    and their applications will automatically work as expected on Apple platforms.

    \section1 Standard Shortcuts

    QKeySequence defines many \l{QKeySequence::StandardKey} {standard
    keyboard shortcuts} to reduce the amount of effort required when
    setting up actions in a typical application. The table below shows
    some common key sequences that are often used for these standard
    shortcuts by applications on four widely-used platforms.  Note
    that on Apple platforms, the \uicontrol Ctrl value corresponds to the \uicontrol
    Command keys on the Macintosh keyboard, and the \uicontrol Meta value
    corresponds to the \uicontrol Control keys.

    \table
    \header \li StandardKey      \li Windows                              \li Apple platforms          \li KDE Plasma   \li GNOME
    \row    \li HelpContents     \li F1                                   \li Ctrl+?                   \li F1           \li F1
    \row    \li WhatsThis        \li Shift+F1                             \li Shift+F1                 \li Shift+F1     \li Shift+F1
    \row    \li Open             \li Ctrl+O                               \li Ctrl+O                   \li Ctrl+O       \li Ctrl+O
    \row    \li Close            \li Ctrl+F4, Ctrl+W                      \li Ctrl+W, Ctrl+F4          \li Ctrl+W       \li Ctrl+W
    \row    \li Save             \li Ctrl+S                               \li Ctrl+S                   \li Ctrl+S       \li Ctrl+S
    \row    \li Quit             \li                                      \li Ctrl+Q                   \li Ctrl+Q       \li Ctrl+Q
    \row    \li SaveAs           \li Ctrl+Shift+S                         \li Ctrl+Shift+S             \li Ctrl+Shift+S \li Ctrl+Shift+S
    \row    \li New              \li Ctrl+N                               \li Ctrl+N                   \li Ctrl+N       \li Ctrl+N
    \row    \li Delete           \li Del                                  \li Forward Delete, Meta+D   \li Del, Ctrl+D  \li Del, Ctrl+D
    \row    \li Cut              \li Ctrl+X, Shift+Del                    \li Ctrl+X, Meta+K           \li Ctrl+X, F20, Shift+Del \li Ctrl+X, F20, Shift+Del
    \row    \li Copy             \li Ctrl+C, Ctrl+Ins                     \li Ctrl+C                   \li Ctrl+C, F16, Ctrl+Ins  \li Ctrl+C, F16, Ctrl+Ins
    \row    \li Paste            \li Ctrl+V, Shift+Ins                    \li Ctrl+V, Meta+Y           \li Ctrl+V, F18, Shift+Ins \li Ctrl+V, F18, Shift+Ins
    \row    \li Preferences      \li                                      \li Ctrl+,                   \li Ctrl+Shift+, \li
    \row    \li Undo             \li Ctrl+Z, Alt+Backspace                \li Ctrl+Z                   \li Ctrl+Z, F14  \li Ctrl+Z, F14
    \row    \li Redo             \li Ctrl+Y, Shift+Ctrl+Z, Alt+Shift+Backspace \li Ctrl+Shift+Z        \li Ctrl+Shift+Z \li Ctrl+Shift+Z
    \row    \li Back             \li Alt+Left, Backspace                  \li Ctrl+[                   \li Alt+Left     \li Alt+Left
    \row    \li Forward          \li Alt+Right, Shift+Backspace           \li Ctrl+]                   \li Alt+Right    \li Alt+Right
    \row    \li Refresh          \li F5                                   \li F5                       \li F5           \li Ctrl+R, F5
    \row    \li ZoomIn           \li Ctrl+Plus                            \li Ctrl+Plus                \li Ctrl+Plus    \li Ctrl+Plus
    \row    \li ZoomOut          \li Ctrl+Minus                           \li Ctrl+Minus               \li Ctrl+Minus   \li Ctrl+Minus
    \row    \li FullScreen       \li F11, Alt+Enter                       \li Ctrl+Meta+F              \li F11, Ctrl+Shift+F \li Ctrl+F11
    \row    \li Print            \li Ctrl+P                               \li Ctrl+P                   \li Ctrl+P       \li Ctrl+P
    \row    \li AddTab           \li Ctrl+T                               \li Ctrl+T                   \li Ctrl+Shift+N, Ctrl+T \li Ctrl+T
    \row    \li NextChild        \li Ctrl+Tab, Forward, Ctrl+F6           \li Ctrl+}, Forward, Ctrl+Tab \li Ctrl+Tab, Forward, Ctrl+Comma \li Ctrl+Tab, Forward
    \row    \li PreviousChild    \li Ctrl+Shift+Tab, Back, Ctrl+Shift+F6  \li Ctrl+{, Back, Ctrl+Shift+Tab \li Ctrl+Shift+Tab, Back, Ctrl+Period \li Ctrl+Shift+Tab, Back
    \row    \li Find             \li Ctrl+F                               \li Ctrl+F                   \li Ctrl+F         \li Ctrl+F
    \row    \li FindNext         \li F3, Ctrl+G                           \li Ctrl+G                   \li F3             \li Ctrl+G, F3
    \row    \li FindPrevious     \li Shift+F3, Ctrl+Shift+G               \li Ctrl+Shift+G             \li Shift+F3       \li Ctrl+Shift+G, Shift+F3
    \row    \li Replace          \li Ctrl+H                               \li (none)                   \li Ctrl+R         \li Ctrl+H
    \row    \li SelectAll        \li Ctrl+A                               \li Ctrl+A                   \li Ctrl+A         \li Ctrl+A
    \row    \li Deselect         \li                                      \li                          \li Ctrl+Shift+A   \li Ctrl+Shift+A
    \row    \li Bold             \li Ctrl+B                               \li Ctrl+B                   \li Ctrl+B         \li Ctrl+B
    \row    \li Italic           \li Ctrl+I                               \li Ctrl+I                   \li Ctrl+I         \li Ctrl+I
    \row    \li Underline        \li Ctrl+U                               \li Ctrl+U                   \li Ctrl+U         \li Ctrl+U
    \row    \li MoveToNextChar       \li Right                            \li Right, Meta+F            \li Right          \li Right
    \row    \li MoveToPreviousChar   \li Left                             \li Left, Meta+B             \li Left           \li Left
    \row    \li MoveToNextWord       \li Ctrl+Right                       \li Alt+Right                \li Ctrl+Right     \li Ctrl+Right
    \row    \li MoveToPreviousWord   \li Ctrl+Left                        \li Alt+Left                 \li Ctrl+Left      \li Ctrl+Left
    \row    \li MoveToNextLine       \li Down                             \li Down, Meta+N             \li Down           \li Down
    \row    \li MoveToPreviousLine   \li Up                               \li Up, Meta+P               \li Up             \li Up
    \row    \li MoveToNextPage       \li PgDown                           \li PgDown, Alt+PgDown, Meta+Down, Meta+PgDown, Meta+V \li PgDown \li PgDown
    \row    \li MoveToPreviousPage   \li PgUp                             \li PgUp, Alt+PgUp, Meta+Up, Meta+PgUp        \li PgUp   \li PgUp
    \row    \li MoveToStartOfLine    \li Home                             \li Ctrl+Left, Meta+Left   \li Home            \li Home
    \row    \li MoveToEndOfLine      \li End                              \li Ctrl+Right, Meta+Right \li End, Ctrl+E     \li End, Ctrl+E
    \row    \li MoveToStartOfBlock   \li (none)                           \li Alt+Up, Meta+A         \li (none)          \li (none)
    \row    \li MoveToEndOfBlock     \li (none)                           \li Alt+Down, Meta+E       \li (none)          \li (none)
    \row    \li MoveToStartOfDocument\li Ctrl+Home                        \li Ctrl+Up, Home          \li Ctrl+Home       \li Ctrl+Home
    \row    \li MoveToEndOfDocument  \li Ctrl+End                         \li Ctrl+Down, End         \li Ctrl+End        \li Ctrl+End
    \row    \li SelectNextChar       \li Shift+Right                      \li Shift+Right            \li Shift+Right     \li Shift+Right
    \row    \li SelectPreviousChar   \li Shift+Left                       \li Shift+Left             \li Shift+Left      \li Shift+Left
    \row    \li SelectNextWord       \li Ctrl+Shift+Right                 \li Alt+Shift+Right        \li Ctrl+Shift+Right \li Ctrl+Shift+Right
    \row    \li SelectPreviousWord   \li Ctrl+Shift+Left                  \li Alt+Shift+Left         \li Ctrl+Shift+Left \li Ctrl+Shift+Left
    \row    \li SelectNextLine       \li Shift+Down                       \li Shift+Down             \li Shift+Down     \li Shift+Down
    \row    \li SelectPreviousLine   \li Shift+Up                         \li Shift+Up               \li Shift+Up       \li Shift+Up
    \row    \li SelectNextPage       \li Shift+PgDown                     \li Shift+PgDown           \li Shift+PgDown   \li Shift+PgDown
    \row    \li SelectPreviousPage   \li Shift+PgUp                       \li Shift+PgUp             \li Shift+PgUp     \li Shift+PgUp
    \row    \li SelectStartOfLine    \li Shift+Home                       \li Ctrl+Shift+Left        \li Shift+Home     \li Shift+Home
    \row    \li SelectEndOfLine      \li Shift+End                        \li Ctrl+Shift+Right       \li Shift+End      \li Shift+End
    \row    \li SelectStartOfBlock   \li (none)                           \li Alt+Shift+Up, Meta+Shift+A \li (none)     \li (none)
    \row    \li SelectEndOfBlock     \li (none)                           \li Alt+Shift+Down, Meta+Shift+E \li (none)   \li (none)
    \row    \li SelectStartOfDocument\li Ctrl+Shift+Home                  \li Ctrl+Shift+Up, Shift+Home          \li Ctrl+Shift+Home\li Ctrl+Shift+Home
    \row    \li SelectEndOfDocument  \li Ctrl+Shift+End                   \li Ctrl+Shift+Down, Shift+End        \li Ctrl+Shift+End \li Ctrl+Shift+End
    \row    \li DeleteStartOfWord    \li Ctrl+Backspace                   \li Alt+Backspace          \li Ctrl+Backspace \li Ctrl+Backspace
    \row    \li DeleteEndOfWord      \li Ctrl+Del                         \li (none)                 \li Ctrl+Del       \li Ctrl+Del
    \row    \li DeleteEndOfLine      \li (none)                           \li (none)                 \li Ctrl+K         \li Ctrl+K
    \row    \li DeleteCompleteLine   \li (none)                           \li (none)                 \li Ctrl+U         \li Ctrl+U
    \row    \li InsertParagraphSeparator     \li Enter                    \li Enter                  \li Enter          \li Enter
    \row    \li InsertLineSeparator          \li Shift+Enter              \li Meta+Enter, Meta+O     \li Shift+Enter    \li Shift+Enter
    \row    \li Backspace             \li (none)                          \li Delete, Meta+H         \li (none)         \li (none)
    \row    \li Cancel                \li Escape                          \li Escape, Ctrl+.         \li Escape         \li Escape
    \endtable

    Note that, since the key sequences used for the standard shortcuts differ
    between platforms, you still need to test your shortcuts on each platform
    to ensure that you do not unintentionally assign the same key sequence to
    many actions.

    \section1 Keyboard Layout Issues

    Many key sequence specifications are chosen by developers based on the
    layout of certain types of keyboard, rather than choosing keys that
    represent the first letter of an action's name, such as \uicontrol{Ctrl S}
    ("Ctrl+S") or \uicontrol{Ctrl C} ("Ctrl+C").
    Additionally, because certain symbols can only be entered with the
    help of modifier keys on certain keyboard layouts, key sequences intended
    for use with one keyboard layout may map to a different key, map to no
    keys at all, or require an additional modifier key to be used on
    different keyboard layouts.

    For example, the shortcuts, \uicontrol{Ctrl plus} and \uicontrol{Ctrl minus}, are often
    used as shortcuts for zoom operations in graphics applications, and these
    may be specified as "Ctrl++" and "Ctrl+-" respectively. However, the way
    these shortcuts are specified and interpreted depends on the keyboard layout.
    Users of Norwegian keyboards will note that the \uicontrol{+} and \uicontrol{-} keys
    are not adjacent on the keyboard, but will still be able to activate both
    shortcuts without needing to press the \uicontrol{Shift} key. However, users
    with British keyboards will need to hold down the \uicontrol{Shift} key
    to enter the \uicontrol{+} symbol, making the shortcut effectively the same as
    "Ctrl+Shift+=".

    Although some developers might resort to fully specifying all the modifiers
    they use on their keyboards to activate a shortcut, this will also result
    in unexpected behavior for users of different keyboard layouts.

    For example, a developer using a British keyboard may decide to specify
    "Ctrl+Shift+=" as the key sequence in order to create a shortcut that
    coincidentally behaves in the same way as \uicontrol{Ctrl plus}. However, the
    \uicontrol{=} key needs to be accessed using the \uicontrol{Shift} key on Norwegian
    keyboard, making the required shortcut effectively \uicontrol{Ctrl Shift Shift =}
    (an impossible key combination).

    As a result, both human-readable strings and hard-coded key codes
    can both be problematic to use when specifying a key sequence that
    can be used on a variety of different keyboard layouts. Only the
    use of \l{QKeySequence::StandardKey} {standard shortcuts}
    guarantees that the user will be able to use the shortcuts that
    the developer intended.

    Despite this, we can address this issue by ensuring that human-readable
    strings are used, making it possible for translations of key sequences to
    be made for users of different languages. This approach will be successful
    for users whose keyboards have the most typical layout for the language
    they are using.

    \section1 GNU Emacs Style Key Sequences

    Key sequences similar to those used in \l{http://www.gnu.org/software/emacs/}{GNU Emacs}, allowing up to four
    key codes, can be created by using the multiple argument constructor,
    or by passing a human-readable string of comma-separated key sequences.

    For example, the key sequence, \uicontrol{Ctrl X} followed by \uicontrol{Ctrl C}, can
    be specified using either of the following ways:

    \snippet code/src_gui_kernel_qkeysequence.cpp 1

    \warning A QApplication instance must have been constructed before a
             QKeySequence is created; otherwise, your application may crash.

    \sa QShortcut
*/

/*!
    \enum QKeySequence::SequenceMatch

    \value NoMatch The key sequences are different; not even partially
    matching.
    \value PartialMatch The key sequences match partially, but are not
    the same.
    \value ExactMatch The key sequences are the same.
*/

/*!
    \enum QKeySequence::SequenceFormat

    \value NativeText The key sequence as a platform specific string.
    This means that it will be shown translated and on Apple platforms it will
    resemble a key sequence from the menu bar. This enum is best used when you
    want to display the string to the user.

    \value PortableText The key sequence is given in a "portable" format,
    suitable for reading and writing to a file. In many cases, it will look
    similar to the native text on Windows and X11.
*/

static constexpr struct {
    int key;
    const char name[25];
} keyname[] = {
    //: This and all following "incomprehensible" strings in QShortcut context
    //: are key names. Please use the localized names appearing on actual
    //: keyboards or whatever is commonly used.
    { BobUI::Key_Space,        BOBUI_TRANSLATE_NOOP("QShortcut", "Space") },
    { BobUI::Key_Escape,       BOBUI_TRANSLATE_NOOP("QShortcut", "Esc") },
    { BobUI::Key_Tab,          BOBUI_TRANSLATE_NOOP("QShortcut", "Tab") },
    { BobUI::Key_Backtab,      BOBUI_TRANSLATE_NOOP("QShortcut", "Backtab") },
    { BobUI::Key_Backspace,    BOBUI_TRANSLATE_NOOP("QShortcut", "Backspace") },
    { BobUI::Key_Return,       BOBUI_TRANSLATE_NOOP("QShortcut", "Return") },
    { BobUI::Key_Enter,        BOBUI_TRANSLATE_NOOP("QShortcut", "Enter") },
    { BobUI::Key_Insert,       BOBUI_TRANSLATE_NOOP("QShortcut", "Ins") },
    { BobUI::Key_Delete,       BOBUI_TRANSLATE_NOOP("QShortcut", "Del") },
    { BobUI::Key_Pause,        BOBUI_TRANSLATE_NOOP("QShortcut", "Pause") },
    { BobUI::Key_Print,        BOBUI_TRANSLATE_NOOP("QShortcut", "Print") },
    { BobUI::Key_SysReq,       BOBUI_TRANSLATE_NOOP("QShortcut", "SysReq") },
    { BobUI::Key_Home,         BOBUI_TRANSLATE_NOOP("QShortcut", "Home") },
    { BobUI::Key_End,          BOBUI_TRANSLATE_NOOP("QShortcut", "End") },
    { BobUI::Key_Left,         BOBUI_TRANSLATE_NOOP("QShortcut", "Left") },
    { BobUI::Key_Up,           BOBUI_TRANSLATE_NOOP("QShortcut", "Up") },
    { BobUI::Key_Right,        BOBUI_TRANSLATE_NOOP("QShortcut", "Right") },
    { BobUI::Key_Down,         BOBUI_TRANSLATE_NOOP("QShortcut", "Down") },
    { BobUI::Key_PageUp,       BOBUI_TRANSLATE_NOOP("QShortcut", "PgUp") },
    { BobUI::Key_PageDown,     BOBUI_TRANSLATE_NOOP("QShortcut", "PgDown") },
    { BobUI::Key_CapsLock,     BOBUI_TRANSLATE_NOOP("QShortcut", "CapsLock") },
    { BobUI::Key_NumLock,      BOBUI_TRANSLATE_NOOP("QShortcut", "NumLock") },
    { BobUI::Key_ScrollLock,   BOBUI_TRANSLATE_NOOP("QShortcut", "ScrollLock") },
    { BobUI::Key_Menu,         BOBUI_TRANSLATE_NOOP("QShortcut", "Menu") },
    { BobUI::Key_Help,         BOBUI_TRANSLATE_NOOP("QShortcut", "Help") },

    // Special keys
    // Includes multimedia, launcher, lan keys ( bluetooth, wireless )
    // window navigation
    { BobUI::Key_Back,                       BOBUI_TRANSLATE_NOOP("QShortcut", "Back") },
    { BobUI::Key_Forward,                    BOBUI_TRANSLATE_NOOP("QShortcut", "Forward") },
    { BobUI::Key_Stop,                       BOBUI_TRANSLATE_NOOP("QShortcut", "Stop") },
    { BobUI::Key_Refresh,                    BOBUI_TRANSLATE_NOOP("QShortcut", "Refresh") },
    { BobUI::Key_VolumeDown,                 BOBUI_TRANSLATE_NOOP("QShortcut", "Volume Down") },
    { BobUI::Key_VolumeMute,                 BOBUI_TRANSLATE_NOOP("QShortcut", "Volume Mute") },
    { BobUI::Key_VolumeUp,                   BOBUI_TRANSLATE_NOOP("QShortcut", "Volume Up") },
    { BobUI::Key_BassBoost,                  BOBUI_TRANSLATE_NOOP("QShortcut", "Bass Boost") },
    { BobUI::Key_BassUp,                     BOBUI_TRANSLATE_NOOP("QShortcut", "Bass Up") },
    { BobUI::Key_BassDown,                   BOBUI_TRANSLATE_NOOP("QShortcut", "Bass Down") },
    { BobUI::Key_TrebleUp,                   BOBUI_TRANSLATE_NOOP("QShortcut", "Treble Up") },
    { BobUI::Key_TrebleDown,                 BOBUI_TRANSLATE_NOOP("QShortcut", "Treble Down") },
    { BobUI::Key_MediaPlay,                  BOBUI_TRANSLATE_NOOP("QShortcut", "Media Play") },
    { BobUI::Key_MediaStop,                  BOBUI_TRANSLATE_NOOP("QShortcut", "Media Stop") },
    { BobUI::Key_MediaPrevious,              BOBUI_TRANSLATE_NOOP("QShortcut", "Media Previous") },
    { BobUI::Key_MediaNext,                  BOBUI_TRANSLATE_NOOP("QShortcut", "Media Next") },
    { BobUI::Key_MediaRecord,                BOBUI_TRANSLATE_NOOP("QShortcut", "Media Record") },
    //: Media player pause button
    { BobUI::Key_MediaPause,                 BOBUI_TRANSLATE_NOOP("QShortcut", "Media Pause") },
    //: Media player button to toggle between playing and paused
    { BobUI::Key_MediaTogglePlayPause,       BOBUI_TRANSLATE_NOOP("QShortcut", "Toggle Media Play/Pause") },
    { BobUI::Key_HomePage,                   BOBUI_TRANSLATE_NOOP("QShortcut", "Home Page") },
    { BobUI::Key_Favorites,                  BOBUI_TRANSLATE_NOOP("QShortcut", "Favorites") },
    { BobUI::Key_Search,                     BOBUI_TRANSLATE_NOOP("QShortcut", "Search") },
    { BobUI::Key_Standby,                    BOBUI_TRANSLATE_NOOP("QShortcut", "Standby") },
    { BobUI::Key_OpenUrl,                    BOBUI_TRANSLATE_NOOP("QShortcut", "Open URL") },
    { BobUI::Key_LaunchMail,                 BOBUI_TRANSLATE_NOOP("QShortcut", "Launch Mail") },
    { BobUI::Key_LaunchMedia,                BOBUI_TRANSLATE_NOOP("QShortcut", "Launch Media") },
    { BobUI::Key_Launch0,                    BOBUI_TRANSLATE_NOOP("QShortcut", "Launch (0)") },
    { BobUI::Key_Launch1,                    BOBUI_TRANSLATE_NOOP("QShortcut", "Launch (1)") },
    { BobUI::Key_Launch2,                    BOBUI_TRANSLATE_NOOP("QShortcut", "Launch (2)") },
    { BobUI::Key_Launch3,                    BOBUI_TRANSLATE_NOOP("QShortcut", "Launch (3)") },
    { BobUI::Key_Launch4,                    BOBUI_TRANSLATE_NOOP("QShortcut", "Launch (4)") },
    { BobUI::Key_Launch5,                    BOBUI_TRANSLATE_NOOP("QShortcut", "Launch (5)") },
    { BobUI::Key_Launch6,                    BOBUI_TRANSLATE_NOOP("QShortcut", "Launch (6)") },
    { BobUI::Key_Launch7,                    BOBUI_TRANSLATE_NOOP("QShortcut", "Launch (7)") },
    { BobUI::Key_Launch8,                    BOBUI_TRANSLATE_NOOP("QShortcut", "Launch (8)") },
    { BobUI::Key_Launch9,                    BOBUI_TRANSLATE_NOOP("QShortcut", "Launch (9)") },
    { BobUI::Key_LaunchA,                    BOBUI_TRANSLATE_NOOP("QShortcut", "Launch (A)") },
    { BobUI::Key_LaunchB,                    BOBUI_TRANSLATE_NOOP("QShortcut", "Launch (B)") },
    { BobUI::Key_LaunchC,                    BOBUI_TRANSLATE_NOOP("QShortcut", "Launch (C)") },
    { BobUI::Key_LaunchD,                    BOBUI_TRANSLATE_NOOP("QShortcut", "Launch (D)") },
    { BobUI::Key_LaunchE,                    BOBUI_TRANSLATE_NOOP("QShortcut", "Launch (E)") },
    { BobUI::Key_LaunchF,                    BOBUI_TRANSLATE_NOOP("QShortcut", "Launch (F)") },
    { BobUI::Key_LaunchG,                    BOBUI_TRANSLATE_NOOP("QShortcut", "Launch (G)") },
    { BobUI::Key_LaunchH,                    BOBUI_TRANSLATE_NOOP("QShortcut", "Launch (H)") },
    { BobUI::Key_MonBrightnessUp,            BOBUI_TRANSLATE_NOOP("QShortcut", "Monitor Brightness Up") },
    { BobUI::Key_MonBrightnessDown,          BOBUI_TRANSLATE_NOOP("QShortcut", "Monitor Brightness Down") },
    { BobUI::Key_KeyboardLightOnOff,         BOBUI_TRANSLATE_NOOP("QShortcut", "Keyboard Light On/Off") },
    { BobUI::Key_KeyboardBrightnessUp,       BOBUI_TRANSLATE_NOOP("QShortcut", "Keyboard Brightness Up") },
    { BobUI::Key_KeyboardBrightnessDown,     BOBUI_TRANSLATE_NOOP("QShortcut", "Keyboard Brightness Down") },
    { BobUI::Key_PowerOff,                   BOBUI_TRANSLATE_NOOP("QShortcut", "Power Off") },
    { BobUI::Key_WakeUp,                     BOBUI_TRANSLATE_NOOP("QShortcut", "Wake Up") },
    { BobUI::Key_Eject,                      BOBUI_TRANSLATE_NOOP("QShortcut", "Eject") },
    { BobUI::Key_ScreenSaver,                BOBUI_TRANSLATE_NOOP("QShortcut", "Screensaver") },
    { BobUI::Key_WWW,                        BOBUI_TRANSLATE_NOOP("QShortcut", "WWW") },
    { BobUI::Key_Sleep,                      BOBUI_TRANSLATE_NOOP("QShortcut", "Sleep") },
    { BobUI::Key_LightBulb,                  BOBUI_TRANSLATE_NOOP("QShortcut", "LightBulb") },
    { BobUI::Key_Shop,                       BOBUI_TRANSLATE_NOOP("QShortcut", "Shop") },
    { BobUI::Key_History,                    BOBUI_TRANSLATE_NOOP("QShortcut", "History") },
    { BobUI::Key_AddFavorite,                BOBUI_TRANSLATE_NOOP("QShortcut", "Add Favorite") },
    { BobUI::Key_HotLinks,                   BOBUI_TRANSLATE_NOOP("QShortcut", "Hot Links") },
    { BobUI::Key_BrightnessAdjust,           BOBUI_TRANSLATE_NOOP("QShortcut", "Adjust Brightness") },
    { BobUI::Key_Finance,                    BOBUI_TRANSLATE_NOOP("QShortcut", "Finance") },
    { BobUI::Key_Community,                  BOBUI_TRANSLATE_NOOP("QShortcut", "Community") },
    { BobUI::Key_AudioRewind,                BOBUI_TRANSLATE_NOOP("QShortcut", "Media Rewind") },
    { BobUI::Key_BackForward,                BOBUI_TRANSLATE_NOOP("QShortcut", "Back Forward") },
    { BobUI::Key_ApplicationLeft,            BOBUI_TRANSLATE_NOOP("QShortcut", "Application Left") },
    { BobUI::Key_ApplicationRight,           BOBUI_TRANSLATE_NOOP("QShortcut", "Application Right") },
    { BobUI::Key_Book,                       BOBUI_TRANSLATE_NOOP("QShortcut", "Book") },
    { BobUI::Key_CD,                         BOBUI_TRANSLATE_NOOP("QShortcut", "CD") },
    { BobUI::Key_Calculator,                 BOBUI_TRANSLATE_NOOP("QShortcut", "Calculator") },
    { BobUI::Key_Calendar,                   BOBUI_TRANSLATE_NOOP("QShortcut", "Calendar") },
    { BobUI::Key_Clear,                      BOBUI_TRANSLATE_NOOP("QShortcut", "Clear") },
    { BobUI::Key_ClearGrab,                  BOBUI_TRANSLATE_NOOP("QShortcut", "Clear Grab") },
    { BobUI::Key_Close,                      BOBUI_TRANSLATE_NOOP("QShortcut", "Close") },
    { BobUI::Key_ContrastAdjust,             BOBUI_TRANSLATE_NOOP("QShortcut", "Adjust contrast") },
    { BobUI::Key_Copy,                       BOBUI_TRANSLATE_NOOP("QShortcut", "Copy") },
    { BobUI::Key_Cut,                        BOBUI_TRANSLATE_NOOP("QShortcut", "Cut") },
    { BobUI::Key_Display,                    BOBUI_TRANSLATE_NOOP("QShortcut", "Display") },
    { BobUI::Key_DOS,                        BOBUI_TRANSLATE_NOOP("QShortcut", "DOS") },
    { BobUI::Key_Documents,                  BOBUI_TRANSLATE_NOOP("QShortcut", "Documents") },
    { BobUI::Key_Excel,                      BOBUI_TRANSLATE_NOOP("QShortcut", "Spreadsheet") },
    { BobUI::Key_Explorer,                   BOBUI_TRANSLATE_NOOP("QShortcut", "Browser") },
    { BobUI::Key_Game,                       BOBUI_TRANSLATE_NOOP("QShortcut", "Game") },
    { BobUI::Key_Go,                         BOBUI_TRANSLATE_NOOP("QShortcut", "Go") },
    { BobUI::Key_iTouch,                     BOBUI_TRANSLATE_NOOP("QShortcut", "iTouch") },
    { BobUI::Key_LogOff,                     BOBUI_TRANSLATE_NOOP("QShortcut", "Logoff") },
    { BobUI::Key_Market,                     BOBUI_TRANSLATE_NOOP("QShortcut", "Market") },
    { BobUI::Key_Meeting,                    BOBUI_TRANSLATE_NOOP("QShortcut", "Meeting") },
    { BobUI::Key_Memo,                       BOBUI_TRANSLATE_NOOP("QShortcut", "Memo") },
    { BobUI::Key_MenuKB,                     BOBUI_TRANSLATE_NOOP("QShortcut", "Keyboard Menu") },
    { BobUI::Key_MenuPB,                     BOBUI_TRANSLATE_NOOP("QShortcut", "Menu PB") },
    { BobUI::Key_MySites,                    BOBUI_TRANSLATE_NOOP("QShortcut", "My Sites") },
    { BobUI::Key_News,                       BOBUI_TRANSLATE_NOOP("QShortcut", "News") },
    { BobUI::Key_OfficeHome,                 BOBUI_TRANSLATE_NOOP("QShortcut", "Home Office") },
    { BobUI::Key_Option,                     BOBUI_TRANSLATE_NOOP("QShortcut", "Option") },
    { BobUI::Key_Paste,                      BOBUI_TRANSLATE_NOOP("QShortcut", "Paste") },
    { BobUI::Key_Phone,                      BOBUI_TRANSLATE_NOOP("QShortcut", "Phone") },
    { BobUI::Key_Reply,                      BOBUI_TRANSLATE_NOOP("QShortcut", "Reply") },
    { BobUI::Key_Reload,                     BOBUI_TRANSLATE_NOOP("QShortcut", "Reload") },
    { BobUI::Key_RotateWindows,              BOBUI_TRANSLATE_NOOP("QShortcut", "Rotate Windows") },
    { BobUI::Key_RotationPB,                 BOBUI_TRANSLATE_NOOP("QShortcut", "Rotation PB") },
    { BobUI::Key_RotationKB,                 BOBUI_TRANSLATE_NOOP("QShortcut", "Rotation KB") },
    { BobUI::Key_Save,                       BOBUI_TRANSLATE_NOOP("QShortcut", "Save") },
    { BobUI::Key_Send,                       BOBUI_TRANSLATE_NOOP("QShortcut", "Send") },
    { BobUI::Key_Spell,                      BOBUI_TRANSLATE_NOOP("QShortcut", "Spellchecker") },
    { BobUI::Key_SplitScreen,                BOBUI_TRANSLATE_NOOP("QShortcut", "Split Screen") },
    { BobUI::Key_Support,                    BOBUI_TRANSLATE_NOOP("QShortcut", "Support") },
    { BobUI::Key_TaskPane,                   BOBUI_TRANSLATE_NOOP("QShortcut", "Task Panel") },
    { BobUI::Key_Terminal,                   BOBUI_TRANSLATE_NOOP("QShortcut", "Terminal") },
    { BobUI::Key_ToDoList,                   BOBUI_TRANSLATE_NOOP("QShortcut", "To-do list") },
    { BobUI::Key_Tools,                      BOBUI_TRANSLATE_NOOP("QShortcut", "Tools") },
    { BobUI::Key_Travel,                     BOBUI_TRANSLATE_NOOP("QShortcut", "Travel") },
    { BobUI::Key_Video,                      BOBUI_TRANSLATE_NOOP("QShortcut", "Video") },
    { BobUI::Key_Word,                       BOBUI_TRANSLATE_NOOP("QShortcut", "Word Processor") },
    { BobUI::Key_Xfer,                       BOBUI_TRANSLATE_NOOP("QShortcut", "XFer") },
    { BobUI::Key_ZoomIn,                     BOBUI_TRANSLATE_NOOP("QShortcut", "Zoom In") },
    { BobUI::Key_ZoomOut,                    BOBUI_TRANSLATE_NOOP("QShortcut", "Zoom Out") },
    { BobUI::Key_Away,                       BOBUI_TRANSLATE_NOOP("QShortcut", "Away") },
    { BobUI::Key_Messenger,                  BOBUI_TRANSLATE_NOOP("QShortcut", "Messenger") },
    { BobUI::Key_WebCam,                     BOBUI_TRANSLATE_NOOP("QShortcut", "WebCam") },
    { BobUI::Key_MailForward,                BOBUI_TRANSLATE_NOOP("QShortcut", "Mail Forward") },
    { BobUI::Key_Pictures,                   BOBUI_TRANSLATE_NOOP("QShortcut", "Pictures") },
    { BobUI::Key_Music,                      BOBUI_TRANSLATE_NOOP("QShortcut", "Music") },
    { BobUI::Key_Battery,                    BOBUI_TRANSLATE_NOOP("QShortcut", "Battery") },
    { BobUI::Key_Bluetooth,                  BOBUI_TRANSLATE_NOOP("QShortcut", "Bluetooth") },
    { BobUI::Key_WLAN,                       BOBUI_TRANSLATE_NOOP("QShortcut", "Wireless") },
    { BobUI::Key_UWB,                        BOBUI_TRANSLATE_NOOP("QShortcut", "Ultra Wide Band") },
    { BobUI::Key_AudioForward,               BOBUI_TRANSLATE_NOOP("QShortcut", "Media Fast Forward") },
    { BobUI::Key_AudioRepeat,                BOBUI_TRANSLATE_NOOP("QShortcut", "Audio Repeat") },
    { BobUI::Key_AudioRandomPlay,            BOBUI_TRANSLATE_NOOP("QShortcut", "Audio Random Play") },
    { BobUI::Key_Subtitle,                   BOBUI_TRANSLATE_NOOP("QShortcut", "Subtitle") },
    { BobUI::Key_AudioCycleTrack,            BOBUI_TRANSLATE_NOOP("QShortcut", "Audio Cycle Track") },
    { BobUI::Key_Time,                       BOBUI_TRANSLATE_NOOP("QShortcut", "Time") },
    { BobUI::Key_Hibernate,                  BOBUI_TRANSLATE_NOOP("QShortcut", "Hibernate") },
    { BobUI::Key_View,                       BOBUI_TRANSLATE_NOOP("QShortcut", "View") },
    { BobUI::Key_TopMenu,                    BOBUI_TRANSLATE_NOOP("QShortcut", "Top Menu") },
    { BobUI::Key_PowerDown,                  BOBUI_TRANSLATE_NOOP("QShortcut", "Power Down") },
    { BobUI::Key_Suspend,                    BOBUI_TRANSLATE_NOOP("QShortcut", "Suspend") },

    { BobUI::Key_MicMute,                    BOBUI_TRANSLATE_NOOP("QShortcut", "Microphone Mute") },

    { BobUI::Key_Red,                        BOBUI_TRANSLATE_NOOP("QShortcut", "Red") },
    { BobUI::Key_Green,                      BOBUI_TRANSLATE_NOOP("QShortcut", "Green") },
    { BobUI::Key_Yellow,                     BOBUI_TRANSLATE_NOOP("QShortcut", "Yellow") },
    { BobUI::Key_Blue,                       BOBUI_TRANSLATE_NOOP("QShortcut", "Blue") },

    { BobUI::Key_ChannelUp,                  BOBUI_TRANSLATE_NOOP("QShortcut", "Channel Up") },
    { BobUI::Key_ChannelDown,                BOBUI_TRANSLATE_NOOP("QShortcut", "Channel Down") },

    { BobUI::Key_Guide,                      BOBUI_TRANSLATE_NOOP("QShortcut", "Guide") },
    { BobUI::Key_Info,                       BOBUI_TRANSLATE_NOOP("QShortcut", "Info") },
    { BobUI::Key_Settings,                   BOBUI_TRANSLATE_NOOP("QShortcut", "Settings") },

    { BobUI::Key_MicVolumeUp,                BOBUI_TRANSLATE_NOOP("QShortcut", "Microphone Volume Up") },
    { BobUI::Key_MicVolumeDown,              BOBUI_TRANSLATE_NOOP("QShortcut", "Microphone Volume Down") },

    { BobUI::Key_Keyboard,                   BOBUI_TRANSLATE_NOOP("QShortcut", "Keyboard") },

    { BobUI::Key_New,                        BOBUI_TRANSLATE_NOOP("QShortcut", "New") },
    { BobUI::Key_Open,                       BOBUI_TRANSLATE_NOOP("QShortcut", "Open") },
    { BobUI::Key_Find,                       BOBUI_TRANSLATE_NOOP("QShortcut", "Find") },
    { BobUI::Key_Undo,                       BOBUI_TRANSLATE_NOOP("QShortcut", "Undo") },
    { BobUI::Key_Redo,                       BOBUI_TRANSLATE_NOOP("QShortcut", "Redo") },

    // --------------------------------------------------------------
    // More consistent namings
    { BobUI::Key_Print,        BOBUI_TRANSLATE_NOOP("QShortcut", "Print Screen") },
    { BobUI::Key_PageUp,       BOBUI_TRANSLATE_NOOP("QShortcut", "Page Up") },
    { BobUI::Key_PageDown,     BOBUI_TRANSLATE_NOOP("QShortcut", "Page Down") },
    { BobUI::Key_CapsLock,     BOBUI_TRANSLATE_NOOP("QShortcut", "Caps Lock") },
    { BobUI::Key_NumLock,      BOBUI_TRANSLATE_NOOP("QShortcut", "Num Lock") },
    { BobUI::Key_NumLock,      BOBUI_TRANSLATE_NOOP("QShortcut", "Number Lock") },
    { BobUI::Key_ScrollLock,   BOBUI_TRANSLATE_NOOP("QShortcut", "Scroll Lock") },
    { BobUI::Key_Insert,       BOBUI_TRANSLATE_NOOP("QShortcut", "Insert") },
    { BobUI::Key_Delete,       BOBUI_TRANSLATE_NOOP("QShortcut", "Delete") },
    { BobUI::Key_Escape,       BOBUI_TRANSLATE_NOOP("QShortcut", "Escape") },
    { BobUI::Key_SysReq,       BOBUI_TRANSLATE_NOOP("QShortcut", "System Request") },

    // --------------------------------------------------------------
    // Keypad navigation keys
    { BobUI::Key_Select,       BOBUI_TRANSLATE_NOOP("QShortcut", "Select") },
    { BobUI::Key_Yes,          BOBUI_TRANSLATE_NOOP("QShortcut", "Yes") },
    { BobUI::Key_No,           BOBUI_TRANSLATE_NOOP("QShortcut", "No") },

    // --------------------------------------------------------------
    // Device keys
    { BobUI::Key_Context1,         BOBUI_TRANSLATE_NOOP("QShortcut", "Context1") },
    { BobUI::Key_Context2,         BOBUI_TRANSLATE_NOOP("QShortcut", "Context2") },
    { BobUI::Key_Context3,         BOBUI_TRANSLATE_NOOP("QShortcut", "Context3") },
    { BobUI::Key_Context4,         BOBUI_TRANSLATE_NOOP("QShortcut", "Context4") },
    //: Button to start a call (note: a separate button is used to end the call)
    { BobUI::Key_Call,             BOBUI_TRANSLATE_NOOP("QShortcut", "Call") },
    //: Button to end a call (note: a separate button is used to start the call)
    { BobUI::Key_Hangup,           BOBUI_TRANSLATE_NOOP("QShortcut", "Hangup") },
    //: Button that will hang up if we're in call, or make a call if we're not.
    { BobUI::Key_ToggleCallHangup, BOBUI_TRANSLATE_NOOP("QShortcut", "Toggle Call/Hangup") },
    { BobUI::Key_Flip,             BOBUI_TRANSLATE_NOOP("QShortcut", "Flip") },
    //: Button to trigger voice dialing
    { BobUI::Key_VoiceDial,        BOBUI_TRANSLATE_NOOP("QShortcut", "Voice Dial") },
    //: Button to redial the last number called
    { BobUI::Key_LastNumberRedial, BOBUI_TRANSLATE_NOOP("QShortcut", "Last Number Redial") },
    //: Button to trigger the camera shutter (take a picture)
    { BobUI::Key_Camera,           BOBUI_TRANSLATE_NOOP("QShortcut", "Camera Shutter") },
    //: Button to focus the camera
    { BobUI::Key_CameraFocus,      BOBUI_TRANSLATE_NOOP("QShortcut", "Camera Focus") },

    // --------------------------------------------------------------
    // Japanese keyboard support
    { BobUI::Key_Kanji,            BOBUI_TRANSLATE_NOOP("QShortcut", "Kanji") },
    { BobUI::Key_Muhenkan,         BOBUI_TRANSLATE_NOOP("QShortcut", "Muhenkan") },
    { BobUI::Key_Henkan,           BOBUI_TRANSLATE_NOOP("QShortcut", "Henkan") },
    { BobUI::Key_Romaji,           BOBUI_TRANSLATE_NOOP("QShortcut", "Romaji") },
    { BobUI::Key_Hiragana,         BOBUI_TRANSLATE_NOOP("QShortcut", "Hiragana") },
    { BobUI::Key_Katakana,         BOBUI_TRANSLATE_NOOP("QShortcut", "Katakana") },
    { BobUI::Key_Hiragana_Katakana,BOBUI_TRANSLATE_NOOP("QShortcut", "Hiragana Katakana") },
    { BobUI::Key_Zenkaku,          BOBUI_TRANSLATE_NOOP("QShortcut", "Zenkaku") },
    { BobUI::Key_Hankaku,          BOBUI_TRANSLATE_NOOP("QShortcut", "Hankaku") },
    { BobUI::Key_Zenkaku_Hankaku,  BOBUI_TRANSLATE_NOOP("QShortcut", "Zenkaku Hankaku") },
    { BobUI::Key_Touroku,          BOBUI_TRANSLATE_NOOP("QShortcut", "Touroku") },
    { BobUI::Key_Massyo,           BOBUI_TRANSLATE_NOOP("QShortcut", "Massyo") },
    { BobUI::Key_Kana_Lock,        BOBUI_TRANSLATE_NOOP("QShortcut", "Kana Lock") },
    { BobUI::Key_Kana_Shift,       BOBUI_TRANSLATE_NOOP("QShortcut", "Kana Shift") },
    { BobUI::Key_Eisu_Shift,       BOBUI_TRANSLATE_NOOP("QShortcut", "Eisu Shift") },
    { BobUI::Key_Eisu_toggle,      BOBUI_TRANSLATE_NOOP("QShortcut", "Eisu toggle") },
    { BobUI::Key_Codeinput,        BOBUI_TRANSLATE_NOOP("QShortcut", "Code input") },
    { BobUI::Key_MultipleCandidate,BOBUI_TRANSLATE_NOOP("QShortcut", "Multiple Candidate") },
    { BobUI::Key_PreviousCandidate,BOBUI_TRANSLATE_NOOP("QShortcut", "Previous Candidate") },

    // --------------------------------------------------------------
    // Korean keyboard support
    { BobUI::Key_Hangul,          BOBUI_TRANSLATE_NOOP("QShortcut", "Hangul") },
    { BobUI::Key_Hangul_Start,    BOBUI_TRANSLATE_NOOP("QShortcut", "Hangul Start") },
    { BobUI::Key_Hangul_End,      BOBUI_TRANSLATE_NOOP("QShortcut", "Hangul End") },
    { BobUI::Key_Hangul_Hanja,    BOBUI_TRANSLATE_NOOP("QShortcut", "Hangul Hanja") },
    { BobUI::Key_Hangul_Jamo,     BOBUI_TRANSLATE_NOOP("QShortcut", "Hangul Jamo") },
    { BobUI::Key_Hangul_Romaja,   BOBUI_TRANSLATE_NOOP("QShortcut", "Hangul Romaja") },
    { BobUI::Key_Hangul_Jeonja,   BOBUI_TRANSLATE_NOOP("QShortcut", "Hangul Jeonja") },
    { BobUI::Key_Hangul_Banja,    BOBUI_TRANSLATE_NOOP("QShortcut", "Hangul Banja") },
    { BobUI::Key_Hangul_PreHanja, BOBUI_TRANSLATE_NOOP("QShortcut", "Hangul PreHanja") },
    { BobUI::Key_Hangul_PostHanja,BOBUI_TRANSLATE_NOOP("QShortcut", "Hangul PostHanja") },
    { BobUI::Key_Hangul_Special,  BOBUI_TRANSLATE_NOOP("QShortcut", "Hangul Special") },

    // --------------------------------------------------------------
    // Miscellaneous keys
    { BobUI::Key_Cancel,  BOBUI_TRANSLATE_NOOP("QShortcut", "Cancel") },
    { BobUI::Key_Printer,  BOBUI_TRANSLATE_NOOP("QShortcut", "Printer") },
    { BobUI::Key_Execute,  BOBUI_TRANSLATE_NOOP("QShortcut", "Execute") },
    { BobUI::Key_Play,  BOBUI_TRANSLATE_NOOP("QShortcut", "Play") },
    { BobUI::Key_Zoom,  BOBUI_TRANSLATE_NOOP("QShortcut", "Zoom") },
    { BobUI::Key_Exit,  BOBUI_TRANSLATE_NOOP("QShortcut", "Exit") },
    { BobUI::Key_TouchpadToggle,  BOBUI_TRANSLATE_NOOP("QShortcut", "Touchpad Toggle") },
    { BobUI::Key_TouchpadOn,  BOBUI_TRANSLATE_NOOP("QShortcut", "Touchpad On") },
    { BobUI::Key_TouchpadOff,  BOBUI_TRANSLATE_NOOP("QShortcut", "Touchpad Off") },
    { BobUI::Key_Shift,  BOBUI_TRANSLATE_NOOP("QShortcut", "Shift") },
    { BobUI::Key_Control,  BOBUI_TRANSLATE_NOOP("QShortcut", "Control") },
    { BobUI::Key_Alt,  BOBUI_TRANSLATE_NOOP("QShortcut", "Alt") },
    { BobUI::Key_Meta,  BOBUI_TRANSLATE_NOOP("QShortcut", "Meta") },

};
static constexpr int numKeyNames = sizeof keyname / sizeof *keyname;

/*!
    \enum QKeySequence::StandardKey
    \since 4.2

    This enum represent standard key bindings. They can be used to
    assign platform dependent keyboard shortcuts to a QAction.

    Note that the key bindings are platform dependent. The currently
    bound shortcuts can be queried using keyBindings().

    \value AddTab           Add new tab.
    \value Back             Navigate back.
    \value Backspace        Delete previous character.
    \value Bold             Bold text.
    \value Close            Close document/tab.
    \value Copy             Copy.
    \value Cut              Cut.
    \value Delete           Delete.
    \value DeleteEndOfLine          Delete end of line.
    \value DeleteEndOfWord          Delete word from the end of the cursor.
    \value DeleteStartOfWord        Delete the beginning of a word up to the cursor.
    \value DeleteCompleteLine       Delete the entire line.
    \value Find             Find in document.
    \value FindNext         Find next result.
    \value FindPrevious     Find previous result.
    \value Forward          Navigate forward.
    \value HelpContents     Open help contents.
    \value InsertLineSeparator      Insert a new line.
    \value InsertParagraphSeparator Insert a new paragraph.
    \value Italic           Italic text.
    \value MoveToEndOfBlock         Move cursor to end of block. This shortcut is only used on Apple platforms.
    \value MoveToEndOfDocument      Move cursor to end of document.
    \value MoveToEndOfLine          Move cursor to end of line.
    \value MoveToNextChar           Move cursor to next character.
    \value MoveToNextLine           Move cursor to next line.
    \value MoveToNextPage           Move cursor to next page.
    \value MoveToNextWord           Move cursor to next word.
    \value MoveToPreviousChar       Move cursor to previous character.
    \value MoveToPreviousLine       Move cursor to previous line.
    \value MoveToPreviousPage       Move cursor to previous page.
    \value MoveToPreviousWord       Move cursor to previous word.
    \value MoveToStartOfBlock       Move cursor to start of a block. This shortcut is only used on Apple platforms.
    \value MoveToStartOfDocument    Move cursor to start of document.
    \value MoveToStartOfLine        Move cursor to start of line.
    \value New              Create new document.
    \value NextChild        Navigate to next tab or child window.
    \value Open             Open document.
    \value Paste            Paste.
    \value Preferences      Open the preferences dialog.
    \value PreviousChild    Navigate to previous tab or child window.
    \value Print            Print document.
    \value Quit             Quit the application.
    \value Redo             Redo.
    \value Refresh          Refresh or reload current document.
    \value Replace          Find and replace.
    \value SaveAs           Save document after prompting the user for a file name.
    \value Save             Save document.
    \value SelectAll        Select all text.
    \value Deselect         Deselect text. Since 5.1
    \value SelectEndOfBlock         Extend selection to the end of a text block. This shortcut is only used on Apple platforms.
    \value SelectEndOfDocument      Extend selection to end of document.
    \value SelectEndOfLine          Extend selection to end of line.
    \value SelectNextChar           Extend selection to next character.
    \value SelectNextLine           Extend selection to next line.
    \value SelectNextPage           Extend selection to next page.
    \value SelectNextWord           Extend selection to next word.
    \value SelectPreviousChar       Extend selection to previous character.
    \value SelectPreviousLine       Extend selection to previous line.
    \value SelectPreviousPage       Extend selection to previous page.
    \value SelectPreviousWord       Extend selection to previous word.
    \value SelectStartOfBlock       Extend selection to the start of a text block. This shortcut is only used on Apple platforms.
    \value SelectStartOfDocument    Extend selection to start of document.
    \value SelectStartOfLine        Extend selection to start of line.
    \value Underline        Underline text.
    \value Undo             Undo.
    \value UnknownKey       Unbound key.
    \value WhatsThis        Activate "what's this".
    \value ZoomIn           Zoom in.
    \value ZoomOut          Zoom out.
    \value FullScreen       Toggle the window state to/from full screen.
    \value Cancel           Cancel the current operation.
*/

/*!
    \fn QKeySequence &QKeySequence::operator=(QKeySequence &&other)

    Move-assigns \a other to this QKeySequence instance.

    \since 5.2
*/

/*!
    \since 4.2

    Constructs a QKeySequence object for the given \a key.
    The result will depend on the currently running platform.

    The resulting object will be based on the first element in the
    list of key bindings for the \a key.
*/
QKeySequence::QKeySequence(StandardKey key)
{
    const QList <QKeySequence> bindings = keyBindings(key);
    //pick only the first/primary shortcut from current bindings
    if (!bindings.isEmpty()) {
        d = bindings.constFirst().d;
        d->ref.ref();
    }
    else
        d = new QKeySequencePrivate();
}


/*!
    Constructs an empty key sequence.
*/
QKeySequence::QKeySequence()
{
    Q_CONSTINIT static QKeySequencePrivate shared_empty;
    d = &shared_empty;
    d->ref.ref();
}

/*!
    Creates a key sequence from the \a key string, based on \a format.

    For example "Ctrl+O" gives CTRL+'O'. The strings "Ctrl",
    "Shift", "Alt" and "Meta" are recognized, as well as their
    translated equivalents in the "QShortcut" context (using
    QObject::tr()).

    Up to four key codes may be entered by separating them with
    commas, e.g. "Alt+X,Ctrl+S,Q".

    This constructor is typically used with \l{QObject::tr()}{tr}(), so
    that shortcut keys can be replaced in translations:

    \snippet code/src_gui_kernel_qkeysequence.cpp 2

    Note the "File|Open" translator comment. It is by no means
    necessary, but it provides some context for the human translator.
*/
QKeySequence::QKeySequence(const QString &key, QKeySequence::SequenceFormat format)
{
    d = new QKeySequencePrivate();
    assign(key, format);
}

static_assert(QKeySequencePrivate::MaxKeyCount == 4, "Change docs and ctor impl below");
/*!
    Constructs a key sequence with up to 4 keys \a k1, \a k2,
    \a k3 and \a k4.

    The key codes are listed in BobUI::Key and can be combined with
    modifiers (see BobUI::KeyboardModifier) such as BobUI::ShiftModifier,
    BobUI::ControlModifier, BobUI::AltModifier, or BobUI::MetaModifier.
*/
QKeySequence::QKeySequence(int k1, int k2, int k3, int k4)
{
    d = new QKeySequencePrivate();
    d->key[0] = k1;
    d->key[1] = k2;
    d->key[2] = k3;
    d->key[3] = k4;
}

/*!
    Constructs a key sequence with up to 4 keys \a k1, \a k2,
    \a k3 and \a k4.

    \sa QKeyCombination
*/
QKeySequence::QKeySequence(QKeyCombination k1, QKeyCombination k2, QKeyCombination k3, QKeyCombination k4)
    : QKeySequence(k1.toCombined(), k2.toCombined(), k3.toCombined(), k4.toCombined())
{
}

/*!
    Copy constructor. Makes a copy of \a keysequence.
 */
QKeySequence::QKeySequence(const QKeySequence& keysequence)
    : d(keysequence.d)
{
    d->ref.ref();
}

/*!
    \since 4.2

    Returns a list of key bindings for the given \a key.
    The result of calling this function will vary based on the target platform.
    The first element of the list indicates the primary shortcut for the given platform.
    If the result contains more than one result, these can
    be considered alternative shortcuts on the same platform for the given \a key.
*/
QList<QKeySequence> QKeySequence::keyBindings(StandardKey key)
{
    return QGuiApplicationPrivate::platformTheme()->keyBindings(key);
}

/*!
    Destroys the key sequence.
 */
QKeySequence::~QKeySequence()
{
    if (!d->ref.deref())
        delete d;
}

/*!
    \internal
    KeySequences should never be modified, but rather just created.
    Internally though we do need to modify to keep pace in event
    delivery.
*/

void QKeySequence::setKey(QKeyCombination key, int index)
{
    Q_ASSERT_X(index >= 0 && index < QKeySequencePrivate::MaxKeyCount, "QKeySequence::setKey", "index out of range");
    qAtomicDetach(d);
    d->key[index] = key.toCombined();
}

static_assert(QKeySequencePrivate::MaxKeyCount == 4, "Change docs below");
/*!
    Returns the number of keys in the key sequence.
    The maximum is 4.
 */
int QKeySequence::count() const
{
    return int(std::distance(d->key, std::find(d->key, d->key + QKeySequencePrivate::MaxKeyCount, 0)));
}


/*!
    Returns \c true if the key sequence is empty; otherwise returns
    false.
*/
bool QKeySequence::isEmpty() const
{
    return !d->key[0];
}


/*!
    Returns the shortcut key sequence for the mnemonic in \a text,
    or an empty key sequence if no mnemonics are found.

    For example, mnemonic("E&xit") returns \c{BobUI::ALT+BobUI::Key_X},
    mnemonic("&Quit") returns \c{ALT+Key_Q}, and mnemonic("Quit")
    returns an empty QKeySequence.
*/
QKeySequence QKeySequence::mnemonic(const QString &text)
{
    QKeySequence ret;

    if (bobui_sequence_no_mnemonics)
        return ret;

    bool found = false;
    qsizetype p = 0;
    while (p >= 0) {
        p = text.indexOf(u'&', p) + 1;
        if (p <= 0 || p >= (int)text.size())
            break;
        if (text.at(p) != u'&') {
            QChar c = text.at(p);
            if (c.isPrint()) {
                if (!found) {
                    c = c.toUpper();
                    ret = QKeySequence(QKeyCombination(BobUI::ALT, BobUI::Key(c.unicode())));
#ifdef BOBUI_NO_DEBUG
                    return ret;
#else
                    found = true;
                } else {
                    qWarning("QKeySequence::mnemonic: \"%s\" contains multiple occurrences of '&'", qPrintable(text));
#endif
                }
            }
        }
        p++;
    }
    return ret;
}

/*!
    \fn int QKeySequence::assign(const QString &keys)

    Adds the given \a keys to the key sequence. \a keys may
    contain up to four key codes, provided they are separated by a
    comma; for example, "Alt+X,Ctrl+S,Z". The return value is the
    number of key codes added.
    \a keys should be in NativeText format.
*/
int QKeySequence::assign(const QString &ks)
{
    return assign(ks, NativeText);
}

/*!
    \fn int QKeySequence::assign(const QString &keys, QKeySequence::SequenceFormat format)
    \since 4.7

    Adds the given \a keys to the key sequence (based on \a format).
    \a keys may contain up to four key codes, provided they are
    separated by a comma; for example, "Alt+X,Ctrl+S,Z". The return
    value is the number of key codes added.
*/
int QKeySequence::assign(const QString &ks, QKeySequence::SequenceFormat format)
{
    QString keyseq = ks;
    int n = 0;
    qsizetype p = 0, diff = 0;

    // Run through the whole string, but stop
    // if we have MaxKeyCount keys before the end.
    while (keyseq.size() && n < QKeySequencePrivate::MaxKeyCount) {
        // We MUST use something to separate each sequence, and space
        // does not cut it, since some of the key names have space
        // in them.. (Let's hope no one translate with a comma in it:)
        p = keyseq.indexOf(u',');
        if (-1 != p) {
            if (p == keyseq.size() - 1) { // Last comma 'Ctrl+,'
                p = -1;
            } else {
                if (u',' == keyseq.at(p+1)) // e.g. 'Ctrl+,, Shift+,,'
                    p++;
                if (u' ' == keyseq.at(p+1)) { // Space after comma
                    diff = 1;
                    p++;
                } else {
                    diff = 0;
                }
            }
        }
        QString part = keyseq.left(-1 == p ? keyseq.size() : p - diff);
        keyseq = keyseq.right(-1 == p ? 0 : keyseq.size() - (p + 1));
        d->key[n] = QKeySequencePrivate::decodeString(std::move(part), format).toCombined();
        ++n;
    }
    return n;
}

struct QModifKeyName {
    QModifKeyName() { }
    QModifKeyName(int q, QChar n) : bobui_key(q), name(n) { }
    QModifKeyName(int q, const QString &n) : bobui_key(q), name(n) { }
    int bobui_key;
    QString name;
};
Q_DECLARE_TYPEINFO(QModifKeyName, Q_RELOCATABLE_TYPE);

Q_GLOBAL_STATIC(QList<QModifKeyName>, globalModifs)
Q_GLOBAL_STATIC(QList<QModifKeyName>, globalPortableModifs)

QKeyCombination QKeySequencePrivate::decodeString(QString accel, QKeySequence::SequenceFormat format)
{
    Q_ASSERT(!accel.isEmpty());

    int ret = 0;
    accel = std::move(accel).toLower();
    bool nativeText = (format == QKeySequence::NativeText);

    QList<QModifKeyName> *gmodifs;
    if (nativeText) {
        gmodifs = globalModifs();
        if (gmodifs->isEmpty()) {
#if defined(Q_OS_APPLE)
            const bool dontSwap = qApp->testAttribute(BobUI::AA_MacDontSwapCtrlAndMeta);
            if (dontSwap)
                *gmodifs << QModifKeyName(BobUI::META, QChar(kCommandUnicode));
            else
                *gmodifs << QModifKeyName(BobUI::CTRL, QChar(kCommandUnicode));
            *gmodifs << QModifKeyName(BobUI::ALT, QChar(kOptionUnicode));
            if (dontSwap)
                *gmodifs << QModifKeyName(BobUI::CTRL, QChar(kControlUnicode));
            else
                *gmodifs << QModifKeyName(BobUI::META, QChar(kControlUnicode));
            *gmodifs << QModifKeyName(BobUI::SHIFT, QChar(kShiftUnicode));
#endif
            *gmodifs << QModifKeyName(BobUI::CTRL, u"ctrl+"_s)
                     << QModifKeyName(BobUI::SHIFT, u"shift+"_s)
                     << QModifKeyName(BobUI::ALT, u"alt+"_s)
                     << QModifKeyName(BobUI::META, u"meta+"_s)
                     << QModifKeyName(BobUI::KeypadModifier, u"num+"_s);
        }
    } else {
        gmodifs = globalPortableModifs();
        if (gmodifs->isEmpty()) {
            *gmodifs << QModifKeyName(BobUI::CTRL, u"ctrl+"_s)
                     << QModifKeyName(BobUI::SHIFT, u"shift+"_s)
                     << QModifKeyName(BobUI::ALT, u"alt+"_s)
                     << QModifKeyName(BobUI::META, u"meta+"_s)
                     << QModifKeyName(BobUI::KeypadModifier, u"num+"_s);
        }
    }


    QList<QModifKeyName> modifs;
    if (nativeText) {
        modifs << QModifKeyName(BobUI::CTRL, QCoreApplication::translate("QShortcut", "Ctrl").toLower().append(u'+'))
               << QModifKeyName(BobUI::SHIFT, QCoreApplication::translate("QShortcut", "Shift").toLower().append(u'+'))
               << QModifKeyName(BobUI::ALT, QCoreApplication::translate("QShortcut", "Alt").toLower().append(u'+'))
               << QModifKeyName(BobUI::META, QCoreApplication::translate("QShortcut", "Meta").toLower().append(u'+'))
               << QModifKeyName(BobUI::KeypadModifier, QCoreApplication::translate("QShortcut", "Num").toLower().append(u'+'));
    }
    modifs += *gmodifs; // Test non-translated ones last

    QString sl = accel;
#if defined(Q_OS_APPLE)
    for (int i = 0; i < modifs.size(); ++i) {
        const QModifKeyName &mkf = modifs.at(i);
        if (sl.contains(mkf.name)) {
            ret |= mkf.bobui_key;
            accel.remove(mkf.name);
            sl = accel;
        }
    }
    if (accel.isEmpty()) // Incomplete, like for "Meta+Shift+"
        return BobUI::Key_unknown;
#endif

    int singlePlus = -1;
    qsizetype i = 0;
    qsizetype lastI = 0;
    while ((i = sl.indexOf(u'+', i + 1)) != -1) {
        QStringView sub = QStringView{ sl }.mid(lastI, i - lastI + 1);
        while (sub.size() > 1 && sub.at(0) == u' ') {
            sub = sub.mid(1);
            ++lastI;
        }

        // If we get here the shortcuts contains at least one '+'. We break up
        // along the following strategy:
        //      Meta+Ctrl++   ( "Meta+", "Ctrl+", "+" )
        //      Super+Shift+A ( "Super+", "Shift+" )
        //      4+3+2=1       ( "4+", "3+" )
        // In other words, everything we try to handle HAS to be a modifier
        // except for a single '+' at the end of the string.

        // Only '+' can have length 1.
        if (sub.size() == 1) {
            // Make sure we only encounter a single '+' at the end of the accel
            if (singlePlus >= 0)
                return BobUI::Key_unknown;
            singlePlus = lastI;
        } else {

            const auto identifyModifier = [&](QStringView sub) {
                for (int j = 0; j < modifs.size(); ++j) {
                    const QModifKeyName &mkf = modifs.at(j);
                    if (sub == mkf.name) {
                        ret |= mkf.bobui_key;
                        return true; // Shortcut, since if we find another it would/should just be a dup
                    }
                }
                return false;
            };

            bool validModifier = identifyModifier(sub);

            if (!validModifier) {
                // Try harder with slower code that trims spaces
                const QString cleanedSub = sub.toString().remove(u' ');
                validModifier = identifyModifier(cleanedSub);
            }
            if (!validModifier)
                return BobUI::Key_unknown;
        }
        lastI = i + 1;
    }

    qsizetype p = accel.lastIndexOf(u'+', singlePlus > 0 ? singlePlus - 1 : accel.size() - 1);
    QStringView accelRef(accel);
    if (p > 0)
        accelRef = accelRef.mid(p + 1);

    while (accelRef.size() > 1 && accelRef.at(0) == u' ')
        accelRef = accelRef.mid(1);
    while (accelRef.size() > 1 && accelRef.endsWith(u' '))
        accelRef.chop(1);

    int fnum = 0;
    if (accelRef.isEmpty())
        return BobUI::Key_unknown;
    else if (accelRef.size() == 1) {
#if defined(Q_OS_APPLE)
        int bobuiKey = bobuikeyForAppleSymbol(accelRef.at(0));
        if (bobuiKey != -1) {
            ret |= bobuiKey;
        } else
#endif
        {
            ret |= accelRef.at(0).toUpper().unicode();
        }
    } else if (accelRef.at(0) == u'f' && (fnum = accelRef.mid(1).toInt()) >= 1 && fnum <= 35) {
        ret |= BobUI::Key_F1 + fnum - 1;
    } else {
        // For NativeText, check the translation table first,
        // if we don't find anything then try it out with just the untranlated stuff.
        // PortableText will only try the untranlated table.
        bool found = false;
        for (int tran = 0; tran < 2; ++tran) {
            if (!nativeText)
                ++tran;
            for (int i = 0; i < numKeyNames; ++i) {
                QString keyName(tran == 0
                                ? QCoreApplication::translate("QShortcut", keyname[i].name)
                                : QString::fromLatin1(keyname[i].name));
                if (accelRef == std::move(keyName).toLower()) {
                    ret |= keyname[i].key;
                    found = true;
                    break;
                }
            }
            if (found)
                break;
        }
        // We couldn't translate the key.
        if (!found)
            return BobUI::Key_unknown;
    }
    return QKeyCombination::fromCombined(ret);
}

static inline void addKey(QString &str, const QString &theKey, QKeySequence::SequenceFormat format)
{
    if (!str.isEmpty()) {
        if (format == QKeySequence::NativeText) {
            //: Key separator in shortcut string
            str += QCoreApplication::translate("QShortcut", "+");
        } else {
            str += u'+';
        }
    }

    str += theKey;
}

QString QKeySequencePrivate::encodeString(QKeyCombination keyCombination, QKeySequence::SequenceFormat format)
{
    bool nativeText = (format == QKeySequence::NativeText);
    QString s;

    const auto key = keyCombination.key();

    // Handle -1 (Invalid Key) and BobUI::Key_unknown gracefully
    if (keyCombination.toCombined() == -1 || key == BobUI::Key_unknown)
        return s;

    const auto modifiers = keyCombination.keyboardModifiers();

#if defined(Q_OS_APPLE)
    if (nativeText) {
        // On Apple platforms the order (by default) is Meta, Alt, Shift, Control.
        // If the AA_MacDontSwapCtrlAndMeta is enabled, then the order
        // is Ctrl, Alt, Shift, Meta. The appleSymbolForBobUIKey helper does this swap
        // for us, which means that we have to adjust our order here.
        // The upshot is a lot more infrastructure to keep the number of
        // if tests down and the code relatively clean.
        static constexpr int ModifierOrder[] = { BobUI::META, BobUI::ALT, BobUI::SHIFT, BobUI::CTRL, 0 };
        static constexpr int BobUIKeyOrder[] = { BobUI::Key_Meta, BobUI::Key_Alt, BobUI::Key_Shift, BobUI::Key_Control, 0 };
        static constexpr int DontSwapModifierOrder[] = { BobUI::CTRL, BobUI::ALT, BobUI::SHIFT, BobUI::META, 0 };
        static constexpr int DontSwapBobUIKeyOrder[] = { BobUI::Key_Control, BobUI::Key_Alt, BobUI::Key_Shift, BobUI::Key_Meta, 0 };
        const int *modifierOrder;
        const int *bobuikeyOrder;
        if (qApp->testAttribute(BobUI::AA_MacDontSwapCtrlAndMeta)) {
            modifierOrder = DontSwapModifierOrder;
            bobuikeyOrder = DontSwapBobUIKeyOrder;
        } else {
            modifierOrder = ModifierOrder;
            bobuikeyOrder = BobUIKeyOrder;
        }

        for (int i = 0; modifierOrder[i] != 0; ++i) {
            if (modifiers & modifierOrder[i])
                s += appleSymbolForBobUIKey(bobuikeyOrder[i]);
        }
    } else
#endif
    {
        // On other systems the order is Meta, Control, Alt, Shift
        if (modifiers & BobUI::MetaModifier)
            s = nativeText ? QCoreApplication::translate("QShortcut", "Meta") : QString::fromLatin1("Meta");
        if (modifiers & BobUI::ControlModifier)
            addKey(s, nativeText ? QCoreApplication::translate("QShortcut", "Ctrl") : QString::fromLatin1("Ctrl"), format);
        if (modifiers & BobUI::AltModifier)
            addKey(s, nativeText ? QCoreApplication::translate("QShortcut", "Alt") : QString::fromLatin1("Alt"), format);
        if (modifiers & BobUI::ShiftModifier)
            addKey(s, nativeText ? QCoreApplication::translate("QShortcut", "Shift") : QString::fromLatin1("Shift"), format);
    }
    if (modifiers & BobUI::KeypadModifier)
        addKey(s, nativeText ? QCoreApplication::translate("QShortcut", "Num") : QString::fromLatin1("Num"), format);

    QString keyName = QKeySequencePrivate::keyName(key, format);

#if defined(Q_OS_APPLE)
    if (nativeText)
        s += keyName;
    else
#endif
    addKey(s, keyName, format);
    return s;
}

/*!
    \internal
    Returns the text representation of the key \a key, which can be used i.e.
    when the sequence is serialized. This does not take modifiers into account
    (see encodeString() for a version that does).

    This static method is used by encodeString() and by the D-Bus menu exporter.
*/
QString QKeySequencePrivate::keyName(BobUI::Key key, QKeySequence::SequenceFormat format)
{
    bool nativeText = (format == QKeySequence::NativeText);
    QString p;

    if (nativeText && (key > 0x00 && key <= 0x1f)) {
        // Map C0 control codes to the corresponding Control Pictures
        p = QChar::fromUcs2(0x2400 + key);
    } else if (key && key < BobUI::Key_Escape && key != BobUI::Key_Space) {
        if (!QChar::requiresSurrogates(key)) {
            p = QChar::fromUcs2(key).toUpper();
        } else {
            p += QChar(QChar::highSurrogate(key));
            p += QChar(QChar::lowSurrogate(key));
        }
    } else if (key >= BobUI::Key_F1 && key <= BobUI::Key_F35) {
            p = nativeText ? QCoreApplication::translate("QShortcut", "F%1").arg(key - BobUI::Key_F1 + 1)
                           : QString::fromLatin1("F%1").arg(key - BobUI::Key_F1 + 1);
    } else if (key) {
        int i=0;
#if defined(Q_OS_APPLE)
        if (nativeText) {
            QChar ch = appleSymbolForBobUIKey(key);
            if (!ch.isNull())
                p = ch;
            else
                goto NonSymbol;
        } else
#endif
        {
#if defined(Q_OS_APPLE)
NonSymbol:
#endif
            while (i < numKeyNames) {
                if (key == keyname[i].key) {
                    p = nativeText ? QCoreApplication::translate("QShortcut", keyname[i].name)
                                   : QString::fromLatin1(keyname[i].name);
                    break;
                }
                ++i;
            }
            // If we can't find the actual translatable keyname,
            // fall back on the unicode representation of it...
            // Or else characters like BobUI::Key_aring may not get displayed
            // (Really depends on you locale)
            if (i >= numKeyNames) {
                if (!QChar::requiresSurrogates(key)) {
                    p = QChar::fromUcs2(key).toUpper();
                } else {
                    p += QChar(QChar::highSurrogate(key));
                    p += QChar(QChar::lowSurrogate(key));
                }
            }
        }
    }
    return p;
}
/*!
    Matches the sequence with \a seq. Returns ExactMatch if
    successful, PartialMatch if \a seq matches incompletely,
    and NoMatch if the sequences have nothing in common.
    Returns NoMatch if \a seq is shorter.
*/
QKeySequence::SequenceMatch QKeySequence::matches(const QKeySequence &seq) const
{
    uint userN = count(),
          seqN = seq.count();

    if (userN > seqN)
        return NoMatch;

    // If equal in length, we have a potential ExactMatch sequence,
    // else we already know it can only be partial.
    SequenceMatch match = (userN == seqN ? ExactMatch : PartialMatch);

    for (uint i = 0; i < userN; ++i) {
        QKeyCombination userKey = (*this)[i],
                    sequenceKey = seq[i];
        if (userKey != sequenceKey)
            return NoMatch;
    }
    return match;
}

/*!
   Returns the key sequence as a QVariant
*/
QKeySequence::operator QVariant() const
{
    return QVariant::fromValue(*this);
}

/*!
    Returns a reference to the element at position \a index in the key
    sequence. This can only be used to read an element.
 */
QKeyCombination QKeySequence::operator[](uint index) const
{
    Q_ASSERT_X(index < QKeySequencePrivate::MaxKeyCount, "QKeySequence::operator[]", "index out of range");
    return QKeyCombination::fromCombined(d->key[index]);
}


/*!
    Assignment operator. Assigns the \a other key sequence to this
    object.
 */
QKeySequence &QKeySequence::operator=(const QKeySequence &other)
{
    qAtomicAssign(d, other.d);
    return *this;
}

/*!
    \fn void QKeySequence::swap(QKeySequence &other)
    \since 4.8
    \memberswap{key sequence}
*/

/*!
    \fn bool QKeySequence::operator!=(const QKeySequence &other) const

    Returns \c true if this key sequence is not equal to the \a other
    key sequence; otherwise returns \c false.
*/


/*!
    Returns \c true if this key sequence is equal to the \a other
    key sequence; otherwise returns \c false.
 */
bool QKeySequence::operator==(const QKeySequence &other) const
{
    return (d->key[0] == other.d->key[0] &&
            d->key[1] == other.d->key[1] &&
            d->key[2] == other.d->key[2] &&
            d->key[3] == other.d->key[3]);
}

/*!
    \since 5.6
    \qhashold{QKeySequence}
*/
size_t qHash(const QKeySequence &key, size_t seed) noexcept
{
    return qHashRange(key.d->key, key.d->key + QKeySequencePrivate::MaxKeyCount, seed);
}

/*!
    Provides an arbitrary comparison of this key sequence and
    \a other key sequence. All that is guaranteed is that the
    operator returns \c false if both key sequences are equal and
    that (ks1 \< ks2) == !( ks2 \< ks1) if the key sequences
    are not equal.

    This function is useful in some circumstances, for example
    if you want to use QKeySequence objects as keys in a QMap.

    \sa operator==(), operator!=(), operator>(), operator<=(), operator>=()
*/
bool QKeySequence::operator< (const QKeySequence &other) const
{
    return std::lexicographical_compare(d->key, d->key + QKeySequencePrivate::MaxKeyCount,
                                        other.d->key, other.d->key + QKeySequencePrivate::MaxKeyCount);
}

/*!
    \fn bool QKeySequence::operator> (const QKeySequence &other) const

    Returns \c true if this key sequence is larger than the \a other key
    sequence; otherwise returns \c false.

    \sa operator==(), operator!=(), operator<(), operator<=(), operator>=()
*/

/*!
    \fn bool QKeySequence::operator<= (const QKeySequence &other) const

    Returns \c true if this key sequence is smaller or equal to the
    \a other key sequence; otherwise returns \c false.

    \sa operator==(), operator!=(), operator<(), operator>(), operator>=()
*/

/*!
    \fn bool QKeySequence::operator>= (const QKeySequence &other) const

    Returns \c true if this key sequence is larger or equal to the
    \a other key sequence; otherwise returns \c false.

    \sa operator==(), operator!=(), operator<(), operator>(), operator<=()
*/

/*!
    \internal
*/
bool QKeySequence::isDetached() const
{
    return d->ref.loadRelaxed() == 1;
}

/*!
    \since 4.1

    Return a string representation of the key sequence,
    based on \a format.

    For example, the value BobUI::CTRL+BobUI::Key_O results in "Ctrl+O".
    If the key sequence has multiple key codes, each is separated
    by commas in the string returned, such as "Alt+X, Ctrl+Y, Z".
    The strings, "Ctrl", "Shift", etc. are translated using
    QObject::tr() in the "QShortcut" context.

    If the key sequence has no keys, an empty string is returned.

    On Apple platforms, the string returned resembles the sequence that is
    shown in the menu bar if \a format is
    QKeySequence::NativeText; otherwise, the string uses the
    "portable" format, suitable for writing to a file.

    \sa fromString()
*/
QString QKeySequence::toString(SequenceFormat format) const
{
    QString finalString;
    // A standard string, with no translation or anything like that. In some ways it will
    // look like our latin case on Windows and X11
    int end = count();
    for (int i = 0; i < end; ++i) {
        finalString += d->encodeString(QKeyCombination::fromCombined(d->key[i]), format);
        finalString += ", "_L1;
    }
    finalString.truncate(finalString.size() - 2);
    return finalString;
}

/*!
    \since 4.1

    Return a QKeySequence from the string \a str based on \a format.

    \sa toString()
*/
QKeySequence QKeySequence::fromString(const QString &str, SequenceFormat format)
{
    return QKeySequence(str, format);
}

/*!
    \since 5.1

    Return a list of QKeySequence from the string \a str based on \a format.

    \sa fromString()
    \sa listToString()
*/
QList<QKeySequence> QKeySequence::listFromString(const QString &str, SequenceFormat format)
{
    QList<QKeySequence> result;

    const QStringList strings = str.split("; "_L1);
    result.reserve(strings.size());
    for (const QString &string : strings) {
        result << fromString(string, format);
    }

    return result;
}

/*!
    \since 5.1

    Return a string representation of \a list based on \a format.

    \sa toString()
    \sa listFromString()
*/
QString QKeySequence::listToString(const QList<QKeySequence> &list, SequenceFormat format)
{
    QString result;

    for (const QKeySequence &sequence : list) {
        result += sequence.toString(format);
        result += "; "_L1;
    }
    result.truncate(result.size() - 2);

    return result;
}

/*****************************************************************************
  QKeySequence stream functions
 *****************************************************************************/
#if !defined(BOBUI_NO_DATASTREAM)
/*!
    \fn QDataStream &operator<<(QDataStream &stream, const QKeySequence &sequence)
    \relates QKeySequence

    Writes the key \a sequence to the \a stream.

    \sa{Serializing BobUI Data Types}{Format of the QDataStream operators}
*/
QDataStream &operator<<(QDataStream &s, const QKeySequence &keysequence)
{
    static_assert(QKeySequencePrivate::MaxKeyCount == 4, "Forgot to adapt QDataStream &operator<<(QDataStream &s, const QKeySequence &keysequence) to new QKeySequence::MaxKeyCount");
    const bool extended = s.version() >= 5 && keysequence.count() > 1;
    s << quint32(extended ? 4 : 1) << quint32(keysequence.d->key[0]);
    if (extended) {
        s << quint32(keysequence.d->key[1])
          << quint32(keysequence.d->key[2])
          << quint32(keysequence.d->key[3]);
    }
    return s;
}


/*!
    \fn QDataStream &operator>>(QDataStream &stream, QKeySequence &sequence)
    \relates QKeySequence

    Reads a key sequence from the \a stream into the key \a sequence.

    \sa{Serializing BobUI Data Types}{Format of the QDataStream operators}
*/
QDataStream &operator>>(QDataStream &s, QKeySequence &keysequence)
{
    const quint32 MaxKeys = QKeySequencePrivate::MaxKeyCount;
    quint32 c;
    s >> c;
    quint32 keys[MaxKeys] = {0};
    for (uint i = 0; i < qMin(c, MaxKeys); ++i) {
        if (s.atEnd()) {
            qWarning("Premature EOF while reading QKeySequence");
            return s;
        }
        s >> keys[i];
    }
    qAtomicDetach(keysequence.d);
    std::copy(keys, keys + MaxKeys, BOBUI_MAKE_CHECKED_ARRAY_ITERATOR(keysequence.d->key, MaxKeys));
    return s;
}

#endif //BOBUI_NO_DATASTREAM

#ifndef BOBUI_NO_DEBUG_STREAM
QDebug operator<<(QDebug dbg, const QKeySequence &p)
{
    QDebugStateSaver saver(dbg);
    dbg.nospace() << "QKeySequence(" << p.toString() << ')';
    return dbg;
}
#endif

/*!
    \typedef QKeySequence::DataPtr
    \internal
*/

 /*!
    \fn DataPtr &QKeySequence::data_ptr()
    \internal
*/

BOBUI_END_NAMESPACE

#include "moc_qkeysequence.cpp"
