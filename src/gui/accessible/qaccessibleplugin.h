// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QACCESSIBLEPLUGIN_H
#define QACCESSIBLEPLUGIN_H

#include <BobUIGui/bobuiguiglobal.h>
#include <BobUIGui/qaccessible.h>
#include <BobUICore/qfactoryinterface.h>

BOBUI_BEGIN_NAMESPACE


#if BOBUI_CONFIG(accessibility)

class QAccessibleInterface;

#define QAccessibleFactoryInterface_iid "org.bobui-project.BobUI.QAccessibleFactoryInterface"

class QAccessiblePluginPrivate;

class Q_GUI_EXPORT QAccessiblePlugin : public QObject
{
    Q_OBJECT
public:
    explicit QAccessiblePlugin(QObject *parent = nullptr);
    ~QAccessiblePlugin();

    virtual QAccessibleInterface *create(const QString &key, QObject *object) = 0;
};

#endif // BOBUI_CONFIG(accessibility)

BOBUI_END_NAMESPACE

#endif // QACCESSIBLEPLUGIN_H
