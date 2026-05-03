// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef BOBUIEXTOBJECT_H
#define BOBUIEXTOBJECT_H

#include <BobUIGui/bobuiguiglobal.h>
#include <BobUICore/qobject.h>
#include <BobUIGui/bobuiextformat.h>
#include <BobUIGui/bobuiextlayout.h>
#include <BobUIGui/qglyphrun.h>

BOBUI_BEGIN_NAMESPACE


class BOBUIextObjectPrivate;
class BOBUIextDocument;
class BOBUIextDocumentPrivate;
class BOBUIextCursor;
class BOBUIextBlock;
class BOBUIextFragment;
class BOBUIextList;

class Q_GUI_EXPORT BOBUIextObject : public QObject
{
    Q_OBJECT

protected:
    explicit BOBUIextObject(BOBUIextDocument *doc);
    ~BOBUIextObject();

    void setFormat(const BOBUIextFormat &format);

public:
    BOBUIextFormat format() const;
    int formatIndex() const;

    BOBUIextDocument *document() const;

    int objectIndex() const;

protected:
    BOBUIextObject(BOBUIextObjectPrivate &p, BOBUIextDocument *doc);

private:
    Q_DECLARE_PRIVATE(BOBUIextObject)
    Q_DISABLE_COPY(BOBUIextObject)
    friend class BOBUIextDocumentPrivate;
};

class BOBUIextBlockGroupPrivate;
class Q_GUI_EXPORT BOBUIextBlockGroup : public BOBUIextObject
{
    Q_OBJECT

protected:
    explicit BOBUIextBlockGroup(BOBUIextDocument *doc);
    ~BOBUIextBlockGroup();

    virtual void blockInserted(const BOBUIextBlock &block);
    virtual void blockRemoved(const BOBUIextBlock &block);
    virtual void blockFormatChanged(const BOBUIextBlock &block);

    QList<BOBUIextBlock> blockList() const;

protected:
    BOBUIextBlockGroup(BOBUIextBlockGroupPrivate &p, BOBUIextDocument *doc);
private:
    Q_DECLARE_PRIVATE(BOBUIextBlockGroup)
    Q_DISABLE_COPY(BOBUIextBlockGroup)
    friend class BOBUIextDocumentPrivate;
};

class Q_GUI_EXPORT BOBUIextFrameLayoutData {
public:
    virtual ~BOBUIextFrameLayoutData();
};

class BOBUIextFramePrivate;
class Q_GUI_EXPORT BOBUIextFrame : public BOBUIextObject
{
    Q_OBJECT

public:
    explicit BOBUIextFrame(BOBUIextDocument *doc);
    ~BOBUIextFrame();

    inline void setFrameFormat(const BOBUIextFrameFormat &format);
    BOBUIextFrameFormat frameFormat() const { return BOBUIextObject::format().toFrameFormat(); }

    BOBUIextCursor firstCursorPosition() const;
    BOBUIextCursor lastCursorPosition() const;
    int firstPosition() const;
    int lastPosition() const;

    BOBUIextFrameLayoutData *layoutData() const;
    void setLayoutData(BOBUIextFrameLayoutData *data);

    QList<BOBUIextFrame *> childFrames() const;
    BOBUIextFrame *parentFrame() const;

    class iterator {
        BOBUIextFrame *f = nullptr;
        int b = 0;
        int e = 0;
        BOBUIextFrame *cf = nullptr;
        int cb = 0;

        friend class BOBUIextFrame;
        friend class BOBUIextTableCell;
        friend class BOBUIextDocumentLayoutPrivate;
        inline iterator(BOBUIextFrame *frame, int block, int begin, int end)
            : f(frame), b(begin), e(end), cb(block)
        {}
    public:
        constexpr iterator() noexcept = default;
        BOBUIextFrame *parentFrame() const { return f; }

        BOBUIextFrame *currentFrame() const { return cf; }
        Q_GUI_EXPORT BOBUIextBlock currentBlock() const;

        bool atEnd() const { return !cf && cb == e; }

        inline bool operator==(const iterator &o) const { return f == o.f && cf == o.cf && cb == o.cb; }
        inline bool operator!=(const iterator &o) const { return f != o.f || cf != o.cf || cb != o.cb; }
        Q_GUI_EXPORT iterator &operator++();
        inline iterator operator++(int) { iterator tmp = *this; operator++(); return tmp; }
        Q_GUI_EXPORT iterator &operator--();
        inline iterator operator--(int) { iterator tmp = *this; operator--(); return tmp; }
    };

    friend class iterator;
    // more BobUI
    typedef iterator Iterator;

    iterator begin() const;
    iterator end() const;

protected:
    BOBUIextFrame(BOBUIextFramePrivate &p, BOBUIextDocument *doc);
private:
    friend class BOBUIextDocumentPrivate;
    Q_DECLARE_PRIVATE(BOBUIextFrame)
    Q_DISABLE_COPY(BOBUIextFrame)
};
Q_DECLARE_TYPEINFO(BOBUIextFrame::iterator, Q_RELOCATABLE_TYPE);

