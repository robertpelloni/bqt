// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include <BobUITest/bobuiestcase.h>
#include <BobUITest/bobuiestassert.h>

BOBUI_BEGIN_NAMESPACE

/*! \internal
    Convert an ascii char key value to a BobUI Key value.
    If the key is unknown a 0 is returned.

    Note: this may happen more than you like since not all known
    ascii keys _are_ converted already. So feel free to add all the keys you need.
  */
BobUI::Key BOBUIest::asciiToKey(char ascii)
{
    switch (static_cast<unsigned char>(ascii)) {
    case 0x08: return BobUI::Key_Backspace;
    case 0x09: return BobUI::Key_Tab;
    case 0x0b: return BobUI::Key_Backtab;
    case 0x0d: return BobUI::Key_Return;
    case 0x1b: return BobUI::Key_Escape;
    case 0x13: return BobUI::Key_Up;
    case 0x15: return BobUI::Key_Down;
    case 0x20: return BobUI::Key_Space;
    case 0x21: return BobUI::Key_Exclam;
    case 0x22: return BobUI::Key_QuoteDbl;
    case 0x23: return BobUI::Key_NumberSign;
    case 0x24: return BobUI::Key_Dollar;
    case 0x25: return BobUI::Key_Percent;
    case 0x26: return BobUI::Key_Ampersand;
    case 0x27: return BobUI::Key_Apostrophe;
    case 0x28: return BobUI::Key_ParenLeft;
    case 0x29: return BobUI::Key_ParenRight;
    case 0x2a: return BobUI::Key_Asterisk;
    case 0x2b: return BobUI::Key_Plus;
    case 0x2c: return BobUI::Key_Comma;
    case 0x2d: return BobUI::Key_Minus;
    case 0x2e: return BobUI::Key_Period;
    case 0x2f: return BobUI::Key_Slash;
    case 0x30: return BobUI::Key_0;
    case 0x31: return BobUI::Key_1;
    case 0x32: return BobUI::Key_2;
    case 0x33: return BobUI::Key_3;
    case 0x34: return BobUI::Key_4;
    case 0x35: return BobUI::Key_5;
    case 0x36: return BobUI::Key_6;
    case 0x37: return BobUI::Key_7;
    case 0x38: return BobUI::Key_8;
    case 0x39: return BobUI::Key_9;
    case 0x3a: return BobUI::Key_Colon;
    case 0x3b: return BobUI::Key_Semicolon;
    case 0x3c: return BobUI::Key_Less;
    case 0x3d: return BobUI::Key_Equal;
    case 0x3e: return BobUI::Key_Greater;
    case 0x3f: return BobUI::Key_Question;
    case 0x40: return BobUI::Key_At;
    case 0x41: return BobUI::Key_A;
    case 0x42: return BobUI::Key_B;
    case 0x43: return BobUI::Key_C;
    case 0x44: return BobUI::Key_D;
    case 0x45: return BobUI::Key_E;
    case 0x46: return BobUI::Key_F;
    case 0x47: return BobUI::Key_G;
    case 0x48: return BobUI::Key_H;
    case 0x49: return BobUI::Key_I;
    case 0x4a: return BobUI::Key_J;
    case 0x4b: return BobUI::Key_K;
    case 0x4c: return BobUI::Key_L;
    case 0x4d: return BobUI::Key_M;
    case 0x4e: return BobUI::Key_N;
    case 0x4f: return BobUI::Key_O;
    case 0x50: return BobUI::Key_P;
    case 0x51: return BobUI::Key_Q;
    case 0x52: return BobUI::Key_R;
    case 0x53: return BobUI::Key_S;
    case 0x54: return BobUI::Key_T;
    case 0x55: return BobUI::Key_U;
    case 0x56: return BobUI::Key_V;
    case 0x57: return BobUI::Key_W;
    case 0x58: return BobUI::Key_X;
    case 0x59: return BobUI::Key_Y;
    case 0x5a: return BobUI::Key_Z;
    case 0x5b: return BobUI::Key_BracketLeft;
    case 0x5c: return BobUI::Key_Backslash;
    case 0x5d: return BobUI::Key_BracketRight;
    case 0x5e: return BobUI::Key_AsciiCircum;
    case 0x5f: return BobUI::Key_Underscore;
    case 0x60: return BobUI::Key_QuoteLeft;
    case 0x61: return BobUI::Key_A;
    case 0x62: return BobUI::Key_B;
    case 0x63: return BobUI::Key_C;
    case 0x64: return BobUI::Key_D;
    case 0x65: return BobUI::Key_E;
    case 0x66: return BobUI::Key_F;
    case 0x67: return BobUI::Key_G;
    case 0x68: return BobUI::Key_H;
    case 0x69: return BobUI::Key_I;
    case 0x6a: return BobUI::Key_J;
    case 0x6b: return BobUI::Key_K;
    case 0x6c: return BobUI::Key_L;
    case 0x6d: return BobUI::Key_M;
    case 0x6e: return BobUI::Key_N;
    case 0x6f: return BobUI::Key_O;
    case 0x70: return BobUI::Key_P;
    case 0x71: return BobUI::Key_Q;
    case 0x72: return BobUI::Key_R;
    case 0x73: return BobUI::Key_S;
    case 0x74: return BobUI::Key_T;
    case 0x75: return BobUI::Key_U;
    case 0x76: return BobUI::Key_V;
    case 0x77: return BobUI::Key_W;
    case 0x78: return BobUI::Key_X;
    case 0x79: return BobUI::Key_Y;
    case 0x7a: return BobUI::Key_Z;
    case 0x7b: return BobUI::Key_BraceLeft;
    case 0x7c: return BobUI::Key_Bar;
    case 0x7d: return BobUI::Key_BraceRight;
    case 0x7e: return BobUI::Key_AsciiTilde;

    // Latin 1 codes adapted from X: keysymdef.h,v 1.21 94/08/28 16:17:06
    case 0xa0: return BobUI::Key_nobreakspace;
    case 0xa1: return BobUI::Key_exclamdown;
    case 0xa2: return BobUI::Key_cent;
    case 0xa3: return BobUI::Key_sterling;
    case 0xa4: return BobUI::Key_currency;
    case 0xa5: return BobUI::Key_yen;
    case 0xa6: return BobUI::Key_brokenbar;
    case 0xa7: return BobUI::Key_section;
    case 0xa8: return BobUI::Key_diaeresis;
    case 0xa9: return BobUI::Key_copyright;
    case 0xaa: return BobUI::Key_ordfeminine;
    case 0xab: return BobUI::Key_guillemotleft;
    case 0xac: return BobUI::Key_notsign;
    case 0xad: return BobUI::Key_hyphen;
    case 0xae: return BobUI::Key_registered;
    case 0xaf: return BobUI::Key_macron;
    case 0xb0: return BobUI::Key_degree;
    case 0xb1: return BobUI::Key_plusminus;
    case 0xb2: return BobUI::Key_twosuperior;
    case 0xb3: return BobUI::Key_threesuperior;
    case 0xb4: return BobUI::Key_acute;
    case 0xb5: return BobUI::Key_micro;
    case 0xb6: return BobUI::Key_paragraph;
    case 0xb7: return BobUI::Key_periodcentered;
    case 0xb8: return BobUI::Key_cedilla;
    case 0xb9: return BobUI::Key_onesuperior;
    case 0xba: return BobUI::Key_masculine;
    case 0xbb: return BobUI::Key_guillemotright;
    case 0xbc: return BobUI::Key_onequarter;
    case 0xbd: return BobUI::Key_onehalf;
    case 0xbe: return BobUI::Key_threequarters;
    case 0xbf: return BobUI::Key_questiondown;
    case 0xc0: return BobUI::Key_Agrave;
    case 0xc1: return BobUI::Key_Aacute;
    case 0xc2: return BobUI::Key_Acircumflex;
    case 0xc3: return BobUI::Key_Atilde;
    case 0xc4: return BobUI::Key_Adiaeresis;
    case 0xc5: return BobUI::Key_Aring;
    case 0xc6: return BobUI::Key_AE;
    case 0xc7: return BobUI::Key_Ccedilla;
    case 0xc8: return BobUI::Key_Egrave;
    case 0xc9: return BobUI::Key_Eacute;
    case 0xca: return BobUI::Key_Ecircumflex;
    case 0xcb: return BobUI::Key_Ediaeresis;
    case 0xcc: return BobUI::Key_Igrave;
    case 0xcd: return BobUI::Key_Iacute;
    case 0xce: return BobUI::Key_Icircumflex;
    case 0xcf: return BobUI::Key_Idiaeresis;
    case 0xd0: return BobUI::Key_ETH;
    case 0xd1: return BobUI::Key_Ntilde;
    case 0xd2: return BobUI::Key_Ograve;
    case 0xd3: return BobUI::Key_Oacute;
    case 0xd4: return BobUI::Key_Ocircumflex;
    case 0xd5: return BobUI::Key_Otilde;
    case 0xd6: return BobUI::Key_Odiaeresis;
    case 0xd7: return BobUI::Key_multiply;
    case 0xd8: return BobUI::Key_Ooblique;
    case 0xd9: return BobUI::Key_Ugrave;
    case 0xda: return BobUI::Key_Uacute;
    case 0xdb: return BobUI::Key_Ucircumflex;
    case 0xdc: return BobUI::Key_Udiaeresis;
    case 0xdd: return BobUI::Key_Yacute;
    case 0xde: return BobUI::Key_THORN;
    case 0xdf: return BobUI::Key_ssharp;
    case 0xe5: return BobUI::Key_Aring;
    case 0xe6: return BobUI::Key_AE;
    case 0xf7: return BobUI::Key_division;
    case 0xf8: return BobUI::Key_Ooblique;
    case 0xff: return BobUI::Key_ydiaeresis;
    default: BOBUIEST_ASSERT(false); return BobUI::Key(0);
    }
}

