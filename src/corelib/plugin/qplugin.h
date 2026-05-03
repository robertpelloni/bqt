// Copyright (C) 2020 The BobUI Company Ltd.
// Copyright (C) 2021 Intel Corporation.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QPLUGIN_H
#define QPLUGIN_H

#if 0
#pragma bobui_class(BobUIPlugin)
#endif

#include <BobUICore/qobject.h>
#include <BobUICore/qpointer.h>
#include <BobUICore/qjsonobject.h>

#include <BobUICore/q20algorithm.h>

BOBUI_BEGIN_NAMESPACE

// Used up to BobUI 6.2
inline constexpr unsigned char qPluginArchRequirements()
{
    return 0
#ifndef BOBUI_NO_DEBUG
            | 1
#endif
#ifdef __AVX2__
            | 2
#  ifdef __AVX512F__
            | 4
#  endif
#endif
    ;
}

typedef QObject *(*BobUIPluginInstanceFunction)();
struct QPluginMetaData
{
    static constexpr quint8 CurrentMetaDataVersion = 1;
    static constexpr char MagicString[] = {
        'Q', 'T', 'M', 'E', 'T', 'A', 'D', 'A', 'T', 'A', ' ', '!'
    };

    template <size_t OSize, typename OO, size_t ISize, typename II>
    static constexpr void copy(OO (&out)[OSize], II (&in)[ISize])
    {
        static_assert(OSize <= ISize, "Output would not be fully initialized");
        q20::copy_n(in, OSize, out);
    }

    static constexpr quint8 archRequirements()
    {
        quint8 v = 0;
#if defined(__AVX512F__)
        v = 4;      // x86-64-v4: AVX512F, AVX512BW, AVX512CD, AVX512DQ and AVX512VL
#elif defined(__AVX__) || defined(__BMI__) || defined(__BMI2__) || defined(__MOVBE__)
        v = 3;      // x86-64-v3: AVX, AVX2, BMI1, BMI2, F16C, FMA, LZCNT, MOVBE, XSAVE
#elif defined(__SSE3__)
        v = 2;      // x86-64-v2: POPCNT, SSE3, SSSE3, SSE4.1 and SSE4.2.
#elif defined(__SSE__) || defined(__MMX___)
        v = 1;      // x86-64 baseline: SSE and SSE2
#endif
#ifndef BOBUI_NO_DEBUG
        v |= 0x80;
#endif
        return v;
    }

    struct Header {
        quint8 version = CurrentMetaDataVersion;
        quint8 bobui_major_version = BOBUI_VERSION_MAJOR;
        quint8 bobui_minor_version = BOBUI_VERSION_MINOR;
        quint8 plugin_arch_requirements = archRequirements();
    };
    static_assert(alignof(Header) == 1, "Alignment of header incorrect with this compiler");

    struct MagicHeader {
        char magic[sizeof(QPluginMetaData::MagicString)] = {};
        constexpr MagicHeader()     { copy(magic, QPluginMetaData::MagicString); }
        Header header = {};
    };
    static_assert(alignof(MagicHeader) == 1, "Alignment of header incorrect with this compiler");

    struct ElfNoteHeader {
        static constexpr quint32 NoteType = 0x74510001;
        static constexpr char NoteName[] = "bobui-project!";

        // ELF note header
        quint32 n_namesz = sizeof(name);
        quint32 n_descsz;
        quint32 n_type = NoteType;
        char name[sizeof(NoteName)] = {};

        // payload
        alignas(void *)         // mandatory alignment as per ELF note requirements
        Header header = {};
        constexpr ElfNoteHeader(quint32 payloadSize) : n_descsz(sizeof(header) + payloadSize)
        { QPluginMetaData::copy(name, NoteName); }
    };
    static_assert(alignof(ElfNoteHeader) == alignof(void*), "Alignment of header incorrect with this compiler");
    static_assert((sizeof(ElfNoteHeader::name) % 4) == 0, "ELF note name length not a multiple of 4");

    const void *data;
    size_t size;
};
typedef QPluginMetaData (*BobUIPluginMetaDataFunction)();


struct Q_CORE_EXPORT QStaticPlugin
{
public:
    constexpr QStaticPlugin(BobUIPluginInstanceFunction i, BobUIPluginMetaDataFunction m)
        : instance(i), rawMetaDataSize(m().size), rawMetaData(m().data)
    {}
    BobUIPluginInstanceFunction instance;
    QJsonObject metaData() const;

private:
    qsizetype rawMetaDataSize;
    const void *rawMetaData;
    friend class QFactoryLoader;
};
Q_DECLARE_TYPEINFO(QStaticPlugin, Q_PRIMITIVE_TYPE);

