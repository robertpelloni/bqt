#include "OmniCheckBox.h"
#include "OmniThemeManager.h"
#include "OmniUserManager.h"
#include "OmniValueTree.h"
#include <QPainter>
#include <QMouseEvent>

OmniCheckBox::OmniCheckBox(QQuickItem *parent)
    : QQuickPaintedItem(parent), m_checked(false), m_text("CheckBox"),
      m_accentColor(QColor("#0078D7")), m_isHovered(false), m_isPressed(false)
{
    setAcceptedMouseButtons(BobUI::LeftButton);
    setAcceptHoverEvents(true);
    setWidth(120); setHeight(24);
    connect(OmniThemeManager::instance(), &OmniThemeManager::themeChanged, this, [this]() { update(); });
}

bool OmniCheckBox::checked() const { return m_checked; }
void OmniCheckBox::setChecked(bool c) {
    if (m_checked != c) {
        m_checked = c;
        emit checkedChanged();
        emit toggled(m_checked);
        if (!m_valueTreeKey.isEmpty()) OmniValueTree::instance()->setParameter(m_valueTreeKey, m_checked);
        update();
    }
}

QString OmniCheckBox::valueTreeKey() const { return m_valueTreeKey; }
void OmniCheckBox::setValueTreeKey(const QString& key) {
    if (m_valueTreeKey != key) {
        m_valueTreeKey = key;
        OmniValueTree::instance()->bindProperty(this, "checked", key);
        emit valueTreeKeyChanged();
    }
}

void OmniCheckBox::mousePressEvent(QMouseEvent *event) {
    QString deviceId = property("_omni_active_device").toString();
    if (OmniUserManager::instance()->tryLockInteraction(deviceId, this)) {
        m_isPressed = true;
        update();
    }
    QQuickPaintedItem::mousePressEvent(event);
}

void OmniCheckBox::mouseReleaseEvent(QMouseEvent *event) {
    QString deviceId = property("_omni_active_device").toString();
    if (m_isPressed) {
        m_isPressed = false;
        setChecked(!m_checked);
        OmniUserManager::instance()->unlockInteraction(deviceId, this);
    }
    QQuickPaintedItem::mouseReleaseEvent(event);
}

void OmniCheckBox::paint(QPainter *painter) {
    painter->setRenderHint(QPainter::Antialiasing);
    QRectF rect = boundingRect();
    auto theme = OmniThemeManager::instance();
    
    qreal boxSize = 18.0;
    QRectF boxRect(0, (rect.height() - boxSize) / 2.0, boxSize, boxSize);

    painter->setBrush(m_checked ? m_accentColor : theme->surface().darker(150));
    painter->setPen(QPen(theme->primary(), 1));
    painter->drawRoundedRect(boxRect, 3, 3);

    if (m_checked) {
        painter->setPen(QPen(BobUI::white, 2));
        painter->drawLine(boxRect.left()+4, boxRect.center().y(), boxRect.left()+8, boxRect.bottom()-4);
        painter->drawLine(boxRect.left()+8, boxRect.bottom()-4, boxRect.right()-4, boxRect.top()+4);
    }

    painter->setPen(theme->text());
    painter->drawText(QRectF(boxRect.right()+8, 0, rect.width(), rect.height()), BobUI::AlignVCenter, m_text);
}

// Logic implementations omitted but preserved in kernel...
QString OmniCheckBox::text() const { return m_text; }
void OmniCheckBox::setText(const QString& t) { m_text = t; update(); }
QColor OmniCheckBox::accentColor() const { return m_accentColor; }
void OmniCheckBox::setAccentColor(const QColor& color) { m_accentColor = color; update(); }
void OmniCheckBox::hoverEnterEvent(QHoverEvent *e) { m_isHovered = true; update(); }
void OmniCheckBox::hoverLeaveEvent(QHoverEvent *e) { m_isHovered = false; update(); }
OmniCheckBox::~OmniCheckBox() = default;
