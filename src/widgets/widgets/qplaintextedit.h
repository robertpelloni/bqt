// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#ifndef QPLAINTEXTEDIT_H
#define QPLAINTEXTEDIT_H

#include <BobUIWidgets/bobuiwidgetsglobal.h>
#include <BobUIWidgets/bobuiextedit.h>

#include <BobUIWidgets/qabstractscrollarea.h>
#include <BobUIGui/bobuiextdocument.h>
#include <BobUIGui/bobuiextoption.h>
#include <BobUIGui/bobuiextcursor.h>
#include <BobUIGui/bobuiextformat.h>
#include <BobUIGui/qabstracttextdocumentlayout.h>

BOBUI_REQUIRE_CONFIG(textedit);

BOBUI_BEGIN_NAMESPACE

class QStyleSheet;
class BOBUIextDocument;
class QMenu;
class QPlainTextEditPrivate;
class QMimeData;
class QPagedPaintDevice;
class QRegularExpression;

class Q_WIDGETS_EXPORT QPlainTextEdit : public QAbstractScrollArea
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(QPlainTextEdit)
    Q_PROPERTY(bool tabChangesFocus READ tabChangesFocus WRITE setTabChangesFocus)
    Q_PROPERTY(QString documentTitle READ documentTitle WRITE setDocumentTitle)
    Q_PROPERTY(bool undoRedoEnabled READ isUndoRedoEnabled WRITE setUndoRedoEnabled)
    Q_PROPERTY(LineWrapMode lineWrapMode READ lineWrapMode WRITE setLineWrapMode)
    QDOC_PROPERTY(BOBUIextOption::WrapMode wordWrapMode READ wordWrapMode WRITE setWordWrapMode)
    Q_PROPERTY(bool readOnly READ isReadOnly WRITE setReadOnly)
    Q_PROPERTY(QString plainText READ toPlainText WRITE setPlainText NOTIFY textChanged USER true)
    Q_PROPERTY(bool overwriteMode READ overwriteMode WRITE setOverwriteMode)
    Q_PROPERTY(qreal tabStopDistance READ tabStopDistance WRITE setTabStopDistance)
    Q_PROPERTY(int cursorWidth READ cursorWidth WRITE setCursorWidth)
    Q_PROPERTY(BobUI::TextInteractionFlags textInteractionFlags READ textInteractionFlags
               WRITE setTextInteractionFlags)
    Q_PROPERTY(int blockCount READ blockCount)
    Q_PROPERTY(int maximumBlockCount READ maximumBlockCount WRITE setMaximumBlockCount)
    Q_PROPERTY(bool backgroundVisible READ backgroundVisible WRITE setBackgroundVisible)
    Q_PROPERTY(bool centerOnScroll READ centerOnScroll WRITE setCenterOnScroll)
    Q_PROPERTY(QString placeholderText READ placeholderText WRITE setPlaceholderText)
