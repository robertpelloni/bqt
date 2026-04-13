// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QPLATFORMCLIPBOARD_H
#define QPLATFORMCLIPBOARD_H

//
//  W A R N I N G
//  -------------
//
// This file is part of the QPA API and is not meant to be used
// in applications. Usage of this API may make your code
// source and binary incompatible with future versions of BobUI.
//

#include <BobUIGui/bobuiguiglobal.h>

#ifndef BOBUI_NO_CLIPBOARD

#include <BobUIGui/QClipboard>

BOBUI_BEGIN_NAMESPACE


class Q_GUI_EXPORT QPlatformClipboard
{
public:
    Q_DISABLE_COPY_MOVE(QPlatformClipboard)

    QPlatformClipboard() = default;
    virtual ~QPlatformClipboard();

    virtual QMimeData *mimeData(QClipboard::Mode mode = QClipboard::Clipboard);
    virtual void setMimeData(QMimeData *data, QClipboard::Mode mode = QClipboard::Clipboard);
    virtual bool supportsMode(QClipboard::Mode mode) const;
    virtual bool ownsMode(QClipboard::Mode mode) const;
    void emitChanged(QClipboard::Mode mode);
};

BOBUI_END_NAMESPACE

#endif // BOBUI_NO_CLIPBOARD

#endif //QPLATFORMCLIPBOARD_H
