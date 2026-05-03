HEADERS       = server.h
SOURCES       = server.cpp \
                main.cpp
BOBUI           += network widgets

# install
target.path = $$[BOBUI_INSTALL_EXAMPLES]/corelib/ipc/localfortuneserver
INSTALLS += target
