// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#ifndef QCOCOACLIPBOARD_H
#define QCOCOACLIPBOARD_H

#include <qpa/qplatformclipboard.h>

#ifndef BOBUI_NO_CLIPBOARD

#include "qmacclipboard.h"
#include <BobUICore/QScopedPointer>

BOBUI_BEGIN_NAMESPACE

class QCocoaClipboard : public QObject, public QPlatformClipboard
{
    Q_OBJECT

public:
    QCocoaClipboard();

    QMimeData *mimeData(QClipboard::Mode mode = QClipboard::Clipboard) override;
    void setMimeData(QMimeData *data, QClipboard::Mode mode = QClipboard::Clipboard) override;
    bool supportsMode(QClipboard::Mode mode) const override;
    bool ownsMode(QClipboard::Mode mode) const override;

private Q_SLOTS:
    void handleApplicationStateChanged(BobUI::ApplicationState state);

protected:
    QMacPasteboard *pasteboardForMode(QClipboard::Mode mode) const;

private:
    QScopedPointer<QMacPasteboard> m_clipboard;
    QScopedPointer<QMacPasteboard> m_find;
};

BOBUI_END_NAMESPACE

#endif // BOBUI_NO_CLIPBOARD

#endif // QCOCOACLIPBOARD_H
