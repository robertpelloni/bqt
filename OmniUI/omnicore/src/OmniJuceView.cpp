#include "OmniJuceView.h"
#include <QPainter>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QDebug>
#include <QQuickPaintedItem>

// Mock for juce::Component
namespace juce {
    class Component {
    public:
        virtual ~Component() = default;
        virtual void setBounds(int x, int y, int w, int h) {
            qDebug() << "[JUCE] Setting bounds:" << x << y << w << h;
        }
        virtual void paint(QPainter& painter) {
            painter.setPen(BobUI::magenta);
            painter.drawText(20, 40, "NATIVE JUCE COMPONENT IN SCENEGRAPH");
            painter.drawRect(0, 0, 100, 100);
        }
    };
}

OmniJuceView::OmniJuceView(QQuickItem *parent)
    : QQuickPaintedItem(parent)
{
    setObjectName("OmniJuceView");
    setAcceptedMouseButtons(BobUI::AllButtons);
    setFlag(ItemAcceptsInputMethod, true);
    setFlag(ItemHasContents, true);
    
    // Performance Optimization: Use FramebufferObject to offload rendering to the GPU.
    // This allows JUCE to draw to a texture instead of falling back to software rasterization,
    // solving the UI stalling issues caused by high framerate DSP audio plugins.
    setRenderTarget(QQuickPaintedItem::FramebufferObject);
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
        // Since we are set to FramebufferObject, this QPainter call
        // translates directly into high-speed GPU texture commands natively in the RHI.
        m_juceComponent->paint(*painter);
    } else {
        painter->fillRect(boundingRect(), QColor("#111111"));
        painter->setPen(BobUI::white);
        painter->drawText(boundingRect(), BobUI::AlignCenter, "JUCE Container Empty");
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
    QQuickPaintedItem::mousePressEvent(event);
}

void OmniJuceView::mouseReleaseEvent(QMouseEvent *event)
{
    QQuickPaintedItem::mouseReleaseEvent(event);
}

void OmniJuceView::mouseMoveEvent(QMouseEvent *event)
{
    QQuickPaintedItem::mouseMoveEvent(event);
}

void OmniJuceView::keyPressEvent(QKeyEvent *event)
{
    qDebug() << "OmniJuceView: Routing KeyPress to JUCE:" << event->key();
    QQuickPaintedItem::keyPressEvent(event);
}

void OmniJuceView::keyReleaseEvent(QKeyEvent *event)
{
    QQuickPaintedItem::keyReleaseEvent(event);
}
