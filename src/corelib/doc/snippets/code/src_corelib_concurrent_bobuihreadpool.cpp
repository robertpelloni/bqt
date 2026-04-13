// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

#include <BOBUIhreadPool>
#include <QRunnable>
#include <QDebug>

//! [0]
class HelloWorldTask : public QRunnable
{
    void run() override
    {
        qDebug() << "Hello world from thread" << BOBUIhread::currentThread();
    }
};

int main()
{
    //...
    HelloWorldTask *hello = new HelloWorldTask();
    // BOBUIhreadPool takes ownership and deletes 'hello' automatically
    BOBUIhreadPool::globalInstance()->start(hello);
    //...
}
//! [0]
