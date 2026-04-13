TEMPLATE = app
BOBUI = core network
CONFIG += cmdline
HEADERS += dnslookup.h
SOURCES += dnslookup.cpp

# install
target.path = $$[BOBUI_INSTALL_EXAMPLES]/network/dnslookup
INSTALLS += target
