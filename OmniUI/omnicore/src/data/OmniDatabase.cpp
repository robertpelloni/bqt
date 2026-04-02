#include "OmniDatabase.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRecord>
#include <QDebug>

OmniDatabase::OmniDatabase(QObject *parent)
    : QObject(parent)
{
    setObjectName("OmniDatabase");
}

OmniDatabase::~OmniDatabase() {
    disconnect();
}

bool OmniDatabase::isOpen() const {
    return m_db.isOpen();
}

QString OmniDatabase::lastError() const {
    return m_lastError;
}

void OmniDatabase::setLastError(const QString& error) {
    m_lastError = error;
    emit lastErrorChanged();
    if (!error.isEmpty()) {
        qWarning() << "OmniDatabase Error:" << error;
    }
}

bool OmniDatabase::connect(const QString& dbPath) {
    if (m_db.isOpen()) {
        disconnect();
    }

    m_db = QSqlDatabase::addDatabase("QSQLITE", "OmniUI_Connection");
    m_db.setDatabaseName(dbPath);

    if (!m_db.open()) {
        setLastError(m_db.lastError().text());
        emit isOpenChanged();
        return false;
    }

    setLastError("");
    emit isOpenChanged();
    qDebug() << "OmniDatabase: Successfully connected to" << dbPath;
    return true;
}

void OmniDatabase::disconnect() {
    if (m_db.isOpen()) {
        m_db.close();
        emit isOpenChanged();
        qDebug() << "OmniDatabase: Disconnected.";
    }
    QSqlDatabase::removeDatabase("OmniUI_Connection");
}

QVariantList OmniDatabase::executeQuery(const QString& queryStr, const QVariantMap& boundValues) {
    QVariantList results;

    if (!m_db.isOpen()) {
        setLastError("Database is not open.");
        return results;
    }

    QSqlQuery query(m_db);
    query.prepare(queryStr);

    // Bind dynamic parameters if provided
    for (auto it = boundValues.constBegin(); it != boundValues.constEnd(); ++it) {
        query.bindValue(":" + it.key(), it.value());
    }

    if (!query.exec()) {
        setLastError(query.lastError().text());
        return results;
    }

    setLastError(""); // Clear error on success

    // If it's a SELECT query, package the results into a QVariantList of QVariantMaps
    if (query.isSelect()) {
        QSqlRecord record = query.record();
        int columnCount = record.count();

        while (query.next()) {
            QVariantMap row;
            for (int i = 0; i < columnCount; ++i) {
                row.insert(record.fieldName(i), query.value(i));
            }
            results.append(row);
        }
    }

    return results;
}
