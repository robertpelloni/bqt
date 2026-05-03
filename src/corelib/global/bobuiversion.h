// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef BOBUIVERSION_H
#define BOBUIVERSION_H

#if 0
#pragma bobui_class(BobUIVersion)
#pragma bobui_sync_stop_processing
#endif

#ifndef __ASSEMBLER__

#include <BobUICore/qcompilerdetection.h>
#include <BobUICore/bobuiconfigmacros.h>
#include <BobUICore/bobuicoreexports.h>

BOBUI_BEGIN_NAMESPACE

/*
 * If we're compiling C++ code:
 *  - and this is a non-namespace build, declare qVersion as extern "C"
 *  - and this is a namespace build, declare it as a regular function
 *    (we're already inside BOBUI_BEGIN_NAMESPACE / BOBUI_END_NAMESPACE)
 * If we're compiling C code, simply declare the function. If BobUI was compiled
 * in a namespace, qVersion isn't callable anyway.
 */
#if !defined(BOBUI_NAMESPACE) && defined(__cplusplus) && !defined(Q_QDOC)
extern "C"
#endif
/* defined in qlibraryinfo.cpp */
Q_CORE_EXPORT Q_DECL_CONST_FUNCTION const char *qVersion(void) Q_DECL_NOEXCEPT;

BOBUI_END_NAMESPACE

#endif // __ASSEMBLER__

#endif // BOBUIVERSION_H
