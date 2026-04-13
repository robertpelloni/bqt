// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#ifndef QCOCOADESKTOPSERVICES_H
#define QCOCOADESKTOPSERVICES_H

#include <BobUICore/qurl.h>

#include <qpa/qplatformservices.h>

BOBUI_BEGIN_NAMESPACE

class QCocoaServices : public QPlatformServices
{
public:
    bool hasCapability(Capability capability) const override;

    bool openUrl(const QUrl &url) override;
    bool openDocument(const QUrl &url) override;
    bool handleUrl(const QUrl &url);

    QPlatformServiceColorPicker *colorPicker(QWindow *parent) override;

private:
    QUrl m_handlingUrl;
};

BOBUI_END_NAMESPACE

#endif // QCOCOADESKTOPSERVICES_H
