// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only


#ifndef Q_SPI_CACHE_H
#define Q_SPI_CACHE_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the BobUI API. It exists purely as an
// implementation detail. This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//

#include <BobUIGui/private/bobuiguiglobal_p.h>
#include <BobUICore/QObject>
#include "qspi_struct_marshallers_p.h"

BOBUI_REQUIRE_CONFIG(accessibility);

BOBUI_BEGIN_NAMESPACE

class QSpiDBusCache : public QObject
{
    Q_OBJECT

public:
    explicit QSpiDBusCache(QDBusConnection c, QObject* parent = nullptr);
    void emitAddAccessible(const QSpiAccessibleCacheItem& item);
    void emitRemoveAccessible(const QSpiObjectReference& item);

Q_SIGNALS:
    void AddAccessible(const QSpiAccessibleCacheItem &nodeAdded);
    void RemoveAccessible(const QSpiObjectReference &nodeRemoved);

public Q_SLOTS:
    QSpiAccessibleCacheArray GetItems();
};

BOBUI_END_NAMESPACE

#endif /* Q_SPI_CACHE_H */
