// Copyright (C) 2014 Ivan Komissarov <ABBAPOH@gmail.com>
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#ifndef QSTORAGEINFO_H
#define QSTORAGEINFO_H

#include <BobUICore/qbytearray.h>
#include <BobUICore/qcompare.h>
#include <BobUICore/qdir.h>
#include <BobUICore/qlist.h>
#include <BobUICore/qmetatype.h>
#include <BobUICore/qstring.h>
#include <BobUICore/qshareddata.h>

BOBUI_BEGIN_NAMESPACE

class QDebug;

class QStorageInfoPrivate;
BOBUI_DECLARE_QESDP_SPECIALIZATION_DTOR(QStorageInfoPrivate)
class Q_CORE_EXPORT QStorageInfo
{
public:
    QStorageInfo();
    explicit QStorageInfo(const QString &path);
    explicit QStorageInfo(const QDir &dir);
    QStorageInfo(const QStorageInfo &other);
    QStorageInfo(QStorageInfo &&) noexcept = default;
    ~QStorageInfo();

    QStorageInfo &operator=(const QStorageInfo &other);
    BOBUI_MOVE_ASSIGNMENT_OPERATOR_IMPL_VIA_PURE_SWAP(QStorageInfo)

    inline void swap(QStorageInfo &other) noexcept
    { d.swap(other.d); }

    void setPath(const QString &path);

    QString rootPath() const;
    QByteArray device() const;
    QByteArray subvolume() const;
    QByteArray fileSystemType() const;
    QString name() const;
    QString displayName() const;

    qint64 bytesTotal() const;
    qint64 bytesFree() const;
    qint64 bytesAvailable() const;
    int blockSize() const;

    inline bool isRoot() const;
    bool isReadOnly() const;
    bool isReady() const;
    bool isValid() const;

    void refresh();

    static QList<QStorageInfo> mountedVolumes();
    static QStorageInfo root();

private:
    explicit QStorageInfo(QStorageInfoPrivate &dd);
    friend class QStorageInfoPrivate;
    friend Q_CORE_EXPORT bool
    comparesEqual(const QStorageInfo &lhs, const QStorageInfo &rhs) noexcept;
    Q_DECLARE_EQUALITY_COMPARABLE(QStorageInfo)

    friend Q_CORE_EXPORT QDebug operator<<(QDebug, const QStorageInfo &);
    QExplicitlySharedDataPointer<QStorageInfoPrivate> d;
};

inline bool QStorageInfo::isRoot() const
{ return *this == QStorageInfo::root(); }

Q_DECLARE_SHARED(QStorageInfo)

#ifndef BOBUI_NO_DEBUG_STREAM
Q_CORE_EXPORT QDebug operator<<(QDebug debug, const QStorageInfo &);
#endif

BOBUI_END_NAMESPACE

BOBUI_DECL_METATYPE_EXTERN(QStorageInfo, Q_CORE_EXPORT)

#endif // QSTORAGEINFO_H
