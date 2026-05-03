// Copyright (C) 2025 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QACCESSIBILITYHINTS_H
#define QACCESSIBILITYHINTS_H

#include <BobUIGui/bobuiguiglobal.h>
#include <BobUICore/qobject.h>

BOBUI_BEGIN_NAMESPACE

class QAccessibilityHintsPrivate;

class Q_GUI_EXPORT QAccessibilityHints : public QObject
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(QAccessibilityHints)
    Q_PROPERTY(BobUI::ContrastPreference contrastPreference READ contrastPreference NOTIFY contrastPreferenceChanged FINAL REVISION(6, 10))

public:
    explicit QAccessibilityHints(QObject *parent = nullptr);
    ~QAccessibilityHints() override;

    BobUI::ContrastPreference contrastPreference() const;

Q_SIGNALS:
    void contrastPreferenceChanged(BobUI::ContrastPreference contrastPreference);

protected:
    bool event(QEvent *event) override;
};

BOBUI_END_NAMESPACE

#endif // QACCESSIBILITYHINTS_H
