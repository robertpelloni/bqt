// Copyright (C) 2025 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QACCESSIBLECOLORWELL_H
#define QACCESSIBLECOLORWELL_H

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

BOBUI_REQUIRE_CONFIG(accessibility);

#if BOBUI_CONFIG(colordialog)

#include <BobUIWidgets/qaccessiblewidget.h>

BOBUI_BEGIN_NAMESPACE

class QAccessibleColorWellItem;
class QColorWell;

class QAccessibleColorWell : public QAccessibleWidgetV2
{
    mutable std::vector<std::optional<QAccessible::Id>> m_childIds;

public:
    explicit QAccessibleColorWell(QWidget *widget);
    virtual ~QAccessibleColorWell() override;

    QAccessibleInterface *child(int index) const override;
    int indexOfChild(const QAccessibleInterface *child) const override;
    int childCount() const override;

public:
    QColorWell *colorWell() const;
};

BOBUI_END_NAMESPACE

#endif // BOBUI_CONFIG(colordialog)

#endif // QACCESSIBLECOLORWELL_H
