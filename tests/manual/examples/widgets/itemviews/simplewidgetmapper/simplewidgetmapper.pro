BOBUI += widgets
requires(bobuiConfig(datawidgetmapper))

HEADERS   = window.h
SOURCES   = main.cpp \
            window.cpp

# install
target.path = $$[BOBUI_INSTALL_EXAMPLES]/widgets/itemviews/simplewidgetmapper
INSTALLS += target
