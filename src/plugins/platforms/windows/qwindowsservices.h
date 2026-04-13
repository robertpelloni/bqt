// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QWINDOWSDESKTOPSERVICES_H
#define QWINDOWSDESKTOPSERVICES_H

#include <qpa/qplatformservices.h>

BOBUI_BEGIN_NAMESPACE

class QWindowsServices : public QPlatformServices
{
public:
    bool openUrl(const QUrl &url) override;
    bool openDocument(const QUrl &url) override;
};

BOBUI_END_NAMESPACE

#endif // QWINDOWSDESKTOPSERVICES_H
