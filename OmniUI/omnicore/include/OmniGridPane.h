#ifndef OMNIGRIDPANE_H
#define OMNIGRIDPANE_H

#include <QQuickItem>
#include <QMap>

class OmniGridPane : public QQuickItem {
    Q_OBJECT
    Q_PROPERTY(int horizontalGap READ horizontalGap WRITE setHorizontalGap NOTIFY horizontalGapChanged)
    Q_PROPERTY(int verticalGap READ verticalGap WRITE setVerticalGap NOTIFY verticalGapChanged)

public:
    explicit OmniGridPane(QQuickItem *parent = nullptr);
    virtual ~OmniGridPane();

    int horizontalGap() const;
    void setHorizontalGap(int gap);

    int verticalGap() const;
    void setVerticalGap(int gap);

    // Dynamic grid assignment for children (JavaFX parity)
    Q_INVOKABLE void setConstraints(QQuickItem* item, int column, int row);

signals:
    void horizontalGapChanged();
    void verticalGapChanged();

protected:
    void geometryChange(const QRectF &newGeometry, const QRectF &oldGeometry) override;
    void itemChildAdded(QQuickItem *child) override;

private:
    void layoutChildren();
    
    int m_hGap;
    int m_vGap;

    struct GridPos { int col; int row; };
    QMap<QQuickItem*, GridPos> m_constraints;
};

#endif // OMNIGRIDPANE_H
