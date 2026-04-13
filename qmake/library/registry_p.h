// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only WITH BobUI-GPL-exception-1.0

#ifndef BOBUI_WINDOWS_REGISTRY_H
#define BOBUI_WINDOWS_REGISTRY_H

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

#ifdef Q_OS_WIN32
   #include <BobUICore/bobui_windows.h>
#else
    typedef void* HKEY;
#endif

#include <BobUICore/qstring.h>

BOBUI_BEGIN_NAMESPACE

/**
 * Read a value from the Windows registry.
 *
 * If the key is not found, or the registry cannot be accessed (for example
 * if this code is compiled for a platform other than Windows), a null
 * string is returned.
 *
 * 32-bit code reads from the registry's 32 bit view (Wow6432Node),
 * 64 bit code reads from the 64 bit view.
 * Pass KEY_WOW64_32KEY to access the 32 bit view regardless of the
 * application's architecture, KEY_WOW64_64KEY respectively.
 */
QString bobui_readRegistryKey(HKEY parentHandle, const QString &rSubkey,
                           unsigned long options = 0);

BOBUI_END_NAMESPACE

#endif // BOBUI_WINDOWS_REGISTRY_H

