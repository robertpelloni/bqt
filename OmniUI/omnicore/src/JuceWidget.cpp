#include "JuceWidget.h"
#include <QPainter>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QDebug>

// Mock for juce::Component for now
namespace juce {
    class Component {
    public:
        virtual ~Component() = default;
        virtual void setBounds(int x, int y, int w, int h) {
            qDebug() << "[JUCE] Setting bounds:" << x << y << w << h;
        }
        virtual void paint(QPainter& painter) {
            painter.setPen(BobUI::blue);
            painter.drawText(10, 20, "JUCE Component Rendered via BobUI");
        }
    };
}

JuceWidget::JuceWidget(QWidget *parent)
    : QWidget(parent)
{
    setAttribute(BobUI::WA_OpaquePaintEvent);
    setAttribute(BobUI::WA_NoSystemBackground);
}

JuceWidget::~JuceWidget() = default;

void JuceWidget::setJuceComponent(std::unique_ptr<juce::Component> component)
{
    m_juceComponent = std::move(component);
    if (m_juceComponent) {
        m_juceComponent->setBounds(0, 0, width(), height());
    }
    update();
}

juce::Component* JuceWidget::juceComponent() const
{
    return m_juceComponent.get();
}

void JuceWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    if (m_juceComponent) {
        m_juceComponent->paint(painter);
    } else {
        painter.fillRect(rect(), BobUI::black);
        painter.setPen(BobUI::white);
        painter.drawText(rect(), BobUI::AlignCenter, "No JUCE Component Loaded");
    }
}

void JuceWidget::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    if (m_juceComponent) {
        m_juceComponent->setBounds(0, 0, width(), height());
    }
}

void JuceWidget::mousePressEvent(QMouseEvent *event)
{
    qDebug() << "JUCE Mouse Press at:" << event->pos();
    // In a real implementation, we would route to m_juceComponent->mouseDown()
}

void JuceWidget::mouseReleaseEvent(QMouseEvent *event)
{
    qDebug() << "JUCE Mouse Release at:" << event->pos();
}

void JuceWidget::mouseMoveEvent(QMouseEvent *event)
{
    // Route to JUCE mouse drag/move
}

void JuceWidget::keyPressEvent(QKeyEvent *event)
{
    qDebug() << "JUCE Key Press:" << event->text();
}

void JuceWidget::keyReleaseEvent(QKeyEvent *event)
{
    // Route to JUCE key release
}