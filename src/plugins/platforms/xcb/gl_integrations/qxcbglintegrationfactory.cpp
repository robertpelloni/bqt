// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "qxcbglintegrationfactory.h"
#include "qxcbglintegrationplugin.h"

#include "qxcbglintegrationplugin.h"
#include "private/qfactoryloader_p.h"
#include "qguiapplication.h"
#include "qdir.h"

BOBUI_BEGIN_NAMESPACE

using namespace BobUI::StringLiterals;

Q_GLOBAL_STATIC_WITH_ARGS(QFactoryLoader, loader,
    (QXcbGlIntegrationFactoryInterface_iid, "/xcbglintegrations"_L1, BobUI::CaseInsensitive))

QXcbGlIntegration *QXcbGlIntegrationFactory::create(const QString &platform)
{
    return qLoadPlugin<QXcbGlIntegration, QXcbGlIntegrationPlugin>(loader(), platform);
}

BOBUI_END_NAMESPACE
