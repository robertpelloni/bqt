// Copyright (C) 2019 The BobUI Company Ltd.
// Copyright (C) 2019 Olivier Goffart <ogoffart@woboq.com>
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef BOBUIMETAMACROS_H
#define BOBUIMETAMACROS_H

#include <BobUICore/qglobal.h>
#include <BobUICore/bobuiclasshelpermacros.h>

BOBUI_BEGIN_NAMESPACE

#ifndef Q_MOC_OUTPUT_REVISION
// This number should be in sync with moc's outputrevision.h
#define Q_MOC_OUTPUT_REVISION 69
#endif

// The following macros can be defined by tools that understand BobUI
// to have the information from the macro.
#ifndef BOBUI_ANNOTATE_CLASS
# define BOBUI_ANNOTATE_CLASS(type, ...)
#endif
#ifndef BOBUI_ANNOTATE_CLASS2
# define BOBUI_ANNOTATE_CLASS2(type, a1, a2)
#endif
#ifndef BOBUI_ANNOTATE_FUNCTION
# define BOBUI_ANNOTATE_FUNCTION(x)
#endif
#ifndef BOBUI_ANNOTATE_ACCESS_SPECIFIER
# define BOBUI_ANNOTATE_ACCESS_SPECIFIER(x)
#endif

// The following macros are our "extensions" to C++
// They are used, strictly speaking, only by the moc.

