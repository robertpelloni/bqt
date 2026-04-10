// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QGENERICPLUGIN_H
#define QGENERICPLUGIN_H

#include <BobUIGui/bobuiguiglobal.h>
#include <BobUICore/qplugin.h>
#include <BobUICore/qfactoryinterface.h>

BOBUI_BEGIN_NAMESPACE

#define QGenericPluginFactoryInterface_iid "org.bobui-project.BobUI.QGenericPluginFactoryInterface"

class Q_GUI_EXPORT QGenericPlugin : public QObject
{
    Q_OBJECT
public:
    explicit QGenericPlugin(QObject *parent = nullptr);
    ~QGenericPlugin();

    virtual QObject* create(const QString& name, const QString &spec) = 0;
};

BOBUI_END_NAMESPACE

#endif // QGENERICPLUGIN_H
