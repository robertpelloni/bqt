#ifndef OMNIAUDIOPLAYER_H
#define OMNIAUDIOPLAYER_H

#include <QObject>
#include <QString>

class OmniAudioPlayer : public QObject {
    Q_OBJECT
    Q_PROPERTY(QString source READ source WRITE setSource NOTIFY sourceChanged)
    Q_PROPERTY(bool isPlaying READ isPlaying NOTIFY isPlayingChanged)

public:
    explicit OmniAudioPlayer(QObject *parent = nullptr);
    virtual ~OmniAudioPlayer();

    QString source() const;
    void setSource(const QString& path);

    bool isPlaying() const;

    Q_INVOKABLE void play();
    Q_INVOKABLE void stop();

    // The actual DSP loop called by the OmniMasterClock or JUCE Thread
    void processAudioBlock(float* leftChannel, float* rightChannel, int numSamples);

signals:
    void sourceChanged();
    void isPlayingChanged();

private:
    QString m_source;
    bool m_isPlaying;
    // In a real implementation: std::unique_ptr<juce::AudioFormatReaderSource> m_readerSource;
};

#endif // OMNIAUDIOPLAYER_H
