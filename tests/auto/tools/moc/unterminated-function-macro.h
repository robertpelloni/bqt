// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

#ifndef UNTERMINATED_FUNCTION_MACRO_H
#define UNTERMINATED_FUNCTION_MACRO_H

class Dummy : public QObject {
    Q_OBJECT
}

#define MACRO(arg) do_something(arg)

static void foo() {
    MACRO(foo
}

#endif // UNTERMINATED_FUNCTION_MACRO_H
