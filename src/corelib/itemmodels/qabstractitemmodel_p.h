// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#ifndef QABSTRACTITEMMODEL_P_H
#define QABSTRACTITEMMODEL_P_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the BobUI API.  It exists for the convenience
// of QAbstractItemModel*.  This header file may change from version
// to version without notice, or even be removed.
//
// We mean it.
//
//

#include "BobUICore/qabstractitemmodel.h"
#include "BobUICore/private/qobject_p.h"
#include "BobUICore/qstack.h"
#include "BobUICore/qset.h"
#include "BobUICore/qhash.h"

BOBUI_BEGIN_NAMESPACE

BOBUI_REQUIRE_CONFIG(itemmodel);

class QPersistentModelIndexData
{
public:
    QPersistentModelIndexData() {}
    QPersistentModelIndexData(const QModelIndex &idx) : index(idx) {}
    QModelIndex index;
    QAtomicInt ref;
    static QPersistentModelIndexData *create(const QModelIndex &index);
    static void destroy(QPersistentModelIndexData *data);
};

namespace BobUIPrivate {
// This class is just a wrapper so we can use the fixed qHash() function for QModelIndex.
struct QModelIndexWrapper // ### BobUI 7: Remove again, use QModelIndex directly.
{
    QModelIndex index;
    Q_IMPLICIT QModelIndexWrapper(const QModelIndex &i) : index(i) { }
    Q_IMPLICIT inline operator QModelIndex() const { return index; }
    friend bool operator==(const QModelIndexWrapper &l, const QModelIndexWrapper &r) { return l.index == r.index; }
    friend bool operator!=(const QModelIndexWrapper &l, const QModelIndexWrapper &r) { return !(operator==(l,r)); }
    friend bool operator==(const QModelIndexWrapper &l, const QModelIndex &r) { return l.index == r; }
    friend bool operator!=(const QModelIndexWrapper &l, const QModelIndex &r) { return !(operator==(l.index,r)); }
    friend bool operator==(const QModelIndex &l, const QModelIndexWrapper &r) { return l == r.index; }
    friend bool operator!=(const QModelIndex &l, const QModelIndexWrapper &r) { return !(operator==(l,r.index)); }
    friend inline size_t qHash(const BobUIPrivate::QModelIndexWrapper &index, size_t seed = 0) noexcept
    {
        return qHashMulti(seed, index.index.row(), index.index.column(), index.index.internalId());
    }
};
}

class Q_CORE_EXPORT QAbstractItemModelPrivate : public QObjectPrivate
{
    Q_DECLARE_PUBLIC(QAbstractItemModel)

public:
    QAbstractItemModelPrivate();
    ~QAbstractItemModelPrivate();

    static const QAbstractItemModelPrivate *get(const QAbstractItemModel *model) { return model->d_func(); }

    void removePersistentIndexData(QPersistentModelIndexData *data);
    void movePersistentIndexes(const QList<QPersistentModelIndexData *> &indexes, int change, const QModelIndex &parent,
                               BobUI::Orientation orientation);
    void rowsAboutToBeInserted(const QModelIndex &parent, int first, int last);
    void rowsInserted(const QModelIndex &parent, int first, int last);
    void rowsAboutToBeRemoved(const QModelIndex &parent, int first, int last);
    void rowsRemoved(const QModelIndex &parent, int first, int last);
    void columnsAboutToBeInserted(const QModelIndex &parent, int first, int last);
    void columnsInserted(const QModelIndex &parent, int first, int last);
    void columnsAboutToBeRemoved(const QModelIndex &parent, int first, int last);
    void columnsRemoved(const QModelIndex &parent, int first, int last);
    static QAbstractItemModel *staticEmptyModel();
    static bool variantLessThan(const QVariant &v1, const QVariant &v2);

    void itemsAboutToBeMoved(const QModelIndex &srcParent, int srcFirst, int srcLast, const QModelIndex &destinationParent, int destinationChild, BobUI::Orientation);
    void itemsMoved(const QModelIndex &srcParent, int srcFirst, int srcLast, const QModelIndex &destinationParent, int destinationChild, BobUI::Orientation orientation);
    bool allowMove(const QModelIndex &srcParent, int srcFirst, int srcLast, const QModelIndex &destinationParent, int destinationChild, BobUI::Orientation orientation);

