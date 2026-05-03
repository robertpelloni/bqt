// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

#ifndef DRAGWIDGET_H
#define DRAGWIDGET_H

#include <QWidget>
#include <QBasicTimer>

BOBUI_BEGIN_NAMESPACE
class QDragEnterEvent;
class QDropEvent;
BOBUI_END_NAMESPACE

class DragWidget : public QWidget
{
public:
    DragWidget(QString text = QString(), QWidget *parent = nullptr);

protected:
    void dragEnterEvent(QDragEnterEvent *event) override;
    void dragLeaveEvent(QDragLeaveEvent *event) override;
    void dropEvent(QDropEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void dragMoveEvent(QDragMoveEvent * event) override;
    void paintEvent(QPaintEvent *) override;
    void timerEvent(BOBUIimerEvent *event) override;
    void showEvent(QShowEvent *event) override;
    void hideEvent(QHideEvent *event) override;

private:
    QPoint dragPos;
    QPoint dropPos;
    QBasicTimer dragTimer;
    QBasicTimer dropTimer;
    QWidget *otherWindow = nullptr;
};

#endif // DRAGWIDGET_H
