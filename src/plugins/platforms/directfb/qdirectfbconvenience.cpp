// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#include "qdirectfbconvenience.h"
#include "qdirectfbblitter.h"
#include "qdirectfbscreen.h"

#include <private/qpixmap_blitter_p.h>

#include <BobUIGui/QWindow>
#include <BobUIGui/QScreen>

BOBUI_BEGIN_NAMESPACE

IDirectFB *QDirectFbConvenience::dfbInterface()
{
    static IDirectFB *dfb = nullptr;
    if (!dfb) {
        DFBResult result = DirectFBCreate(&dfb);
        if (result != DFB_OK) {
            DirectFBErrorFatal("QDirectFBConvenience: error creating DirectFB interface", result);
            return 0;
        }
    }
    return dfb;
}

IDirectFBDisplayLayer *QDirectFbConvenience::dfbDisplayLayer(int display)
{
    IDirectFBDisplayLayer *layer;
    DFBResult result = QDirectFbConvenience::dfbInterface()->GetDisplayLayer(QDirectFbConvenience::dfbInterface(),display,&layer);
    if (result != DFB_OK)
        DirectFBErrorFatal("QDirectFbConvenience: "
                           "Unable to get primary display layer!", result);
    return layer;
}

QImage::Format QDirectFbConvenience::imageFormatFromSurfaceFormat(const DFBSurfacePixelFormat format, const DFBSurfaceCapabilities caps)
{
    switch (format) {
    case DSPF_LUT8:
        return QImage::Format_Indexed8;
    case DSPF_RGB24:
        return QImage::Format_RGB888;
    case DSPF_ARGB4444:
        return QImage::Format_ARGB4444_Premultiplied;
    case DSPF_RGB444:
        return QImage::Format_RGB444;
    case DSPF_RGB555:
    case DSPF_ARGB1555:
        return QImage::Format_RGB555;
    case DSPF_RGB16:
        return QImage::Format_RGB16;
    case DSPF_ARGB6666:
        return QImage::Format_ARGB6666_Premultiplied;
    case DSPF_RGB18:
        return QImage::Format_RGB666;
    case DSPF_RGB32:
        return QImage::Format_RGB32;
    case DSPF_ARGB: {
            if (caps & DSCAPS_PREMULTIPLIED)
                    return QImage::Format_ARGB32_Premultiplied;
            else return QImage::Format_ARGB32; }
    default:
        break;
    }
    return QImage::Format_Invalid;

}

int QDirectFbConvenience::colorDepthForSurface(const DFBSurfacePixelFormat format)
{
    return ((0x1f << 7) & format) >> 7;
}

/**
 * This is borrowing the reference of the QDirectFbBlitter. You may not store this
 * pointer as a class member but must only use it locally.
 */
IDirectFBSurface *QDirectFbConvenience::dfbSurfaceForPlatformPixmap(QPlatformPixmap *handle)
{
    QBlittablePlatformPixmap *blittablePmData = static_cast<QBlittablePlatformPixmap *>(handle);
    if (blittablePmData) {
        QBlittable *blittable = blittablePmData->blittable();
        QDirectFbBlitter *dfbBlitter = static_cast<QDirectFbBlitter *>(blittable);
        return dfbBlitter->m_surface.data();
    }
    return 0;
}

BobUI::MouseButton QDirectFbConvenience::mouseButton(DFBInputDeviceButtonIdentifier identifier)
{
    // The Enum contains values for DIBI_FIRST (= DIBI_LEFT), DIBI_LAST (= 0x1f,) and
    // just 3 enumerated Mouse Buttons. To convert into *ALL* possible BobUI::MoueButton values,
    // the parameter is cast as integer.

    switch (int(identifier)) {
    case DIBI_LEFT:                 // value is 0x00
        return BobUI::LeftButton;
    case DIBI_RIGHT:                // value is 0x01
        return BobUI::RightButton;
    case DIBI_MIDDLE:               // value is 0x02
        return BobUI::MiddleButton;
    case 0x03:
        return BobUI::BackButton;
    case 0x04:
        return BobUI::ForwardButton;
    case 0x05:
        return BobUI::ExtraButton3;
    case 0x06:
        return BobUI::ExtraButton4;
    case 0x07:
        return BobUI::ExtraButton5;
    case 0x08:
        return BobUI::ExtraButton6;
    case 0x09:
        return BobUI::ExtraButton7;
    case 0x0a:
        return BobUI::ExtraButton8;
    case 0x0b:
        return BobUI::ExtraButton9;
    case 0x0c:
        return BobUI::ExtraButton10;
    case 0x0d:
        return BobUI::ExtraButton11;
    case 0x0e:
        return BobUI::ExtraButton12;
    case 0x0f:
        return BobUI::ExtraButton13;
    case 0x10:
        return BobUI::ExtraButton14;
    case 0x11:
        return BobUI::ExtraButton15;
    case 0x12:
        return BobUI::ExtraButton16;
    case 0x13:
        return BobUI::ExtraButton17;
    case 0x14:
        return BobUI::ExtraButton18;
    case 0x15:
        return BobUI::ExtraButton19;
    case 0x16:
        return BobUI::ExtraButton20;
    case 0x17:
        return BobUI::ExtraButton21;
    case 0x18:
        return BobUI::ExtraButton22;
    case 0x19:
        return BobUI::ExtraButton23;
    case 0x1a:
        return BobUI::ExtraButton24;
    default:
        return BobUI::NoButton;
    }
}

