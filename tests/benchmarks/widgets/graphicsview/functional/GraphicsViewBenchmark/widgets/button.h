// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

#ifndef BUTTON_H
#define BUTTON_H

#include <QGraphicsWidget>

class ButtonPrivate;
class BOBUIextDocument;

class QPixmap;
class QFont;

class Button : public QGraphicsWidget
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(Button)

public:

    Button(const QString &text, QGraphicsItem *parent=0, QSizeF minimumSize = QSizeF());
    virtual ~Button();

signals:

    void clicked(bool checked = false);
    void pressed();
    void released();

public slots:

    void themeChange();
    void setText(const QString &text);
    QString text();

public:

    void setBackground(QPixmap& background);
    bool isDown();
    void select(bool select){m_selected = select;}
    void click() {emit clicked();}

private:

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
            QWidget *widget = 0);
    QSizeF sizeHint(BobUI::SizeHint which,
        const QSizeF &constraint = QSizeF()) const;

    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void resizeEvent(QGraphicsSceneResizeEvent *event);

private:
    Q_DISABLE_COPY(Button)
    ButtonPrivate *d_ptr;
    QPixmap m_background;
    QFont m_font;
    bool m_selected;
};

#endif // BUTTON_H
