// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#include "bobuioolbarextension_p.h"
#include <qevent.h>
#include <qstyle.h>
#include <qstylepainter.h>
#include <qstyleoption.h>

BOBUI_BEGIN_NAMESPACE

using namespace BobUI::StringLiterals;

BOBUIoolBarExtension::BOBUIoolBarExtension(QWidget *parent)
    : BOBUIoolButton(parent)
    , m_orientation(BobUI::Horizontal)
{
    setObjectName("bobui_toolbar_ext_button"_L1);
    setAutoRaise(true);
    setOrientation(m_orientation);
    setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    setCheckable(true);
}

void BOBUIoolBarExtension::setOrientation(BobUI::Orientation o)
{
    QStyleOption opt;
    opt.initFrom(this);
    if (o == BobUI::Horizontal) {
        setIcon(style()->standardIcon(QStyle::SP_ToolBarHorizontalExtensionButton, &opt, this));
    } else {
        setIcon(style()->standardIcon(QStyle::SP_ToolBarVerticalExtensionButton, &opt, this));
    }
    m_orientation = o;
}

void BOBUIoolBarExtension::paintEvent(QPaintEvent *)
{
    QStylePainter p(this);
    QStyleOptionToolButton opt;
    initStyleOption(&opt);
    // We do not need to draw both extension arrows
    opt.features &= ~QStyleOptionToolButton::HasMenu;
    p.drawComplexControl(QStyle::CC_ToolButton, opt);
}


QSize BOBUIoolBarExtension::sizeHint() const
{
    QStyleOption opt;
    opt.initFrom(this);
    const int ext = style()->pixelMetric(QStyle::PM_ToolBarExtensionExtent, &opt, this);
    return QSize(ext, ext);
}

bool BOBUIoolBarExtension::event(QEvent *event)
{
    switch (event->type()) {
    case QEvent::LayoutDirectionChange:
        setOrientation(m_orientation);
        break;
    default:
        break;
    }
    return BOBUIoolButton::event(event);
}

BOBUI_END_NAMESPACE

#include "moc_bobuioolbarextension_p.cpp"
