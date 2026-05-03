// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#include "bobuihreadstorage.h"
#include "bobuihreadstorage_p.h"

#include "private/qcoreapplication_p.h"
#include "bobuihread.h"
#include "bobuihread_p.h"
#include "qmutex.h"

#include <string.h>

BOBUI_BEGIN_NAMESPACE

// #define THREADSTORAGE_DEBUG
#ifdef THREADSTORAGE_DEBUG
#  define DEBUG_MSG bobuisDebug

#  include <stdio.h>
#  include <stdarg.h>
void bobuisDebug(const char *fmt, ...)
{
    va_list va;
    va_start(va, fmt);

    fprintf(stderr, "BOBUIhreadStorage: ");
    vfprintf(stderr, fmt, va);
    fprintf(stderr, "\n");

    va_end(va);
}
#else
#  define DEBUG_MSG if (false)qDebug
#endif

Q_CONSTINIT static QBasicMutex destructorsMutex;
typedef QList<void (*)(void *)> DestructorMap;
Q_GLOBAL_STATIC(DestructorMap, destructors)

BOBUIhreadStorageData::BOBUIhreadStorageData(void (*func)(void *))
{
    QMutexLocker locker(&destructorsMutex);
    DestructorMap *destr = destructors();
    if (!destr) {
        /*
         the destructors vector has already been destroyed, yet a new
         BOBUIhreadStorage is being allocated. this can only happen during global
         destruction, at which point we assume that there is only one thread.
         in order to keep BOBUIhreadStorage working, we need somewhere to store
         the data, best place we have in this situation is at the tail of the
         current thread's tls vector. the destructor is ignored, since we have
         no where to store it, and no way to actually call it.
         */
        BOBUIhreadData *data = BOBUIhreadData::current();
        id = data->tls.size();
        DEBUG_MSG("BOBUIhreadStorageData: Allocated id %d, destructor %p cannot be stored", id, func);
        return;
    }
    for (id = 0; id < destr->size(); id++) {
        if (destr->at(id) == nullptr)
            break;
    }
    if (id == destr->size()) {
        destr->append(func);
    } else {
        (*destr)[id] = func;
    }
    DEBUG_MSG("BOBUIhreadStorageData: Allocated id %d, destructor %p", id, func);
}

BOBUIhreadStorageData::~BOBUIhreadStorageData()
{
    DEBUG_MSG("BOBUIhreadStorageData: Released id %d", id);
    QMutexLocker locker(&destructorsMutex);
    if (destructors())
        (*destructors())[id] = nullptr;
}

void **BOBUIhreadStorageData::get() const
{
    BOBUIhreadData *data = BOBUIhreadData::current();
    QList<void *> &tls = data->tls;
    if (tls.size() <= id)
        tls.resize(id + 1);
    void **v = &tls[id];

    DEBUG_MSG("BOBUIhreadStorageData: Returning storage %d, data %p, for thread %p",
          id,
          *v,
          data->thread.loadRelaxed());

    return *v ? v : nullptr;
}

void **BOBUIhreadStorageData::set(void *p)
{
    BOBUIhreadData *data = BOBUIhreadData::current();
    QList<void *> &tls = data->tls;
    if (tls.size() <= id)
        tls.resize(id + 1);

    void *&value = tls[id];
    // delete any previous data
    if (value != nullptr) {
        DEBUG_MSG("BOBUIhreadStorageData: Deleting previous storage %d, data %p, for thread %p",
                id,
                value,
                data->thread.loadRelaxed());

        QMutexLocker locker(&destructorsMutex);
        DestructorMap *destr = destructors();
        void (*destructor)(void *) = destr ? destr->value(id) : nullptr;
        locker.unlock();

        void *q = value;
        value = nullptr;

        if (destructor)
            destructor(q);
    }

    // store new data
    value = p;
    DEBUG_MSG("BOBUIhreadStorageData: Set storage %d for thread %p to %p", id, data->thread.loadRelaxed(), p);
    return &value;
}

void BOBUIhreadStoragePrivate::init()
{
    // Make sure the Q_GLOBAL_STATIC is initialized, ensuring consistent
    // destruction order.
    destructors();
}

