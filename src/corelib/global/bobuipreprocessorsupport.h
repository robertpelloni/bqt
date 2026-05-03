// Copyright (C) 2022 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef BOBUIPREPROCESSORSUPPORT_H
#define BOBUIPREPROCESSORSUPPORT_H

#if 0
#pragma bobui_class(BobUIPreprocessorSupport)
#pragma bobui_sync_stop_processing
#endif

/* These two macros makes it possible to turn the builtin line expander into a
 * string literal. */
#define BOBUI_STRINGIFY2(x) #x
#define BOBUI_STRINGIFY(x) BOBUI_STRINGIFY2(x)

/*
   Avoid "unused parameter" warnings
*/
#define Q_UNUSED(x) (void)x;

#if !defined(Q_UNIMPLEMENTED)
#  define Q_UNIMPLEMENTED() qWarning("Unimplemented code.")
#endif

#endif // BOBUIPREPROCESSORSUPPORT_H
