HEADERS       = chatdialog.h \
                client.h \
                connection.h \
                peermanager.h \
                server.h
SOURCES       = chatdialog.cpp \
                client.cpp \
                connection.cpp \
                main.cpp \
                peermanager.cpp \
                server.cpp
FORMS         = chatdialog.ui
BOBUI           += network widgets
DEFINES      += BOBUI_USE_QSTRINGBUILDER
requires(bobuiConfig(udpsocket))
requires(bobuiConfig(listwidget))

# install
target.path = $$[BOBUI_INSTALL_EXAMPLES]/network/network-chat
INSTALLS += target


