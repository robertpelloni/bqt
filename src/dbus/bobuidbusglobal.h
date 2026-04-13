<<<<<<< HEAD
// Copyright (C) 2016 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// Qt-Security score:significant reason:default
=======
// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default
>>>>>>> origin/dev

#ifndef BOBUIDBUSGLOBAL_H
#define BOBUIDBUSGLOBAL_H

<<<<<<< HEAD
#include <QtCore/qglobal.h>
#include <QtCore/qmetatype.h>
#include <QtCore/qvariant.h>
=======
#include <BobUICore/qglobal.h>
#include <BobUICore/qmetatype.h>
#include <BobUICore/qvariant.h>
>>>>>>> origin/dev

#ifndef Q_MOC_RUN
# define Q_NOREPLY
#endif

#ifdef Q_CC_MSVC
<<<<<<< HEAD
#include <QtCore/qlist.h>
#include <QtCore/qset.h>
#endif

#ifndef QT_NO_DBUS
#include <BobUIDBus/bobuidbusexports.h>
#endif // QT_NO_DBUS
=======
#include <BobUICore/qlist.h>
#include <BobUICore/qset.h>
#endif

#ifndef BOBUI_NO_DBUS
#include <BobUIDBus/bobuidbusexports.h>
#endif // BOBUI_NO_DBUS
>>>>>>> origin/dev

#endif
