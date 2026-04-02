#include "OmniPluginManager.h"

OmniPluginManager::OmniPluginManager(QObject *parent) : QObject(parent) {
    setObjectName("OmniPluginManager");
    // qDebug() << "Initialized OmniPluginManager";
}

OmniPluginManager::~OmniPluginManager() = default;
