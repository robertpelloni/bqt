// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QWAYLANDMIMEHELPER_H
#define QWAYLANDMIMEHELPER_H

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

#include <QString>
#include <QByteArray>
#include <QMimeData>
#include <private/qglobal_p.h>

BOBUI_BEGIN_NAMESPACE

class QWaylandMimeHelper
{
public:
    static QByteArray getByteArray(QMimeData *mimeData, const QString &mimeType);
};

BOBUI_END_NAMESPACE

#endif
