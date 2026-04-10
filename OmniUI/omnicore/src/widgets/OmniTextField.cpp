#include "OmniTextField.h"
#include <QPainter>
#include <QPainterPath>
#include <QKeyEvent>
#include <QGuiApplication>
#include <QClipboard>

OmniTextField::OmniTextField(QQuickItem *parent)
    : QQuickPaintedItem(parent),
      m_text(""),
      m_placeholderText("Enter text..."),
      m_accentColor(QColor("#0078D7")),
      m_cursorPosition(0),
      m_isFocused(false)
{
    setObjectName("OmniTextField");
    setAcceptedMouseButtons(BobUI::LeftButton);
    setFlag(ItemAcceptsInputMethod, true);
    setFlag(ItemHasContents, true);
    setFocusOnTouch(true); // BobUI focus integration

    setWidth(200);
    setHeight(30);
}

OmniTextField::~OmniTextField() = default;

QString OmniTextField::text() const { return m_text; }
void OmniTextField::setText(const QString& text) {
    if (m_text != text) {
        m_text = text;
        m_cursorPosition = m_text.length(); // Simplified cursor tracking
        emit textChanged();
        update();
    }
}

QString OmniTextField::placeholderText() const { return m_placeholderText; }
void OmniTextField::setPlaceholderText(const QString& text) {
    if (m_placeholderText != text) {
        m_placeholderText = text;
        emit placeholderTextChanged();
        update();
    }
}

QColor OmniTextField::accentColor() const { return m_accentColor; }
void OmniTextField::setAccentColor(const QColor& color) {
    if (m_accentColor != color) {
        m_accentColor = color;
        emit accentColorChanged();
        update();
    }
}

void OmniTextField::focusInEvent(QFocusEvent *event) {
    m_isFocused = true;
    update();
    QQuickPaintedItem::focusInEvent(event);
}

void OmniTextField::focusOutEvent(QFocusEvent *event) {
    m_isFocused = false;
    update();
    QQuickPaintedItem::focusOutEvent(event);
}

void OmniTextField::mousePressEvent(QMouseEvent *event) {
    forceActiveFocus(); // Request standard BobUI focus
    // The OmniInputManager intercepts this and assigns device-specific focus
    QQuickPaintedItem::mousePressEvent(event);
}

void OmniTextField::keyPressEvent(QKeyEvent *event) {
    // This is the core logic tested by the Multi-Cursor routing.
    // When OmniInputManager routes a key here, it proves independent typing works.

    if (event->key() == BobUI::Key_Backspace) {
        if (m_cursorPosition > 0) {
            m_text.remove(m_cursorPosition - 1, 1);
            m_cursorPosition--;
            emit textChanged();
            update();
        }
    } else if (event->key() == BobUI::Key_Delete) {
        if (m_cursorPosition < m_text.length()) {
            m_text.remove(m_cursorPosition, 1);
            emit textChanged();
            update();
        }
    } else if (event->key() == BobUI::Key_Left) {
        if (m_cursorPosition > 0) {
            m_cursorPosition--;
            update();
        }
    } else if (event->key() == BobUI::Key_Right) {
        if (m_cursorPosition < m_text.length()) {
            m_cursorPosition++;
            update();
        }
    } else if (event->key() == BobUI::Key_Return || event->key() == BobUI::Key_Enter) {
        emit returnPressed();
    } else if (!event->text().isEmpty()) {
        m_text.insert(m_cursorPosition, event->text());
        m_cursorPosition += event->text().length();
        emit textChanged();
        update();
    } else {
        QQuickPaintedItem::keyPressEvent(event);
    }
}

void OmniTextField::paint(QPainter *painter) {
    painter->setRenderHint(QPainter::Antialiasing);

    QRectF rect = boundingRect();
    QPainterPath path;
    path.addRoundedRect(rect, 4, 4);

    // Draw background
    painter->fillPath(path, QColor("#1E1E1E"));

    // Draw border
    painter->setPen(m_isFocused ? m_accentColor : QColor("#444444"));
    painter->drawPath(path);

    // Draw text
    painter->setFont(font());
    QRectF textRect = rect.adjusted(5, 0, -5, 0);

    if (m_text.isEmpty() && !m_isFocused) {
        painter->setPen(QColor("#888888"));
        painter->drawText(textRect, BobUI::AlignLeft | BobUI::AlignVCenter, m_placeholderText);
    } else {
        painter->setPen(BobUI::white);
        painter->drawText(textRect, BobUI::AlignLeft | BobUI::AlignVCenter, m_text);
        
        // Draw cursor (if focused)
        if (m_isFocused) {
            // Simplified cursor rendering
            QFontMetrics metrics(font());
            int textWidth = metrics.horizontalAdvance(m_text.left(m_cursorPosition));
            painter->setPen(m_accentColor);
            painter->drawLine(textRect.left() + textWidth + 2, textRect.top() + 5, 
                              textRect.left() + textWidth + 2, textRect.bottom() - 5);
        }
    }
}
