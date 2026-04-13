// Copyright (C) 2020 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#ifndef QPROMISE_H
#define QPROMISE_H

#include <BobUICore/qglobal.h>
#include <BobUICore/qfutureinterface.h>

#include <utility>

BOBUI_REQUIRE_CONFIG(future);

BOBUI_BEGIN_NAMESPACE

namespace BobUIPrivate {

template<class T, class U>
using EnableIfSameOrConvertible = std::enable_if_t<std::is_convertible_v<T, U>>;

} // namespace BobUIPrivate

template<typename T>
class QPromise
{
    static_assert (std::is_move_constructible_v<T>
                   || std::is_same_v<T, void>,
                   "A move-constructible type or type void is required");
public:
    QPromise() = default;
    Q_DISABLE_COPY(QPromise)
    QPromise(QPromise<T> &&other) = default;
    QPromise(const QFutureInterface<T> &other) : d(other) {}
    QPromise(QFutureInterface<T> &&other) noexcept : d(std::move(other)) {}
    BOBUI_MOVE_ASSIGNMENT_OPERATOR_IMPL_VIA_MOVE_AND_SWAP(QPromise)
    ~QPromise()
    {
        // If computation is not finished at this point, cancel
        // potential waits
        if (d.d && !(d.loadState() & QFutureInterfaceBase::State::Finished)) {
            d.cancelAndFinish(); // cancel and finalize the state
            d.runContinuation();
        }
        d.cleanContinuation();
    }

    // Core QPromise APIs
    QFuture<T> future() const { return d.future(); }
    template<typename...Args, std::enable_if_t<std::is_constructible_v<T, Args...>, bool> = true>
    bool emplaceResultAt(int index, Args&&...args)
    {
        return d.reportAndEmplaceResult(index, std::forward<Args>(args)...);
    }
    template<typename...Args, std::enable_if_t<std::is_constructible_v<T, Args...>, bool> = true>
    bool emplaceResult(Args&&...args)
    {
        return d.reportAndEmplaceResult(-1, std::forward<Args>(args)...);
    }
    template<typename U = T, typename = BobUIPrivate::EnableIfSameOrConvertible<U, T>>
    bool addResult(U &&result, int index = -1)
    {
        return d.reportAndEmplaceResult(index, std::forward<U>(result));
    }
    bool addResults(const QList<T> &result)
    { return d.reportResults(result); }
#ifndef BOBUI_NO_EXCEPTIONS
    void setException(const QException &e) { d.reportException(e); }
#if BOBUI_VERSION < BOBUI_VERSION_CHECK(7, 0, 0)
    void setException(std::exception_ptr e) { d.reportException(e); }
#else
    void setException(const std::exception_ptr &e) { d.reportException(e); }
#endif
#endif
    void start() { d.reportStarted(); }
    void finish() { d.reportFinished(); }

    void suspendIfRequested() { d.suspendIfRequested(); }

    bool isCanceled() const { return d.isCanceled(); }

    // Progress methods
    void setProgressRange(int minimum, int maximum) { d.setProgressRange(minimum, maximum); }
    void setProgressValue(int progressValue) { d.setProgressValue(progressValue); }
    void setProgressValueAndText(int progressValue, const QString &progressText)
    {
        d.setProgressValueAndText(progressValue, progressText);
    }

    void swap(QPromise<T> &other) noexcept
    {
        d.swap(other.d);
    }

#if defined(Q_QDOC)  // documentation-only simplified signatures
    bool addResult(const T &result, int index = -1) { }
    bool addResult(T &&result, int index = -1) { }
#endif
private:
    mutable QFutureInterface<T> d;
};

template<typename T>
inline void swap(QPromise<T> &a, QPromise<T> &b) noexcept
{
    a.swap(b);
}

BOBUI_END_NAMESPACE

#endif  // QPROMISE_H
