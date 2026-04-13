#! [0]
TEMPLATE      = lib
CONFIG       += plugin static
BOBUI           += widgets
INCLUDEPATH  += ../../app
HEADERS       = basictoolsplugin.h
SOURCES       = basictoolsplugin.cpp
TARGET        = $$bobuiLibraryTarget(pnp_basictools)
DESTDIR       = ../../plugins
#! [0]

# install
target.path = $$[BOBUI_INSTALL_EXAMPLES]/widgets/tools/plugandpaint/plugins
INSTALLS += target

CONFIG += install_ok  # Do not cargo-cult this!
uikit: CONFIG += debug_and_release
