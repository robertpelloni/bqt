// Copyright (C) 2019 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#ifndef BOBUIEXTEDIT_H
#define BOBUIEXTEDIT_H

#include <BobUIWidgets/bobuiwidgetsglobal.h>
#include <BobUIWidgets/qabstractscrollarea.h>
#include <BobUIGui/bobuiextdocument.h>
#include <BobUIGui/bobuiextoption.h>
#include <BobUIGui/bobuiextcursor.h>
#include <BobUIGui/bobuiextformat.h>

BOBUI_REQUIRE_CONFIG(textedit);

class tst_BOBUIextEdit;

BOBUI_BEGIN_NAMESPACE

class QStyleSheet;
class BOBUIextDocument;
class QMenu;
class BOBUIextEditPrivate;
class QMimeData;
class QPagedPaintDevice;
class QRegularExpression;

class Q_WIDGETS_EXPORT BOBUIextEdit : public QAbstractScrollArea
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(BOBUIextEdit)
    Q_PROPERTY(AutoFormatting autoFormatting READ autoFormatting WRITE setAutoFormatting)
    Q_PROPERTY(bool tabChangesFocus READ tabChangesFocus WRITE setTabChangesFocus)
    Q_PROPERTY(QString documentTitle READ documentTitle WRITE setDocumentTitle)
    Q_PROPERTY(bool undoRedoEnabled READ isUndoRedoEnabled WRITE setUndoRedoEnabled)
    Q_PROPERTY(LineWrapMode lineWrapMode READ lineWrapMode WRITE setLineWrapMode)
    QDOC_PROPERTY(BOBUIextOption::WrapMode wordWrapMode READ wordWrapMode WRITE setWordWrapMode)
    Q_PROPERTY(int lineWrapColumnOrWidth READ lineWrapColumnOrWidth WRITE setLineWrapColumnOrWidth)
    Q_PROPERTY(bool readOnly READ isReadOnly WRITE setReadOnly)
#if BOBUI_CONFIG(textmarkdownreader) && BOBUI_CONFIG(textmarkdownwriter)
    Q_PROPERTY(QString markdown READ toMarkdown WRITE setMarkdown NOTIFY textChanged)
#endif
#ifndef BOBUI_NO_TEXTHTMLPARSER
    Q_PROPERTY(QString html READ toHtml WRITE setHtml NOTIFY textChanged USER true)
#endif
    Q_PROPERTY(QString plainText READ toPlainText WRITE setPlainText DESIGNABLE false)
    Q_PROPERTY(bool overwriteMode READ overwriteMode WRITE setOverwriteMode)
    Q_PROPERTY(qreal tabStopDistance READ tabStopDistance WRITE setTabStopDistance)
    Q_PROPERTY(bool acceptRichText READ acceptRichText WRITE setAcceptRichText)
    Q_PROPERTY(int cursorWidth READ cursorWidth WRITE setCursorWidth)
    Q_PROPERTY(BobUI::TextInteractionFlags textInteractionFlags READ textInteractionFlags
               WRITE setTextInteractionFlags)
    Q_PROPERTY(BOBUIextDocument *document READ document WRITE setDocument DESIGNABLE false)
    Q_PROPERTY(QString placeholderText READ placeholderText WRITE setPlaceholderText)
public:
    enum LineWrapMode {
        NoWrap,
        WidgetWidth,
        FixedPixelWidth,
        FixedColumnWidth
    };
    Q_ENUM(LineWrapMode)

    enum AutoFormattingFlag {
        AutoNone = 0,
        AutoBulletList = 0x00000001,
        AutoAll = 0xffffffff
    };

    Q_DECLARE_FLAGS(AutoFormatting, AutoFormattingFlag)
    Q_FLAG(AutoFormatting)

    explicit BOBUIextEdit(QWidget *parent = nullptr);
    explicit BOBUIextEdit(const QString &text, QWidget *parent = nullptr);
    virtual ~BOBUIextEdit();

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

    qreal fontPointSize() const;
    QString fontFamily() const;
    int fontWeight() const;
    bool fontUnderline() const;
    bool fontItalic() const;
    QColor textColor() const;
    QColor textBackgroundColor() const;
    QFont currentFont() const;
    BobUI::Alignment alignment() const;

    void mergeCurrentCharFormat(const BOBUIextCharFormat &modifier);

    void setCurrentCharFormat(const BOBUIextCharFormat &format);
    BOBUIextCharFormat currentCharFormat() const;

    AutoFormatting autoFormatting() const;
    void setAutoFormatting(AutoFormatting features);

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

    LineWrapMode lineWrapMode() const;
    void setLineWrapMode(LineWrapMode mode);

    int lineWrapColumnOrWidth() const;
    void setLineWrapColumnOrWidth(int w);

    BOBUIextOption::WrapMode wordWrapMode() const;
    void setWordWrapMode(BOBUIextOption::WrapMode policy);

    bool find(const QString &exp, BOBUIextDocument::FindFlags options = BOBUIextDocument::FindFlags());
