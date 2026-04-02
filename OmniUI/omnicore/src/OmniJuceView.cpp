#include "OmniJuceView.h"
#include <QPainter>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QDebug>

// Mock for juce::Component
namespace juce {
    class Component {
    public:
        virtual ~Component() = default;
        virtual void setBounds(int x, int y, int w, int h) {
            qDebug() << "[JUCE] Setting bounds:" << x << y << w << h;
        }
        virtual void paint(QPainter& painter) {
            painter.setPen(Qt::magenta);
            painter.drawText(20, 40, "NATIVE JUCE COMPONENT IN SCENEGRAPH");
            painter.drawRect(0, 0, 100, 100);
        }
    };
}

OmniJuceView::OmniJuceView(QQuickItem *parent)
    : QQuickPaintedItem(parent)
{
    setObjectName("OmniJuceView");
    setAcceptedMouseButtons(Qt::AllButtons);
    setFlag(ItemAcceptsInputMethod, true);
    setFlag(ItemHasContents, true);
}

OmniJuceView::~OmniJuceView() = default;

void OmniJuceView::setJuceComponent(std::unique_ptr<juce::Component> component)
{
    m_juceComponent = std::move(component);
    if (m_juceComponent) {
        m_juceComponent->setBounds(0, 0, width(), height());
    }
    update();
}

juce::Component* OmniJuceView::juceComponent() const
{
    return m_juceComponent.get();
}

void OmniJuceView::paint(QPainter *painter)
{
    if (m_juceComponent) {
        // Here we bridge QPainter to JUCE Graphics.
        // In a true RHI implementation, we would bypass QPainter entirely
        // and share a texture ID between JUCE's OpenGL context and Qt's Vulkan/D3D context.
        m_juceComponent->paint(*painter);
    } else {
        painter->fillRect(boundingRect(), QColor("#111111"));
        painter->setPen(Qt::white);
        painter->drawText(boundingRect(), Qt::AlignCenter, "JUCE Container Empty");
    }
}

void OmniJuceView::geometryChange(const QRectF &newGeometry, const QRectF &oldGeometry)
{
    QQuickPaintedItem::geometryChange(newGeometry, oldGeometry);
    if (m_juceComponent) {
        m_juceComponent->setBounds(0, 0, newGeometry.width(), newGeometry.height());
    }
}

void OmniJuceView::mousePressEvent(QMouseEvent *event)
{
    qDebug() << "OmniJuceView: Routing MousePress to JUCE at" << event->position();
    // Route to m_juceComponent->mouseDown()
    QQuickPaintedItem::mousePressEvent(event);
}

void OmniJuceView::mouseReleaseEvent(QMouseEvent *event)
{
    // Route to JUCE
    QQuickPaintedItem::mouseReleaseEvent(event);
}

void OmniJuceView::mouseMoveEvent(QMouseEvent *event)
{
    // Route to JUCE
    QQuickPaintedItem::mouseMoveEvent(event);
}

void OmniJuceView::keyPressEvent(QKeyEvent *event)
{
    qDebug() << "OmniJuceView: Routing KeyPress to JUCE:" << event->key();
    // Route to JUCE
    QQuickPaintedItem::keyPressEvent(event);
}

void OmniJuceView::keyReleaseEvent(QKeyEvent *event)
{
    QQuickPaintedItem::keyReleaseEvent(event);
}
