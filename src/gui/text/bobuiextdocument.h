// Copyright (C) 2019 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef BOBUIEXTDOCUMENT_H
#define BOBUIEXTDOCUMENT_H

#include <BobUIGui/bobuiguiglobal.h>
#include <BobUICore/qobject.h>
#include <BobUICore/qsize.h>
#include <BobUICore/qrect.h>
#include <BobUICore/qvariant.h>
#include <BobUIGui/qfont.h>
#include <BobUICore/qurl.h>
#include <BobUICore/qcontainerfwd.h>
Q_MOC_INCLUDE(<BobUIGui/bobuiextcursor.h>)

BOBUI_BEGIN_NAMESPACE


class BOBUIextFormatCollection;
class BOBUIextListFormat;
class QRect;
class QPainter;
class QPagedPaintDevice;
class QAbstractTextDocumentLayout;
class QPoint;
class BOBUIextObject;
class BOBUIextFormat;
class BOBUIextFrame;
class BOBUIextBlock;
class QVariant;
class QRectF;
class BOBUIextOption;
class BOBUIextCursor;

namespace BobUI
{
#if BOBUI_GUI_REMOVED_SINCE(6, 7)
    Q_GUI_EXPORT bool mightBeRichText(const QString&);
#endif
    Q_GUI_EXPORT bool mightBeRichText(QAnyStringView);
    Q_GUI_EXPORT QString convertFromPlainText(const QString &plain, WhiteSpaceMode mode = WhiteSpacePre);
}

class Q_GUI_EXPORT QAbstractUndoItem
{
public:
    virtual ~QAbstractUndoItem();
    virtual void undo() = 0;
    virtual void redo() = 0;
};

class BOBUIextDocumentPrivate;

class Q_GUI_EXPORT BOBUIextDocument : public QObject
{
    Q_OBJECT

    Q_PROPERTY(bool undoRedoEnabled READ isUndoRedoEnabled WRITE setUndoRedoEnabled)
    Q_PROPERTY(bool modified READ isModified WRITE setModified DESIGNABLE false)
    Q_PROPERTY(QSizeF pageSize READ pageSize WRITE setPageSize)
    Q_PROPERTY(QFont defaultFont READ defaultFont WRITE setDefaultFont)
    Q_PROPERTY(bool useDesignMetrics READ useDesignMetrics WRITE setUseDesignMetrics)
    Q_PROPERTY(bool layoutEnabled READ isLayoutEnabled WRITE setLayoutEnabled)
    Q_PROPERTY(QSizeF size READ size)
    Q_PROPERTY(qreal textWidth READ textWidth WRITE setTextWidth)
    Q_PROPERTY(int blockCount READ blockCount)
    Q_PROPERTY(qreal indentWidth READ indentWidth WRITE setIndentWidth)
#ifndef BOBUI_NO_CSSPARSER
    Q_PROPERTY(QString defaultStyleSheet READ defaultStyleSheet WRITE setDefaultStyleSheet)
#endif
    Q_PROPERTY(int maximumBlockCount READ maximumBlockCount WRITE setMaximumBlockCount)
    Q_PROPERTY(qreal documentMargin READ documentMargin WRITE setDocumentMargin)
    QDOC_PROPERTY(BOBUIextOption defaultTextOption READ defaultTextOption WRITE setDefaultTextOption)
    Q_PROPERTY(QUrl baseUrl READ baseUrl WRITE setBaseUrl NOTIFY baseUrlChanged)

public:
    explicit BOBUIextDocument(QObject *parent = nullptr);
    explicit BOBUIextDocument(const QString &text, QObject *parent = nullptr);
    ~BOBUIextDocument();

    BOBUIextDocument *clone(QObject *parent = nullptr) const;

    bool isEmpty() const;
    virtual void clear();

    void setUndoRedoEnabled(bool enable);
    bool isUndoRedoEnabled() const;

    bool isUndoAvailable() const;
    bool isRedoAvailable() const;

    int availableUndoSteps() const;
    int availableRedoSteps() const;

    int revision() const;

    void setDocumentLayout(QAbstractTextDocumentLayout *layout);
    QAbstractTextDocumentLayout *documentLayout() const;

    enum MetaInformation {
        DocumentTitle,
        DocumentUrl,
        CssMedia,
        FrontMatter,
    };
    void setMetaInformation(MetaInformation info, const QString &);
    QString metaInformation(MetaInformation info) const;

#ifndef BOBUI_NO_TEXTHTMLPARSER
    QString toHtml() const;
    void setHtml(const QString &html);
#endif

#if BOBUI_CONFIG(textmarkdownwriter) || BOBUI_CONFIG(textmarkdownreader)
    enum MarkdownFeature {
        MarkdownNoHTML = 0x0020 | 0x0040,
        MarkdownDialectCommonMark = 0,
        MarkdownDialectGitHub = 0x0004 | 0x0008 | 0x0400 | 0x0100 | 0x0200 | 0x0800 | 0x4000 | 0x100000
    };
    Q_DECLARE_FLAGS(MarkdownFeatures, MarkdownFeature)
    Q_FLAG(MarkdownFeatures)
#endif

#if BOBUI_CONFIG(textmarkdownwriter)
    QString toMarkdown(MarkdownFeatures features = MarkdownDialectGitHub) const;
#endif

#if BOBUI_CONFIG(textmarkdownreader)
    void setMarkdown(const QString &markdown, MarkdownFeatures features = MarkdownDialectGitHub);
#endif

    QString toRawText() const;
    QString toPlainText() const;
    void setPlainText(const QString &text);

    QChar characterAt(int pos) const;

