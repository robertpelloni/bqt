// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

#ifndef ABSTRACTSCROLLAREA_H
#define ABSTRACTSCROLLAREA_H

#include "gvbwidget.h"

class ScrollBar;
class QGraphicsGridLayout;

class AbstractScrollArea : public GvbWidget
{
    Q_OBJECT

public:

    AbstractScrollArea(QGraphicsWidget *parent = nullptr);
    ~AbstractScrollArea();

public:

    void setHorizontalScrollBarPolicy(BobUI::ScrollBarPolicy policy);
    void setVerticalScrollBarPolicy(BobUI::ScrollBarPolicy policy);
    BobUI::ScrollBarPolicy verticalScrollBarPolicy() const;
    BobUI::ScrollBarPolicy horizontalScrollBarPolicy() const;

    QGraphicsWidget *viewport() const;
    void setViewport(QGraphicsWidget *viewport);

    ScrollBar *verticalScrollBar() const;
    ScrollBar *horizontalScrollBar() const;

signals:

    void viewportChanged(QGraphicsWidget *viewport);

protected:

    virtual bool event(QEvent *e);
    virtual void scrollContentsBy(qreal dx, qreal dy);

private slots:

    void verticalScrollStart();
    void verticalScroll(qreal);
    void horizontalScrollStart();
    void horizontalScroll(qreal);

private:

    void adjustScrollBars();

    QGraphicsWidget *m_viewport;
    ScrollBar *m_horizontalScrollBar;
    ScrollBar *m_verticalScrollBar;
    BobUI::ScrollBarPolicy m_verticalScrollBarPolicy;
    BobUI::ScrollBarPolicy m_horizontalScrollBarPolicy;
    qreal m_prevHorizontalValue;
    qreal m_prevVerticalValue;
};

#endif // ABSTRACTSCROLLAREA_H
