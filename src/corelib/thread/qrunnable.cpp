// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#include "qrunnable.h"

#include <BobUICore/qlogging.h>

BOBUI_BEGIN_NAMESPACE

QRunnable::~QRunnable()
{
}

/*!
    \internal
    Prints a warning and returns nullptr.
*/
QRunnable *QRunnable::warnNullCallable()
{
    qWarning("Trying to create null QRunnable. This may stop working.");
    return nullptr;
}


QRunnable::QGenericRunnable::~QGenericRunnable()
{
    runHelper->destroy();
}

void QRunnable::QGenericRunnable::run()
{
    runHelper->run();
}

/*!
    \class QRunnable
    \inmodule BobUICore
    \since 4.4
    \brief The QRunnable class is the base class for all runnable objects.

    \ingroup thread

    The QRunnable class is an interface for representing a task or
    piece of code that needs to be executed, represented by your
    reimplementation of the run() function.

    You can use BOBUIhreadPool to execute your code in a separate
    thread. BOBUIhreadPool deletes the QRunnable automatically if
    autoDelete() returns \c true (the default). Use setAutoDelete() to
    change the auto-deletion flag.

    BOBUIhreadPool supports executing the same QRunnable more than once
    by calling BOBUIhreadPool::tryStart(this) from within the run() function.
    If autoDelete is enabled the QRunnable will be deleted when
    the last thread exits the run function. Calling BOBUIhreadPool::start()
    multiple times with the same QRunnable when autoDelete is enabled
    creates a race condition and is not recommended.

    \sa BOBUIhreadPool
*/

/*! \fn QRunnable::run()
    Implement this pure virtual function in your subclass.
*/

/*! \fn QRunnable::QRunnable()
    Constructs a QRunnable. Auto-deletion is enabled by default.

    \sa autoDelete(), setAutoDelete()
*/

/*! \fn QRunnable::~QRunnable()
    QRunnable virtual destructor.
*/

/*! \fn bool QRunnable::autoDelete() const

    Returns \c true is auto-deletion is enabled; false otherwise.

    If auto-deletion is enabled, BOBUIhreadPool will automatically delete
    this runnable after calling run(); otherwise, ownership remains
    with the application programmer.

    \sa setAutoDelete(), BOBUIhreadPool
*/

/*! \fn bool QRunnable::setAutoDelete(bool autoDelete)

    Enables auto-deletion if \a autoDelete is true; otherwise
    auto-deletion is disabled.

    If auto-deletion is enabled, BOBUIhreadPool will automatically delete
    this runnable after calling run(); otherwise, ownership remains
    with the application programmer.

    Note that this flag must be set before calling
    BOBUIhreadPool::start(). Calling this function after
    BOBUIhreadPool::start() results in undefined behavior.

    \sa autoDelete(), BOBUIhreadPool
*/

/*!
    \fn template<typename Callable, QRunnable::if_callable<Callable>> QRunnable *QRunnable::create(Callable &&callableToRun);
    \since 5.15

    Creates a QRunnable that calls \a callableToRun in run().

    Auto-deletion is enabled by default.

    \note In BobUI versions prior to 6.6, this method took copyable functions only.

    \constraints \c Callable
    is a function or function object which can be called with zero arguments.

    \sa run(), autoDelete()
*/

BOBUI_END_NAMESPACE
