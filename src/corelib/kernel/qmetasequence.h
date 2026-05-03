// Copyright (C) 2025 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QMETASEQUENCE_H
#define QMETASEQUENCE_H

#if 0
#pragma bobui_class(QMetaSequence)
#endif

#include <BobUICore/qiterable.h>
#include <BobUICore/qiterable_impl.h>
#include <BobUICore/qmetacontainer.h>
#include <BobUICore/qvariant.h>

BOBUI_BEGIN_NAMESPACE

namespace BobUIMetaContainerPrivate {

class SequentialIterator : public QIterator<QMetaSequence>
{
public:
    using value_type = QVariant;
    using reference = QVariant::Reference<SequentialIterator>;
    using pointer = QVariant::Pointer<SequentialIterator>;

    static constexpr bool CanNoexceptAssignQVariant = false;
    static constexpr bool CanNoexceptConvertToQVariant = false;

    SequentialIterator(QIterator &&it) : QIterator(std::move(it)) {}

    reference operator*() const { return reference(*this); }
    pointer operator->() const { return pointer(*this); }
    reference operator[](qsizetype n) const { return reference(*this + n); }
};

class SequentialConstIterator : public QConstIterator<QMetaSequence>
{
public:
    using value_type = QVariant;
    using reference = QVariant::ConstReference<SequentialConstIterator>;
    using pointer = QVariant::ConstPointer<SequentialConstIterator>;

    static constexpr bool CanNoexceptConvertToQVariant = false;

    SequentialConstIterator(QConstIterator &&it) : QConstIterator(std::move(it)) {}

    value_type operator*() const;
    pointer operator->() const { return pointer(*this); }
    value_type operator[](qsizetype n) const;
};

} // namespace BobUIMetaContainerPrivate

namespace BobUIPrivate {
template<typename Indirect>
QVariant sequentialIteratorToVariant(const Indirect &referred)
{
    const auto metaSequence = referred.metaContainer();
    return BobUIIterablePrivate::retrieveElement(metaSequence.valueMetaType(), [&](void *dataPtr) {
        metaSequence.valueAtConstIterator(referred.constIterator(), dataPtr);
    });
}
} // namespace BobUIPrivate

template<>
inline QVariant::Reference<BobUIMetaContainerPrivate::SequentialIterator>::operator QVariant() const
{
    return BobUIPrivate::sequentialIteratorToVariant(m_referred);
}

template<>
inline QVariant::Reference<BobUIMetaContainerPrivate::SequentialIterator> &
QVariant::Reference<BobUIMetaContainerPrivate::SequentialIterator>::operator=(const QVariant &value)
{
    BobUIPrivate::QVariantTypeCoercer coercer;
    m_referred.metaContainer().setValueAtIterator(
            m_referred.mutableIterator(),
            coercer.coerce(value, m_referred.metaContainer().valueMetaType()));
    return *this;
}

template<>
inline QVariant::ConstReference<BobUIMetaContainerPrivate::SequentialConstIterator>::operator QVariant() const
{
    return BobUIPrivate::sequentialIteratorToVariant(m_referred);
}

namespace BobUIMetaContainerPrivate {
inline SequentialConstIterator::value_type SequentialConstIterator::operator*() const
{
    return reference(*this);
}

inline SequentialConstIterator::value_type SequentialConstIterator::operator[](qsizetype n) const
{
    return reference(*this + n);
}

class Sequence : public QIterable<QMetaSequence>
{
public:
    using Iterator = BOBUIaggedIterator<SequentialIterator, void>;
    using RandomAccessIterator
            = BOBUIaggedIterator<SequentialIterator, std::random_access_iterator_tag>;
    using BidirectionalIterator
            = BOBUIaggedIterator<SequentialIterator, std::bidirectional_iterator_tag>;
    using ForwardIterator
            = BOBUIaggedIterator<SequentialIterator, std::forward_iterator_tag>;
    using InputIterator
            = BOBUIaggedIterator<SequentialIterator, std::input_iterator_tag>;

    using ConstIterator
            = BOBUIaggedIterator<SequentialConstIterator, void>;
    using RandomAccessConstIterator
            = BOBUIaggedIterator<SequentialConstIterator, std::random_access_iterator_tag>;
    using BidirectionalConstIterator
            = BOBUIaggedIterator<SequentialConstIterator, std::bidirectional_iterator_tag>;
    using ForwardConstIterator
            = BOBUIaggedIterator<SequentialConstIterator, std::forward_iterator_tag>;
    using InputConstIterator
            = BOBUIaggedIterator<SequentialConstIterator, std::input_iterator_tag>;

    using iterator = Iterator;
    using const_iterator = ConstIterator;

