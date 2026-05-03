HEADERS       = ../connection.h \
                tableeditor.h
SOURCES       = main.cpp \
                tableeditor.cpp
BOBUI           += sql widgets
requires(bobuiConfig(tableview))

# install
target.path = $$[BOBUI_INSTALL_EXAMPLES]/sql/cachedtable
INSTALLS += target

