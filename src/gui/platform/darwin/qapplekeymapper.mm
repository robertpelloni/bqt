// Copyright (C) 2021 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include <qglobal.h>

#ifdef Q_OS_MACOS
#include <AppKit/AppKit.h>
#endif

#if defined(BOBUI_PLATFORM_UIKIT)
#include <UIKit/UIKit.h>
#endif

#include "qapplekeymapper_p.h"

#include <BobUICore/qloggingcategory.h>
#include <BobUIGui/QGuiApplication>

BOBUI_BEGIN_NAMESPACE

#ifdef Q_OS_MACOS
Q_STATIC_LOGGING_CATEGORY(lcQpaKeyMapperKeys, "bobui.qpa.keymapper.keys");
#endif

static BobUI::KeyboardModifiers swapModifiersIfNeeded(const BobUI::KeyboardModifiers modifiers)
{
    if (QCoreApplication::testAttribute(BobUI::AA_MacDontSwapCtrlAndMeta))
        return modifiers;

    BobUI::KeyboardModifiers swappedModifiers = modifiers;
    swappedModifiers &= ~(BobUI::MetaModifier | BobUI::ControlModifier);

    if (modifiers & BobUI::ControlModifier)
        swappedModifiers |= BobUI::MetaModifier;
    if (modifiers & BobUI::MetaModifier)
        swappedModifiers |= BobUI::ControlModifier;

    return swappedModifiers;
}

#ifdef Q_OS_MACOS
static constexpr std::tuple<NSEventModifierFlags, BobUI::KeyboardModifier> cocoaModifierMap[] = {
    { NSEventModifierFlagShift, BobUI::ShiftModifier },
    { NSEventModifierFlagControl, BobUI::ControlModifier },
    { NSEventModifierFlagCommand, BobUI::MetaModifier },
    { NSEventModifierFlagOption, BobUI::AltModifier },
    { NSEventModifierFlagNumericPad, BobUI::KeypadModifier }
};

BobUI::KeyboardModifiers QAppleKeyMapper::fromCocoaModifiers(NSEventModifierFlags cocoaModifiers)
{
    BobUI::KeyboardModifiers bobuiModifiers = BobUI::NoModifier;
    for (const auto &[cocoaModifier, bobuiModifier] : cocoaModifierMap) {
        if (cocoaModifiers & cocoaModifier)
            bobuiModifiers |= bobuiModifier;
    }

    return swapModifiersIfNeeded(bobuiModifiers);
}

NSEventModifierFlags QAppleKeyMapper::toCocoaModifiers(BobUI::KeyboardModifiers bobuiModifiers)
{
    bobuiModifiers = swapModifiersIfNeeded(bobuiModifiers);

    NSEventModifierFlags cocoaModifiers = 0;
    for (const auto &[cocoaModifier, bobuiModifier] : cocoaModifierMap) {
        if (bobuiModifiers & bobuiModifier)
            cocoaModifiers |= cocoaModifier;
    }

    return cocoaModifiers;
}

using CarbonModifiers = UInt32; // As opposed to EventModifiers which is UInt16

static CarbonModifiers toCarbonModifiers(BobUI::KeyboardModifiers bobuiModifiers)
{
    bobuiModifiers = swapModifiersIfNeeded(bobuiModifiers);

    static constexpr std::tuple<int, BobUI::KeyboardModifier> carbonModifierMap[] = {
        { shiftKey, BobUI::ShiftModifier },
        { controlKey, BobUI::ControlModifier },
        { cmdKey, BobUI::MetaModifier },
        { optionKey, BobUI::AltModifier },
        { kEventKeyModifierNumLockMask, BobUI::KeypadModifier }
    };

    CarbonModifiers carbonModifiers = 0;
    for (const auto &[carbonModifier, bobuiModifier] : carbonModifierMap) {
        if (bobuiModifiers & bobuiModifier)
            carbonModifiers |= carbonModifier;
    }

    return carbonModifiers;
}

