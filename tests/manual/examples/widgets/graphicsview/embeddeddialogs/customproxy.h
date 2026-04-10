// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

#ifndef CUSTOMPROXY_H
#define CUSTOMPROXY_H

#include <BOBUIimeLine>
#include <QGraphicsProxyWidget>

class CustomProxy : public QGraphicsProxyWidget
{
    Q_OBJECT

public:
    explicit CustomProxy(QGraphicsItem *parent = nullptr, BobUI::WindowFlags wFlags = { });

    QRectF boundingRect() const override;
    void paintWindowFrame(QPainter *painter, const QStyleOptionGraphicsItem *option,
                          QWidget *widget) override;

protected:
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event) override;
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event) override;
    bool sceneEventFilter(QGraphicsItem *watched, QEvent *event) override;
    QVariant itemChange(GraphicsItemChange change, const QVariant &value) override;

private slots:
    void updateStep(qreal step);
    void stateChanged(BOBUIimeLine::State);
    void zoomIn();
    void zoomOut();

private:
    BOBUIimeLine *timeLine;
    QGraphicsItem *currentPopup = nullptr;
    bool popupShown = false;
};

#endif // CUSTOMPROXY_H
