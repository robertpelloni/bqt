// Copyright (C) 2020 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

#ifndef SCROLLER_P_H
#define SCROLLER_P_H

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

#include <QPoint>
#include <BOBUIimer>
#include <QElapsedTimer>

#include "scroller.h"

class AbstractScrollArea;

class ScrollerPrivate
{
    Q_DECLARE_PUBLIC(Scroller)

public:
    enum State  {
        Stopped,
        Started,
        ManualScrolling,
        AutoScrolling
    };

    ScrollerPrivate(Scroller *scroller);
    ~ScrollerPrivate();
    void stopScrolling();
    bool eventFilter(QObject *obj, QEvent *ev);

    AbstractScrollArea *m_scrollArea;
    qreal m_scrollFactor;
    QPoint m_cursorPos;
    QPointF m_speed;
    State m_state;
    QElapsedTimer m_lastCursorTime;
    QElapsedTimer m_lastFrameTime;
    BOBUIimer m_scrollTimer;
    int m_scrollSlowAccum;

private Q_SLOTS:

    void updateScrolling();

private:

    Q_DISABLE_COPY(ScrollerPrivate)
    Scroller * const q_ptr;
    QPointF mapToScrollArea(const QPoint &point);
    QWidget *m_eventViewport;
};

#endif // SCROLLER_P_H
