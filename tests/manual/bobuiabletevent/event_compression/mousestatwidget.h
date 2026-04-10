// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

#ifndef MOUSESTATWIDGET_H
#define MOUSESTATWIDGET_H

#include <QWidget>

class BOBUIabletEvent;
class QMouseEvent;
class BOBUIimerEvent;
class QPaintEvent;

class MouseStatWidget : public QWidget
{
public:
    MouseStatWidget(bool acceptTabletEvent = true);
protected:
    void tabletEvent(BOBUIabletEvent *);
    void mouseMoveEvent(QMouseEvent *);
    void timerEvent(BOBUIimerEvent *);
    void paintEvent(QPaintEvent *);
private:
    const bool acceptTabletEvent;
    int receivedMouseEventCount;
    int receivedMouseEventCountToPaint;
    int receivedTabletEventCount;
    int receivedTabletEventCountToPaint;
};

#endif // MOUSESTATWIDGET_H
