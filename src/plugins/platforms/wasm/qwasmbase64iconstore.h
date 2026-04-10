// Copyright (C) 2018 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

#ifndef QWASMBASE64IMAGESTORE_H
#define QWASMBASE64IMAGESTORE_H

#include <string>
#include <string_view>

#include <BobUICore/bobuiconfigmacros.h>

BOBUI_BEGIN_NAMESPACE
class Base64IconStore
{
public:
    enum class IconType {
        Maximize,
        First = Maximize,
        BobUILogo,
        Restore,
        X,
        Size,
    };

    Base64IconStore();
    ~Base64IconStore();

    static Base64IconStore *get();

    std::string_view getIcon(IconType type) const;

private:
    std::string m_storage[static_cast<size_t>(IconType::Size)];
};

BOBUI_END_NAMESPACE
#endif // QWASMBASE64IMAGESTORE_H
