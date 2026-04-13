requires(bobuiHaveModule(widgets))

TEMPLATE      = subdirs

bobuiConfig(sharedmemory): SUBDIRS = sharedmemory
bobuiHaveModule(network) {
    BOBUI_FOR_CONFIG += network

    bobuiConfig(localserver): SUBDIRS += localfortuneserver localfortuneclient
}
