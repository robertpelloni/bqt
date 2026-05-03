#include "OmniPluginManager.h"
#include "OmniPluginInterface.h"
#include <QPluginLoader>
#include <QDir>
#include <QDebug>
#include <QQmlEngine>

OmniPluginManager* OmniPluginManager::instance()
{
    static OmniPluginManager* manager = new OmniPluginManager();
    return manager;
}

OmniPluginManager::OmniPluginManager(QObject *parent)
    : QObject(parent),
      m_qmlEngine(nullptr)
{
    setObjectName("OmniPluginManager");
}

OmniPluginManager::~OmniPluginManager() = default;

void OmniPluginManager::setQmlEngine(QQmlEngine* engine) {
    m_qmlEngine = engine;
}

QStringList OmniPluginManager::loadedPluginNames() const {
    return m_loadedPluginNames;
}

QList<OmniPluginInterface*> OmniPluginManager::loadedPlugins() const {
    return m_loadedPlugins;
}

bool OmniPluginManager::loadPlugin(const QString& absolutePath) {
    if (!m_qmlEngine) {
        qCritical() << "OmniPluginManager: Cannot load plugin, QML Engine is not set!";
        emit pluginLoadFailed(absolutePath, "QML Engine not initialized.");
        return false;
    }

    QPluginLoader loader(absolutePath);
    QObject* pluginInstance = loader.instance();

    if (pluginInstance) {
        // Attempt to cast the loaded QObject to our specific OmniPluginInterface
        auto* omniPlugin = qobject_cast<OmniPluginInterface*>(pluginInstance);
        
        if (omniPlugin) {
            // Success! Allow the plugin to register its custom QML C++ components
            omniPlugin->registerQmlTypes(m_qmlEngine);
            
            m_loadedPlugins.append(omniPlugin);
            
            QString name = omniPlugin->pluginName();
            QString version = omniPlugin->pluginVersion();
            m_loadedPluginNames.append(name + " v" + version);
            
            qDebug() << "OmniPluginManager: Successfully loaded plugin ->" << name << "v" << version;
            
            emit pluginLoaded(name, version);
            emit pluginsChanged();
            return true;
        } else {
            QString err = "Loaded library is not a valid OmniPluginInterface.";
            qWarning() << "OmniPluginManager:" << err << absolutePath;
            loader.unload();
            emit pluginLoadFailed(absolutePath, err);
            return false;
        }
    }

    QString err = loader.errorString();
    qWarning() << "OmniPluginManager: Failed to load plugin instance." << err;
    emit pluginLoadFailed(absolutePath, err);
    return false;
}

int OmniPluginManager::loadPluginsFromDirectory(const QString& directoryPath) {
    QDir dir(directoryPath);
    if (!dir.exists()) {
        qWarning() << "OmniPluginManager: Plugin directory does not exist:" << directoryPath;
        return 0;
    }

    int loadedCount = 0;
    
    // Look for .dll on Windows, .so on Linux, .dylib on Mac
    QStringList filters;
#ifdef Q_OS_WIN
    filters << "*.dll";
#elif defined(Q_OS_MAC)
    filters << "*.dylib";
#else
    filters << "*.so";
#endif

    dir.setNameFilters(filters);
    
    for (const QString& filename : dir.entryList(QDir::Files)) {
        QString absolutePath = dir.absoluteFilePath(filename);
        if (loadPlugin(absolutePath)) {
            loadedCount++;
        }
    }
    
    return loadedCount;
}
