#include "OmniTimeMachine.h"
#include "OmniMeshNode.h"
#include <QDebug>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QJsonObject>

OmniTimeMachine* OmniTimeMachine::instance() {
    static OmniTimeMachine* tm = new OmniTimeMachine();
    return tm;
}

OmniTimeMachine::OmniTimeMachine(QObject *parent) : QObject(parent), m_isInitialized(false) {
    setObjectName("OmniTimeMachine");
}

OmniTimeMachine::~OmniTimeMachine() = default;

bool OmniTimeMachine::isInitialized() const { return m_isInitialized; }

bool OmniTimeMachine::initializeLedger(const QString& dbPath) {
    m_dbPath = dbPath;
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", "OmniUI_TimeMachine");
    db.setDatabaseName(m_dbPath);

    if (!db.open()) return false;

    QSqlQuery query(db);
    QString createTable = "CREATE TABLE IF NOT EXISTS _omni_timemachine ("
                          "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                          "filepath TEXT NOT NULL, "
                          "content TEXT, "
                          "timestamp DATETIME DEFAULT CURRENT_TIMESTAMP)";
                          
    if (!query.exec(createTable)) { db.close(); return false; }

    m_isInitialized = true;
    emit initializationChanged();
    return true;
}

void OmniTimeMachine::commitState(const QString& absolutePath, const QString& content) {
    if (!m_isInitialized) return;

    QSqlDatabase db = QSqlDatabase::database("OmniUI_TimeMachine");
    if (!db.isOpen()) return;

    QSqlQuery query(db);
    query.prepare("INSERT INTO _omni_timemachine (filepath, content) VALUES (:path, :content)");
    query.bindValue(":path", absolutePath);
    query.bindValue(":content", content);

    if (query.exec()) {
        int count = getStateCount(absolutePath);
        emit stateCommitted(absolutePath, count);
        
        // --- DISTRIBUTED SYNC ---
        // Broadcast the new file state to all peers in the mesh
        QJsonObject obj;
        obj["type"] = "ledger_update";
        obj["path"] = absolutePath;
        obj["content"] = content;
        // OmniMeshNode::instance()->broadcastJson(obj); // Assuming broadcastJson is accessible
    }
}

int OmniTimeMachine::getStateCount(const QString& absolutePath) const {
    QSqlDatabase db = QSqlDatabase::database("OmniUI_TimeMachine");
    QSqlQuery query(db);
    query.prepare("SELECT COUNT(*) FROM _omni_timemachine WHERE filepath = :path");
    query.bindValue(":path", absolutePath);
    if (query.exec() && query.next()) return query.value(0).toInt();
    return 0;
}

QString OmniTimeMachine::getStateAt(const QString& absolutePath, int index) const {
    QSqlDatabase db = QSqlDatabase::database("OmniUI_TimeMachine");
    QSqlQuery query(db);
    query.prepare("SELECT content FROM _omni_timemachine WHERE filepath = :path ORDER BY id ASC LIMIT 1 OFFSET :idx");
    query.bindValue(":path", absolutePath);
    query.bindValue(":idx", index);
    if (query.exec() && query.next()) return query.value(0).toString();
    return "";
}

QVariantList OmniTimeMachine::getTimeline(const QString& absolutePath) const {
    QVariantList timeline;
    QSqlDatabase db = QSqlDatabase::database("OmniUI_TimeMachine");
    QSqlQuery query(db);
    query.prepare("SELECT timestamp FROM _omni_timemachine WHERE filepath = :path ORDER BY id ASC");
    query.bindValue(":path", absolutePath);
    while (query.exec() && query.next()) timeline.append(query.value(0).toString());
    return timeline;
}
