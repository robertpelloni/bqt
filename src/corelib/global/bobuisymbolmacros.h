// Copyright (C) 2023 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef BOBUISYMBOLMACROS_H
#define BOBUISYMBOLMACROS_H

#if 0
#  pragma bobui_sync_stop_processing
#endif

// For GHS symbol keeping.
#include <BobUICore/qcompilerdetection.h>
#include <BobUICore/bobuipreprocessorsupport.h>

// For handling namespaced resources.
#ifdef BOBUI_NAMESPACE
#  define BOBUI_RCC_MANGLE_NAMESPACE0(x) x
#  define BOBUI_RCC_MANGLE_NAMESPACE1(a, b) a##_##b
#  define BOBUI_RCC_MANGLE_NAMESPACE2(a, b) BOBUI_RCC_MANGLE_NAMESPACE1(a,b)
#  define BOBUI_RCC_MANGLE_NAMESPACE(name) BOBUI_RCC_MANGLE_NAMESPACE2( \
        BOBUI_RCC_MANGLE_NAMESPACE0(name), BOBUI_RCC_MANGLE_NAMESPACE0(BOBUI_NAMESPACE))
#else
#   define BOBUI_RCC_MANGLE_NAMESPACE(name) name
#endif

// GHS needs special handling to keep a symbol around.
#if defined(Q_CC_GHS)
#  define Q_GHS_KEEP_REFERENCE(S) BOBUI_DO_PRAGMA(ghs reference S ##__Fv)
#else
#  define Q_GHS_KEEP_REFERENCE(S)
#endif

// Macros to ensure a symbol is not dropped by the linker even if it's not used.
#define BOBUI_DECLARE_EXTERN_SYMBOL(NAME, RETURN_TYPE) \
    extern RETURN_TYPE NAME(); \
    Q_GHS_KEEP_REFERENCE(NAME)

#define BOBUI_DECLARE_EXTERN_SYMBOL_INT(NAME) \
    BOBUI_DECLARE_EXTERN_SYMBOL(NAME, int)

#define BOBUI_DECLARE_EXTERN_SYMBOL_VOID(NAME) \
    BOBUI_DECLARE_EXTERN_SYMBOL(NAME, void)

#define BOBUI_KEEP_SYMBOL_VAR_NAME(NAME) NAME ## _keep

#define BOBUI_KEEP_SYMBOL_HELPER(NAME, VAR_NAME) \
    volatile auto VAR_NAME = &NAME; \
    Q_UNUSED(VAR_NAME)

#define BOBUI_KEEP_SYMBOL(NAME) \
    BOBUI_KEEP_SYMBOL_HELPER(NAME, BOBUI_KEEP_SYMBOL_VAR_NAME(NAME))


// Similar to the ones above, but for rcc resource symbols specifically.
#define BOBUI_GET_RESOURCE_INIT_SYMBOL(NAME) \
    BOBUI_RCC_MANGLE_NAMESPACE(qInitResources_ ## NAME)

#define BOBUI_DECLARE_EXTERN_RESOURCE(NAME) \
    BOBUI_DECLARE_EXTERN_SYMBOL_INT(BOBUI_GET_RESOURCE_INIT_SYMBOL(NAME))

#define BOBUI_KEEP_RESOURCE(NAME) \
    BOBUI_KEEP_SYMBOL(BOBUI_GET_RESOURCE_INIT_SYMBOL(NAME))

#endif // BOBUISYMBOLMACROS_H

