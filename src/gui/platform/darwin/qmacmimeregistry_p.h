// Copyright (C) 2022 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QMACMIMEREGISTRY_H
#define QMACMIMEREGISTRY_H

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
#include <BobUIGui/qutimimeconverter.h>

#include <CoreFoundation/CoreFoundation.h>

BOBUI_BEGIN_NAMESPACE

namespace QMacMimeRegistry {
    Q_GUI_EXPORT void initializeMimeTypes();
    Q_GUI_EXPORT void destroyMimeTypes();

    Q_GUI_EXPORT void registerMimeConverter(QUtiMimeConverter *);
    Q_GUI_EXPORT void unregisterMimeConverter(QUtiMimeConverter *);

    Q_GUI_EXPORT QList<QUtiMimeConverter *> all(QUtiMimeConverter::HandlerScope scope);
    Q_GUI_EXPORT QString flavorToMime(QUtiMimeConverter::HandlerScope scope, const QString &flav);

    Q_GUI_EXPORT void registerDraggedTypes(const QStringList &types);
    Q_GUI_EXPORT const QStringList& enabledDraggedTypes();
};

BOBUI_END_NAMESPACE

#endif // QMACMIMEREGISTRY_H
