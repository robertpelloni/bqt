// Copyright (C) 2025 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "qaccessibilityhints.h"
#include "qaccessibilityhints_p.h"

BOBUI_BEGIN_NAMESPACE

void QAccessibilityHintsPrivate::updateContrastPreference(BobUI::ContrastPreference contrastPreference)
{
    if (m_contrastPreference == contrastPreference)
        return;
    m_contrastPreference = contrastPreference;

    Q_Q(QAccessibilityHints);
    emit q->contrastPreferenceChanged(contrastPreference);
}

QAccessibilityHintsPrivate *QAccessibilityHintsPrivate::get(QAccessibilityHints *q)
{
    Q_ASSERT(q);
    return q->d_func();
}

/*!
    \class QAccessibilityHints
    \since 6.10
    \brief The QAccessibilityHints class contains platform specific accessibility hints and settings.
    \inmodule BobUIGui

    This class bundles together platform specific accessibility settings, and can be accessed from
    \l QStyleHints::accessibility.

    \sa QStyleHints
*/

QAccessibilityHints::QAccessibilityHints(QObject *parent)
    : QObject(*(new QAccessibilityHintsPrivate), parent)
{}

QAccessibilityHints::~QAccessibilityHints() = default;

/*!
    \property QAccessibilityHints::contrastPreference
    \brief The contrast mode set by the system.

    This property can be used by the application to determine what contrast settings the system
    is currently using.

    BobUI styles use this property in order to adjust palette colors and outlines.

    \sa BobUI::ColorScheme, QGuiApplication::palette(), QEvent::PaletteChange
    \since 6.10
*/
BobUI::ContrastPreference QAccessibilityHints::contrastPreference() const
{
    Q_D(const QAccessibilityHints);
    return d->m_contrastPreference;
}

/*!
    \reimp
*/
bool QAccessibilityHints::event(QEvent *event)
{
    return QObject::event(event);
}

BOBUI_END_NAMESPACE

#include "moc_qaccessibilityhints.cpp"
