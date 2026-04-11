#ifndef OMNIMASTERCLOCK_H
#define OMNIMASTERCLOCK_H

#include <QObject>
#include <QTimer>
#include <atomic>

class OmniMasterClock : public QObject {
    Q_OBJECT
    Q_PROPERTY(int bpm READ bpm WRITE setBpm NOTIFY bpmChanged)
    Q_PROPERTY(bool isRunning READ isRunning NOTIFY isRunningChanged)

public:
    static OmniMasterClock* instance();

    int bpm() const;
    void setBpm(int b);

    bool isRunning() const;

    Q_INVOKABLE void start();
    Q_INVOKABLE void stop();
    Q_INVOKABLE void reset();

    // In a true JUCE implementation, the AudioProcessor calls this from the real-time thread.
    // The UI thread then safely polls the atomic counters.
    void advanceDspTime(double deltaSeconds);

signals:
    void bpmChanged();
    void isRunningChanged();
    
    // Fired on the Qt Main Thread, synced as closely as possible to the DSP time
    void beat(int beatNumber);
    void sixteenthStep(int stepNumber);

private slots:
    void onUiSyncTick();

private:
    explicit OmniMasterClock(QObject *parent = nullptr);
    ~OmniMasterClock() override;

    int m_bpm;
    bool m_isRunning;

    std::atomic<double> m_dspTime;
    double m_lastSyncTime;
    
    int m_currentBeat;
    int m_currentStep;

    QTimer m_uiSyncTimer; // Simulates the VSync / DSP polling loop
};

#endif // OMNIMASTERCLOCK_H
