#ifndef OMNIPLUGINMANAGER_H
#define OMNIPLUGINMANAGER_H

#include <QObject>
#include <QString>
#include <QStringList>
#include <QList>

class QQmlEngine;
class OmniPluginInterface;

class OmniPluginManager : public QObject {
    Q_OBJECT
    Q_PROPERTY(QStringList loadedPluginNames READ loadedPluginNames NOTIFY pluginsChanged)

public:
    static OmniPluginManager* instance();

    // Must be called early in the OS lifecycle to establish the QML context
    void setQmlEngine(QQmlEngine* engine);

    // Dynamically loads a .dll or .so at runtime
    Q_INVOKABLE bool loadPlugin(const QString& absolutePath);
    
    // Scans a directory and loads all valid OmniUI plugins
    Q_INVOKABLE int loadPluginsFromDirectory(const QString& directoryPath);

    QStringList loadedPluginNames() const;
    
    // Returns the raw interfaces for internal OS routing (e.g. Developer Overlay)
    QList<OmniPluginInterface*> loadedPlugins() const;

signals:
    void pluginsChanged();
    void pluginLoaded(const QString& name, const QString& version);
    void pluginLoadFailed(const QString& path, const QString& errorString);

private:
    explicit OmniPluginManager(QObject *parent = nullptr);
    ~OmniPluginManager() override;

    QQmlEngine* m_qmlEngine;
    QList<OmniPluginInterface*> m_loadedPlugins;
    QStringList m_loadedPluginNames;
};

#endif // OMNIPLUGINMANAGER_H
