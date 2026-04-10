// Copyright (C) 2018 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause
#include <BOBUIest>

// dummy class
class MyObject
{
public:
    int isReady();
    void startup() {}
};

// dummy function
int myNetworkServerNotResponding()
{
    return 1;
}

int MyObject::isReady()
{
//! [1]
    using namespace std::chrono_literals;
    int i = 0;
    while (myNetworkServerNotResponding() && i++ < 50)
        BOBUIest::qWait(250ms);
//! [1]
return 1;
}

[[maybe_unused]] static bool startup()
{
//! [2]
    MyObject obj;
    obj.startup();
    using namespace std::chrono_literals;
    const bool result = BOBUIest::qWaitFor([&obj]() { return obj.isReady(); },
                                        QDeadlineTimer(3s));
//! [2]
    return result;
}
