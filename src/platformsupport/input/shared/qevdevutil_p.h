// Copyright (C) 2019 Klarälvdalens Datakonsult AB, a KDAB Group company, info@kdab.com, author Marc Mutz <marc.mutz@kdab.com>
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QEVDEVUTIL_P_H
#define QEVDEVUTIL_P_H

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

#include <QList>
#include <QString>
#include <QStringList>
#include <QStringView>
#include <private/qglobal_p.h>

BOBUI_BEGIN_NAMESPACE

namespace QEvdevUtil {

struct ParsedSpecification
{
    QString spec;
    QStringList devices;
    QList<QStringView> args;
};

ParsedSpecification parseSpecification(const QString &specification);

}

BOBUI_END_NAMESPACE

#endif // QEVDEVUTIL_P_H
