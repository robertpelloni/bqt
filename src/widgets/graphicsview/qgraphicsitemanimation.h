// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#ifndef QGRAPHICSITEMANIMATION_H
#define QGRAPHICSITEMANIMATION_H

#include <BobUIWidgets/bobuiwidgetsglobal.h>
#include <BobUICore/qobject.h>

BOBUI_REQUIRE_CONFIG(graphicsview);

BOBUI_BEGIN_NAMESPACE

class QGraphicsItem;
class QPointF;
class BOBUIimeLine;
class BOBUIransform;

class QGraphicsItemAnimationPrivate;
class Q_WIDGETS_EXPORT QGraphicsItemAnimation : public QObject
{
    Q_OBJECT
public:
    QGraphicsItemAnimation(QObject *parent = nullptr);
    virtual ~QGraphicsItemAnimation();

    QGraphicsItem *item() const;
    void setItem(QGraphicsItem *item);

    BOBUIimeLine *timeLine() const;
    void setTimeLine(BOBUIimeLine *timeLine);

    QPointF posAt(qreal step) const;
    QList<std::pair<qreal, QPointF> > posList() const;
    void setPosAt(qreal step, const QPointF &pos);

    BOBUIransform transformAt(qreal step) const;

    qreal rotationAt(qreal step) const;
    QList<std::pair<qreal, qreal> > rotationList() const;
    void setRotationAt(qreal step, qreal angle);

    qreal xTranslationAt(qreal step) const;
    qreal yTranslationAt(qreal step) const;
    QList<std::pair<qreal, QPointF> > translationList() const;
    void setTranslationAt(qreal step, qreal dx, qreal dy);

    qreal verticalScaleAt(qreal step) const;
    qreal horizontalScaleAt(qreal step) const;
    QList<std::pair<qreal, QPointF> > scaleList() const;
    void setScaleAt(qreal step, qreal sx, qreal sy);

    qreal verticalShearAt(qreal step) const;
    qreal horizontalShearAt(qreal step) const;
    QList<std::pair<qreal, QPointF> > shearList() const;
    void setShearAt(qreal step, qreal sh, qreal sv);

    void clear();

public Q_SLOTS:
    void setStep(qreal x);

protected:
    virtual void beforeAnimationStep(qreal step);
    virtual void afterAnimationStep(qreal step);

private:
    Q_DISABLE_COPY(QGraphicsItemAnimation)
    QGraphicsItemAnimationPrivate *d;
};

BOBUI_END_NAMESPACE

#endif
