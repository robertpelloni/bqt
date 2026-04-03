#include "OmniFileSystem.h"
#include <QFile>
#include <QDir>
#include <QTextStream>
#include <QStandardPaths>
#include <QtConcurrent>
#include <QDebug>

OmniFileSystem* OmniFileSystem::instance()
{
    static OmniFileSystem* fs = new OmniFileSystem();
    return fs;
}

OmniFileSystem::OmniFileSystem(QObject *parent) : QObject(parent) {
    setObjectName("OmniFileSystem");
}

OmniFileSystem::~OmniFileSystem() = default;

void OmniFileSystem::readFile(const QString& absolutePath) {
    // Execute file I/O on a background thread pool to prevent 60fps UI stutter
    QFuture<void> future = QtConcurrent::run([this, absolutePath]() {
        QFile file(absolutePath);
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QString err = "Failed to open file for reading: " + file.errorString();
            qWarning() << "OmniFileSystem:" << err << absolutePath;
            // Emit on the main thread via queued connection inherently handled by Qt cross-thread signal emissions
            emit fileRead(absolutePath, "", false, err);
            return;
        }

        QTextStream in(&file);
        QString content = in.readAll();
        file.close();

        qDebug() << "OmniFileSystem: Successfully read file:" << absolutePath;
        emit fileRead(absolutePath, content, true, "");
    });
}

void OmniFileSystem::writeFile(const QString& absolutePath, const QString& content) {
    QFuture<void> future = QtConcurrent::run([this, absolutePath, content]() {
        QFile file(absolutePath);
        if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QString err = "Failed to open file for writing: " + file.errorString();
            qWarning() << "OmniFileSystem:" << err << absolutePath;
            emit fileWritten(absolutePath, false, err);
            return;
        }

        QTextStream out(&file);
        out << content;
        file.close();

        qDebug() << "OmniFileSystem: Successfully wrote to file:" << absolutePath;
        emit fileWritten(absolutePath, true, "");
    });
}

bool OmniFileSystem::fileExists(const QString& absolutePath) const {
    return QFile::exists(absolutePath);
}

bool OmniFileSystem::directoryExists(const QString& absolutePath) const {
    QDir dir(absolutePath);
    return dir.exists();
}

QString OmniFileSystem::getAppLocalDataLocation() const {
    return QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
}
