TEMPLATE      = subdirs
CONFIG += no_docs_target

SUBDIRS = \
    corelib

!contains(TEMPLATE, "vc.*") {    # BOBUIBUG-91033
    bobuiHaveModule(dbus): SUBDIRS += dbus
}
bobuiHaveModule(network): SUBDIRS += network
bobuiHaveModule(testlib): SUBDIRS += bobuiestlib
bobuiHaveModule(concurrent): SUBDIRS += bobuiconcurrent
bobuiHaveModule(sql): SUBDIRS += sql
bobuiHaveModule(widgets): SUBDIRS += widgets
bobuiHaveModule(xml): SUBDIRS += xml
bobuiHaveModule(gui) {
    SUBDIRS += gui
    bobuiConfig(opengl): SUBDIRS += opengl
    bobuiConfig(vulkan): SUBDIRS += vulkan
}

aggregate.files = aggregate/examples.pro
aggregate.path = $$[BOBUI_INSTALL_EXAMPLES]
readme.files = README
readme.path = $$[BOBUI_INSTALL_EXAMPLES]
INSTALLS += aggregate readme
