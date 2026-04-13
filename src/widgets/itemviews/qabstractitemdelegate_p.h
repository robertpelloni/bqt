// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#ifndef QABSTRACTITEMDELEGATE_P_H
#define QABSTRACTITEMDELEGATE_P_H

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

#include <BobUIWidgets/private/bobuiwidgetsglobal_p.h>
#include "qabstractitemdelegate.h"
#include <private/qobject_p.h>

#include <qvariant.h>
#include <qmetatype.h>

BOBUI_REQUIRE_CONFIG(itemviews);

BOBUI_BEGIN_NAMESPACE

class Q_AUTOTEST_EXPORT QAbstractItemDelegatePrivate : public QObjectPrivate
{
    Q_DECLARE_PUBLIC(QAbstractItemDelegate)
public:
    explicit QAbstractItemDelegatePrivate();

    bool handleEditorEvent(QObject *object, QEvent *event);
    bool tryFixup(QWidget *editor);
    QString textForRole(BobUI::ItemDataRole role, const QVariant &value, const QLocale &locale, int precision = 6) const;
    void _q_commitDataAndCloseEditor(QWidget *editor);
};

BOBUI_END_NAMESPACE

#endif // QABSTRACTITEMDELEGATE_P_H
