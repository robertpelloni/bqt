#include "OmniCheckBox.h"
#include <QPainter>
#include <QPainterPath>
#include <QMouseEvent>

OmniCheckBox::OmniCheckBox(QQuickItem *parent)
    : QQuickPaintedItem(parent),
      m_checked(false),
      m_text("CheckBox"),
      m_accentColor(QColor("#0078D7")),
      m_isHovered(false),
      m_isPressed(false)
{
    setObjectName("OmniCheckBox");
    setAcceptedMouseButtons(Qt::LeftButton);
    setAcceptHoverEvents(true);
    
    setWidth(120);
    setHeight(24);
}

OmniCheckBox::~OmniCheckBox() = default;

bool OmniCheckBox::checked() const { return m_checked; }
void OmniCheckBox::setChecked(bool c) {
    if (m_checked != c) {
        m_checked = c;
        emit checkedChanged();
        emit toggled(m_checked);
        update();
    }
}

QString OmniCheckBox::text() const { return m_text; }
void OmniCheckBox::setText(const QString& t) {
    if (m_text != t) {
        m_text = t;
        emit textChanged();
        update();
    }
}

QColor OmniCheckBox::accentColor() const { return m_accentColor; }
void OmniCheckBox::setAccentColor(const QColor& color) {
    if (m_accentColor != color) {
        m_accentColor = color;
        emit accentColorChanged();
        update();
    }
}

void OmniCheckBox::hoverEnterEvent(QHoverEvent *event) {
    m_isHovered = true;
    update();
    QQuickPaintedItem::hoverEnterEvent(event);
}

void OmniCheckBox::hoverLeaveEvent(QHoverEvent *event) {
    m_isHovered = false;
    update();
    QQuickPaintedItem::hoverLeaveEvent(event);
}

void OmniCheckBox::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        m_isPressed = true;
        update();
    }
    QQuickPaintedItem::mousePressEvent(event);
}

void OmniCheckBox::mouseReleaseEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton && m_isPressed) {
        m_isPressed = false;
        setChecked(!m_checked); // Toggle state
    }
    QQuickPaintedItem::mouseReleaseEvent(event);
}

void OmniCheckBox::paint(QPainter *painter) {
    painter->setRenderHint(QPainter::Antialiasing);

    QRectF rect = boundingRect();
    qreal boxSize = 18.0;
    QRectF boxRect(0, (rect.height() - boxSize) / 2.0, boxSize, boxSize);

    // Dynamic theming based on interaction state
    QColor bgColor = m_checked ? m_accentColor : QColor("#222222");
    if (m_isHovered) {
        bgColor = bgColor.lighter(120);
    }

    // Draw the box
    QPainterPath path;
    path.addRoundedRect(boxRect, 3, 3);
    painter->fillPath(path, bgColor);
    
    // Draw border if unchecked
    if (!m_checked) {
        painter->setPen(QPen(QColor("#555555"), 1));
        painter->drawPath(path);
    }

    // Draw the checkmark
    if (m_checked) {
        painter->setPen(QPen(Qt::white, 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
        QPolygonF checkMark;
        checkMark << QPointF(boxRect.left() + 4, boxRect.center().y())
                  << QPointF(boxRect.left() + 8, boxRect.bottom() - 4)
                  << QPointF(boxRect.right() - 3, boxRect.top() + 4);
        painter->drawPolyline(checkMark);
    }

    // Draw the text
    painter->setPen(Qt::white);
    painter->setFont(font());
    QRectF textRect(boxRect.right() + 8, 0, rect.width() - boxRect.right() - 8, rect.height());
    painter->drawText(textRect, Qt::AlignLeft | Qt::AlignVCenter, m_text);
}
