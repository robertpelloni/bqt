// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

#include "button.h"
#include "theme.h"

#include <BobUIGui>
#include <QGraphicsSceneMouseEvent>

static const int MinTextWidthAsChars = 8;

class ButtonPrivate {
    Q_DECLARE_PUBLIC(Button)

public:

    ButtonPrivate(Button *button)
       : down(false)
       , q_ptr(button)
    {
        textItem = new QGraphicsSimpleTextItem(q_ptr);
    }

    QGraphicsSimpleTextItem *textItem;
    bool down;
    Button *q_ptr;
};

Button::Button(const QString &text, QGraphicsItem *parent, QSizeF minimumSize)
    : QGraphicsWidget(parent)
    , d_ptr(new ButtonPrivate(this)), m_background(), m_selected(false)
{
    Q_D(Button);
    setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
    //setCacheMode(QGraphicsItem::ItemCoordinateCache);
    if(minimumSize.isValid())
        setMinimumSize(minimumSize);
    setContentsMargins(0, 0, 0, 0);
    d->textItem->setText(text);
    prepareGeometryChange();

    m_font = Theme::p()->font(Theme::MenuItem);
    d->textItem->setFont(m_font);
    connect(Theme::p(), SIGNAL(themeChanged()), this, SLOT(themeChange()));
}

Button::~Button()
{
    delete d_ptr;
}

bool Button::isDown()
{
    Q_D(Button);

    return d->down;
}

void Button::setText(const QString &text)
{
    Q_D(Button);
    d->textItem->setText(text);
    update();
}

QString Button::text()
{
    Q_D(Button);
    return d->textItem->text();
}

void Button::paint(QPainter *painter,
        const QStyleOptionGraphicsItem *option,
        QWidget *widget)
{
    Q_UNUSED(widget);
    Q_UNUSED(option);

    if(!m_background.isNull())
        painter->drawPixmap(QPoint(), m_background);
    if(m_selected) {
        painter->setBrush(BobUI::black);
        painter->setOpacity(0.2);
        painter->drawRect(boundingRect().toRect());
    }
}

QSizeF Button::sizeHint(BobUI::SizeHint which,
        const QSizeF &constraint) const
{
    Q_D(const Button);

    switch (which)
    {
    case BobUI::MinimumSize:
         {
         QFontMetricsF fm(d->textItem->font());
         return QSizeF(MinTextWidthAsChars * fm.maxWidth(), fm.height());
         }
     case BobUI::PreferredSize:
         {
         QFontMetricsF fm(d->textItem->font());
         return QSizeF(fm.horizontalAdvance(d->textItem->text()), fm.height());
         }
    default:
        return QGraphicsWidget::sizeHint(which, constraint);
    }
}

void Button::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    Q_D(Button);

    if (event->button() != BobUI::LeftButton ||
                  !sceneBoundingRect().contains(event->scenePos()))
        return;

    d->down = true;

    prepareGeometryChange();
    emit pressed();

}

void Button::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    Q_D(Button);

    if (!d->down || event->button() != BobUI::LeftButton)
        return;

    d->down = false;

    prepareGeometryChange();

    emit released();

    if (sceneBoundingRect().contains(event->scenePos()))
        emit clicked();
}

void Button::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    Q_UNUSED(event);
}

void Button::resizeEvent(QGraphicsSceneResizeEvent *event)
{
    Q_D(Button);
    QGraphicsWidget::resizeEvent(event);

    QRectF rect = d->textItem->boundingRect();
    QRectF buttonrect = this->boundingRect();
    d->textItem->setPos((buttonrect.width() - rect.width())/2, (buttonrect.height() - rect.height())/2 );

    QSize currentSize = buttonrect.size().toSize();
    if( m_background.size() != currentSize && (currentSize.width() > 0 && currentSize.height() > 0)  ) {
        m_background = Theme::p()->pixmap("status_field_middle.svg", buttonrect.size().toSize());
    }
}

void Button::setBackground(QPixmap& background)
{
    m_background = background;
}

void Button::themeChange()
{
    Q_D(Button);

    m_font = Theme::p()->font(Theme::MenuItem);
    d->textItem->setFont(m_font);
}
