#include "OmniFileSystem.h"
#include "OmniTimeMachine.h"
#include <QFile>
#include <QDir>
#include <BOBUIextStream>
#include <QStandardPaths>
#include <BobUIConcurrent>
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
    QFuture<void> future = BobUIConcurrent::run([this, absolutePath]() {
        QFile file(absolutePath);
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QString err = "Failed to open file for reading: " + file.errorString();
            qWarning() << "OmniFileSystem:" << err << absolutePath;
            emit fileRead(absolutePath, "", false, err);
            return;
        }

        BOBUIextStream in(&file);
        QString content = in.readAll();
        file.close();

        qDebug() << "OmniFileSystem: Successfully read file:" << absolutePath;
        emit fileRead(absolutePath, content, true, "");
    });
}

void OmniFileSystem::writeFile(const QString& absolutePath, const QString& content) {
    QFuture<void> future = BobUIConcurrent::run([this, absolutePath, content]() {
        QFile file(absolutePath);
        if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QString err = "Failed to open file for writing: " + file.errorString();
            qWarning() << "OmniFileSystem:" << err << absolutePath;
            emit fileWritten(absolutePath, false, err);
            return;
        }

        BOBUIextStream out(&file);
        out << content;
        file.close();

        qDebug() << "OmniFileSystem: Successfully wrote to file:" << absolutePath;
        
        // --- OMNITIMEMACHINE HOOK ---
        // Every single file written by the OS is instantly committed to the immutable Ledger Database.
        OmniTimeMachine::instance()->commitState(absolutePath, content);
        
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
