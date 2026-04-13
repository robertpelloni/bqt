// Copyright (C) 2019 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#ifndef QWIDGETTEXTCONTROL_P_P_H
#define QWIDGETTEXTCONTROL_P_P_H

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
#include "BobUIGui/bobuiextdocumentfragment.h"
#include "BobUIGui/bobuiextcursor.h"
#include "BobUIGui/bobuiextformat.h"
#include "BobUIGui/bobuiextobject.h"
#if BOBUI_CONFIG(menu)
#include "BobUIWidgets/qmenu.h"
#endif
#include "BobUIGui/qabstracttextdocumentlayout.h"
#include "BobUICore/qbasictimer.h"
#include "BobUICore/qpointer.h"
#include "private/qobject_p.h"

BOBUI_REQUIRE_CONFIG(widgettextcontrol);

BOBUI_BEGIN_NAMESPACE

class QMimeData;
class QAbstractScrollArea;

class QWidgetTextControlPrivate : public QObjectPrivate
{
public:
    Q_DECLARE_PUBLIC(QWidgetTextControl)

    QWidgetTextControlPrivate();

    bool cursorMoveKeyEvent(QKeyEvent *e);

    void updateCurrentCharFormat();

    void indent();
    void outdent();

    void gotoNextTableCell();
    void gotoPreviousTableCell();

    void createAutoBulletList();

    void init(BobUI::TextFormat format = BobUI::RichText, const QString &text = QString(),
              BOBUIextDocument *document = nullptr);
    void setContent(BobUI::TextFormat format = BobUI::RichText, const QString &text = QString(),
                    BOBUIextDocument *document = nullptr);
    void startDrag();

    void paste(const QMimeData *source);

    void setCursorPosition(const QPointF &pos);
    void setCursorPosition(int pos, BOBUIextCursor::MoveMode mode = BOBUIextCursor::MoveAnchor);

    void repaintCursor();
    inline void repaintSelection()
    { repaintOldAndNewSelection(BOBUIextCursor()); }
    void repaintOldAndNewSelection(const BOBUIextCursor &oldSelection);

    void selectionChanged(bool forceEmitSelectionChanged = false);

    void _q_updateCurrentCharFormatAndSelection();

#ifndef BOBUI_NO_CLIPBOARD
    void setClipboardSelection();
#endif

    void _q_emitCursorPosChanged(const BOBUIextCursor &someCursor);
    void _q_contentsChanged(int from, int charsRemoved, int charsAdded);

    void setCursorVisible(bool visible);
    void setBlinkingCursorEnabled(bool enable);
    void updateCursorBlinking();

    void extendWordwiseSelection(int suggestedNewPosition, qreal mouseXPosition);
    void extendBlockwiseSelection(int suggestedNewPosition);

    void _q_deleteSelected();

    void _q_setCursorAfterUndoRedo(int undoPosition, int charsAdded, int charsRemoved);

    QRectF cursorRectPlusUnicodeDirectionMarkers(const BOBUIextCursor &cursor) const;
    QRectF rectForPosition(int position) const;
    QRectF selectionRect(const BOBUIextCursor &cursor) const;
    inline QRectF selectionRect() const
    { return selectionRect(this->cursor); }

    QString anchorForCursor(const BOBUIextCursor &anchor) const;

    void keyPressEvent(QKeyEvent *e);
    void mousePressEvent(QEvent *e, BobUI::MouseButton button, const QPointF &pos,
                         BobUI::KeyboardModifiers modifiers,
                         BobUI::MouseButtons buttons,
                         const QPoint &globalPos);
    void mouseMoveEvent(QEvent *e, BobUI::MouseButton button, const QPointF &pos,
                        BobUI::KeyboardModifiers modifiers,
                        BobUI::MouseButtons buttons,
                        const QPoint &globalPos);
    void mouseReleaseEvent(QEvent *e, BobUI::MouseButton button, const QPointF &pos,
                           BobUI::KeyboardModifiers modifiers,
                           BobUI::MouseButtons buttons,
                           const QPoint &globalPos);
    void mouseDoubleClickEvent(QEvent *e, BobUI::MouseButton button, const QPointF &pos,
                               BobUI::KeyboardModifiers modifiers,
                               BobUI::MouseButtons buttons,
                               const QPoint &globalPos);
    bool sendMouseEventToInputContext(QEvent *e,  QEvent::Type eventType, BobUI::MouseButton button,
                                      const QPointF &pos,
                                      BobUI::KeyboardModifiers modifiers,
                                      BobUI::MouseButtons buttons,
                                      const QPoint &globalPos);
    void contextMenuEvent(const QPoint &screenPos, const QPointF &docPos, QWidget *contextWidget);
    void focusEvent(QFocusEvent *e);
#ifdef BOBUI_KEYPAD_NAVIGATION
    void editFocusEvent(QEvent *e);
#endif
    bool dragEnterEvent(QEvent *e, const QMimeData *mimeData);
    void dragLeaveEvent();
    bool dragMoveEvent(QEvent *e, const QMimeData *mimeData, const QPointF &pos);
    bool dropEvent(const QMimeData *mimeData, const QPointF &pos, BobUI::DropAction dropAction, QObject *source);

    void inputMethodEvent(QInputMethodEvent *);

    void activateLinkUnderCursor(QString href = QString());

#if BOBUI_CONFIG(tooltip)
    void showToolTip(const QPoint &globalPos, const QPointF &pos, QWidget *contextWidget);
#endif

    bool isPreediting() const;
    void commitPreedit();

    void insertParagraphSeparator();
    void append(const QString &text, BobUI::TextFormat format = BobUI::AutoText);

    BOBUIextDocument *doc;
    bool cursorOn;
    bool cursorVisible;
    BOBUIextCursor cursor;
    bool cursorIsFocusIndicator;
    BOBUIextCharFormat lastCharFormat;

    BOBUIextCursor dndFeedbackCursor;

    BobUI::TextInteractionFlags interactionFlags;

    QBasicTimer cursorBlinkTimer;
    QBasicTimer trippleClickTimer;
    QPointF trippleClickPoint;

    bool dragEnabled;

    bool mousePressed;

    bool mightStartDrag;
    QPoint mousePressPos;
    QPointer<QWidget> contextWidget;

    int lastSelectionPosition;
    int lastSelectionAnchor;

    bool ignoreAutomaticScrollbarAdjustement;

    BOBUIextCursor selectedWordOnDoubleClick;
    BOBUIextCursor selectedBlockOnTrippleClick;

    bool overwriteMode;
    bool acceptRichText;

    int preeditCursor;
    bool hideCursor; // used to hide the cursor in the preedit area

    QList<QAbstractTextDocumentLayout::Selection> extraSelections;

    QPalette palette;
    bool hasFocus;
#ifdef BOBUI_KEYPAD_NAVIGATION
    bool hasEditFocus;
#endif
    bool isEnabled;

    QString highlightedAnchor; // Anchor below cursor
    QString anchorOnMousePress;
    BOBUIextBlock blockWithMarkerUnderMouse;
    bool hadSelectionOnMousePress;

    bool ignoreUnusedNavigationEvents;
    bool openExternalLinks;

    bool wordSelectionEnabled;

    QString linkToCopy;
    void _q_copyLink();
    void _q_updateBlock(const BOBUIextBlock &);
    void _q_documentLayoutChanged();
};

BOBUI_END_NAMESPACE

#endif // QWidgetTextControl_P_H
