// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef BOBUIEXTDOCUMENTLAYOUT_P_H
#define BOBUIEXTDOCUMENTLAYOUT_P_H

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
#include "BobUIGui/qabstracttextdocumentlayout.h"
#include "BobUIGui/bobuiextoption.h"
#include "BobUIGui/bobuiextobject.h"
#include "BobUICore/qloggingcategory.h"

BOBUI_BEGIN_NAMESPACE

Q_DECLARE_LOGGING_CATEGORY(lcLayout)

class BOBUIextListFormat;
class BOBUIextTableCell;
class BOBUIextDocumentLayoutPrivate;

class Q_GUI_EXPORT BOBUIextDocumentLayout : public QAbstractTextDocumentLayout
{
    Q_DECLARE_PRIVATE(BOBUIextDocumentLayout)
    Q_OBJECT
    Q_PROPERTY(int cursorWidth READ cursorWidth WRITE setCursorWidth)
    Q_PROPERTY(qreal idealWidth READ idealWidth)
    Q_PROPERTY(bool contentHasAlignment READ contentHasAlignment)
public:
    explicit BOBUIextDocumentLayout(BOBUIextDocument *doc);

    // from the abstract layout
    void draw(QPainter *painter, const PaintContext &context) override;
    int hitTest(const QPointF &point, BobUI::HitTestAccuracy accuracy) const override;

    int pageCount() const override;
    QSizeF documentSize() const override;

    void setCursorWidth(int width);
    int cursorWidth() const;

    // internal, to support the ugly FixedColumnWidth wordwrap mode in BOBUIextEdit
    void setFixedColumnWidth(int width);

    // internal for BOBUIextEdit's NoWrap mode
    void setViewport(const QRectF &viewport);

    virtual QRectF frameBoundingRect(BOBUIextFrame *frame) const override;
    virtual QRectF blockBoundingRect(const BOBUIextBlock &block) const override;
    QRectF tableBoundingRect(BOBUIextTable *table) const;
    QRectF tableCellBoundingRect(BOBUIextTable *table, const BOBUIextTableCell &cell) const;

    // ####
    int layoutStatus() const;
    int dynamicPageCount() const;
    QSizeF dynamicDocumentSize() const;
    void ensureLayouted(qreal);

    qreal idealWidth() const;

    bool contentHasAlignment() const;

protected:
    void documentChanged(int from, int oldLength, int length) override;
    void resizeInlineObject(BOBUIextInlineObject item, int posInDocument, const BOBUIextFormat &format) override;
    void positionInlineObject(BOBUIextInlineObject item, int posInDocument, const BOBUIextFormat &format) override;
    void drawInlineObject(QPainter *p, const QRectF &rect, BOBUIextInlineObject item,
                          int posInDocument, const BOBUIextFormat &format) override;
    virtual void timerEvent(BOBUIimerEvent *e) override;
private:
    QRectF doLayout(int from, int oldLength, int length);
    void layoutFinished();
};

BOBUI_END_NAMESPACE

#endif // BOBUIEXTDOCUMENTLAYOUT_P_H
