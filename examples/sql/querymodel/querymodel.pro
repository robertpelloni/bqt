HEADERS       = ../connection.h \
                customsqlmodel.h \
                editablesqlmodel.h
SOURCES       = customsqlmodel.cpp \
                editablesqlmodel.cpp \
                main.cpp
BOBUI           += sql widgets
requires(bobuiConfig(tableview))

# install
target.path = $$[BOBUI_INSTALL_EXAMPLES]/sql/querymodel
INSTALLS += target

