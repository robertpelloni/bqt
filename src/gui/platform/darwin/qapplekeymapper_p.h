// Copyright (C) 2021 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QAPPLEKEYMAPPER_H
#define QAPPLEKEYMAPPER_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the BobUI API.  It exists purely as an
// implementation detail.  This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//

#ifdef Q_OS_MACOS
#include <Carbon/Carbon.h>
#endif

#include <qpa/qplatformkeymapper.h>

#include <BobUICore/QList>
#include <BobUICore/QHash>
#include <BobUIGui/QKeyEvent>

#include <BobUICore/private/qcore_mac_p.h>

BOBUI_BEGIN_NAMESPACE

class Q_GUI_EXPORT QAppleKeyMapper : public QPlatformKeyMapper
{
public:
    BobUI::KeyboardModifiers queryKeyboardModifiers() const override;
    QList<QKeyCombination> possibleKeyCombinations(const QKeyEvent *event) const override;

#ifdef Q_OS_MACOS
    static BobUI::KeyboardModifiers fromCocoaModifiers(NSEventModifierFlags cocoaModifiers);
    static NSEventModifierFlags toCocoaModifiers(BobUI::KeyboardModifiers);

    static QChar toCocoaKey(BobUI::Key key);
    static BobUI::Key fromCocoaKey(QChar keyCode);
#else
    static BobUI::Key fromNSString(BobUI::KeyboardModifiers bobuiMods, NSString *characters,
                            NSString *charactersIgnoringModifiers, QString &text);

    static BobUI::Key fromUIKitKey(NSString *keyCode);
    static BobUI::KeyboardModifiers fromUIKitModifiers(ulong uikitModifiers);
    static ulong toUIKitModifiers(BobUI::KeyboardModifiers);
#endif
private:
#ifdef Q_OS_MACOS
    static constexpr int kNumModifierCombinations = 16;
    struct KeyMap : std::array<char32_t, kNumModifierCombinations>
    {
        // Initialize first element to a sentinel that allows us
        // to distinguish an uninitialized map from an initialized.
        // Using 0 would not allow us to map U+0000 (NUL), however
        // unlikely that is.
        KeyMap() : std::array<char32_t, 16>{BobUI::Key_unknown} {}
    };

    bool updateKeyboard();

    using VirtualKeyCode = unsigned short;
    const KeyMap &keyMapForKey(VirtualKeyCode virtualKey) const;

    QCFType<TISInputSourceRef> m_currentInputSource = nullptr;

    enum { NullMode, UnicodeMode, OtherMode } m_keyboardMode = NullMode;
    const UCKeyboardLayout *m_keyboardLayoutFormat = nullptr;
    KeyboardLayoutKind m_keyboardKind = kKLKCHRuchrKind;

    mutable QHash<VirtualKeyCode, KeyMap> m_keyMap;
#endif
};

BOBUI_END_NAMESPACE

#endif