BobUI::MouseButtons QDirectFbConvenience::mouseButtons(DFBInputDeviceButtonMask mask)
{
    BobUI::MouseButtons buttons = BobUI::NoButton;

    if (mask & DIBM_LEFT) {
        buttons |= BobUI::LeftButton;
    }
    if (mask & DIBM_MIDDLE) {
        buttons |= BobUI::MiddleButton;
    }
    if (mask & DIBM_RIGHT) {
        buttons |= BobUI::RightButton;
    }
    return buttons;
}

BobUI::KeyboardModifiers QDirectFbConvenience::keyboardModifiers(DFBInputDeviceModifierMask mask)
{
    BobUI::KeyboardModifiers modifiers = BobUI::NoModifier;

    if (mask & DIMM_SHIFT) {
        modifiers |= BobUI::ShiftModifier;
    }
    if (mask & DIMM_ALT) {
        modifiers |= BobUI::AltModifier;
    }
    if (mask & DIMM_ALTGR) {
        modifiers |= BobUI::MetaModifier;
    }
    if (mask & DIMM_CONTROL) {
        modifiers |= BobUI::ControlModifier;
    }
    if (mask & DIMM_META) {
        modifiers |= BobUI::MetaModifier;
    }
    return modifiers;
}

QEvent::Type QDirectFbConvenience::eventType(DFBWindowEventType type)
{
    switch(type) {
    case DWET_BUTTONDOWN:
        return QEvent::MouseButtonPress;
    case DWET_BUTTONUP:
        return QEvent::MouseButtonRelease;
    case DWET_MOTION:
        return QEvent::MouseMove;
    case DWET_WHEEL:
        return QEvent::Wheel;
    case DWET_KEYDOWN:
        return QEvent::KeyPress;
    case DWET_KEYUP:
        return QEvent::KeyRelease;
    default:
        return QEvent::None;
    }
}
QDirectFbKeyMap *QDirectFbConvenience::dfbKeymap = nullptr;
QDirectFbKeyMap *QDirectFbConvenience::keyMap()
{
    if (!dfbKeymap)
        dfbKeymap = new QDirectFbKeyMap();
    return dfbKeymap;
}

