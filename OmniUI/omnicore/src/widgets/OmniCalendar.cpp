#include "OmniCalendar.h"
#include <QPainter>
#include <QPainterPath>
#include <QMouseEvent>
#include <QLocale>

OmniCalendar::OmniCalendar(QQuickItem *parent)
    : QQuickPaintedItem(parent),
      m_selectedDate(QDate::currentDate()),
      m_currentMonth(QDate(QDate::currentDate().year(), QDate::currentDate().month(), 1)),
      m_accentColor(QColor("#0078D7")),
      m_cellWidth(40),
      m_cellHeight(40)
{
    setObjectName("OmniCalendar");
    setAcceptedMouseButtons(BobUI::LeftButton);

    setWidth(m_cellWidth * 7);
    setHeight(m_cellHeight * 7 + 40); // 7 rows + header
}

OmniCalendar::~OmniCalendar() = default;

QDate OmniCalendar::selectedDate() const { return m_selectedDate; }
void OmniCalendar::setSelectedDate(const QDate& date) {
    if (m_selectedDate != date) {
        m_selectedDate = date;
        emit selectedDateChanged();
        update();
    }
}

QColor OmniCalendar::accentColor() const { return m_accentColor; }
void OmniCalendar::setAccentColor(const QColor& color) {
    if (m_accentColor != color) {
        m_accentColor = color;
        emit accentColorChanged();
        update();
    }
}

void OmniCalendar::mousePressEvent(QMouseEvent *event) {
    QPointF pos = event->position();

    // Check Header Buttons
    if (m_prevButtonRect.contains(pos)) {
        m_currentMonth = m_currentMonth.addMonths(-1);
        update();
        return;
    }
    if (m_nextButtonRect.contains(pos)) {
        m_currentMonth = m_currentMonth.addMonths(1);
        update();
        return;
    }

    // Check Grid
    qreal gridYOffset = 40 + m_cellHeight; // Header + Days of week row
    if (pos.y() > gridYOffset) {
        int col = pos.x() / m_cellWidth;
        int row = (pos.y() - gridYOffset) / m_cellHeight;
        
        int firstDayOfWeek = m_currentMonth.dayOfWeek(); // 1 = Monday, 7 = Sunday
        // Convert to 0 = Sunday, 6 = Saturday (depending on locale, assuming standard)
        int offset = (firstDayOfWeek == 7) ? 0 : firstDayOfWeek;
        
        int dayClicked = (row * 7) + col - offset + 1;
        
        if (dayClicked >= 1 && dayClicked <= m_currentMonth.daysInMonth()) {
            setSelectedDate(QDate(m_currentMonth.year(), m_currentMonth.month(), dayClicked));
        }
    }

    QQuickPaintedItem::mousePressEvent(event);
}

void OmniCalendar::paint(QPainter *painter) {
    painter->setRenderHint(QPainter::Antialiasing);
    QRectF rect = boundingRect();

    // Background
    painter->fillRect(rect, QColor("#1E1E1E"));

    // Header
    painter->setPen(BobUI::white);
    QFont font = painter->font();
    font.setBold(true);
    font.setPointSize(12);
    painter->setFont(font);

    QString monthStr = QLocale().monthName(m_currentMonth.month()) + " " + QString::number(m_currentMonth.year());
    painter->drawText(QRectF(0, 0, rect.width(), 40), BobUI::AlignCenter, monthStr);

    // Prev/Next Buttons
    m_prevButtonRect = QRectF(10, 10, 20, 20);
    m_nextButtonRect = QRectF(rect.width() - 30, 10, 20, 20);
    
    painter->drawText(m_prevButtonRect, BobUI::AlignCenter, "<");
    painter->drawText(m_nextButtonRect, BobUI::AlignCenter, ">");

    // Days of week header
    font.setBold(false);
    font.setPointSize(10);
    painter->setFont(font);
    
    QStringList days = {"Su", "Mo", "Tu", "We", "Th", "Fr", "Sa"};
    for (int i = 0; i < 7; ++i) {
        painter->setPen(QColor("#888888"));
        painter->drawText(QRectF(i * m_cellWidth, 40, m_cellWidth, m_cellHeight), BobUI::AlignCenter, days[i]);
    }

    // Grid Dates
    int firstDayOfWeek = m_currentMonth.dayOfWeek(); // 1 = Monday, 7 = Sunday
    int offset = (firstDayOfWeek == 7) ? 0 : firstDayOfWeek;
    int daysInMonth = m_currentMonth.daysInMonth();

    int row = 0;
    int col = offset;

    for (int day = 1; day <= daysInMonth; ++day) {
        QRectF cellRect(col * m_cellWidth, 40 + m_cellHeight + (row * m_cellHeight), m_cellWidth, m_cellHeight);
        
        // Highlight selected
        if (m_selectedDate.year() == m_currentMonth.year() && 
            m_selectedDate.month() == m_currentMonth.month() && 
            m_selectedDate.day() == day) 
        {
            QPainterPath bgPath;
            bgPath.addRoundedRect(cellRect.adjusted(2, 2, -2, -2), 4, 4);
            painter->fillPath(bgPath, m_accentColor);
            painter->setPen(BobUI::white);
        } else {
            painter->setPen(BobUI::white);
        }

        painter->drawText(cellRect, BobUI::AlignCenter, QString::number(day));

        col++;
        if (col > 6) {
            col = 0;
            row++;
        }
    }
}