// NSEvent.keyCode codes for keys that are independent of keyboard layout.
// Some of these are technically possible to add custom key maps for, but
// doing so would be unexpected.
static QHash<char16_t, BobUI::Key> layoutIndependentKeyCodes = {
    { kVK_F1, BobUI::Key_F1 },
    { kVK_F2, BobUI::Key_F2 },
    { kVK_F3, BobUI::Key_F3 },
    { kVK_F4, BobUI::Key_F4 },
    { kVK_F5, BobUI::Key_F5 },
    { kVK_F6, BobUI::Key_F6 },
    { kVK_F7, BobUI::Key_F7 },
    { kVK_F8, BobUI::Key_F8 },
    { kVK_F9, BobUI::Key_F9 },
    { kVK_F10, BobUI::Key_F10 },
    { kVK_F11, BobUI::Key_F11 },
    { kVK_F12, BobUI::Key_F12 },
    { kVK_F13, BobUI::Key_F13 },
    { kVK_F14, BobUI::Key_F14 },
    { kVK_F15, BobUI::Key_F15 },
    { kVK_F16, BobUI::Key_F16 },
    { kVK_F17, BobUI::Key_F17 },
    { kVK_F18, BobUI::Key_F18 },
    { kVK_F19, BobUI::Key_F19 },
    { kVK_F20, BobUI::Key_F20 },

    { kVK_Return, BobUI::Key_Return },
    { kVK_Tab, BobUI::Key_Tab },
    { kVK_Space, BobUI::Key_Space },
    { kVK_Escape, BobUI::Key_Escape },
    { kVK_Delete, BobUI::Key_Backspace },
    { kVK_ForwardDelete, BobUI::Key_Delete },

    { kVK_Home, BobUI::Key_Home },
    { kVK_End, BobUI::Key_End },
    { kVK_PageUp, BobUI::Key_PageUp },
    { kVK_PageDown, BobUI::Key_PageDown },

    { kVK_UpArrow, BobUI::Key_Up },
    { kVK_DownArrow, BobUI::Key_Down },
    { kVK_LeftArrow, BobUI::Key_Left },
    { kVK_RightArrow, BobUI::Key_Right },

    { kVK_CapsLock, BobUI::Key_CapsLock },
    { kVK_Shift, BobUI::Key_Shift },
    { kVK_RightShift, BobUI::Key_Shift },

#if 0
    // FIXME: Map these here instead of relying on
    // custom logic in [QNSView flagsChanged:]

    { kVK_Command, BobUI::Key_unknown },
    { kVK_RightCommand, BobUI::Key_unknown },
    { kVK_Option, BobUI::Key_unknown },
    { kVK_RightOption, BobUI::Key_unknown },
    { kVK_Control, BobUI::Key_unknown },
    { kVK_RightControl, BobUI::Key_unknown },
    { kVK_Function, BobUI::Key_unknown },
#endif

    { kVK_VolumeUp, BobUI::Key_VolumeUp },
    { kVK_VolumeDown, BobUI::Key_VolumeDown },
    { kVK_Mute, BobUI::Key_VolumeMute },

#if 0
    // FIXME: Figure out which BobUI::Key this maps to
    { kVK_ContextualMenu, BobUI::Key_unknown },
#endif
    { kVK_Help, BobUI::Key_Help },

    { kVK_ANSI_KeypadClear, BobUI::Key_Clear },
    { kVK_ANSI_KeypadEnter, BobUI::Key_Enter },
};

static QHash<char16_t, BobUI::Key> functionKeys = {
    { NSUpArrowFunctionKey, BobUI::Key_Up },
    { NSDownArrowFunctionKey, BobUI::Key_Down },
    { NSLeftArrowFunctionKey, BobUI::Key_Left },
    { NSRightArrowFunctionKey, BobUI::Key_Right },
    // F1-35 function keys handled manually below
    { NSInsertFunctionKey, BobUI::Key_Insert },
    { NSDeleteFunctionKey, BobUI::Key_Delete },
    { NSHomeFunctionKey, BobUI::Key_Home },
    { NSEndFunctionKey, BobUI::Key_End },
    { NSPageUpFunctionKey, BobUI::Key_PageUp },
    { NSPageDownFunctionKey, BobUI::Key_PageDown },
    { NSPrintScreenFunctionKey, BobUI::Key_Print },
    { NSScrollLockFunctionKey, BobUI::Key_ScrollLock },
    { NSPauseFunctionKey, BobUI::Key_Pause },
    { NSSysReqFunctionKey, BobUI::Key_SysReq },
    { NSMenuFunctionKey, BobUI::Key_Menu },
    { NSPrintFunctionKey, BobUI::Key_Printer },
    { NSClearDisplayFunctionKey, BobUI::Key_Clear },
    { NSInsertCharFunctionKey, BobUI::Key_Insert },
    { NSDeleteCharFunctionKey, BobUI::Key_Delete },
    { NSSelectFunctionKey, BobUI::Key_Select },
    { NSExecuteFunctionKey, BobUI::Key_Execute },
    { NSUndoFunctionKey, BobUI::Key_Undo },
    { NSRedoFunctionKey, BobUI::Key_Redo },
    { NSFindFunctionKey, BobUI::Key_Find },
    { NSHelpFunctionKey, BobUI::Key_Help },
    { NSModeSwitchFunctionKey, BobUI::Key_Mode_switch }
};

