// Copyright (C) 2016 The BobUI Company Ltd.
// Copyright (C) 2013 Konstantin Ritt
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QHARFBUZZNG_P_H
#define QHARFBUZZNG_P_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the BobUI API.  It exists for the convenience
// of other BobUI classes.  This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//

#include <BobUIGui/private/bobuiguiglobal_p.h>

BOBUI_REQUIRE_CONFIG(harfbuzz);

#include <BobUICore/qchar.h>

#if defined(BOBUI_BUILD_GUI_LIB)
#  include <hb.h>
#else
// a minimal set of HB types required for BobUI libs other than Gui

typedef struct hb_face_t hb_face_t;
typedef struct hb_font_t hb_font_t;

#endif // BOBUI_BUILD_GUI_LIB

BOBUI_BEGIN_NAMESPACE

class QFontEngine;

#if defined(BOBUI_BUILD_GUI_LIB)

// Unicode

hb_script_t hb_bobui_script_to_script(QChar::Script script);
QChar::Script hb_bobui_script_from_script(hb_script_t script);

hb_unicode_funcs_t *hb_bobui_get_unicode_funcs();

#endif // BOBUI_BUILD_GUI_LIB

// Font

Q_GUI_EXPORT hb_face_t *hb_bobui_face_get_for_engine(QFontEngine *fe);
Q_GUI_EXPORT hb_font_t *hb_bobui_font_get_for_engine(QFontEngine *fe);

Q_GUI_EXPORT void hb_bobui_font_set_use_design_metrics(hb_font_t *font, uint value);
Q_GUI_EXPORT uint hb_bobui_font_get_use_design_metrics(hb_font_t *font);

BOBUI_END_NAMESPACE

#endif // QHARFBUZZNG_P_H
