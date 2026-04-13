#ifndef OMNISPLITVIEW_H
#define OMNISPLITVIEW_H

#include <QQuickItem>
#include <BobUI> // For BobUI::Orientation

class OmniSplitView : public QQuickItem {
    Q_OBJECT
    Q_PROPERTY(BobUI::Orientation orientation READ orientation WRITE setOrientation NOTIFY orientationChanged)
    Q_PROPERTY(qreal handleSize READ handleSize WRITE setHandleSize NOTIFY handleSizeChanged)

public:
    explicit OmniSplitView(QQuickItem *parent = nullptr);
    virtual ~OmniSplitView();

    BobUI::Orientation orientation() const;
    void setOrientation(BobUI::Orientation o);

    qreal handleSize() const;
    void setHandleSize(qreal size);

signals:
    void orientationChanged();
    void handleSizeChanged();

protected:
    void geometryChange(const QRectF &newGeometry, const QRectF &oldGeometry) override;

private:
    void layoutChildren();

    BobUI::Orientation m_orientation;
    qreal m_handleSize;
};

#endif // OMNISPLITVIEW_H
