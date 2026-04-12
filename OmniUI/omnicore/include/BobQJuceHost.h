#ifndef BOBQJUCEHOST_H
#define BOBQJUCEHOST_H

#include <QQuickPaintedItem>
#include <QImage>
#include <QPainter>
#include <QMouseEvent>
#include <QKeyEvent>
#include <memory>

// Forward declaration of JUCE classes
namespace juce {
    class Component;
    class Graphics;
    class Image;
}

/**
 * @class BobQJuceHost
 * @brief A QQuickPaintedItem that acts as a bridge to host a juce::Component.
 *
 * This class achieves 1:1 JUCE UI parity by rendering a juce::Component
 * into a QImage memory buffer and painting it onto a Qt QQuick painted surface.
 * It manually intercepts Qt mouse and keyboard events and marshals them
 * into equivalent JUCE events, sending them directly into the JUCE component tree.
 */
class BobQJuceHost : public QQuickPaintedItem {
    Q_OBJECT
    QML_ELEMENT
public:
    explicit BobQJuceHost(QQuickItem *parent = nullptr);
    virtual ~BobQJuceHost();

    // Sets the underlying JUCE component to be hosted. BobQ takes ownership.
    void setJuceComponent(std::unique_ptr<juce::Component> component);
    juce::Component* juceComponent() const;

    void paint(QPainter *painter) override;

protected:
    void geometryChange(const QRectF &newGeometry, const QRectF &oldGeometry) override;

    // Mouse Event Interception
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void hoverMoveEvent(QHoverEvent *event) override;

    // Key Event Interception
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;

private:
    void renderJuceToImage();

    std::unique_ptr<juce::Component> m_juceComponent;
    QImage m_renderBuffer;
    bool m_needsRepaint;
};

#endif // BOBQJUCEHOST_H
