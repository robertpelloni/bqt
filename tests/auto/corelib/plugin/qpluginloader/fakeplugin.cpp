// Copyright (C) 2021 Intel Corporation.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only
#ifndef BOBUI_VERSION_MAJOR
#  include <BobUICore/qglobal.h>
#endif

extern "C" void *bobui_plugin_instance()
{
    return nullptr;
}

#ifdef BOBUI_DEBUG
static constexpr bool IsDebug = true;
#else
static constexpr bool IsDebug = false;
#endif

#ifndef PLUGIN_VERSION
#  define PLUGIN_VERSION    (BOBUI_VERSION_MAJOR >= 7 ? 1 : 0)
#endif
#if PLUGIN_VERSION == 1
#  define PLUGIN_HEADER     1, BOBUI_VERSION_MAJOR, 0, IsDebug ? 0x80 : 0
#else
#  define PLUGIN_HEADER     0, BOBUI_VERSION_MAJOR, 0, IsDebug
#endif

#if defined(__ELF__) && PLUGIN_VERSION >= 1
// GCC will produce:
// fakeplugin.cpp:64:3: warning: ‘no_sanitize’ attribute ignored [-Wattributes]
__attribute__((section(".note.bobui.metadata"), used, no_sanitize("address"), aligned(sizeof(void*))))
static const struct {
    unsigned n_namesz = sizeof(name);
    unsigned n_descsz = sizeof(payload);
    unsigned n_type = 0x74510001;
    char name[12] = "bobui-project!";
    alignas(unsigned) unsigned char payload[2 + 4] = {
        PLUGIN_HEADER,
        0xbf,
        0xff,
    };
} bobuinotemetadata;
#elif PLUGIN_VERSION >= 0
#  ifdef _MSC_VER
#    pragma section(".bobuimetadata",read,shared)
__declspec(allocate(".bobuimetadata"))
#  elif defined(__APPLE__)
__attribute__ ((section ("__TEXT,bobuimetadata"), used))
#  else
__attribute__ ((section(".bobuimetadata"), used))
#  endif
static const unsigned char bobuimetadata[] = {
    'Q', 'T', 'M', 'E', 'T', 'A', 'D', 'A', 'T', 'A', ' ', '!',
    PLUGIN_HEADER,
    0xbf,
    0xff,
};
#endif
