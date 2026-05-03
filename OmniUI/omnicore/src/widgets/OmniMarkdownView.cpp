#include "OmniMarkdownView.h"
#include "OmniThemeManager.h"
#include <QPainter>
#include <QPainterPath>
#include <QWheelEvent>
#include <QDebug>
#include <algorithm>

OmniMarkdownView::OmniMarkdownView(QQuickItem *parent)
    : QQuickPaintedItem(parent),
      m_scrollY(0),
      m_maxScroll(0),
      m_needsLayout(true)
{
    setObjectName("OmniMarkdownView");
    setAcceptedMouseButtons(BobUI::LeftButton);
    setFlag(ItemHasContents, true);
    setClip(true);
    setWidth(600);
    setHeight(400);

    connect(OmniThemeManager::instance(), &OmniThemeManager::themeChanged, this, [this]() {
        update(); 
    });
}

OmniMarkdownView::~OmniMarkdownView() = default;

QString OmniMarkdownView::text() const { return m_text; }

void OmniMarkdownView::setText(const QString& text) {
    if (m_text != text) {
        m_text = text;
        m_scrollY = 0;
        parseMarkdown();
        emit textChanged();
        update();
    }
}

void OmniMarkdownView::parseMarkdown() {
    m_blocks.clear();
    QStringList lines = m_text.split('\n');
    
    bool inCodeBlock = false;
    QString currentCodeBlock = "";

    for (const QString& line : lines) {
        if (line.trimmed().startsWith("```")) {
            if (inCodeBlock) {
                m_blocks.append({OmniMarkdownBlock::CodeBlock, currentCodeBlock, QRectF()});
                currentCodeBlock.clear();
            }
            inCodeBlock = !inCodeBlock;
            continue;
        }

        if (inCodeBlock) {
            currentCodeBlock += line + "\n";
            continue;
        }

        if (line.trimmed() == "---" || line.trimmed() == "***") {
            m_blocks.append({OmniMarkdownBlock::HorizontalRule, "", QRectF()});
        } else if (line.startsWith("# ")) {
            m_blocks.append({OmniMarkdownBlock::Header1, line.mid(2).trimmed(), QRectF()});
        } else if (line.startsWith("## ")) {
            m_blocks.append({OmniMarkdownBlock::Header2, line.mid(3).trimmed(), QRectF()});
        } else if (line.startsWith("### ")) {
            m_blocks.append({OmniMarkdownBlock::Header3, line.mid(4).trimmed(), QRectF()});
        } else if (line.trimmed().startsWith("- ") || line.trimmed().startsWith("* ")) {
            m_blocks.append({OmniMarkdownBlock::ListItem, line.trimmed().mid(2), QRectF()});
        } else if (!line.trimmed().isEmpty()) {
            m_blocks.append({OmniMarkdownBlock::Paragraph, line.trimmed(), QRectF()});
        }
    }
    
    m_needsLayout = true;
}

void OmniMarkdownView::geometryChange(const QRectF &newGeometry, const QRectF &oldGeometry) {
    QQuickPaintedItem::geometryChange(newGeometry, oldGeometry);
    if (newGeometry.width() != oldGeometry.width()) {
        m_needsLayout = true;
        update();
    }
}

void OmniMarkdownView::wheelEvent(QWheelEvent *event) {
    m_scrollY -= (event->angleDelta().y() / 4);
    m_scrollY = std::clamp(m_scrollY, 0, std::max(0, m_maxScroll));
    update();
    QQuickPaintedItem::wheelEvent(event);
}

