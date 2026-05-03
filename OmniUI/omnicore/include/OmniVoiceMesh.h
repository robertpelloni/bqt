#ifndef OMNIVOICEMESH_H
#define OMNIVOICEMESH_H

#include <QObject>
#include <QString>

class OmniVoiceMesh : public QObject {
    Q_OBJECT
public:
    static OmniVoiceMesh* instance();

    // Activates the P2P voice link
    Q_INVOKABLE void enableVoiceChat(bool enabled);

private slots:
    void onLocalTranscript(const QString& text);
    void onRemotePayload(const QString& message);

private:
    explicit OmniVoiceMesh(QObject *parent = nullptr);
    ~OmniVoiceMesh() override;

    bool m_isEnabled;
};

#endif // OMNIVOICEMESH_H
