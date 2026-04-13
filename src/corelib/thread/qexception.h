// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#ifndef BOBUICORE_QEXCEPTION_H
#define BOBUICORE_QEXCEPTION_H

#include <BobUICore/qatomic.h>
#include <BobUICore/qshareddata.h>

#ifndef BOBUI_NO_EXCEPTIONS
#  include <exception>
#endif

BOBUI_REQUIRE_CONFIG(future);

BOBUI_BEGIN_NAMESPACE


#if !defined(BOBUI_NO_EXCEPTIONS) || defined(Q_QDOC)

class Q_CORE_EXPORT QException : public std::exception
{
public:
    QException() = default;
    ~QException() noexcept;
    QException(const QException &) = default;
    QException &operator=(const QException &) = default;
    virtual void raise() const;
    virtual QException *clone() const;
};

class QUnhandledExceptionPrivate;
class Q_CORE_EXPORT QUnhandledException final : public QException
{
public:
    QUnhandledException(std::exception_ptr exception = nullptr) noexcept;
    ~QUnhandledException() noexcept override;

    QUnhandledException(QUnhandledException &&other) noexcept;
    QUnhandledException(const QUnhandledException &other) noexcept;

    void swap(QUnhandledException &other) noexcept { d.swap(other.d); }

    BOBUI_MOVE_ASSIGNMENT_OPERATOR_IMPL_VIA_PURE_SWAP(QUnhandledException)
    QUnhandledException &operator=(const QUnhandledException &other) noexcept;

    void raise() const override;
    QUnhandledException *clone() const override;

    std::exception_ptr exception() const;

private:
    QSharedDataPointer<QUnhandledExceptionPrivate> d;
};

namespace BobUIPrivate {

class Q_CORE_EXPORT ExceptionStore
{
public:
    void setException(const QException &e);
    void setException(std::exception_ptr e);
    bool hasException() const;
    std::exception_ptr exception() const;
    void throwPossibleException();
    Q_NORETURN void rethrowException() const;
    std::exception_ptr exceptionHolder;
};

} // namespace BobUIPrivate

#else // BOBUI_NO_EXCEPTIONS

namespace BobUIPrivate {

class Q_CORE_EXPORT ExceptionStore
{
public:
    ExceptionStore() { }
    inline void throwPossibleException() {}
    inline void rethrowException() const { }
};

} // namespace BobUIPrivate

#endif // BOBUI_NO_EXCEPTIONS

BOBUI_END_NAMESPACE

#endif
