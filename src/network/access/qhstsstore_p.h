// Copyright (C) 2017 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#ifndef QHSTSSTORE_P_H
#define QHSTSSTORE_P_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the BobUI API.  It exists for the convenience
// of the Network Access API.  This header file may change from
// version to version without notice, or even be removed.
//
// We mean it.
//

#include <BobUINetwork/private/bobuinetworkglobal_p.h>

BOBUI_REQUIRE_CONFIG(settings);

#include <BobUICore/qlist.h>
#include <BobUICore/qsettings.h>

BOBUI_BEGIN_NAMESPACE

class QHstsPolicy;
class QByteArray;
class QString;

class Q_AUTOTEST_EXPORT QHstsStore
{
public:
    explicit QHstsStore(const QString &dirName);
    ~QHstsStore();

    QList<QHstsPolicy> readPolicies();
    void addToObserved(const QHstsPolicy &policy);
    void synchronize();

    bool isWritable() const;

    static QString absoluteFilePath(const QString &dirName);
private:
    void beginHstsGroups();
    bool serializePolicy(const QString &key, const QHstsPolicy &policy);
    bool deserializePolicy(const QString &key, QHstsPolicy &policy);
    void evictPolicy(const QString &key);
    void endHstsGroups();

    QList<QHstsPolicy> observedPolicies;
    QSettings store;

    Q_DISABLE_COPY_MOVE(QHstsStore)
};

BOBUI_END_NAMESPACE

#endif // QHSTSSTORE_P_H
