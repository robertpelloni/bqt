#include "BobQUltimatePPHost.h"
#include <QDebug>

// Mock definitions for the U++ namespace to ensure clean CMake validation
// until the UPP_ROOT variables and build scripts fully link the entire library.
namespace Upp {
    class Image {
    public:
        Image() {}
    };

    class SystemDraw {
    public:
        SystemDraw() {}
    };

    class Ctrl {
    public:
        virtual ~Ctrl() {}
        virtual void SetRect(int x, int y, int width, int height) {}
        virtual void Paint(SystemDraw& draw) {}

        // U++ uses LeftDown, LeftUp, MouseMove, etc.
        virtual void LeftDown(int x, int y, int flags) {}
        virtual void LeftUp(int x, int y, int flags) {}
        virtual void MouseMove(int x, int y, int flags) {}
        virtual bool Key(int keyCode, int count) { return false; }
    };
}

BobQUltimatePPHost::BobQUltimatePPHost(QQuickItem *parent)
    : QQuickPaintedItem(parent), m_needsRepaint(true)
{
    setAcceptedMouseButtons(Qt::AllButtons);
    setAcceptHoverEvents(true);
}

BobQUltimatePPHost::~BobQUltimatePPHost() {
    // Unique ptr will clean up the U++ Ctrl automatically
}

void BobQUltimatePPHost::setUppCtrl(std::unique_ptr<Upp::Ctrl> ctrl) {
    m_uppCtrl = std::move(ctrl);
    if (m_uppCtrl) {
        m_uppCtrl->SetRect(0, 0, static_cast<int>(width()), static_cast<int>(height()));
    }
    m_needsRepaint = true;
    update();
}

Upp::Ctrl* BobQUltimatePPHost::uppCtrl() const {
    return m_uppCtrl.get();
}

void BobQUltimatePPHost::geometryChange(const QRectF &newGeometry, const QRectF &oldGeometry) {
    QQuickPaintedItem::geometryChange(newGeometry, oldGeometry);

    if (m_uppCtrl) {
        m_uppCtrl->SetRect(0, 0, static_cast<int>(newGeometry.width()), static_cast<int>(newGeometry.height()));
    }

    if (newGeometry.width() > 0 && newGeometry.height() > 0) {
        m_renderBuffer = QImage(newGeometry.width(), newGeometry.height(), QImage::Format_ARGB32_Premultiplied);
    }

    m_needsRepaint = true;
    update();
}

void BobQUltimatePPHost::renderUppToImage() {
    if (!m_uppCtrl || m_renderBuffer.isNull()) return;

    // In a full implementation, we'd map a U++ SystemDraw instance to the QImage's
    // raw pixel data pointer (m_renderBuffer.bits()) or use a U++ ImageBuffer.

    m_renderBuffer.fill(Qt::transparent);

    // Mock simulation:
    Upp::SystemDraw drawContext;
    m_uppCtrl->Paint(drawContext);

    m_needsRepaint = false;
}

void BobQUltimatePPHost::paint(QPainter *painter) {
    if (m_needsRepaint) {
        renderUppToImage();
    }

    // Blit the rendered U++ buffer directly onto the Qt QML scene
    if (!m_renderBuffer.isNull()) {
        painter->drawImage(0, 0, m_renderBuffer);
    }
}

// --- Event Bridging ---
// Map Qt events into U++ coordinate space and trigger component interactions

void BobQUltimatePPHost::mousePressEvent(QMouseEvent *event) {
    if (m_uppCtrl) {
        // Map Qt mouse buttons to U++ flags
        int flags = 0; // Upp::K_MOUSELEFT, etc.
        m_uppCtrl->LeftDown(static_cast<int>(event->position().x()), static_cast<int>(event->position().y()), flags);
        m_needsRepaint = true;
        update();
    }
}

void BobQUltimatePPHost::mouseReleaseEvent(QMouseEvent *event) {
    if (m_uppCtrl) {
        int flags = 0;
        m_uppCtrl->LeftUp(static_cast<int>(event->position().x()), static_cast<int>(event->position().y()), flags);
        m_needsRepaint = true;
        update();
    }
}

void BobQUltimatePPHost::mouseMoveEvent(QMouseEvent *event) {
    if (m_uppCtrl) {
        int flags = 0;
        m_uppCtrl->MouseMove(static_cast<int>(event->position().x()), static_cast<int>(event->position().y()), flags);
        m_needsRepaint = true;
        update();
    }
}

void BobQUltimatePPHost::hoverMoveEvent(QHoverEvent *event) {
    if (m_uppCtrl) {
        int flags = 0;
        m_uppCtrl->MouseMove(static_cast<int>(event->position().x()), static_cast<int>(event->position().y()), flags);
        m_needsRepaint = true;
        update();
    }
}

void BobQUltimatePPHost::keyPressEvent(QKeyEvent *event) {
    if (m_uppCtrl) {
        // Map Qt Key to Upp::K_* constants
        m_uppCtrl->Key(event->key(), 1);
        m_needsRepaint = true;
        update();
    }
}

void BobQUltimatePPHost::keyReleaseEvent(QKeyEvent *event) {
    // U++ typically handles key up globally or specifically per-control via KeyUp
}
