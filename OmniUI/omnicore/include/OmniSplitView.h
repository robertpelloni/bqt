#ifndef OMNISPLITVIEW_H
#define OMNISPLITVIEW_H

#include <QQuickItem>
#include <Qt> // For Qt::Orientation

class OmniSplitView : public QQuickItem {
    Q_OBJECT
    Q_PROPERTY(Qt::Orientation orientation READ orientation WRITE setOrientation NOTIFY orientationChanged)
    Q_PROPERTY(qreal handleSize READ handleSize WRITE setHandleSize NOTIFY handleSizeChanged)

public:
    explicit OmniSplitView(QQuickItem *parent = nullptr);
    virtual ~OmniSplitView();

    Qt::Orientation orientation() const;
    void setOrientation(Qt::Orientation o);

    qreal handleSize() const;
    void setHandleSize(qreal size);

signals:
    void orientationChanged();
    void handleSizeChanged();

protected:
    void geometryChange(const QRectF &newGeometry, const QRectF &oldGeometry) override;

private:
    void layoutChildren();

    Qt::Orientation m_orientation;
    qreal m_handleSize;
};

#endif // OMNISPLITVIEW_H
