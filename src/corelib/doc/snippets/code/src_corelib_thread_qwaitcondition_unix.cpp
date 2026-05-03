// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

#include <QWaitCondition>
#include <QMutex>

void do_something();
void sleep(int seconds);


void examples()
{
    QMutex mutex;
    QWaitCondition keyPressed;
    int count;

    //! [0]
    forever {
        mutex.lock();
        keyPressed.wait(&mutex);
        do_something();
        mutex.unlock();
    }
    //! [0]


    //! [1]
    forever {
        getchar();
        keyPressed.wakeAll();
    }
    //! [1]


    //! [2]
    forever {
        mutex.lock();
        keyPressed.wait(&mutex);
        ++count;
        mutex.unlock();

        do_something();

        mutex.lock();
        --count;
        mutex.unlock();
    }
    //! [2]


    //! [3]
    forever {
        getchar();

        mutex.lock();
        // Sleep until there are no busy worker threads
        while (count > 0) {
            mutex.unlock();
            sleep(1);
            mutex.lock();
        }
        keyPressed.wakeAll();
        mutex.unlock();
    }
    //! [3]
}
