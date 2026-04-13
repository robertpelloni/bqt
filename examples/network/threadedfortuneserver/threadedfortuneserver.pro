BOBUI += network widgets

HEADERS       = dialog.h \
                fortuneserver.h \
                fortunethread.h
SOURCES       = dialog.cpp \
                fortuneserver.cpp \
                fortunethread.cpp \
                main.cpp

# install
target.path = $$[BOBUI_INSTALL_EXAMPLES]/network/threadedfortuneserver
INSTALLS += target


