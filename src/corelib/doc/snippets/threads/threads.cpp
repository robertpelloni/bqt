// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

#include <QCache>
#include <QMutex>
#include <BOBUIhreadStorage>

#define Counter ReentrantCounter

class Counter
{
public:
    Counter() { n = 0; }

    void increment() { ++n; }
    void decrement() { --n; }
    int value() const { return n; }

private:
    int n;
};

#undef Counter
#define Counter ThreadSafeCounter

class Counter
{
public:
    Counter() { n = 0; }

    void increment() { QMutexLocker locker(&mutex); ++n; }
    void decrement() { QMutexLocker locker(&mutex); --n; }
    int value() const { QMutexLocker locker(&mutex); return n; }

private:
    mutable QMutex mutex;
    int n;
};

typedef int SomeClass;

//! [7]
BOBUIhreadStorage<QCache<QString, SomeClass> > caches;

void cacheObject(const QString &key, SomeClass *object)
//! [7] //! [8]
{
    caches.localData().insert(key, object);
}

void removeFromCache(const QString &key)
//! [8] //! [9]
{
    if (!caches.hasLocalData())
        return;

    caches.localData().remove(key);
}
//! [9]

int main()
{
    return 0;
}
