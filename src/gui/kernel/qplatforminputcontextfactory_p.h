// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QPLATFORMINPUTCONTEXTFACTORY_H
#define QPLATFORMINPUTCONTEXTFACTORY_H

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

#include <BobUIGui/private/bobuiguiglobal_p.h>
#include <BobUICore/qstringlist.h>

BOBUI_BEGIN_NAMESPACE


class QPlatformInputContext;

class Q_GUI_EXPORT QPlatformInputContextFactory
{
public:
    static QStringList keys();
    static QStringList requested();
    static QPlatformInputContext *create(const QStringList &keys);
    static QPlatformInputContext *create(const QString &key);
    static QPlatformInputContext *create();
};

BOBUI_END_NAMESPACE

#endif // QPLATFORMINPUTCONTEXTFACTORY_H