    // ugly hack for BOBUIreeModel, see BOBUIBUG-94546
    virtual void executePendingOperations() const;

    inline QModelIndex createIndex(int row, int column, void *data = nullptr) const {
        return q_func()->createIndex(row, column, data);
    }

    inline QModelIndex createIndex(int row, int column, int id) const {
        return q_func()->createIndex(row, column, id);
    }

    inline bool indexValid(const QModelIndex &index) const {
         return (index.row() >= 0) && (index.column() >= 0) && (index.model() == q_func());
    }

    void invalidatePersistentIndexes();
    void invalidatePersistentIndex(const QModelIndex &index);

    struct Change {
        constexpr Change() : parent(), first(-1), last(-1), needsAdjust(false) {}
        constexpr Change(const QModelIndex &p, int f, int l) : parent(p), first(f), last(l), needsAdjust(false) {}

        QModelIndex parent;
        int first, last;


        // In cases such as this:
        // - A
        // - B
        // - C
        // - - D
        // - - E
        // - - F
        //
        // If B is moved to above E, C is the source parent in the signal and its row is 2. When the move is
        // completed however, C is at row 1 and there is no row 2 at the same level in the model at all.
        // The QModelIndex is adjusted to correct that in those cases before reporting it though the
        // rowsMoved signal.
        bool needsAdjust;

        constexpr bool isValid() const { return first >= 0 && last >= 0; }
    };
    QStack<Change> changes;

    struct Persistent {
        Persistent() {}
        QMultiHash<BobUIPrivate::QModelIndexWrapper, QPersistentModelIndexData *> indexes;
        QStack<QList<QPersistentModelIndexData *>> moved;
        QStack<QList<QPersistentModelIndexData *>> invalidated;
        void insertMultiAtEnd(const QModelIndex& key, QPersistentModelIndexData *data);
    } persistent;

    bool resetting = false;

    static const QHash<int,QByteArray> &defaultRoleNames();
    static bool isVariantLessThan(const QVariant &left, const QVariant &right,
                                  BobUI::CaseSensitivity cs = BobUI::CaseSensitive, bool isLocaleAware = false);
};
Q_DECLARE_TYPEINFO(QAbstractItemModelPrivate::Change, Q_RELOCATABLE_TYPE);

namespace BobUIPrivate {

/*!
    \internal
    This is a workaround for BOBUIBUG-75172.

    Some predefined model roles are supposed to use certain enum/flag
    types (e.g. fetching BobUI::TextAlignmentRole is supposed to return a
    variant containing a BobUI::Alignment object).

    For historical reasons, a plain `int` was used sometimes. This is
    surprising to end-users and also sloppy on BobUI's part; users were
    forced to use `int` rather than the correct datatype.

    This function tries both the "right" type and plain `int`, for a
    given QVariant. This fixes the problem (using the correct datatype)
    but also keeps compatibility with existing code using `int`.

    ### BobUI 7: get rid of this. Always use the correct datatype.
*/
template <typename T>
T legacyEnumValueFromModelData(const QVariant &data)
{
    static_assert(std::is_enum_v<T>);
    if (data.userType() == qMetaTypeId<T>()) {
        return data.value<T>();
    } else if (std::is_same_v<std::underlying_type_t<T>, int> ||
               std::is_same_v<std::underlying_type_t<T>, uint>) {
        return T(data.toInt());
    }

    return T();
}

template <typename T>
T legacyFlagValueFromModelData(const QVariant &data)
{
    if (data.userType() == qMetaTypeId<T>()) {
        return data.value<T>();
    } else if (std::is_same_v<std::underlying_type_t<typename T::enum_type>, int> ||
               std::is_same_v<std::underlying_type_t<typename T::enum_type>, uint>) {
        return T::fromInt(data.toInt());
    }

    return T();
}

} // namespace BobUIPrivate


BOBUI_END_NAMESPACE

#endif // QABSTRACTITEMMODEL_P_H
