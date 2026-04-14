#include "BobQJuceHost.h"
#include <QDebug>

// In a real build, we would include juce_gui_basics/juce_gui_basics.h
// We mock the definitions here structurally for demonstration of the bridge architecture.
namespace juce {
    class Image {
    public:
        enum PixelFormat { ARGB };
        Image(PixelFormat format, int width, int height, bool clearImage) {}
        int getWidth() const { return 100; }
        int getHeight() const { return 100; }
    };

    class Graphics {
    public:
        Graphics(Image& image) {}
    };

    class Component {
    public:
        virtual ~Component() {}
        virtual void setBounds(int x, int y, int width, int height) {}
        virtual void paint(Graphics& g) {}
        virtual void mouseDown(int x, int y) {}
        virtual void mouseUp(int x, int y) {}
        virtual void mouseDrag(int x, int y) {}
        virtual void mouseMove(int x, int y) {}
        virtual bool keyPressed(int keyCode) { return false; }
    };
}

BobQJuceHost::BobQJuceHost(QQuickItem *parent)
    : QQuickPaintedItem(parent), m_needsRepaint(true)
{
    setAcceptedMouseButtons(Qt::AllButtons);
    setAcceptHoverEvents(true);
}

BobQJuceHost::~BobQJuceHost() {
    // Unique ptr will clean up the JUCE component automatically
}

void BobQJuceHost::setJuceComponent(std::unique_ptr<juce::Component> component) {
    m_juceComponent = std::move(component);
    if (m_juceComponent) {
        m_juceComponent->setBounds(0, 0, width(), height());
    }
    m_needsRepaint = true;
    update();
}

juce::Component* BobQJuceHost::juceComponent() const {
    return m_juceComponent.get();
}

void BobQJuceHost::geometryChange(const QRectF &newGeometry, const QRectF &oldGeometry) {
    QQuickPaintedItem::geometryChange(newGeometry, oldGeometry);

    if (m_juceComponent) {
        m_juceComponent->setBounds(0, 0, newGeometry.width(), newGeometry.height());
    }

    // Resize our internal Qt render buffer
    if (newGeometry.width() > 0 && newGeometry.height() > 0) {
        m_renderBuffer = QImage(newGeometry.width(), newGeometry.height(), QImage::Format_ARGB32_Premultiplied);
    }

    m_needsRepaint = true;
    update();
}

void BobQJuceHost::renderJuceToImage() {
    if (!m_juceComponent || m_renderBuffer.isNull()) return;

    // In a real implementation:
    // 1. Create a juce::Image mapped to the QImage's raw pixel data pointer (m_renderBuffer.bits()).
    // 2. Create a juce::Graphics context for that juce::Image.
    // 3. Call m_juceComponent->paintWholeComponent(graphicsContext, ...);

    m_renderBuffer.fill(Qt::transparent);

    // Mock simulation:
    juce::Image juceImg(juce::Image::ARGB, m_renderBuffer.width(), m_renderBuffer.height(), true);
    juce::Graphics g(juceImg);
    m_juceComponent->paint(g);

    m_needsRepaint = false;
}

void BobQJuceHost::paint(QPainter *painter) {
    if (m_needsRepaint) {
        renderJuceToImage();
    }

    // Blit the rendered JUCE buffer directly onto the Qt QML scene
    if (!m_renderBuffer.isNull()) {
        painter->drawImage(0, 0, m_renderBuffer);
    }
}

// --- Event Bridging ---
// Map Qt events into JUCE coordinate space and trigger component interactions

void BobQJuceHost::mousePressEvent(QMouseEvent *event) {
    if (m_juceComponent) {
        m_juceComponent->mouseDown(event->position().x(), event->position().y());
        m_needsRepaint = true;
        update();
    }
}

void BobQJuceHost::mouseReleaseEvent(QMouseEvent *event) {
    if (m_juceComponent) {
        m_juceComponent->mouseUp(event->position().x(), event->position().y());
        m_needsRepaint = true;
        update();
    }
}

void BobQJuceHost::mouseMoveEvent(QMouseEvent *event) {
    if (m_juceComponent) {
        m_juceComponent->mouseDrag(event->position().x(), event->position().y());
        m_needsRepaint = true;
        update();
    }
}

void BobQJuceHost::hoverMoveEvent(QHoverEvent *event) {
    if (m_juceComponent) {
        m_juceComponent->mouseMove(event->position().x(), event->position().y());
        m_needsRepaint = true;
        update();
    }
}

void BobQJuceHost::keyPressEvent(QKeyEvent *event) {
    if (m_juceComponent) {
        m_juceComponent->keyPressed(event->key());
        m_needsRepaint = true;
        update();
    }
}

void BobQJuceHost::keyReleaseEvent(QKeyEvent *event) {
    // JUCE key release handling
}