void OmniMarkdownView::layoutDocument() {
    if (!m_needsLayout) return;
    
    qreal currentY = 0;
    qreal padding = 20;
    qreal maxWidth = width() - (padding * 2);
    
    QFont baseFont("Segoe UI", 12);
    QFont h1Font(baseFont); h1Font.setPixelSize(32); h1Font.setBold(true);
    QFont h2Font(baseFont); h2Font.setPixelSize(24); h2Font.setBold(true);
    QFont h3Font(baseFont); h3Font.setPixelSize(18); h3Font.setBold(true);
    QFont codeFont("Consolas", 11);

    for (auto& block : m_blocks) {
        QFont f = baseFont;
        if (block.type == OmniMarkdownBlock::Header1) f = h1Font;
        else if (block.type == OmniMarkdownBlock::Header2) f = h2Font;
        else if (block.type == OmniMarkdownBlock::Header3) f = h3Font;
        else if (block.type == OmniMarkdownBlock::CodeBlock) f = codeFont;

        QFontMetricsF fm(f);
        
        if (block.type == OmniMarkdownBlock::HorizontalRule) {
            block.rect = QRectF(padding, currentY + 10, maxWidth, 2);
            currentY += 30;
        } else if (block.type == OmniMarkdownBlock::CodeBlock) {
            int lines = block.text.count('\n') + 1;
            qreal h = (lines * fm.lineSpacing()) + 20;
            block.rect = QRectF(padding, currentY, maxWidth, h);
            currentY += h + 15;
        } else {
            QRectF bound = fm.boundingRect(QRectF(0, 0, maxWidth, 10000), BobUI::TextWordWrap | BobUI::AlignLeft, block.text);
            qreal h = bound.height() + (block.type == OmniMarkdownBlock::Paragraph ? 10 : 20);
            block.rect = QRectF(padding, currentY, maxWidth, h);
            currentY += h;
        }
    }
    
    m_maxScroll = static_cast<int>(currentY - height() + padding);
    m_needsLayout = false;
}

void OmniMarkdownView::paint(QPainter *painter) {
    if (m_needsLayout) layoutDocument();

    painter->setRenderHint(QPainter::Antialiasing);
    QRectF viewRect = boundingRect();
    
    auto theme = OmniThemeManager::instance();
    painter->fillRect(viewRect, theme->backgroundColor());

    painter->setClipRect(viewRect);
    painter->translate(0, -m_scrollY);

    QFont baseFont("Segoe UI", 12);
    QFont h1Font(baseFont); h1Font.setPixelSize(32); h1Font.setBold(true);
    QFont h2Font(baseFont); h2Font.setPixelSize(24); h2Font.setBold(true);
    QFont h3Font(baseFont); h3Font.setPixelSize(18); h3Font.setBold(true);
    QFont codeFont("Consolas", 11);

    for (const auto& block : m_blocks) {
        if (block.rect.bottom() < m_scrollY || block.rect.top() > m_scrollY + height()) {
            continue;
        }

        painter->setPen(theme->textColor());

        if (block.type == OmniMarkdownBlock::Header1) {
            painter->setFont(h1Font);
            painter->setPen(theme->primaryColor());
            painter->drawText(block.rect, BobUI::TextWordWrap | BobUI::AlignLeft | BobUI::AlignTop, block.text);
            painter->drawLine(block.rect.bottomLeft() + QPointF(0,-5), block.rect.bottomRight() + QPointF(0,-5));
        } else if (block.type == OmniMarkdownBlock::Header2) {
            painter->setFont(h2Font);
            painter->drawText(block.rect, BobUI::TextWordWrap | BobUI::AlignLeft | BobUI::AlignTop, block.text);
        } else if (block.type == OmniMarkdownBlock::Header3) {
            painter->setFont(h3Font);
            painter->drawText(block.rect, BobUI::TextWordWrap | BobUI::AlignLeft | BobUI::AlignTop, block.text);
        } else if (block.type == OmniMarkdownBlock::HorizontalRule) {
            painter->setPen(QPen(theme->surfaceColor().lighter(), 2));
            painter->drawLine(block.rect.topLeft(), block.rect.topRight());
        } else if (block.type == OmniMarkdownBlock::CodeBlock) {
            painter->fillRect(block.rect, theme->surfaceColor());
            painter->setPen(theme->textColor().darker(120));
            painter->drawRect(block.rect);
            
            painter->setFont(codeFont);
            painter->setPen(theme->successColor()); 
            painter->drawText(block.rect.adjusted(10, 10, -10, -10), BobUI::AlignLeft | BobUI::AlignTop, block.text);
        } else if (block.type == OmniMarkdownBlock::ListItem) {
            painter->setFont(baseFont);
            painter->setBrush(theme->primaryColor());
            painter->setPen(BobUI::NoPen);
            painter->drawEllipse(block.rect.topLeft() + QPointF(10, 10), 4, 4);
            
            painter->setPen(theme->textColor());
            painter->drawText(block.rect.adjusted(25, 0, 0, 0), BobUI::TextWordWrap | BobUI::AlignLeft | BobUI::AlignTop, block.text);
        } else {
            painter->setFont(baseFont);
            painter->drawText(block.rect, BobUI::TextWordWrap | BobUI::AlignLeft | BobUI::AlignTop, block.text);
        }
    }
}
