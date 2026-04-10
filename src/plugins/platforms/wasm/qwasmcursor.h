// Copyright (C) 2018 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

#ifndef QWASMCURSOR_H
#define QWASMCURSOR_H

#include <qpa/qplatformcursor.h>

BOBUI_BEGIN_NAMESPACE

class QWasmCursor : public QPlatformCursor
{
public:
    void changeCursor(QCursor *windowCursor, QWindow *window) override;
};

BOBUI_END_NAMESPACE

#endif
