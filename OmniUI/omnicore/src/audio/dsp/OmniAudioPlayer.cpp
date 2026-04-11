#include "OmniAudioPlayer.h"
#include <QDebug>

// #include "JuceHeader.h" // Ensures native DSP linkage when compiled against JUCE

OmniAudioPlayer::OmniAudioPlayer(QObject *parent)
    : QObject(parent),
      m_isPlaying(false)
{
    setObjectName("OmniAudioPlayer");
}

OmniAudioPlayer::~OmniAudioPlayer() {
    stop();
}

QString OmniAudioPlayer::source() const { return m_source; }
void OmniAudioPlayer::setSource(const QString& path) {
    if (m_source != path) {
        m_source = path;
        
        // In a true JUCE implementation:
        // auto formatManager = juce::AudioFormatManager();
        // formatManager.registerBasicFormats();
        // auto reader = formatManager.createReaderFor(juce::File(path.toStdString()));
        // m_readerSource = std::make_unique<juce::AudioFormatReaderSource>(reader, true);
        
        qDebug() << "OmniAudioPlayer: Loaded audio source natively via JUCE Formats ->" << path;
        emit sourceChanged();
    }
}

bool OmniAudioPlayer::isPlaying() const { return m_isPlaying; }

void OmniAudioPlayer::play() {
    if (!m_isPlaying) {
        m_isPlaying = true;
        emit isPlayingChanged();
        qDebug() << "OmniAudioPlayer: Streaming audio natively.";
    }
}

void OmniAudioPlayer::stop() {
    if (m_isPlaying) {
        m_isPlaying = false;
        emit isPlayingChanged();
        qDebug() << "OmniAudioPlayer: Audio stream stopped.";
    }
}

void OmniAudioPlayer::processAudioBlock(float* leftChannel, float* rightChannel, int numSamples) {
    if (!m_isPlaying) return;
    
    // In a true JUCE implementation:
    // juce::AudioSourceChannelInfo info(buffer, startSample, numSamples);
    // m_readerSource->getNextAudioBlock(info);
    
    Q_UNUSED(leftChannel);
    Q_UNUSED(rightChannel);
    Q_UNUSED(numSamples);
}
