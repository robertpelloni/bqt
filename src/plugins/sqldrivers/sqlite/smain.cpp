// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#include <qsqldriverplugin.h>
#include <qstringlist.h>
#include "qsql_sqlite_p.h"
#include "qsql_sqlite_vfs_p.h"

BOBUI_BEGIN_NAMESPACE

using namespace BobUI::StringLiterals;

class QSQLiteDriverPlugin : public QSqlDriverPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.bobui-project.BobUI.QSqlDriverFactoryInterface" FILE "sqlite.json")

public:
    QSQLiteDriverPlugin();

    QSqlDriver* create(const QString &) override;
};

QSQLiteDriverPlugin::QSQLiteDriverPlugin()
    : QSqlDriverPlugin()
{
    register_bobui_vfs();
}

QSqlDriver* QSQLiteDriverPlugin::create(const QString &name)
{
    if (name == "QSQLITE"_L1) {
        QSQLiteDriver* driver = new QSQLiteDriver();
        return driver;
    }

    return nullptr;
}

BOBUI_END_NAMESPACE

#include "smain.moc"
