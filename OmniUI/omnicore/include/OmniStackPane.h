#ifndef OMNISTACKPANE_H
#define OMNISTACKPANE_H

#include <QQuickItem>
#include <Qt>

class OmniStackPane : public QQuickItem {
    Q_OBJECT
    Q_PROPERTY(Qt::Alignment alignment READ alignment WRITE setAlignment NOTIFY alignmentChanged)

public:
    explicit OmniStackPane(QQuickItem *parent = nullptr);
    virtual ~OmniStackPane();

    Qt::Alignment alignment() const;
    void setAlignment(Qt::Alignment align);

signals:
    void alignmentChanged();

protected:
    void geometryChange(const QRectF &newGeometry, const QRectF &oldGeometry) override;
    void itemChildAdded(QQuickItem *child) override;

private:
    void layoutChildren();
    Qt::Alignment m_alignment;
};

#endif // OMNISTACKPANE_H
