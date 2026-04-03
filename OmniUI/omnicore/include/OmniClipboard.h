#ifndef OMNICLIPBOARD_H
#define OMNICLIPBOARD_H

#include <QObject>
#include <QString>
#include <QClipboard>
#include <QGuiApplication>

// Distributed OS Clipboard. Syncs local copy/paste events across the P2P Mesh.
class OmniClipboard : public QObject {
    Q_OBJECT
    Q_PROPERTY(QString text READ text WRITE setText NOTIFY textChanged)

public:
    static OmniClipboard* instance();

    QString text() const;
    
    // Sets text locally and broadcasts to the entire OS Mesh
    Q_INVOKABLE void setText(const QString& text);

    // Internal hook to receive text from remote peers
    void receiveRemoteText(const QString& text);

signals:
    void textChanged(const QString& text);
    void remoteUpdateReceived(const QString& text);

private:
    explicit OmniClipboard(QObject *parent = nullptr);
    ~OmniClipboard() override;

    QString m_text;
};

#endif // OMNICIPBOARD_H
