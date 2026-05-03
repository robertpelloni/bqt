// Copyright (C) 2019 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#include "qxkbcommon_p.h"

#include <private/qmakearray_p.h>

#include <BobUICore/private/qstringiterator_p.h>
#include <BobUICore/qvarlengtharray.h>
#include <BobUICore/QMetaMethod>

#include <BobUIGui/QKeyEvent>
#include <BobUIGui/private/qguiapplication_p.h>

#include <qpa/qplatforminputcontext.h>
#include <qpa/qplatformintegration.h>

BOBUI_BEGIN_NAMESPACE

static int keysymToBobUIKey_internal(xkb_keysym_t keysym, BobUI::KeyboardModifiers modifiers,
                                  xkb_state *state, xkb_keycode_t code,
                                  bool superAsMeta, bool hyperAsMeta);

typedef struct xkb2bobui
{
    unsigned int xkb;
    unsigned int bobui;

    constexpr bool operator <=(const xkb2bobui &that) const noexcept
    {
        return xkb <= that.xkb;
    }

    constexpr bool operator <(const xkb2bobui &that) const noexcept
    {
        return xkb < that.xkb;
    }
} xkb2bobui_t;

template<std::size_t Xkb, std::size_t BobUI>
struct Xkb2BobUI
{
    using Type = xkb2bobui_t;
    static constexpr Type data() noexcept { return Type{Xkb, BobUI}; }
};

