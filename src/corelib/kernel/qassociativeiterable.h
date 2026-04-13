// Copyright (C) 2020 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QASSOCIATIVEITERABLE_H
#define QASSOCIATIVEITERABLE_H

#include <BobUICore/qiterable.h>
#include <BobUICore/qvariant.h>

BOBUI_BEGIN_NAMESPACE

#if BOBUI_DEPRECATED_SINCE(6, 15)
BOBUI_WARNING_PUSH
BOBUI_WARNING_DISABLE_DEPRECATED

// Keep this a single long line, otherwise syncbobui doesn't create a class forwarding header
class Q_CORE_EXPORT BOBUI_DEPRECATED_VERSION_X_6_15("Use QMetaAssociation's iterables and iterators instead.") QAssociativeIterator : public QIterator<QMetaAssociation>
{
public:
    using key_type = QVariant;
    using mapped_type = QVariant;
    using reference = QVariantRef<QAssociativeIterator>;
    using pointer = QVariantPointer<QAssociativeIterator>;

    QAssociativeIterator(QIterator &&it)
        : QIterator(std::move(it))
    {}

    QVariant key() const;
    QVariantRef<QAssociativeIterator> value() const;

    QVariantRef<QAssociativeIterator> operator*() const;
    QVariantPointer<QAssociativeIterator> operator->() const;
};

// Keep this a single long line, otherwise syncbobui doesn't create a class forwarding header
class Q_CORE_EXPORT BOBUI_DEPRECATED_VERSION_X_6_15("Use QMetaAssociation's iterables and iterators instead.") QAssociativeConstIterator : public QConstIterator<QMetaAssociation>
{
public:
    using key_type = QVariant;
    using mapped_type = QVariant;
    using reference = const QVariant &;
    using pointer = QVariantConstPointer;

    QAssociativeConstIterator(QConstIterator &&it)
        : QConstIterator(std::move(it))
    {}

    QVariant key() const;
    QVariant value() const;

    QVariant operator*() const;
    QVariantConstPointer operator->() const;
};

// Keep this a single long line, otherwise syncbobui doesn't create a class forwarding header
class Q_CORE_EXPORT BOBUI_DEPRECATED_VERSION_X_6_15("Use QMetaAssociation's iterables and iterators instead.") QAssociativeIterable : public QIterable<QMetaAssociation>
{
public:
    using iterator = BOBUIaggedIterator<QAssociativeIterator, void>;
    using const_iterator = BOBUIaggedIterator<QAssociativeConstIterator, void>;

    using RandomAccessIterator = BOBUIaggedIterator<iterator, std::random_access_iterator_tag>;
    using BidirectionalIterator = BOBUIaggedIterator<iterator, std::bidirectional_iterator_tag>;
    using ForwardIterator = BOBUIaggedIterator<iterator, std::forward_iterator_tag>;
    using InputIterator = BOBUIaggedIterator<iterator, std::input_iterator_tag>;

    using RandomAccessConstIterator = BOBUIaggedIterator<const_iterator, std::random_access_iterator_tag>;
    using BidirectionalConstIterator = BOBUIaggedIterator<const_iterator, std::bidirectional_iterator_tag>;
    using ForwardConstIterator = BOBUIaggedIterator<const_iterator, std::forward_iterator_tag>;
    using InputConstIterator = BOBUIaggedIterator<const_iterator, std::input_iterator_tag>;

    template<class T>
    QAssociativeIterable(const T *p)
        : QIterable(QMetaAssociation::fromContainer<T>(), p)
    {
    }

    template<class T>
    QAssociativeIterable(T *p)
        : QIterable(QMetaAssociation::fromContainer<T>(), p)
    {
    }

    QAssociativeIterable()
        : QIterable(QMetaAssociation(), nullptr)
    {
    }

    template<typename Pointer>
    QAssociativeIterable(const QMetaAssociation &metaAssociation, Pointer iterable)
        : QIterable(metaAssociation, iterable)
    {
    }

    QAssociativeIterable(const QMetaAssociation &metaAssociation, const QMetaType &metaType,
                         void *iterable)
        : QIterable(metaAssociation, metaType.alignOf(), iterable)
    {
    }

    QAssociativeIterable(const QMetaAssociation &metaAssociation, const QMetaType &metaType,
                         const void *iterable)
        : QIterable(metaAssociation, metaType.alignOf(), iterable)
    {
    }

    QAssociativeIterable(QIterable<QMetaAssociation> &&other) : QIterable(std::move(other)) {}

    QAssociativeIterable &operator=(QIterable<QMetaAssociation> &&other)
    {
        QIterable::operator=(std::move(other));
        return *this;
    }

    const_iterator begin() const { return constBegin(); }
    const_iterator end() const { return constEnd(); }

    const_iterator constBegin() const { return const_iterator(QIterable::constBegin()); }
    const_iterator constEnd() const { return const_iterator(QIterable::constEnd()); }

    iterator mutableBegin() { return iterator(QIterable::mutableBegin()); }
    iterator mutableEnd() { return iterator(QIterable::mutableEnd()); }

    const_iterator find(const QVariant &key) const;
    const_iterator constFind(const QVariant &key) const { return find(key); }
    iterator mutableFind(const QVariant &key);

    bool containsKey(const QVariant &key);
    void insertKey(const QVariant &key);
    void removeKey(const QVariant &key);

    QVariant value(const QVariant &key) const;
    void setValue(const QVariant &key, const QVariant &mapped);
};

template<>
inline QVariantRef<QAssociativeIterator>::operator QVariant() const
{
    if (m_pointer == nullptr)
        return QVariant();

    const auto metaAssociation = m_pointer->metaContainer();
    const QMetaType metaType(metaAssociation.mappedMetaType());
    if (!metaType.isValid())
        return m_pointer->key();

    return [&] {
    QVariant v(metaType);
    metaAssociation.mappedAtIterator(m_pointer->constIterator(),
                                     metaType == QMetaType::fromType<QVariant>() ? &v : v.data());
    return v;
    }();
}

template<>
inline QVariantRef<QAssociativeIterator> &QVariantRef<QAssociativeIterator>::operator=(
        const QVariant &value)
{
    if (m_pointer == nullptr)
        return *this;

    const auto metaAssociation = m_pointer->metaContainer();
    const QMetaType metaType(metaAssociation.mappedMetaType());
    if (metaType.isValid()) {
        BobUIPrivate::QVariantTypeCoercer coercer;
        metaAssociation.setMappedAtIterator(m_pointer->constIterator(),
                                            coercer.coerce(value, metaType));
    }

    return *this;
}

Q_DECLARE_TYPEINFO(QAssociativeIterable, Q_RELOCATABLE_TYPE);
Q_DECLARE_TYPEINFO(QAssociativeIterable::iterator, Q_RELOCATABLE_TYPE);
Q_DECLARE_TYPEINFO(QAssociativeIterable::const_iterator, Q_RELOCATABLE_TYPE);

BOBUI_WARNING_POP
#endif // BOBUI_DEPRECATED_SINCE(6, 15)

BOBUI_END_NAMESPACE

#endif // QASSOCIATIVEITERABLE_H
