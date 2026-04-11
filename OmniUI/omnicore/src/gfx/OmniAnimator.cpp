#include "OmniAnimator.h"
#include <QVariant>
#include <QDebug>
#include <cmath>

OmniAnimator* OmniAnimator::instance() {
    static OmniAnimator* animator = new OmniAnimator();
    return animator;
}

OmniAnimator::OmniAnimator(QObject *parent)
    : QObject(parent)
{
    setObjectName("OmniAnimator");
    m_clock.start();
    connect(&m_frameTimer, &QTimer::timeout, this, &OmniAnimator::processFrame);
}

OmniAnimator::~OmniAnimator() = default;

void OmniAnimator::animateTo(QObject* target, const QString& propertyName, qreal targetValue, int durationMs) {
    if (!target || propertyName.isEmpty()) return;

    QByteArray prop = propertyName.toUtf8();
    QVariant currentVar = target->property(prop);
    
    if (!currentVar.isValid() || !currentVar.canConvert<qreal>()) {
        qWarning() << "OmniAnimator: Cannot animate property" << propertyName << "on" << target->objectName();
        return;
    }

    qreal startValue = currentVar.toReal();

    // Stop any existing animation on this property
    for (int i = 0; i < m_animations.size(); ++i) {
        if (m_animations[i].target == target && m_animations[i].propertyName == prop) {
            m_animations.removeAt(i);
            break;
        }
    }

    OmniAnimation anim;
    anim.target = target;
    anim.propertyName = prop;
    anim.startValue = startValue;
    anim.endValue = targetValue;
    anim.startTimeMs = m_clock.elapsed();
    anim.durationMs = durationMs;

    m_animations.append(anim);
    
    if (!m_frameTimer.isActive()) {
        m_frameTimer.start(16); // ~60fps
    }
}

void OmniAnimator::stopAll(QObject* target) {
    if (!target) return;
    
    for (int i = m_animations.size() - 1; i >= 0; --i) {
        if (m_animations[i].target == target) {
            m_animations.removeAt(i);
        }
    }
    
    if (m_animations.isEmpty()) {
        m_frameTimer.stop();
    }
}

// Ease-Out Cubic interpolation function
static qreal easeOutCubic(qreal t) {
    t -= 1.0;
    return t * t * t + 1.0;
}

void OmniAnimator::processFrame() {
    if (m_animations.isEmpty()) {
        m_frameTimer.stop();
        return;
    }

    qint64 now = m_clock.elapsed();

    for (int i = m_animations.size() - 1; i >= 0; --i) {
        OmniAnimation& anim = m_animations[i];

        // If target was deleted dynamically by the OS, clean up safely via QPointer check
        if (!anim.target) {
            m_animations.removeAt(i);
            continue;
        }

        qint64 elapsed = now - anim.startTimeMs;
        qreal progress = 1.0;

        if (anim.durationMs > 0) {
            progress = static_cast<qreal>(elapsed) / anim.durationMs;
        }

        if (progress >= 1.0) {
            // Finish
            anim.target->setProperty(anim.propertyName, anim.endValue);
            emit animationFinished(anim.target, QString::fromUtf8(anim.propertyName));
            m_animations.removeAt(i);
        } else {
            // Interpolate using ease-out
            qreal easedProgress = easeOutCubic(progress);
            qreal val = anim.startValue + (anim.endValue - anim.startValue) * easedProgress;
            anim.target->setProperty(anim.propertyName, val);
        }
    }
}
