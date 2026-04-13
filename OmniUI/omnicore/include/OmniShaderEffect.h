#ifndef OMNISHADEREFFECT_H
#define OMNISHADEREFFECT_H

#include <QQuickPaintedItem>
#include <QString>
#include <QColor>
#include <BOBUIimer>

// Simulates complex fragment shaders on the CPU/QPainter level natively
// to provide cross-platform compatibility without Vulkan/RHI dependencies.
class OmniShaderEffect : public QQuickPaintedItem {
    Q_OBJECT
    Q_PROPERTY(QString effectType READ effectType WRITE setEffectType NOTIFY effectTypeChanged)
    Q_PROPERTY(qreal intensity READ intensity WRITE setIntensity NOTIFY intensityChanged)
    Q_PROPERTY(QColor baseColor READ baseColor WRITE setBaseColor NOTIFY baseColorChanged)

public:
    explicit OmniShaderEffect(QQuickItem *parent = nullptr);
    virtual ~OmniShaderEffect();

    QString effectType() const;
    void setEffectType(const QString& type);

    qreal intensity() const;
    void setIntensity(qreal i);

    QColor baseColor() const;
    void setBaseColor(const QColor& c);

    void paint(QPainter *painter) override;

signals:
    void effectTypeChanged();
    void intensityChanged();
    void baseColorChanged();

private slots:
    void advanceTime();

private:
    void renderPlasmaGlow(QPainter* painter, const QRectF& rect);
    void renderCyberGrid(QPainter* painter, const QRectF& rect);
    void renderLiquidGlass(QPainter* painter, const QRectF& rect);

    QString m_effectType;
    qreal m_intensity;
    QColor m_baseColor;
    
    double m_time;
    BOBUIimer m_animTimer;
};

#endif // OMNISHADEREFFECT_H