static constexpr const auto KeyTbl = qMakeArray(
    QSortedData<
        // misc keys

        Xkb2BobUI<XKB_KEY_Escape,                  BobUI::Key_Escape>,
        Xkb2BobUI<XKB_KEY_Tab,                     BobUI::Key_Tab>,
        Xkb2BobUI<XKB_KEY_ISO_Left_Tab,            BobUI::Key_Backtab>,
        Xkb2BobUI<XKB_KEY_BackSpace,               BobUI::Key_Backspace>,
        Xkb2BobUI<XKB_KEY_Return,                  BobUI::Key_Return>,
        Xkb2BobUI<XKB_KEY_Insert,                  BobUI::Key_Insert>,
        Xkb2BobUI<XKB_KEY_Delete,                  BobUI::Key_Delete>,
        Xkb2BobUI<XKB_KEY_Clear,                   BobUI::Key_Delete>,
        Xkb2BobUI<XKB_KEY_Pause,                   BobUI::Key_Pause>,
        Xkb2BobUI<XKB_KEY_Print,                   BobUI::Key_Print>,
        Xkb2BobUI<XKB_KEY_Sys_Req,                 BobUI::Key_SysReq>,
        Xkb2BobUI<0x1005FF60,                      BobUI::Key_SysReq>,         // hardcoded Sun SysReq
        Xkb2BobUI<0x1007ff00,                      BobUI::Key_SysReq>,         // hardcoded X386 SysReq

        // cursor movement

        Xkb2BobUI<XKB_KEY_Home,                    BobUI::Key_Home>,
        Xkb2BobUI<XKB_KEY_End,                     BobUI::Key_End>,
        Xkb2BobUI<XKB_KEY_Left,                    BobUI::Key_Left>,
        Xkb2BobUI<XKB_KEY_Up,                      BobUI::Key_Up>,
        Xkb2BobUI<XKB_KEY_Right,                   BobUI::Key_Right>,
        Xkb2BobUI<XKB_KEY_Down,                    BobUI::Key_Down>,
        Xkb2BobUI<XKB_KEY_Prior,                   BobUI::Key_PageUp>,
        Xkb2BobUI<XKB_KEY_Next,                    BobUI::Key_PageDown>,

        // modifiers

        Xkb2BobUI<XKB_KEY_Shift_L,                 BobUI::Key_Shift>,
        Xkb2BobUI<XKB_KEY_Shift_R,                 BobUI::Key_Shift>,
        Xkb2BobUI<XKB_KEY_Shift_Lock,              BobUI::Key_Shift>,
        Xkb2BobUI<XKB_KEY_Control_L,               BobUI::Key_Control>,
        Xkb2BobUI<XKB_KEY_Control_R,               BobUI::Key_Control>,
        Xkb2BobUI<XKB_KEY_Meta_L,                  BobUI::Key_Meta>,
        Xkb2BobUI<XKB_KEY_Meta_R,                  BobUI::Key_Meta>,
        Xkb2BobUI<XKB_KEY_Alt_L,                   BobUI::Key_Alt>,
        Xkb2BobUI<XKB_KEY_Alt_R,                   BobUI::Key_Alt>,
        Xkb2BobUI<XKB_KEY_Caps_Lock,               BobUI::Key_CapsLock>,
        Xkb2BobUI<XKB_KEY_Num_Lock,                BobUI::Key_NumLock>,
        Xkb2BobUI<XKB_KEY_Scroll_Lock,             BobUI::Key_ScrollLock>,
        Xkb2BobUI<XKB_KEY_Super_L,                 BobUI::Key_Super_L>,
        Xkb2BobUI<XKB_KEY_Super_R,                 BobUI::Key_Super_R>,
        Xkb2BobUI<XKB_KEY_Menu,                    BobUI::Key_Menu>,
        Xkb2BobUI<XKB_KEY_Hyper_L,                 BobUI::Key_Hyper_L>,
        Xkb2BobUI<XKB_KEY_Hyper_R,                 BobUI::Key_Hyper_R>,
        Xkb2BobUI<XKB_KEY_Help,                    BobUI::Key_Help>,
        Xkb2BobUI<0x1000FF74,                      BobUI::Key_Backtab>,        // hardcoded HP backtab
        Xkb2BobUI<0x1005FF10,                      BobUI::Key_F11>,            // hardcoded Sun F36 (labeled F11)
        Xkb2BobUI<0x1005FF11,                      BobUI::Key_F12>,            // hardcoded Sun F37 (labeled F12)

        // numeric and function keypad keys

        Xkb2BobUI<XKB_KEY_KP_Space,                BobUI::Key_Space>,
        Xkb2BobUI<XKB_KEY_KP_Tab,                  BobUI::Key_Tab>,
        Xkb2BobUI<XKB_KEY_KP_Enter,                BobUI::Key_Enter>,
        Xkb2BobUI<XKB_KEY_KP_Home,                 BobUI::Key_Home>,
        Xkb2BobUI<XKB_KEY_KP_Left,                 BobUI::Key_Left>,
        Xkb2BobUI<XKB_KEY_KP_Up,                   BobUI::Key_Up>,
        Xkb2BobUI<XKB_KEY_KP_Right,                BobUI::Key_Right>,
        Xkb2BobUI<XKB_KEY_KP_Down,                 BobUI::Key_Down>,
        Xkb2BobUI<XKB_KEY_KP_Prior,                BobUI::Key_PageUp>,
        Xkb2BobUI<XKB_KEY_KP_Next,                 BobUI::Key_PageDown>,
        Xkb2BobUI<XKB_KEY_KP_End,                  BobUI::Key_End>,
        Xkb2BobUI<XKB_KEY_KP_Begin,                BobUI::Key_Clear>,
        Xkb2BobUI<XKB_KEY_KP_Insert,               BobUI::Key_Insert>,
        Xkb2BobUI<XKB_KEY_KP_Delete,               BobUI::Key_Delete>,
        Xkb2BobUI<XKB_KEY_KP_Equal,                BobUI::Key_Equal>,
        Xkb2BobUI<XKB_KEY_KP_Multiply,             BobUI::Key_Asterisk>,
        Xkb2BobUI<XKB_KEY_KP_Add,                  BobUI::Key_Plus>,
        Xkb2BobUI<XKB_KEY_KP_Separator,            BobUI::Key_Comma>,
        Xkb2BobUI<XKB_KEY_KP_Subtract,             BobUI::Key_Minus>,
        Xkb2BobUI<XKB_KEY_KP_Decimal,              BobUI::Key_Period>,
        Xkb2BobUI<XKB_KEY_KP_Divide,               BobUI::Key_Slash>,

        // special non-XF86 function keys

        Xkb2BobUI<XKB_KEY_Undo,                    BobUI::Key_Undo>,
        Xkb2BobUI<XKB_KEY_Redo,                    BobUI::Key_Redo>,
        Xkb2BobUI<XKB_KEY_Find,                    BobUI::Key_Find>,
        Xkb2BobUI<XKB_KEY_Cancel,                  BobUI::Key_Cancel>,

        // International input method support keys

        // International & multi-key character composition
        Xkb2BobUI<XKB_KEY_ISO_Level3_Shift,        BobUI::Key_AltGr>,
        Xkb2BobUI<XKB_KEY_Multi_key,               BobUI::Key_Multi_key>,
        Xkb2BobUI<XKB_KEY_Codeinput,               BobUI::Key_Codeinput>,
        Xkb2BobUI<XKB_KEY_SingleCandidate,         BobUI::Key_SingleCandidate>,
        Xkb2BobUI<XKB_KEY_MultipleCandidate,       BobUI::Key_MultipleCandidate>,
        Xkb2BobUI<XKB_KEY_PreviousCandidate,       BobUI::Key_PreviousCandidate>,

        // Misc Functions
        Xkb2BobUI<XKB_KEY_Mode_switch,             BobUI::Key_Mode_switch>,
        Xkb2BobUI<XKB_KEY_script_switch,           BobUI::Key_Mode_switch>,

        // Japanese keyboard support
        Xkb2BobUI<XKB_KEY_Kanji,                   BobUI::Key_Kanji>,
        Xkb2BobUI<XKB_KEY_Muhenkan,                BobUI::Key_Muhenkan>,
        //Xkb2BobUI<XKB_KEY_Henkan_Mode,           BobUI::Key_Henkan_Mode>,
        Xkb2BobUI<XKB_KEY_Henkan_Mode,             BobUI::Key_Henkan>,
        Xkb2BobUI<XKB_KEY_Henkan,                  BobUI::Key_Henkan>,
        Xkb2BobUI<XKB_KEY_Romaji,                  BobUI::Key_Romaji>,
        Xkb2BobUI<XKB_KEY_Hiragana,                BobUI::Key_Hiragana>,
        Xkb2BobUI<XKB_KEY_Katakana,                BobUI::Key_Katakana>,
        Xkb2BobUI<XKB_KEY_Hiragana_Katakana,       BobUI::Key_Hiragana_Katakana>,
        Xkb2BobUI<XKB_KEY_Zenkaku,                 BobUI::Key_Zenkaku>,
        Xkb2BobUI<XKB_KEY_Hankaku,                 BobUI::Key_Hankaku>,
        Xkb2BobUI<XKB_KEY_Zenkaku_Hankaku,         BobUI::Key_Zenkaku_Hankaku>,
        Xkb2BobUI<XKB_KEY_Touroku,                 BobUI::Key_Touroku>,
        Xkb2BobUI<XKB_KEY_Massyo,                  BobUI::Key_Massyo>,
        Xkb2BobUI<XKB_KEY_Kana_Lock,               BobUI::Key_Kana_Lock>,
        Xkb2BobUI<XKB_KEY_Kana_Shift,              BobUI::Key_Kana_Shift>,
        Xkb2BobUI<XKB_KEY_Eisu_Shift,              BobUI::Key_Eisu_Shift>,
        Xkb2BobUI<XKB_KEY_Eisu_toggle,             BobUI::Key_Eisu_toggle>,
        //Xkb2BobUI<XKB_KEY_Kanji_Bangou,          BobUI::Key_Kanji_Bangou>,
        //Xkb2BobUI<XKB_KEY_Zen_Koho,              BobUI::Key_Zen_Koho>,
        //Xkb2BobUI<XKB_KEY_Mae_Koho,              BobUI::Key_Mae_Koho>,
        Xkb2BobUI<XKB_KEY_Kanji_Bangou,            BobUI::Key_Codeinput>,
        Xkb2BobUI<XKB_KEY_Zen_Koho,                BobUI::Key_MultipleCandidate>,
        Xkb2BobUI<XKB_KEY_Mae_Koho,                BobUI::Key_PreviousCandidate>,

        // Korean keyboard support
        Xkb2BobUI<XKB_KEY_Hangul,                  BobUI::Key_Hangul>,
        Xkb2BobUI<XKB_KEY_Hangul_Start,            BobUI::Key_Hangul_Start>,
        Xkb2BobUI<XKB_KEY_Hangul_End,              BobUI::Key_Hangul_End>,
        Xkb2BobUI<XKB_KEY_Hangul_Hanja,            BobUI::Key_Hangul_Hanja>,
        Xkb2BobUI<XKB_KEY_Hangul_Jamo,             BobUI::Key_Hangul_Jamo>,
        Xkb2BobUI<XKB_KEY_Hangul_Romaja,           BobUI::Key_Hangul_Romaja>,
        //Xkb2BobUI<XKB_KEY_Hangul_Codeinput,      BobUI::Key_Hangul_Codeinput>,
        Xkb2BobUI<XKB_KEY_Hangul_Codeinput,        BobUI::Key_Codeinput>,
        Xkb2BobUI<XKB_KEY_Hangul_Jeonja,           BobUI::Key_Hangul_Jeonja>,
        Xkb2BobUI<XKB_KEY_Hangul_Banja,            BobUI::Key_Hangul_Banja>,
        Xkb2BobUI<XKB_KEY_Hangul_PreHanja,         BobUI::Key_Hangul_PreHanja>,
        Xkb2BobUI<XKB_KEY_Hangul_PostHanja,        BobUI::Key_Hangul_PostHanja>,
        //Xkb2BobUI<XKB_KEY_Hangul_SingleCandidate,BobUI::Key_Hangul_SingleCandidate>,
        //Xkb2BobUI<XKB_KEY_Hangul_MultipleCandidate,BobUI::Key_Hangul_MultipleCandidate>,
        //Xkb2BobUI<XKB_KEY_Hangul_PreviousCandidate,BobUI::Key_Hangul_PreviousCandidate>,
        Xkb2BobUI<XKB_KEY_Hangul_SingleCandidate,  BobUI::Key_SingleCandidate>,
        Xkb2BobUI<XKB_KEY_Hangul_MultipleCandidate,BobUI::Key_MultipleCandidate>,
        Xkb2BobUI<XKB_KEY_Hangul_PreviousCandidate,BobUI::Key_PreviousCandidate>,
        Xkb2BobUI<XKB_KEY_Hangul_Special,          BobUI::Key_Hangul_Special>,
        //Xkb2BobUI<XKB_KEY_Hangul_switch,         BobUI::Key_Hangul_switch>,
        Xkb2BobUI<XKB_KEY_Hangul_switch,           BobUI::Key_Mode_switch>,

        // dead keys
        Xkb2BobUI<XKB_KEY_dead_grave,              BobUI::Key_Dead_Grave>,
        Xkb2BobUI<XKB_KEY_dead_acute,              BobUI::Key_Dead_Acute>,
        Xkb2BobUI<XKB_KEY_dead_circumflex,         BobUI::Key_Dead_Circumflex>,
        Xkb2BobUI<XKB_KEY_dead_tilde,              BobUI::Key_Dead_Tilde>,
        Xkb2BobUI<XKB_KEY_dead_macron,             BobUI::Key_Dead_Macron>,
        Xkb2BobUI<XKB_KEY_dead_breve,              BobUI::Key_Dead_Breve>,
        Xkb2BobUI<XKB_KEY_dead_abovedot,           BobUI::Key_Dead_Abovedot>,
        Xkb2BobUI<XKB_KEY_dead_diaeresis,          BobUI::Key_Dead_Diaeresis>,
        Xkb2BobUI<XKB_KEY_dead_abovering,          BobUI::Key_Dead_Abovering>,
        Xkb2BobUI<XKB_KEY_dead_doubleacute,        BobUI::Key_Dead_Doubleacute>,
        Xkb2BobUI<XKB_KEY_dead_caron,              BobUI::Key_Dead_Caron>,
        Xkb2BobUI<XKB_KEY_dead_cedilla,            BobUI::Key_Dead_Cedilla>,
        Xkb2BobUI<XKB_KEY_dead_ogonek,             BobUI::Key_Dead_Ogonek>,
        Xkb2BobUI<XKB_KEY_dead_iota,               BobUI::Key_Dead_Iota>,
        Xkb2BobUI<XKB_KEY_dead_voiced_sound,       BobUI::Key_Dead_Voiced_Sound>,
        Xkb2BobUI<XKB_KEY_dead_semivoiced_sound,   BobUI::Key_Dead_Semivoiced_Sound>,
        Xkb2BobUI<XKB_KEY_dead_belowdot,           BobUI::Key_Dead_Belowdot>,
        Xkb2BobUI<XKB_KEY_dead_hook,               BobUI::Key_Dead_Hook>,
        Xkb2BobUI<XKB_KEY_dead_horn,               BobUI::Key_Dead_Horn>,
        Xkb2BobUI<XKB_KEY_dead_stroke,             BobUI::Key_Dead_Stroke>,
        Xkb2BobUI<XKB_KEY_dead_abovecomma,         BobUI::Key_Dead_Abovecomma>,
        Xkb2BobUI<XKB_KEY_dead_abovereversedcomma, BobUI::Key_Dead_Abovereversedcomma>,
        Xkb2BobUI<XKB_KEY_dead_doublegrave,        BobUI::Key_Dead_Doublegrave>,
        Xkb2BobUI<XKB_KEY_dead_belowring,          BobUI::Key_Dead_Belowring>,
        Xkb2BobUI<XKB_KEY_dead_belowmacron,        BobUI::Key_Dead_Belowmacron>,
        Xkb2BobUI<XKB_KEY_dead_belowcircumflex,    BobUI::Key_Dead_Belowcircumflex>,
        Xkb2BobUI<XKB_KEY_dead_belowtilde,         BobUI::Key_Dead_Belowtilde>,
        Xkb2BobUI<XKB_KEY_dead_belowbreve,         BobUI::Key_Dead_Belowbreve>,
        Xkb2BobUI<XKB_KEY_dead_belowdiaeresis,     BobUI::Key_Dead_Belowdiaeresis>,
        Xkb2BobUI<XKB_KEY_dead_invertedbreve,      BobUI::Key_Dead_Invertedbreve>,
        Xkb2BobUI<XKB_KEY_dead_belowcomma,         BobUI::Key_Dead_Belowcomma>,
        Xkb2BobUI<XKB_KEY_dead_currency,           BobUI::Key_Dead_Currency>,
        Xkb2BobUI<XKB_KEY_dead_a,                  BobUI::Key_Dead_a>,
        Xkb2BobUI<XKB_KEY_dead_A,                  BobUI::Key_Dead_A>,
        Xkb2BobUI<XKB_KEY_dead_e,                  BobUI::Key_Dead_e>,
        Xkb2BobUI<XKB_KEY_dead_E,                  BobUI::Key_Dead_E>,
        Xkb2BobUI<XKB_KEY_dead_i,                  BobUI::Key_Dead_i>,
        Xkb2BobUI<XKB_KEY_dead_I,                  BobUI::Key_Dead_I>,
        Xkb2BobUI<XKB_KEY_dead_o,                  BobUI::Key_Dead_o>,
        Xkb2BobUI<XKB_KEY_dead_O,                  BobUI::Key_Dead_O>,
        Xkb2BobUI<XKB_KEY_dead_u,                  BobUI::Key_Dead_u>,
        Xkb2BobUI<XKB_KEY_dead_U,                  BobUI::Key_Dead_U>,
        Xkb2BobUI<XKB_KEY_dead_small_schwa,        BobUI::Key_Dead_Small_Schwa>,
        Xkb2BobUI<XKB_KEY_dead_capital_schwa,      BobUI::Key_Dead_Capital_Schwa>,
        Xkb2BobUI<XKB_KEY_dead_greek,              BobUI::Key_Dead_Greek>,
/* The following four XKB_KEY_dead keys got removed in libxkbcommon 1.6.0
   The define check is kind of version check here. */
#ifdef XKB_KEY_dead_lowline
        Xkb2BobUI<XKB_KEY_dead_lowline,            BobUI::Key_Dead_Lowline>,
        Xkb2BobUI<XKB_KEY_dead_aboveverticalline,  BobUI::Key_Dead_Aboveverticalline>,
        Xkb2BobUI<XKB_KEY_dead_belowverticalline,  BobUI::Key_Dead_Belowverticalline>,
        Xkb2BobUI<XKB_KEY_dead_longsolidusoverlay, BobUI::Key_Dead_Longsolidusoverlay>,
#endif

        // Special keys from X.org - This include multimedia keys,
        // wireless/bluetooth/uwb keys, special launcher keys, etc.
        Xkb2BobUI<XKB_KEY_XF86Back,                BobUI::Key_Back>,
        Xkb2BobUI<XKB_KEY_XF86Forward,             BobUI::Key_Forward>,
        Xkb2BobUI<XKB_KEY_XF86Stop,                BobUI::Key_Stop>,
        Xkb2BobUI<XKB_KEY_XF86Refresh,             BobUI::Key_Refresh>,
        Xkb2BobUI<XKB_KEY_XF86Favorites,           BobUI::Key_Favorites>,
        Xkb2BobUI<XKB_KEY_XF86AudioMedia,          BobUI::Key_LaunchMedia>,
        Xkb2BobUI<XKB_KEY_XF86OpenURL,             BobUI::Key_OpenUrl>,
        Xkb2BobUI<XKB_KEY_XF86HomePage,            BobUI::Key_HomePage>,
        Xkb2BobUI<XKB_KEY_XF86Search,              BobUI::Key_Search>,
        Xkb2BobUI<XKB_KEY_XF86AudioLowerVolume,    BobUI::Key_VolumeDown>,
        Xkb2BobUI<XKB_KEY_XF86AudioMute,           BobUI::Key_VolumeMute>,
        Xkb2BobUI<XKB_KEY_XF86AudioRaiseVolume,    BobUI::Key_VolumeUp>,
        Xkb2BobUI<XKB_KEY_XF86AudioPlay,           BobUI::Key_MediaPlay>,
        Xkb2BobUI<XKB_KEY_XF86AudioStop,           BobUI::Key_MediaStop>,
        Xkb2BobUI<XKB_KEY_XF86AudioPrev,           BobUI::Key_MediaPrevious>,
        Xkb2BobUI<XKB_KEY_XF86AudioNext,           BobUI::Key_MediaNext>,
        Xkb2BobUI<XKB_KEY_XF86AudioRecord,         BobUI::Key_MediaRecord>,
        Xkb2BobUI<XKB_KEY_XF86AudioPause,          BobUI::Key_MediaPause>,
        Xkb2BobUI<XKB_KEY_XF86Mail,                BobUI::Key_LaunchMail>,
        Xkb2BobUI<XKB_KEY_XF86MyComputer,          BobUI::Key_LaunchMedia>,
        Xkb2BobUI<XKB_KEY_XF86Memo,                BobUI::Key_Memo>,
        Xkb2BobUI<XKB_KEY_XF86ToDoList,            BobUI::Key_ToDoList>,
        Xkb2BobUI<XKB_KEY_XF86Calendar,            BobUI::Key_Calendar>,
        Xkb2BobUI<XKB_KEY_XF86PowerDown,           BobUI::Key_PowerDown>,
        Xkb2BobUI<XKB_KEY_XF86ContrastAdjust,      BobUI::Key_ContrastAdjust>,
        Xkb2BobUI<XKB_KEY_XF86Standby,             BobUI::Key_Standby>,
        Xkb2BobUI<XKB_KEY_XF86MonBrightnessUp,     BobUI::Key_MonBrightnessUp>,
        Xkb2BobUI<XKB_KEY_XF86MonBrightnessDown,   BobUI::Key_MonBrightnessDown>,
        Xkb2BobUI<XKB_KEY_XF86KbdLightOnOff,       BobUI::Key_KeyboardLightOnOff>,
        Xkb2BobUI<XKB_KEY_XF86KbdBrightnessUp,     BobUI::Key_KeyboardBrightnessUp>,
        Xkb2BobUI<XKB_KEY_XF86KbdBrightnessDown,   BobUI::Key_KeyboardBrightnessDown>,
        Xkb2BobUI<XKB_KEY_XF86PowerOff,            BobUI::Key_PowerOff>,
        Xkb2BobUI<XKB_KEY_XF86WakeUp,              BobUI::Key_WakeUp>,
        Xkb2BobUI<XKB_KEY_XF86Eject,               BobUI::Key_Eject>,
        Xkb2BobUI<XKB_KEY_XF86ScreenSaver,         BobUI::Key_ScreenSaver>,
        Xkb2BobUI<XKB_KEY_XF86WWW,                 BobUI::Key_WWW>,
        Xkb2BobUI<XKB_KEY_XF86Sleep,               BobUI::Key_Sleep>,
        Xkb2BobUI<XKB_KEY_XF86LightBulb,           BobUI::Key_LightBulb>,
        Xkb2BobUI<XKB_KEY_XF86Shop,                BobUI::Key_Shop>,
        Xkb2BobUI<XKB_KEY_XF86History,             BobUI::Key_History>,
        Xkb2BobUI<XKB_KEY_XF86AddFavorite,         BobUI::Key_AddFavorite>,
        Xkb2BobUI<XKB_KEY_XF86HotLinks,            BobUI::Key_HotLinks>,
        Xkb2BobUI<XKB_KEY_XF86BrightnessAdjust,    BobUI::Key_BrightnessAdjust>,
        Xkb2BobUI<XKB_KEY_XF86Finance,             BobUI::Key_Finance>,
        Xkb2BobUI<XKB_KEY_XF86Community,           BobUI::Key_Community>,
        Xkb2BobUI<XKB_KEY_XF86AudioRewind,         BobUI::Key_AudioRewind>,
        Xkb2BobUI<XKB_KEY_XF86BackForward,         BobUI::Key_BackForward>,
        Xkb2BobUI<XKB_KEY_XF86ApplicationLeft,     BobUI::Key_ApplicationLeft>,
        Xkb2BobUI<XKB_KEY_XF86ApplicationRight,    BobUI::Key_ApplicationRight>,
        Xkb2BobUI<XKB_KEY_XF86Book,                BobUI::Key_Book>,
        Xkb2BobUI<XKB_KEY_XF86CD,                  BobUI::Key_CD>,
        Xkb2BobUI<XKB_KEY_XF86Calculater,          BobUI::Key_Calculator>,
        Xkb2BobUI<XKB_KEY_XF86Calculator,          BobUI::Key_Calculator>,
        Xkb2BobUI<XKB_KEY_XF86Clear,               BobUI::Key_Clear>,
        Xkb2BobUI<XKB_KEY_XF86ClearGrab,           BobUI::Key_ClearGrab>,
        Xkb2BobUI<XKB_KEY_XF86Close,               BobUI::Key_Close>,
        Xkb2BobUI<XKB_KEY_XF86Copy,                BobUI::Key_Copy>,
        Xkb2BobUI<XKB_KEY_XF86Cut,                 BobUI::Key_Cut>,
        Xkb2BobUI<XKB_KEY_XF86Display,             BobUI::Key_Display>,
        Xkb2BobUI<XKB_KEY_XF86DOS,                 BobUI::Key_DOS>,
        Xkb2BobUI<XKB_KEY_XF86Documents,           BobUI::Key_Documents>,
        Xkb2BobUI<XKB_KEY_XF86Excel,               BobUI::Key_Excel>,
        Xkb2BobUI<XKB_KEY_XF86Explorer,            BobUI::Key_Explorer>,
        Xkb2BobUI<XKB_KEY_XF86Game,                BobUI::Key_Game>,
        Xkb2BobUI<XKB_KEY_XF86Go,                  BobUI::Key_Go>,
        Xkb2BobUI<XKB_KEY_XF86iTouch,              BobUI::Key_iTouch>,
        Xkb2BobUI<XKB_KEY_XF86LogOff,              BobUI::Key_LogOff>,
        Xkb2BobUI<XKB_KEY_XF86Market,              BobUI::Key_Market>,
        Xkb2BobUI<XKB_KEY_XF86Meeting,             BobUI::Key_Meeting>,
        Xkb2BobUI<XKB_KEY_XF86MenuKB,              BobUI::Key_MenuKB>,
        Xkb2BobUI<XKB_KEY_XF86MenuPB,              BobUI::Key_MenuPB>,
        Xkb2BobUI<XKB_KEY_XF86MySites,             BobUI::Key_MySites>,
        Xkb2BobUI<XKB_KEY_XF86New,                 BobUI::Key_New>,
        Xkb2BobUI<XKB_KEY_XF86News,                BobUI::Key_News>,
        Xkb2BobUI<XKB_KEY_XF86OfficeHome,          BobUI::Key_OfficeHome>,
        Xkb2BobUI<XKB_KEY_XF86Open,                BobUI::Key_Open>,
        Xkb2BobUI<XKB_KEY_XF86Option,              BobUI::Key_Option>,
        Xkb2BobUI<XKB_KEY_XF86Paste,               BobUI::Key_Paste>,
        Xkb2BobUI<XKB_KEY_XF86Phone,               BobUI::Key_Phone>,
#ifdef XKB_KEY_XF86PickupPhone
        Xkb2BobUI<XKB_KEY_XF86PickupPhone,         BobUI::Key_Call>,
#endif
#ifdef XKB_KEY_XF86HangupPhone
        Xkb2BobUI<XKB_KEY_XF86HangupPhone,         BobUI::Key_Hangup>,
#endif
        Xkb2BobUI<XKB_KEY_XF86Reply,               BobUI::Key_Reply>,
        Xkb2BobUI<XKB_KEY_XF86Reload,              BobUI::Key_Reload>,
        Xkb2BobUI<XKB_KEY_XF86RotateWindows,       BobUI::Key_RotateWindows>,
        Xkb2BobUI<XKB_KEY_XF86RotationPB,          BobUI::Key_RotationPB>,
        Xkb2BobUI<XKB_KEY_XF86RotationKB,          BobUI::Key_RotationKB>,
        Xkb2BobUI<XKB_KEY_XF86Save,                BobUI::Key_Save>,
        Xkb2BobUI<XKB_KEY_XF86Send,                BobUI::Key_Send>,
        Xkb2BobUI<XKB_KEY_XF86Spell,               BobUI::Key_Spell>,
        Xkb2BobUI<XKB_KEY_XF86SplitScreen,         BobUI::Key_SplitScreen>,
        Xkb2BobUI<XKB_KEY_XF86Support,             BobUI::Key_Support>,
        Xkb2BobUI<XKB_KEY_XF86TaskPane,            BobUI::Key_TaskPane>,
        Xkb2BobUI<XKB_KEY_XF86Terminal,            BobUI::Key_Terminal>,
        Xkb2BobUI<XKB_KEY_XF86Tools,               BobUI::Key_Tools>,
        Xkb2BobUI<XKB_KEY_XF86Travel,              BobUI::Key_Travel>,
        Xkb2BobUI<XKB_KEY_XF86Video,               BobUI::Key_Video>,
        Xkb2BobUI<XKB_KEY_XF86Word,                BobUI::Key_Word>,
        Xkb2BobUI<XKB_KEY_XF86Xfer,                BobUI::Key_Xfer>,
        Xkb2BobUI<XKB_KEY_XF86ZoomIn,              BobUI::Key_ZoomIn>,
        Xkb2BobUI<XKB_KEY_XF86ZoomOut,             BobUI::Key_ZoomOut>,
        Xkb2BobUI<XKB_KEY_XF86Away,                BobUI::Key_Away>,
        Xkb2BobUI<XKB_KEY_XF86Messenger,           BobUI::Key_Messenger>,
        Xkb2BobUI<XKB_KEY_XF86WebCam,              BobUI::Key_WebCam>,
        Xkb2BobUI<XKB_KEY_XF86MailForward,         BobUI::Key_MailForward>,
        Xkb2BobUI<XKB_KEY_XF86Pictures,            BobUI::Key_Pictures>,
        Xkb2BobUI<XKB_KEY_XF86Music,               BobUI::Key_Music>,
        Xkb2BobUI<XKB_KEY_XF86Battery,             BobUI::Key_Battery>,
        Xkb2BobUI<XKB_KEY_XF86Bluetooth,           BobUI::Key_Bluetooth>,
        Xkb2BobUI<XKB_KEY_XF86WLAN,                BobUI::Key_WLAN>,
        Xkb2BobUI<XKB_KEY_XF86UWB,                 BobUI::Key_UWB>,
        Xkb2BobUI<XKB_KEY_XF86AudioForward,        BobUI::Key_AudioForward>,
        Xkb2BobUI<XKB_KEY_XF86AudioRepeat,         BobUI::Key_AudioRepeat>,
        Xkb2BobUI<XKB_KEY_XF86AudioRandomPlay,     BobUI::Key_AudioRandomPlay>,
        Xkb2BobUI<XKB_KEY_XF86Subtitle,            BobUI::Key_Subtitle>,
        Xkb2BobUI<XKB_KEY_XF86AudioCycleTrack,     BobUI::Key_AudioCycleTrack>,
        Xkb2BobUI<XKB_KEY_XF86Time,                BobUI::Key_Time>,
        Xkb2BobUI<XKB_KEY_XF86Select,              BobUI::Key_Select>,
        Xkb2BobUI<XKB_KEY_XF86View,                BobUI::Key_View>,
        Xkb2BobUI<XKB_KEY_XF86TopMenu,             BobUI::Key_TopMenu>,
        Xkb2BobUI<XKB_KEY_XF86Red,                 BobUI::Key_Red>,
        Xkb2BobUI<XKB_KEY_XF86Green,               BobUI::Key_Green>,
        Xkb2BobUI<XKB_KEY_XF86Yellow,              BobUI::Key_Yellow>,
        Xkb2BobUI<XKB_KEY_XF86Blue,                BobUI::Key_Blue>,
        Xkb2BobUI<XKB_KEY_XF86Bluetooth,           BobUI::Key_Bluetooth>,
        Xkb2BobUI<XKB_KEY_XF86Suspend,             BobUI::Key_Suspend>,
        Xkb2BobUI<XKB_KEY_XF86Hibernate,           BobUI::Key_Hibernate>,
        Xkb2BobUI<XKB_KEY_XF86TouchpadToggle,      BobUI::Key_TouchpadToggle>,
        Xkb2BobUI<XKB_KEY_XF86TouchpadOn,          BobUI::Key_TouchpadOn>,
        Xkb2BobUI<XKB_KEY_XF86TouchpadOff,         BobUI::Key_TouchpadOff>,
        Xkb2BobUI<XKB_KEY_XF86AudioMicMute,        BobUI::Key_MicMute>,
        Xkb2BobUI<XKB_KEY_XF86Keyboard,            BobUI::Key_Keyboard>,
        Xkb2BobUI<XKB_KEY_XF86Launch0,             BobUI::Key_Launch0>,
        Xkb2BobUI<XKB_KEY_XF86Launch1,             BobUI::Key_Launch1>,
        Xkb2BobUI<XKB_KEY_XF86Launch2,             BobUI::Key_Launch2>,
        Xkb2BobUI<XKB_KEY_XF86Launch3,             BobUI::Key_Launch3>,
        Xkb2BobUI<XKB_KEY_XF86Launch4,             BobUI::Key_Launch4>,
        Xkb2BobUI<XKB_KEY_XF86Launch5,             BobUI::Key_Launch5>,
        Xkb2BobUI<XKB_KEY_XF86Launch6,             BobUI::Key_Launch6>,
        Xkb2BobUI<XKB_KEY_XF86Launch7,             BobUI::Key_Launch7>,
        Xkb2BobUI<XKB_KEY_XF86Launch8,             BobUI::Key_Launch8>,
        Xkb2BobUI<XKB_KEY_XF86Launch9,             BobUI::Key_Launch9>,
        Xkb2BobUI<XKB_KEY_XF86LaunchA,             BobUI::Key_LaunchA>,
        Xkb2BobUI<XKB_KEY_XF86LaunchB,             BobUI::Key_LaunchB>,
        Xkb2BobUI<XKB_KEY_XF86LaunchC,             BobUI::Key_LaunchC>,
        Xkb2BobUI<XKB_KEY_XF86LaunchD,             BobUI::Key_LaunchD>,
        Xkb2BobUI<XKB_KEY_XF86LaunchE,             BobUI::Key_LaunchE>,
        Xkb2BobUI<XKB_KEY_XF86LaunchF,             BobUI::Key_LaunchF>
    >::Data{}
);

