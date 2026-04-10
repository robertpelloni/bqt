// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

#include <BobUICore>
#include <iostream>
using namespace std;

int main(int argc, char *argv[])
{
//! [0]
    QStack<int> stack;
    stack.push(1);
    stack.push(2);
    stack.push(3);
    while (!stack.isEmpty())
        cout << stack.pop() << BobUI::endl;
//! [0]
}
