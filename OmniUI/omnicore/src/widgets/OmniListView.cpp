#include "OmniListView.h"
#include "OmniThemeManager.h"
#include <QPainter>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QKeyEvent>
#include <algorithm>

OmniListView::OmniListView(QQuickItem *parent)
    : QQuickPaintedItem(parent),
      m_currentIndex(-1),
      m_rowHeight(40),
      m_scrollY(0)
{
    setObjectName("OmniListView");
    setAcceptedMouseButtons(Qt::LeftButton);
    setFlag(ItemAcceptsInputMethod, true);
    setFlag(ItemHasContents, true);
    setFocusOnTouch(true);

    setWidth(300);
    setHeight(400);

    connect(OmniThemeManager::instance(), &OmniThemeManager::themeChanged, this, [this]() {
        update(); 
    });
}

OmniListView::~OmniListView() = default;

QVariantList OmniListView::model() const { return m_model; }
void OmniListView::setModel(const QVariantList& model) {
    if (m_model != model) {
        m_model = model;
        m_currentIndex = -1;
        m_scrollY = 0;
        emit modelChanged();
        update();
    }
}

int OmniListView::currentIndex() const { return m_currentIndex; }
void OmniListView::setCurrentIndex(int index) {
    if (m_currentIndex != index && index >= -1 && index < m_model.size()) {
        m_currentIndex = index;
        emit currentIndexChanged(m_currentIndex);
        update();
    }
}

int OmniListView::rowHeight() const { return m_rowHeight; }
void OmniListView::setRowHeight(int h) {
    if (m_rowHeight != h && h > 0) {
        m_rowHeight = h;
        emit rowHeightChanged();
        update();
    }
}

void OmniListView::mousePressEvent(QMouseEvent *event) {
    forceActiveFocus();
    
    int clickY = event->position().y() + m_scrollY;
    int index = clickY / m_rowHeight;

    if (index >= 0 && index < m_model.size()) {
        setCurrentIndex(index);
        emit itemClicked(index, m_model[index]);
    }
    
    QQuickPaintedItem::mousePressEvent(event);
}

void OmniListView::wheelEvent(QWheelEvent *event) {
    int maxScroll = std::max(0, (m_model.size() * m_rowHeight) - static_cast<int>(height()));
    m_scrollY -= (event->angleDelta().y() / 4);
    m_scrollY = std::clamp(m_scrollY, 0, maxScroll);
    update();
    QQuickPaintedItem::wheelEvent(event);
}

void OmniListView::keyPressEvent(QKeyEvent *event) {
    if (m_model.isEmpty()) return;

    if (event->key() == Qt::Key_Up) {
        setCurrentIndex(std::max(0, m_currentIndex - 1));
        
        // Auto-scroll
        if (m_currentIndex * m_rowHeight < m_scrollY) {
            m_scrollY = m_currentIndex * m_rowHeight;
        }
    } else if (event->key() == Qt::Key_Down) {
        setCurrentIndex(std::min((int)m_model.size() - 1, m_currentIndex + 1));
        
        // Auto-scroll
        if ((m_currentIndex + 1) * m_rowHeight > m_scrollY + height()) {
            m_scrollY = ((m_currentIndex + 1) * m_rowHeight) - height();
        }
    } else if (event->key() == Qt::Key_Return || event->key() == Qt::Key_Enter) {
        if (m_currentIndex >= 0) emit itemClicked(m_currentIndex, m_model[m_currentIndex]);
    }

    update();
    QQuickPaintedItem::keyPressEvent(event);
}

void OmniListView::paint(QPainter *painter) {
    painter->setRenderHint(QPainter::Antialiasing);
    QRectF rect = boundingRect();

    auto themeMgr = OmniThemeManager::instance();
    
    // Background
    painter->fillRect(rect, themeMgr->surfaceColor().darker(110));

    if (m_model.isEmpty()) {
        painter->setPen(themeMgr->textColor().darker(150));
        painter->drawText(rect, Qt::AlignCenter, "No Items");
        return;
    }

    int startRow = m_scrollY / m_rowHeight;
    int endRow = std::min(startRow + static_cast<int>(height() / m_rowHeight) + 2, (int)m_model.size());

    QFont font = painter->font();
    font.setPixelSize(14);
    painter->setFont(font);

    for (int i = startRow; i < endRow; ++i) {
        QRectF rowRect(0, (i * m_rowHeight) - m_scrollY, rect.width(), m_rowHeight);
        
        // Draw Selection / Hover
        if (i == m_currentIndex) {
            painter->fillRect(rowRect, themeMgr->primaryColor());
            painter->setPen(Qt::white);
        } else {
            painter->setPen(themeMgr->textColor());
            // Alternating row colors
            if (i % 2 == 0) painter->fillRect(rowRect, themeMgr->surfaceColor().lighter(105));
        }

        // Draw Data (Handles simple string mapping natively)
        QString textToDraw;
        QVariant data = m_model[i];
        if (data.typeId() == QMetaType::QString) textToDraw = data.toString();
        else if (data.typeId() == QMetaType::QVariantMap) textToDraw = data.toMap().value("text").toString();
        else textToDraw = "Item " + QString::number(i);

        painter->drawText(rowRect.adjusted(15, 0, -15, 0), Qt::AlignLeft | Qt::AlignVCenter, textToDraw);
    }
}
