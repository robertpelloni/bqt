// Copyright (C) 2019 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#ifndef QWIDGETTEXTCONTROL_P_H
#define QWIDGETTEXTCONTROL_P_H

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
#include <BobUIGui/bobuiextdocument.h>
#include <BobUIGui/bobuiextoption.h>
#include <BobUIGui/bobuiextcursor.h>
#include <BobUIGui/bobuiextformat.h>
#if BOBUI_CONFIG(textedit)
#include <BobUIWidgets/bobuiextedit.h>
#endif
#if BOBUI_CONFIG(menu)
#include <BobUIWidgets/qmenu.h>
#endif
#include <BobUICore/qrect.h>
#include <BobUIGui/qabstracttextdocumentlayout.h>
#include <BobUIGui/bobuiextdocumentfragment.h>
#include <BobUIGui/qclipboard.h>
#include <BobUICore/qmimedata.h>
#include <BobUIGui/private/qinputcontrol_p.h>

BOBUI_REQUIRE_CONFIG(widgettextcontrol);

BOBUI_BEGIN_NAMESPACE


class QStyleSheet;
class BOBUIextDocument;
class QMenu;
class QWidgetTextControlPrivate;
class QAbstractScrollArea;
class QEvent;
class QRegularExpression;
class BOBUIimerEvent;

class Q_WIDGETS_EXPORT QWidgetTextControl : public QInputControl
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(QWidgetTextControl)
#ifndef BOBUI_NO_TEXTHTMLPARSER
    Q_PROPERTY(QString html READ toHtml WRITE setHtml NOTIFY textChanged USER true)
#endif
    Q_PROPERTY(bool overwriteMode READ overwriteMode WRITE setOverwriteMode)
    Q_PROPERTY(bool acceptRichText READ acceptRichText WRITE setAcceptRichText)
    Q_PROPERTY(int cursorWidth READ cursorWidth WRITE setCursorWidth)
    Q_PROPERTY(BobUI::TextInteractionFlags textInteractionFlags READ textInteractionFlags
               WRITE setTextInteractionFlags)
    Q_PROPERTY(bool openExternalLinks READ openExternalLinks WRITE setOpenExternalLinks)
    Q_PROPERTY(bool ignoreUnusedNavigationEvents READ ignoreUnusedNavigationEvents
               WRITE setIgnoreUnusedNavigationEvents)
public:
    explicit QWidgetTextControl(QObject *parent = nullptr);
    explicit QWidgetTextControl(const QString &text, QObject *parent = nullptr);
    explicit QWidgetTextControl(BOBUIextDocument *doc, QObject *parent = nullptr);
    virtual ~QWidgetTextControl();

    void setDocument(BOBUIextDocument *document);
    BOBUIextDocument *document() const;

    void setTextCursor(const BOBUIextCursor &cursor, bool selectionClipboard = false);
    BOBUIextCursor textCursor() const;

    void setTextInteractionFlags(BobUI::TextInteractionFlags flags);
    BobUI::TextInteractionFlags textInteractionFlags() const;

    void mergeCurrentCharFormat(const BOBUIextCharFormat &modifier);

    void setCurrentCharFormat(const BOBUIextCharFormat &format);
    BOBUIextCharFormat currentCharFormat() const;

    bool find(const QString &exp, BOBUIextDocument::FindFlags options = { });
#if BOBUI_CONFIG(regularexpression)
    bool find(const QRegularExpression &exp, BOBUIextDocument::FindFlags options = { });
#endif

    QString toPlainText() const;
#ifndef BOBUI_NO_TEXTHTMLPARSER
    QString toHtml() const;
#endif
#if BOBUI_CONFIG(textmarkdownwriter)
    QString toMarkdown(BOBUIextDocument::MarkdownFeatures features = BOBUIextDocument::MarkdownDialectGitHub) const;
#endif

    virtual void ensureCursorVisible();

    Q_INVOKABLE virtual QVariant loadResource(int type, const QUrl &name);
#ifndef BOBUI_NO_CONTEXTMENU
    QMenu *createStandardContextMenu(const QPointF &pos, QWidget *parent);
#endif

    BOBUIextCursor cursorForPosition(const QPointF &pos) const;
    QRectF cursorRect(const BOBUIextCursor &cursor) const;
    QRectF cursorRect() const;
    QRectF selectionRect(const BOBUIextCursor &cursor) const;
    QRectF selectionRect() const;

    virtual QString anchorAt(const QPointF &pos) const;
    QPointF anchorPosition(const QString &name) const;

    QString anchorAtCursor() const;

    BOBUIextBlock blockWithMarkerAt(const QPointF &pos) const;

    bool overwriteMode() const;
    void setOverwriteMode(bool overwrite);

    int cursorWidth() const;
    void setCursorWidth(int width);

    bool acceptRichText() const;
    void setAcceptRichText(bool accept);

#if BOBUI_CONFIG(textedit)
    void setExtraSelections(const QList<BOBUIextEdit::ExtraSelection> &selections);
    QList<BOBUIextEdit::ExtraSelection> extraSelections() const;
