#ifndef OMNITILEPANE_H
#define OMNITILEPANE_H

#include <QQuickItem>

class OmniTilePane : public QQuickItem {
    Q_OBJECT
    Q_PROPERTY(int prefTileWidth READ prefTileWidth WRITE setPrefTileWidth NOTIFY prefTileWidthChanged)
    Q_PROPERTY(int prefTileHeight READ prefTileHeight WRITE setPrefTileHeight NOTIFY prefTileHeightChanged)

public:
    explicit OmniTilePane(QQuickItem *parent = nullptr);
    virtual ~OmniTilePane();

    int prefTileWidth() const; void setPrefTileWidth(int w);
    int prefTileHeight() const; void setPrefTileHeight(int h);

signals:
    void prefTileWidthChanged();
    void prefTileHeightChanged();

protected:
    void geometryChange(const QRectF &newGeometry, const QRectF &oldGeometry) override;
    void itemChildAdded(QQuickItem *child) override;

private:
    void layoutChildren();
    int m_tileW;
    int m_tileH;
};

#endif // OMNITILEPANE_H
