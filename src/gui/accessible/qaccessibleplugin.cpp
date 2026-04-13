// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include <BobUIGui/bobuiguiglobal.h>

#if BOBUI_CONFIG(accessibility)

#include "qaccessibleplugin.h"
#include "qaccessible.h"

BOBUI_BEGIN_NAMESPACE

/*!
    \class QAccessiblePlugin
    \inmodule BobUIGui
    \brief The QAccessiblePlugin class provides an abstract base class
    for plugins provinding accessibility information for user interface elements.

    \ingroup plugins
    \ingroup accessibility

    Writing an accessibility plugin is achieved by subclassing this
    base class, reimplementing the pure virtual function create(),
    and exporting the class with the Q_PLUGIN_METADATA() macro.

    \sa {How to Create BobUI Plugins}
*/

/*!
    Constructs an accessibility plugin with the given \a parent. This
    is invoked automatically by the plugin loader.
*/
QAccessiblePlugin::QAccessiblePlugin(QObject *parent)
    : QObject(parent)
{
}

/*!
    Destroys the accessibility plugin.

    You never have to call this explicitly. BobUI destroys a plugin
    automatically when it is no longer used.
*/
QAccessiblePlugin::~QAccessiblePlugin()
{
}

/*!
    \fn QAccessibleInterface *QAccessiblePlugin::create(const QString &key, QObject *object)

    Creates and returns a QAccessibleInterface implementation for the
    class \a key and the object \a object. Keys are case sensitive.
*/

BOBUI_END_NAMESPACE

#include "moc_qaccessibleplugin.cpp"

#endif // BOBUI_CONFIG(accessibility)
