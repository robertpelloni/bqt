// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#ifndef QGRAPHICSLAYOUT_H
#define QGRAPHICSLAYOUT_H

#include <BobUIWidgets/bobuiwidgetsglobal.h>
#include <BobUIWidgets/qgraphicslayoutitem.h>

BOBUI_REQUIRE_CONFIG(graphicsview);

BOBUI_BEGIN_NAMESPACE

class QGraphicsLayoutPrivate;
class QGraphicsLayoutItem;
class QGraphicsWidget;

class Q_WIDGETS_EXPORT QGraphicsLayout : public QGraphicsLayoutItem
{
public:
    QGraphicsLayout(QGraphicsLayoutItem *parent = nullptr);
    ~QGraphicsLayout();

    void setContentsMargins(qreal left, qreal top, qreal right, qreal bottom);
    void getContentsMargins(qreal *left, qreal *top, qreal *right, qreal *bottom) const override;

    void activate();
    bool isActivated() const;
    virtual void invalidate();
    virtual void updateGeometry() override;

    virtual void widgetEvent(QEvent *e);

    virtual int count() const = 0;
    virtual QGraphicsLayoutItem *itemAt(int i) const = 0;
    virtual void removeAt(int index) = 0;

    static void setInstantInvalidatePropagation(bool enable);
    static bool instantInvalidatePropagation();
protected:
    QGraphicsLayout(QGraphicsLayoutPrivate &, QGraphicsLayoutItem *);
    void addChildLayoutItem(QGraphicsLayoutItem *layoutItem);

private:
    Q_DISABLE_COPY(QGraphicsLayout)
    Q_DECLARE_PRIVATE(QGraphicsLayout)
    friend class QGraphicsWidget;
};

#ifndef Q_QDOC
Q_DECLARE_INTERFACE(QGraphicsLayout, "org.bobui-project.BobUI.QGraphicsLayout")
#endif

BOBUI_END_NAMESPACE

#endif
