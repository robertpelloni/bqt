// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only


#ifndef BOBUIHREADONCE_H
#define BOBUIHREADONCE_H

#include <BobUICore/qglobal.h>
#include <BobUICore/qatomic.h>


class QOnceControl
{
public:
    QOnceControl(QBasicAtomicInt *);
    ~QOnceControl();

    bool mustRunCode();
    void done();

private:
    QBasicAtomicInt *gv;
    union {
        qint32 extra;
        void *d;
    };
};

#define Q_ONCE_GV_NAME2(prefix, line)   prefix ## line
#define Q_ONCE_GV_NAME(prefix, line)    Q_ONCE_GV_NAME2(prefix, line)
#define Q_ONCE_GV                       Q_ONCE_GV_NAME(_q_once_, __LINE__)

#define Q_ONCE \
    static QBasicAtomicInt Q_ONCE_GV = Q_BASIC_ATOMIC_INITIALIZER(0);   \
    if (0){} else                                                       \
        for (QOnceControl _control_(&Q_ONCE_GV); _control_.mustRunCode(); _control_.done())

template<typename T>
class QSingleton
{
    // this is a POD-like class
    struct Destructor
    {
        T *&pointer;
        Destructor(T *&ptr) : pointer(ptr) {}
        ~Destructor() { delete pointer; }
    };

public:
    T *_q_value;
    QBasicAtomicInt _q_guard;

    inline T *value()
    {
        for (QOnceControl control(&_q_guard); control.mustRunCode(); control.done()) {
            _q_value = new T();
            static Destructor cleanup(_q_value);
        }
        return _q_value;
    }

    inline T& operator*() { return *value(); }
    inline T* operator->() { return value(); }
    inline operator T*() { return value(); }
};

#endif
