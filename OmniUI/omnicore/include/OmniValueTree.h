#ifndef OMNIVALUETREE_H
#define OMNIVALUETREE_H

#include <QObject>
#include <QString>
#include <QVariantMap>
#include <QMutex>

// Central OS State Tree (JUCE ValueTree Parity)
class OmniValueTree : public QObject {
    Q_OBJECT
public:
    static OmniValueTree* instance();

    // Sets a parameter value and notifies all bound widgets/DSP nodes
    Q_INVOKABLE void setParameter(const QString& key, const QVariant& value, bool broadcast = true);
    
    Q_INVOKABLE QVariant getParameter(const QString& key, const QVariant& fallback = QVariant()) const;

    // Registers a property to be auto-synced with a tree key
    Q_INVOKABLE void bindProperty(QObject* target, const QString& propertyName, const QString& key);

signals:
    void parameterChanged(const QString& key, const QVariant& value);

private:
    explicit OmniValueTree(QObject *parent = nullptr);
    ~OmniValueTree() override;

    QVariantMap m_parameters;
    mutable QMutex m_mutex;
};

#endif // OMNIVALUETREE_H
