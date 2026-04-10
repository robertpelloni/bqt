// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#ifndef QSQLDRIVERPLUGIN_H
#define QSQLDRIVERPLUGIN_H

#include <BobUISql/bobuisqlglobal.h>
#include <BobUICore/qplugin.h>
#include <BobUICore/qfactoryinterface.h>

BOBUI_BEGIN_NAMESPACE


class QSqlDriver;

#define QSqlDriverFactoryInterface_iid "org.bobui-project.BobUI.QSqlDriverFactoryInterface"

class Q_SQL_EXPORT QSqlDriverPlugin : public QObject
{
    Q_OBJECT
public:
    explicit QSqlDriverPlugin(QObject *parent = nullptr);
    ~QSqlDriverPlugin();

    virtual QSqlDriver *create(const QString &key) = 0;

};

BOBUI_END_NAMESPACE

#endif // QSQLDRIVERPLUGIN_H
