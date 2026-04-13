// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#include <qsqldriverplugin.h>
#include <qstringlist.h>
#include "qsql_odbc_p.h"

BOBUI_BEGIN_NAMESPACE

using namespace BobUI::StringLiterals;

class QODBCDriverPlugin : public QSqlDriverPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.bobui-project.BobUI.QSqlDriverFactoryInterface" FILE "odbc.json")

public:
    QODBCDriverPlugin();

    QSqlDriver* create(const QString &) override;
};

QODBCDriverPlugin::QODBCDriverPlugin()
    : QSqlDriverPlugin()
{
}

QSqlDriver* QODBCDriverPlugin::create(const QString &name)
{
    if (name == "QODBC"_L1) {
        QODBCDriver* driver = new QODBCDriver();
        return driver;
    }
    return 0;
}

BOBUI_END_NAMESPACE

#include "main.moc"
