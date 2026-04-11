#ifndef OMNILISTVIEW_H
#define OMNILISTVIEW_H

#include <QQuickPaintedItem>
#include <QVariantList>

class OmniListView : public QQuickPaintedItem {
    Q_OBJECT
    Q_PROPERTY(QVariantList model READ model WRITE setModel NOTIFY modelChanged)
    Q_PROPERTY(int currentIndex READ currentIndex WRITE setCurrentIndex NOTIFY currentIndexChanged)
    Q_PROPERTY(int rowHeight READ rowHeight WRITE setRowHeight NOTIFY rowHeightChanged)

public:
    explicit OmniListView(QQuickItem *parent = nullptr);
    virtual ~OmniListView();

    QVariantList model() const;
    void setModel(const QVariantList& model);

    int currentIndex() const;
    void setCurrentIndex(int index);

    int rowHeight() const;
    void setRowHeight(int h);

    void paint(QPainter *painter) override;

signals:
    void modelChanged();
    void currentIndexChanged(int index);
    void rowHeightChanged();
    void itemClicked(int index, const QVariant& itemData);

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;

private:
    QVariantList m_model;
    int m_currentIndex;
    int m_rowHeight;
    int m_scrollY;
};

#endif // OMNILISTVIEW_H
