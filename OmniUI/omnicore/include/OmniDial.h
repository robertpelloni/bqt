#ifndef OMNIDIAL_H
#define OMNIDIAL_H

#include <QQuickPaintedItem>
#include <QColor>

class OmniDial : public QQuickPaintedItem {
    Q_OBJECT
    Q_PROPERTY(qreal value READ value WRITE setValue NOTIFY valueChanged)
    Q_PROPERTY(qreal minimum READ minimum WRITE setMinimum NOTIFY minimumChanged)
    Q_PROPERTY(qreal maximum READ maximum WRITE setMaximum NOTIFY maximumChanged)
    Q_PROPERTY(QColor accentColor READ accentColor WRITE setAccentColor NOTIFY accentColorChanged)
    Q_PROPERTY(QString valueTreeKey READ valueTreeKey WRITE setValueTreeKey NOTIFY valueTreeKeyChanged)

public:
    explicit OmniDial(QQuickItem *parent = nullptr);
    virtual ~OmniDial();

    qreal value() const;
    void setValue(qreal val);

    qreal minimum() const;
    void setMinimum(qreal min);

    qreal maximum() const;
    void setMaximum(qreal max);

    QColor accentColor() const;
    void setAccentColor(const QColor& color);

    QString valueTreeKey() const;
    void setValueTreeKey(const QString& key);

    void paint(QPainter *painter) override;

signals:
    void valueChanged();
    void minimumChanged();
    void maximumChanged();
    void accentColorChanged();
    void valueTreeKeyChanged();

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
    QString m_valueTreeKey;
    bool m_isDragging;
    QPointF m_lastMousePos;
};

#endif // OMNIDIAL_H
