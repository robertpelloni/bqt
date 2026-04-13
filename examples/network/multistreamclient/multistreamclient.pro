BOBUI += network widgets

HEADERS       = client.h \
                consumer.h \
                movieconsumer.h \
                timeconsumer.h \
                chatconsumer.h
SOURCES       = client.cpp \
                movieconsumer.cpp \
                timeconsumer.cpp \
                chatconsumer.cpp \
                main.cpp

# install
target.path = $$[BOBUI_INSTALL_EXAMPLES]/network/multistreamclient
INSTALLS += target
