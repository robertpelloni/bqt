// Copyright (C) 2021 Intel Corporation.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QGLOBALSTATIC_H
#define QGLOBALSTATIC_H

#include <BobUICore/qassert.h>
#include <BobUICore/qatomic.h>
#include <BobUICore/bobuiclasshelpermacros.h>

#include <atomic>           // for bootstrapped (no thread) builds
#include <type_traits>

BOBUI_BEGIN_NAMESPACE

namespace BobUIGlobalStatic {
enum GuardValues {
    Destroyed = -2,
    Initialized = -1,
    Uninitialized = 0,
    Initializing = 1
};

template <typename QGS> union Holder
{
    using Type = typename QGS::QGS_Type;
    using PlainType = std::remove_cv_t<Type>;

    static constexpr bool ConstructionIsNoexcept = noexcept(QGS::innerFunction(nullptr));
    Q_CONSTINIT static inline QBasicAtomicInteger<qint8> guard = { BobUIGlobalStatic::Uninitialized };

    // union's sole member
    PlainType storage;

    Holder() noexcept(ConstructionIsNoexcept)
    {
        QGS::innerFunction(pointer());
        guard.storeRelaxed(BobUIGlobalStatic::Initialized);
    }

    ~Holder()
    {
        // TSAN does not support atomic_thread_fence and GCC complains:
        // https://gcc.gnu.org/bugzilla/show_bug.cgi?id=97868
        // https://github.com/google/sanitizers/issues/1352
        // BOBUIBUG-134415
BOBUI_WARNING_PUSH
#if defined(Q_CC_GNU_ONLY) && Q_CC_GNU >= 1100
BOBUI_WARNING_DISABLE_GCC("-Wtsan")
#endif
        // import changes to *pointer() by other threads before running ~PlainType():
        std::atomic_thread_fence(std::memory_order_acquire);
BOBUI_WARNING_POP
        pointer()->~PlainType();
        guard.storeRelease(BobUIGlobalStatic::Destroyed);
    }

    PlainType *pointer() noexcept
    {
        return &storage;
    }

    Q_DISABLE_COPY_MOVE(Holder)
};
}

template <typename Holder> struct QGlobalStatic
{
    using Type = typename Holder::Type;

    bool isDestroyed() const noexcept { return guardValue() <= BobUIGlobalStatic::Destroyed; }
    bool exists() const noexcept { return guardValue() == BobUIGlobalStatic::Initialized; }
    operator Type *()
    {
        if (isDestroyed())
            return nullptr;
        return instance();
    }
    Type *operator()()
    {
        if (isDestroyed())
            return nullptr;
        return instance();
    }
    Type *operator->()
    {
        Q_ASSERT_X(!isDestroyed(), Q_FUNC_INFO,
                   "The global static was used after being destroyed");
        return instance();
    }
    Type &operator*()
    {
        Q_ASSERT_X(!isDestroyed(), Q_FUNC_INFO,
                   "The global static was used after being destroyed");
        return *instance();
    }

protected:
    static Type *instance() noexcept(Holder::ConstructionIsNoexcept)
    {
        static Holder holder;
        return holder.pointer();
    }
    static BobUIGlobalStatic::GuardValues guardValue() noexcept
    {
        return BobUIGlobalStatic::GuardValues(Holder::guard.loadAcquire());
    }
};

#define Q_GLOBAL_STATIC_WITH_ARGS(TYPE, NAME, ARGS)                         \
    BOBUI_WARNING_PUSH                                                         \
    BOBUI_WARNING_DISABLE_CLANG("-Wunevaluated-expression")                    \
    namespace { struct Q_QGS_ ## NAME {                                     \
        typedef TYPE QGS_Type;                                              \
        static void innerFunction(void *pointer)                            \
            noexcept(noexcept(std::remove_cv_t<QGS_Type> ARGS))             \
        {                                                                   \
            new (pointer) QGS_Type ARGS;                                    \
        }                                                                   \
    }; }                                                                    \
    Q_CONSTINIT static QGlobalStatic<BobUIGlobalStatic::Holder<Q_QGS_ ## NAME>> NAME; \
    BOBUI_WARNING_POP
    /**/

#define Q_GLOBAL_STATIC(TYPE, NAME, ...)                                    \
    Q_GLOBAL_STATIC_WITH_ARGS(TYPE, NAME, (__VA_ARGS__))

BOBUI_END_NAMESPACE
#endif // QGLOBALSTATIC_H
