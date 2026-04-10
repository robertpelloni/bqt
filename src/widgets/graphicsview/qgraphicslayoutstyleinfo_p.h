// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#ifndef QGRAPHICSLAYOUTSTYLEINFO_P_H
#define QGRAPHICSLAYOUTSTYLEINFO_P_H

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
#include <BobUIGui/private/qabstractlayoutstyleinfo_p.h>
#include <BobUIWidgets/qstyleoption.h>

#include <memory>

BOBUI_REQUIRE_CONFIG(graphicsview);

BOBUI_BEGIN_NAMESPACE

class QStyle;
class QWidget;
class QGraphicsLayoutPrivate;

class QGraphicsLayoutStyleInfo : public QAbstractLayoutStyleInfo
{
public:
    QGraphicsLayoutStyleInfo(const QGraphicsLayoutPrivate *layout);
    ~QGraphicsLayoutStyleInfo();

    virtual qreal combinedLayoutSpacing(QLayoutPolicy::ControlTypes controls1,
                                        QLayoutPolicy::ControlTypes controls2,
                                        BobUI::Orientation orientation) const override;

    virtual qreal perItemSpacing(QLayoutPolicy::ControlType control1,
                                 QLayoutPolicy::ControlType control2,
                                 BobUI::Orientation orientation) const override;

    virtual qreal spacing(BobUI::Orientation orientation) const override;

    virtual qreal windowMargin(BobUI::Orientation orientation) const override;

    virtual void invalidate() override
    {
        m_style = nullptr;
        QAbstractLayoutStyleInfo::invalidate();
    }

    QWidget *widget() const;
    QStyle *style() const;

private:
    const QGraphicsLayoutPrivate *m_layout;
    mutable QStyle *m_style;
    QStyleOption m_styleOption;
    std::unique_ptr<QWidget> m_widget;
};

BOBUI_END_NAMESPACE

#endif // QGRAPHICSLAYOUTSTYLEINFO_P_H
