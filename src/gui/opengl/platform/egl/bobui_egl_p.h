// Copyright (C) 2020 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef BOBUI_EGL_P_H
#define BOBUI_EGL_P_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the BobUI API. It exists purely as an
// implementation detail. This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//

// q(data/text)stream.h must be included before any header file that defines Status
#include <BobUICore/qdatastream.h>
#include <BobUICore/bobuiextstream.h>
#include <BobUICore/private/qglobal_p.h>

#ifdef BOBUI_EGL_NO_X11
# ifndef EGL_NO_X11
#  define EGL_NO_X11
# endif
# ifndef MESA_EGL_NO_X11_HEADERS
#  define MESA_EGL_NO_X11_HEADERS // MESA
# endif
# if !defined(Q_OS_INTEGRITY)
#  define WIN_INTERFACE_CUSTOM   // NV
# endif // Q_OS_INTEGRITY
#else // BOBUI_EGL_NO_X11
// If one has an eglplatform.h with https://github.com/KhronosGroup/EGL-Registry/pull/130
// that needs USE_X11 to be defined.
# define USE_X11
#endif

#ifdef BOBUI_EGL_WAYLAND
# define WAYLAND // NV
#endif // BOBUI_EGL_WAYLAND

#include <EGL/egl.h>
#include <EGL/eglext.h>

#ifndef BOBUI_EGL_NO_X11
#undef Status
#undef None
#undef Bool
#undef CursorShape
#undef KeyPress
#undef KeyRelease
#undef FocusIn
#undef FocusOut
#undef FontChange
#undef Expose
#undef Unsorted
#endif

#include <stdint.h>

BOBUI_BEGIN_NAMESPACE

namespace BobUIInternal {

template <class FromType, class ToType>
struct BobUIEglConverter
{
    static inline ToType convert(FromType v)
    { return v; }
};

template <>
struct BobUIEglConverter<uint32_t, uintptr_t>
{
    static inline uintptr_t convert(uint32_t v)
    { return v; }
};

#if BOBUI_POINTER_SIZE > 4
template <>
struct BobUIEglConverter<uintptr_t, uint32_t>
{
    static inline uint32_t convert(uintptr_t v)
    { return uint32_t(v); }
};
#endif

template <>
struct BobUIEglConverter<uint32_t, void *>
{
    static inline void *convert(uint32_t v)
    { return reinterpret_cast<void *>(uintptr_t(v)); }
};

template <>
struct BobUIEglConverter<void *, uint32_t>
{
    static inline uint32_t convert(void *v)
    { return uintptr_t(v); }
};

} // BobUIInternal

template <class ToType, class FromType>
static inline ToType bobui_egl_cast(FromType from)
{ return BobUIInternal::BobUIEglConverter<FromType, ToType>::convert(from); }

BOBUI_END_NAMESPACE

#endif // BOBUI_EGL_P_H
