#ifndef OMNIFLOWPANE_H
#define OMNIFLOWPANE_H

#include <QQuickItem>

class OmniFlowPane : public QQuickItem {
    Q_OBJECT
    Q_PROPERTY(int hgap READ hgap WRITE setHgap NOTIFY gapsChanged)
    Q_PROPERTY(int vgap READ vgap WRITE setVgap NOTIFY gapsChanged)

public:
    explicit OmniFlowPane(QQuickItem *parent = nullptr);
    virtual ~OmniFlowPane();

    int hgap() const; void setHgap(int gap);
    int vgap() const; void setVgap(int gap);

signals:
    void gapsChanged();

protected:
    void geometryChange(const QRectF &newGeometry, const QRectF &oldGeometry) override;
    void itemChildAdded(QQuickItem *child) override;

private:
    void layoutChildren();
    int m_hgap;
    int m_vgap;
};

#endif // OMNIFLOWPANE_H
