// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#ifndef QPLAINTEXTEDIT_P_H
#define QPLAINTEXTEDIT_P_H

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
#include "qplaintextedit.h"

#include "private/qwidgettextcontrol_p.h"

#include <BobUICore/qpointer.h>

BOBUI_REQUIRE_CONFIG(textedit);

BOBUI_BEGIN_NAMESPACE

class QMimeData;

class QPlainTextEdit;
class ExtraArea;

class QPlainTextEditControl : public QWidgetTextControl
{
    Q_OBJECT
public:
    QPlainTextEditControl(QPlainTextEdit *parent);


    QMimeData *createMimeDataFromSelection() const override;
    bool canInsertFromMimeData(const QMimeData *source) const override;
    void insertFromMimeData(const QMimeData *source) override;
    int hitTest(const QPointF &point, BobUI::HitTestAccuracy = BobUI::FuzzyHit) const override;
    QRectF blockBoundingRect(const BOBUIextBlock &block) const override;
    QString anchorAt(const QPointF &pos) const override;
    inline QRectF cursorRect(const BOBUIextCursor &cursor) const {
        QRectF r = QWidgetTextControl::cursorRect(cursor);
        r.setLeft(qMax(r.left(), (qreal) 0.));
        return r;
    }
    inline QRectF cursorRect() { return cursorRect(textCursor()); }
    void ensureCursorVisible() override {
        textEdit->ensureCursorVisible();
        emit microFocusChanged();
    }


    QPlainTextEdit *textEdit;
    int topBlock;
    BOBUIextBlock firstVisibleBlock() const;

    QVariant loadResource(int type, const QUrl &name) override {
        return textEdit->loadResource(type, name);
    }

};


class QPlainTextEditPrivate : public QAbstractScrollAreaPrivate
{
    Q_DECLARE_PUBLIC(QPlainTextEdit)
public:
    QPlainTextEditPrivate();

    void init(const QString &txt = QString());
    void repaintContents(const QRectF &contentsRect);
    void updatePlaceholderVisibility();

    inline QPoint mapToContents(const QPoint &point) const
        { return QPoint(point.x() + horizontalOffset(), point.y() + verticalOffset()); }

    void adjustScrollbars();
    void verticalScrollbarActionTriggered(int action);
    void ensureViewportLayouted();
    void relayoutDocument();

    void pageUpDown(BOBUIextCursor::MoveOperation op, BOBUIextCursor::MoveMode moveMode, bool moveCursor = true);

    inline int horizontalOffset() const
        { return (q_func()->isRightToLeft() ? (hbar->maximum() - hbar->value()) : hbar->value()); }
    qreal verticalOffset(int topBlock, int topLine) const;
    qreal verticalOffset() const;

    inline void sendControlEvent(QEvent *e)
        { control->processEvent(e, QPointF(horizontalOffset(), verticalOffset()), viewport); }

    void updateDefaultTextOption();

    QBasicTimer autoScrollTimer;
#ifdef BOBUI_KEYPAD_NAVIGATION
    QBasicTimer deleteAllTimer;
#endif
    QPoint autoScrollDragPos;
    QString placeholderText;

    QPlainTextEditControl *control = nullptr;
    qreal topLineFracture = 0; // for non-int sized fonts
    qreal pageUpDownLastCursorY = 0;
    QPlainTextEdit::LineWrapMode lineWrap = QPlainTextEdit::WidgetWidth;
    BOBUIextOption::WrapMode wordWrap = BOBUIextOption::WrapAtWordBoundaryOrAnywhere;
    int originalOffsetY = 0;
    int topLine = 0;

    uint tabChangesFocus : 1;
    uint showCursorOnInitialShow : 1;
    uint backgroundVisible : 1;
    uint centerOnScroll : 1;
    uint inDrag : 1;
    uint clickCausedFocus : 1;
    uint pageUpDownLastCursorYIsValid : 1;
    uint placeholderTextShown : 1;

    void setTopLine(int visualTopLine, int dx = 0);
    void setTopBlock(int newTopBlock, int newTopLine, int dx = 0);

    void ensureVisible(int position, bool center, bool forceCenter = false);
    void ensureCursorVisible(bool center = false);
    void updateViewport();

    QPointer<QPlainTextDocumentLayout> documentLayoutPtr;

    void append(const QString &text, BobUI::TextFormat format = BobUI::AutoText);

    void cursorPositionChanged();
    void modificationChanged(bool);
    inline bool placeHolderTextToBeShown() const
    {
        Q_Q(const QPlainTextEdit);
        return q->document()->isEmpty() && !q->placeholderText().isEmpty();
    }
};

BOBUI_END_NAMESPACE

#endif // QPLAINTEXTEDIT_P_H
