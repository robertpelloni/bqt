// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef BOBUIEXTDOCUMENT_P_H
#define BOBUIEXTDOCUMENT_P_H

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

#include <BobUIGui/private/bobuiguiglobal_p.h>
#include "BobUIGui/bobuiextcursor.h"
#include "BobUIGui/bobuiextdocument.h"
#include "BobUIGui/bobuiextlayout.h"
#include "BobUIGui/bobuiextobject.h"
#include "BobUIGui/bobuiextoption.h"

#include "BobUICore/qlist.h"
#include "BobUICore/qmap.h"
#include "BobUICore/qset.h"
#include "BobUICore/qstring.h"
#include "BobUICore/qurl.h"
#include "BobUICore/qvariant.h"

#if BOBUI_CONFIG(cssparser)
#include "private/qcssparser_p.h"
#endif
#include "private/qfragmentmap_p.h"
#include "private/qobject_p.h"
#include "private/bobuiextformat_p.h"

// #define BOBUI_QMAP_DEBUG

#ifdef BOBUI_QMAP_DEBUG
#include <iostream>
#endif

BOBUI_BEGIN_NAMESPACE

class BOBUIextFormatCollection;
class BOBUIextFormat;
class BOBUIextBlockFormat;
class BOBUIextCursorPrivate;
class QAbstractTextDocumentLayout;
class BOBUIextDocument;
class BOBUIextFrame;

#define BOBUIextBeginningOfFrame QChar(u'\xfdd0')
#define BOBUIextEndOfFrame QChar(u'\xfdd1')

class BOBUIextFragmentData : public QFragment<>
{
public:
    inline void initialize() {}
    inline void invalidate() const {}
    inline void free() {}
    int stringPosition;
    int format;
};

class BOBUIextBlockData : public QFragment<3>
{
public:
    inline void initialize()
        { layout = nullptr; userData = nullptr; userState = -1; revision = 0; hidden = 0; }
    void invalidate() const;
    inline void free()
    { delete layout; layout = nullptr; delete userData; userData = nullptr; }

    mutable int format;
    // ##### probably store a BOBUIextEngine * here!
    mutable BOBUIextLayout *layout;
    mutable BOBUIextBlockUserData *userData;
    mutable int userState;
    mutable signed int revision : 31;
    mutable uint hidden : 1;
};


class QAbstractUndoItem;

class BOBUIextUndoCommand
{
public:
    enum Command {
        Inserted = 0,
        Removed = 1,
        CharFormatChanged = 2,
        BlockFormatChanged = 3,
        BlockInserted = 4,
        BlockRemoved = 5,
        BlockAdded = 6,
        BlockDeleted = 7,
        GroupFormatChange = 8,
        CursorMoved = 9,
        Custom = 256
    };
    enum Operation {
        KeepCursor = 0,
        MoveCursor = 1
    };
    quint16 command;
    uint block_part : 1; // all commands that are part of an undo block (including the first and the last one) have this set to 1
    uint block_end : 1; // the last command in an undo block has this set to 1.
    uint block_padding : 6; // padding since block used to be a quint8
    quint8 operation;
    int format;
    quint32 strPos;
    quint32 pos;
    union {
        int blockFormat;
        quint32 length;
        QAbstractUndoItem *custom;
        int objectIndex;
    };
    quint32 revision;

    bool tryMerge(const BOBUIextUndoCommand &other);
};
Q_DECLARE_TYPEINFO(BOBUIextUndoCommand, Q_PRIMITIVE_TYPE);

class Q_GUI_EXPORT BOBUIextDocumentPrivate : public QObjectPrivate
{
    Q_DECLARE_PUBLIC(BOBUIextDocument)
public:
    typedef QFragmentMap<BOBUIextFragmentData> FragmentMap;
    typedef FragmentMap::ConstIterator FragmentIterator;
    typedef QFragmentMap<BOBUIextBlockData> BlockMap;