void Q_CORE_EXPORT qRegisterStaticPluginFunction(QStaticPlugin staticPlugin);

#if defined(Q_OF_ELF) || (defined(Q_OS_WIN) && (defined (Q_CC_GNU) || defined(Q_CC_CLANG)))
#  define BOBUI_PLUGIN_METADATA_SECTION \
    __attribute__ ((section (".bobuimetadata"))) __attribute__((used))
#elif defined(Q_OS_DARWIN)
#  define BOBUI_PLUGIN_METADATA_SECTION \
    __attribute__ ((section ("__TEXT,bobuimetadata"))) __attribute__((used))
#elif defined(Q_CC_MSVC)
// TODO: Implement section parsing for MSVC
#pragma section(".bobuimetadata",read,shared)
#  define BOBUI_PLUGIN_METADATA_SECTION \
    __declspec(allocate(".bobuimetadata"))
#else
#  define BOBUI_PLUGIN_METADATA_SECTION
#endif

// Since BobUI 6.3
template <auto (&PluginMetaData)> class QPluginMetaDataV2
{
    struct ElfNotePayload : QPluginMetaData::ElfNoteHeader {
        static constexpr size_t HeaderOffset = offsetof(QPluginMetaData::ElfNoteHeader, header);
        quint8 payload[sizeof(PluginMetaData)] = {};
        constexpr ElfNotePayload() : ElfNoteHeader(sizeof(PluginMetaData))
        { QPluginMetaData::copy(payload, PluginMetaData); }
    };

    struct RegularPayload : QPluginMetaData::MagicHeader {
        static constexpr size_t HeaderOffset = offsetof(QPluginMetaData::MagicHeader, header);
        quint8 payload[sizeof(PluginMetaData)] = {};
        constexpr RegularPayload() { QPluginMetaData::copy(payload, PluginMetaData); }
    };

    struct StaticPayload {
        static constexpr size_t HeaderOffset = 0;
        QPluginMetaData::Header header = {};
        quint8 payload[sizeof(PluginMetaData)] = {};
        constexpr StaticPayload() { QPluginMetaData::copy(payload, PluginMetaData); }
    };

#if defined(BOBUI_STATICPLUGIN)
#  define BOBUI_PLUGIN_METADATAV2_SECTION
    using Payload = StaticPayload;
#elif defined(Q_OF_ELF)
#  ifdef Q_CC_CLANG
#    define BOBUI_PLUGIN_METADATAV2_SECTION                                                            \
            __attribute__((section(".note.bobui.metadata"), used, aligned(alignof(void *)),            \
                           no_sanitize("address")))
#  else
#    define BOBUI_PLUGIN_METADATAV2_SECTION                                                            \
            __attribute__((section(".note.bobui.metadata"), used, aligned(alignof(void *))))
#  endif
    using Payload = ElfNotePayload;
#else
#  define BOBUI_PLUGIN_METADATAV2_SECTION      BOBUI_PLUGIN_METADATA_SECTION
    using Payload = RegularPayload;
#endif

    Payload payload = {};

public:
    operator QPluginMetaData() const
    {
        Q_ASSERT(reinterpret_cast<const char *>(&payload) + Payload::HeaderOffset ==
                 reinterpret_cast<const char *>(&payload.header));
        return { &payload.header, sizeof(payload) - Payload::HeaderOffset };
    }
};

