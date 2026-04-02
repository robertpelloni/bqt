#ifndef OMNISLIDER_H
#define OMNISLIDER_H

#include <QQuickPaintedItem>
#include <QColor>

class OmniSlider : public QQuickPaintedItem {
    Q_OBJECT
    Q_PROPERTY(qreal value READ value WRITE setValue NOTIFY valueChanged)
    Q_PROPERTY(qreal minimum READ minimum WRITE setMinimum NOTIFY minimumChanged)
    Q_PROPERTY(qreal maximum READ maximum WRITE setMaximum NOTIFY maximumChanged)
    Q_PROPERTY(QColor accentColor READ accentColor WRITE setAccentColor NOTIFY accentColorChanged)

public:
    explicit OmniSlider(QQuickItem *parent = nullptr);
    virtual ~OmniSlider();

    qreal value() const;
    void setValue(qreal val);

    qreal minimum() const;
    void setMinimum(qreal min);

    qreal maximum() const;
    void setMaximum(qreal max);

    QColor accentColor() const;
    void setAccentColor(const QColor& color);

    void paint(QPainter *painter) override;

signals:
    void valueChanged();
    void minimumChanged();
    void maximumChanged();
    void accentColorChanged();

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

private:
    void updateValueFromMouse(const QPointF& pos);

    qreal m_value;
    qreal m_minimum;
    qreal m_maximum;
    QColor m_accentColor;
    bool m_isDragging;
};

#endif // OMNISLIDER_H
