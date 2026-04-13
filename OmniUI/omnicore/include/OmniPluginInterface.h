#ifndef OMNIPLUGININTERFACE_H
#define OMNIPLUGININTERFACE_H

#include <BobUIPlugin>
#include <QString>

class QQmlEngine;

/**
 * @brief The OmniPluginInterface class
 * 
 * This is the pure virtual interface that all external C++ OmniUI plugins must implement.
 * It allows developers to compile standalone .dll or .so libraries that dynamically
 * inject new QML components, backend C++ logic, or Audio DSP nodes into the running OS
 * without requiring a full recompile of BobUI.
 */
class OmniPluginInterface {
public:
    virtual ~OmniPluginInterface() = default;

    // The core hook: The OS passes the active QML engine to the plugin
    // so it can call qmlRegisterType<T>() for its custom components.
    virtual void registerQmlTypes(QQmlEngine* engine) = 0;

    // Meta information for the Developer Overlay
    virtual QString pluginName() const = 0;
    virtual QString pluginVersion() const = 0;
};

#define OmniPluginInterface_iid "com.bobui.OmniPluginInterface/1.0"
Q_DECLARE_INTERFACE(OmniPluginInterface, OmniPluginInterface_iid)

#endif // OMNIPLUGININTERFACE_H
