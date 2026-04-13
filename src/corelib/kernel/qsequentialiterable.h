// Copyright (C) 2020 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QSEQUENTIALITERABLE_H
#define QSEQUENTIALITERABLE_H

#include <BobUICore/qiterable.h>
#include <BobUICore/qvariant.h>

BOBUI_BEGIN_NAMESPACE

#if BOBUI_DEPRECATED_SINCE(6, 15)
BOBUI_WARNING_PUSH
BOBUI_WARNING_DISABLE_DEPRECATED

// Keep this a single long line, otherwise syncbobui doesn't create a class forwarding header
class Q_CORE_EXPORT BOBUI_DEPRECATED_VERSION_X_6_15("Use QMetaSequence's iterables and iterators instead.") QSequentialIterator : public QIterator<QMetaSequence>
{
public:
    using value_type = QVariant;
    using reference = QVariantRef<QSequentialIterator>;
    using pointer = QVariantPointer<QSequentialIterator>;

    QSequentialIterator(QIterator &&it)
        : QIterator(std::move(it))
    {}

    QVariantRef<QSequentialIterator> operator*() const;
    QVariantPointer<QSequentialIterator> operator->() const;
};

// Keep this a single long line, otherwise syncbobui doesn't create a class forwarding header
class Q_CORE_EXPORT BOBUI_DEPRECATED_VERSION_X_6_15("Use QMetaSequence's iterables and iterators instead.") QSequentialConstIterator : public QConstIterator<QMetaSequence>
{
public:
    using value_type = QVariant;
    using reference = const QVariant &;
    using pointer = QVariantConstPointer;

    QSequentialConstIterator(QConstIterator &&it)
        : QConstIterator(std::move(it))
    {}

    QVariant operator*() const;
    QVariantConstPointer operator->() const;
};

// Keep this a single long line, otherwise syncbobui doesn't create a class forwarding header
class Q_CORE_EXPORT BOBUI_DEPRECATED_VERSION_X_6_15("Use QMetaSequence's iterables and iterators instead.") QSequentialIterable : public QIterable<QMetaSequence>
{
public:
    using iterator = BOBUIaggedIterator<QSequentialIterator, void>;
    using const_iterator = BOBUIaggedIterator<QSequentialConstIterator, void>;

    using RandomAccessIterator = BOBUIaggedIterator<iterator, std::random_access_iterator_tag>;
    using BidirectionalIterator = BOBUIaggedIterator<iterator, std::bidirectional_iterator_tag>;
    using ForwardIterator = BOBUIaggedIterator<iterator, std::forward_iterator_tag>;
    using InputIterator = BOBUIaggedIterator<iterator, std::input_iterator_tag>;

    using RandomAccessConstIterator = BOBUIaggedIterator<const_iterator, std::random_access_iterator_tag>;
    using BidirectionalConstIterator = BOBUIaggedIterator<const_iterator, std::bidirectional_iterator_tag>;
    using ForwardConstIterator = BOBUIaggedIterator<const_iterator, std::forward_iterator_tag>;
    using InputConstIterator = BOBUIaggedIterator<const_iterator, std::input_iterator_tag>;

    template<class T>
    QSequentialIterable(const T *p)
        : QIterable(QMetaSequence::fromContainer<T>(), p)
    {
        Q_UNUSED(m_revision);
    }

    template<class T>
    QSequentialIterable(T *p)
        : QIterable(QMetaSequence::fromContainer<T>(), p)
    {
    }

    QSequentialIterable()
        : QIterable(QMetaSequence(), nullptr)
    {
    }

    template<typename Pointer>
    QSequentialIterable(const QMetaSequence &metaSequence, Pointer iterable)
        : QIterable(metaSequence, iterable)
    {
    }

    QSequentialIterable(const QMetaSequence &metaSequence, const QMetaType &metaType,
                        void *iterable)
        : QIterable(metaSequence, metaType.alignOf(), iterable)
    {
    }

    QSequentialIterable(const QMetaSequence &metaSequence, const QMetaType &metaType,
                        const void *iterable)
        : QIterable(metaSequence, metaType.alignOf(), iterable)
    {
    }

    QSequentialIterable(QIterable<QMetaSequence> &&other) : QIterable(std::move(other)) {}

    QSequentialIterable &operator=(QIterable<QMetaSequence> &&other)
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

    QVariant at(qsizetype idx) const;
    void set(qsizetype idx, const QVariant &value);

    enum Position { Unspecified, AtBegin, AtEnd };
    void addValue(const QVariant &value, Position position = Unspecified);
    void removeValue(Position position = Unspecified);

    QMetaType valueMetaType() const;
};

template<>
inline QVariantRef<QSequentialIterator>::operator QVariant() const
{
    if (m_pointer == nullptr)
        return QVariant();
    const QMetaType metaType(m_pointer->metaContainer().valueMetaType());

    return [&] {
    QVariant v(metaType);
    void *dataPtr = metaType == QMetaType::fromType<QVariant>() ? &v : v.data();
    m_pointer->metaContainer().valueAtIterator(m_pointer->constIterator(), dataPtr);
    return v;
    }();
}

template<>
inline QVariantRef<QSequentialIterator> &QVariantRef<QSequentialIterator>::operator=(
        const QVariant &value)
{
    if (m_pointer == nullptr)
        return *this;

    BobUIPrivate::QVariantTypeCoercer coercer;
    m_pointer->metaContainer().setValueAtIterator(
                m_pointer->constIterator(),
                coercer.coerce(value, m_pointer->metaContainer().valueMetaType()));
    return *this;
}

Q_DECLARE_TYPEINFO(QSequentialIterable, Q_RELOCATABLE_TYPE);
Q_DECLARE_TYPEINFO(QSequentialIterable::iterator, Q_RELOCATABLE_TYPE);
Q_DECLARE_TYPEINFO(QSequentialIterable::const_iterator, Q_RELOCATABLE_TYPE);

BOBUI_WARNING_POP
#endif // BOBUI_DEPRECATED_SINCE(6, 15)

BOBUI_END_NAMESPACE

#endif // QSEQUENTIALITERABLE_H
