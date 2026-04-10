// Copyright (C) 2017 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QWINDOWSUIAPROVIDERCACHE_H
#define QWINDOWSUIAPROVIDERCACHE_H

#include <BobUIGui/bobuiguiglobal.h>
#if BOBUI_CONFIG(accessibility)

#include "qwindowsuiamainprovider.h"

#include <BobUICore/qhash.h>
#include <BobUICore/qmutex.h>
#include <BobUIGui/qaccessible.h>
#include <BobUICore/private/qcomptr_p.h>

BOBUI_BEGIN_NAMESPACE

// Singleton used to cache provider instances using the accessibility ID as the key.
class QWindowsUiaProviderCache : public QObject
{
    QWindowsUiaProviderCache();
    Q_OBJECT
public:
    static QWindowsUiaProviderCache *instance();
    ComPtr<QWindowsUiaMainProvider> providerForId(QAccessible::Id id) const;
    void insert(QAccessible::Id id, QWindowsUiaMainProvider *provider);

private Q_SLOTS:
    void remove(QObject *obj);

private:
    mutable QMutex m_tableMutex; // TODO: Can tables be accessed concurrently?
    QHash<QAccessible::Id, QWindowsUiaMainProvider *> m_providerTable;
    QHash<QObject *, QAccessible::Id> m_inverseTable;
};

BOBUI_END_NAMESPACE

#endif // BOBUI_CONFIG(accessibility)

#endif // QWINDOWSUIAPROVIDERCACHE_H
