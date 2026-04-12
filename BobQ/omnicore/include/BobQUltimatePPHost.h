#ifndef BOBQULTIMATEPPHOST_H
#define BOBQULTIMATEPPHOST_H

#include <QQuickPaintedItem>
#include <QImage>
#include <QPainter>
#include <QMouseEvent>
#include <QKeyEvent>
#include <memory>

// Forward declaration of U++ classes
// In reality, this lives in <CtrlCore/CtrlCore.h>
namespace Upp {
    class Ctrl;
    class SystemDraw;
    class Image;
}

/**
 * @class BobQUltimatePPHost
 * @brief A QQuickPaintedItem that bridges U++ framework layouts into Qt.
 *
 * Achieves 1:1 Ultimate++ UI parity by rendering a U++ `Ctrl` node
 * into a QImage memory buffer and painting it onto the Qt scenegraph.
 * Maps BobQ/Qt interactions (mouse, keyboard, focus) directly into the
 * internal U++ `Ctrl` event dispatcher.
 */
class BobQUltimatePPHost : public QQuickPaintedItem {
    Q_OBJECT
    QML_ELEMENT
public:
    explicit BobQUltimatePPHost(QQuickItem *parent = nullptr);
    virtual ~BobQUltimatePPHost();

    // Sets the underlying U++ control to be hosted. BobQ takes ownership.
    void setUppCtrl(std::unique_ptr<Upp::Ctrl> ctrl);
    Upp::Ctrl* uppCtrl() const;

    void paint(QPainter *painter) override;

protected:
    void geometryChange(const QRectF &newGeometry, const QRectF &oldGeometry) override;

    // Qt -> U++ Input Forwarding
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void hoverMoveEvent(QHoverEvent *event) override;

    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;

private:
    void renderUppToImage();

    std::unique_ptr<Upp::Ctrl> m_uppCtrl;
    QImage m_renderBuffer;
    bool m_needsRepaint;
};

#endif // BOBQULTIMATEPPHOST_H
