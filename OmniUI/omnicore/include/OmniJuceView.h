#ifndef OMNIJUCEVIEW_H
#define OMNIJUCEVIEW_H

#include <QQuickPaintedItem>
#include <memory>

// Forward declaration of a JUCE component
namespace juce {
    class Component;
}

class OmniJuceView : public QQuickPaintedItem {
    Q_OBJECT
public:
    explicit OmniJuceView(QQuickItem *parent = nullptr);
    virtual ~OmniJuceView();

    // Sets the underlying JUCE component to be hosted
    void setJuceComponent(std::unique_ptr<juce::Component> component);
    juce::Component* juceComponent() const;

    void paint(QPainter *painter) override;

protected:
    void geometryChange(const QRectF &newGeometry, const QRectF &oldGeometry) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;

private:
    std::unique_ptr<juce::Component> m_juceComponent;
};

#endif // OMNIJUCEVIEW_H
