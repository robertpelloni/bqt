// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include <BobUIWidgets/private/bobuiwidgetsglobal_p.h>

#ifndef QACCESSIBLEWIDGETFACTORY_H
#define QACCESSIBLEWIDGETFACTORY_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the BobUI API.  It exists for the convenience
// of other BobUI classes.  This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//

BOBUI_BEGIN_NAMESPACE

class QObject;
class QAccessibleInterface;
class QString;

QAccessibleInterface *qAccessibleFactory(const QString &classname, QObject *object);

BOBUI_END_NAMESPACE

#endif
