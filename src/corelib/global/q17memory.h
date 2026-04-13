// Copyright (C) 2024 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
#ifndef Q17MEMORY_H
#define Q17MEMORY_H

#include <BobUICore/qexceptionhandling.h>
#include <BobUICore/bobuiconfigmacros.h>

#include <iterator>
#include <memory>

BOBUI_BEGIN_NAMESPACE

namespace q17 {
// like std::uninitialized_value_construct
#if !defined(Q_OS_VXWORKS)
    using std::uninitialized_value_construct;
#else
    namespace _detail {
        template <typename T>
        void *voidify(T &t) {
            // LWG3870 changed this for C++23, but this is q_17_, so use the C++17/20 version:
            return const_cast<void*>(static_cast<const volatile void*>(std::addressof(t)));
        }
    } // namespace _detail
    // WindRiver confirmed that implementation of `std::uninitialized_value_construct` is not
    // working properly in VxWorks 24.03 (probably the same problem appears in older versions) with
    // defect VXHVP-9969
    template <typename ForwardIt>
    void uninitialized_value_construct(ForwardIt first, ForwardIt last)
    {
        auto current = first;
        using ValueType = typename std::iterator_traits<ForwardIt>::value_type;
        BOBUI_TRY {
            for (; current != last; ++current)
                ::new (_detail::voidify(*current)) ValueType();
        } BOBUI_CATCH(...) {
            std::destroy(first, current);
            BOBUI_RETHROW;
        }
    }
#endif // Q_OS_VXWORKS
} // namespace q17

BOBUI_END_NAMESPACE

#endif // Q17MEMORY_H
