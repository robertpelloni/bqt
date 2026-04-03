#ifndef OMNISYNTHESIZER_H
#define OMNISYNTHESIZER_H

#include <QObject>

class OmniSynthesizer : public QObject {
    Q_OBJECT
    Q_PROPERTY(int polyphony READ polyphony WRITE setPolyphony NOTIFY polyphonyChanged)

public:
    explicit OmniSynthesizer(QObject *parent = nullptr);
    virtual ~OmniSynthesizer();

    int polyphony() const;
    void setPolyphony(int voices);

    Q_INVOKABLE void noteOn(int channel, int note, int velocity);
    Q_INVOKABLE void noteOff(int channel, int note);

    void processAudioBlock(float* leftChannel, float* rightChannel, int numSamples);

signals:
    void polyphonyChanged();

private:
    int m_polyphony;
    // In a real implementation: juce::Synthesiser m_synth;
};

#endif // OMNISYNTHESIZER_H
