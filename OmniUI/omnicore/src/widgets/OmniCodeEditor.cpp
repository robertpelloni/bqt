#include "OmniCodeEditor.h"
#include <QPainter>
#include <QPainterPath>
#include <QKeyEvent>
#include <QWheelEvent>
#include <algorithm>

OmniCodeEditor::OmniCodeEditor(QQuickItem *parent)
    : QQuickPaintedItem(parent),
      m_language("cpp"),
      m_accentColor(QColor("#0078D7")),
      m_cursorLine(0),
      m_cursorCol(0),
      m_isFocused(false),
      m_cursorVisible(true),
      m_scrollY(0)
{
    setObjectName("OmniCodeEditor");
    setAcceptedMouseButtons(BobUI::LeftButton);
    setFlag(ItemAcceptsInputMethod, true);
    setFlag(ItemHasContents, true);
    setFocusOnTouch(true);

    m_lines.append("");

    connect(&m_blinkTimer, &BOBUIimer::timeout, this, &OmniCodeEditor::blinkCursor);
    m_blinkTimer.setInterval(500);

    setWidth(600);
    setHeight(400);
}

OmniCodeEditor::~OmniCodeEditor() = default;

QString OmniCodeEditor::text() const { return m_lines.join("\n"); }
void OmniCodeEditor::setText(const QString& text) {
    QStringList newLines = text.split("\n");
    if (m_lines != newLines) {
        m_lines = newLines;
        if (m_lines.isEmpty()) m_lines.append("");
        m_cursorLine = std::clamp(m_cursorLine, 0, (int)m_lines.size() - 1);
        m_cursorCol = std::clamp(m_cursorCol, 0, (int)m_lines[m_cursorLine].length());
        emit textChanged();
        update();
    }
}

QString OmniCodeEditor::language() const { return m_language; }
void OmniCodeEditor::setLanguage(const QString& lang) {
    if (m_language != lang) {
        m_language = lang;
        emit languageChanged();
        update();
    }
}

QColor OmniCodeEditor::accentColor() const { return m_accentColor; }
void OmniCodeEditor::setAccentColor(const QColor& color) {
    if (m_accentColor != color) {
        m_accentColor = color;
        emit accentColorChanged();
        update();
    }
}

void OmniCodeEditor::blinkCursor() {
    if (m_isFocused) {
        m_cursorVisible = !m_cursorVisible;
        update();
    }
}

void OmniCodeEditor::focusInEvent(QFocusEvent *event) {
    m_isFocused = true;
    m_cursorVisible = true;
    m_blinkTimer.start();
    update();
    QQuickPaintedItem::focusInEvent(event);
}

void OmniCodeEditor::focusOutEvent(QFocusEvent *event) {
    m_isFocused = false;
    m_cursorVisible = false;
    m_blinkTimer.stop();
    update();
    QQuickPaintedItem::focusOutEvent(event);
}

void OmniCodeEditor::mousePressEvent(QMouseEvent *event) {
    forceActiveFocus();
    QQuickPaintedItem::mousePressEvent(event);
}

void OmniCodeEditor::wheelEvent(QWheelEvent *event) {
    m_scrollY -= event->angleDelta().y();
    m_scrollY = std::max(0, m_scrollY);
    update();
    QQuickPaintedItem::wheelEvent(event);
}

