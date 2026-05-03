#include "OmniShaderEffect.h"
#include <QPainter>
#include <QPainterPath>
#include <cmath>

OmniShaderEffect::OmniShaderEffect(QQuickItem *parent)
    : QQuickPaintedItem(parent),
      m_effectType("plasma"),
      m_intensity(1.0),
      m_baseColor(QColor("#00F0FF")),
      m_time(0.0)
{
    setObjectName("OmniShaderEffect");
    setFlag(ItemHasContents, true);
    
    connect(&m_animTimer, &BOBUIimer::timeout, this, &OmniShaderEffect::advanceTime);
    m_animTimer.start(33); // ~30fps visual update for complex CPU shaders
}

OmniShaderEffect::~OmniShaderEffect() = default;

QString OmniShaderEffect::effectType() const { return m_effectType; }
void OmniShaderEffect::setEffectType(const QString& type) {
    if (m_effectType != type) { m_effectType = type; emit effectTypeChanged(); }
}

qreal OmniShaderEffect::intensity() const { return m_intensity; }
void OmniShaderEffect::setIntensity(qreal i) {
    if (!qFuzzyCompare(m_intensity, i)) { m_intensity = i; emit intensityChanged(); }
}

QColor OmniShaderEffect::baseColor() const { return m_baseColor; }
void OmniShaderEffect::setBaseColor(const QColor& c) {
    if (m_baseColor != c) { m_baseColor = c; emit baseColorChanged(); }
}

void OmniShaderEffect::advanceTime() {
    m_time += 0.1;
    if (m_time > 1000.0) m_time = 0.0;
    update();
}

void OmniShaderEffect::renderPlasmaGlow(QPainter* painter, const QRectF& rect) {
    // Mathematical simulation of a GPU plasma shader
    qreal cx = rect.width() / 2.0;
    qreal cy = rect.height() / 2.0;
    
    qreal x1 = cx + std::cos(m_time * 0.5) * (cx * 0.5);
    qreal y1 = cy + std::sin(m_time * 0.3) * (cy * 0.5);
    
    qreal x2 = cx + std::sin(m_time * 0.4) * (cx * 0.5);
    qreal y2 = cy + std::cos(m_time * 0.6) * (cy * 0.5);

    QRadialGradient grad1(QPointF(x1, y1), rect.width() * 0.8 * m_intensity);
    grad1.setColorAt(0, m_baseColor);
    grad1.setColorAt(1, BobUI::transparent);

    QRadialGradient grad2(QPointF(x2, y2), rect.width() * 0.6 * m_intensity);
    QColor c2 = m_baseColor.darker(150);
    grad2.setColorAt(0, c2);
    grad2.setColorAt(1, BobUI::transparent);

    painter->fillRect(rect, grad1);
    painter->setCompositionMode(QPainter::CompositionMode_Plus);
    painter->fillRect(rect, grad2);
    painter->setCompositionMode(QPainter::CompositionMode_SourceOver);
}

void OmniShaderEffect::renderCyberGrid(QPainter* painter, const QRectF& rect) {
    int gridSize = 40;
    qreal offset = std::fmod(m_time * 20.0 * m_intensity, gridSize);
    
    QColor gridColor = m_baseColor;
    gridColor.setAlpha(100);
    painter->setPen(gridColor);
    
    for (int y = -gridSize; y < rect.height() + gridSize; y += gridSize) {
        painter->drawLine(0, y + offset, rect.width(), y + offset);
    }
    
    for (int x = -gridSize; x < rect.width() + gridSize; x += gridSize) {
        // Skewed grid effect
        painter->drawLine(x + offset, 0, x + offset - 100, rect.height());
    }
}

void OmniShaderEffect::renderLiquidGlass(QPainter* painter, const QRectF& rect) {
    QLinearGradient grad(0, 0, rect.width(), rect.height());
    QColor c = m_baseColor;
    c.setAlpha(static_cast<int>(100 * m_intensity));
    grad.setColorAt(0, c);
    
    c.setAlpha(static_cast<int>(20 * m_intensity));
    grad.setColorAt(0.5, c);
    
    c.setAlpha(static_cast<int>(60 * m_intensity));
    grad.setColorAt(1, c);

    QPainterPath path;
    path.addRoundedRect(rect, 10, 10);
    painter->fillPath(path, grad);
}

void OmniShaderEffect::paint(QPainter *painter) {
    painter->setRenderHint(QPainter::Antialiasing);
    
    if (m_effectType == "plasma") renderPlasmaGlow(painter, boundingRect());
    else if (m_effectType == "grid") renderCyberGrid(painter, boundingRect());
    else if (m_effectType == "glass") renderLiquidGlass(painter, boundingRect());
}