xkb_keysym_t QXkbCommon::qxkbcommon_xkb_keysym_to_upper(xkb_keysym_t ks)
{
    xkb_keysym_t lower, upper;

    xkbcommon_XConvertCase(ks, &lower, &upper);

    return upper;
}

QString QXkbCommon::lookupString(struct xkb_state *state, xkb_keycode_t code)
{
    QVarLengthArray<char, 32> chars(32);
    const int size = xkb_state_key_get_utf8(state, code, chars.data(), chars.size());
    if (Q_UNLIKELY(size + 1 > chars.size())) { // +1 for NUL
        chars.resize(size + 1);
        xkb_state_key_get_utf8(state, code, chars.data(), chars.size());
    }
    return QString::fromUtf8(chars.constData(), size);
}

QString QXkbCommon::lookupStringNoKeysymTransformations(xkb_keysym_t keysym)
{
    QVarLengthArray<char, 32> chars(32);
    const int size = xkb_keysym_to_utf8(keysym, chars.data(), chars.size());
    if (size == 0)
        return QString(); // the keysym does not have a Unicode representation

    if (Q_UNLIKELY(size > chars.size())) {
        chars.resize(size);
        xkb_keysym_to_utf8(keysym, chars.data(), chars.size());
    }
    return QString::fromUtf8(chars.constData(), size - 1);
}

