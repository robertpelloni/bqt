#ifndef OMNIDEVELOPEROVERLAY_H
#define OMNIDEVELOPEROVERLAY_H

#include <QQuickItem>

class OmniDeveloperOverlay : public QQuickItem {
    Q_OBJECT
public:
    explicit OmniDeveloperOverlay(QQuickItem *parent = nullptr);
    ~OmniDeveloperOverlay();

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
};

#endif // OMNIDEVELOPEROVERLAY_H