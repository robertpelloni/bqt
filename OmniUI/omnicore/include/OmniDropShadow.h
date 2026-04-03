#ifndef OMNIDROPSHADOW_H
#define OMNIDROPSHADOW_H

#include <QQuickItem>
#include <QColor>

class OmniDropShadow : public QQuickItem {
    Q_OBJECT
    Q_PROPERTY(qreal radius READ radius WRITE setRadius NOTIFY effectChanged)
    Q_PROPERTY(QColor color READ color WRITE setColor NOTIFY effectChanged)
    Q_PROPERTY(QPointF offset READ offset WRITE setOffset NOTIFY effectChanged)

public:
    explicit OmniDropShadow(QQuickItem *parent = nullptr);
    virtual ~OmniDropShadow();

    qreal radius() const { return m_radius; }
    void setRadius(qreal r) { m_radius = r; emit effectChanged(); update(); }

    QColor color() const { return m_color; }
    void setColor(const QColor& c) { m_color = c; emit effectChanged(); update(); }

    QPointF offset() const { return m_offset; }
    void setOffset(const QPointF& o) { m_offset = o; emit effectChanged(); update(); }

signals:
    void effectChanged();

protected:
    QSGNode* updatePaintNode(QSGNode* oldNode, UpdatePaintNodeData*) override;

private:
    qreal m_radius;
    QColor m_color;
    QPointF m_offset;
};

#endif // OMNIDROPSHADOW_H
