// Copyright (C) 2016 The BobUI Company Ltd.
// Copyright (C) 2016 Intel Corporation.
// Copyright (C) 2014 by Southwest Research Institute (R)
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#include <BobUICore/qlist.h>

#ifndef QBYTEARRAYLIST_H
#define QBYTEARRAYLIST_H

#include <BobUICore/qbytearray.h>

#include <limits>

BOBUI_BEGIN_NAMESPACE

#if !defined(BOBUI_NO_JAVA_STYLE_ITERATORS)
typedef QListIterator<QByteArray> QByteArrayListIterator;
typedef QMutableListIterator<QByteArray> QMutableByteArrayListIterator;
#endif

#ifndef Q_QDOC

namespace BobUIPrivate {
#if BOBUI_CORE_REMOVED_SINCE(6, 3) && BOBUI_POINTER_SIZE != 4
    QByteArray Q_CORE_EXPORT QByteArrayList_join(const QByteArrayList *that, const char *separator, int separatorLength);
#endif
    QByteArray Q_CORE_EXPORT QByteArrayList_join(const QByteArrayList *that, const char *sep, qsizetype len);
}
#endif

#ifdef Q_QDOC
class QByteArrayList : public QList<QByteArray>
#else
template <> struct QListSpecialMethods<QByteArray> : QListSpecialMethodsBase<QByteArray>
#endif
{
#ifndef Q_QDOC
protected:
    QListSpecialMethods() = default;
    ~QListSpecialMethods() = default;
    QListSpecialMethods(const QListSpecialMethods &) = default;
    QListSpecialMethods(QListSpecialMethods &&) = default;
    QListSpecialMethods &operator=(const QListSpecialMethods &) = default;
    QListSpecialMethods &operator=(QListSpecialMethods &&) = default;
#endif
public:
    using QListSpecialMethodsBase<QByteArray>::indexOf;
    using QListSpecialMethodsBase<QByteArray>::lastIndexOf;
    using QListSpecialMethodsBase<QByteArray>::contains;

    QByteArray join(QByteArrayView sep = {}) const
    {
        return BobUIPrivate::QByteArrayList_join(self(), sep.data(), sep.size());
    }
    Q_WEAK_OVERLOAD
    inline QByteArray join(const QByteArray &sep) const
    { return join(qToByteArrayViewIgnoringNull(sep)); }
    inline QByteArray join(char sep) const
    { return join({&sep, 1}); }
};

BOBUI_END_NAMESPACE

#endif // QBYTEARRAYLIST_H
