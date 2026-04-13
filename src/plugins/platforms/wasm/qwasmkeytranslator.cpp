// Copyright (C) 2023 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

#include "qwasmkeytranslator.h"
#include "qwasmevent.h"

#include <BobUICore/private/qmakearray_p.h>
#include <BobUICore/qglobal.h>
#include <BobUICore/qobject.h>

#include <algorithm>

BOBUI_BEGIN_NAMESPACE

namespace {
struct WebKb2BobUIData
{
    static constexpr char StringTerminator = '\0';

    const char *web;
    unsigned int bobui;

    constexpr bool operator<=(const WebKb2BobUIData &that) const noexcept
    {
        return !(strcmp(that) > 0);
    }

    bool operator<(const WebKb2BobUIData &that) const noexcept { return ::strcmp(web, that.web) < 0; }

    constexpr bool operator==(const WebKb2BobUIData &that) const noexcept { return strcmp(that) == 0; }

    constexpr int strcmp(const WebKb2BobUIData &that, const int i = 0) const
    {
        return web[i] == StringTerminator && that.web[i] == StringTerminator ? 0
                : web[i] == StringTerminator                                 ? -1
                : that.web[i] == StringTerminator                            ? 1
                : web[i] < that.web[i]                                       ? -1
                : web[i] > that.web[i]                                       ? 1
                                                                             : strcmp(that, i + 1);
    }
};

template<unsigned int BobUI, char... WebChar>
struct Web2BobUI
{
    static constexpr const char storage[sizeof...(WebChar) + 1] = { WebChar..., '\0' };
    using Type = WebKb2BobUIData;
    static constexpr Type data() noexcept { return Type{ storage, BobUI }; }
};

template<unsigned int BobUI, char... WebChar>
constexpr char Web2BobUI<BobUI, WebChar...>::storage[];

static constexpr const auto WebToBobUIKeyCodeMappings = qMakeArray(
        QSortedData<Web2BobUI<BobUI::Key_Alt, 'A', 'l', 't', 'L', 'e', 'f', 't'>,
                    Web2BobUI<BobUI::Key_Alt, 'A', 'l', 't'>,
                    Web2BobUI<BobUI::Key_AltGr, 'A', 'l', 't', 'R', 'i', 'g', 'h', 't'>,
                    Web2BobUI<BobUI::Key_Apostrophe, 'Q', 'u', 'o', 't', 'e'>,
                    Web2BobUI<BobUI::Key_Backspace, 'B', 'a', 'c', 'k', 's', 'p', 'a', 'c', 'e'>,
                    Web2BobUI<BobUI::Key_CapsLock, 'C', 'a', 'p', 's', 'L', 'o', 'c', 'k'>,
                    Web2BobUI<BobUI::Key_Control, 'C', 'o', 'n', 't', 'r', 'o', 'l'>,
                    Web2BobUI<BobUI::Key_Delete, 'D', 'e', 'l', 'e', 't', 'e'>,
                    Web2BobUI<BobUI::Key_Down, 'A', 'r', 'r', 'o', 'w', 'D', 'o', 'w', 'n'>,
                    Web2BobUI<BobUI::Key_Escape, 'E', 's', 'c', 'a', 'p', 'e'>,
                    Web2BobUI<BobUI::Key_F1, 'F', '1'>, Web2BobUI<BobUI::Key_F2, 'F', '2'>,
                    Web2BobUI<BobUI::Key_F11, 'F', '1', '1'>, Web2BobUI<BobUI::Key_F12, 'F', '1', '2'>,
                    Web2BobUI<BobUI::Key_F13, 'F', '1', '3'>, Web2BobUI<BobUI::Key_F14, 'F', '1', '4'>,
                    Web2BobUI<BobUI::Key_F15, 'F', '1', '5'>, Web2BobUI<BobUI::Key_F16, 'F', '1', '6'>,
                    Web2BobUI<BobUI::Key_F17, 'F', '1', '7'>, Web2BobUI<BobUI::Key_F18, 'F', '1', '8'>,
                    Web2BobUI<BobUI::Key_F19, 'F', '1', '9'>, Web2BobUI<BobUI::Key_F20, 'F', '2', '0'>,
                    Web2BobUI<BobUI::Key_F21, 'F', '2', '1'>, Web2BobUI<BobUI::Key_F22, 'F', '2', '2'>,
                    Web2BobUI<BobUI::Key_F23, 'F', '2', '3'>,
                    Web2BobUI<BobUI::Key_F3, 'F', '3'>, Web2BobUI<BobUI::Key_F4, 'F', '4'>,
                    Web2BobUI<BobUI::Key_F5, 'F', '5'>, Web2BobUI<BobUI::Key_F6, 'F', '6'>,
                    Web2BobUI<BobUI::Key_F7, 'F', '7'>, Web2BobUI<BobUI::Key_F8, 'F', '8'>,
                    Web2BobUI<BobUI::Key_F9, 'F', '9'>, Web2BobUI<BobUI::Key_F10, 'F', '1', '0'>,
                    Web2BobUI<BobUI::Key_Help, 'H', 'e', 'l', 'p'>,
                    Web2BobUI<BobUI::Key_Home, 'H', 'o', 'm', 'e'>, Web2BobUI<BobUI::Key_End, 'E', 'n', 'd'>,
                    Web2BobUI<BobUI::Key_Insert, 'I', 'n', 's', 'e', 'r', 't'>,
                    Web2BobUI<BobUI::Key_Left, 'A', 'r', 'r', 'o', 'w', 'L', 'e', 'f', 't'>,
                    Web2BobUI<BobUI::Key_Meta, 'M', 'e', 't', 'a'>, Web2BobUI<BobUI::Key_Meta, 'O', 'S'>,
                    Web2BobUI<BobUI::Key_Menu, 'C', 'o', 'n', 't', 'e', 'x', 't', 'M', 'e', 'n', 'u'>,
                    Web2BobUI<BobUI::Key_NumLock, 'N', 'u', 'm', 'L', 'o', 'c', 'k'>,
                    Web2BobUI<BobUI::Key_PageDown, 'P', 'a', 'g', 'e', 'D', 'o', 'w', 'n'>,
                    Web2BobUI<BobUI::Key_PageUp, 'P', 'a', 'g', 'e', 'U', 'p'>,
                    Web2BobUI<BobUI::Key_Paste, 'P', 'a', 's', 't', 'e'>,
                    Web2BobUI<BobUI::Key_Pause, 'C', 'l', 'e', 'a', 'r'>,
                    Web2BobUI<BobUI::Key_Pause, 'P', 'a', 'u', 's', 'e'>,
                    Web2BobUI<BobUI::Key_QuoteLeft, 'B', 'a', 'c', 'k', 'q', 'u', 'o', 't', 'e'>,
                    Web2BobUI<BobUI::Key_QuoteLeft, 'I', 'n', 't', 'l', 'B', 'a', 'c', 'k', 's', 'l', 'a', 's', 'h'>,
                    Web2BobUI<BobUI::Key_Return, 'E', 'n', 't', 'e', 'r'>,
                    Web2BobUI<BobUI::Key_Right, 'A', 'r', 'r', 'o', 'w', 'R', 'i', 'g', 'h', 't'>,
                    Web2BobUI<BobUI::Key_ScrollLock, 'S', 'c', 'r', 'o', 'l', 'l', 'L', 'o', 'c', 'k'>,
                    Web2BobUI<BobUI::Key_Shift, 'S', 'h', 'i', 'f', 't'>,
                    Web2BobUI<BobUI::Key_Tab, 'T', 'a', 'b'>,
                    Web2BobUI<BobUI::Key_Up, 'A', 'r', 'r', 'o', 'w', 'U', 'p'>,
                    Web2BobUI<BobUI::Key_yen, 'I', 'n', 't', 'l', 'Y', 'e', 'n'>>::Data{});

static constexpr const auto DiacriticalCharsKeyToTextLowercase = qMakeArray(
        QSortedData<
                Web2BobUI<BobUI::Key_Aacute, '\xc3', '\xa1'>,
                Web2BobUI<BobUI::Key_Acircumflex, '\xc3', '\xa2'>,
                Web2BobUI<BobUI::Key_Adiaeresis, '\xc3', '\xa4'>,
                Web2BobUI<BobUI::Key_AE, '\xc3', '\xa6'>,
                Web2BobUI<BobUI::Key_Agrave, '\xc3', '\xa0'>,
                Web2BobUI<BobUI::Key_Aring, '\xc3', '\xa5'>,
                Web2BobUI<BobUI::Key_Atilde, '\xc3', '\xa3'>,
                Web2BobUI<BobUI::Key_Ccedilla, '\xc3', '\xa7'>,
                Web2BobUI<BobUI::Key_Eacute, '\xc3', '\xa9'>,
                Web2BobUI<BobUI::Key_Ecircumflex, '\xc3', '\xaa'>,
                Web2BobUI<BobUI::Key_Ediaeresis, '\xc3', '\xab'>,
                Web2BobUI<BobUI::Key_Egrave, '\xc3', '\xa8'>,
                Web2BobUI<BobUI::Key_Iacute, '\xc3', '\xad'>,
                Web2BobUI<BobUI::Key_Icircumflex, '\xc3', '\xae'>,
                Web2BobUI<BobUI::Key_Idiaeresis, '\xc3', '\xaf'>,
                Web2BobUI<BobUI::Key_Igrave, '\xc3', '\xac'>,
                Web2BobUI<BobUI::Key_Ntilde, '\xc3', '\xb1'>,
                Web2BobUI<BobUI::Key_Oacute, '\xc3', '\xb3'>,
                Web2BobUI<BobUI::Key_Ocircumflex, '\xc3', '\xb4'>,
                Web2BobUI<BobUI::Key_Odiaeresis, '\xc3', '\xb6'>,
                Web2BobUI<BobUI::Key_Ograve, '\xc3', '\xb2'>,
                Web2BobUI<BobUI::Key_Ooblique, '\xc3', '\xb8'>,
                Web2BobUI<BobUI::Key_Otilde, '\xc3', '\xb5'>,
                Web2BobUI<BobUI::Key_Uacute, '\xc3', '\xba'>,
                Web2BobUI<BobUI::Key_Ucircumflex, '\xc3', '\xbb'>,
                Web2BobUI<BobUI::Key_Udiaeresis, '\xc3', '\xbc'>,
                Web2BobUI<BobUI::Key_Ugrave, '\xc3', '\xb9'>,
                Web2BobUI<BobUI::Key_Yacute, '\xc3', '\xbd'>,
                Web2BobUI<BobUI::Key_ydiaeresis, '\xc3', '\xbf'>>::Data{});

static constexpr const auto DiacriticalCharsKeyToTextUppercase = qMakeArray(
        QSortedData<
                Web2BobUI<BobUI::Key_Aacute, '\xc3', '\x81'>,
                Web2BobUI<BobUI::Key_Acircumflex, '\xc3', '\x82'>,
                Web2BobUI<BobUI::Key_Adiaeresis, '\xc3', '\x84'>,
                Web2BobUI<BobUI::Key_AE, '\xc3', '\x86'>,
                Web2BobUI<BobUI::Key_Agrave, '\xc3', '\x80'>,
                Web2BobUI<BobUI::Key_Aring, '\xc3', '\x85'>,
                Web2BobUI<BobUI::Key_Atilde, '\xc3', '\x83'>,
                Web2BobUI<BobUI::Key_Ccedilla, '\xc3', '\x87'>,
                Web2BobUI<BobUI::Key_Eacute, '\xc3', '\x89'>,
                Web2BobUI<BobUI::Key_Ecircumflex, '\xc3', '\x8a'>,
                Web2BobUI<BobUI::Key_Ediaeresis, '\xc3', '\x8b'>,
                Web2BobUI<BobUI::Key_Egrave, '\xc3', '\x88'>,
                Web2BobUI<BobUI::Key_Iacute, '\xc3', '\x8d'>,
                Web2BobUI<BobUI::Key_Icircumflex, '\xc3', '\x8e'>,
                Web2BobUI<BobUI::Key_Idiaeresis, '\xc3', '\x8f'>,
                Web2BobUI<BobUI::Key_Igrave, '\xc3', '\x8c'>,
                Web2BobUI<BobUI::Key_Ntilde, '\xc3', '\x91'>,
                Web2BobUI<BobUI::Key_Oacute, '\xc3', '\x93'>,
                Web2BobUI<BobUI::Key_Ocircumflex, '\xc3', '\x94'>,
                Web2BobUI<BobUI::Key_Odiaeresis, '\xc3', '\x96'>,
                Web2BobUI<BobUI::Key_Ograve, '\xc3', '\x92'>,
                Web2BobUI<BobUI::Key_Ooblique, '\xc3', '\x98'>,
                Web2BobUI<BobUI::Key_Otilde, '\xc3', '\x95'>,
                Web2BobUI<BobUI::Key_Uacute, '\xc3', '\x9a'>,
                Web2BobUI<BobUI::Key_Ucircumflex, '\xc3', '\x9b'>,
                Web2BobUI<BobUI::Key_Udiaeresis, '\xc3', '\x9c'>,
                Web2BobUI<BobUI::Key_Ugrave, '\xc3', '\x99'>,
                Web2BobUI<BobUI::Key_Yacute, '\xc3', '\x9d'>,
                Web2BobUI<BobUI::Key_ydiaeresis, '\xc5', '\xb8'>>::Data{});

static_assert(DiacriticalCharsKeyToTextLowercase.size()
                      == DiacriticalCharsKeyToTextUppercase.size(),
              "Add the new key to both arrays");

struct KeyMapping
{
    BobUI::Key from, to;
};

constexpr KeyMapping tildeKeyTable[] = {
    // ~
    { BobUI::Key_A, BobUI::Key_Atilde },
    { BobUI::Key_N, BobUI::Key_Ntilde },
    { BobUI::Key_O, BobUI::Key_Otilde },
};
constexpr KeyMapping graveKeyTable[] = {
    // `
    { BobUI::Key_A, BobUI::Key_Agrave }, { BobUI::Key_E, BobUI::Key_Egrave }, { BobUI::Key_I, BobUI::Key_Igrave },
    { BobUI::Key_O, BobUI::Key_Ograve }, { BobUI::Key_U, BobUI::Key_Ugrave },
};
constexpr KeyMapping acuteKeyTable[] = {
    // '
    { BobUI::Key_A, BobUI::Key_Aacute }, { BobUI::Key_E, BobUI::Key_Eacute }, { BobUI::Key_I, BobUI::Key_Iacute },
    { BobUI::Key_O, BobUI::Key_Oacute }, { BobUI::Key_U, BobUI::Key_Uacute }, { BobUI::Key_Y, BobUI::Key_Yacute },
};
constexpr KeyMapping diaeresisKeyTable[] = {
    // umlaut ¨
    { BobUI::Key_A, BobUI::Key_Adiaeresis }, { BobUI::Key_E, BobUI::Key_Ediaeresis },
    { BobUI::Key_I, BobUI::Key_Idiaeresis }, { BobUI::Key_O, BobUI::Key_Odiaeresis },
    { BobUI::Key_U, BobUI::Key_Udiaeresis }, { BobUI::Key_Y, BobUI::Key_ydiaeresis },
};
constexpr KeyMapping circumflexKeyTable[] = {
    // ^
    { BobUI::Key_A, BobUI::Key_Acircumflex }, { BobUI::Key_E, BobUI::Key_Ecircumflex },
    { BobUI::Key_I, BobUI::Key_Icircumflex }, { BobUI::Key_O, BobUI::Key_Ocircumflex },
    { BobUI::Key_U, BobUI::Key_Ucircumflex },
};

static BobUI::Key find_impl(const KeyMapping *first, const KeyMapping *last, BobUI::Key key) noexcept
{
    while (first != last) {
        if (first->from == key)
            return first->to;
        ++first;
    }
    return BobUI::Key_unknown;
}

template<size_t N>
static BobUI::Key find(const KeyMapping (&map)[N], BobUI::Key key) noexcept
{
    return find_impl(map, map + N, key);
}

BobUI::Key translateBaseKeyUsingDeadKey(BobUI::Key accentBaseKey, BobUI::Key deadKey)
{
    switch (deadKey) {
    case BobUI::Key_Dead_Grave:
        return find(graveKeyTable, accentBaseKey);
    case BobUI::Key_Dead_Acute:
        return find(acuteKeyTable, accentBaseKey);
    case BobUI::Key_Dead_Tilde:
        return find(tildeKeyTable, accentBaseKey);
    case BobUI::Key_Dead_Diaeresis:
        return find(diaeresisKeyTable, accentBaseKey);
    case BobUI::Key_Dead_Circumflex:
        return find(circumflexKeyTable, accentBaseKey);
    default:
        return BobUI::Key_unknown;
    };
}

template<class T>
std::optional<QString> findKeyTextByKeyId(const T &mappingArray, BobUI::Key bobuiKey)
{
    const auto it = std::find_if(mappingArray.cbegin(), mappingArray.cend(),
                                 [bobuiKey](const WebKb2BobUIData &data) { return data.bobui == bobuiKey; });
    return it != mappingArray.cend() ? it->web : std::optional<QString>();
}
} // namespace

std::optional<BobUI::Key> QWasmKeyTranslator::mapWebKeyTextToBobUIKey(const char *toFind)
{
    const WebKb2BobUIData searchKey{ toFind, 0 };
    const auto it = std::lower_bound(WebToBobUIKeyCodeMappings.cbegin(), WebToBobUIKeyCodeMappings.cend(),
                                     searchKey);
    return it != WebToBobUIKeyCodeMappings.cend() && searchKey == *it ? static_cast<BobUI::Key>(it->bobui)
                                                                   : std::optional<BobUI::Key>();
}

BOBUI_END_NAMESPACE
