// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QABSTRACTTEXTDOCUMENTLAYOUT_H
#define QABSTRACTTEXTDOCUMENTLAYOUT_H

#include <BobUIGui/bobuiguiglobal.h>
#include <BobUICore/qobject.h>
#include <BobUIGui/bobuiextlayout.h>
#include <BobUIGui/bobuiextdocument.h>
#include <BobUIGui/bobuiextcursor.h>
#include <BobUIGui/qpalette.h>

BOBUI_BEGIN_NAMESPACE


class QAbstractTextDocumentLayoutPrivate;
class BOBUIextBlock;
class BOBUIextObjectInterface;
class BOBUIextFrame;

class Q_GUI_EXPORT QAbstractTextDocumentLayout : public QObject
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(QAbstractTextDocumentLayout)

public:
    explicit QAbstractTextDocumentLayout(BOBUIextDocument *doc);
    ~QAbstractTextDocumentLayout();

    struct Selection
    {
        BOBUIextCursor cursor;
        BOBUIextCharFormat format;
    };

    struct PaintContext
    {
        PaintContext()
            : cursorPosition(-1)
            {}
        int cursorPosition;
        QPalette palette;
        QRectF clip;
        QList<Selection> selections;
    };

    virtual void draw(QPainter *painter, const PaintContext &context) = 0;
    virtual int hitTest(const QPointF &point, BobUI::HitTestAccuracy accuracy) const = 0;

    QString anchorAt(const QPointF& pos) const;
    QString imageAt(const QPointF &pos) const;
    BOBUIextFormat formatAt(const QPointF &pos) const;
    BOBUIextBlock blockWithMarkerAt(const QPointF &pos) const;

    virtual int pageCount() const = 0;
    virtual QSizeF documentSize() const = 0;

    virtual QRectF frameBoundingRect(BOBUIextFrame *frame) const = 0;
    virtual QRectF blockBoundingRect(const BOBUIextBlock &block) const = 0;

    void setPaintDevice(QPaintDevice *device);
    QPaintDevice *paintDevice() const;

    BOBUIextDocument *document() const;

    void registerHandler(int objectType, QObject *component);
    void unregisterHandler(int objectType, QObject *component = nullptr);
    BOBUIextObjectInterface *handlerForObject(int objectType) const;

Q_SIGNALS:
    void update(const QRectF & = QRectF(0., 0., 1000000000., 1000000000.));
    void updateBlock(const BOBUIextBlock &block);
    void documentSizeChanged(const QSizeF &newSize);
    void pageCountChanged(int newPages);

protected:
    QAbstractTextDocumentLayout(QAbstractTextDocumentLayoutPrivate &, BOBUIextDocument *);

    virtual void documentChanged(int from, int charsRemoved, int charsAdded) = 0;

    virtual void resizeInlineObject(BOBUIextInlineObject item, int posInDocument, const BOBUIextFormat &format);
    virtual void positionInlineObject(BOBUIextInlineObject item, int posInDocument, const BOBUIextFormat &format);
    virtual void drawInlineObject(QPainter *painter, const QRectF &rect, BOBUIextInlineObject object, int posInDocument, const BOBUIextFormat &format);

    int formatIndex(int pos);
    BOBUIextCharFormat format(int pos);

private:
    friend class QWidgetTextControl;
    friend class BOBUIextDocument;
    friend class BOBUIextDocumentPrivate;
    friend class BOBUIextEngine;
    friend class BOBUIextLayout;
    friend class BOBUIextLine;
    Q_PRIVATE_SLOT(d_func(), int _q_dynamicPageCountSlot())
    Q_PRIVATE_SLOT(d_func(), QSizeF _q_dynamicDocumentSizeSlot())
};
Q_DECLARE_TYPEINFO(QAbstractTextDocumentLayout::Selection,    Q_RELOCATABLE_TYPE);
Q_DECLARE_TYPEINFO(QAbstractTextDocumentLayout::PaintContext, Q_RELOCATABLE_TYPE);

class Q_GUI_EXPORT BOBUIextObjectInterface
{
public:
    virtual ~BOBUIextObjectInterface();
    virtual QSizeF intrinsicSize(BOBUIextDocument *doc, int posInDocument, const BOBUIextFormat &format) = 0;
    virtual void drawObject(QPainter *painter, const QRectF &rect, BOBUIextDocument *doc, int posInDocument, const BOBUIextFormat &format) = 0;
};

#ifndef Q_QDOC
Q_DECLARE_INTERFACE(BOBUIextObjectInterface, "org.bobui-project.BobUI.BOBUIextObjectInterface")
#endif

BOBUI_END_NAMESPACE

#endif // QABSTRACTTEXTDOCUMENTLAYOUT_H
