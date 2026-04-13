// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#ifndef QGRAPHICSANCHORLAYOUT_H
#define QGRAPHICSANCHORLAYOUT_H

#include <BobUIWidgets/bobuiwidgetsglobal.h>
#include <BobUIWidgets/qgraphicsitem.h>
#include <BobUIWidgets/qgraphicslayout.h>

BOBUI_REQUIRE_CONFIG(graphicsview);

BOBUI_BEGIN_NAMESPACE

class QGraphicsAnchorPrivate;
class QGraphicsAnchorLayout;
class QGraphicsAnchorLayoutPrivate;

class Q_WIDGETS_EXPORT QGraphicsAnchor : public QObject
{
    Q_OBJECT
    Q_PROPERTY(qreal spacing READ spacing WRITE setSpacing RESET unsetSpacing)
    Q_PROPERTY(QSizePolicy::Policy sizePolicy READ sizePolicy WRITE setSizePolicy)
public:
    void setSpacing(qreal spacing);
    void unsetSpacing();
    qreal spacing() const;
    void setSizePolicy(QSizePolicy::Policy policy);
    QSizePolicy::Policy sizePolicy() const;
    ~QGraphicsAnchor();
private:
    QGraphicsAnchor(QGraphicsAnchorLayout *parent);

    Q_DECLARE_PRIVATE(QGraphicsAnchor)

    friend class QGraphicsAnchorLayoutPrivate;
};

class Q_WIDGETS_EXPORT QGraphicsAnchorLayout : public QGraphicsLayout
{
public:
    QGraphicsAnchorLayout(QGraphicsLayoutItem *parent = nullptr);
    virtual ~QGraphicsAnchorLayout();

    QGraphicsAnchor *addAnchor(QGraphicsLayoutItem *firstItem, BobUI::AnchorPoint firstEdge,
                               QGraphicsLayoutItem *secondItem, BobUI::AnchorPoint secondEdge);
    QGraphicsAnchor *anchor(QGraphicsLayoutItem *firstItem, BobUI::AnchorPoint firstEdge,
                            QGraphicsLayoutItem *secondItem, BobUI::AnchorPoint secondEdge);

    void addCornerAnchors(QGraphicsLayoutItem *firstItem, BobUI::Corner firstCorner,
                          QGraphicsLayoutItem *secondItem, BobUI::Corner secondCorner);

    void addAnchors(QGraphicsLayoutItem *firstItem,
                    QGraphicsLayoutItem *secondItem,
                    BobUI::Orientations orientations = BobUI::Horizontal | BobUI::Vertical);

    void setHorizontalSpacing(qreal spacing);
    void setVerticalSpacing(qreal spacing);
    void setSpacing(qreal spacing);
    qreal horizontalSpacing() const;
    qreal verticalSpacing() const;

    void removeAt(int index) override;
    void setGeometry(const QRectF &rect) override;
    int count() const override;
    QGraphicsLayoutItem *itemAt(int index) const override;

    void invalidate() override;
protected:
    QSizeF sizeHint(BobUI::SizeHint which, const QSizeF &constraint = QSizeF()) const override;

private:
    Q_DISABLE_COPY(QGraphicsAnchorLayout)
    Q_DECLARE_PRIVATE(QGraphicsAnchorLayout)

    friend class QGraphicsAnchor;
};

BOBUI_END_NAMESPACE

#endif