static int toKeyCode(const QChar &key, int virtualKey, int modifiers)
{
    qCDebug(lcQpaKeyMapperKeys, "Mapping key: %d (0x%04x) / vk %d (0x%04x)",
        key.unicode(), key.unicode(), virtualKey, virtualKey);

    // Check first if we have a virtual key that should be treated as layout
    // independent. If so, we want to return early without inspecting the key.
    if (auto bobuiKey = layoutIndependentKeyCodes.value(virtualKey)) {
        qCDebug(lcQpaKeyMapperKeys) << "Got" << bobuiKey << "based on layout independent virtual key";
        // To work like BobUI for X11 we issue Backtab when Shift + Tab are pressed
        if (bobuiKey == BobUI::Key_Tab && (modifiers & BobUI::ShiftModifier)) {
            qCDebug(lcQpaKeyMapperKeys, "Transformed into BobUI::Key_Backtab");
            return BobUI::Key_Backtab;
        }
        return bobuiKey;
    }

    // Then check if the key is one of the functions keys in the private Unicode range
    if (key >= char16_t(NSUpArrowFunctionKey) && key <= char16_t(NSModeSwitchFunctionKey)) {
        if (auto bobuiKey = functionKeys.value(key.unicode())) {
            qCDebug(lcQpaKeyMapperKeys) << "Got" << bobuiKey;
            return bobuiKey;
        } else if (key >= char16_t(NSF1FunctionKey) && key <= char16_t(NSF35FunctionKey)) {
            auto functionKey = BobUI::Key_F1 + (key.unicode() - NSF1FunctionKey) ;
            qCDebug(lcQpaKeyMapperKeys) << "Got" << functionKey;
            return functionKey;
        }
    }

    if (key.isDigit()) {
        qCDebug(lcQpaKeyMapperKeys, "Got digit key: %d", key.digitValue());
        return key.digitValue() + BobUI::Key_0;
    }

    if (key.isLetter()) {
        qCDebug(lcQpaKeyMapperKeys, "Got letter key: %d", (key.toUpper().unicode() - 'A'));
        return (key.toUpper().unicode() - 'A') + BobUI::Key_A;
    }
    if (key.isSymbol()) {
        qCDebug(lcQpaKeyMapperKeys, "Got symbol key: %d", (key.unicode()));
        return key.unicode();
    }

    qCDebug(lcQpaKeyMapperKeys, "Unknown case.. %d[%d] %d", key.unicode(), key.toLatin1(), virtualKey);
    return BobUI::Key_unknown;
}

// --------- Cocoa key mapping moved from BobUI Core ---------

static const int NSEscapeCharacter = 27; // not defined by Cocoa headers