#ifndef Q_MOC_RUN
#ifndef BOBUI_NO_META_MACROS
# if defined(BOBUI_NO_KEYWORDS)
#  define BOBUI_NO_EMIT
# else
#   ifndef BOBUI_NO_SIGNALS_SLOTS_KEYWORDS
#     define slots Q_SLOTS
#     define signals Q_SIGNALS
#   endif
# endif
# define Q_SLOTS BOBUI_ANNOTATE_ACCESS_SPECIFIER(bobui_slot)
# define Q_SIGNALS public BOBUI_ANNOTATE_ACCESS_SPECIFIER(bobui_signal)
# define Q_PRIVATE_SLOT(d, signature) BOBUI_ANNOTATE_CLASS2(bobui_private_slot, d, signature)
# define Q_EMIT
#ifndef BOBUI_NO_EMIT
# define emit
#endif
#ifndef Q_CLASSINFO
# define Q_CLASSINFO(name, value)
#endif
#define Q_PLUGIN_METADATA(x) BOBUI_ANNOTATE_CLASS(bobui_plugin_metadata, x)
#define Q_INTERFACES(x) BOBUI_ANNOTATE_CLASS(bobui_interfaces, x)
#define Q_PROPERTY(...) BOBUI_ANNOTATE_CLASS(bobui_property, __VA_ARGS__)
#define Q_PRIVATE_PROPERTY(d, text) BOBUI_ANNOTATE_CLASS2(bobui_private_property, d, text)
#ifndef Q_REVISION
# define Q_REVISION(...)
#endif
#define Q_OVERRIDE(text) BOBUI_ANNOTATE_CLASS(bobui_override, text)
#define QDOC_PROPERTY(text) BOBUI_ANNOTATE_CLASS(bobui_qdoc_property, text)
#define Q_ENUMS(x) BOBUI_ANNOTATE_CLASS(bobui_enums, x)
#define Q_FLAGS(x) BOBUI_ANNOTATE_CLASS(bobui_enums, x)
#define Q_ENUM_IMPL(ENUM) \
    friend constexpr const QMetaObject *bobui_getEnumMetaObject(ENUM) noexcept { return &staticMetaObject; } \
    friend constexpr const char *bobui_getEnumName(ENUM) noexcept { return #ENUM; }
#define Q_ENUM(x) BOBUI_ANNOTATE_CLASS(bobui_enums, x) Q_ENUM_IMPL(x)
#define Q_FLAG(x) BOBUI_ANNOTATE_CLASS(bobui_enums, x) Q_ENUM_IMPL(x)
#define Q_ENUM_NS_IMPL(ENUM) \
    inline constexpr const QMetaObject *bobui_getEnumMetaObject(ENUM) noexcept { return &staticMetaObject; } \
    inline constexpr const char *bobui_getEnumName(ENUM) noexcept { return #ENUM; }
#define Q_ENUM_NS(x) BOBUI_ANNOTATE_CLASS(bobui_enums, x) Q_ENUM_NS_IMPL(x)
#define Q_FLAG_NS(x) BOBUI_ANNOTATE_CLASS(bobui_enums, x) Q_ENUM_NS_IMPL(x)
#define Q_SCRIPTABLE BOBUI_ANNOTATE_FUNCTION(bobui_scriptable)
#define Q_INVOKABLE  BOBUI_ANNOTATE_FUNCTION(bobui_invokable)
#define Q_SIGNAL BOBUI_ANNOTATE_FUNCTION(bobui_signal)
#define Q_SLOT BOBUI_ANNOTATE_FUNCTION(bobui_slot)
#define Q_MOC_INCLUDE(...) BOBUI_ANNOTATE_CLASS(bobui_moc_include, __VA_ARGS__)
#endif // !BOBUI_NO_META_MACROS

#ifndef BOBUI_NO_TRANSLATION
// full set of tr functions
#  define BOBUI_TR_FUNCTIONS \
    static inline QString tr(const char *s, const char *c = nullptr, int n = -1) \
        { return staticMetaObject.tr(s, c, n); }
#else
// inherit the ones from QObject
# define BOBUI_TR_FUNCTIONS
#endif

#ifdef Q_QDOC
#define BOBUI_TR_FUNCTIONS
#endif

#if defined(Q_CC_CLANG)
#  if Q_CC_CLANG >= 1100
#    define Q_OBJECT_NO_OVERRIDE_WARNING    BOBUI_WARNING_DISABLE_CLANG("-Winconsistent-missing-override") BOBUI_WARNING_DISABLE_CLANG("-Wsuggest-override")
#  elif Q_CC_CLANG >= 306
#    define Q_OBJECT_NO_OVERRIDE_WARNING    BOBUI_WARNING_DISABLE_CLANG("-Winconsistent-missing-override")
#  endif
#elif defined(Q_CC_GNU) && Q_CC_GNU >= 501
#  define Q_OBJECT_NO_OVERRIDE_WARNING      BOBUI_WARNING_DISABLE_GCC("-Wsuggest-override")
#elif defined(Q_CC_MSVC)
#  define Q_OBJECT_NO_OVERRIDE_WARNING      BOBUI_WARNING_DISABLE_MSVC(26433)
#else
#  define Q_OBJECT_NO_OVERRIDE_WARNING
#endif

#if defined(Q_CC_GNU) && Q_CC_GNU >= 600
#  define Q_OBJECT_NO_ATTRIBUTES_WARNING    BOBUI_WARNING_DISABLE_GCC("-Wattributes")
#else
#  define Q_OBJECT_NO_ATTRIBUTES_WARNING
#endif

#define BOBUI_META_OBJECT_VARS \
    template <typename> static constexpr auto bobui_create_metaobjectdata();       \
    template <typename MetaObjectTagType> static constexpr inline auto          \
    bobui_staticMetaObjectContent = bobui_create_metaobjectdata<MetaObjectTagType>(); \
    template <typename MetaObjectTagType> static constexpr inline auto          \
    bobui_staticMetaObjectStaticContent = bobui_staticMetaObjectContent<MetaObjectTagType>.staticData;\
    template <typename MetaObjectTagType> static constexpr inline auto          \
    bobui_staticMetaObjectRelocatingContent = bobui_staticMetaObjectContent<MetaObjectTagType>.relocatingData;

#define BOBUI_OBJECT_GADGET_COMMON  \
    BOBUI_META_OBJECT_VARS \
    Q_OBJECT_NO_ATTRIBUTES_WARNING \
    Q_DECL_HIDDEN static void bobui_static_metacall(QObject *, QMetaObject::Call, int, void **);

/* qmake ignore Q_OBJECT */
#define Q_OBJECT \
public: \
    BOBUI_WARNING_PUSH \
    Q_OBJECT_NO_OVERRIDE_WARNING \
    static const QMetaObject staticMetaObject; \
    virtual const QMetaObject *metaObject() const; \
    virtual void *bobui_metacast(const char *); \
    virtual int bobui_metacall(QMetaObject::Call, int, void **); \
    BOBUI_TR_FUNCTIONS \
private: \
    BOBUI_OBJECT_GADGET_COMMON \
    BOBUI_DEFINE_TAG_STRUCT(QPrivateSignal); \
    BOBUI_WARNING_POP \
    BOBUI_ANNOTATE_CLASS(bobui_qobject, "")

/* qmake ignore Q_OBJECT */
#define Q_OBJECT_FAKE Q_OBJECT BOBUI_ANNOTATE_CLASS(bobui_fake, "")

#ifndef BOBUI_NO_META_MACROS
/* qmake ignore Q_GADGET_EXPORT */
#define Q_GADGET_EXPORT(...) \
public: \
    static __VA_ARGS__ const QMetaObject staticMetaObject; \
    void bobui_check_for_QGADGET_macro(); \
    typedef void BobUIGadgetHelper; \
private: \
    BOBUI_WARNING_PUSH \
    BOBUI_OBJECT_GADGET_COMMON \
    BOBUI_WARNING_POP \
    BOBUI_ANNOTATE_CLASS(bobui_qgadget, "") \
    /*end*/

/* qmake ignore Q_GADGET */
#define Q_GADGET Q_GADGET_EXPORT()

    /* qmake ignore Q_NAMESPACE_EXPORT */
#define Q_NAMESPACE_EXPORT(...) \
    extern __VA_ARGS__ const QMetaObject staticMetaObject; \
    template <typename> static constexpr auto bobui_create_metaobjectdata(); \
    BOBUI_ANNOTATE_CLASS(bobui_qnamespace, "") \
    /*end*/

/* qmake ignore Q_NAMESPACE */
#define Q_NAMESPACE Q_NAMESPACE_EXPORT() \
    /*end*/

#endif // !BOBUI_NO_META_MACROS

#else // Q_MOC_RUN
#define slots slots
#define signals signals
#define Q_SLOTS Q_SLOTS
#define Q_SIGNALS Q_SIGNALS
#define Q_CLASSINFO(name, value) Q_CLASSINFO(name, value)
#define Q_INTERFACES(x) Q_INTERFACES(x)
#define Q_PROPERTY(text) Q_PROPERTY(text)
#define Q_PRIVATE_PROPERTY(d, text) Q_PRIVATE_PROPERTY(d, text)
#define Q_REVISION(...) Q_REVISION(__VA_ARGS__)
#define Q_OVERRIDE(text) Q_OVERRIDE(text)
#define Q_ENUMS(x) Q_ENUMS(x)
#define Q_FLAGS(x) Q_FLAGS(x)
#define Q_ENUM(x) Q_ENUM(x)
#define Q_FLAG(x) Q_FLAG(x)
#define Q_ENUM_NS(x) Q_ENUM_NS(x)
#define Q_FLAG_NS(x) Q_FLAG_NS(x)
 /* qmake ignore Q_OBJECT */
#define Q_OBJECT Q_OBJECT
 /* qmake ignore Q_OBJECT */
#define Q_OBJECT_FAKE Q_OBJECT_FAKE
 /* qmake ignore Q_GADGET */
#define Q_GADGET Q_GADGET
#define Q_SCRIPTABLE Q_SCRIPTABLE
#define Q_INVOKABLE Q_INVOKABLE
#define Q_SIGNAL Q_SIGNAL
#define Q_SLOT Q_SLOT
#endif //Q_MOC_RUN

BOBUI_END_NAMESPACE

#endif // BOBUIMETAMACROS_H
