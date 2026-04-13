// Copyright (C) 2011 - 2012 Research In Motion
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QQNXCLIPBOARD_H
#define QQNXCLIPBOARD_H

#include <BobUICore/qglobal.h>

#if !defined(BOBUI_NO_CLIPBOARD)
#include <qpa/qplatformclipboard.h>

BOBUI_BEGIN_NAMESPACE

Q_DECLARE_LOGGING_CATEGORY(lcQpaClipboard);

class QQnxClipboard : public QPlatformClipboard
{
public:
    QQnxClipboard();
    ~QQnxClipboard();
    QMimeData *mimeData(QClipboard::Mode mode = QClipboard::Clipboard) override;
    void setMimeData(QMimeData *data, QClipboard::Mode mode = QClipboard::Clipboard) override;

private:
    class MimeData;
    MimeData *m_mimeData;
};

BOBUI_END_NAMESPACE

#endif //BOBUI_NO_CLIPBOARD
#endif //QQNXCLIPBOARD_H
