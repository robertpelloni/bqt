BOBUI += network widgets
requires(bobuiConfig(udpsocket))

HEADERS       = sender.h
SOURCES       = sender.cpp \
                main.cpp

# install
target.path = $$[BOBUI_INSTALL_EXAMPLES]/network/broadcastsender
INSTALLS += target
