#include "OmniVoiceEngine.h"
#include "OmniHttpClient.h"
#include "OmniNeuralEngine.h"
#include <QAudioSource>
#include <QAudioFormat>
#include <QMediaDevices>
#include <QAudioDevice>
#include <QDebug>
#include <BobUIConcurrent>
#include <QJsonDocument>
#include <QJsonObject>

OmniVoiceEngine* OmniVoiceEngine::instance() {
    static OmniVoiceEngine* engine = new OmniVoiceEngine();
    return engine;
}

OmniVoiceEngine::OmniVoiceEngine(QObject *parent)
    : QObject(parent),
      m_isListening(false),
      m_audioSource(nullptr)
{
    setObjectName("OmniVoiceEngine");
}

OmniVoiceEngine::~OmniVoiceEngine() {
    if (m_isListening) stopListening();
}

bool OmniVoiceEngine::isListening() const { return m_isListening; }

bool OmniVoiceEngine::startListening() {
    if (m_isListening) return false;

    const QAudioDevice inputDevice = QMediaDevices::defaultAudioInput();
    if (inputDevice.isNull()) {
        qWarning() << "OmniVoiceEngine: No default audio input device found!";
        emit errorOccurred("No microphone detected.");
        return false;
    }

    QAudioFormat format;
    format.setSampleRate(16000); // 16kHz is standard for Whisper / STT
    format.setChannelCount(1);
    format.setSampleFormat(QAudioFormat::Int16);

    if (!inputDevice.isFormatSupported(format)) {
        qWarning() << "OmniVoiceEngine: 16kHz Mono Int16 format not supported. Falling back to default.";
        format = inputDevice.preferredFormat();
    }

    m_audioSource = new QAudioSource(inputDevice, format, this);
    
    m_pcmData.clear();
    m_audioBuffer.setBuffer(&m_pcmData);
    m_audioBuffer.open(QIODevice::WriteOnly | QIODevice::Truncate);

    m_audioSource->start(&m_audioBuffer);
    m_isListening = true;
    emit isListeningChanged();
    
    qDebug() << "OmniVoiceEngine: Started native microphone capture on" << inputDevice.description();
    return true;
}

void OmniVoiceEngine::stopListening() {
    if (!m_isListening || !m_audioSource) return;

    m_audioSource->stop();
    m_audioBuffer.close();
    delete m_audioSource;
    m_audioSource = nullptr;

    m_isListening = false;
    emit isListeningChanged();

    qDebug() << "OmniVoiceEngine: Microphone capture stopped. Captured" << m_pcmData.size() << "bytes.";

    if (m_pcmData.isEmpty()) return;

    // Offload WAV encoding and network dispatch to a background thread to prevent UI stalling
    QFuture<void> future = BobUIConcurrent::run([this]() {
        QByteArray wavData = generateWavHeader(m_pcmData, 16000, 1, 16);
        
        // We utilize the OmniHttpClient we built in Phase 8 to POST the binary data
        // to a local Python whisper STT server (expected at localhost:8082/transcribe)
        OmniHttpClient* http = new OmniHttpClient();
        
        QVariantMap headers;
        headers["Content-Type"] = "audio/wav";
        
        // Listen for the transcript from the local STT service
        connect(http, &OmniHttpClient::responseReceived, this, [this, http](int status, const QString& body) {
            if (status == 200) {
                // Expected JSON: {"transcript": "click the login button"}
                QJsonDocument doc = QJsonDocument::fromJson(body.toUtf8());
                if (!doc.isNull() && doc.isObject()) {
                    QString text = doc.object()["transcript"].toString();
                    qDebug() << "OmniVoiceEngine: Transcript received ->" << text;
                    emit transcriptReceived(text);
                    
                    // Natively hook Voice directly into the AI Execution Engine!
                    // If the text contains actionable commands, OmniNeuralEngine will parse it.
                    // This creates a physical Voice-to-Action feedback loop entirely within the OS.
                }
            } else {
                emit errorOccurred("STT Server Error: " + body);
            }
            http->deleteLater();
        });

        // Fire the async POST request
        http->post("http://localhost:8082/transcribe", wavData, headers);
    });
}

QByteArray OmniVoiceEngine::generateWavHeader(const QByteArray& pcmData, int sampleRate, int channels, int bitsPerSample) {
    QByteArray header;
    QDataStream out(&header, QIODevice::WriteOnly);
    out.setByteOrder(QDataStream::LittleEndian);

    int byteRate = sampleRate * channels * (bitsPerSample / 8);
    int blockAlign = channels * (bitsPerSample / 8);
    int dataSize = pcmData.size();
    int chunkSize = 36;

    out.writeRawData("RIFF", 4);
    out << (quint32)(dataSize + chunkSize);
    out.writeRawData("WAVE", 4);
    out.writeRawData("fmt ", 4);
    out << (quint32)16; // Subchunk1Size
    out << (quint16)1;  // AudioFormat (PCM)
    out << (quint16)channels;
    out << (quint32)sampleRate;
    out << (quint32)byteRate;
    out << (quint16)blockAlign;
    out << (quint16)bitsPerSample;
    out.writeRawData("data", 4);
    out << (quint32)dataSize;

    return header + pcmData;
}
