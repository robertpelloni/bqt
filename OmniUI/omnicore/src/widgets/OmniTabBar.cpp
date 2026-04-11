#include "OmniTabBar.h"
#include "OmniThemeManager.h"
#include <QPainter>
#include <QPainterPath>
#include <QMouseEvent>
#include <QHoverEvent>

OmniTabBar::OmniTabBar(QQuickItem *parent)
    : QQuickPaintedItem(parent),
      m_currentIndex(-1),
      m_accentColor(QColor("#0078D7")),
      m_hoveredIndex(-1)
{
    setObjectName("OmniTabBar");
    setAcceptedMouseButtons(Qt::LeftButton);
    setAcceptHoverEvents(true);
    setFlag(ItemHasContents, true);
    
    setWidth(400);
    setHeight(30);

    connect(OmniThemeManager::instance(), &OmniThemeManager::themeChanged, this, [this]() {
        update(); 
    });
}

OmniTabBar::~OmniTabBar() = default;

QStringList OmniTabBar::tabs() const { return m_tabs; }
void OmniTabBar::setTabs(const QStringList& tabs) {
    if (m_tabs != tabs) {
        m_tabs = tabs;
        recalculateRects();
        emit tabsChanged();
        if (m_currentIndex >= m_tabs.size()) setCurrentIndex(m_tabs.size() - 1);
        update();
    }
}

int OmniTabBar::currentIndex() const { return m_currentIndex; }
void OmniTabBar::setCurrentIndex(int index) {
    if (m_currentIndex != index && index >= -1 && index < m_tabs.size()) {
        m_currentIndex = index;
        emit currentIndexChanged(m_currentIndex);
        update();
    }
}

QColor OmniTabBar::accentColor() const { return m_accentColor; }
void OmniTabBar::setAccentColor(const QColor& color) {
    if (m_accentColor != color) {
        m_accentColor = color;
        emit accentColorChanged();
        update();
    }
}

void OmniTabBar::addTab(const QString& title) {
    m_tabs.append(title);
    recalculateRects();
    emit tabsChanged();
    if (m_currentIndex == -1) setCurrentIndex(0);
    update();
}

void OmniTabBar::removeTab(int index) {
    if (index >= 0 && index < m_tabs.size()) {
        m_tabs.removeAt(index);
        recalculateRects();
        emit tabsChanged();
        if (m_currentIndex == index) setCurrentIndex(std::max(0, m_currentIndex - 1));
        update();
    }
}

void OmniTabBar::recalculateRects() {
    m_tabRects.clear();
    qreal x = 0;
    qreal widthPerTab = 120; // Fixed width for simple docking simulation

    for (int i = 0; i < m_tabs.size(); ++i) {
        m_tabRects.append(QRectF(x, 0, widthPerTab, height()));
        x += widthPerTab + 2; // Spacing
    }
}

int OmniTabBar::tabAt(const QPointF& pos) const {
    for (int i = 0; i < m_tabRects.size(); ++i) {
        if (m_tabRects[i].contains(pos)) return i;
    }
    return -1;
}

void OmniTabBar::mousePressEvent(QMouseEvent *event) {
    int index = tabAt(event->position());
    if (index != -1) {
        setCurrentIndex(index);
        emit tabClicked(index);
    }
    QQuickPaintedItem::mousePressEvent(event);
}

void OmniTabBar::mouseMoveEvent(QMouseEvent *event) {
    QQuickPaintedItem::mouseMoveEvent(event);
}

void OmniTabBar::hoverMoveEvent(QHoverEvent *event) {
    int index = tabAt(event->position());
    if (m_hoveredIndex != index) {
        m_hoveredIndex = index;
        update();
    }
    QQuickPaintedItem::hoverMoveEvent(event);
}

void OmniTabBar::hoverLeaveEvent(QHoverEvent *event) {
    m_hoveredIndex = -1;
    update();
    QQuickPaintedItem::hoverLeaveEvent(event);
}

void OmniTabBar::paint(QPainter *painter) {
    painter->setRenderHint(QPainter::Antialiasing);
    
    auto themeMgr = OmniThemeManager::instance();
    auto currentTheme = themeMgr->currentTheme();
    
    // Draw background track
    painter->fillRect(boundingRect(), themeMgr->surfaceColor().darker(150));

    for (int i = 0; i < m_tabs.size(); ++i) {
        QRectF rect = m_tabRects[i];
        
        bool isSelected = (i == m_currentIndex);
        bool isHovered = (i == m_hoveredIndex);

        QColor baseColor = themeMgr->surfaceColor();
        if (isSelected) baseColor = m_accentColor;
        else if (isHovered) baseColor = baseColor.lighter(120);

        QPainterPath path;
        
        if (currentTheme == OmniThemeManager::Cyberpunk) {
            path.moveTo(rect.x() + 5, rect.y()); 
            path.lineTo(rect.right() - 5, rect.y());
            path.lineTo(rect.right(), rect.bottom()); 
            path.lineTo(rect.x(), rect.bottom()); 
            path.closeSubpath();
            
            painter->setPen(QPen(baseColor.darker(200), 2));
            painter->fillPath(path, isSelected ? QColor(0,0,0, 200) : QColor(20,20,20, 200));
            
            if (isSelected) {
                painter->setPen(QPen(baseColor, 2));
                painter->drawLine(rect.x() + 5, rect.y(), rect.right() - 5, rect.y());
            }
        } else {
            path.addRoundedRect(rect, 4, 4); // Standard tabs
            painter->setPen(Qt::NoPen);
            painter->fillPath(path, baseColor);
        }

        // Draw text
        painter->setPen(isSelected ? Qt::white : themeMgr->textColor());
        QFont f = painter->font();
        f.setPixelSize(12);
        if (currentTheme == OmniThemeManager::Cyberpunk) f.setItalic(true);
        painter->setFont(f);
        
        painter->drawText(rect, Qt::AlignCenter, m_tabs[i]);
    }
}
