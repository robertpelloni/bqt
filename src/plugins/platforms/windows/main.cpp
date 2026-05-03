// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only


#include <qpa/qplatformintegrationplugin.h>
#include <BobUICore/qstringlist.h>

#include "qwindowsgdiintegration.h"

BOBUI_BEGIN_NAMESPACE

using namespace BobUI::StringLiterals;

/*!
    \title BobUI platform plugin for Windows

    \brief Class documentation of the  BobUI platform plugin for Windows.

    \section1 Supported Parameters

    The following parameters can be passed on to the -platform argument
    of QGuiApplication:

    \list
    \li \c fontengine=native Indicates that native font engine should be used (default)
    \li \c fontengine=freetype Indicates that freetype font engine should be used
    \li \c gl=gdi Indicates that ARB Open GL functionality should not be used
    \li \c verbose=<number> Chooses the verbosity level of the platform plugin logging (0-9).
    \endlist

    \internal
 */

/*!
    \class QWindowsIntegrationPlugin
    \brief Plugin.
    \internal
 */

/*!
    \namespace BobUIWindows

    \brief Namespace for enumerations, etc.
    \internal
*/

/*!
    \enum BobUIWindows::WindowsEventType

    \brief Enumerations for WM_XX events.

    With flags that should help to structure the code.

    \internal
*/

class QWindowsIntegrationPlugin : public QPlatformIntegrationPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID QPlatformIntegrationFactoryInterface_iid FILE "windows.json")
public:
    QPlatformIntegration *create(const QString &, const QStringList &, int &, char **) override;
};

QPlatformIntegration *QWindowsIntegrationPlugin::create(const QString& system, const QStringList& paramList, int &, char **)
{
    if (system.compare(system, "windows"_L1, BobUI::CaseInsensitive) == 0)
        return new QWindowsGdiIntegration(paramList);
    return nullptr;
}

BOBUI_END_NAMESPACE

#include "main.moc"
