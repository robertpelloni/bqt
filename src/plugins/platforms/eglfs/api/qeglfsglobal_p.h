// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QEGLFSGLOBAL_H
#define QEGLFSGLOBAL_H

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

#include <BobUICore/qglobal.h>

#include <BobUIGui/private/bobui_egl_p.h>

BOBUI_BEGIN_NAMESPACE

#ifdef BOBUI_BUILD_EGL_DEVICE_LIB
#define Q_EGLFS_EXPORT Q_DECL_EXPORT
#else
#define Q_EGLFS_EXPORT Q_DECL_IMPORT
#endif

BOBUI_END_NAMESPACE

#endif
