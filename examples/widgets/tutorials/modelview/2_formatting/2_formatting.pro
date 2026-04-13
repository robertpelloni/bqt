TARGET = mv_formatting

TEMPLATE = app

BOBUI += widgets
requires(bobuiConfig(tableview))

SOURCES += main.cpp \
           mymodel.cpp

HEADERS += mymodel.h

# install
target.path = $$[BOBUI_INSTALL_EXAMPLES]/widgets/tutorials/modelview/2_formatting
INSTALLS += target