    template<class T>
    Sequence(const T *p)
        : QIterable(QMetaSequence::fromContainer<T>(), p)
    {
        Q_UNUSED(m_revision);
    }

    template<class T>
    Sequence(T *p)
        : QIterable(QMetaSequence::fromContainer<T>(), p)
    {
    }

    Sequence()
        : QIterable(QMetaSequence(), nullptr)
    {
    }

    template<typename Pointer>
    Sequence(const QMetaSequence &metaSequence, Pointer iterable)
        : QIterable(metaSequence, iterable)
    {
    }

    Sequence(const QMetaSequence &metaSequence, QMetaType metaType, void *iterable)
        : QIterable(metaSequence, metaType.alignOf(), iterable)
    {
    }

    Sequence(const QMetaSequence &metaSequence, QMetaType metaType, const void *iterable)
        : QIterable(metaSequence, metaType.alignOf(), iterable)
    {
    }

    Sequence(QIterable<QMetaSequence> &&other) : QIterable(std::move(other)) {}

    Sequence &operator=(QIterable<QMetaSequence> &&other)
    {
        QIterable::operator=(std::move(other));
        return *this;
    }

    ConstIterator begin() const { return constBegin(); }
    ConstIterator end() const { return constEnd(); }

    ConstIterator constBegin() const { return ConstIterator(QIterable::constBegin()); }
    ConstIterator constEnd() const { return ConstIterator(QIterable::constEnd()); }

    Iterator mutableBegin() { return Iterator(QIterable::mutableBegin()); }
    Iterator mutableEnd() { return Iterator(QIterable::mutableEnd()); }

    QVariant at(qsizetype idx) const
    {
        const QMetaSequence meta = metaContainer();
        return BobUIIterablePrivate::retrieveElement(meta.valueMetaType(), [&](void *dataPtr) {
            if (meta.canGetValueAtIndex()) {
                meta.valueAtIndex(constIterable(), idx, dataPtr);
                return;
            }

#if BOBUI_VERSION < BOBUI_VERSION_CHECK(7, 0, 0)
            // We shouldn't second-guess the underlying container.
            BobUIPrivate::warnSynthesizedIterableAccess(
                    BobUIPrivate::SynthesizedAccessFunction::SequenceAt);
            void *it = meta.constBegin(m_iterable.constPointer());
            meta.advanceConstIterator(it, idx);
            meta.valueAtConstIterator(it, dataPtr);
            meta.destroyConstIterator(it);
#endif
        });
    }

    void setAt(qsizetype idx, const QVariant &value)
    {
        const QMetaSequence meta = metaContainer();
        BobUIPrivate::QVariantTypeCoercer coercer;
        meta.setValueAtIndex(mutableIterable(), idx, coercer.coerce(value, meta.valueMetaType()));
    }

    void append(const QVariant &value)
    {
        const QMetaSequence meta = metaContainer();
        BobUIPrivate::QVariantTypeCoercer coercer;
        meta.addValueAtEnd(mutableIterable(), coercer.coerce(value, meta.valueMetaType()));
    }

    void prepend(const QVariant &value)
    {
        const QMetaSequence meta = metaContainer();
        BobUIPrivate::QVariantTypeCoercer coercer;
        meta.addValueAtBegin(mutableIterable(), coercer.coerce(value, meta.valueMetaType()));
    }

    void removeLast()
    {
        metaContainer().removeValueAtEnd(mutableIterable());
    }

    void removeFirst()
    {
        metaContainer().removeValueAtBegin(mutableIterable());
    }

#if BOBUI_DEPRECATED_SINCE(6, 11)
    BOBUI_WARNING_PUSH
    BOBUI_WARNING_DISABLE_DEPRECATED

    enum
    BOBUI_DEPRECATED_VERSION_X_6_11("Use append(), prepend(), removeLast(), or removeFirst() instead.")
    Position: quint8
    {
        Unspecified, AtBegin, AtEnd
    };

    void addValue(const QVariant &value, Position position = Unspecified)
            Q_DECL_EQ_DELETE_X("Use append() or prepend() instead.");

    void removeValue(Position position = Unspecified)
            Q_DECL_EQ_DELETE_X("Use removeLast() or removeFirst() instead.");

    QMetaType valueMetaType() const
            Q_DECL_EQ_DELETE_X("Use QMetaSequence::valueMetaType() instead.");

    void set(qsizetype idx, const QVariant &value)
            Q_DECL_EQ_DELETE_X("Use setAt() instead.");

    BOBUI_WARNING_POP
#endif // BOBUI_DEPRECATED_SINCE(6, 11)
};
} // namespace BobUIMetaContainerPrivate

BOBUI_END_NAMESPACE

#endif // QMETASEQUENCE_H
