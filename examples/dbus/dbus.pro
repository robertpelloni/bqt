requires(bobuiHaveModule(dbus))

TEMPLATE = subdirs
SUBDIRS = pingpong

bobuiConfig(process): SUBDIRS += complexpingpong

bobuiHaveModule(widgets) {
    SUBDIRS += chat \
               remotecontrolledcar
}