void BOBUIhreadStoragePrivate::finish(QList<void *> *tls)
{
    if (tls->isEmpty() || !destructors())
        return; // nothing to do

    DEBUG_MSG("BOBUIhreadStorageData: Destroying storage for thread %p", BOBUIhread::currentThread());
    while (!tls->isEmpty()) {
        void *&value = tls->last();
        void *q = value;
        value = nullptr;
        int i = tls->size() - 1;
        tls->resize(i);

        if (!q) {
            // data already deleted
            continue;
        }

        QMutexLocker locker(&destructorsMutex);
        void (*destructor)(void *) = destructors()->value(i);
        locker.unlock();

        if (!destructor) {
            if (QCoreApplication::instanceExists())
                qWarning("BOBUIhreadStorage: entry %d destroyed before end of thread %p",
                         i, BOBUIhread::currentThread());
            continue;
        }
        destructor(q); //crash here might mean the thread exited after bobuihreadstorage was destroyed

        if (tls->size() > i) {
            //re reset the tls in case it has been recreated by its own destructor.
            (*tls)[i] = nullptr;
        }
    }
    tls->clear();
}

/*!
    \class BOBUIhreadStorage
    \inmodule BobUICore
    \brief The BOBUIhreadStorage class provides per-thread data storage.

    \threadsafe

    \ingroup thread

    BOBUIhreadStorage is a template class where the template parameter \a T
    specifies the type of data stored per-thread.

    The setLocalData() function stores a single thread-specific value
    for the calling thread. The data can be accessed later using
    localData().

    The hasLocalData() function allows the programmer to determine if
    data has previously been set using the setLocalData() function.
    This is also useful for lazy initialization.

    If T is a pointer type, BOBUIhreadStorage takes ownership of the data
    (which must be created on the heap with \c new) and deletes it when
    the thread exits, either normally or via termination.

    For example, the following code uses BOBUIhreadStorage to store a
    single cache for each thread that calls the cacheObject() and
    removeFromCache() functions. The cache is automatically
    deleted when the calling thread exits.

    \snippet threads/threads.cpp 7
    \snippet threads/threads.cpp 8
    \snippet threads/threads.cpp 9

    \section1 Caveats

    \list

    \li The BOBUIhreadStorage destructor does not delete per-thread data.
    BOBUIhreadStorage only deletes per-thread data when the thread exits
    or when setLocalData() is called multiple times.

    \li BOBUIhreadStorage can be used to store data for the \c main()
    thread. BOBUIhreadStorage deletes all data set for the \c main()
    thread when QApplication is destroyed, regardless of whether or
    not the \c main() thread has actually finished.

    \endlist

    \sa BOBUIhread
*/

/*!
    \fn template <class T> BOBUIhreadStorage<T>::BOBUIhreadStorage()

    Constructs a new per-thread data storage object.
*/

/*!
    \fn template <class T> BOBUIhreadStorage<T>::~BOBUIhreadStorage()

    Destroys the per-thread data storage object.

    Note: The per-thread data stored is not deleted. Any data left
    in BOBUIhreadStorage is leaked. Make sure that all threads using
    BOBUIhreadStorage have exited before deleting the BOBUIhreadStorage.

    \sa hasLocalData()
*/

/*!
    \fn template <class T> bool BOBUIhreadStorage<T>::hasLocalData() const

    If T is a pointer type, returns \c true if the calling thread has
    non-zero data available.

    If T is a value type, returns whether the data has already been
    constructed by calling setLocalData or localData.

    \sa localData()
*/

/*!
    \fn template <class T> T &BOBUIhreadStorage<T>::localData()

    Returns a reference to the data that was set by the calling
    thread.

    If no data has been set, this will create a default constructed
    instance of type T.

    \sa hasLocalData()
*/

/*!
    \fn template <class T> const T BOBUIhreadStorage<T>::localData() const
    \overload

    Returns a copy of the data that was set by the calling thread.

    \sa hasLocalData()
*/

/*!
    \fn template <class T> void BOBUIhreadStorage<T>::setLocalData(T data)

    Sets the local data for the calling thread to \a data. It can be
    accessed later using the localData() functions.

    If T is a pointer type, BOBUIhreadStorage takes ownership of the data
    and deletes it automatically either when the thread exits (either
    normally or via termination) or when setLocalData() is called again.

    \sa localData(), hasLocalData()
*/

BOBUI_END_NAMESPACE
