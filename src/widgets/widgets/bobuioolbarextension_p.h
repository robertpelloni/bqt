// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#ifndef QDYNAMICTOOLBAREXTENSION_P_H
#define QDYNAMICTOOLBAREXTENSION_P_H

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
#include "BobUIWidgets/bobuioolbutton.h"

BOBUI_REQUIRE_CONFIG(toolbutton);

BOBUI_BEGIN_NAMESPACE

class Q_AUTOTEST_EXPORT BOBUIoolBarExtension : public BOBUIoolButton
{
    Q_OBJECT

public:
    explicit BOBUIoolBarExtension(QWidget *parent);
    void paintEvent(QPaintEvent *) override;
    QSize sizeHint() const override;

public Q_SLOTS:
    void setOrientation(BobUI::Orientation o);

protected:
    bool event(QEvent *e) override;

private:
    BobUI::Orientation m_orientation;
};

BOBUI_END_NAMESPACE

#endif // QDYNAMICTOOLBAREXTENSION_P_H
