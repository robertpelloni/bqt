#ifndef OMNINEURALENGINE_H
#define OMNINEURALENGINE_H

#include <QObject>
#include <QString>
#include <QJsonObject>
#include <QJsonArray>
#include <QTimer>

class QQuickItem;

class OmniNeuralEngine : public QObject {
    Q_OBJECT
    Q_PROPERTY(bool isStreaming READ isStreaming WRITE setIsStreaming NOTIFY isStreamingChanged)
    Q_PROPERTY(int streamFramerate READ streamFramerate WRITE setStreamFramerate NOTIFY streamFramerateChanged)

public:
    static OmniNeuralEngine* instance();

    bool isStreaming() const;
    void setIsStreaming(bool streaming);

    int streamFramerate() const;
    void setStreamFramerate(int fps);

    // Binds the root QML Window so the engine can recurse the visual DOM
    Q_INVOKABLE void setRootItem(QQuickItem* root);

    // Forces an immediate full DOM snapshot
    Q_INVOKABLE QString captureVisualDOM() const;

    // AI Action Execution (LLM to OS Hook)
    // Allows an LLM to command the OS (e.g., action="click", targetId="OmniButton_1")
    Q_INVOKABLE bool executeAction(const QString& targetObjectName, const QString& action, const QString& payload = "");

signals:
    void isStreamingChanged();
    void streamFramerateChanged();
    
    // Emits the JSON DOM payload dynamically for OmniIPC or WebSockets to broadcast to local LLMs
    void domSnapshotGenerated(const QString& jsonPayload);

private slots:
    void processStreamFrame();

private:
    explicit OmniNeuralEngine(QObject *parent = nullptr);
    ~OmniNeuralEngine() override;

    QJsonObject serializeNode(QQuickItem* item) const;

    QQuickItem* m_rootItem;
    bool m_isStreaming;
    int m_streamFramerate;
    QTimer m_streamTimer;
};

#endif // OMNINEURALENGINE_H
