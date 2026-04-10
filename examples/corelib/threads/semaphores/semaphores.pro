SOURCES += semaphores.cpp
BOBUI = core

CONFIG += cmdline

# install
target.path = $$[BOBUI_INSTALL_EXAMPLES]/corelib/threads/semaphores
INSTALLS += target