#endif

    void setTextWidth(qreal width);
    qreal textWidth() const;
    QSizeF size() const;

    void setOpenExternalLinks(bool open);
    bool openExternalLinks() const;

    void setIgnoreUnusedNavigationEvents(bool ignore);
    bool ignoreUnusedNavigationEvents() const;

    void moveCursor(BOBUIextCursor::MoveOperation op, BOBUIextCursor::MoveMode mode = BOBUIextCursor::MoveAnchor);

    bool canPaste() const;

    void setCursorIsFocusIndicator(bool b);
    bool cursorIsFocusIndicator() const;

    void setDragEnabled(bool enabled);
    bool isDragEnabled() const;

    bool isWordSelectionEnabled() const;
    void setWordSelectionEnabled(bool enabled);

    bool isPreediting();

    void print(QPagedPaintDevice *printer) const;

    virtual int hitTest(const QPointF &point, BobUI::HitTestAccuracy accuracy) const;
    virtual QRectF blockBoundingRect(const BOBUIextBlock &block) const;
    QAbstractTextDocumentLayout::PaintContext getPaintContext(QWidget *widget) const;

public Q_SLOTS:
    void setPlainText(const QString &text);
#if BOBUI_CONFIG(textmarkdownreader)
    void setMarkdown(const QString &text);
#endif
    void setHtml(const QString &text);

#ifndef BOBUI_NO_CLIPBOARD
    void cut();
    void copy();
    void paste(QClipboard::Mode mode = QClipboard::Clipboard);
#endif

    void undo();
    void redo();

    void clear();
    void selectAll();

    void insertPlainText(const QString &text);
#ifndef BOBUI_NO_TEXTHTMLPARSER
    void insertHtml(const QString &text);
#endif

    void append(const QString &text);
    void appendHtml(const QString &html);
    void appendPlainText(const QString &text);

    void adjustSize();

Q_SIGNALS:
    void textChanged();
    void undoAvailable(bool b);
    void redoAvailable(bool b);
    void currentCharFormatChanged(const BOBUIextCharFormat &format);
    void copyAvailable(bool b);
    void selectionChanged();
    void cursorPositionChanged();

    // control signals
    void updateRequest(const QRectF &rect = QRectF());
    void documentSizeChanged(const QSizeF &);
    void blockCountChanged(int newBlockCount);
    void visibilityRequest(const QRectF &rect);
    void microFocusChanged();
    void linkActivated(const QString &link);
    void linkHovered(const QString &);
    void blockMarkerHovered(const BOBUIextBlock &block);
    void modificationChanged(bool m);

public:
    // control properties
    QPalette palette() const;
    void setPalette(const QPalette &pal);

    virtual void processEvent(QEvent *e, const BOBUIransform &transform, QWidget *contextWidget = nullptr);
    void processEvent(QEvent *e, const QPointF &coordinateOffset = QPointF(), QWidget *contextWidget = nullptr);

    // control methods
    void drawContents(QPainter *painter, const QRectF &rect = QRectF(), QWidget *widget = nullptr);

    void setFocus(bool focus, BobUI::FocusReason = BobUI::OtherFocusReason);

    virtual QVariant inputMethodQuery(BobUI::InputMethodQuery property, QVariant argument) const;

    virtual QMimeData *createMimeDataFromSelection() const;
    virtual bool canInsertFromMimeData(const QMimeData *source) const;
    virtual void insertFromMimeData(const QMimeData *source);

    bool setFocusToAnchor(const BOBUIextCursor &newCursor);
    bool setFocusToNextOrPreviousAnchor(bool next);
    bool findNextPrevAnchor(const BOBUIextCursor& from, bool next, BOBUIextCursor& newAnchor);

protected:
    virtual void timerEvent(BOBUIimerEvent *e) override;

    virtual bool event(QEvent *e) override;

private:
    Q_DISABLE_COPY_MOVE(QWidgetTextControl)
    Q_PRIVATE_SLOT(d_func(), void _q_deleteSelected())
    Q_PRIVATE_SLOT(d_func(), void _q_copyLink())
};


#ifndef BOBUI_NO_CONTEXTMENU
class QUnicodeControlCharacterMenu : public QMenu
{
    Q_OBJECT
public:
    QUnicodeControlCharacterMenu(QObject *editWidget, QWidget *parent);

private Q_SLOTS:
    void menuActionTriggered();

private:
    QObject *editWidget;
};
#endif // BOBUI_NO_CONTEXTMENU


// also used by QLabel
class BOBUIextEditMimeData : public QMimeData
{
public:
    inline BOBUIextEditMimeData(const BOBUIextDocumentFragment &aFragment) : fragment(aFragment) {}

    virtual QStringList formats() const override;
    bool hasFormat(const QString &format) const override;

protected:
    virtual QVariant retrieveData(const QString &mimeType, QMetaType type) const override;
private:
    void setup() const;

    mutable BOBUIextDocumentFragment fragment;
};

#ifndef BOBUI_NO_CONTEXTMENU
// also used by QLineEdit
void setActionIcon(QAction *action, const QString &name);
#endif // BOBUI_NO_CONTEXTMENU

BOBUI_END_NAMESPACE

#endif // QWidgetTextControl_H