void OmniCodeEditor::keyPressEvent(QKeyEvent *event) {
    m_cursorVisible = true; // Force visible while typing

    if (event->key() == BobUI::Key_Backspace) {
        if (m_cursorCol > 0) {
            m_lines[m_cursorLine].remove(m_cursorCol - 1, 1);
            m_cursorCol--;
            update();
        } else if (m_cursorLine > 0) {
            int prevLen = m_lines[m_cursorLine - 1].length();
            m_lines[m_cursorLine - 1].append(m_lines[m_cursorLine]);
            m_lines.removeAt(m_cursorLine);
            m_cursorLine--;
            m_cursorCol = prevLen;
            update();
        }
    } else if (event->key() == BobUI::Key_Return || event->key() == BobUI::Key_Enter) {
        QString remainder = m_lines[m_cursorLine].mid(m_cursorCol);
        m_lines[m_cursorLine].truncate(m_cursorCol);
        m_lines.insert(m_cursorLine + 1, remainder);
        m_cursorLine++;
        m_cursorCol = 0;
        update();
    } else if (event->key() == BobUI::Key_Left) {
        if (m_cursorCol > 0) m_cursorCol--;
        else if (m_cursorLine > 0) {
            m_cursorLine--;
            m_cursorCol = m_lines[m_cursorLine].length();
        }
        update();
    } else if (event->key() == BobUI::Key_Right) {
        if (m_cursorCol < m_lines[m_cursorLine].length()) m_cursorCol++;
        else if (m_cursorLine < m_lines.size() - 1) {
            m_cursorLine++;
            m_cursorCol = 0;
        }
        update();
    } else if (event->key() == BobUI::Key_Up) {
        if (m_cursorLine > 0) {
            m_cursorLine--;
            m_cursorCol = std::min(m_cursorCol, (int)m_lines[m_cursorLine].length());
            update();
        }
    } else if (event->key() == BobUI::Key_Down) {
        if (m_cursorLine < m_lines.size() - 1) {
            m_cursorLine++;
            m_cursorCol = std::min(m_cursorCol, (int)m_lines[m_cursorLine].length());
            update();
        }
    } else if (!event->text().isEmpty()) {
        m_lines[m_cursorLine].insert(m_cursorCol, event->text());
        m_cursorCol += event->text().length();
        update();
    } else {
        QQuickPaintedItem::keyPressEvent(event);
    }
}

void OmniCodeEditor::highlightSyntax(QPainter* painter, const QString& line, const QRectF& rect) {
    // Basic mock syntax highlighting for "cpp"
    QStringList keywords = {"class", "public", "private", "void", "int", "bool", "return", "if", "else", "#include"};
    
    // In a real implementation, we would tokenize the string.
    // Here we just draw the string directly, but color it if it starts with a keyword (simulation).
    painter->setPen(BobUI::white);
    
    for (const QString& kw : keywords) {
        if (line.trimmed().startsWith(kw)) {
            painter->setPen(QColor("#569CD6")); // VSCode Blue
            break;
        }
    }
    
    painter->drawText(rect, BobUI::AlignLeft | BobUI::AlignVCenter, line);
}

void OmniCodeEditor::paint(QPainter *painter) {
    QRectF rect = boundingRect();
    
    // Background
    painter->fillRect(rect, QColor("#1E1E1E"));

    QFont font("Consolas", 11);
    painter->setFont(font);
    QFontMetrics metrics(font);
    int lineHeight = metrics.lineSpacing();

    // Calculate visible lines based on scroll
    int startLine = m_scrollY / lineHeight;
    int endLine = std::min(startLine + static_cast<int>(rect.height() / lineHeight) + 1, (int)m_lines.size());

    // Draw Line Numbers & Text
    int leftMargin = 40;
    
    // Line Number Margin Background
    painter->fillRect(QRectF(0, 0, leftMargin, rect.height()), QColor("#252526"));
    
    for (int i = startLine; i < endLine; ++i) {
        qreal y = (i - startLine) * lineHeight;
        QRectF lineRect(leftMargin + 10, y, rect.width() - leftMargin - 10, lineHeight);
        
        // Line number
        painter->setPen(QColor("#858585"));
        painter->drawText(QRectF(0, y, leftMargin - 5, lineHeight), BobUI::AlignRight | BobUI::AlignVCenter, QString::number(i + 1));
        
        // Syntax Highlighted Text
        highlightSyntax(painter, m_lines[i], lineRect);
        
        // Draw Cursor
        if (m_isFocused && m_cursorVisible && i == m_cursorLine) {
            QString textBeforeCursor = m_lines[i].left(m_cursorCol);
            int cursorX = leftMargin + 10 + metrics.horizontalAdvance(textBeforeCursor);
            painter->fillRect(QRectF(cursorX, y + 2, 2, lineHeight - 4), BobUI::white);
        }
    }
}
