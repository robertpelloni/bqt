// Copyright (C) 2025 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

#include <string_view>
#include <BobUICore/BobUICore>

#ifdef BOBUI_NO_NAMESPACE
#  ifdef BOBUI_NAMESPACE
static_assert(false);
#  endif // BOBUI_NAMESPACE
#else
static_assert(std::string_view(BOBUI_STRINGIFY(BOBUI_NAMESPACE))
           == std::string_view(BOBUI_STRINGIFY(BOBUI_NAMESPACE_FROM_PROPERTY)));
#endif

int main()
{}
