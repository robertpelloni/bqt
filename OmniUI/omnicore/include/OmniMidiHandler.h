#ifndef OMNIMIDIHANDLER_H
#define OMNIMIDIHANDLER_H

#include <QObject>
#include <QString>

class OmniMidiHandler : public QObject {
    Q_OBJECT
    Q_PROPERTY(QString currentDevice READ currentDevice WRITE setCurrentDevice NOTIFY currentDeviceChanged)

public:
    explicit OmniMidiHandler(QObject *parent = nullptr);
    virtual ~OmniMidiHandler();

    QString currentDevice() const;
    void setCurrentDevice(const QString& device);

    Q_INVOKABLE void sendNoteOn(int channel, int note, int velocity);
    Q_INVOKABLE void sendNoteOff(int channel, int note);

signals:
    void currentDeviceChanged();
    void noteReceived(int channel, int note, int velocity); // From hardware to UI

private:
    QString m_currentDevice;
};

#endif // OMNIMIDIHANDLER_H