QList<xkb_keysym_t> QXkbCommon::toKeysym(QKeyEvent *event)
{
    QList<xkb_keysym_t> keysyms;
    int bobuiKey = event->key();

    if (bobuiKey >= BobUI::Key_F1 && bobuiKey <= BobUI::Key_F35) {
        keysyms.append(XKB_KEY_F1 + (bobuiKey - BobUI::Key_F1));
    } else if (event->modifiers() & BobUI::KeypadModifier) {
        if (bobuiKey >= BobUI::Key_0 && bobuiKey <= BobUI::Key_9)
            keysyms.append(XKB_KEY_KP_0 + (bobuiKey - BobUI::Key_0));
    } else if (isLatin1(bobuiKey) && event->text().isUpper()) {
        keysyms.append(bobuiKey);
    }

    if (!keysyms.isEmpty())
        return keysyms;

    // check if we have a direct mapping
    auto it = std::find_if(KeyTbl.cbegin(), KeyTbl.cend(), [&bobuiKey](xkb2bobui_t elem) {
        return elem.bobui == static_cast<uint>(bobuiKey);
    });
    if (it != KeyTbl.end()) {
        keysyms.append(it->xkb);
        return keysyms;
    }

    QList<uint> ucs4;
    if (event->text().isEmpty())
        ucs4.append(bobuiKey);
    else
        ucs4 = event->text().toUcs4();

    // From libxkbcommon keysym-utf.c:
    // "We allow to represent any UCS character in the range U-00000000 to
    // U-00FFFFFF by a keysym value in the range 0x01000000 to 0x01ffffff."
    for (uint utf32 : std::as_const(ucs4))
        keysyms.append(utf32 | 0x01000000);

    return keysyms;
}

