// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

/*
 * This is a precompiled header file for use in Xcode / Mac GCC /
 * GCC >= 3.4 / VC to greatly speed the building of BobUI. It may also be
 * of use to people developing their own project, but it is probably
 * better to define your own header.  Use of this header is currently
 * UNSUPPORTED.
 */

#include "../../gui/kernel/bobui_gui_pch.h"

#if defined __cplusplus
#include <bobuiwidgetsexports.h>
#include <bobuiwidgetsglobal.h>
#include <qapplication.h>
#if BOBUI_CONFIG(abstractbutton)
#  include <qabstractbutton.h>
#endif
#include <qabstractscrollarea.h>
#if BOBUI_CONFIG(abstractslider)
#  include <qabstractslider.h>
#endif
#if BOBUI_CONFIG(action)
#  include <qaction.h>
#endif
#include <qcommonstyle.h>
#include <qlayout.h>
#include <qstyle.h>
#include <qwidget.h>
#endif