    BOBUIextDocumentPrivate();
    ~BOBUIextDocumentPrivate();

    void init();
    void clear();

    void setLayout(QAbstractTextDocumentLayout *layout);

    void insert(int pos, QStringView text, int format);
    void insert(int pos, QChar c, int format)
    { insert(pos, QStringView(&c, 1), format); }
    void insert(int pos, int strPos, int strLength, int format);
    int insertBlock(int pos, int blockFormat, int charFormat, BOBUIextUndoCommand::Operation = BOBUIextUndoCommand::MoveCursor);
    int insertBlock(QChar blockSeparator, int pos, int blockFormat, int charFormat,
                     BOBUIextUndoCommand::Operation op = BOBUIextUndoCommand::MoveCursor);

    void move(int from, int to, int length, BOBUIextUndoCommand::Operation = BOBUIextUndoCommand::MoveCursor);
    void remove(int pos, int length, BOBUIextUndoCommand::Operation = BOBUIextUndoCommand::MoveCursor);

    void aboutToRemoveCell(int cursorFrom, int cursorEnd);

    BOBUIextFrame *insertFrame(int start, int end, const BOBUIextFrameFormat &format);
    void removeFrame(BOBUIextFrame *frame);

    enum FormatChangeMode { MergeFormat, SetFormat, SetFormatAndPreserveObjectIndices };

    void setCharFormat(int pos, int length, const BOBUIextCharFormat &newFormat, FormatChangeMode mode = SetFormat);
    void setBlockFormat(const BOBUIextBlock &from, const BOBUIextBlock &to,
                        const BOBUIextBlockFormat &newFormat, FormatChangeMode mode = SetFormat);

    void emitUndoAvailable(bool available);
    void emitRedoAvailable(bool available);

    int undoRedo(bool undo);
    inline void undo() { undoRedo(true); }
    inline void redo() { undoRedo(false); }
    void appendUndoItem(QAbstractUndoItem *);
    inline void beginEditBlock() { if (0 == editBlock++) ++revision; }
    void joinPreviousEditBlock();
    void endEditBlock();
    void finishEdit();
    inline bool isInEditBlock() const { return editBlock; }
    void enableUndoRedo(bool enable);
    inline bool isUndoRedoEnabled() const { return undoEnabled; }

    inline bool isUndoAvailable() const { return undoEnabled && undoState > 0; }
    inline bool isRedoAvailable() const { return undoEnabled && undoState < undoStack.size(); }

    inline int availableUndoSteps() const { return undoEnabled ? undoState : 0; }
    inline int availableRedoSteps() const { return undoEnabled ? qMax(undoStack.size() - undoState - 1, 0) : 0; }

    inline QString buffer() const { return text; }
    QString plainText() const;
    inline int length() const { return fragments.length(); }

    inline BOBUIextFormatCollection *formatCollection() { return &formats; }
    inline const BOBUIextFormatCollection *formatCollection() const { return &formats; }
    inline QAbstractTextDocumentLayout *layout() const { return lout; }

    inline FragmentIterator find(int pos) const { return fragments.find(pos); }
    inline FragmentIterator begin() const { return fragments.begin(); }
    inline FragmentIterator end() const { return fragments.end(); }

    inline BOBUIextBlock blocksBegin() const { return BOBUIextBlock(const_cast<BOBUIextDocumentPrivate *>(this), blocks.firstNode()); }
    inline BOBUIextBlock blocksEnd() const { return BOBUIextBlock(const_cast<BOBUIextDocumentPrivate *>(this), 0); }
    inline BOBUIextBlock blocksFind(int pos) const { return BOBUIextBlock(const_cast<BOBUIextDocumentPrivate *>(this), blocks.findNode(pos)); }
    int blockCharFormatIndex(int node) const;

    inline int numBlocks() const { return blocks.numNodes(); }

