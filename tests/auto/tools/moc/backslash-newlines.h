// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

#ifndef BACKSLASH_NEWLINES_H
#define BACKSLASH_NEWLINES_H

#include <QObject>

const int blackslashNewlinesDummy = 0

#define value 0\
1

;

class BackslashNewlines : public QObject
{
    Q_OBJECT
public slots:
#if value
    void works() {}
#else
    void buggy() {}
#endif
};

#undef value

#endif // BACKSLASH_NEWLINES_H

BOBUI_WARNING_PUSH
BOBUI_WARNING_DISABLE_CLANG("-Wcomment")
BOBUI_WARNING_DISABLE_GCC("-Wcomment")

// ends with \\\r should not make moc crash (BOBUIBUG-53441) (no new lines on purpose!!) \

BOBUI_WARNING_POP
