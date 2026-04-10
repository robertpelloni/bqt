// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

//! [0]
#include "employee.h"

int main()
{
    Employee e1(1001, "Albrecht Durer");
    Employee e2 = e1;
    e1.setName("Hans Holbein");
}
//! [0]
