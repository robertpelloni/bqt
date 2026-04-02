#ifndef OMNIFILTER_H
#define OMNIFILTER_H

#include <QObject>

class OmniFilter : public QObject {
    Q_OBJECT
    Q_PROPERTY(int type READ type WRITE setType NOTIFY typeChanged)
    Q_PROPERTY(qreal cutoff READ cutoff WRITE setCutoff NOTIFY cutoffChanged)
    Q_PROPERTY(qreal resonance READ resonance WRITE setResonance NOTIFY resonanceChanged)

public:
    enum FilterType {
        LowPass,
        HighPass,
        BandPass
    };
    Q_ENUM(FilterType)

    explicit OmniFilter(QObject *parent = nullptr);
    virtual ~OmniFilter();

    int type() const;
    void setType(int t);

    qreal cutoff() const;
    void setCutoff(qreal c);

    qreal resonance() const;
    void setResonance(qreal r);

    // Mock processing function. In reality, this would wrap juce::dsp::StateVariableTPTFilter
    void processAudioBlock(float* leftChannel, float* rightChannel, int numSamples);

signals:
    void typeChanged();
    void cutoffChanged();
    void resonanceChanged();

private:
    int m_type;
    qreal m_cutoff;
    qreal m_resonance;
};

#endif // OMNIFILTER_H