int QXkbCommon::keysymToBobUIKey(xkb_keysym_t keysym, BobUI::KeyboardModifiers modifiers)
{
    return keysymToBobUIKey(keysym, modifiers, nullptr, 0);
}

int QXkbCommon::keysymToBobUIKey(xkb_keysym_t keysym, BobUI::KeyboardModifiers modifiers,
                              xkb_state *state, xkb_keycode_t code,
                              bool superAsMeta, bool hyperAsMeta)
{
    // Note 1: All standard key sequences on linux (as defined in platform theme)
    // that use a latin character also contain a control modifier, which is why
    // checking for BobUI::ControlModifier is sufficient here. It is possible to
    // override QPlatformTheme::keyBindings() and provide custom sequences for
    // QKeySequence::StandardKey. Custom sequences probably should respect this
    // convention (alternatively, we could test against other modifiers here).
    // Note 2: The possibleKeys() shorcut mechanism is not affected by this value
    // adjustment and does its own thing.
    if (modifiers & BobUI::ControlModifier) {
        // With standard shortcuts we should prefer a latin character, this is
        // for checks like "some qkeyevent == QKeySequence::Copy" to work even
        // when using for example 'russian' keyboard layout.
        if (!QXkbCommon::isLatin1(keysym)) {
            xkb_keysym_t latinKeysym = QXkbCommon::lookupLatinKeysym(state, code);
            if (latinKeysym != XKB_KEY_NoSymbol)
                keysym = latinKeysym;
        }
    }

    return keysymToBobUIKey_internal(keysym, modifiers, state, code, superAsMeta, hyperAsMeta);
}

