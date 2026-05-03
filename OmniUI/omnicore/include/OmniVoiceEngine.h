#ifndef OMNIVOICEENGINE_H
#define OMNIVOICEENGINE_H

#include <QObject>
#include <QBuffer>
#include <QByteArray>
#include <memory>

class QAudioSource;
class QAudioFormat;

// The native OS microphone capture and Voice-to-Action routing engine.
class OmniVoiceEngine : public QObject {
    Q_OBJECT
    Q_PROPERTY(bool isListening READ isListening NOTIFY isListeningChanged)

public:
    static OmniVoiceEngine* instance();

    bool isListening() const;

    // Activates the system microphone and begins buffering PCM audio natively
    Q_INVOKABLE bool startListening();
    
    // Stops the microphone, packages the WAV, and dispatches it to the local LLM/STT bridge
    Q_INVOKABLE void stopListening();

signals:
    void isListeningChanged();
    void audioLevelChanged(qreal level); // For UI visualizers
    void transcriptReceived(const QString& text);
    void errorOccurred(const QString& errorMsg);

private slots:
    void handleAudioData();

private:
    explicit OmniVoiceEngine(QObject *parent = nullptr);
    ~OmniVoiceEngine() override;

    QByteArray generateWavHeader(const QByteArray& pcmData, int sampleRate, int channels, int bitsPerSample);

    bool m_isListening;
    QAudioSource* m_audioSource;
    QBuffer m_audioBuffer;
    QByteArray m_pcmData;
};

#endif // OMNIVOICEENGINE_H
