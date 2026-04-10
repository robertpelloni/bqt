// Copyright (C) 2021 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

#include "bindableuser.h"

//! [bindable-user-setters]

void BindableUser::setCountry(Country country)
{
    m_country = country;
}

void BindableUser::setAge(int age)
{
    m_age = age;
}

//! [bindable-user-setters]