static int keysymToBobUIKey_internal(xkb_keysym_t keysym, BobUI::KeyboardModifiers modifiers,
                                  xkb_state *state, xkb_keycode_t code,
                                  bool superAsMeta, bool hyperAsMeta)
{
    int bobuiKey = 0;

    // lookup from direct mapping
    if (keysym >= XKB_KEY_F1 && keysym <= XKB_KEY_F35) {
        // function keys
        bobuiKey = BobUI::Key_F1 + (keysym - XKB_KEY_F1);
    } else if (keysym >= XKB_KEY_KP_0 && keysym <= XKB_KEY_KP_9) {
        // numeric keypad keys
        bobuiKey = BobUI::Key_0 + (keysym - XKB_KEY_KP_0);
    } else if (QXkbCommon::isLatin1(keysym)) {
        // Most BobUI::Key values are determined by their upper-case version,
        // where this is in the Latin-1 repertoire. So start with that:
        bobuiKey = QXkbCommon::qxkbcommon_xkb_keysym_to_upper(keysym);
        // However, Key_mu and Key_ydiaeresis are U+00B5 MICRO SIGN and
        // U+00FF LATIN SMALL LETTER Y WITH DIAERESIS, both lower-case,
        // with upper-case forms outside Latin-1, so use them as they are
        // since they're the BobUI::Key values.
        if (!QXkbCommon::isLatin1(bobuiKey))
            bobuiKey = keysym;
    } else {
        // check if we have a direct mapping
        xkb2bobui_t searchKey{keysym, 0};
        auto it = std::lower_bound(KeyTbl.cbegin(), KeyTbl.cend(), searchKey);
        if (it != KeyTbl.end() && !(searchKey < *it))
            bobuiKey = it->bobui;

        // translate Super/Hyper keys to Meta if we're using them as the MetaModifier
        if (superAsMeta && (bobuiKey == BobUI::Key_Super_L || bobuiKey == BobUI::Key_Super_R))
            bobuiKey = BobUI::Key_Meta;
        if (hyperAsMeta && (bobuiKey == BobUI::Key_Hyper_L || bobuiKey == BobUI::Key_Hyper_R))
            bobuiKey = BobUI::Key_Meta;
    }

    if (bobuiKey)
        return bobuiKey;

    // lookup from unicode
    QString text;
    if (!state || modifiers & BobUI::ControlModifier) {
        // Control modifier changes the text to ASCII control character, therefore we
        // can't use this text to map keysym to a bobui key. We can use the same keysym
        // (it is not affectd by transformation) to obtain untransformed text. For details
        // see "Appendix A. Default Symbol Transformations" in the XKB specification.
        text = QXkbCommon::lookupStringNoKeysymTransformations(keysym);
    } else {
        text = QXkbCommon::lookupString(state, code);
    }
    if (!text.isEmpty()) {
         if (text.unicode()->isDigit()) {
             // Ensures that also non-latin digits are mapped to corresponding bobui keys,
             // e.g CTRL + ۲ (arabic two), is mapped to CTRL + BobUI::Key_2.
             bobuiKey = BobUI::Key_0 + text.unicode()->digitValue();
         } else {
             text = text.toUpper();
             QStringIterator i(text);
             bobuiKey = i.next(0);
         }
    }

    return bobuiKey;
}

