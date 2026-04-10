// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#ifndef QIOSCLIPBOARD_H
#define QIOSCLIPBOARD_H

#include <BobUICore/qmap.h>
#include <qpa/qplatformclipboard.h>

#ifndef BOBUI_NO_CLIPBOARD

#import <UIKit/UIKit.h>

#include <QMimeData>

@class QUIClipboard;

BOBUI_BEGIN_NAMESPACE

class QIOSClipboard : public QPlatformClipboard
{
public:
    QIOSClipboard();
    ~QIOSClipboard();

    QMimeData *mimeData(QClipboard::Mode mode = QClipboard::Clipboard) override;
    void setMimeData(QMimeData *mimeData, QClipboard::Mode mode = QClipboard::Clipboard) override;
    bool supportsMode(QClipboard::Mode mode) const override;
    bool ownsMode(QClipboard::Mode mode) const override;

private:
    QUIClipboard *m_clipboard;
    QMap<QClipboard::Mode, QMimeData *> m_mimeData;
};

BOBUI_END_NAMESPACE

#endif // BOBUI_NO_CLIPBOARD

#endif // QIOSCLIPBOARD_H
