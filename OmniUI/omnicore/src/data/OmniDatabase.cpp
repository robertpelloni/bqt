#include "OmniDatabase.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRecord>
#include <QDebug>
#include <exception>

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
    if (m_db.isOpen()) disconnect();

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

    try {
        QSqlQuery query(m_db);
        query.prepare(queryStr);

        // Bind dynamic parameters securely
        for (auto it = boundValues.constBegin(); it != boundValues.constEnd(); ++it) {
            query.bindValue(":" + it.key(), it.value());
        }

        if (!query.exec()) {
            setLastError(query.lastError().text());
            return results;
        }

        setLastError("");

        if (query.isSelect()) {
            QSqlRecord record = query.record();
            int columnCount = record.count();

            // Bounds check columns
            if (columnCount > 0) {
                while (query.next()) {
                    QVariantMap row;
                    for (int i = 0; i < columnCount; ++i) {
                        row.insert(record.fieldName(i), query.value(i));
                    }
                    results.append(row);
                }
            }
        }
    } catch (const std::exception& e) {
        qCritical() << "OmniDatabase: Caught fatal C++ exception during execution:" << e.what();
        setLastError(QString("Fatal Exception: %1").arg(e.what()));
    } catch (...) {
        qCritical() << "OmniDatabase: Caught unknown fatal C++ exception.";
        setLastError("Unknown Fatal Exception");
    }

    return results;
}

bool OmniDatabase::executeTransaction(const QStringList& queries) {
    if (!m_db.isOpen()) {
        setLastError("Database is not open.");
        return false;
    }

    if (queries.isEmpty()) return true;

    try {
        m_db.transaction();
        
        QSqlQuery query(m_db);
        for (const QString& qStr : queries) {
            if (!query.exec(qStr)) {
                setLastError(QString("Transaction failed on query: %1 | Error: %2").arg(qStr, query.lastError().text()));
                m_db.rollback();
                return false;
            }
        }
        
        if (!m_db.commit()) {
            setLastError("Transaction commit failed: " + m_db.lastError().text());
            m_db.rollback();
            return false;
        }
        
        setLastError("");
        qDebug() << "OmniDatabase: Successfully executed transaction of" << queries.size() << "queries.";
        return true;
        
    } catch (...) {
        m_db.rollback();
        setLastError("Fatal Exception during transaction.");
        return false;
    }
}
