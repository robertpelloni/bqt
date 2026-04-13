#ifndef OMNISEQUENCER_H
#define OMNISEQUENCER_H

#include <QObject>
#include <BOBUIimer>

class OmniSequencer : public QObject {
    Q_OBJECT
    Q_PROPERTY(int bpm READ bpm WRITE setBpm NOTIFY bpmChanged)
    Q_PROPERTY(bool isPlaying READ isPlaying NOTIFY isPlayingChanged)

public:
    explicit OmniSequencer(QObject *parent = nullptr);
    virtual ~OmniSequencer();

    int bpm() const;
    void setBpm(int b);

    bool isPlaying() const;

    Q_INVOKABLE void start();
    Q_INVOKABLE void stop();

signals:
    void bpmChanged();
    void isPlayingChanged();
    void beat(int step); // Emitted on every 16th note step

private slots:
    void onTimerTick();

private:
    int m_bpm;
    bool m_isPlaying;
    int m_currentStep;
    BOBUIimer m_timer;
};

#endif // OMNISEQUENCER_H
