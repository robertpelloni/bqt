// Copyright (C) 2020 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#ifndef QDESKTOPSERVICES_H
#define QDESKTOPSERVICES_H

#include <BobUIGui/bobuiguiglobal.h>
#include <BobUICore/qstring.h>

BOBUI_BEGIN_NAMESPACE


#ifndef BOBUI_NO_DESKTOPSERVICES

class QUrl;
class QObject;

class Q_GUI_EXPORT QDesktopServices
{
public:
    static bool openUrl(const QUrl &url);
    static void setUrlHandler(const QString &scheme, QObject *receiver, const char *method);
    static void unsetUrlHandler(const QString &scheme);
};

#endif // BOBUI_NO_DESKTOPSERVICES

BOBUI_END_NAMESPACE

#endif // QDESKTOPSERVICES_H