static const QHash<char16_t, BobUI::Key> cocoaKeys = {
    { NSEnterCharacter, BobUI::Key_Enter },
    { NSBackspaceCharacter, BobUI::Key_Backspace },
    { NSTabCharacter, BobUI::Key_Tab },
    { NSNewlineCharacter, BobUI::Key_Return },
    { NSCarriageReturnCharacter, BobUI::Key_Return },
    { NSBackTabCharacter, BobUI::Key_Backtab },
    { NSEscapeCharacter, BobUI::Key_Escape },
    { NSDeleteCharacter, BobUI::Key_Backspace },
    { NSUpArrowFunctionKey, BobUI::Key_Up },
    { NSDownArrowFunctionKey, BobUI::Key_Down },
    { NSLeftArrowFunctionKey, BobUI::Key_Left },
    { NSRightArrowFunctionKey, BobUI::Key_Right },
    { NSF1FunctionKey, BobUI::Key_F1 },
    { NSF2FunctionKey, BobUI::Key_F2 },
    { NSF3FunctionKey, BobUI::Key_F3 },
    { NSF4FunctionKey, BobUI::Key_F4 },
    { NSF5FunctionKey, BobUI::Key_F5 },
    { NSF6FunctionKey, BobUI::Key_F6 },
    { NSF7FunctionKey, BobUI::Key_F7 },
    { NSF8FunctionKey, BobUI::Key_F8 },
    { NSF9FunctionKey, BobUI::Key_F9 },
    { NSF10FunctionKey, BobUI::Key_F10 },
    { NSF11FunctionKey, BobUI::Key_F11 },
    { NSF12FunctionKey, BobUI::Key_F12 },
    { NSF13FunctionKey, BobUI::Key_F13 },
    { NSF14FunctionKey, BobUI::Key_F14 },
    { NSF15FunctionKey, BobUI::Key_F15 },
    { NSF16FunctionKey, BobUI::Key_F16 },
    { NSF17FunctionKey, BobUI::Key_F17 },
    { NSF18FunctionKey, BobUI::Key_F18 },
    { NSF19FunctionKey, BobUI::Key_F19 },
    { NSF20FunctionKey, BobUI::Key_F20 },
    { NSF21FunctionKey, BobUI::Key_F21 },
    { NSF22FunctionKey, BobUI::Key_F22 },
    { NSF23FunctionKey, BobUI::Key_F23 },
    { NSF24FunctionKey, BobUI::Key_F24 },
    { NSF25FunctionKey, BobUI::Key_F25 },
    { NSF26FunctionKey, BobUI::Key_F26 },
    { NSF27FunctionKey, BobUI::Key_F27 },
    { NSF28FunctionKey, BobUI::Key_F28 },
    { NSF29FunctionKey, BobUI::Key_F29 },
    { NSF30FunctionKey, BobUI::Key_F30 },
    { NSF31FunctionKey, BobUI::Key_F31 },
    { NSF32FunctionKey, BobUI::Key_F32 },
    { NSF33FunctionKey, BobUI::Key_F33 },
    { NSF34FunctionKey, BobUI::Key_F34 },
    { NSF35FunctionKey, BobUI::Key_F35 },
    { NSInsertFunctionKey, BobUI::Key_Insert },
    { NSDeleteFunctionKey, BobUI::Key_Delete },
    { NSHomeFunctionKey, BobUI::Key_Home },
    { NSEndFunctionKey, BobUI::Key_End },
    { NSPageUpFunctionKey, BobUI::Key_PageUp },
    { NSPageDownFunctionKey, BobUI::Key_PageDown },
    { NSPrintScreenFunctionKey, BobUI::Key_Print },
    { NSScrollLockFunctionKey, BobUI::Key_ScrollLock },
    { NSPauseFunctionKey, BobUI::Key_Pause },
    { NSSysReqFunctionKey, BobUI::Key_SysReq },
    { NSMenuFunctionKey, BobUI::Key_Menu },
    { NSHelpFunctionKey, BobUI::Key_Help },
};

QChar QAppleKeyMapper::toCocoaKey(BobUI::Key key)
{
    // Prioritize overloaded keys
    if (key == BobUI::Key_Return)
        return char16_t(NSCarriageReturnCharacter);
    if (key == BobUI::Key_Backspace)
        return char16_t(NSBackspaceCharacter);

    Q_CONSTINIT static QHash<BobUI::Key, char16_t> reverseCocoaKeys;
    if (reverseCocoaKeys.isEmpty()) {
        reverseCocoaKeys.reserve(cocoaKeys.size());
        for (auto it = cocoaKeys.begin(); it != cocoaKeys.end(); ++it)
            reverseCocoaKeys.insert(it.value(), it.key());
    }

    return reverseCocoaKeys.value(key);
}

