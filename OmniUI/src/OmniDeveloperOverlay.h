#ifndef OMNIDEVELOPEROVERLAY_H
#define OMNIDEVELOPEROVERLAY_H

#include <QWidget>

class OmniDeveloperOverlay : public QWidget {
    Q_OBJECT
public:
    explicit OmniDeveloperOverlay(QWidget *parent = nullptr);
    ~OmniDeveloperOverlay();

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
};

#endif // OMNIDEVELOPEROVERLAY_H