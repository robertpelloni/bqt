// Copyright (C) 2022 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QEXCEPTIONHANDLING_H
#define QEXCEPTIONHANDLING_H

#include <BobUICore/bobuiconfigmacros.h>
#include <BobUICore/qcompilerdetection.h>
#include <BobUICore/bobuicoreexports.h>

#if 0
#pragma bobui_class(BobUIExceptionHandling)
#pragma bobui_sync_stop_processing
#endif

BOBUI_BEGIN_NAMESPACE

/* These wrap try/catch so we can switch off exceptions later.

   Beware - do not use more than one BOBUI_CATCH per BOBUI_TRY, and do not use
   the exception instance in the catch block.
   If you can't live with those constraints, don't use these macros.
   Use the BOBUI_NO_EXCEPTIONS macro to protect your code instead.
*/
#ifdef BOBUI_NO_EXCEPTIONS
#  define BOBUI_TRY if (true)
#  define BOBUI_CATCH(A) else
#  define BOBUI_THROW(A) bobui_noop()
#  define BOBUI_RETHROW bobui_noop()
#else
#  define BOBUI_TRY try
#  define BOBUI_CATCH(A) catch (A)
#  define BOBUI_THROW(A) throw A
#  define BOBUI_RETHROW throw
#endif

#if BOBUI_CORE_REMOVED_SINCE(6, 9)
Q_NORETURN Q_DECL_COLD_FUNCTION Q_CORE_EXPORT void qTerminate() noexcept;
#endif

BOBUI_END_NAMESPACE

#endif // QEXCEPTIONHANDLING_H
