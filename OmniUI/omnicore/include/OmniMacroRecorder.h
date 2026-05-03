#ifndef OMNIMACRORECORDER_H
#define OMNIMACRORECORDER_H

#include <QObject>
#include <QStringList>
#include <QString>

class OmniMacroRecorder : public QObject {
    Q_OBJECT
    Q_PROPERTY(bool isRecording READ isRecording WRITE setIsRecording NOTIFY isRecordingChanged)
    Q_PROPERTY(bool isPlaying READ isPlaying WRITE setIsPlaying NOTIFY isPlayingChanged)

public:
    static OmniMacroRecorder* instance();

    bool isRecording() const;
    void setIsRecording(bool recording);

    bool isPlaying() const;
    void setIsPlaying(bool playing);

    Q_INVOKABLE void startRecording();
    Q_INVOKABLE void stopRecording();
    
    // Compiles the recorded OS events into a pure OmniScript (Lisp) payload
    Q_INVOKABLE QString getGeneratedScript() const;

    // Feeds the generated script directly into the OmniVM Hypervisor for autonomous playback
    Q_INVOKABLE void playMacro();

    // Clears the buffer
    Q_INVOKABLE void clearMacro();

    // Hook called by the OS Event Loop (OmniInputManager) to log raw human actions
    void recordClick(const QString& targetObjectName);
    void recordKey(const QString& targetObjectName, const QString& keyText);

signals:
    void isRecordingChanged();
    void isPlayingChanged();
    void macroGenerated(const QString& scriptPayload);

private:
    explicit OmniMacroRecorder(QObject *parent = nullptr);
    ~OmniMacroRecorder() override;

    bool m_isRecording;
    bool m_isPlaying;
    QStringList m_recordedCommands;
};

#endif // OMNIMACRORECORDER_H
