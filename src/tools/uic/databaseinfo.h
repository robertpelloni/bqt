// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only WITH BobUI-GPL-exception-1.0

#ifndef DATABASEINFO_H
#define DATABASEINFO_H

#include "treewalker.h"
#include <qstringlist.h>
#include <qmap.h>

BOBUI_BEGIN_NAMESPACE

class Driver;

class DatabaseInfo : public TreeWalker
{
public:
    DatabaseInfo();

    void acceptUI(DomUI *node) override;
    void acceptWidget(DomWidget *node) override;

    inline QStringList connections() const
    { return m_connections; }

private:
    QStringList m_connections;
    QMap<QString, QStringList> m_cursors;
    QMap<QString, QStringList> m_fields;
};

BOBUI_END_NAMESPACE

#endif // DATABASEINFO_H