public:
    enum LineWrapMode {
        NoWrap,
        WidgetWidth
    };
    Q_ENUM(LineWrapMode)

    explicit QPlainTextEdit(QWidget *parent = nullptr);
    explicit QPlainTextEdit(const QString &text, QWidget *parent = nullptr);
    virtual ~QPlainTextEdit();

    void setDocument(BOBUIextDocument *document);
    BOBUIextDocument *document() const;

    void setPlaceholderText(const QString &placeholderText);
    QString placeholderText() const;

    void setTextCursor(const BOBUIextCursor &cursor);
    BOBUIextCursor textCursor() const;

    bool isReadOnly() const;
    void setReadOnly(bool ro);

    void setTextInteractionFlags(BobUI::TextInteractionFlags flags);
    BobUI::TextInteractionFlags textInteractionFlags() const;

    void mergeCurrentCharFormat(const BOBUIextCharFormat &modifier);
    void setCurrentCharFormat(const BOBUIextCharFormat &format);
    BOBUIextCharFormat currentCharFormat() const;

    bool tabChangesFocus() const;
    void setTabChangesFocus(bool b);

    inline void setDocumentTitle(const QString &title)
    { document()->setMetaInformation(BOBUIextDocument::DocumentTitle, title); }
    inline QString documentTitle() const
    { return document()->metaInformation(BOBUIextDocument::DocumentTitle); }

    inline bool isUndoRedoEnabled() const
    { return document()->isUndoRedoEnabled(); }
    inline void setUndoRedoEnabled(bool enable)
    { document()->setUndoRedoEnabled(enable); }

    inline void setMaximumBlockCount(int maximum)
    { document()->setMaximumBlockCount(maximum); }
    inline int maximumBlockCount() const
    { return document()->maximumBlockCount(); }


    LineWrapMode lineWrapMode() const;
    void setLineWrapMode(LineWrapMode mode);

    BOBUIextOption::WrapMode wordWrapMode() const;
    void setWordWrapMode(BOBUIextOption::WrapMode policy);

    void setBackgroundVisible(bool visible);
    bool backgroundVisible() const;

    void setCenterOnScroll(bool enabled);
    bool centerOnScroll() const;

    bool find(const QString &exp, BOBUIextDocument::FindFlags options = BOBUIextDocument::FindFlags());
#if BOBUI_CONFIG(regularexpression)
    bool find(const QRegularExpression &exp, BOBUIextDocument::FindFlags options = BOBUIextDocument::FindFlags());
#endif

    inline QString toPlainText() const
    { return document()->toPlainText(); }

    void ensureCursorVisible();

    virtual QVariant loadResource(int type, const QUrl &name);
#ifndef BOBUI_NO_CONTEXTMENU
    QMenu *createStandardContextMenu();
    QMenu *createStandardContextMenu(const QPoint &position);
#endif

    BOBUIextCursor cursorForPosition(const QPoint &pos) const;
    QRect cursorRect(const BOBUIextCursor &cursor) const;
    QRect cursorRect() const;

    QString anchorAt(const QPoint &pos) const;

    bool overwriteMode() const;
    void setOverwriteMode(bool overwrite);

    qreal tabStopDistance() const;
    void setTabStopDistance(qreal distance);

    int cursorWidth() const;
    void setCursorWidth(int width);

    void setExtraSelections(const QList<BOBUIextEdit::ExtraSelection> &selections);
    QList<BOBUIextEdit::ExtraSelection> extraSelections() const;

    void moveCursor(BOBUIextCursor::MoveOperation operation, BOBUIextCursor::MoveMode mode = BOBUIextCursor::MoveAnchor);

    bool canPaste() const;

    void print(QPagedPaintDevice *printer) const;

    int blockCount() const;
    QVariant inputMethodQuery(BobUI::InputMethodQuery property) const override;
    Q_INVOKABLE QVariant inputMethodQuery(BobUI::InputMethodQuery query, QVariant argument) const;

public Q_SLOTS:

    void setPlainText(const QString &text);

#ifndef BOBUI_NO_CLIPBOARD
    void cut();
    void copy();
    void paste();
#endif

    void undo();
    void redo();

    void clear();
    void selectAll();

    void insertPlainText(const QString &text);

    void appendPlainText(const QString &text);
    void appendHtml(const QString &html);

    void centerCursor();

    void zoomIn(int range = 1);
    void zoomOut(int range = 1);

Q_SIGNALS:
    void textChanged();
    void undoAvailable(bool b);
    void redoAvailable(bool b);
    void copyAvailable(bool b);
    void selectionChanged();
    void cursorPositionChanged();

    void updateRequest(const QRect &rect, int dy);
    void blockCountChanged(int newBlockCount);
    void modificationChanged(bool);

