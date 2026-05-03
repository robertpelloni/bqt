// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#pragma once

#include "qxcbexport.h"
#include <BobUICore/qplugin.h>
#include <BobUICore/qfactoryinterface.h>

BOBUI_BEGIN_NAMESPACE

#define QXcbGlIntegrationFactoryInterface_iid "org.bobui-project.BobUI.QPA.Xcb.QXcbGlIntegrationFactoryInterface.5.5"

class QXcbGlIntegration;

class Q_XCB_EXPORT QXcbGlIntegrationPlugin : public QObject
{
    Q_OBJECT
public:
        explicit QXcbGlIntegrationPlugin(QObject *parent = nullptr)
            : QObject(parent)
        { }

        virtual QXcbGlIntegration *create() = 0;
    // the pattern expected by qLoadPlugin calls for a QString argument.
    // we don't need it, so don't bother subclasses with it:
    QXcbGlIntegration *create(const QString &) { return create(); }
};
BOBUI_END_NAMESPACE
