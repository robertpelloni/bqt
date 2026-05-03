BOBUI += network widgets

HEADERS       = server.h
SOURCES       = server.cpp \
                main.cpp

# install
target.path = $$[BOBUI_INSTALL_EXAMPLES]/network/fortuneserver
INSTALLS += target
