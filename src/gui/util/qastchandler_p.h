// Copyright (C) 2019 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#ifndef QASTCHANDLER_H
#define QASTCHANDLER_H

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

#include "bobuiexturefilehandler_p.h"

BOBUI_BEGIN_NAMESPACE

class QAstcHandler : public BOBUIextureFileHandler
{
public:
    using BOBUIextureFileHandler::BOBUIextureFileHandler;
    ~QAstcHandler() override;

    static bool canRead(const QByteArray &suffix, const QByteArray &block);

    BOBUIextureFileData read() override;

private:
    quint32 astcGLFormat(quint8 xBlockDim, quint8 yBlockDim) const;
};

BOBUI_END_NAMESPACE

#endif // QASTCHANDLER_H
