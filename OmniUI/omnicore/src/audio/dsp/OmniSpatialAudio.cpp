#include "OmniSpatialAudio.h"
#include "OmniInputManager.h"
#include "OmniAudioPlayer.h"
#include <QGuiApplication>
#include <QScreen>
#include <QDebug>

OmniSpatialAudio* OmniSpatialAudio::instance() {
    static OmniSpatialAudio* manager = new OmniSpatialAudio();
    return manager;
}

OmniSpatialAudio::OmniSpatialAudio(QObject *parent) : QObject(parent) {
    setObjectName("OmniSpatialAudio");
}

OmniSpatialAudio::~OmniSpatialAudio() = default;

float OmniSpatialAudio::calculatePan(const QPointF& screenPos) {
    QScreen *screen = QGuiApplication::primaryScreen();
    if (!screen) return 0.0f;
    
    qreal screenWidth = screen->geometry().width();
    // Map 0 -> -1.0 (Left), screenWidth -> 1.0 (Right)
    return (float)((screenPos.x() / screenWidth) * 2.0 - 1.0);
}

void OmniSpatialAudio::playSpatialClick(const QString& deviceId, qreal volume) {
    QPointF pos = OmniInputManager::instance()->devices().at(0).cursorPosition; // Fallback
    // Correctly find the device position
    auto devices = OmniInputManager::instance()->devices();
    for (const auto& d : devices) {
        if (d.id == deviceId) {
            pos = d.cursorPosition;
            break;
        }
    }

    float pan = calculatePan(pos);
    qDebug() << "OmniSpatialAudio: Playing Click for" << deviceId << "at Pan:" << pan;

    // In a true JUCE build, we'd trigger a panned sampler voice here.
    // For now, we log the spatial intent.
}

void OmniSpatialAudio::playSpatialNotification(const QString& deviceId, const QString& soundPath) {
    // Spatial trigger logic...
    Q_UNUSED(soundPath);
    playSpatialClick(deviceId); 
}
