// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#ifndef BOBUIEXTEDIT_P_H
#define BOBUIEXTEDIT_P_H

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
#include "private/qabstractscrollarea_p.h"
#include "BobUIGui/bobuiextdocumentfragment.h"
#if BOBUI_CONFIG(scrollbar)
#include "BobUIWidgets/qscrollbar.h"
#endif
#include "BobUIGui/bobuiextcursor.h"
#include "BobUIGui/bobuiextformat.h"
#if BOBUI_CONFIG(menu)
#include "BobUIWidgets/qmenu.h"
#endif
#include "BobUIGui/qabstracttextdocumentlayout.h"
#include "BobUICore/qbasictimer.h"
#include "BobUICore/qurl.h"
#include "bobuiextedit.h"

#include "private/qwidgettextcontrol_p.h"

#include <array>

BOBUI_REQUIRE_CONFIG(textedit);

BOBUI_BEGIN_NAMESPACE

class QMimeData;
class BOBUIextEditPrivate : public QAbstractScrollAreaPrivate
{
    Q_DECLARE_PUBLIC(BOBUIextEdit)
public:
    BOBUIextEditPrivate();
    ~BOBUIextEditPrivate();

    void init(const QString &html = QString());
    void paint(QPainter *p, QPaintEvent *e);
    void repaintContents(const QRectF &contentsRect);

    inline QPoint mapToContents(const QPoint &point) const
    { return QPoint(point.x() + horizontalOffset(), point.y() + verticalOffset()); }

    void adjustScrollbars();
    void ensureVisible(const QRectF &rect);
    void relayoutDocument();

    void createAutoBulletList();
    void pageUpDown(BOBUIextCursor::MoveOperation op, BOBUIextCursor::MoveMode moveMode);

    inline int horizontalOffset() const
    { return q_func()->isRightToLeft() ? (hbar->maximum() - hbar->value()) : hbar->value(); }
    inline int verticalOffset() const
    { return vbar->value(); }

    inline void sendControlEvent(QEvent *e)
    { control->processEvent(e, QPointF(horizontalOffset(), verticalOffset()), viewport); }

    void cursorPositionChanged();
    void hoveredBlockWithMarkerChanged(const BOBUIextBlock &block);

    void updateDefaultTextOption();

    // re-implemented by BOBUIextBrowser, called by BOBUIextDocument::loadResource
    virtual QUrl resolveUrl(const QUrl &url) const
    { return url; }

    QWidgetTextControl *control;

    BOBUIextEdit::AutoFormatting autoFormatting;
    bool tabChangesFocus;

    QBasicTimer autoScrollTimer;
    QPoint autoScrollDragPos;

    BOBUIextEdit::LineWrapMode lineWrap;
    int lineWrapColumnOrWidth;
    BOBUIextOption::WrapMode wordWrap;

    uint ignoreAutomaticScrollbarAdjustment : 1;
    uint preferRichText : 1;
    uint showCursorOnInitialShow : 1;
    uint inDrag : 1;
    uint clickCausedFocus : 1;

    QString anchorToScrollToWhenVisible;

    QString placeholderText;

    BobUI::CursorShape cursorToRestoreAfterHover = BobUI::IBeamCursor;

    std::array<QMetaObject::Connection, 13> connections;

#ifdef BOBUI_KEYPAD_NAVIGATION
    QBasicTimer deleteAllTimer;
#endif
};

BOBUI_END_NAMESPACE

#endif // BOBUIEXTEDIT_P_H
