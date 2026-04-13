// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QICONENGINEPLUGIN_H
#define QICONENGINEPLUGIN_H

#include <BobUIGui/bobuiguiglobal.h>
#include <BobUICore/qplugin.h>
#include <BobUICore/qfactoryinterface.h>

BOBUI_BEGIN_NAMESPACE


class QIconEngine;

#define QIconEngineFactoryInterface_iid "org.bobui-project.BobUI.QIconEngineFactoryInterface"

class Q_GUI_EXPORT QIconEnginePlugin : public QObject
{
    Q_OBJECT
public:
    QIconEnginePlugin(QObject *parent = nullptr);
    ~QIconEnginePlugin();

    virtual QIconEngine *create(const QString &filename = QString()) = 0;
};

BOBUI_END_NAMESPACE

#endif // QICONENGINEPLUGIN_H
