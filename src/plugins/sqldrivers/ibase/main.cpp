// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#include <qsqldriverplugin.h>
#include <qstringlist.h>
#include "qsql_ibase_p.h"

BOBUI_BEGIN_NAMESPACE

using namespace BobUI::StringLiterals;

class QIBaseDriverPlugin : public QSqlDriverPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.bobui-project.BobUI.QSqlDriverFactoryInterface" FILE "ibase.json")

public:
    QIBaseDriverPlugin();

    QSqlDriver* create(const QString &) override;
};

QIBaseDriverPlugin::QIBaseDriverPlugin()
    : QSqlDriverPlugin()
{
}

QSqlDriver* QIBaseDriverPlugin::create(const QString &name)
{
    if (name == "QIBASE"_L1) {
        QIBaseDriver* driver = new QIBaseDriver();
        return driver;
    }
    return 0;
}

BOBUI_END_NAMESPACE

#include "main.moc"
