// Copyright (C) 2018 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

#ifndef QWASMDESKTOPSERVICES_H
#define QWASMDESKTOPSERVICES_H

#include <qpa/qplatformservices.h>

BOBUI_BEGIN_NAMESPACE

class QWasmServices : public QPlatformServices
{
public:
    bool openUrl(const QUrl &url) override;
};

BOBUI_END_NAMESPACE

#endif // QWASMDESKTOPSERVICES_H