#if BOBUI_CONFIG(regularexpression)
    bool find(const QRegularExpression &exp, BOBUIextDocument::FindFlags options = BOBUIextDocument::FindFlags());
#endif

    QString toPlainText() const;
#ifndef BOBUI_NO_TEXTHTMLPARSER
    QString toHtml() const;
#endif
#if BOBUI_CONFIG(textmarkdownwriter)
    QString toMarkdown(BOBUIextDocument::MarkdownFeatures features = BOBUIextDocument::MarkdownDialectGitHub) const;
#endif

    void ensureCursorVisible();

    Q_INVOKABLE virtual QVariant loadResource(int type, const QUrl &name);
#ifndef BOBUI_NO_CONTEXTMENU
    QMenu *createStandardContextMenu();
    QMenu *createStandardContextMenu(const QPoint &position);
#endif

    BOBUIextCursor cursorForPosition(const QPoint &pos) const;
    QRect cursorRect(const BOBUIextCursor &cursor) const;
    QRect cursorRect() const;

    QString anchorAt(const QPoint& pos) const;

    bool overwriteMode() const;
    void setOverwriteMode(bool overwrite);

    qreal tabStopDistance() const;
    void setTabStopDistance(qreal distance);

    int cursorWidth() const;
    void setCursorWidth(int width);

    bool acceptRichText() const;
    void setAcceptRichText(bool accept);

    struct ExtraSelection
    {
        BOBUIextCursor cursor;
        BOBUIextCharFormat format;
    };
    void setExtraSelections(const QList<ExtraSelection> &selections);
    QList<ExtraSelection> extraSelections() const;

    void moveCursor(BOBUIextCursor::MoveOperation operation, BOBUIextCursor::MoveMode mode = BOBUIextCursor::MoveAnchor);

    bool canPaste() const;

    void print(QPagedPaintDevice *printer) const;

    QVariant inputMethodQuery(BobUI::InputMethodQuery property) const override;
    Q_INVOKABLE QVariant inputMethodQuery(BobUI::InputMethodQuery query, QVariant argument) const;

public Q_SLOTS:
    void setFontPointSize(qreal s);
    void setFontFamily(const QString &fontFamily);
    void setFontWeight(int w);
    void setFontUnderline(bool b);
    void setFontItalic(bool b);
    void setTextColor(const QColor &c);
    void setTextBackgroundColor(const QColor &c);
    void setCurrentFont(const QFont &f);
    void setAlignment(BobUI::Alignment a);

    void setPlainText(const QString &text);
#ifndef BOBUI_NO_TEXTHTMLPARSER
    void setHtml(const QString &text);
#endif
#if BOBUI_CONFIG(textmarkdownreader)
    void setMarkdown(const QString &markdown);
#endif
    void setText(const QString &text);

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
#ifndef BOBUI_NO_TEXTHTMLPARSER
    void insertHtml(const QString &text);
#endif // BOBUI_NO_TEXTHTMLPARSER

    void append(const QString &text);

    void scrollToAnchor(const QString &name);

    void zoomIn(int range = 1);
    void zoomOut(int range = 1);

Q_SIGNALS:
    void textChanged();
    void undoAvailable(bool b);
    void redoAvailable(bool b);
    void currentCharFormatChanged(const BOBUIextCharFormat &format);
    void copyAvailable(bool b);
    void selectionChanged();
    void cursorPositionChanged();

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

    BOBUIextEdit(BOBUIextEditPrivate &dd, QWidget *parent);

    virtual void scrollContentsBy(int dx, int dy) override;
    virtual void doSetTextCursor(const BOBUIextCursor &cursor);

    void zoomInF(float range);

private:
    Q_DISABLE_COPY(BOBUIextEdit)
    friend class BOBUIextEditControl;
    friend class BOBUIextDocument;
    friend class QWidgetTextControl;
    friend class ::tst_BOBUIextEdit;
};

Q_DECLARE_OPERATORS_FOR_FLAGS(BOBUIextEdit::AutoFormatting)

BOBUI_END_NAMESPACE

#endif // BOBUIEXTEDIT_H
