HEADERS       = ../connection.h
SOURCES       = tablemodel.cpp
BOBUI           += sql widgets
requires(bobuiConfig(tableview))

# install
target.path = $$[BOBUI_INSTALL_EXAMPLES]/sql/tablemodel
INSTALLS += target