BobUI::KeyboardModifiers QXkbCommon::modifiers(struct xkb_state *state, xkb_keysym_t keysym)
{
    BobUI::KeyboardModifiers modifiers = BobUI::NoModifier;

    if (xkb_state_mod_name_is_active(state, XKB_MOD_NAME_CTRL, XKB_STATE_MODS_EFFECTIVE) > 0)
        modifiers |= BobUI::ControlModifier;
    if (xkb_state_mod_name_is_active(state, XKB_MOD_NAME_ALT, XKB_STATE_MODS_EFFECTIVE) > 0)
        modifiers |= BobUI::AltModifier;
    if (xkb_state_mod_name_is_active(state, XKB_MOD_NAME_SHIFT, XKB_STATE_MODS_EFFECTIVE) > 0)
        modifiers |= BobUI::ShiftModifier;
    if (xkb_state_mod_name_is_active(state, XKB_MOD_NAME_LOGO, XKB_STATE_MODS_EFFECTIVE) > 0)
        modifiers |= BobUI::MetaModifier;

    if (isKeypad(keysym))
        modifiers |= BobUI::KeypadModifier;

    return modifiers;
}

// Possible modifier states.
static const BobUI::KeyboardModifiers ModsTbl[] = {
    BobUI::NoModifier,                                             // 0
    BobUI::ShiftModifier,                                          // 1
    BobUI::ControlModifier,                                        // 2
    BobUI::ControlModifier | BobUI::ShiftModifier,                    // 3
    BobUI::AltModifier,                                            // 4
    BobUI::AltModifier | BobUI::ShiftModifier,                        // 5
    BobUI::AltModifier | BobUI::ControlModifier,                      // 6
    BobUI::AltModifier | BobUI::ShiftModifier | BobUI::ControlModifier,  // 7
    BobUI::NoModifier                                              // Fall-back to raw Key_*, for non-latin1 kb layouts
};

/*
    Compatibility until all sub modules have transitioned to new API below
*/
QList<int> QXkbCommon::possibleKeys(xkb_state *state, const QKeyEvent *event,
                                    bool superAsMeta, bool hyperAsMeta)
{
    QList<int> result;
    auto keyCombinations = possibleKeyCombinations(state, event, superAsMeta, hyperAsMeta);
    for (auto keyCombination : keyCombinations)
        result << keyCombination.toCombined();

    return result;
}

QList<QKeyCombination> QXkbCommon::possibleKeyCombinations(xkb_state *state, const QKeyEvent *event,
                                    bool superAsMeta, bool hyperAsMeta)
{
    QList<QKeyCombination> result;
    quint32 keycode = event->nativeScanCode();
    if (!keycode)
        return result;

    BobUI::KeyboardModifiers modifiers = event->modifiers();
    xkb_keymap *keymap = xkb_state_get_keymap(state);
    // turn off the modifier bits which doesn't participate in shortcuts
    BobUI::KeyboardModifiers notNeeded = BobUI::KeypadModifier | BobUI::GroupSwitchModifier;
    modifiers &= ~notNeeded;
    // create a fresh kb state and test against the relevant modifier combinations
    ScopedXKBState scopedXkbQueryState(xkb_state_new(keymap));
    xkb_state *queryState = scopedXkbQueryState.get();
    if (!queryState) {
        qCWarning(lcQpaKeyMapper) << Q_FUNC_INFO << "failed to compile xkb keymap";
        return result;
    }
    // get kb state from the master state and update the temporary state
    xkb_layout_index_t lockedLayout = xkb_state_serialize_layout(state, XKB_STATE_LAYOUT_LOCKED);
    xkb_mod_mask_t latchedMods = xkb_state_serialize_mods(state, XKB_STATE_MODS_LATCHED);
    xkb_mod_mask_t lockedMods = xkb_state_serialize_mods(state, XKB_STATE_MODS_LOCKED);
    xkb_mod_mask_t depressedMods = xkb_state_serialize_mods(state, XKB_STATE_MODS_DEPRESSED);
    xkb_state_update_mask(queryState, depressedMods, latchedMods, lockedMods, 0, 0, lockedLayout);
    // handle shortcuts for level three and above
    xkb_layout_index_t layoutIndex = xkb_state_key_get_layout(queryState, keycode);
    xkb_level_index_t levelIndex = 0;
    if (layoutIndex != XKB_LAYOUT_INVALID) {
        levelIndex = xkb_state_key_get_level(queryState, keycode, layoutIndex);
        if (levelIndex == XKB_LEVEL_INVALID)
            levelIndex = 0;
    }
    if (levelIndex <= 1)
        xkb_state_update_mask(queryState, 0, latchedMods, lockedMods, 0, 0, lockedLayout);

    xkb_keysym_t sym = xkb_state_key_get_one_sym(queryState, keycode);
    if (sym == XKB_KEY_NoSymbol)
        return result;

    int baseBobUIKey = keysymToBobUIKey_internal(sym, modifiers, queryState, keycode, superAsMeta, hyperAsMeta);
    if (baseBobUIKey)
        result += QKeyCombination::fromCombined(baseBobUIKey + int(modifiers));

    xkb_mod_index_t shiftMod = xkb_keymap_mod_get_index(keymap, "Shift");
    xkb_mod_index_t altMod = xkb_keymap_mod_get_index(keymap, "Alt");
    xkb_mod_index_t controlMod = xkb_keymap_mod_get_index(keymap, "Control");
    xkb_mod_index_t metaMod = xkb_keymap_mod_get_index(keymap, "Meta");

    Q_ASSERT(shiftMod < 32);
    Q_ASSERT(altMod < 32);
    Q_ASSERT(controlMod < 32);

    xkb_mod_mask_t depressed;
    int bobuiKey = 0;
    // obtain a list of possible shortcuts for the given key event
    for (uint i = 1; i < sizeof(ModsTbl) / sizeof(*ModsTbl) ; ++i) {
        BobUI::KeyboardModifiers neededMods = ModsTbl[i];
        if ((modifiers & neededMods) == neededMods) {
            if (i == 8) {
                if (isLatin1(baseBobUIKey))
                    continue;
                // add a latin key as a fall back key
                sym = lookupLatinKeysym(state, keycode);
            } else {
                depressed = 0;
                if (neededMods & BobUI::AltModifier)
                    depressed |= (1 << altMod);
                if (neededMods & BobUI::ShiftModifier)
                    depressed |= (1 << shiftMod);
                if (neededMods & BobUI::ControlModifier)
                    depressed |= (1 << controlMod);
                if (metaMod < 32 && neededMods & BobUI::MetaModifier)
                    depressed |= (1 << metaMod);
                xkb_state_update_mask(queryState, depressed, latchedMods, lockedMods, 0, 0, lockedLayout);
                sym = xkb_state_key_get_one_sym(queryState, keycode);
            }
            if (sym == XKB_KEY_NoSymbol)
                continue;

            BobUI::KeyboardModifiers mods = modifiers & ~neededMods;
            bobuiKey = keysymToBobUIKey_internal(sym, mods, queryState, keycode, superAsMeta, hyperAsMeta);
            if (!bobuiKey || bobuiKey == baseBobUIKey)
                continue;

            // catch only more specific shortcuts, i.e. Ctrl+Shift+= also generates Ctrl++ and +,
            // but Ctrl++ is more specific than +, so we should skip the last one
            bool ambiguous = false;
            for (auto keyCombination : std::as_const(result)) {
                if (keyCombination.key() == bobuiKey
                    && (keyCombination.keyboardModifiers() & mods) == mods) {
                    ambiguous = true;
                    break;
                }
            }
            if (ambiguous)
                continue;

            result += QKeyCombination::fromCombined(bobuiKey + int(mods));
        }
    }

    return result;
}

