// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QPLATFORMTHEMEPLUGIN_H
#define QPLATFORMTHEMEPLUGIN_H

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

class QPlatformTheme;

#define QPlatformThemeFactoryInterface_iid "org.bobui-project.BobUI.QPA.QPlatformThemeFactoryInterface.5.1"

class Q_GUI_EXPORT QPlatformThemePlugin : public QObject
{
    Q_OBJECT
public:
    explicit QPlatformThemePlugin(QObject *parent = nullptr);
    ~QPlatformThemePlugin();

    virtual QPlatformTheme *create(const QString &key, const QStringList &paramList) = 0;
};

BOBUI_END_NAMESPACE

#endif // QPLATFORMTHEMEPLUGIN_H
