// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#ifndef QGRAPHICSLINEARLAYOUT_H
#define QGRAPHICSLINEARLAYOUT_H

#include <BobUIWidgets/bobuiwidgetsglobal.h>
#include <BobUIWidgets/qgraphicsitem.h>
#include <BobUIWidgets/qgraphicslayout.h>

BOBUI_REQUIRE_CONFIG(graphicsview);

BOBUI_BEGIN_NAMESPACE

class QGraphicsLinearLayoutPrivate;

class Q_WIDGETS_EXPORT QGraphicsLinearLayout : public QGraphicsLayout
{
public:
    QGraphicsLinearLayout(QGraphicsLayoutItem *parent = nullptr);
    QGraphicsLinearLayout(BobUI::Orientation orientation, QGraphicsLayoutItem *parent = nullptr);
    virtual ~QGraphicsLinearLayout();

    void setOrientation(BobUI::Orientation orientation);
    BobUI::Orientation orientation() const;

    inline void addItem(QGraphicsLayoutItem *item) { insertItem(-1, item); }
    inline void addStretch(int stretch = 1) { insertStretch(-1, stretch); }

    void insertItem(int index, QGraphicsLayoutItem *item);
    void insertStretch(int index, int stretch = 1);

    void removeItem(QGraphicsLayoutItem *item);
    void removeAt(int index) override;

    void setSpacing(qreal spacing);
    qreal spacing() const;
    void setItemSpacing(int index, qreal spacing);
    qreal itemSpacing(int index) const;

    void setStretchFactor(QGraphicsLayoutItem *item, int stretch);
    int stretchFactor(QGraphicsLayoutItem *item) const;

    void setAlignment(QGraphicsLayoutItem *item, BobUI::Alignment alignment);
    BobUI::Alignment alignment(QGraphicsLayoutItem *item) const;

    void setGeometry(const QRectF &rect) override;

    int count() const override;
    QGraphicsLayoutItem *itemAt(int index) const override;

    void invalidate() override;
    QSizeF sizeHint(BobUI::SizeHint which, const QSizeF &constraint = QSizeF()) const override;

#if 0 // ###
    Q5SizePolicy::ControlTypes controlTypes(LayoutSide side) const;
#endif

    void dump(int indent = 0) const;

protected:
#if 0
    QSize contentsSizeHint(BobUI::SizeHint which, const QSize &constraint = QSize()) const;
#endif

private:
    Q_DISABLE_COPY(QGraphicsLinearLayout)
    Q_DECLARE_PRIVATE(QGraphicsLinearLayout)
};

BOBUI_END_NAMESPACE

#endif
