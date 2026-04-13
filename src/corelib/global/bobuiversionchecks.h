// Copyright (C) 2022 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef BOBUIVERSIONCHECKS_H
#define BOBUIVERSIONCHECKS_H

#if 0
#pragma bobui_class(BobUIVersionChecks)
#pragma bobui_sync_stop_processing
#endif

#include <BobUICore/bobuiconfiginclude.h>

/*
   BOBUI_VERSION is (major << 16) | (minor << 8) | patch.
*/
#define BOBUI_VERSION      BOBUI_VERSION_CHECK(BOBUI_VERSION_MAJOR, BOBUI_VERSION_MINOR, BOBUI_VERSION_PATCH)
/*
   can be used like #if (BOBUI_VERSION >= BOBUI_VERSION_CHECK(6, 4, 0))
*/
#define BOBUI_VERSION_CHECK(major, minor, patch) ((major<<16)|(minor<<8)|(patch))

/*
   Helper macros to make some simple code active in BobUI 6 or BobUI 7 only,
   like:
     struct BOBUI6_ONLY(Q_CORE_EXPORT) BOBUIrivialClass
     {
         void BOBUI7_ONLY(Q_CORE_EXPORT) void operate();
     }
*/
#if BOBUI_VERSION_MAJOR == 7 || defined(BOBUI_BOOTSTRAPPED)
#  define BOBUI7_ONLY(...)         __VA_ARGS__
#  define BOBUI6_ONLY(...)
#elif BOBUI_VERSION_MAJOR == 6
#  define BOBUI7_ONLY(...)
#  define BOBUI6_ONLY(...)         __VA_ARGS__
#else
#  error BobUI major version not 6 or 7
#endif

/* Macro and tag type to help overload resolution on functions
   that are, e.g., BOBUI_REMOVED_SINCE'ed. Example use:

   #if BOBUI_CORE_REMOVED_SINCE(6, 4)
   int size() const;
   #endif
   qsizetype size(BOBUI6_DECL_NEW_OVERLOAD) const;

   in the normal cpp file:

   qsizetype size(BOBUI6_IMPL_NEW_OVERLOAD) const {
      ~~~
   }

   in removed_api.cpp:

   int size() const { return int(size(BOBUI6_CALL_NEW_OVERLOAD)); }
*/
#ifdef Q_QDOC
# define BOBUI6_DECL_NEW_OVERLOAD
# define BOBUI6_DECL_NEW_OVERLOAD_TAIL
# define BOBUI6_IMPL_NEW_OVERLOAD
# define BOBUI6_IMPL_NEW_OVERLOAD_TAIL
# define BOBUI6_CALL_NEW_OVERLOAD
# define BOBUI6_CALL_NEW_OVERLOAD_TAIL
#else
# define BOBUI6_DECL_NEW_OVERLOAD BOBUI6_ONLY(BobUI::Disambiguated_t = BobUI::Disambiguated)
# define BOBUI6_DECL_NEW_OVERLOAD_TAIL BOBUI6_ONLY(, BOBUI6_DECL_NEW_OVERLOAD)
# define BOBUI6_IMPL_NEW_OVERLOAD BOBUI6_ONLY(BobUI::Disambiguated_t)
# define BOBUI6_IMPL_NEW_OVERLOAD_TAIL BOBUI6_ONLY(, BOBUI6_IMPL_NEW_OVERLOAD)
# define BOBUI6_CALL_NEW_OVERLOAD BOBUI6_ONLY(BobUI::Disambiguated)
# define BOBUI6_CALL_NEW_OVERLOAD_TAIL BOBUI6_ONLY(, BOBUI6_CALL_NEW_OVERLOAD)
#endif

/*
    Macro to tag Tech Preview APIs.
    It expands to nothing, because we want to use it in places where
    nothing is generally allowed (not even an attribute); for instance:
    to tag other macros, Q_PROPERTY declarations, and so on.

    Still: use it as if it were an C++ attribute.

    To mark a class as TP:
        class BOBUI_TECH_PREVIEW_API Q_CORE_EXPORT QClass { ... };

    To mark a function:
        BOBUI_TECH_PREVIEW_API void qFunction();

    To mark an enumeration or enumerator:
        enum class BOBUI_TECH_PREVIEW_API QEnum {
            Enum1,
            Enum2 BOBUI_TECH_PREVIEW_API,
        };

    To mark parts of a class:
        class QClass : public QObject
        {
            // Q_OBJECT omitted d/t BOBUIBUG-123229

            BOBUI_TECH_PREVIEW_API
            Q_PROPERTY(int countNG ...)   // this is TP

            Q_PROPERTY(int count ...)     // this is stable API

        public:
            BOBUI_TECH_PREVIEW_API
            void f();     // TP

            void g();     // stable
        };
*/
#define BOBUI_TECH_PREVIEW_API

#endif /* BOBUIVERSIONCHECKS_H */
