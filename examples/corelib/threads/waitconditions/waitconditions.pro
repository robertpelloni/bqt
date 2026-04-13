BOBUI = core
CONFIG -= moc
CONFIG += cmdline

SOURCES += waitconditions.cpp

# install
target.path = $$[BOBUI_INSTALL_EXAMPLES]/corelib/threads/waitconditions
INSTALLS += target