    enum FindFlag
    {
        FindBackward        = 0x00001,
        FindCaseSensitively = 0x00002,
        FindWholeWords      = 0x00004
    };
    Q_DECLARE_FLAGS(FindFlags, FindFlag)

    BOBUIextCursor find(const QString &subString, int from = 0, FindFlags options = FindFlags()) const;
    BOBUIextCursor find(const QString &subString, const BOBUIextCursor &cursor, FindFlags options = FindFlags()) const;

#if BOBUI_CONFIG(regularexpression)
    BOBUIextCursor find(const QRegularExpression &expr, int from = 0, FindFlags options = FindFlags()) const;
    BOBUIextCursor find(const QRegularExpression &expr, const BOBUIextCursor &cursor, FindFlags options = FindFlags()) const;
#endif

    BOBUIextFrame *frameAt(int pos) const;
    BOBUIextFrame *rootFrame() const;

    BOBUIextObject *object(int objectIndex) const;
    BOBUIextObject *objectForFormat(const BOBUIextFormat &) const;

    BOBUIextBlock findBlock(int pos) const;
    BOBUIextBlock findBlockByNumber(int blockNumber) const;
    BOBUIextBlock findBlockByLineNumber(int blockNumber) const;
    BOBUIextBlock begin() const;
    BOBUIextBlock end() const;

    BOBUIextBlock firstBlock() const;
    BOBUIextBlock lastBlock() const;

    void setPageSize(const QSizeF &size);
    QSizeF pageSize() const;

    void setDefaultFont(const QFont &font);
    QFont defaultFont() const;

    void setSuperScriptBaseline(qreal baseline);
    qreal superScriptBaseline() const;

    void setSubScriptBaseline(qreal baseline);
    qreal subScriptBaseline() const;

    void setBaselineOffset(qreal baseline);
    qreal baselineOffset() const;

    int pageCount() const;

    bool isModified() const;

#ifndef BOBUI_NO_PRINTER
    void print(QPagedPaintDevice *printer) const;
#endif

    enum ResourceType {
        UnknownResource = 0,
        HtmlResource  = 1,
        ImageResource = 2,
        StyleSheetResource = 3,
        MarkdownResource = 4,

        UserResource  = 100
    };
    Q_ENUM(ResourceType)

    QVariant resource(int type, const QUrl &name) const;
    void addResource(int type, const QUrl &name, const QVariant &resource);

    using ResourceProvider = std::function<QVariant(const QUrl&)>;

    BOBUIextDocument::ResourceProvider resourceProvider() const;
    void setResourceProvider(const ResourceProvider &provider);

    static BOBUIextDocument::ResourceProvider defaultResourceProvider();
    static void setDefaultResourceProvider(const ResourceProvider &provider);

    QList<BOBUIextFormat> allFormats() const;

    void markContentsDirty(int from, int length);

    void setUseDesignMetrics(bool b);
    bool useDesignMetrics() const;

    void setLayoutEnabled(bool b);
    bool isLayoutEnabled() const;

    void drawContents(QPainter *painter, const QRectF &rect = QRectF());

    void setTextWidth(qreal width);
    qreal textWidth() const;

    qreal idealWidth() const;

    qreal indentWidth() const;
    void setIndentWidth(qreal width);

    qreal documentMargin() const;
    void setDocumentMargin(qreal margin);

    void adjustSize();
    QSizeF size() const;

    int blockCount() const;
    int lineCount() const;
    int characterCount() const;

#ifndef BOBUI_NO_CSSPARSER
    void setDefaultStyleSheet(const QString &sheet);
    QString defaultStyleSheet() const;
#endif

    void undo(BOBUIextCursor *cursor);
    void redo(BOBUIextCursor *cursor);

    enum Stacks {
        UndoStack = 0x01,
        RedoStack = 0x02,
        UndoAndRedoStacks = UndoStack | RedoStack
    };
    void clearUndoRedoStacks(Stacks historyToClear = UndoAndRedoStacks);

    int maximumBlockCount() const;
    void setMaximumBlockCount(int maximum);

    BOBUIextOption defaultTextOption() const;
    void setDefaultTextOption(const BOBUIextOption &option);

    QUrl baseUrl() const;
    void setBaseUrl(const QUrl &url);

    BobUI::CursorMoveStyle defaultCursorMoveStyle() const;
    void setDefaultCursorMoveStyle(BobUI::CursorMoveStyle style);

Q_SIGNALS:
    void contentsChange(int from, int charsRemoved, int charsAdded);
    void contentsChanged();
    void undoAvailable(bool);
    void redoAvailable(bool);
    void undoCommandAdded();
    void modificationChanged(bool m);
    void cursorPositionChanged(const BOBUIextCursor &cursor);
    void blockCountChanged(int newBlockCount);
    void baseUrlChanged(const QUrl &url);
    void documentLayoutChanged();

public Q_SLOTS:
    void undo();
    void redo();
    void appendUndoItem(QAbstractUndoItem *);
    void setModified(bool m = true);

protected:
    virtual BOBUIextObject *createObject(const BOBUIextFormat &f);
    Q_INVOKABLE virtual QVariant loadResource(int type, const QUrl &name);

    BOBUIextDocument(BOBUIextDocumentPrivate &dd, QObject *parent);
private:
    Q_DISABLE_COPY(BOBUIextDocument)
    Q_DECLARE_PRIVATE(BOBUIextDocument)
    friend class BOBUIextObjectPrivate;
};

Q_DECLARE_OPERATORS_FOR_FLAGS(BOBUIextDocument::FindFlags)

BOBUI_END_NAMESPACE

#endif // BOBUIEXTDOCUMENT_H
