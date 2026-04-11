#ifndef OMNIFILESYSTEM_H
#define OMNIFILESYSTEM_H

#include <QObject>
#include <QString>

class OmniFileSystem : public QObject {
    Q_OBJECT

public:
    static OmniFileSystem* instance();

    // Asynchronously reads a file from disk. Will emit fileRead on completion.
    Q_INVOKABLE void readFile(const QString& absolutePath);
    
    // Asynchronously writes text content to disk. Will emit fileWritten on completion.
    Q_INVOKABLE void writeFile(const QString& absolutePath, const QString& content);

    // Synchronous checks (safe for UI thread as they only query metadata)
    Q_INVOKABLE bool fileExists(const QString& absolutePath) const;
    Q_INVOKABLE bool directoryExists(const QString& absolutePath) const;
    Q_INVOKABLE QString getAppLocalDataLocation() const;

signals:
    void fileRead(const QString& absolutePath, const QString& content, bool success, const QString& errorMsg);
    void fileWritten(const QString& absolutePath, bool success, const QString& errorMsg);

private:
    explicit OmniFileSystem(QObject *parent = nullptr);
    ~OmniFileSystem() override;
};

#endif // OMNIFILESYSTEM_H