QDirectFbKeyMap::QDirectFbKeyMap()
{
    insert(DIKS_BACKSPACE             , BobUI::Key_Backspace);
    insert(DIKS_TAB                   , BobUI::Key_Tab);
    insert(DIKS_RETURN                , BobUI::Key_Return);
    insert(DIKS_ESCAPE                , BobUI::Key_Escape);
    insert(DIKS_DELETE                , BobUI::Key_Delete);

    insert(DIKS_CURSOR_LEFT           , BobUI::Key_Left);
    insert(DIKS_CURSOR_RIGHT          , BobUI::Key_Right);
    insert(DIKS_CURSOR_UP             , BobUI::Key_Up);
    insert(DIKS_CURSOR_DOWN           , BobUI::Key_Down);
    insert(DIKS_INSERT                , BobUI::Key_Insert);
    insert(DIKS_HOME                  , BobUI::Key_Home);
    insert(DIKS_END                   , BobUI::Key_End);
    insert(DIKS_PAGE_UP               , BobUI::Key_PageUp);
    insert(DIKS_PAGE_DOWN             , BobUI::Key_PageDown);
    insert(DIKS_PRINT                 , BobUI::Key_Print);
    insert(DIKS_PAUSE                 , BobUI::Key_Pause);
    insert(DIKS_SELECT                , BobUI::Key_Select);
    insert(DIKS_GOTO                  , BobUI::Key_OpenUrl);
    insert(DIKS_CLEAR                 , BobUI::Key_Clear);
    insert(DIKS_MENU                  , BobUI::Key_Menu);
    insert(DIKS_HELP                  , BobUI::Key_Help);
    insert(DIKS_INFO                  , BobUI::Key_Info);
    insert(DIKS_EXIT                  , BobUI::Key_Exit);
    insert(DIKS_SETUP                 , BobUI::Key_Settings);

    insert(DIKS_CD                    , BobUI::Key_CD);
    insert(DIKS_INTERNET              , BobUI::Key_HomePage);
    insert(DIKS_MAIL                  , BobUI::Key_LaunchMail);
    insert(DIKS_FAVORITES             , BobUI::Key_Favorites);
    insert(DIKS_KEYBOARD              , BobUI::Key_Keyboard);
    insert(DIKS_PHONE                 , BobUI::Key_Phone);
    insert(DIKS_CALL                  , BobUI::Key_Call)
    insert(DIKS_PROGRAM               , BobUI::Key_Guide);
    insert(DIKS_TIME                  , BobUI::Key_Time);

    insert(DIKS_RED                   , BobUI::Key_Red);
    insert(DIKS_GREEN                 , BobUI::Key_Green);
    insert(DIKS_YELLOW                , BobUI::Key_Yellow);
    insert(DIKS_BLUE                  , BobUI::Key_Blue);

    insert(DIKS_CHANNEL_UP            , BobUI::Key_ChannelUp);
    insert(DIKS_CHANNEL_DOWN          , BobUI::Key_ChannelDown);

    insert(DIKS_BACK                  , BobUI::Key_Back);
    insert(DIKS_FORWARD               , BobUI::Key_Forward);
    insert(DIKS_VOLUME_UP             , BobUI::Key_VolumeUp);
    insert(DIKS_VOLUME_DOWN           , BobUI::Key_VolumeDown);
    insert(DIKS_MUTE                  , BobUI::Key_VolumeMute);
    insert(DIKS_PLAYPAUSE             , BobUI::Key_MediaTogglePlayPause);
    insert(DIKS_PLAY                  , BobUI::Key_MediaPlay);
    insert(DIKS_STOP                  , BobUI::Key_MediaStop);
    insert(DIKS_RECORD                , BobUI::Key_MediaRecord);
    insert(DIKS_PREVIOUS              , BobUI::Key_MediaPrevious);
    insert(DIKS_NEXT                  , BobUI::Key_MediaNext);
    insert(DIKS_REWIND                , BobUI::Key_AudioRewind);
    insert(DIKS_FASTFORWARD           , BobUI::Key_AudioForward);
    insert(DIKS_SUBTITLE              , BobUI::Key_Subtitle);

    insert(DIKS_F1                    , BobUI::Key_F1);
    insert(DIKS_F2                    , BobUI::Key_F2);
    insert(DIKS_F3                    , BobUI::Key_F3);
    insert(DIKS_F4                    , BobUI::Key_F4);
    insert(DIKS_F5                    , BobUI::Key_F5);
    insert(DIKS_F6                    , BobUI::Key_F6);
    insert(DIKS_F7                    , BobUI::Key_F7);
    insert(DIKS_F8                    , BobUI::Key_F8);
    insert(DIKS_F9                    , BobUI::Key_F9);
    insert(DIKS_F10                   , BobUI::Key_F10);
    insert(DIKS_F11                   , BobUI::Key_F11);
    insert(DIKS_F12                   , BobUI::Key_F12);

    insert(DIKS_SHIFT                 , BobUI::Key_Shift);
    insert(DIKS_CONTROL               , BobUI::Key_Control);
    insert(DIKS_ALT                   , BobUI::Key_Alt);
    insert(DIKS_ALTGR                 , BobUI::Key_AltGr);

    insert(DIKS_META                  , BobUI::Key_Meta);
    insert(DIKS_SUPER                 , BobUI::Key_Super_L); // ???
    insert(DIKS_HYPER                 , BobUI::Key_Hyper_L); // ???

    insert(DIKS_CAPS_LOCK             , BobUI::Key_CapsLock);
    insert(DIKS_NUM_LOCK              , BobUI::Key_NumLock);
    insert(DIKS_SCROLL_LOCK           , BobUI::Key_ScrollLock);

    insert(DIKS_DEAD_ABOVEDOT         , BobUI::Key_Dead_Abovedot);
    insert(DIKS_DEAD_ABOVERING        , BobUI::Key_Dead_Abovering);
    insert(DIKS_DEAD_ACUTE            , BobUI::Key_Dead_Acute);
    insert(DIKS_DEAD_BREVE            , BobUI::Key_Dead_Breve);
    insert(DIKS_DEAD_CARON            , BobUI::Key_Dead_Caron);
    insert(DIKS_DEAD_CEDILLA          , BobUI::Key_Dead_Cedilla);
    insert(DIKS_DEAD_CIRCUMFLEX       , BobUI::Key_Dead_Circumflex);
    insert(DIKS_DEAD_DIAERESIS        , BobUI::Key_Dead_Diaeresis);
    insert(DIKS_DEAD_DOUBLEACUTE      , BobUI::Key_Dead_Doubleacute);
    insert(DIKS_DEAD_GRAVE            , BobUI::Key_Dead_Grave);
    insert(DIKS_DEAD_IOTA             , BobUI::Key_Dead_Iota);
    insert(DIKS_DEAD_MACRON           , BobUI::Key_Dead_Macron);
    insert(DIKS_DEAD_OGONEK           , BobUI::Key_Dead_Ogonek);
    insert(DIKS_DEAD_SEMIVOICED_SOUND , BobUI::Key_Dead_Semivoiced_Sound);
    insert(DIKS_DEAD_TILDE            , BobUI::Key_Dead_Tilde);
    insert(DIKS_DEAD_VOICED_SOUND     , BobUI::Key_Dead_Voiced_Sound);
    insert(DIKS_SPACE                 , BobUI::Key_Space);
    insert(DIKS_EXCLAMATION_MARK      , BobUI::Key_Exclam);
    insert(DIKS_QUOTATION             , BobUI::Key_QuoteDbl);
    insert(DIKS_NUMBER_SIGN           , BobUI::Key_NumberSign);
    insert(DIKS_DOLLAR_SIGN           , BobUI::Key_Dollar);
    insert(DIKS_PERCENT_SIGN          , BobUI::Key_Percent);
    insert(DIKS_AMPERSAND             , BobUI::Key_Ampersand);
    insert(DIKS_APOSTROPHE            , BobUI::Key_Apostrophe);
    insert(DIKS_PARENTHESIS_LEFT      , BobUI::Key_ParenLeft);
    insert(DIKS_PARENTHESIS_RIGHT     , BobUI::Key_ParenRight);
    insert(DIKS_ASTERISK              , BobUI::Key_Asterisk);
    insert(DIKS_PLUS_SIGN             , BobUI::Key_Plus);
    insert(DIKS_COMMA                 , BobUI::Key_Comma);
    insert(DIKS_MINUS_SIGN            , BobUI::Key_Minus);
    insert(DIKS_PERIOD                , BobUI::Key_Period);
    insert(DIKS_SLASH                 , BobUI::Key_Slash);
    insert(DIKS_0                     , BobUI::Key_0);
    insert(DIKS_1                     , BobUI::Key_1);
    insert(DIKS_2                     , BobUI::Key_2);
    insert(DIKS_3                     , BobUI::Key_3);
    insert(DIKS_4                     , BobUI::Key_4);
    insert(DIKS_5                     , BobUI::Key_5);
    insert(DIKS_6                     , BobUI::Key_6);
    insert(DIKS_7                     , BobUI::Key_7);
    insert(DIKS_8                     , BobUI::Key_8);
    insert(DIKS_9                     , BobUI::Key_9);
    insert(DIKS_COLON                 , BobUI::Key_Colon);
    insert(DIKS_SEMICOLON             , BobUI::Key_Semicolon);
    insert(DIKS_LESS_THAN_SIGN        , BobUI::Key_Less);
    insert(DIKS_EQUALS_SIGN           , BobUI::Key_Equal);
    insert(DIKS_GREATER_THAN_SIGN     , BobUI::Key_Greater);
    insert(DIKS_QUESTION_MARK         , BobUI::Key_Question);
    insert(DIKS_AT                    , BobUI::Key_At);
    insert(DIKS_CAPITAL_A             , BobUI::Key_A);
    insert(DIKS_CAPITAL_B             , BobUI::Key_B);
    insert(DIKS_CAPITAL_C             , BobUI::Key_C);
    insert(DIKS_CAPITAL_D             , BobUI::Key_D);
    insert(DIKS_CAPITAL_E             , BobUI::Key_E);
    insert(DIKS_CAPITAL_F             , BobUI::Key_F);
    insert(DIKS_CAPITAL_G             , BobUI::Key_G);
    insert(DIKS_CAPITAL_H             , BobUI::Key_H);
    insert(DIKS_CAPITAL_I             , BobUI::Key_I);
    insert(DIKS_CAPITAL_J             , BobUI::Key_J);
    insert(DIKS_CAPITAL_K             , BobUI::Key_K);
    insert(DIKS_CAPITAL_L             , BobUI::Key_L);
    insert(DIKS_CAPITAL_M             , BobUI::Key_M);
    insert(DIKS_CAPITAL_N             , BobUI::Key_N);
    insert(DIKS_CAPITAL_O             , BobUI::Key_O);
    insert(DIKS_CAPITAL_P             , BobUI::Key_P);
    insert(DIKS_CAPITAL_Q             , BobUI::Key_Q);
    insert(DIKS_CAPITAL_R             , BobUI::Key_R);
    insert(DIKS_CAPITAL_S             , BobUI::Key_S);
    insert(DIKS_CAPITAL_T             , BobUI::Key_T);
    insert(DIKS_CAPITAL_U             , BobUI::Key_U);
    insert(DIKS_CAPITAL_V             , BobUI::Key_V);
    insert(DIKS_CAPITAL_W             , BobUI::Key_W);
    insert(DIKS_CAPITAL_X             , BobUI::Key_X);
    insert(DIKS_CAPITAL_Y             , BobUI::Key_Y);
    insert(DIKS_CAPITAL_Z             , BobUI::Key_Z);
    insert(DIKS_SQUARE_BRACKET_LEFT   , BobUI::Key_BracketLeft);
    insert(DIKS_BACKSLASH             , BobUI::Key_Backslash);
    insert(DIKS_SQUARE_BRACKET_RIGHT  , BobUI::Key_BracketRight);
    insert(DIKS_CIRCUMFLEX_ACCENT     , BobUI::Key_AsciiCircum);
    insert(DIKS_UNDERSCORE            , BobUI::Key_Underscore);
    insert(DIKS_SMALL_A               , BobUI::Key_A);
    insert(DIKS_SMALL_B               , BobUI::Key_B);
    insert(DIKS_SMALL_C               , BobUI::Key_C);
    insert(DIKS_SMALL_D               , BobUI::Key_D);
    insert(DIKS_SMALL_E               , BobUI::Key_E);
    insert(DIKS_SMALL_F               , BobUI::Key_F);
    insert(DIKS_SMALL_G               , BobUI::Key_G);
    insert(DIKS_SMALL_H               , BobUI::Key_H);
    insert(DIKS_SMALL_I               , BobUI::Key_I);
    insert(DIKS_SMALL_J               , BobUI::Key_J);
    insert(DIKS_SMALL_K               , BobUI::Key_K);
    insert(DIKS_SMALL_L               , BobUI::Key_L);
    insert(DIKS_SMALL_M               , BobUI::Key_M);
    insert(DIKS_SMALL_N               , BobUI::Key_N);
    insert(DIKS_SMALL_O               , BobUI::Key_O);
    insert(DIKS_SMALL_P               , BobUI::Key_P);
    insert(DIKS_SMALL_Q               , BobUI::Key_Q);
    insert(DIKS_SMALL_R               , BobUI::Key_R);
    insert(DIKS_SMALL_S               , BobUI::Key_S);
    insert(DIKS_SMALL_T               , BobUI::Key_T);
    insert(DIKS_SMALL_U               , BobUI::Key_U);
    insert(DIKS_SMALL_V               , BobUI::Key_V);
    insert(DIKS_SMALL_W               , BobUI::Key_W);
    insert(DIKS_SMALL_X               , BobUI::Key_X);
    insert(DIKS_SMALL_Y               , BobUI::Key_Y);
    insert(DIKS_SMALL_Z               , BobUI::Key_Z);
    insert(DIKS_CURLY_BRACKET_LEFT    , BobUI::Key_BraceLeft);
    insert(DIKS_VERTICAL_BAR          , BobUI::Key_Bar);
    insert(DIKS_CURLY_BRACKET_RIGHT   , BobUI::Key_BraceRight);
    insert(DIKS_TILDE                 , BobUI::Key_AsciiTilde);
}

QDirectFbScreen *toDfbScreen(QWindow *window)
{
    return static_cast<QDirectFbScreen*>(window->screen()->handle());
}

IDirectFBDisplayLayer *toDfbLayer(QPlatformScreen *screen)
{
    return static_cast<QDirectFbScreen*>(screen)->dfbLayer();
}

BOBUI_END_NAMESPACE
