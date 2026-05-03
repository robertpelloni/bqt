// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#ifndef QDYNAMICTOOLBARSEPARATOR_P_H
#define QDYNAMICTOOLBARSEPARATOR_P_H

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
#include "BobUIWidgets/qwidget.h"

BOBUI_REQUIRE_CONFIG(toolbar);

BOBUI_BEGIN_NAMESPACE

class QStyleOption;
class BOBUIoolBar;

class BOBUIoolBarSeparator : public QWidget
{
    Q_OBJECT
    BobUI::Orientation orient;

public:
    explicit BOBUIoolBarSeparator(BOBUIoolBar *parent);

    BobUI::Orientation orientation() const;

    QSize sizeHint() const override;

    void paintEvent(QPaintEvent *) override;
    void initStyleOption(QStyleOption *option) const;

public Q_SLOTS:
    void setOrientation(BobUI::Orientation orientation);
};

BOBUI_END_NAMESPACE

#endif // QDYNAMICTOOLBARSEPARATOR_P_H
