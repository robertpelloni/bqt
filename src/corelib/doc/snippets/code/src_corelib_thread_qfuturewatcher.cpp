// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

#include <QFutureWatcher>
#include <BobUIConcurrent>

class MyClass : public QObject
{
    Q_OBJECT
public:
signals:
    void handleFinished();
};

void examples()
{
    int result;

    //! [0]
    // Instantiate the objects and connect to the finished signal.
    MyClass myObject;
    QFutureWatcher<int> watcher;
    QObject::connect(&watcher, &QFutureWatcher<int>::finished, &myObject, &MyClass::handleFinished);

    // Start the computation.
    QFuture<int> future = BobUIConcurrent::run([result](){ /*...*/ return result;});
    watcher.setFuture(future);
    //! [0]
}