    const BlockMap &blockMap() const { return blocks; }
    const FragmentMap &fragmentMap() const { return fragments; }
    BlockMap &blockMap() { return blocks; }
    FragmentMap &fragmentMap() { return fragments; }

    static const BOBUIextBlockData *block(const BOBUIextBlock &it) { return it.p->blocks.fragment(it.n); }

    int nextCursorPosition(int position, BOBUIextLayout::CursorMode mode) const;
    int previousCursorPosition(int position, BOBUIextLayout::CursorMode mode) const;
    int leftCursorPosition(int position) const;
    int rightCursorPosition(int position) const;

    void changeObjectFormat(BOBUIextObject *group, int format);

    void setModified(bool m);
    inline bool isModified() const { return modified; }

    inline QFont defaultFont() const { return formats.defaultFont(); }
    inline void setDefaultFont(const QFont &f) { formats.setDefaultFont(f); }

    void clearUndoRedoStacks(BOBUIextDocument::Stacks stacksToClear, bool emitSignals = false);

private:
    bool split(int pos);
    bool unite(uint f);

    void insert_string(int pos, uint strPos, uint length, int format, BOBUIextUndoCommand::Operation op);
    int insert_block(int pos, uint strPos, int format, int blockformat, BOBUIextUndoCommand::Operation op, int command);
    int remove_string(int pos, uint length, BOBUIextUndoCommand::Operation op);
    int remove_block(int pos, int *blockformat, int command, BOBUIextUndoCommand::Operation op);

    void insert_frame(BOBUIextFrame *f);
    void scan_frames(int pos, int charsRemoved, int charsAdded);
    static void clearFrame(BOBUIextFrame *f);

    void adjustDocumentChangesAndCursors(int from, int addedOrRemoved, BOBUIextUndoCommand::Operation op);

    bool wasUndoAvailable;
    bool wasRedoAvailable;

public:
    void documentChange(int from, int length);

    void addCursor(BOBUIextCursorPrivate *c);
    void removeCursor(BOBUIextCursorPrivate *c);

    BOBUIextFrame *frameAt(int pos) const;
    BOBUIextFrame *rootFrame() const;

    BOBUIextObject *objectForIndex(int objectIndex) const;
    BOBUIextObject *objectForFormat(int formatIndex) const;
    BOBUIextObject *objectForFormat(const BOBUIextFormat &f) const;

    BOBUIextObject *createObject(const BOBUIextFormat &newFormat, int objectIndex = -1);
    void deleteObject(BOBUIextObject *object);

    BOBUIextDocument *document() { return q_func(); }
    const BOBUIextDocument *document() const { return q_func(); }

    bool ensureMaximumBlockCount();

    static inline const BOBUIextDocumentPrivate *get(const BOBUIextDocument *document)
    {
        return document->d_func();
    }

    static inline BOBUIextDocumentPrivate *get(BOBUIextDocument *document)
    {
        return document->d_func();
    }

    static inline BOBUIextDocumentPrivate *get(BOBUIextBlock &block)
    {
        return block.p;
    }

    static inline const BOBUIextDocumentPrivate *get(const BOBUIextBlock &block)
    {
        return block.p;
    }

    static inline BOBUIextDocumentPrivate *get(BOBUIextObject *object)
    {
        return get(object->document());
    }

    static inline const BOBUIextDocumentPrivate *get(const BOBUIextObject *object)
    {
        return get(object->document());
    }

    bool canLayout() const { return layoutEnabled && !pageSize.isNull(); }

private:
    BOBUIextDocumentPrivate(const BOBUIextDocumentPrivate& m);
    BOBUIextDocumentPrivate& operator= (const BOBUIextDocumentPrivate& m);

    void appendUndoItem(const BOBUIextUndoCommand &c);

    void contentsChanged();

    void compressPieceTable();

    QString text;
    uint unreachableCharacterCount;

