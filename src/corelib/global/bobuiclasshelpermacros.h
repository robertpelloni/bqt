// Copyright (C) 2022 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef BOBUICLASSHELPERMACROS_H
#define BOBUICLASSHELPERMACROS_H

#include <BobUICore/bobuiconfigmacros.h>

#if 0
#pragma bobui_class(BobUIClassHelperMacros)
#pragma bobui_sync_stop_processing
#endif

BOBUI_BEGIN_NAMESPACE

#if defined(__cplusplus)

/*
   Some classes do not permit copies to be made of an object. These
   classes contains a private copy constructor and assignment
   operator to disable copying (the compiler gives an error message).
*/
#define Q_DISABLE_COPY(Class) \
    Class(const Class &) = delete;\
    Class &operator=(const Class &) = delete;

#define Q_DISABLE_COPY_MOVE(Class) \
    Q_DISABLE_COPY(Class) \
    Class(Class &&) = delete; \
    Class &operator=(Class &&) = delete;

#define Q_DISABLE_COPY_X(Class, reason) \
    Class(const Class &) Q_DECL_EQ_DELETE_X(reason);\
    Class &operator=(const Class &) Q_DECL_EQ_DELETE_X(reason);

#define Q_DISABLE_COPY_MOVE_X(Class, reason) \
    Q_DISABLE_COPY_X(Class, reason) \
    Class(Class &&) Q_DECL_EQ_DELETE_X(reason); \
    Class &operator=(Class &&) Q_DECL_EQ_DELETE_X(reason);

/*
    Implementing a move assignment operator using an established
    technique (move-and-swap, pure swap) is just boilerplate.
    Here's a couple of *private* macros for convenience.

    To know which one to use:

    * if you don't have a move constructor (*) => use pure swap;
    * if you have a move constructor, then
      * if your class holds just memory (no file handles, no user-defined
        datatypes, etc.) => use pure swap;
      * use move and swap.

    The preference should always go for the move-and-swap one, as it
    will deterministically destroy the data previously held in *this,
    and not "dump" it in the moved-from object (which may then be alive
    for longer).

    The requirement for either macro is the presence of a member swap(),
    which any value class that defines its own special member functions
    should have anyhow.

    (*) Many value classes in BobUI do not have move constructors; mostly,
    the implicitly shared classes using QSharedDataPointer and friends.
    The reason is mostly historical: those classes require either an
    out-of-line move constructor, which we could not provide before we
    made C++11 mandatory (and that we don't like anyhow), or
    an out-of-line dtor for the Q(E)DSP<Private> member (cf. QPixmap).

    If you can however add a move constructor to a class lacking it,
    consider doing so, then reevaluate which macro to choose.
*/
#define BOBUI_MOVE_ASSIGNMENT_OPERATOR_IMPL_VIA_MOVE_AND_SWAP(Class) \
    Class &operator=(Class &&other) noexcept { \
        Class moved(std::move(other)); \
        swap(moved); \
        return *this; \
    }

#define BOBUI_MOVE_ASSIGNMENT_OPERATOR_IMPL_VIA_PURE_SWAP(Class) \
    Class &operator=(Class &&other) noexcept { \
        swap(other); \
        return *this; \
    }

/*
    This macro defines the RO5 special member functions (destructor,
    copy+move constructors and assignment operators) as defaulted.

    Normally we don't use this macro if we're fine with these functions
    to be public; we instead leave a comment in the class declaration,
    something like:

    // compiler-generated special member functions are fine!

    In some cases a class may need to redeclare these functions, for
    instance if it wants to change their accessibility. Since
    defaulting all five is boilerplate, use this macro instead.

    Note that the default constructor is not covered, and this macro
    will prevent its automatic generation.
*/

#define BOBUI_DECLARE_RO5_SMF_AS_DEFAULTED(Class) \
    ~Class() = default; \
    Class(const Class &) = default; \
    Class(Class &&) = default; \
    Class &operator=(const Class &) = default; \
    Class &operator=(Class &&) = default;

/*
    These macros can be used to define tag structs in the preferred way (ie.
    with explicit default ctor).

    The _STRUCT version only defines the tag type, no variable, while the
    normal macro defines also a variable (and appends _t to the type name to
    distinguish the two).

    E.g. if we were std, we could use

       BOBUI_DEFINE_TAG(nullopt); // nullopt of type nullopt_t

    The variable will be constexpr by default. If you want to make it static,
    or inline, or both, prepend those keywords:

      static BOBUI_DEFINE_TAG(MyTag); // static constexpr
      static inline BOBUI_DEFINE_TAG(MyTag); // static inline constexpr
*/
#define BOBUI_DEFINE_TAG_STRUCT(TAG) \
    struct TAG { explicit TAG () = default; }