BobUI::Key QAppleKeyMapper::fromCocoaKey(QChar keyCode)
{
    if (auto key = cocoaKeys.value(keyCode.unicode()))
        return key;

    return BobUI::Key(keyCode.toUpper().unicode());
}

// ------------------------------------------------

BobUI::KeyboardModifiers QAppleKeyMapper::queryKeyboardModifiers() const
{
    return fromCocoaModifiers(NSEvent.modifierFlags);
}

bool QAppleKeyMapper::updateKeyboard()
{
    QCFType<TISInputSourceRef> source = TISCopyInputMethodKeyboardLayoutOverride();
    if (!source)
        source = TISCopyCurrentKeyboardInputSource();

    if (m_keyboardMode != NullMode && source == m_currentInputSource)
        return false;

    Q_ASSERT(source);
    m_currentInputSource = source;
    m_keyboardKind = LMGetKbdType();

    m_keyMap.clear();

    if (auto data = CFDataRef(TISGetInputSourceProperty(source, kTISPropertyUnicodeKeyLayoutData))) {
        const UCKeyboardLayout *uchrData = reinterpret_cast<const UCKeyboardLayout *>(CFDataGetBytePtr(data));
        Q_ASSERT(uchrData);
        m_keyboardLayoutFormat = uchrData;
        m_keyboardMode = UnicodeMode;
    } else {
        m_keyboardLayoutFormat = nullptr;
        m_keyboardMode = NullMode;
    }

    qCDebug(lcQpaKeyMapper) << "Updated keyboard to"
        << QString::fromCFString(CFStringRef(TISGetInputSourceProperty(
            m_currentInputSource, kTISPropertyLocalizedName)));

    return true;
}

static constexpr BobUI::KeyboardModifiers modifierCombinations[] = {
    BobUI::NoModifier,                                             // 0
    BobUI::ShiftModifier,                                          // 1
    BobUI::ControlModifier,                                        // 2
    BobUI::ControlModifier | BobUI::ShiftModifier,                    // 3
    BobUI::AltModifier,                                            // 4
    BobUI::AltModifier | BobUI::ShiftModifier,                        // 5
    BobUI::AltModifier | BobUI::ControlModifier,                      // 6
    BobUI::AltModifier | BobUI::ShiftModifier | BobUI::ControlModifier,  // 7
    BobUI::MetaModifier,                                           // 8
    BobUI::MetaModifier | BobUI::ShiftModifier,                       // 9
    BobUI::MetaModifier | BobUI::ControlModifier,                     // 10
    BobUI::MetaModifier | BobUI::ControlModifier | BobUI::ShiftModifier, // 11
    BobUI::MetaModifier | BobUI::AltModifier,                         // 12
    BobUI::MetaModifier | BobUI::AltModifier | BobUI::ShiftModifier,     // 13
    BobUI::MetaModifier | BobUI::AltModifier | BobUI::ControlModifier,   // 14
    BobUI::MetaModifier | BobUI::AltModifier | BobUI::ShiftModifier | BobUI::ControlModifier,  // 15
};