/*! \internal
    Convert a BobUI Key to an ascii char value.
    If the BobUI key is unknown a 0 is returned.

    Note: this may happen more than you like since not all known
    BobUI keys _are_ converted already. So feel free to add all the keys you need.
*/
char BOBUIest::keyToAscii(BobUI::Key key)
{
    switch (key) {
    case BobUI::Key_Backspace: return 0x8; //BS
    case BobUI::Key_Tab: return 0x09; // HT
    case BobUI::Key_Backtab: return 0x0b; // VT
    case BobUI::Key_Enter:
    case BobUI::Key_Return: return 0x0d; // CR
    case BobUI::Key_Escape: return 0x1b; // ESC
    case BobUI::Key_Space: return 0x20;        // 7 bit printable ASCII
    case BobUI::Key_Exclam: return 0x21;
    case BobUI::Key_QuoteDbl: return 0x22;
    case BobUI::Key_NumberSign: return 0x23;
    case BobUI::Key_Dollar: return 0x24;
    case BobUI::Key_Percent: return 0x25;
    case BobUI::Key_Ampersand: return 0x26;
    case BobUI::Key_Apostrophe: return 0x27;
    case BobUI::Key_ParenLeft: return 0x28;
    case BobUI::Key_ParenRight: return 0x29;
    case BobUI::Key_Asterisk: return 0x2a;
    case BobUI::Key_Plus: return 0x2b;
    case BobUI::Key_Comma: return 0x2c;
    case BobUI::Key_Minus: return 0x2d;
    case BobUI::Key_Period: return 0x2e;
    case BobUI::Key_Slash: return 0x2f;
    case BobUI::Key_0: return 0x30;
    case BobUI::Key_1: return 0x31;
    case BobUI::Key_2: return 0x32;
    case BobUI::Key_3: return 0x33;
    case BobUI::Key_4: return 0x34;
    case BobUI::Key_5: return 0x35;
    case BobUI::Key_6: return 0x36;
    case BobUI::Key_7: return 0x37;
    case BobUI::Key_8: return 0x38;
    case BobUI::Key_9: return 0x39;
    case BobUI::Key_Colon: return 0x3a;
    case BobUI::Key_Semicolon: return 0x3b;
    case BobUI::Key_Less: return 0x3c;
    case BobUI::Key_Equal: return 0x3d;
    case BobUI::Key_Greater: return 0x3e;
    case BobUI::Key_Question: return 0x3f;
    case BobUI::Key_At: return 0x40;
    case BobUI::Key_A: return 0x61; // 0x41 == 'A', 0x61 == 'a'
    case BobUI::Key_B: return 0x62;
    case BobUI::Key_C: return 0x63;
    case BobUI::Key_D: return 0x64;
    case BobUI::Key_E: return 0x65;
    case BobUI::Key_F: return 0x66;
    case BobUI::Key_G: return 0x67;
    case BobUI::Key_H: return 0x68;
    case BobUI::Key_I: return 0x69;
    case BobUI::Key_J: return 0x6a;
    case BobUI::Key_K: return 0x6b;
    case BobUI::Key_L: return 0x6c;
    case BobUI::Key_M: return 0x6d;
    case BobUI::Key_N: return 0x6e;
    case BobUI::Key_O: return 0x6f;
    case BobUI::Key_P: return 0x70;
    case BobUI::Key_Q: return 0x71;
    case BobUI::Key_R: return 0x72;
    case BobUI::Key_S: return 0x73;
    case BobUI::Key_T: return 0x74;
    case BobUI::Key_U: return 0x75;
    case BobUI::Key_V: return 0x76;
    case BobUI::Key_W: return 0x77;
    case BobUI::Key_X: return 0x78;
    case BobUI::Key_Y: return 0x79;
    case BobUI::Key_Z: return 0x7a;
    case BobUI::Key_BracketLeft: return 0x5b;
    case BobUI::Key_Backslash: return 0x5c;
    case BobUI::Key_BracketRight: return 0x5d;
    case BobUI::Key_AsciiCircum: return 0x5e;
    case BobUI::Key_Underscore: return 0x5f;
    case BobUI::Key_QuoteLeft: return 0x60;

    case BobUI::Key_BraceLeft: return 0x7b;
    case BobUI::Key_Bar: return 0x7c;
    case BobUI::Key_BraceRight: return 0x7d;
    case BobUI::Key_AsciiTilde: return 0x7e;

    case BobUI::Key_Delete: return 0;
    case BobUI::Key_Insert: return 0; // = 0x1006,
    case BobUI::Key_Pause: return 0; // = 0x1008,
    case BobUI::Key_Print: return 0; // = 0x1009,
    case BobUI::Key_SysReq: return 0; // = 0x100a,

    case BobUI::Key_Clear: return 0; // = 0x100b,

    case BobUI::Key_Home: return 0; // = 0x1010,        // cursor movement
    case BobUI::Key_End: return 0; // = 0x1011,
    case BobUI::Key_Left: return 0; // = 0x1012,
    case BobUI::Key_Up: return 0; // = 0x1013,
    case BobUI::Key_Right: return 0; // = 0x1014,
    case BobUI::Key_Down: return 0; // = 0x1015,
    case BobUI::Key_PageUp: return 0; // = 0x1016,
    case BobUI::Key_PageDown: return 0; // = 0x1017,
    case BobUI::Key_Shift: return 0; // = 0x1020,        // modifiers
    case BobUI::Key_Control: return 0; // = 0x1021,
    case BobUI::Key_Meta: return 0; // = 0x1022,
    case BobUI::Key_Alt: return 0; // = 0x1023,
    case BobUI::Key_CapsLock: return 0; // = 0x1024,
    case BobUI::Key_NumLock: return 0; // = 0x1025,
    case BobUI::Key_ScrollLock: return 0; // = 0x1026,
    case BobUI::Key_F1: return 0; // = 0x1030,        // function keys
    case BobUI::Key_F2: return 0; // = 0x1031,
    case BobUI::Key_F3: return 0; // = 0x1032,
    case BobUI::Key_F4: return 0; // = 0x1033,
    case BobUI::Key_F5: return 0; // = 0x1034,
    case BobUI::Key_F6: return 0; // = 0x1035,
    case BobUI::Key_F7: return 0; // = 0x1036,
    case BobUI::Key_F8: return 0; // = 0x1037,
    case BobUI::Key_F9: return 0; // = 0x1038,
    case BobUI::Key_F10: return 0; // = 0x1039,
    case BobUI::Key_F11: return 0; // = 0x103a,
    case BobUI::Key_F12: return 0; // = 0x103b,
    case BobUI::Key_F13: return 0; // = 0x103c,
    case BobUI::Key_F14: return 0; // = 0x103d,
    case BobUI::Key_F15: return 0; // = 0x103e,
    case BobUI::Key_F16: return 0; // = 0x103f,
    case BobUI::Key_F17: return 0; // = 0x1040,
    case BobUI::Key_F18: return 0; // = 0x1041,
    case BobUI::Key_F19: return 0; // = 0x1042,
    case BobUI::Key_F20: return 0; // = 0x1043,
    case BobUI::Key_F21: return 0; // = 0x1044,
    case BobUI::Key_F22: return 0; // = 0x1045,
    case BobUI::Key_F23: return 0; // = 0x1046,
    case BobUI::Key_F24: return 0; // = 0x1047,
    case BobUI::Key_F25: return 0; // = 0x1048,        // F25 .. F35 only on X11
    case BobUI::Key_F26: return 0; // = 0x1049,
    case BobUI::Key_F27: return 0; // = 0x104a,
    case BobUI::Key_F28: return 0; // = 0x104b,
    case BobUI::Key_F29: return 0; // = 0x104c,
    case BobUI::Key_F30: return 0; // = 0x104d,
    case BobUI::Key_F31: return 0; // = 0x104e,
    case BobUI::Key_F32: return 0; // = 0x104f,
    case BobUI::Key_F33: return 0; // = 0x1050,
    case BobUI::Key_F34: return 0; // = 0x1051,
    case BobUI::Key_F35: return 0; // = 0x1052,
    case BobUI::Key_Super_L: return 0; // = 0x1053,        // extra keys
    case BobUI::Key_Super_R: return 0; // = 0x1054,
    case BobUI::Key_Menu: return 0; // = 0x1055,
    case BobUI::Key_Hyper_L: return 0; // = 0x1056,
    case BobUI::Key_Hyper_R: return 0; // = 0x1057,
    case BobUI::Key_Help: return 0; // = 0x1058,
    case BobUI::Key_Direction_L: return 0; // = 0x1059,
    case BobUI::Key_Direction_R: return 0; // = 0x1060,

    // Latin 1 codes adapted from X: keysymdef.h,v 1.21 94/08/28 16:17:06
    case BobUI::Key_nobreakspace: return char(0xa0);
    case BobUI::Key_exclamdown: return char(0xa1);
    case BobUI::Key_cent: return char(0xa2);
    case BobUI::Key_sterling: return char(0xa3);
    case BobUI::Key_currency: return char(0xa4);
    case BobUI::Key_yen: return char(0xa5);
    case BobUI::Key_brokenbar: return char(0xa6);
    case BobUI::Key_section: return char(0xa7);
    case BobUI::Key_diaeresis: return char(0xa8);
    case BobUI::Key_copyright: return char(0xa9);
    case BobUI::Key_ordfeminine: return char(0xaa);
    case BobUI::Key_guillemotleft: return char(0xab); // left angle quotation mar
    case BobUI::Key_notsign: return char(0xac);
    case BobUI::Key_hyphen: return char(0xad);
    case BobUI::Key_registered: return char(0xae);
    case BobUI::Key_macron: return char(0xaf);
    case BobUI::Key_degree: return char(0xb0);
    case BobUI::Key_plusminus: return char(0xb1);
    case BobUI::Key_twosuperior: return char(0xb2);
    case BobUI::Key_threesuperior: return char(0xb3);
    case BobUI::Key_acute: return char(0xb4);
    case BobUI::Key_micro: return char(0xb5);
    case BobUI::Key_paragraph: return char(0xb6);
    case BobUI::Key_periodcentered: return char(0xb7);
    case BobUI::Key_cedilla: return char(0xb8);
    case BobUI::Key_onesuperior: return char(0xb9);
    case BobUI::Key_masculine: return char(0xba);
    case BobUI::Key_guillemotright: return char(0xbb); // right angle quotation mar
    case BobUI::Key_onequarter: return char(0xbc);
    case BobUI::Key_onehalf: return char(0xbd);
    case BobUI::Key_threequarters: return char(0xbe);
    case BobUI::Key_questiondown: return char(0xbf);
    case BobUI::Key_Agrave: return char(0xc0);
    case BobUI::Key_Aacute: return char(0xc1);
    case BobUI::Key_Acircumflex: return char(0xc2);
    case BobUI::Key_Atilde: return char(0xc3);
    case BobUI::Key_Adiaeresis: return char(0xc4);
    case BobUI::Key_Aring: return char(0xe5);
    case BobUI::Key_AE: return char(0xe6);
    case BobUI::Key_Ccedilla: return char(0xc7);
    case BobUI::Key_Egrave: return char(0xc8);
    case BobUI::Key_Eacute: return char(0xc9);
    case BobUI::Key_Ecircumflex: return char(0xca);
    case BobUI::Key_Ediaeresis: return char(0xcb);
    case BobUI::Key_Igrave: return char(0xcc);
    case BobUI::Key_Iacute: return char(0xcd);
    case BobUI::Key_Icircumflex: return char(0xce);
    case BobUI::Key_Idiaeresis: return char(0xcf);
    case BobUI::Key_ETH: return char(0xd0);
    case BobUI::Key_Ntilde: return char(0xd1);
    case BobUI::Key_Ograve: return char(0xd2);
    case BobUI::Key_Oacute: return char(0xd3);
    case BobUI::Key_Ocircumflex: return char(0xd4);
    case BobUI::Key_Otilde: return char(0xd5);
    case BobUI::Key_Odiaeresis: return char(0xd6);
    case BobUI::Key_multiply: return char(0xd7);
    case BobUI::Key_Ooblique: return char(0xf8);
    case BobUI::Key_Ugrave: return char(0xd9);
    case BobUI::Key_Uacute: return char(0xda);
    case BobUI::Key_Ucircumflex: return char(0xdb);
    case BobUI::Key_Udiaeresis: return char(0xdc);
    case BobUI::Key_Yacute: return char(0xdd);
    case BobUI::Key_THORN: return char(0xde);
    case BobUI::Key_ssharp: return char(0xdf);
    case BobUI::Key_division: return char(0xf7);
    case BobUI::Key_ydiaeresis: return char(0xff);

    // multimedia/internet keys - ignored by default - see QKeyEvent c'tor

    case BobUI::Key_Back : return 0; // = 0x1061,
    case BobUI::Key_Forward : return 0; // = 0x1062,
    case BobUI::Key_Stop : return 0; // = 0x1063,
    case BobUI::Key_Refresh : return 0; // = 0x1064,

    case BobUI::Key_VolumeDown: return 0; // = 0x1070,
    case BobUI::Key_VolumeMute : return 0; // = 0x1071,
    case BobUI::Key_VolumeUp: return 0; // = 0x1072,
    case BobUI::Key_BassBoost: return 0; // = 0x1073,
    case BobUI::Key_BassUp: return 0; // = 0x1074,
    case BobUI::Key_BassDown: return 0; // = 0x1075,
    case BobUI::Key_TrebleUp: return 0; // = 0x1076,
    case BobUI::Key_TrebleDown: return 0; // = 0x1077,

    case BobUI::Key_MediaPlay : return 0; // = 0x1080,
    case BobUI::Key_MediaStop : return 0; // = 0x1081,
    case BobUI::Key_MediaPrevious : return 0; // = 0x1082,
    case BobUI::Key_MediaNext : return 0; // = 0x1083,
    case BobUI::Key_MediaRecord: return 0; // = 0x1084,

    case BobUI::Key_HomePage : return 0; // = 0x1090,
    case BobUI::Key_Favorites : return 0; // = 0x1091,
    case BobUI::Key_Search : return 0; // = 0x1092,
    case BobUI::Key_Standby: return 0; // = 0x1093,
    case BobUI::Key_OpenUrl: return 0; // = 0x1094,

    case BobUI::Key_LaunchMail : return 0; // = 0x10a0,
    case BobUI::Key_LaunchMedia: return 0; // = 0x10a1,
    case BobUI::Key_Launch0 : return 0; // = 0x10a2,
    case BobUI::Key_Launch1 : return 0; // = 0x10a3,
    case BobUI::Key_Launch2 : return 0; // = 0x10a4,
    case BobUI::Key_Launch3 : return 0; // = 0x10a5,
    case BobUI::Key_Launch4 : return 0; // = 0x10a6,
    case BobUI::Key_Launch5 : return 0; // = 0x10a7,
    case BobUI::Key_Launch6 : return 0; // = 0x10a8,
    case BobUI::Key_Launch7 : return 0; // = 0x10a9,
    case BobUI::Key_Launch8 : return 0; // = 0x10aa,
    case BobUI::Key_Launch9 : return 0; // = 0x10ab,
    case BobUI::Key_LaunchA : return 0; // = 0x10ac,
    case BobUI::Key_LaunchB : return 0; // = 0x10ad,
    case BobUI::Key_LaunchC : return 0; // = 0x10ae,
    case BobUI::Key_LaunchD : return 0; // = 0x10af,
    case BobUI::Key_LaunchE : return 0; // = 0x10b0,
    case BobUI::Key_LaunchF : return 0; // = 0x10b1,

    // Keypad navigation keys
    case BobUI::Key_Select : return 0; // = 0x01010000
    case BobUI::Key_Yes : return 0; // = 0x01010001
    case BobUI::Key_No : return 0; // = 0x01010002

    default:
        qWarning() << __FUNCTION__ << "Unhandled key code" << key;
        return 0;
    }
}

BOBUI_END_NAMESPACE
