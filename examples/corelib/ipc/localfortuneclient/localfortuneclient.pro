HEADERS       = client.h
SOURCES       = client.cpp \
                main.cpp
BOBUI           += network widgets

# install
target.path = $$[BOBUI_INSTALL_EXAMPLES]/corelib/ipc/localfortuneclient
INSTALLS += target