/*
    Returns a key map for the given \virtualKey based on all
    possible modifier combinations.
*/
const QAppleKeyMapper::KeyMap &QAppleKeyMapper::keyMapForKey(VirtualKeyCode virtualKey) const
{
    static_assert(sizeof(modifierCombinations) / sizeof(BobUI::KeyboardModifiers) == kNumModifierCombinations);

    const_cast<QAppleKeyMapper *>(this)->updateKeyboard();

    auto &keyMap = m_keyMap[virtualKey];
    if (keyMap[BobUI::NoModifier] != BobUI::Key_unknown)
        return keyMap; // Already filled

    qCDebug(lcQpaKeyMapper, "Updating key map for virtual key 0x%02x", (uint)virtualKey);

    // Key mapping via [NSEvent charactersByApplyingModifiers:] only works for key down
    // events, but we might (wrongly) get into this code path for other key events such
    // as NSEventTypeFlagsChanged.
    const bool canMapCocoaEvent = NSApp.currentEvent.type == NSEventTypeKeyDown;

    if (!canMapCocoaEvent)
        qCWarning(lcQpaKeyMapper) << "Could not map key to character for event" << NSApp.currentEvent;

    for (int i = 0; i < kNumModifierCombinations; ++i) {
        Q_ASSERT(!i || keyMap[i] == 0);

        auto bobuiModifiers = modifierCombinations[i];
        auto carbonModifiers = toCarbonModifiers(bobuiModifiers);
        const UInt32 modifierKeyState = (carbonModifiers >> 8) & 0xFF;

        UInt32 deadKeyState = 0;
        static const UniCharCount maxStringLength = 10;
        static UniChar unicodeString[maxStringLength];
        UniCharCount actualStringLength = 0;
        OSStatus err = UCKeyTranslate(m_keyboardLayoutFormat, virtualKey,
            kUCKeyActionDown, modifierKeyState, m_keyboardKind,
            kUCKeyTranslateNoDeadKeysMask, &deadKeyState,
            maxStringLength, &actualStringLength,
            unicodeString);

        // Use translated Unicode key if valid
        QChar carbonUnicodeKey;
        if (err == noErr && actualStringLength)
            carbonUnicodeKey = QChar(unicodeString[0]);

        if (canMapCocoaEvent) {
            // Until we've verified that the Cocoa API works as expected
            // we first run the event through the Carbon APIs and then
            // compare the results to Cocoa.
            auto cocoaModifiers = toCocoaModifiers(bobuiModifiers);
            auto *charactersWithModifiers = [NSApp.currentEvent charactersByApplyingModifiers:cocoaModifiers];

            QChar cocoaUnicodeKey;
            if (charactersWithModifiers.length > 0)
                cocoaUnicodeKey = QChar([charactersWithModifiers characterAtIndex:0]);

            if (cocoaUnicodeKey != carbonUnicodeKey) {
                qCWarning(lcQpaKeyMapper) << "Mismatch between Cocoa" << cocoaUnicodeKey
                    << "and Carbon" << carbonUnicodeKey << "for virtual key" << virtualKey
                    << "with" << bobuiModifiers;
            }
        }

        int bobuiKey = toKeyCode(carbonUnicodeKey, virtualKey, bobuiModifiers);
        if (bobuiKey == BobUI::Key_unknown)
            bobuiKey = carbonUnicodeKey.unicode();

        keyMap[i] = bobuiKey;

        qCDebug(lcQpaKeyMapper).verbosity(0) << "\t" << bobuiModifiers
            << "+" << qUtf8Printable(QString::asprintf("0x%02x", virtualKey))
            << "=" << qUtf8Printable(QString::asprintf("%d / 0x%02x /", bobuiKey, bobuiKey))
                   << QKeySequence(bobuiKey).toString();
    }

    return keyMap;
}

