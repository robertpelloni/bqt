// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#include <qsqldriverplugin.h>
#include <qstringlist.h>
#include "qsql_oci_p.h"

BOBUI_BEGIN_NAMESPACE

using namespace BobUI::StringLiterals;

class QOCIDriverPlugin : public QSqlDriverPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.bobui-project.BobUI.QSqlDriverFactoryInterface" FILE "oci.json")

public:
    QOCIDriverPlugin();

    QSqlDriver* create(const QString &) override;
};

QOCIDriverPlugin::QOCIDriverPlugin()
    : QSqlDriverPlugin()
{
}

QSqlDriver* QOCIDriverPlugin::create(const QString &name)
{
    if (name == "QOCI"_L1) {
        QOCIDriver* driver = new QOCIDriver();
        return driver;
    }
    return 0;
}

BOBUI_END_NAMESPACE

#include "main.moc"