    QList<BOBUIextUndoCommand> undoStack;
    bool undoEnabled;
    int undoState;
    int revision;
    // position in undo stack of the last setModified(false) call
    int modifiedState;
    bool modified;

    int editBlock;
    int editBlockCursorPosition;
    int docChangeFrom;
    int docChangeOldLength;
    int docChangeLength;
    bool framesDirty;

    BOBUIextFormatCollection formats;
    mutable BOBUIextFrame *rtFrame;
    QAbstractTextDocumentLayout *lout;
    FragmentMap fragments;
    BlockMap blocks;
    int initialBlockCharFormatIndex;

    QSet<BOBUIextCursorPrivate *> cursors;
    QMap<int, BOBUIextObject *> objects;
    QMap<QUrl, QVariant> resources;
    QMap<QUrl, QVariant> cachedResources;
    BOBUIextDocument::ResourceProvider resourceProvider;
    QString defaultStyleSheet;

    int lastBlockCount;

public:
    bool inContentsChange;
    bool layoutEnabled = true;
    BOBUIextOption defaultTextOption;
    BobUI::CursorMoveStyle defaultCursorMoveStyle;
#ifndef BOBUI_NO_CSSPARSER
    QCss::StyleSheet parsedDefaultStyleSheet;
#endif
    int maximumBlockCount;
    uint needsEnsureMaximumBlockCount : 1;
    uint blockCursorAdjustment : 1;
    QSizeF pageSize;
    QString title;
    QString url;
    QString cssMedia;
    QString frontMatter;
    qreal indentWidth;
    qreal documentMargin;
    QUrl baseUrl;

    void mergeCachedResources(const BOBUIextDocumentPrivate *priv);

    friend struct BOBUIextHtmlParserNode;
    friend class BOBUIextHtmlExporter;
    friend class BOBUIextCursor;
};

class BOBUIextTable;
class BOBUIextHtmlExporter
{
public:
    BOBUIextHtmlExporter(const BOBUIextDocument *_doc);

    enum ExportMode {
        ExportEntireDocument,
        ExportFragment
    };

    QString toHtml(ExportMode mode = ExportEntireDocument);

private:
    enum StyleMode { EmitStyleTag, OmitStyleTag };
    enum FrameType { TextFrame, TableFrame, RootFrame };

    void emitFrame(const BOBUIextFrame::Iterator &frameIt);
    void emitTextFrame(const BOBUIextFrame *frame);
    void emitBlock(const BOBUIextBlock &block);
    void emitTable(const BOBUIextTable *table);
    void emitFragment(const BOBUIextFragment &fragment);

    void emitBlockAttributes(const BOBUIextBlock &block);
    bool emitCharFormatStyle(const BOBUIextCharFormat &format);
    void emitTextLength(const char *attribute, const BOBUIextLength &length);
    void emitAlignment(BobUI::Alignment alignment);
    void emitFloatStyle(BOBUIextFrameFormat::Position pos, StyleMode mode = EmitStyleTag);
    void emitMargins(const QString &top, const QString &bottom, const QString &left, const QString &right);
    void emitAttribute(const char *attribute, const QString &value);
    void emitFrameStyle(const BOBUIextFrameFormat &format, FrameType frameType);
    void emitBorderStyle(BOBUIextFrameFormat::BorderStyle style);
    void emitPageBreakPolicy(BOBUIextFormat::PageBreakFlags policy);

    void emitFontFamily(const QStringList &families);

    void emitBackgroundAttribute(const BOBUIextFormat &format);
    QString findUrlForImage(const BOBUIextDocument *doc, qint64 cacheKey, bool isPixmap);

    QString html;
    BOBUIextCharFormat defaultCharFormat;
    const BOBUIextDocument *doc;
    bool fragmentMarkers;
    QStringList closingTags;
};

BOBUI_END_NAMESPACE

#endif // BOBUIEXTDOCUMENT_P_H
