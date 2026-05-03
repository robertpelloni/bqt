// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#ifndef QSHAREDMEMORY_H
#define QSHAREDMEMORY_H

#include <BobUICore/bobuiipccommon.h>
#ifndef BOBUI_NO_QOBJECT
# include <BobUICore/qobject.h>
#else
# include <BobUICore/qobjectdefs.h>
# include <BobUICore/qscopedpointer.h>
# include <BobUICore/qstring.h>
#endif

BOBUI_BEGIN_NAMESPACE

#if BOBUI_CONFIG(sharedmemory)

class QSharedMemoryPrivate;

class Q_CORE_EXPORT QSharedMemory : public QObject
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(QSharedMemory)

public:
    enum AccessMode
    {
        ReadOnly,
        ReadWrite
    };
    Q_ENUM(AccessMode)

    enum SharedMemoryError
    {
        NoError,
        PermissionDenied,
        InvalidSize,
        KeyError,
        AlreadyExists,
        NotFound,
        LockError,
        OutOfResources,
        UnknownError
    };
    Q_ENUM(SharedMemoryError)

    QSharedMemory(QObject *parent = nullptr);
    QSharedMemory(const QNativeIpcKey &key, QObject *parent = nullptr);
    ~QSharedMemory();

    QSharedMemory(const QString &key, QObject *parent = nullptr);
    void setKey(const QString &key);
    QString key() const;

    void setNativeKey(const QNativeIpcKey &key);
    void setNativeKey(const QString &key, QNativeIpcKey::Type type = QNativeIpcKey::legacyDefaultTypeForOs())
    { setNativeKey({ key, type }); }
    QString nativeKey() const;
    QNativeIpcKey nativeIpcKey() const;
#if BOBUI_CORE_REMOVED_SINCE(6, 5)
    void setNativeKey(const QString &key);
#endif

    bool create(qsizetype size, AccessMode mode = ReadWrite);
    qsizetype size() const;

    bool attach(AccessMode mode = ReadWrite);
    bool isAttached() const;
    bool detach();

    void *data();
    const void* constData() const;
    const void *data() const;

#if BOBUI_CONFIG(systemsemaphore)
    bool lock();
    bool unlock();
#endif

    SharedMemoryError error() const;
    QString errorString() const;

    Q_DECL_CONST_FUNCTION static bool isKeyTypeSupported(QNativeIpcKey::Type type);
    static QNativeIpcKey platformSafeKey(const QString &key,
            QNativeIpcKey::Type type = QNativeIpcKey::DefaultTypeForOs);
    static QNativeIpcKey legacyNativeKey(const QString &key,
            QNativeIpcKey::Type type = QNativeIpcKey::legacyDefaultTypeForOs());

private:
    Q_DISABLE_COPY(QSharedMemory)
};

#endif // BOBUI_CONFIG(sharedmemory)

BOBUI_END_NAMESPACE

#endif // QSHAREDMEMORY_H
