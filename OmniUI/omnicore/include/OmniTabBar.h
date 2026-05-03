#ifndef OMNITABBAR_H
#define OMNITABBAR_H

#include <QQuickPaintedItem>
#include <QStringList>
#include <QColor>
#include <QList>

class OmniTabBar : public QQuickPaintedItem {
    Q_OBJECT
    Q_PROPERTY(QStringList tabs READ tabs WRITE setTabs NOTIFY tabsChanged)
    Q_PROPERTY(int currentIndex READ currentIndex WRITE setCurrentIndex NOTIFY currentIndexChanged)
    Q_PROPERTY(QColor accentColor READ accentColor WRITE setAccentColor NOTIFY accentColorChanged)

public:
    explicit OmniTabBar(QQuickItem *parent = nullptr);
    virtual ~OmniTabBar();

    QStringList tabs() const;
    void setTabs(const QStringList& tabs);

    int currentIndex() const;
    void setCurrentIndex(int index);

    QColor accentColor() const;
    void setAccentColor(const QColor& color);

    Q_INVOKABLE void addTab(const QString& title);
    Q_INVOKABLE void removeTab(int index);

    void paint(QPainter *painter) override;

signals:
    void tabsChanged();
    void currentIndexChanged(int index);
    void accentColorChanged();
    void tabClicked(int index);

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void hoverMoveEvent(QHoverEvent *event) override;
    void hoverLeaveEvent(QHoverEvent *event) override;

private:
    int tabAt(const QPointF& pos) const;

    QStringList m_tabs;
    int m_currentIndex;
    QColor m_accentColor;
    
    int m_hoveredIndex;
    
    // Geometry caching
    QList<QRectF> m_tabRects;
    void recalculateRects();
};

#endif // OMNITABBAR_H