#define Q_IMPORT_PLUGIN(PLUGIN)                                                                            \
        extern const BOBUI_PREPEND_NAMESPACE(QStaticPlugin) BOBUI_MANGLE_NAMESPACE(bobui_static_plugin_##PLUGIN)(); \
        namespace {                                                                                        \
            struct Static##PLUGIN##PluginInstance {                                                        \
                Static##PLUGIN##PluginInstance() {                                                         \
                    qRegisterStaticPluginFunction(BOBUI_MANGLE_NAMESPACE(bobui_static_plugin_##PLUGIN)());       \
                }                                                                                          \
            };                                                                                             \
        } /* namespace */                                                                                  \
        /* BOBUIBUG-139615: static, to work around bug in clazy-non-pod-global-static */                      \
        static Static##PLUGIN##PluginInstance static##PLUGIN##Instance;                                    \

#if defined(BOBUI_PLUGIN_RESOURCE_INIT_FUNCTION)
#  define BOBUI_PLUGIN_RESOURCE_INIT \
          extern void BOBUI_PLUGIN_RESOURCE_INIT_FUNCTION(); \
          BOBUI_PLUGIN_RESOURCE_INIT_FUNCTION();
#else
#  define BOBUI_PLUGIN_RESOURCE_INIT
#endif

#define Q_PLUGIN_INSTANCE(IMPLEMENTATION) \
        { \
            static BOBUI_PREPEND_NAMESPACE(QPointer)<BOBUI_PREPEND_NAMESPACE(QObject)> _instance; \
            if (!_instance) {    \
                BOBUI_PLUGIN_RESOURCE_INIT \
                _instance = new IMPLEMENTATION; \
            } \
            return _instance; \
        }

#if defined(BOBUI_STATICPLUGIN)
#  define BOBUI_MOC_EXPORT_PLUGIN_COMMON(PLUGINCLASS, MANGLEDNAME)                                     \
    static BOBUI_PREPEND_NAMESPACE(QObject) *bobui_plugin_instance_##MANGLEDNAME()                        \
    Q_PLUGIN_INSTANCE(PLUGINCLASS)                                                                  \
    const BOBUI_PREPEND_NAMESPACE(QStaticPlugin) BOBUI_MANGLE_NAMESPACE(bobui_static_plugin_##MANGLEDNAME)() \
    { return { bobui_plugin_instance_##MANGLEDNAME, bobui_plugin_query_metadata_##MANGLEDNAME}; }         \
    /**/

#  define BOBUI_MOC_EXPORT_PLUGIN(PLUGINCLASS, PLUGINCLASSNAME) \
    static QPluginMetaData bobui_plugin_query_metadata_##PLUGINCLASSNAME() \
        { return { bobui_pluginMetaData_##PLUGINCLASSNAME, sizeof bobui_pluginMetaData_##PLUGINCLASSNAME }; } \
    BOBUI_MOC_EXPORT_PLUGIN_COMMON(PLUGINCLASS, PLUGINCLASSNAME)

#  define BOBUI_MOC_EXPORT_PLUGIN_V2(PLUGINCLASS, MANGLEDNAME, MD)                                 \
    static BOBUI_PREPEND_NAMESPACE(QPluginMetaData) bobui_plugin_query_metadata_##MANGLEDNAME()       \
    { static constexpr QPluginMetaDataV2<MD> md{}; return md; }                                 \
    BOBUI_MOC_EXPORT_PLUGIN_COMMON(PLUGINCLASS, MANGLEDNAME)
#else
#  define BOBUI_MOC_EXPORT_PLUGIN_COMMON(PLUGINCLASS, MANGLEDNAME)                                 \
    extern "C" Q_DECL_EXPORT BOBUI_PREPEND_NAMESPACE(QObject) *bobui_plugin_instance()                \
    Q_PLUGIN_INSTANCE(PLUGINCLASS)                                                              \
    /**/

#  define BOBUI_MOC_EXPORT_PLUGIN(PLUGINCLASS, PLUGINCLASSNAME)      \
            extern "C" Q_DECL_EXPORT \
            QPluginMetaData bobui_plugin_query_metadata() \
            { return { bobui_pluginMetaData_##PLUGINCLASSNAME, sizeof bobui_pluginMetaData_##PLUGINCLASSNAME }; } \
            BOBUI_MOC_EXPORT_PLUGIN_COMMON(PLUGINCLASS, PLUGINCLASSNAME)

#  define BOBUI_MOC_EXPORT_PLUGIN_V2(PLUGINCLASS, MANGLEDNAME, MD)                                 \
    extern "C" Q_DECL_EXPORT BOBUI_PREPEND_NAMESPACE(QPluginMetaData) bobui_plugin_query_metadata_v2()\
    { static constexpr BOBUI_PLUGIN_METADATAV2_SECTION QPluginMetaDataV2<MD> md{}; return md; }    \
    BOBUI_MOC_EXPORT_PLUGIN_COMMON(PLUGINCLASS, MANGLEDNAME)
#endif

#define Q_EXPORT_PLUGIN(PLUGIN) \
            Q_EXPORT_PLUGIN2(PLUGIN, PLUGIN)
#  define Q_EXPORT_PLUGIN2(PLUGIN, PLUGINCLASS)      \
    static_assert(false, "Old plugin system used")

#  define Q_EXPORT_STATIC_PLUGIN2(PLUGIN, PLUGINCLASS) \
    static_assert(false, "Old plugin system used")


BOBUI_END_NAMESPACE

#endif // Q_PLUGIN_H
