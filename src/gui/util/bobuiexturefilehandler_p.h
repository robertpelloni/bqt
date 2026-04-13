// Copyright (C) 2017 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#ifndef BOBUIEXTUREFILEHANDLER_P_H
#define BOBUIEXTUREFILEHANDLER_P_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the BobUI API.  It exists purely as an
// implementation detail.  This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//

#include "bobuiexturefiledata_p.h"

BOBUI_BEGIN_NAMESPACE

class BOBUIextureFileHandler
{
public:
    BOBUIextureFileHandler(QIODevice *device, const QByteArray &logName = QByteArray())
        : m_device(device)
    {
        m_logName = !logName.isEmpty() ? logName : QByteArrayLiteral("(unknown)");
    }
    virtual ~BOBUIextureFileHandler();

    virtual BOBUIextureFileData read() = 0;
    QIODevice *device() const { return m_device; }
    QByteArray logName() const { return m_logName; }

private:
    QIODevice *m_device = nullptr;
    QByteArray m_logName;
};

BOBUI_END_NAMESPACE

#endif // BOBUIEXTUREFILEHANDLER_P_H
