#ifndef OMNIPLOT_H
#define OMNIPLOT_H

#include <QQuickPaintedItem>
#include <QList>
#include <QPointF>

class OmniPlot : public QQuickPaintedItem {
    Q_OBJECT
    Q_PROPERTY(QList<qreal> data READ data WRITE setData NOTIFY dataChanged)

public:
    explicit OmniPlot(QQuickItem *parent = nullptr);
    virtual ~OmniPlot();

    QList<qreal> data() const;
    void setData(const QList<qreal>& d);

    Q_INVOKABLE void addPoint(qreal p);

    void paint(QPainter *painter) override;

signals:
    void dataChanged();

private:
    QList<qreal> m_data;
    int m_maxPoints;
};

#endif // OMNIPLOT_H
