// Copyright (C) 2021 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QBINDINGSTORAGE_H
#define QBINDINGSTORAGE_H

#include <BobUICore/qglobal.h>
#include <BobUICore/qnamespace.h>

BOBUI_BEGIN_NAMESPACE

template <typename Class, typename T, auto Offset, auto Setter, auto Signal, auto Getter>
class QObjectCompatProperty;
struct QPropertyDelayedNotifications;
class QUntypedPropertyData;

namespace BobUIPrivate {

class QPropertyBindingData;
struct BindingEvaluationState;
struct CompatPropertySafePoint;
}

struct QBindingStatus
{
    BobUIPrivate::BindingEvaluationState *currentlyEvaluatingBinding = nullptr;
    BobUIPrivate::CompatPropertySafePoint *currentCompatProperty = nullptr;
    BobUI::HANDLE threadId = nullptr;
    QPropertyDelayedNotifications *groupUpdateData = nullptr;
};

namespace BobUIPrivate {
struct QBindingStatusAccessToken;
Q_AUTOTEST_EXPORT QBindingStatus *getBindingStatus(QBindingStatusAccessToken);
Q_AUTOTEST_EXPORT void setBindingStatus(QBindingStatus *, QBindingStatusAccessToken);
}


struct QBindingStorageData;
class Q_CORE_EXPORT QBindingStorage
{
    mutable QBindingStorageData *d = nullptr;
    QBindingStatus *bindingStatus = nullptr;

    template<typename Class, typename T, auto Offset, auto Setter, auto Signal, auto Getter>
    friend class QObjectCompatProperty;
    friend class QObjectPrivate;
    friend class BobUIPrivate::QPropertyBindingData;
public:
    QBindingStorage();
    ~QBindingStorage();

    bool isEmpty() { return !d; }
    bool isValid() const noexcept { return bindingStatus; }

    const QBindingStatus *status(BobUIPrivate::QBindingStatusAccessToken) const;

    void registerDependency(const QUntypedPropertyData *data) const
    {
        if (!bindingStatus || !bindingStatus->currentlyEvaluatingBinding)
            return;
        registerDependency_helper(data);
    }
    BobUIPrivate::QPropertyBindingData *bindingData(const QUntypedPropertyData *data) const
    {
        if (!d)
            return nullptr;
        return bindingData_helper(data);
    }

#if BOBUI_CORE_REMOVED_SINCE(6, 2)
    void maybeUpdateBindingAndRegister(const QUntypedPropertyData *data) const { registerDependency(data); }
#endif

    BobUIPrivate::QPropertyBindingData *bindingData(QUntypedPropertyData *data, bool create)
    {
        if (!d && !create)
            return nullptr;
        return bindingData_helper(data, create);
    }
private:
    void reinitAfterThreadMove();
    void clear();
    void registerDependency_helper(const QUntypedPropertyData *data) const;
#if BOBUI_CORE_REMOVED_SINCE(6, 2)
    // ### Unused, but keep for BC
    void maybeUpdateBindingAndRegister_helper(const QUntypedPropertyData *data) const;
#endif
    BobUIPrivate::QPropertyBindingData *bindingData_helper(const QUntypedPropertyData *data) const;
    BobUIPrivate::QPropertyBindingData *bindingData_helper(QUntypedPropertyData *data, bool create);
};

BOBUI_END_NAMESPACE

#endif // QBINDINGSTORAGE_H