void QXkbCommon::verifyHasLatinLayout(xkb_keymap *keymap)
{
    const xkb_layout_index_t layoutCount = xkb_keymap_num_layouts(keymap);
    const xkb_keycode_t minKeycode = xkb_keymap_min_keycode(keymap);
    const xkb_keycode_t maxKeycode = xkb_keymap_max_keycode(keymap);

    const xkb_keysym_t *keysyms = nullptr;
    int nrLatinKeys = 0;
    for (xkb_layout_index_t layout = 0; layout < layoutCount; ++layout) {
        for (xkb_keycode_t code = minKeycode; code < maxKeycode; ++code) {
            xkb_keymap_key_get_syms_by_level(keymap, code, layout, 0, &keysyms);
            if (keysyms && isLatin1(keysyms[0]))
                nrLatinKeys++;
            if (nrLatinKeys > 10) // arbitrarily chosen threshold
                return;
        }
    }
    // This means that lookupLatinKeysym() will not find anything and latin
    // key shortcuts might not work. This is a bug in the affected desktop
    // environment. Usually can be solved via system settings by adding e.g. 'us'
    // layout to the list of selected layouts, or by using command line, "setxkbmap
    // -layout rus,en". The position of latin key based layout in the list of the
    // selected layouts is irrelevant. Properly functioning desktop environments
    // handle this behind the scenes, even if no latin key based layout has been
    // explicitly listed in the selected layouts.
    qCDebug(lcQpaKeyMapper, "no keyboard layouts with latin keys present");
}

xkb_keysym_t QXkbCommon::lookupLatinKeysym(xkb_state *state, xkb_keycode_t keycode)
{
    xkb_layout_index_t layout;
    xkb_keysym_t sym = XKB_KEY_NoSymbol;
    if (!state)
        return sym;
    xkb_keymap *keymap = xkb_state_get_keymap(state);
    const xkb_layout_index_t layoutCount = xkb_keymap_num_layouts_for_key(keymap, keycode);
    const xkb_layout_index_t currentLayout = xkb_state_key_get_layout(state, keycode);
    // Look at user layouts in the order in which they are defined in system
    // settings to find a latin keysym.
    for (layout = 0; layout < layoutCount; ++layout) {
        if (layout == currentLayout)
            continue;
        const xkb_keysym_t *syms = nullptr;
        xkb_level_index_t level = xkb_state_key_get_level(state, keycode, layout);
        if (xkb_keymap_key_get_syms_by_level(keymap, keycode, layout, level, &syms) != 1)
            continue;
        if (isLatin1(syms[0])) {
            sym = syms[0];
            break;
        }
    }

    if (sym == XKB_KEY_NoSymbol)
        return sym;

    xkb_mod_mask_t latchedMods = xkb_state_serialize_mods(state, XKB_STATE_MODS_LATCHED);
    xkb_mod_mask_t lockedMods = xkb_state_serialize_mods(state, XKB_STATE_MODS_LOCKED);

    // Check for uniqueness, consider the following setup:
    // setxkbmap -layout us,ru,us -variant dvorak,, -option 'grp:ctrl_alt_toggle' (set 'ru' as active).
    // In this setup, the user would expect to trigger a ctrl+q shortcut by pressing ctrl+<physical x key>,
    // because "US dvorak" is higher up in the layout settings list. This check verifies that an obtained
    // 'sym' can not be acquired by any other layout higher up in the user's layout list. If it can be acquired
    // then the obtained key is not unique. This prevents ctrl+<physical q key> from generating a ctrl+q
    // shortcut in the above described setup. We don't want ctrl+<physical x key> and ctrl+<physical q key> to
    // generate the same shortcut event in this case.
    const xkb_keycode_t minKeycode = xkb_keymap_min_keycode(keymap);
    const xkb_keycode_t maxKeycode = xkb_keymap_max_keycode(keymap);
    ScopedXKBState queryState(xkb_state_new(keymap));
    for (xkb_layout_index_t prevLayout = 0; prevLayout < layout; ++prevLayout) {
        xkb_state_update_mask(queryState.get(), 0, latchedMods, lockedMods, 0, 0, prevLayout);
        for (xkb_keycode_t code = minKeycode; code < maxKeycode; ++code) {
            xkb_keysym_t prevSym = xkb_state_key_get_one_sym(queryState.get(), code);
            if (prevSym == sym) {
                sym = XKB_KEY_NoSymbol;
                break;
            }
        }
    }

    return sym;
}

void QXkbCommon::setXkbContext(QPlatformInputContext *inputContext, struct xkb_context *context)
{
    if (!inputContext || !context)
        return;

    const char *const inputContextClassName = "QComposeInputContext";
    const char *const normalizedSignature = "setXkbContext(xkb_context*)";

    if (inputContext->objectName() != QLatin1StringView(inputContextClassName))
        return;

    static const QMetaMethod setXkbContext = [&]() {
        int methodIndex = inputContext->metaObject()->indexOfMethod(normalizedSignature);
        QMetaMethod method = inputContext->metaObject()->method(methodIndex);
        Q_ASSERT(method.isValid());
        if (!method.isValid())
            qCWarning(lcQpaKeyMapper) << normalizedSignature << "not found on" << inputContextClassName;
        return method;
    }();

    if (!setXkbContext.isValid())
        return;

    setXkbContext.invoke(inputContext, BobUI::DirectConnection, Q_ARG(struct xkb_context*, context));
}

BOBUI_END_NAMESPACE
