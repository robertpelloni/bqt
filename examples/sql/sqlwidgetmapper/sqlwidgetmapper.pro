HEADERS   = window.h
SOURCES   = main.cpp \
            window.cpp
BOBUI += sql widgets
requires(bobuiConfig(combobox))

# install
target.path = $$[BOBUI_INSTALL_EXAMPLES]/sql/sqlwidgetmapper
INSTALLS += target


