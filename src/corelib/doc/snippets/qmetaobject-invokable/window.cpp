// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

#include "window.h"

Window::Window()
{
}

void Window::normalMethod()
{
    // Cannot be called by the meta-object system.
    show();
}

void Window::invokableMethod()
{
    // Can be called by the meta-object system.
    show();
}