/*
    Compute the possible key combinations that can map to the event's
    virtual key and modifiers, in the current keyboard layout.

    For example, given a normal US keyboard layout, the virtual key
    23 combined with the Alt (⌥) and Shift (⇧) modifiers, can map
    to the following key combinations:

        - Alt+Shift+5
        - Alt+%
        - Shift+∞
        - ﬁ

    The function builds on a key map produced by keyMapForKey(),
    where each modifier-key combination has been mapped to the
    key it will produce.
*/
QList<QKeyCombination> QAppleKeyMapper::possibleKeyCombinations(const QKeyEvent *event) const
{
    QList<QKeyCombination> ret;

    const auto nativeVirtualKey = event->nativeVirtualKey();
    if (!nativeVirtualKey)
        return ret;

    auto keyMap = keyMapForKey(nativeVirtualKey);

    auto unmodifiedKey = keyMap[BobUI::NoModifier];
    Q_ASSERT(unmodifiedKey != BobUI::Key_unknown);

    auto eventModifiers = event->modifiers();

    int startingModifierLayer = 0;
    if (toCocoaModifiers(eventModifiers) & NSEventModifierFlagCommand) {
        // When the Command key is pressed AppKit seems to do key equivalent
        // matching using a Latin/Roman interpretation of the current keyboard
        // layout. For example, for a Greek layout, pressing Option+Command+C
        // produces a key event with chars="ç" and unmodchars="ψ", but AppKit
        // still treats this as a match for a key equivalent of Option+Command+C.
        // We can't do the same by just applying the modifiers to our key map,
        // as that too contains "ψ" for the Option+Command combination. What we
        // can do instead is take advantage of the fact that the Command
        // modifier layer in all/most keyboard layouts contains a Latin
        // layer. We then combine that with the modifiers of the event
        // to produce the resulting "Latin" key combination.

        // Depending on whether BobUI::AA_MacDontSwapCtrlAndMeta is set or not
        // the index of the Command layer in modifierCombinations will differ.
        const int commandLayer = qApp->testAttribute(BobUI::AA_MacDontSwapCtrlAndMeta) ? 8 : 2;
        // FIXME: We don't clear the key map when BobUI::AA_MacDontSwapCtrlAndMeta
        // is set, so changing BobUI::AA_MacDontSwapCtrlAndMeta at runtime will
        // fail once we've built the key map.
        ret << QKeyCombination::fromCombined(int(eventModifiers) + int(keyMap[commandLayer]));

        // If the unmodified key is outside of Latin1, we also treat
        // that as a valid key combination, even if AppKit natively
        // does not. For example, for a Greek layout, we still want
        // to support Option+Command+ψ as a key combination, as it's
        // unlikely to clash with the Latin key combination we added
        // above.

        // However, if the unmodified key is within Latin1, we skip
        // it, to avoid these types of conflicts. For example, in
        // the same Greek layout, pressing the key next to Tab will
        // produce a Latin ';' symbol, but we've already treated that
        // as 'q' above, thanks to the Command modifier, so we skip
        // the potential Command+; key combination. This is also in
        // line with what AppKit natively does.

        // Skipping Latin1 unmodified keys also handles the case of
        // a Latin layout, where the unmodified and modified keys
        // are the same.

        if (unmodifiedKey <= 0xff)
            startingModifierLayer = 1;
    }

    // FIXME: We only compute the first 8 combinations. Why?
    for (int i = startingModifierLayer; i < 15; ++i) {
        auto keyAfterApplyingModifiers = keyMap[i];
        if (!keyAfterApplyingModifiers)
             continue;

        // Include key if the event modifiers match exactly,
        // or are a superset of the current candidate modifiers.
        auto candidateModifiers = modifierCombinations[i];
        if ((eventModifiers & candidateModifiers) == candidateModifiers) {
            // If the event includes more modifiers than the candidate they
            // will need to be included in the resulting key combination.
            auto additionalModifiers = eventModifiers & ~candidateModifiers;

            auto keyCombination = QKeyCombination::fromCombined(
                int(additionalModifiers) + int(keyAfterApplyingModifiers));

            // If there's an existing key combination with the same key,
            // but a different set of modifiers, we want to choose only
            // one of them, by priority (see below).
            const auto existingCombination = std::find_if(
                ret.begin(), ret.end(), [&](auto existingCombination) {
                    return existingCombination.key() == keyAfterApplyingModifiers;
                });

            if (existingCombination != ret.end()) {
                // We prioritize the combination with the more specific
                // modifiers. In the case where the number of modifiers
                // are the same, we want to prioritize Command over Option
                // over Control over Shift. Unfortunately the order (and
                // hence value) of the modifiers in BobUI::KeyboardModifier
                // does not match our preferred order when Control and
                // Meta is switched, but we can work around that by
                // explicitly swapping the modifiers and using that
                // for the comparison. This also works when the
                // BobUI::AA_MacDontSwapCtrlAndMeta application attribute
                // is set, as the incoming modifiers are then left
                // as is, and we can still trust the order.
                auto existingModifiers = swapModifiersIfNeeded(existingCombination->keyboardModifiers());
                auto replacementModifiers = swapModifiersIfNeeded(additionalModifiers);
                if (replacementModifiers > existingModifiers)
                    *existingCombination = keyCombination;
            } else {
                // All is good, no existing combination has this key
                ret << keyCombination;
            }
        }
    }

    return ret;
}



#else // iOS

