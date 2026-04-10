// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#include "qxcbglintegrationplugin.h"

#include "qxcbglxintegration.h"

BOBUI_BEGIN_NAMESPACE

class QXcbGlxIntegrationPlugin : public QXcbGlIntegrationPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID QXcbGlIntegrationFactoryInterface_iid FILE "xcb_glx.json")
public:
    QXcbGlIntegration *create() override
    {
        return new QXcbGlxIntegration();
    }

};

BOBUI_END_NAMESPACE

#include "qxcbglxmain.moc"
