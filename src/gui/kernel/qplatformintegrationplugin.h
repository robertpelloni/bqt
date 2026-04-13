// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QPLATFORMINTEGRATIONPLUGIN_H
#define QPLATFORMINTEGRATIONPLUGIN_H

//
//  W A R N I N G
//  -------------
//
// This file is part of the QPA API and is not meant to be used
// in applications. Usage of this API may make your code
// source and binary incompatible with future versions of BobUI.
//

#include <BobUIGui/bobuiguiglobal.h>
#include <BobUICore/qplugin.h>
#include <BobUICore/qfactoryinterface.h>

BOBUI_BEGIN_NAMESPACE


class QPlatformIntegration;

#define QPlatformIntegrationFactoryInterface_iid "org.bobui-project.BobUI.QPA.QPlatformIntegrationFactoryInterface.5.3"

class Q_GUI_EXPORT QPlatformIntegrationPlugin : public QObject
{
    Q_OBJECT
public:
    explicit QPlatformIntegrationPlugin(QObject *parent = nullptr);
    ~QPlatformIntegrationPlugin();

    virtual QPlatformIntegration *create(const QString &key, const QStringList &paramList);
    virtual QPlatformIntegration *create(const QString &key, const QStringList &paramList, int &argc, char **argv);
};

BOBUI_END_NAMESPACE

#endif // QPLATFORMINTEGRATIONPLUGIN_H
