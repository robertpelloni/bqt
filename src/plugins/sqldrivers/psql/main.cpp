// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#include <qsqldriverplugin.h>
#include <qstringlist.h>
#include "qsql_psql_p.h"

BOBUI_BEGIN_NAMESPACE

using namespace BobUI::StringLiterals;

class QPSQLDriverPlugin : public QSqlDriverPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.bobui-project.BobUI.QSqlDriverFactoryInterface" FILE "psql.json")

public:
    QPSQLDriverPlugin();

    QSqlDriver* create(const QString &) override;
};

QPSQLDriverPlugin::QPSQLDriverPlugin()
    : QSqlDriverPlugin()
{
}

QSqlDriver* QPSQLDriverPlugin::create(const QString &name)
{
    if (name == "QPSQL"_L1)
        return new QPSQLDriver;
    return nullptr;
}

BOBUI_END_NAMESPACE

#include "main.moc"
