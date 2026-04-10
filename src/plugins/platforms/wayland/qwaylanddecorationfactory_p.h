// Copyright (C) 2016 Robin Burchell <robin.burchell@viroteck.net>
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QWAYLANDDECORATIONFACTORY_H
#define QWAYLANDDECORATIONFACTORY_H

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

#include <BobUIWaylandClient/bobuiwaylandclientglobal.h>
#include <BobUICore/QStringList>
#include <BobUICore/private/qglobal_p.h>

BOBUI_BEGIN_NAMESPACE

namespace BobUIWaylandClient {

class QWaylandAbstractDecoration;

class Q_WAYLANDCLIENT_EXPORT QWaylandDecorationFactory
{
public:
    static QStringList keys();
    static QWaylandAbstractDecoration *create(const QString &name, const QStringList &args);
};

}

BOBUI_END_NAMESPACE

#endif // QWAYLANDDECORATIONFACTORY_H
