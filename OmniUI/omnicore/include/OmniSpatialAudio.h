#ifndef OMNISPATIALAUDIO_H
#define OMNISPATIALAUDIO_H

#include <QObject>
#include <QString>
#include <QPointF>

// Native Spatial Audio Kernel. 
// Pans UI sounds based on the DeviceId's physical screen coordinates.
class OmniSpatialAudio : public QObject {
    Q_OBJECT
public:
    static OmniSpatialAudio* instance();

    // Plays a sound spatially panned to the location of a specific device
    Q_INVOKABLE void playSpatialClick(const QString& deviceId, qreal volume = 1.0);
    
    Q_INVOKABLE void playSpatialNotification(const QString& deviceId, const QString& soundPath);

private:
    explicit OmniSpatialAudio(QObject *parent = nullptr);
    ~OmniSpatialAudio() override;

    // Calculates the Stereo Pan (-1.0 to 1.0) based on screen width
    float calculatePan(const QPointF& screenPos);
};

#endif // OMNISPATIALAUDIO_H
