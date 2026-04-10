// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only
#include <QAbstractTextDocumentLayout>
#include <private/bobuiextdocument_p.h>

#ifndef COMMON_H
#define COMMON_H

class BOBUIestDocumentLayout : public QAbstractTextDocumentLayout
{
    Q_OBJECT
public:
    BOBUIestDocumentLayout(BOBUIextDocument *doc) : QAbstractTextDocumentLayout(doc), f(-1), called(false) {}
    virtual void draw(QPainter *, const PaintContext &) override {}
    virtual int hitTest(const QPointF &, BobUI::HitTestAccuracy ) const override { return 0; }

    virtual void documentChanged(int from, int oldLength, int length) override
    {
        called = true;
        lastDocumentLengths.append(BOBUIextDocumentPrivate::get(document())->length());

        if (f < 0)
            return;

        if(from != f ||
                o != oldLength ||
                l != length) {
            qDebug("checkDocumentChanged: got %d %d %d, expected %d %d %d", from, oldLength, length, f, o, l);
            error = true;
        }
    }

    virtual int pageCount() const override { return 1; }
    virtual QSizeF documentSize() const override { return QSizeF(); }

    virtual QRectF frameBoundingRect(BOBUIextFrame *) const override { return QRectF(); }
    virtual QRectF blockBoundingRect(const BOBUIextBlock &) const override { return QRectF(); }

    int f;
    int o;
    int l;

    void expect(int from, int oldLength, int length) {
        f = from;
        o = oldLength;
        l = length;
        error = false;
        called = false;
    }
    bool error;
    bool called;
    QList<int> lastDocumentLengths;
};

#endif
