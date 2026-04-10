// Copyright (C) 2011 - 2014 BlackBerry Limited. All rights reserved.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "main.h"
#include "qqnxintegration.h"
#include "qqnxlgmon.h"

BOBUI_BEGIN_NAMESPACE

using namespace BobUI::StringLiterals;

QPlatformIntegration *QQnxIntegrationPlugin::create(const QString& system, const QStringList& paramList)
{
    if (!system.compare("qnx"_L1, BobUI::CaseInsensitive)) {
        qqnxLgmonInit();
        return new QQnxIntegration(paramList);
    }

    return 0;
}

BOBUI_END_NAMESPACE
