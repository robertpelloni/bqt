#ifndef OMNISTACKPANE_H
#define OMNISTACKPANE_H

#include <QQuickItem>
#include <BobUI>

class OmniStackPane : public QQuickItem {
    Q_OBJECT
    Q_PROPERTY(BobUI::Alignment alignment READ alignment WRITE setAlignment NOTIFY alignmentChanged)

public:
    explicit OmniStackPane(QQuickItem *parent = nullptr);
    virtual ~OmniStackPane();

    BobUI::Alignment alignment() const;
    void setAlignment(BobUI::Alignment align);

signals:
    void alignmentChanged();

protected:
    void geometryChange(const QRectF &newGeometry, const QRectF &oldGeometry) override;
    void itemChildAdded(QQuickItem *child) override;

private:
    void layoutChildren();
    BobUI::Alignment m_alignment;
};

#endif // OMNISTACKPANE_H
