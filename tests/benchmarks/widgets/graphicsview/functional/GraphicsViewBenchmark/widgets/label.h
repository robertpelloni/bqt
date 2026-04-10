// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

#ifndef TEXTITEM_H
#define TEXTITEM_H

#include "gvbwidget.h"

class QPainter;
class QStyleOptionGraphicsItem;
class QGraphicsTextItem;

class Label : public GvbWidget
{
    Q_OBJECT

public:

    Label(const QString& text, QGraphicsItem *parent = nullptr);
    ~Label();

public:

    void setText(const QString& text);
    QString text() const;
    void setFont(const QFont font);

private:
    void resizeEvent(QGraphicsSceneResizeEvent *event);
    QSizeF sizeHint(BobUI::SizeHint which,
    const QSizeF &constraint = QSizeF()) const;

private:
    Q_DISABLE_COPY(Label)
    QGraphicsSimpleTextItem *m_textItem;
};

#endif
