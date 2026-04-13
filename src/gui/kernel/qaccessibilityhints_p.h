// Copyright (C) 2025 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QACCESSIBILITYHINTS_P_H
#define QACCESSIBILITYHINTS_P_H

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

#include "qaccessibilityhints.h"

#include <BobUICore/private/qobject_p.h>

BOBUI_BEGIN_NAMESPACE

class QAccessibilityHintsPrivate : public QObjectPrivate
{
    Q_DECLARE_PUBLIC(QAccessibilityHints)
public:
    void updateContrastPreference(BobUI::ContrastPreference contrastPreference);

    static QAccessibilityHintsPrivate *get(QAccessibilityHints *q);
private:
    BobUI::ContrastPreference m_contrastPreference = BobUI::ContrastPreference::NoPreference;
};

BOBUI_END_NAMESPACE

#endif // QACCESSIBILITYHINTS_P_H
