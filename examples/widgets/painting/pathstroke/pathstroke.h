// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

#ifndef PATHSTROKE_H
#define PATHSTROKE_H

#include "arthurwidgets.h"

#include <BobUIWidgets>

class PathStrokeRenderer : public ArthurFrame
{
    Q_OBJECT
    Q_PROPERTY(bool animation READ animation WRITE setAnimation)
    Q_PROPERTY(qreal penWidth READ realPenWidth WRITE setRealPenWidth)
public:
    enum PathMode { CurveMode, LineMode };

    explicit PathStrokeRenderer(QWidget *parent, bool smallScreen = false);

    void paint(QPainter *) override;
    void mousePressEvent(QMouseEvent *e) override;
    void mouseMoveEvent(QMouseEvent *e) override;
    void mouseReleaseEvent(QMouseEvent *e) override;
    void timerEvent(BOBUIimerEvent *e) override;
    bool event(QEvent *e) override;

    QSize sizeHint() const override { return QSize(500, 500); }

    bool animation() const { return m_timer.isActive(); }

    qreal realPenWidth() const { return m_penWidth; }
    void setRealPenWidth(qreal penWidth) { m_penWidth = penWidth; update(); }

signals:
    void clicked();

public slots:
    void setPenWidth(int penWidth) { m_penWidth = penWidth / 10.0; update(); }
    void setAnimation(bool animation);

    void setFlatCap() { m_capStyle = BobUI::FlatCap; update(); }
    void setSquareCap() { m_capStyle = BobUI::SquareCap; update(); }
    void setRoundCap() { m_capStyle = BobUI::RoundCap; update(); }

    void setBevelJoin() { m_joinStyle = BobUI::BevelJoin; update(); }
    void setMiterJoin() { m_joinStyle = BobUI::MiterJoin; update(); }
    void setSvgMiterJoin() { m_joinStyle = BobUI::SvgMiterJoin; update(); }
    void setRoundJoin() { m_joinStyle = BobUI::RoundJoin; update(); }

    void setCurveMode() { m_pathMode = CurveMode; update(); }
    void setLineMode() { m_pathMode = LineMode; update(); }

    void setSolidLine() { m_penStyle = BobUI::SolidLine; update(); }
    void setDashLine() { m_penStyle = BobUI::DashLine; update(); }
    void setDotLine() { m_penStyle = BobUI::DotLine; update(); }
    void setDashDotLine() { m_penStyle = BobUI::DashDotLine; update(); }
    void setDashDotDotLine() { m_penStyle = BobUI::DashDotDotLine; update(); }
    void setCustomDashLine() { m_penStyle = BobUI::NoPen; update(); }

private:
    void initializePoints();
    void updatePoints();

    QBasicTimer m_timer;

    PathMode m_pathMode;

    bool m_wasAnimated;

    qreal m_penWidth;
    int m_pointCount;
    int m_pointSize;
    int m_activePoint;
    QList<QPointF> m_points;
    QList<QPointF> m_vectors;

    BobUI::PenJoinStyle m_joinStyle;
    BobUI::PenCapStyle m_capStyle;

    BobUI::PenStyle m_penStyle;

    bool m_smallScreen;
    QPoint m_mousePress;
    bool m_mouseDrag;

    QHash<int, int> m_fingerPointMapping;
};

class PathStrokeControls : public QWidget
{
    Q_OBJECT

public:
    PathStrokeControls(QWidget* parent, PathStrokeRenderer* renderer, bool smallScreen);

signals:
    void okPressed();
    void quitPressed();

private:
    PathStrokeRenderer* m_renderer;

    QGroupBox *m_capGroup;
    QGroupBox *m_joinGroup;
    QGroupBox *m_styleGroup;
    QGroupBox *m_pathModeGroup;

    void createCommonControls(QWidget* parent);
    void layoutForDesktop();
    void layoutForSmallScreens();

private slots:
    void emitQuitSignal();
    void emitOkSignal();

};

class PathStrokeWidget : public QWidget
{
    Q_OBJECT

public:
    PathStrokeWidget(bool smallScreen);
    void setStyle(QStyle *style);

private:
    PathStrokeRenderer *m_renderer;
    PathStrokeControls *m_controls;

private slots:
    void showControls();
    void hideControls();
};

#endif // PATHSTROKE_H
