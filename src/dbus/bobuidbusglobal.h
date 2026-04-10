// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#ifndef BOBUIDBUSGLOBAL_H
#define BOBUIDBUSGLOBAL_H

#include <BobUICore/qglobal.h>
#include <BobUICore/qmetatype.h>
#include <BobUICore/qvariant.h>

#ifndef Q_MOC_RUN
# define Q_NOREPLY
#endif

#ifdef Q_CC_MSVC
#include <BobUICore/qlist.h>
#include <BobUICore/qset.h>
#endif

#ifndef BOBUI_NO_DBUS
#include <BobUIDBus/bobuidbusexports.h>
#endif // BOBUI_NO_DBUS

#endif
