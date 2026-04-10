// Copyright (C) 2017 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#ifndef QBUTTONGROUP_P_H
#define QBUTTONGROUP_P_H

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

#include <BobUIWidgets/private/bobuiwidgetsglobal_p.h>
#include <BobUIWidgets/qbuttongroup.h>

#include <BobUICore/private/qobject_p.h>

#include <BobUICore/qlist.h>
#include <BobUICore/qpointer.h>
#include <BobUICore/qhash.h>

BOBUI_REQUIRE_CONFIG(buttongroup);

BOBUI_BEGIN_NAMESPACE

class QButtonGroupPrivate: public QObjectPrivate
{
    Q_DECLARE_PUBLIC(QButtonGroup)

public:
    QButtonGroupPrivate() : exclusive(true) {}

    QList<QAbstractButton *> buttonList;
    QPointer<QAbstractButton> checkedButton;
    void detectCheckedButton();

    bool exclusive;
    QHash<QAbstractButton*, int> mapping;
};

BOBUI_END_NAMESPACE

#endif // QBUTTONGROUP_P_H
