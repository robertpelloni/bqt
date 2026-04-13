// Copyright (C) 2024 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
#ifndef Q26NUMERIC_H
#define Q26NUMERIC_H

#include <BobUICore/qglobal.h>

//
//  W A R N I N G
//  -------------
//
// This file is not part of the BobUI API. Types and functions defined in this
// file can reliably be replaced by their std counterparts, once available.
// You may use these definitions in your own code, but be aware that we
// will remove them once BobUI depends on the C++ version that supports
// them in namespace std. There will be NO deprecation warning, the
// definitions will JUST go away.
//
// If you can't agree to these terms, don't use these definitions!
//
// We mean it.
//

#include <numeric>
#include <limits>
#include <type_traits>

BOBUI_BEGIN_NAMESPACE

namespace q26 {

// Like std::saturate_cast
#ifdef __cpp_lib_saturation_arithmetic
using std::saturate_cast;
#else
template <typename To, typename From>
constexpr auto saturate_cast(From x)
{
    static_assert(std::is_integral_v<To>);
    static_assert(std::is_integral_v<From>);

    [[maybe_unused]]
    constexpr auto Lo = (std::numeric_limits<To>::min)();
    constexpr auto Hi = (std::numeric_limits<To>::max)();

    if constexpr (std::is_signed_v<From> == std::is_signed_v<To>) {
        // same signedness, we can accept regular integer conversion rules
        return x < Lo  ? Lo :
               x > Hi  ? Hi :
               /*else*/  To(x);
    } else {
        if constexpr (std::is_signed_v<From>) { // ie. !is_signed_v<To>
            if (x < From{0})
                return To{0};
        }

        // from here on, x >= 0
        using FromU = std::make_unsigned_t<From>;
        using ToU = std::make_unsigned_t<To>;
        return FromU(x) > ToU(Hi) ? Hi : To(x); // assumes Hi >= 0
    }
}
#endif // __cpp_lib_saturation_arithmetic

} // namespace q26

BOBUI_END_NAMESPACE

#endif /* Q26NUMERIC_H */
