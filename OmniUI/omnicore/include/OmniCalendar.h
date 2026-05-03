#ifndef OMNICALENDAR_H
#define OMNICALENDAR_H

#include <QQuickPaintedItem>
#include <QDate>
#include <QColor>

class OmniCalendar : public QQuickPaintedItem {
    Q_OBJECT
    Q_PROPERTY(QDate selectedDate READ selectedDate WRITE setSelectedDate NOTIFY selectedDateChanged)
    Q_PROPERTY(QColor accentColor READ accentColor WRITE setAccentColor NOTIFY accentColorChanged)

public:
    explicit OmniCalendar(QQuickItem *parent = nullptr);
    virtual ~OmniCalendar();

    QDate selectedDate() const;
    void setSelectedDate(const QDate& date);

    QColor accentColor() const;
    void setAccentColor(const QColor& color);

    void paint(QPainter *painter) override;

signals:
    void selectedDateChanged();
    void accentColorChanged();

protected:
    void mousePressEvent(QMouseEvent *event) override;

private:
    QDate m_selectedDate;
    QDate m_currentMonth;
    QColor m_accentColor;
    
    QRectF m_prevButtonRect;
    QRectF m_nextButtonRect;
    
    int m_cellWidth;
    int m_cellHeight;
};

#endif // OMNICALENDAR_H
