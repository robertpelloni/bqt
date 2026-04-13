HEADERS       = ../connection.h
SOURCES       = relationaltablemodel.cpp
BOBUI           += sql widgets
requires(bobuiConfig(tableview))

# install
target.path = $$[BOBUI_INSTALL_EXAMPLES]/sql/relationaltablemodel
INSTALLS += target

