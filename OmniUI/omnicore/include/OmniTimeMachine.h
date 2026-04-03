#ifndef OMNITIMEMACHINE_H
#define OMNITIMEMACHINE_H

#include <QObject>
#include <QString>
#include <QVariantList>

class OmniTimeMachine : public QObject {
    Q_OBJECT
    Q_PROPERTY(bool isInitialized READ isInitialized NOTIFY initializationChanged)

public:
    static OmniTimeMachine* instance();

    bool isInitialized() const;

    // Initializes the hidden OS-level SQLite ledger
    Q_INVOKABLE bool initializeLedger(const QString& dbPath);

    // Commits a file state to the immutable OS ledger
    Q_INVOKABLE void commitState(const QString& absolutePath, const QString& content);

    // Retrieves the number of saved states for a specific file
    Q_INVOKABLE int getStateCount(const QString& absolutePath) const;

    // Retrieves the exact content of a file at a specific historical index
    Q_INVOKABLE QString getStateAt(const QString& absolutePath, int index) const;
    
    // Retrieves a list of all historical timestamps for a file
    Q_INVOKABLE QVariantList getTimeline(const QString& absolutePath) const;

signals:
    void initializationChanged();
    void stateCommitted(const QString& absolutePath, int newTotalStates);

private:
    explicit OmniTimeMachine(QObject *parent = nullptr);
    ~OmniTimeMachine() override;

    bool m_isInitialized;
    QString m_dbPath;
};

#endif // OMNITIMEMACHINE_H
