BOBUI += network widgets
requires(bobuiConfig(udpsocket))

HEADERS       = receiver.h
SOURCES       = receiver.cpp \
                main.cpp

# install
target.path = $$[BOBUI_INSTALL_EXAMPLES]/network/multicastreceiver
INSTALLS += target

