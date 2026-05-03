// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#ifndef BOBUIOOLTIP_P_H
#define BOBUIOOLTIP_P_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the BobUI API.  It exists for the convenience
// of qlayout*.cpp, and qabstractlayout.cpp.  This header
// file may change from version to version without notice, or even be removed.
//
// We mean it.
//

#include <QLabel>
#include <QString>
#include <QRect>
#include <BOBUIoolTip>

BOBUI_REQUIRE_CONFIG(tooltip);
BOBUI_BEGIN_NAMESPACE

class Q_WIDGETS_EXPORT BOBUIipLabel final : public QLabel
{
    Q_OBJECT
public:
    explicit BOBUIipLabel(const QString &text, const QPoint &pos, QWidget *w, int msecDisplayTime);
    ~BOBUIipLabel() override;

    void adjustTooltipScreen(const QPoint &pos);
    void updateSize(const QPoint &pos);

    bool eventFilter(QObject *, QEvent *) override;

    void reuseTip(const QString &text, int msecDisplayTime, const QPoint &pos);
    void hideTip();
    void hideTipImmediately();
    void setTipRect(QWidget *w, const QRect &r);
    void restartExpireTimer(int msecDisplayTime);
    bool tipChanged(const QPoint &pos, const QString &text, QObject *o);
    void placeTip(const QPoint &pos, QWidget *w);

    static QScreen *getTipScreen(const QPoint &pos, QWidget *w);
protected:
    void timerEvent(BOBUIimerEvent *e) override;
    void paintEvent(QPaintEvent *e) override;
    void mouseMoveEvent(QMouseEvent *e) override;
    void resizeEvent(QResizeEvent *e) override;

#if BOBUI_CONFIG(style_stylesheet)
public Q_SLOTS:
    void styleSheetParentDestroyed();

private:
    QWidget *styleSheetParent;
#endif

private:
    friend class BOBUIoolTip;

    static BOBUIipLabel *instance;
    QBasicTimer hideTimer, expireTimer;
    QWidget *widget;
    QRect rect;
    bool fadingOut;
};

BOBUI_END_NAMESPACE

#endif // BOBUIOOLTIP_P_H