inline void BOBUIextFrame::setFrameFormat(const BOBUIextFrameFormat &aformat)
{ BOBUIextObject::setFormat(aformat); }

class Q_GUI_EXPORT BOBUIextBlockUserData {
public:
    virtual ~BOBUIextBlockUserData();
};

class Q_GUI_EXPORT BOBUIextBlock
{
    friend class QSyntaxHighlighter;
public:
    inline BOBUIextBlock(BOBUIextDocumentPrivate *priv, int b) : p(priv), n(b) {}
    inline BOBUIextBlock() : p(nullptr), n(0) {}
    inline BOBUIextBlock(const BOBUIextBlock &o) : p(o.p), n(o.n) {}
    inline BOBUIextBlock &operator=(const BOBUIextBlock &o) { p = o.p; n = o.n; return *this; }

    bool isValid() const;

    inline bool operator==(const BOBUIextBlock &o) const { return p == o.p && n == o.n; }
    inline bool operator!=(const BOBUIextBlock &o) const { return p != o.p || n != o.n; }
    inline bool operator<(const BOBUIextBlock &o) const { return position() < o.position(); }

    int position() const;
    int length() const;
    bool contains(int position) const;

    BOBUIextLayout *layout() const;
    void clearLayout();
    BOBUIextBlockFormat blockFormat() const;
    int blockFormatIndex() const;
    BOBUIextCharFormat charFormat() const;
    int charFormatIndex() const;

    BobUI::LayoutDirection textDirection() const;

    QString text() const;

    QList<BOBUIextLayout::FormatRange> textFormats() const;

    const BOBUIextDocument *document() const;

    BOBUIextList *textList() const;

    BOBUIextBlockUserData *userData() const;
    void setUserData(BOBUIextBlockUserData *data);

    int userState() const;
    void setUserState(int state);

    int revision() const;
    void setRevision(int rev);

    bool isVisible() const;
    void setVisible(bool visible);

    int blockNumber() const;
    int firstLineNumber() const;

    void setLineCount(int count);
    int lineCount() const;

    class iterator {
        const BOBUIextDocumentPrivate *p = nullptr;
        int b = 0;
        int e = 0;
        int n = 0;
        friend class BOBUIextBlock;
        iterator(const BOBUIextDocumentPrivate *priv, int begin, int end, int f)
            : p(priv), b(begin), e(end), n(f) {}
    public:
        constexpr iterator() = default;

        Q_GUI_EXPORT BOBUIextFragment fragment() const;

        bool atEnd() const { return n == e; }

        inline bool operator==(const iterator &o) const { return p == o.p && n == o.n; }
        inline bool operator!=(const iterator &o) const { return p != o.p || n != o.n; }
        Q_GUI_EXPORT iterator &operator++();
        inline iterator operator++(int) { iterator tmp = *this; operator++(); return tmp; }
        Q_GUI_EXPORT iterator &operator--();
        inline iterator operator--(int) { iterator tmp = *this; operator--(); return tmp; }
    };

    // more BobUI
    typedef iterator Iterator;

    iterator begin() const;
    iterator end() const;

    BOBUIextBlock next() const;
    BOBUIextBlock previous() const;

    inline int fragmentIndex() const { return n; }

private:
    BOBUIextDocumentPrivate *p;
    int n;
    friend class BOBUIextDocumentPrivate;
    friend class BOBUIextLayout;
};

Q_DECLARE_TYPEINFO(BOBUIextBlock, Q_RELOCATABLE_TYPE);
Q_DECLARE_TYPEINFO(BOBUIextBlock::iterator, Q_RELOCATABLE_TYPE);


class Q_GUI_EXPORT BOBUIextFragment
{
public:
    inline BOBUIextFragment(const BOBUIextDocumentPrivate *priv, int f, int fe) : p(priv), n(f), ne(fe) {}
    inline BOBUIextFragment() : p(nullptr), n(0), ne(0) {}
    inline BOBUIextFragment(const BOBUIextFragment &o) : p(o.p), n(o.n), ne(o.ne) {}
    inline BOBUIextFragment &operator=(const BOBUIextFragment &o) { p = o.p; n = o.n; ne = o.ne; return *this; }

    inline bool isValid() const { return p && n; }

    inline bool operator==(const BOBUIextFragment &o) const { return p == o.p && n == o.n; }
    inline bool operator!=(const BOBUIextFragment &o) const { return p != o.p || n != o.n; }
    inline bool operator<(const BOBUIextFragment &o) const { return position() < o.position(); }

    int position() const;
    int length() const;
    bool contains(int position) const;

    BOBUIextCharFormat charFormat() const;
    int charFormatIndex() const;
    QString text() const;

#if !defined(BOBUI_NO_RAWFONT)
    QList<QGlyphRun> glyphRuns(int from = -1, int length = -1) const;
#endif

private:
    const BOBUIextDocumentPrivate *p;
    int n;
    int ne;
};

Q_DECLARE_TYPEINFO(BOBUIextFragment, Q_RELOCATABLE_TYPE);

BOBUI_END_NAMESPACE

#endif // BOBUIEXTOBJECT_H
