#ifndef OMNIGAIN_H
#define OMNIGAIN_H

#include <QObject>

class OmniGain : public QObject {
    Q_OBJECT
    Q_PROPERTY(qreal gain READ gain WRITE setGain NOTIFY gainChanged)

public:
    explicit OmniGain(QObject *parent = nullptr);
    virtual ~OmniGain();

    qreal gain() const;
    void setGain(qreal g);

    void processAudioBlock(float* leftChannel, float* rightChannel, int numSamples);

signals:
    void gainChanged();

private:
    qreal m_gain;
};

#endif // OMNIGAIN_H