protected:
    virtual bool event(QEvent *e) override;
    virtual void timerEvent(BOBUIimerEvent *e) override;
    virtual void keyPressEvent(QKeyEvent *e) override;
    virtual void keyReleaseEvent(QKeyEvent *e) override;
    virtual void resizeEvent(QResizeEvent *e) override;
    virtual void paintEvent(QPaintEvent *e) override;
    virtual void mousePressEvent(QMouseEvent *e) override;
    virtual void mouseMoveEvent(QMouseEvent *e) override;
    virtual void mouseReleaseEvent(QMouseEvent *e) override;
    virtual void mouseDoubleClickEvent(QMouseEvent *e) override;
    virtual bool focusNextPrevChild(bool next) override;
#ifndef BOBUI_NO_CONTEXTMENU
    virtual void contextMenuEvent(QContextMenuEvent *e) override;
#endif
#if BOBUI_CONFIG(draganddrop)
    virtual void dragEnterEvent(QDragEnterEvent *e) override;
    virtual void dragLeaveEvent(QDragLeaveEvent *e) override;
    virtual void dragMoveEvent(QDragMoveEvent *e) override;
    virtual void dropEvent(QDropEvent *e) override;
#endif
    virtual void focusInEvent(QFocusEvent *e) override;
    virtual void focusOutEvent(QFocusEvent *e) override;
    virtual void showEvent(QShowEvent *) override;
    virtual void changeEvent(QEvent *e) override;
#if BOBUI_CONFIG(wheelevent)
    virtual void wheelEvent(QWheelEvent *e) override;
#endif

    virtual QMimeData *createMimeDataFromSelection() const;
    virtual bool canInsertFromMimeData(const QMimeData *source) const;
    virtual void insertFromMimeData(const QMimeData *source);

    virtual void inputMethodEvent(QInputMethodEvent *) override;

    QPlainTextEdit(QPlainTextEditPrivate &dd, QWidget *parent);

    virtual void scrollContentsBy(int dx, int dy) override;
    virtual void doSetTextCursor(const BOBUIextCursor &cursor);

    BOBUIextBlock firstVisibleBlock() const;
    QPointF contentOffset() const;
    QRectF blockBoundingRect(const BOBUIextBlock &block) const;
    QRectF blockBoundingGeometry(const BOBUIextBlock &block) const;
    QAbstractTextDocumentLayout::PaintContext getPaintContext() const;

    void zoomInF(float range);

private:
    Q_DISABLE_COPY(QPlainTextEdit)

    friend class QPlainTextEditControl;
};


class QPlainTextDocumentLayoutPrivate;
class Q_WIDGETS_EXPORT QPlainTextDocumentLayout : public QAbstractTextDocumentLayout
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(QPlainTextDocumentLayout)
    Q_PROPERTY(int cursorWidth READ cursorWidth WRITE setCursorWidth)

public:
    QPlainTextDocumentLayout(BOBUIextDocument *document);
    ~QPlainTextDocumentLayout();

    void draw(QPainter *, const PaintContext &) override;
    int hitTest(const QPointF &, BobUI::HitTestAccuracy ) const override;

    int pageCount() const override;
    QSizeF documentSize() const override;

    QRectF frameBoundingRect(BOBUIextFrame *) const override;
    QRectF blockBoundingRect(const BOBUIextBlock &block) const override;

    void ensureBlockLayout(const BOBUIextBlock &block) const;

    void setCursorWidth(int width);
    int cursorWidth() const;

    void requestUpdate();

protected:
    void documentChanged(int from, int /*charsRemoved*/, int charsAdded) override;


private:
    void setTextWidth(qreal newWidth);
    qreal textWidth() const;
    void layoutBlock(const BOBUIextBlock &block);
    qreal blockWidth(const BOBUIextBlock &block);

    QPlainTextDocumentLayoutPrivate *priv() const;

    friend class QPlainTextEdit;
    friend class QPlainTextEditPrivate;
};

BOBUI_END_NAMESPACE

#endif // QPLAINTEXTEDIT_H
