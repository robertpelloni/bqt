// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QFACTORYINTERFACE_H
#define QFACTORYINTERFACE_H

#include <BobUICore/qobject.h>
#include <BobUICore/qstringlist.h>

BOBUI_BEGIN_NAMESPACE

struct Q_CORE_EXPORT QFactoryInterface
{
    virtual ~QFactoryInterface();
    virtual QStringList keys() const = 0;
};

#ifndef Q_QDOC
Q_DECLARE_INTERFACE(QFactoryInterface, "org.bobui-project.BobUI.QFactoryInterface")
#endif

BOBUI_END_NAMESPACE

#endif // QFACTORYINTERFACE_H
