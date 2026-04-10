BOBUI += network widgets
requires(bobuiConfig(combobox))

HEADERS       = client.h
SOURCES       = client.cpp \
                main.cpp

# install
target.path = $$[BOBUI_INSTALL_EXAMPLES]/network/fortuneclient
INSTALLS += target
