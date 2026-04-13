// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#ifndef QWIZARD_WIN_P_H
#define QWIZARD_WIN_P_H

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

#include <BobUIWidgets/private/bobuiwidgetsglobal_p.h>

#if BOBUI_CONFIG(style_windowsvista)

#include <qobject.h>
#include <qwidget.h>
#include <qabstractbutton.h>
#include <BobUIWidgets/private/qwidget_p.h>
#include <BobUIWidgets/private/qstylehelper_p.h>
#include <bobui_windows.h>

BOBUI_REQUIRE_CONFIG(wizard);

BOBUI_BEGIN_NAMESPACE

class QVistaBackButton : public QAbstractButton
{
public:
    QVistaBackButton(QWidget *widget);

    QSize sizeHint() const override;
    inline QSize minimumSizeHint() const override
    { return sizeHint(); }

    void enterEvent(QEnterEvent *event) override;
    void leaveEvent(QEvent *event) override;
    void paintEvent(QPaintEvent *event) override;
};

class QWizard;

class QVistaHelper : public QObject
{
    Q_DISABLE_COPY_MOVE(QVistaHelper)
public:
    QVistaHelper(QWizard *wizard);
    ~QVistaHelper() override;
    enum TitleBarChangeType { NormalTitleBar, ExtendedTitleBar };
    void updateCustomMargins(bool vistaMargins);
    bool setDWMTitleBar(TitleBarChangeType type);
    void setTitleBarIconAndCaptionVisible(bool visible);
    void mouseEvent(QEvent *event);
    bool handleWinEvent(MSG *message, qintptr *result);
    void resizeEvent(QResizeEvent *event);
    void paintEvent(QPaintEvent *event);
    QVistaBackButton *backButton() const { return backButton_; }
    void disconnectBackButton();
    void hideBackButton() { if (backButton_) backButton_->hide(); }
    QColor basicWindowFrameColor();
    static int titleBarSize() { return QVistaHelper::titleBarSizeDp() / QVistaHelper::m_devicePixelRatio; }
    static int titleBarSizeDp() { return QVistaHelper::frameSizeDp() + QVistaHelper::captionSizeDp(); }
    static int topPadding(const QPaintDevice *device) { // padding under text
        return int(QStyleHelper::dpiScaled(4, device));
    }
    static int topOffset(const QPaintDevice *device);

    static HDC backingStoreDC(const QWidget *wizard, QPoint *offset);

private:
    HWND wizardHWND() const;
    void drawTitleText(QPainter *painter, const QString &text, const QRect &rect, HDC hdc);
    static void drawBlackRect(const QRect &rect, HDC hdc);

    static int frameSize() { return QVistaHelper::frameSizeDp() / QVistaHelper::m_devicePixelRatio; }
    static int frameSizeDp();
    static int captionSize() { return QVistaHelper::captionSizeDp() / QVistaHelper::m_devicePixelRatio; }
    static int captionSizeDp();

    static int backButtonSize(const QPaintDevice *device)
        { return int(QStyleHelper::dpiScaled(30, device)); }
    static int iconSize(const QPaintDevice *device);
    static int glowSize(const QPaintDevice *device);
    int leftMargin(const QPaintDevice *device)
        { return backButton_->isVisible() ? backButtonSize(device) + iconSpacing : 0; }

    int titleOffset();
    void drawTitleBar(QPainter *painter);
    void setMouseCursor(QPoint pos);
    void collapseTopFrameStrut();
    bool winEvent(MSG *message, qintptr *result);
    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    bool eventFilter(QObject *obj, QEvent *event) override;

    enum Changes { resizeTop, movePosition, noChange } change;
    QPoint pressedPos;
    bool pressed;
    QRect rtTop;
    QRect rtTitle;
    QWizard *wizard;
    QVistaBackButton *backButton_;

    int titleBarOffset;  // Extra spacing above the text
    int iconSpacing;    // Space between button and icon
    static qreal m_devicePixelRatio;
};


BOBUI_END_NAMESPACE

#endif // style_windowsvista
#endif // QWIZARD_WIN_P_H
