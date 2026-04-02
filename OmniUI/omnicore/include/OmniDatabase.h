#ifndef OMNIDATABASE_H
#define OMNIDATABASE_H

#include <QObject>
#include <QString>
#include <QVariantList>
#include <QVariantMap>
#include <QSqlDatabase>

class OmniDatabase : public QObject {
    Q_OBJECT
    Q_PROPERTY(bool isOpen READ isOpen NOTIFY isOpenChanged)
    Q_PROPERTY(QString lastError READ lastError NOTIFY lastErrorChanged)

public:
    explicit OmniDatabase(QObject *parent = nullptr);
    virtual ~OmniDatabase();

    bool isOpen() const;
    QString lastError() const;

    // Connects to a local SQLite database file
    Q_INVOKABLE bool connect(const QString& dbPath);
    
    // Closes the connection
    Q_INVOKABLE void disconnect();

    // Executes a query and returns the results as a QML-friendly array of objects
    Q_INVOKABLE QVariantList executeQuery(const QString& queryStr, const QVariantMap& boundValues = QVariantMap());

signals:
    void isOpenChanged();
    void lastErrorChanged();

private:
    void setLastError(const QString& error);

    QSqlDatabase m_db;
    QString m_lastError;
};

#endif // OMNIDATABASE_H
