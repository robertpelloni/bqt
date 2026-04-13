// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#ifndef QABSTRACTLAYOUTSTYLEINFO_P_H
#define QABSTRACTLAYOUTSTYLEINFO_P_H

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

#include <BobUIGui/private/bobuiguiglobal_p.h>
#include <BobUICore/qnamespace.h>
#include "qlayoutpolicy_p.h"

BOBUI_BEGIN_NAMESPACE


class Q_GUI_EXPORT QAbstractLayoutStyleInfo {
public:

    QAbstractLayoutStyleInfo() : m_isWindow(false) {}
    virtual ~QAbstractLayoutStyleInfo() {}
    virtual qreal combinedLayoutSpacing(QLayoutPolicy::ControlTypes /*controls1*/,
                                        QLayoutPolicy::ControlTypes /*controls2*/, BobUI::Orientation /*orientation*/) const {
        return -1;
    }

    virtual qreal perItemSpacing(QLayoutPolicy::ControlType /*control1*/,
                                 QLayoutPolicy::ControlType /*control2*/,
                                 BobUI::Orientation /*orientation*/) const {
        return -1;
    }

    virtual qreal spacing(BobUI::Orientation orientation) const = 0;

    virtual bool hasChangedCore() const { return false; }   // ### Remove when usage is gone from subclasses

    virtual void invalidate() { }

    virtual qreal windowMargin(BobUI::Orientation orientation) const = 0;

    bool isWindow() const {
        return m_isWindow;
    }

protected:
    unsigned m_isWindow : 1;
    mutable unsigned m_hSpacingState: 2;
    mutable unsigned m_vSpacingState: 2;
    mutable qreal m_spacing[2];
};

BOBUI_END_NAMESPACE

#endif // QABSTRACTLAYOUTSTYLEINFO_P_H
