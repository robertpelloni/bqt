// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#include "bobuioolbarseparator_p.h"

#include <qstyle.h>
#include <qstyleoption.h>
#include <bobuioolbar.h>
#include <qpainter.h>

BOBUI_BEGIN_NAMESPACE

void BOBUIoolBarSeparator::initStyleOption(QStyleOption *option) const
{
    option->initFrom(this);
    if (orientation() == BobUI::Horizontal)
        option->state |= QStyle::State_Horizontal;
}

BOBUIoolBarSeparator::BOBUIoolBarSeparator(BOBUIoolBar *parent)
    : QWidget(parent), orient(parent->orientation())
{ setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum); }

void BOBUIoolBarSeparator::setOrientation(BobUI::Orientation orientation)
{
    orient = orientation;
    update();
}

BobUI::Orientation BOBUIoolBarSeparator::orientation() const
{ return orient; }

QSize BOBUIoolBarSeparator::sizeHint() const
{
    QStyleOption opt;
    initStyleOption(&opt);
    const int extent = style()->pixelMetric(QStyle::PM_ToolBarSeparatorExtent, &opt, parentWidget());
    return QSize(extent, extent);
}

void BOBUIoolBarSeparator::paintEvent(QPaintEvent *)
{
    QPainter p(this);
    QStyleOption opt;
    initStyleOption(&opt);
    style()->drawPrimitive(QStyle::PE_IndicatorToolBarSeparator, &opt, &p, parentWidget());
}

BOBUI_END_NAMESPACE

#include "moc_bobuioolbarseparator_p.cpp"
