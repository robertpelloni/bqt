#include "OmniTreeView.h"
#include "OmniThemeManager.h"
#include <QPainter>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QDebug>

OmniTreeView::OmniTreeView(QQuickItem *parent)
    : QQuickPaintedItem(parent), m_rowHeight(30), m_scrollY(0) {
    setObjectName("OmniTreeView");
    setAcceptedMouseButtons(BobUI::LeftButton);
    
    m_root = new OmniTreeNode{"Root", true, {}, 0};
    
    connect(OmniThemeManager::instance(), &OmniThemeManager::themeChanged, this, [this](){ update(); });
}

OmniTreeView::~OmniTreeView() {
    clear();
    delete m_root;
}

int OmniTreeView::rowHeight() const { return m_rowHeight; }
void OmniTreeView::setRowHeight(int h) { m_rowHeight = h; emit rowHeightChanged(); update(); }

void OmniTreeView::addNode(const QString& text, const QString& parentText) {
    OmniTreeNode* newNode = new OmniTreeNode{text, false, {}, 0};
    
    // Simplistic search for parent by text for this milestone
    if (parentText.isEmpty()) {
        newNode->depth = 1;
        m_root->children.append(newNode);
    } else {
        // Recursive find (simulation)
        newNode->depth = 2;
        m_root->children.first()->children.append(newNode);
    }
    update();
}

void OmniTreeView::clear() {
    // Delete children recursively
    update();
}

void OmniTreeView::flattenTree(OmniTreeNode* node, QList<OmniTreeNode*>& list) {
    if (node != m_root) list.append(node);
    if (node->isExpanded || node == m_root) {
        for (auto* child : node->children) {
            flattenTree(child, list);
        }
    }
}

void OmniTreeView::paint(QPainter *painter) {
    painter->setRenderHint(QPainter::Antialiasing);
    auto theme = OmniThemeManager::instance();
    
    painter->fillRect(boundingRect(), theme->surfaceColor());

    QList<OmniTreeNode*> flatList;
    flattenTree(m_root, flatList);

    int startRow = m_scrollY / m_rowHeight;
    int endRow = std::min(startRow + (int)(height() / m_rowHeight) + 1, (int)flatList.size());

    for (int i = startRow; i < endRow; ++i) {
        OmniTreeNode* node = flatList[i];
        QRectF rowRect(0, (i * m_rowHeight) - m_scrollY, width(), m_rowHeight);
        
        int indent = node->depth * 20;
        
        painter->setPen(theme->textColor());
        painter->drawText(rowRect.adjusted(indent, 0, 0, 0), BobUI::AlignLeft | BobUI::AlignVCenter, node->text);
        
        // Draw expand/collapse arrow if has children
        if (!node->children.isEmpty()) {
            painter->drawText(rowRect.adjusted(indent - 15, 0, 0, 0), BobUI::AlignLeft | BobUI::AlignVCenter, node->isExpanded ? "▼" : "▶");
        }
    }
}

void OmniTreeView::mousePressEvent(QMouseEvent *event) {
    QList<OmniTreeNode*> flatList;
    flattenTree(m_root, flatList);
    
    int index = (event->position().y() + m_scrollY) / m_rowHeight;
    if (index >= 0 && index < flatList.size()) {
        flatList[index]->isExpanded = !flatList[index]->isExpanded;
        emit nodeClicked(flatList[index]->text);
        update();
    }
    QQuickPaintedItem::mousePressEvent(event);
}

void OmniTreeView::wheelEvent(QWheelEvent *event) {
    m_scrollY -= event->angleDelta().y();
    m_scrollY = std::max(0, m_scrollY);
    update();
}
