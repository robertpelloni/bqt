#ifndef OMNIMASTERCLOCK_H
#define OMNIMASTERCLOCK_H

#include <QObject>
#include <QTimer>

/**
 * @class OmniMasterClock
 * @brief Singleton that unifies and drives external framework event loops.
 *
 * It attaches to the main Qt QEventLoop via a high-frequency QTimer (or QAbstractEventDispatcher hook)
 * and pumps the event queues for JUCE (MessageManager) and Ultimate++ (Ctrl::ProcessEvents).
 * This eliminates the need for multithreading the UI logic and prevents tearing/deadlocks
 * between the disparate frameworks.
 */
class OmniMasterClock : public QObject {
    Q_OBJECT
public:
    static OmniMasterClock* instance();

    // Starts the unified event pump
    void start(int refreshRateHz = 60);
    void stop();

signals:
    void tick(); // Emitted on every loop iteration

private slots:
    void onTick();

private:
    OmniMasterClock(QObject* parent = nullptr);
    ~OmniMasterClock();

    QTimer m_timer;
    static OmniMasterClock* s_instance;
};

#endif // OMNIMASTERCLOCK_H
