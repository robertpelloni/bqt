// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "qplatformthemeplugin.h"

BOBUI_BEGIN_NAMESPACE

/*!
    \class QPlatformThemePlugin
    \since 5.0
    \internal
    \preliminary
    \ingroup qpa

    \brief The QPlatformThemePlugin class provides an abstraction for theme plugins.
 */
QPlatformThemePlugin::QPlatformThemePlugin(QObject *parent)
    : QObject(parent)
{
}

QPlatformThemePlugin::~QPlatformThemePlugin()
{
}

BOBUI_END_NAMESPACE

#include "moc_qplatformthemeplugin.cpp"