BobUI::Key QAppleKeyMapper::fromNSString(BobUI::KeyboardModifiers bobuiModifiers, NSString *characters,
                                      NSString *charactersIgnoringModifiers, QString &text)
{
    if ([characters isEqualToString:@"\t"]) {
        if (bobuiModifiers & BobUI::ShiftModifier)
            return BobUI::Key_Backtab;
        return BobUI::Key_Tab;
    } else if ([characters isEqualToString:@"\r"]) {
        if (bobuiModifiers & BobUI::KeypadModifier)
            return BobUI::Key_Enter;
        return BobUI::Key_Return;
    }
    if ([characters length] != 0 || [charactersIgnoringModifiers length] != 0) {
        QChar ch;
        if (((bobuiModifiers & BobUI::MetaModifier) || (bobuiModifiers & BobUI::AltModifier)) &&
            ([charactersIgnoringModifiers length] != 0)) {
            ch = QChar([charactersIgnoringModifiers characterAtIndex:0]);
        } else if ([characters length] != 0) {
            ch = QChar([characters characterAtIndex:0]);
        }
        if (!(bobuiModifiers & (BobUI::ControlModifier | BobUI::MetaModifier)) &&
            (ch.unicode() < 0xf700 || ch.unicode() > 0xf8ff)) {
            text = QString::fromNSString(characters);
        }
        if (!ch.isNull())
            return BobUI::Key(ch.toUpper().unicode());
    }
    return BobUI::Key_unknown;
}

// Keyboard keys (non-modifiers)
API_AVAILABLE(ios(13.4)) BobUI::Key QAppleKeyMapper::fromUIKitKey(NSString *keyCode)
{
    static QHash<NSString *, BobUI::Key> uiKitKeys = {
        { UIKeyInputF1, BobUI::Key_F1 },
        { UIKeyInputF2, BobUI::Key_F2 },
        { UIKeyInputF3, BobUI::Key_F3 },
        { UIKeyInputF4, BobUI::Key_F4 },
        { UIKeyInputF5, BobUI::Key_F5 },
        { UIKeyInputF6, BobUI::Key_F6 },
        { UIKeyInputF7, BobUI::Key_F7 },
        { UIKeyInputF8, BobUI::Key_F8 },
        { UIKeyInputF9, BobUI::Key_F9 },
        { UIKeyInputF10, BobUI::Key_F10 },
        { UIKeyInputF11, BobUI::Key_F11 },
        { UIKeyInputF12, BobUI::Key_F12 },
        { UIKeyInputHome, BobUI::Key_Home },
        { UIKeyInputEnd, BobUI::Key_End },
        { UIKeyInputPageUp, BobUI::Key_PageUp },
        { UIKeyInputPageDown, BobUI::Key_PageDown },
        { UIKeyInputEscape, BobUI::Key_Escape },
        { UIKeyInputUpArrow, BobUI::Key_Up },
        { UIKeyInputDownArrow, BobUI::Key_Down },
        { UIKeyInputLeftArrow, BobUI::Key_Left },
        { UIKeyInputRightArrow, BobUI::Key_Right }
    };

    if (auto key = uiKitKeys.value(keyCode))
        return key;

    return BobUI::Key_unknown;
}

static constexpr std::tuple<ulong, BobUI::KeyboardModifier> uiKitModifierMap[] = {
    { UIKeyModifierShift, BobUI::ShiftModifier },
    { UIKeyModifierControl, BobUI::ControlModifier },
    { UIKeyModifierCommand, BobUI::MetaModifier },
    { UIKeyModifierAlternate, BobUI::AltModifier },
    { UIKeyModifierNumericPad, BobUI::KeypadModifier }
};

ulong QAppleKeyMapper::toUIKitModifiers(BobUI::KeyboardModifiers bobuiModifiers)
{
    bobuiModifiers = swapModifiersIfNeeded(bobuiModifiers);

    ulong nativeModifiers = 0;
    for (const auto &[nativeModifier, bobuiModifier] : uiKitModifierMap) {
        if (bobuiModifiers & bobuiModifier)
            nativeModifiers |= nativeModifier;
    }

    return nativeModifiers;
}

BobUI::KeyboardModifiers QAppleKeyMapper::fromUIKitModifiers(ulong nativeModifiers)
{
    BobUI::KeyboardModifiers bobuiModifiers = BobUI::NoModifier;
    for (const auto &[nativeModifier, bobuiModifier] : uiKitModifierMap) {
        if (nativeModifiers & nativeModifier)
            bobuiModifiers |= bobuiModifier;
    }

    return swapModifiersIfNeeded(bobuiModifiers);
}
#endif

BOBUI_END_NAMESPACE