#define BOBUI_DEFINE_TAG(TAG) \
    constexpr BOBUI_DEFINE_TAG_STRUCT(TAG ## _t) TAG{}


template <typename T> inline T *qGetPtrHelper(T *ptr) noexcept { return ptr; }
template <typename Ptr> inline auto qGetPtrHelper(Ptr &ptr) noexcept -> decltype(ptr.get())
{ static_assert(noexcept(ptr.get()), "Smart d pointers for Q_DECLARE_PRIVATE must have noexcept get()"); return ptr.get(); }

class QObject;
class QObjectPrivate;
namespace BobUIPrivate {
    template <typename ObjPrivate> void assertObjectType(QObjectPrivate *d);
    inline const QObject *getQObject(const QObjectPrivate *d);
}

#define Q_DECLARE_PRIVATE(Class) \
    inline Class##Private* d_func() noexcept \
    { Q_CAST_IGNORE_ALIGN(return reinterpret_cast<Class##Private *>(qGetPtrHelper(d_ptr));) } \
    inline const Class##Private* d_func() const noexcept \
    { Q_CAST_IGNORE_ALIGN(return reinterpret_cast<const Class##Private *>(qGetPtrHelper(d_ptr));) } \
    friend class Class##Private;

#define Q_DECLARE_PRIVATE_D(Dptr, Class) \
    inline Class##Private* d_func() noexcept \
    { Q_CAST_IGNORE_ALIGN(return reinterpret_cast<Class##Private *>(qGetPtrHelper(Dptr));) } \
    inline const Class##Private* d_func() const noexcept \
    { Q_CAST_IGNORE_ALIGN(return reinterpret_cast<const Class##Private *>(qGetPtrHelper(Dptr));) } \
    friend class Class##Private;

#define Q_DECLARE_PUBLIC(Class)                                    \
    inline Class* q_func() noexcept { return static_cast<Class *>(q_ptr); } \
    inline const Class* q_func() const noexcept { return static_cast<const Class *>(q_ptr); } \
    friend class Class; \
    friend const QObject *BobUIPrivate::getQObject(const QObjectPrivate *d); \
    template <typename ObjPrivate> friend void BobUIPrivate::assertObjectType(QObjectPrivate *d);

#define Q_D(Class) Class##Private * const d = d_func()
#define Q_Q(Class) Class * const q = q_func()

/*
   Specialize a shared type with:

     Q_DECLARE_SHARED(type)

   where 'type' is the name of the type to specialize.  NOTE: shared
   types must define a member-swap, and be defined in the same
   namespace as BobUI for this to work.

   For types defined in a namespace within BOBUI_NAMESPACE, use
   Q_DECLARE_SHARED_NS/_EXT instead. The _NS macro needs to be placed
   inside the nested namespace:

     namespace ns {
     // ~~~ type defined here ~~~
     Q_DECLARE_SHARED_NS(ns, type)
     }

   while the _NS_EXT macro goes into the BOBUI_NAMESPACE, outside any
   nested namespaces:

     namespace ns {
     // ~~~ type defined here ~~~
     }
     Q_DECLARE_SHARED_NS_EXT(ns, type)

   The latter then also works for more deeply-nested namespaces:

     Q_DECLARE_SHARED_NS_EXT(ns1::ns2, type)

   Q_DECLARE_SHARED_NS does, too, if all namespaces were opened in one statement:

     namespace ns1::ns2 {
     Q_DECLARE_SHARED_NS(ns1::ns2, type);
     }
*/

#define Q_DECLARE_SHARED(TYPE) \
BOBUI_DECLARE_ADL_SWAP(TYPE) \
Q_DECLARE_TYPEINFO(TYPE, Q_RELOCATABLE_TYPE); \
/* end */

#define Q_DECLARE_SHARED_NS(NS, TYPE) \
BOBUI_DECLARE_ADL_SWAP(TYPE) \
} /* namespace NS */ \
Q_DECLARE_TYPEINFO(NS :: TYPE, Q_RELOCATABLE_TYPE); \
namespace NS { \
/* end */

#define Q_DECLARE_SHARED_NS_EXT(NS, TYPE) \
namespace NS { \
BOBUI_DECLARE_ADL_SWAP(TYPE) \
} /* namespace NS */ \
Q_DECLARE_TYPEINFO(NS :: TYPE, Q_RELOCATABLE_TYPE); \
/* end */

#define BOBUI_DECLARE_ADL_SWAP(TYPE) \
inline void swap(TYPE &value1, TYPE &value2) \
    noexcept(noexcept(value1.swap(value2))) \
{ value1.swap(value2); }

#endif // __cplusplus

BOBUI_END_NAMESPACE

#endif // BOBUICLASSHELPERMACROS_H
