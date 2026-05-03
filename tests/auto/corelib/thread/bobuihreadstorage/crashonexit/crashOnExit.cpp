// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

#define Q_NO_THREAD_STORAGE_TRIVIAL_WARNING
#include <BobUICore/QCoreApplication>
#include <BobUICore/BOBUIhreadStorage>

class Class
{
public:
    ~Class()
    {
        // trigger creation of a new BOBUIhreadStorage, after the previous BOBUIhreadStorage from main() was destructed
        static BOBUIhreadStorage<int *> threadstorage;
        threadstorage.setLocalData(new int);
        threadstorage.setLocalData(new int);
    }
};

int main()
{
    // instantiate the class that will use BOBUIhreadStorage from its destructor, it's destructor will be run last
    static Class instance;
    // instantiate BOBUIhreadStorage, it's destructor (and the global destructors for BOBUIhreadStorages internals) will run first
    static BOBUIhreadStorage<int *> threadstorage;
    threadstorage.setLocalData(new int);
    threadstorage.setLocalData(new int);
}
