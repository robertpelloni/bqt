#ifndef OMNIPARTICLESYSTEM_H
#define OMNIPARTICLESYSTEM_H

#include <QQuickPaintedItem>
#include <QColor>
#include <QList>
#include <QPointF>

struct OmniParticle {
    QPointF position;
    QPointF velocity;
    qreal life;
    qreal maxLife;
    QColor color;
};

class OmniParticleSystem : public QQuickPaintedItem {
    Q_OBJECT
    Q_PROPERTY(bool isRunning READ isRunning WRITE setIsRunning NOTIFY isRunningChanged)
    Q_PROPERTY(int emissionRate READ emissionRate WRITE setEmissionRate NOTIFY emissionRateChanged)
    Q_PROPERTY(qreal gravity READ gravity WRITE setGravity NOTIFY gravityChanged)
    Q_PROPERTY(QColor particleColor READ particleColor WRITE setParticleColor NOTIFY particleColorChanged)

public:
    explicit OmniParticleSystem(QQuickItem *parent = nullptr);
    virtual ~OmniParticleSystem();

    bool isRunning() const;
    void setIsRunning(bool running);

    int emissionRate() const;
    void setEmissionRate(int rate);

    qreal gravity() const;
    void setGravity(qreal g);

    QColor particleColor() const;
    void setParticleColor(const QColor& color);

    Q_INVOKABLE void burst(int count, const QPointF& pos);

    void paint(QPainter *painter) override;

signals:
    void isRunningChanged();
    void emissionRateChanged();
    void gravityChanged();
    void particleColorChanged();

private slots:
    void onClockTick(int stepNumber);

private:
    void updatePhysics(qreal dt);
    void spawnParticle(const QPointF& pos);

    bool m_isRunning;
    int m_emissionRate;
    qreal m_gravity;
    QColor m_particleColor;

    QList<OmniParticle> m_particles;
    int m_maxParticles;
};

#endif // OMNIPARTICLESYSTEM_H
