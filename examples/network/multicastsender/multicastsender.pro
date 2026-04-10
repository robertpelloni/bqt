HEADERS       = sender.h
SOURCES       = sender.cpp \
                main.cpp
BOBUI           += network widgets
requires(bobuiConfig(udpsocket))

# install
target.path = $$[BOBUI_INSTALL_EXAMPLES]/network/multicastsender
INSTALLS += target

